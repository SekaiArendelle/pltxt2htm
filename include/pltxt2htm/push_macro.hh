/**
 * @brief Macro defined by pltxt2htm
 * @note This header may be included multiple times with pop_macro.hh,
 *       therefore, do not `#pragma once`
 */

#pragma push_macro("pltxt2htm_assert")
#undef pltxt2htm_assert
/**
 * @brief Assert whether the condition expression is true, if not, print
 *         the message and terminate the program.
 */
#define pltxt2htm_assert(condition, message) \
    do { \
        if constexpr (ndebug == false || disable_log == false) { \
            if ((condition) == false) [[unlikely]] { \
                if constexpr (disable_log == false) { \
                } \
                if constexpr (ndebug == false) { \
                    ::exception::terminate(); \
                } \
            } \
        } \
    } while (0)
