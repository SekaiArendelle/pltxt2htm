#pragma once

#if defined(__wasm__)
    #error "pltxt2htm can't print any info when target triplet is wasm"
#endif

#include <stacktrace>
#include <fast_io/fast_io.h>

namespace pltxt2htm::details {

constexpr void panic_print() {
}

} // namespace pltxt2htm::details
