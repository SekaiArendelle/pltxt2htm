#pragma once

#if defined(__clang__)
    #pragma clang attribute push(__attribute__((no_sanitize("memory"))), apply_to = any(function))
#endif
#include <doctest/doctest.h>
#if defined(__clang__)
    #pragma clang attribute pop
#endif

#include "precompile.hh"
