/**
 * @file contracts.hh
 * @brief Compile-time contract checking configuration.
 * @details Defines the build-wide `contract_mode` constant selected by the
 *          `PLTXT2HTM_CONTRACTS` macro.
 */
#pragma once

#ifndef PLTXT2HTM_CONTRACTS
    #define PLTXT2HTM_CONTRACTS 0
#endif
#if PLTXT2HTM_CONTRACTS != 0 && PLTXT2HTM_CONTRACTS != 1
    #error "PLTXT2HTM_CONTRACTS must be 0 (enforce) or 1 (ignore)"
#endif
namespace pltxt2htm {

/**
 * @brief Contract checking mode for the current build.
 * @details `0` turns contract violations into panics; `1` replaces the checks
 *          with optimizer assumptions, so a violation is undefined behavior.
 *          It is a public constant so callers and the C++ module can inspect it.
 * @note The value is build-wide: every translation unit of a program must be
 *       compiled with the same `PLTXT2HTM_CONTRACTS` value.
 */
inline constexpr auto contract_mode = PLTXT2HTM_CONTRACTS;

} // namespace pltxt2htm
