/**
 * @file contract.hh
 * @brief Contract failure handling for stacktrace internals.
 * @details The stacktrace subtree sits below the panic path and cannot include
 *          the assert machinery that prints diagnostics. Violations trap in
 *          enforce and quick_enforce builds and become optimizer assumptions in
 *          ignore builds, matching the pltxt2htm_assert failure policy.
 */

#pragma once

#include "../../details/contracts.hh"
#include "../../details/trap.hh"

namespace pltxt2htm::stacktrace::details {
[[noreturn]]
inline void contract_violation() noexcept {
    if constexpr (::pltxt2htm::details::contract_mode == ::pltxt2htm::details::Contracts::ignore) {
        ::pltxt2htm::details::unreachable();
    }
    else {
        ::pltxt2htm::details::trap();
    }
}
} // namespace pltxt2htm::stacktrace::details
