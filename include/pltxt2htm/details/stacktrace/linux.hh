#pragma once

#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <memory>
#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif
#include "entry.hh"

namespace pltxt2htm::details::stacktrace {
/** Decode only the symbol component of libc's module(symbol+offset) text.
 * The writable libc buffer is restored before returning, including on failure.
 */
[[nodiscard]]
inline bool copy_symbol_text(::std::span<char> destination, char* symbol) noexcept {
    if (symbol == nullptr || destination.empty()) {
        return false;
    }
#if __has_include(<cxxabi.h>)
    auto* begin = ::std::strrchr(symbol, '(');
    auto* end = begin != nullptr ? ::std::strchr(begin + 1, '+') : nullptr;
    if (end != nullptr && begin[1] == '_' && begin[2] == 'Z') {
        auto const saved = *end;
        *end = '\0';
        int status{};
        auto* decoded = ::abi::__cxa_demangle(begin + 1, nullptr, nullptr, ::std::addressof(status));
        *end = saved;
        if (status == 0 && decoded != nullptr) {
            auto const prefix_size = static_cast<::std::size_t>(begin + 1 - symbol);
            ::std::size_t used{};
            auto const capacity = destination.size() - 1;
            while (used < prefix_size && used < capacity) {
                destination[used] = symbol[used];
                ++used;
            }
            destination[used] = '\0';
            auto truncated = used != prefix_size;
            if (!truncated) {
                truncated = ::pltxt2htm::details::stacktrace::copy_text(destination.subspan(used), decoded);
                used += ::std::strlen(destination.data() + used);
                if (!truncated) {
                    truncated = ::pltxt2htm::details::stacktrace::copy_text(destination.subspan(used), end);
                }
            }
            ::std::free(decoded);
            return truncated;
        }
        ::std::free(decoded);
    }
#endif
    return ::pltxt2htm::details::stacktrace::copy_text(destination, symbol);
}

[[nodiscard]]
inline ResolvedFrame resolve_native(void* address) noexcept {
    ResolvedFrame result{.address = address};
    // backtrace_symbols allocates inside libc. Copy before freeing so the data
    // can outlive the backend and can be printed more than once, anywhere.
    auto* symbols = ::backtrace_symbols(::std::addressof(address), 1);
    if (symbols != nullptr) {
        result.text_truncated = ::pltxt2htm::details::stacktrace::copy_symbol_text(result.description, symbols[0]);
        ::std::free(symbols);
    }
    return result;
}
} // namespace pltxt2htm::details::stacktrace
