/**
 * @file contracts.hh
 * @brief Compile-time contract checking configuration.
 * @details Defines the internal `Contracts` enum and the build-wide
 *          `contract_mode` constant selected by the `PLTXT2HTM_CONTRACTS` macro.
 */
#pragma once

namespace pltxt2htm::details {

/**
 * @brief Contract checking mode.
 * @details `enforce` turns violations into panics; `ignore` replaces the checks
 *          with optimizer assumptions, so a violation is undefined behavior.
 */
enum class Contracts : unsigned {
    enforce = 0, ///< Enforce runtime checks and panic on violation.
    ignore, ///< Replace checks with optimizer assumptions.
};

/**
 * @brief Contract checking mode for the current build.
 * @details `PLTXT2HTM_CONTRACTS` is mapped to this constant: `0` selects
 *          `Contracts::enforce`, `1` selects `Contracts::ignore`. This is an
 *          implementation detail; the macro is the only configuration entry.
 * @note The value is build-wide: every translation unit of a program must be
 *       compiled with the same `PLTXT2HTM_CONTRACTS` value.
 */
inline constexpr Contracts contract_mode{
#if PLTXT2HTM_CONTRACTS == 0
    Contracts::enforce
#elif PLTXT2HTM_CONTRACTS == 1
    Contracts::ignore
#else
    #error "PLTXT2HTM_CONTRACTS must be 0 (enforce) or 1 (ignore)"
#endif
};

} // namespace pltxt2htm::details
