/**
 * @file contracts.hh
 * @brief Contract checking mode for compile-time assertion selection.
 * @details Defines the `Contracts` enum used throughout pltxt2htm to select
 *          between enforced and ignored contract checks at compile time.
 */
#pragma once

namespace pltxt2htm {

/**
 * @brief Contract checking mode.
 * @details `quick_enforce` performs runtime assertions; `ignore` elides them.
 */
enum class Contracts : unsigned {
    quick_enforce = 0, ///< Perform runtime assertion checks.
    ignore, ///< Skip all runtime assertion checks.
};

} // namespace pltxt2htm
