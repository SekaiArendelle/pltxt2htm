/**
 * @file contracts.hh
 * @brief Compile-time contract checking configuration.
 */
#pragma once

#ifndef PLTXT2HTM_CONTRACTS
    #define PLTXT2HTM_CONTRACTS 0
#endif
#if PLTXT2HTM_CONTRACTS != 0 && PLTXT2HTM_CONTRACTS != 1
    #error "PLTXT2HTM_CONTRACTS must be 0 (enforce) or 1 (ignore)"
#endif
namespace pltxt2htm {

constexpr auto contract_mode = PLTXT2HTM_CONTRACTS;

} // namespace pltxt2htm
