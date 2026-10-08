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

/** Extract libc's symbol and displacement into the common frame representation.
 * Unrecognized text or an unnamed frame retains only the supplied address.
 * The writable libc buffer is restored before returning.
 */
inline void resolve_symbol_text(ResolvedFrame& result, char* symbol) noexcept {
    if (symbol == nullptr) {
        return;
    }
    auto* begin = ::std::strrchr(symbol, '(');
    auto* end = begin != nullptr ? ::std::strchr(begin + 1, '+') : nullptr;
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
    auto const saved = *end;
    *end = '\0';
    auto const* name = begin + 1;
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
    result.text_truncated = ::pltxt2htm::details::stacktrace::copy_text(result.description, name);
    result.displacement = displacement;
#if __has_include(<cxxabi.h>)
    ::std::free(decoded);
#endif
    *end = saved;
}

[[nodiscard]]
inline ResolvedFrame resolve_native(void* address) noexcept {
    ResolvedFrame result{.address = address};
    // Copy before freeing so saved frames own all their symbol data.
    auto* symbols = ::backtrace_symbols(::std::addressof(address), 1);
    if (symbols != nullptr) {
        ::pltxt2htm::details::stacktrace::resolve_symbol_text(result, symbols[0]);
        ::std::free(symbols);
    }
    return result;
}
} // namespace pltxt2htm::details::stacktrace
