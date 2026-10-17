#pragma once

#include <charconv>
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <memory>
#include <span>
#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif
#include "native_frame.hh"
#if defined(PLTXT2HTM_DETAIL_STACKTRACE_HAS_LIBDWFL)
    #include "linux_dw.hh"
#endif

namespace pltxt2htm::details::stacktrace {
/** Drop ordinary argument lists, retaining operator() and template arguments.
 * Preserve complex return declarators rather than stripping their type syntax.
 */
constexpr void remove_symbol_parameters(::std::span<char> name) noexcept {
    ::std::size_t end = name.size();
    while (end != 0 && name[end - 1] != ')') {
        --end;
    }
    if (end == 0) {
        return;
    }
    ::std::size_t depth{};
    for (auto i = end; i != 0;) {
        --i;
        if (name[i] == ')') {
            ++depth;
        }
        else if (name[i] == '(' && --depth == 0) {
            if (i != 0 && name[i - 1] == ')') {
                // A function returning a function pointer, such as
                // void (*make_callback<int>())(double), has its return type's
                // arguments here. It is not the function parameter list.
                ::std::size_t return_depth{};
                for (auto j = i; j != 0;) {
                    --j;
                    if (name[j] == ')') {
                        ++return_depth;
                    }
                    else if (name[j] == '(' && --return_depth == 0) {
                        if (name[j + 1] == '*' || name[j + 1] == '&') {
                            return;
                        }
                        break;
                    }
                }
            }
            name[i] = '\0';
            return;
        }
    }
}

inline void copy_function_name(NativeResolvedFrame& result, char const* name) noexcept {
#if __has_include(<cxxabi.h>)
    char* decoded{};
    int status{};
    if (name[0] == '_' && name[1] == 'Z') {
        decoded = ::abi::__cxa_demangle(name, nullptr, nullptr, ::std::addressof(status));
    }
    if (decoded != nullptr && status == 0) {
        // Strip before copying so the stored text omits the parameter list.
        ::pltxt2htm::details::stacktrace::remove_symbol_parameters({decoded, ::std::strlen(decoded)});
        name = decoded;
    }
#endif
    result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(result.description, name) ||
                            result.text_truncated;
#if __has_include(<cxxabi.h>)
    ::std::free(decoded);
#endif
}

/** Preserve libc's module path even when no function symbol is available. */
inline void resolve_symbol_text(NativeResolvedFrame& result, char* symbol) noexcept {
    if (symbol == nullptr) {
        return;
    }
    auto* begin = ::std::strrchr(symbol, '(');
    if (begin == nullptr) {
        auto* address = ::std::strrchr(symbol, '[');
        if (address != nullptr && address != symbol && address[-1] == ' ') {
            address[-1] = '\0';
            result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(result.module_file, symbol) ||
                                    result.text_truncated;
            address[-1] = ' ';
        }
        return;
    }
    *begin = '\0';
    result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(result.module_file, symbol) ||
                            result.text_truncated;
    *begin = '(';
    auto* end = ::std::strchr(begin + 1, '+');
    auto* close = end != nullptr ? ::std::strchr(end + 1, ')') : nullptr;
    if (end == nullptr || close == nullptr || end == begin + 1) {
        return;
    }
    auto const* offset = end + 1;
    if (close - offset < 3 || offset[0] != '0' || offset[1] != 'x') {
        return;
    }
    ::std::uint64_t displacement{};
    auto const parsed = ::std::from_chars(offset + 2, close, displacement, 16);
    if (parsed.ec != ::std::errc{} || parsed.ptr != close) {
        return;
    }
    *end = '\0';
    ::pltxt2htm::details::stacktrace::copy_function_name(result, begin + 1);
    *end = '+';
    result.displacement = displacement;
}

#if defined(PLTXT2HTM_DETAIL_STACKTRACE_HAS_LIBDWFL)
inline void resolve_dwarf(NativeResolvedFrame& result) noexcept {
    if (result.address == nullptr) {
        return;
    }
    DwflSession session{};
    if (session.handle == nullptr) {
        return;
    }
    auto const address = static_cast<GElf_Addr>(reinterpret_cast<::std::uintptr_t>(result.address));
    auto* module = ::dwfl_addrmodule(session.handle, address);
    if (module == nullptr) {
        return;
    }
    GElf_Off displacement{};
    GElf_Sym symbol{};
    auto const* name = ::dwfl_module_addrinfo(module, address, ::std::addressof(displacement), ::std::addressof(symbol),
                                              nullptr, nullptr, nullptr);
    if (name != nullptr) {
        ::pltxt2htm::details::stacktrace::copy_function_name(result, name);
        result.displacement = displacement;
    }
    char const* main_file{};
    auto const* module_name =
        ::dwfl_module_info(module, nullptr, nullptr, nullptr, nullptr, nullptr, ::std::addressof(main_file), nullptr);
    result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(
                                result.module_file, main_file != nullptr ? main_file : module_name) ||
                            result.text_truncated;
    auto* source = ::dwfl_module_getsrc(module, address);
    if (source != nullptr) {
        int line{};
        auto const* file = ::dwfl_lineinfo(source, nullptr, ::std::addressof(line), nullptr, nullptr, nullptr);
        ::pltxt2htm::details::stacktrace::assign_source_location(result, file, ::dwfl_line_comp_dir(source), line);
    }
}
#endif

[[nodiscard]]
inline NativeResolvedFrame resolve_native(void* address) noexcept {
    NativeResolvedFrame result{.address = address};
    if (address == nullptr) {
        return result;
    }
    // Copy before freeing so saved frames own all their symbol data.
    auto* symbols = ::backtrace_symbols(::std::addressof(address), 1);
    if (symbols != nullptr) {
        ::pltxt2htm::details::stacktrace::resolve_symbol_text(result, symbols[0]);
        ::std::free(symbols);
    }
#if defined(PLTXT2HTM_DETAIL_STACKTRACE_HAS_LIBDWFL)
    ::pltxt2htm::details::stacktrace::resolve_dwarf(result);
#endif
    return result;
}
} // namespace pltxt2htm::details::stacktrace
