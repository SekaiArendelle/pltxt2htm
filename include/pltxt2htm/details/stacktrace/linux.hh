#pragma once

#include <cstdlib>
#include <execinfo.h>
#include <memory>
#include "entry.hh"

namespace pltxt2htm::details::stacktrace {
[[nodiscard]]
inline ResolvedFrame resolve_native(void* address) noexcept {
    ResolvedFrame result{.address = address};
    // backtrace_symbols allocates inside libc. Copy before freeing so the data
    // can outlive the backend and can be printed more than once, anywhere.
    auto* symbols = ::backtrace_symbols(::std::addressof(address), 1);
    if (symbols != nullptr) {
        result.text_truncated = ::pltxt2htm::details::stacktrace::copy_text(result.description, symbols[0]);
        ::std::free(symbols);
    }
    return result;
}
} // namespace pltxt2htm::details::stacktrace
