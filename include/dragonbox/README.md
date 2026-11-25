# Dragonbox (vendored)

This directory contains an unmodified copy of the single-header Dragonbox
implementation by Junekey Jeon.

- Upstream: <https://github.com/jk-jeon/dragonbox>
- Revision: `beeeef91cf6fef89a4d4ba5e95d47ca64ccb3a44` (`1.1.3-248-gbeeeef9`)
- Files: `dragonbox.h` (copied verbatim from the upstream
  `include/dragonbox/dragonbox.h`), `LICENSE-Apache2-LLVM`, `LICENSE-Boost`

Dragonbox computes the shortest decimal significand and exponent such that a
correctly rounding parser recovers the original binary floating-point value. It
is included by `include/pltxt2htm/format/floating.hh`, which selects the private
namespace and the cache policy before including it; include that header rather
than `dragonbox.h` directly so the configuration is applied exactly once.

Do not edit `dragonbox.h` and do not reformat it: updates should replace the
file with a newer upstream revision so that the diff stays reviewable.

## License

Dragonbox is dual licensed under the Apache License v2.0 with LLVM Exceptions or
the Boost Software License v1.0; see `LICENSE-Apache2-LLVM` and `LICENSE-Boost`.
