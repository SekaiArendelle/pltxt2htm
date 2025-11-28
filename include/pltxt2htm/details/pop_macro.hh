/**
 * @file pop_macro.hh
 * @brief Macro cleanup for pltxt2htm assertions and debugging
 * @details This header restores the previous state of macros that were
 *          modified by push_macro.hh. It ensures proper macro hygiene
 *          and prevents macro pollution in other parts of the code.
 * @note This header may be included multiple times with push_macro.hh,
 *       therefore, do not use `#pragma once`
 * @warning Always include this file after push_macro.hh to restore macro state
 */

#pragma pop_macro("pltxt2htm_assert")
