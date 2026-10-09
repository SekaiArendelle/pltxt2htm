#pragma once

#include <charconv>
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <memory>
#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif
#include "entry.hh"
#include "linux_dw.hh"

namespace pltxt2htm::details::stacktrace {
/** Drop ordinary argument lists, retaining operator() and template arguments.
 * Preserve complex return declarators rather than stripping their type syntax.
 */
constexpr void remove_symbol_parameters(::std::span<char> name) noexcept {
    auto const text = ::pltxt2htm::details::stacktrace::text_view(name);
    ::std::size_t end = text.size();
    while (end != 0 && text[end - 1] != ')') {
        --end;
    }
    if (end == 0) {
        return;
    }
    ::std::size_t depth{};
    for (auto i = end; i != 0;) {
        --i;
        if (text[i] == ')') {
            ++depth;
        }
        else if (text[i] == '(' && --depth == 0) {
            if (i != 0 && text[i - 1] == ')') {
                // A function returning a function pointer, such as
                // void (*make_callback<int>())(double), has its return type's
                // arguments here. It is not the function parameter list.
                ::std::size_t return_depth{};
                for (auto j = i; j != 0;) {
                    --j;
                    if (text[j] == ')') {
                        ++return_depth;
                    }
                    else if (text[j] == '(' && --return_depth == 0) {
                        if (text[j + 1] == '*' || text[j + 1] == '&') {
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

inline void copy_function_name(ResolvedFrame& result, char const* name) noexcept {
#if __has_include(<cxxabi.h>)
    char* decoded{};
    int status{};
    if (name[0] == '_' && name[1] == 'Z') {
        decoded = ::abi::__cxa_demangle(name, nullptr, nullptr, ::std::addressof(status));
    }
    if (decoded != nullptr && status == 0) {
        // Strip before bounded copying to retain the signature boundary.
        ::pltxt2htm::details::stacktrace::remove_symbol_parameters({decoded, ::std::strlen(decoded)});
        name = decoded;
    }
#endif
    result.text_truncated =
        ::pltxt2htm::details::stacktrace::copy_text(result.description, name) || result.text_truncated;
#if __has_include(<cxxabi.h>)
    ::std::free(decoded);
#endif
}

/** Preserve libc's module path even when no function symbol is available. */
inline void resolve_symbol_text(ResolvedFrame& result, char* symbol) noexcept {
    if (symbol == nullptr) {
        return;
    }
    auto* begin = ::std::strrchr(symbol, '(');
    if (begin == nullptr) {
        auto* address = ::std::strrchr(symbol, '[');
        if (address != nullptr && address != symbol && address[-1] == ' ') {
            address[-1] = '\0';
            result.text_truncated =
                ::pltxt2htm::details::stacktrace::copy_text(result.module_file, symbol) || result.text_truncated;
            address[-1] = ' ';
        }
        return;
    }
    *begin = '\0';
    result.text_truncated =
        ::pltxt2htm::details::stacktrace::copy_text(result.module_file, symbol) || result.text_truncated;
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

inline void resolve_dwarf(ResolvedFrame& result, LibdwApi& api) noexcept {
    if (result.address == nullptr) {
        return;
    }
    DwflSession session{api};
    if (session.handle == nullptr) {
        return;
    }
    auto const address = static_cast<Elf64_Addr>(reinterpret_cast<::std::uintptr_t>(result.address));
    auto* module = api.addr_module(session.handle, address);
    if (module == nullptr) {
        return;
    }
    Elf64_Off displacement{};
    Elf64_Sym symbol{};
    auto const* name = api.addr_info(module, address, ::std::addressof(displacement), ::std::addressof(symbol), nullptr,
                                     nullptr, nullptr);
    if (name != nullptr) {
        ::pltxt2htm::details::stacktrace::copy_function_name(result, name);
        result.displacement = displacement;
    }
    char const* main_file{};
    auto const* module_name =
        api.module_info(module, nullptr, nullptr, nullptr, nullptr, nullptr, ::std::addressof(main_file), nullptr);
    result.text_truncated = ::pltxt2htm::details::stacktrace::copy_text(
                                result.module_file, main_file != nullptr ? main_file : module_name) ||
                            result.text_truncated;
    auto* source = api.get_source(module, address);
    if (source != nullptr) {
        int line{};
        auto const* file = api.line_info(source, nullptr, ::std::addressof(line), nullptr, nullptr, nullptr);
        ::pltxt2htm::details::stacktrace::copy_source_location(result, file, api.comp_dir(source), line);
    }
}

[[nodiscard]]
inline ResolvedFrame resolve_native(void* address) noexcept {
    ResolvedFrame result{.address = address};
    if (address == nullptr) {
        return result;
    }
    // Copy before freeing so saved frames own all their symbol data.
    auto* symbols = ::backtrace_symbols(::std::addressof(address), 1);
    if (symbols != nullptr) {
        ::pltxt2htm::details::stacktrace::resolve_symbol_text(result, symbols[0]);
        ::std::free(symbols);
    }
    LibdwApi api{};
    ::pltxt2htm::details::stacktrace::resolve_dwarf(result, api);
    return result;
}
} // namespace pltxt2htm::details::stacktrace
