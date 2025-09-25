/**
 * @brief Macro defined by pltxt2htm
 * @note This header may be included multiple times with pop_macro.hh,
 *       therefore, do not `#pragma once`
 */

#if __cpp_exceptions >= 199711L
    #include "panic.hh"
#endif

#pragma push_macro("pltxt2htm_assert")
#undef pltxt2htm_assert
/**
 * @brief Assert whether the condition expression is true, if not, print
 *         the message and terminate the program.
 */
#if __cpp_exceptions >= 199711L
    #define pltxt2htm_assert(condition, message) \
        do { \
            if constexpr (ndebug == false) { \
                if ((condition) == false) [[unlikely]] { \
                    ::pltxt2htm::details::panic_print(::std::source_location::current(), #condition, message); \
                    ::exception::terminate(); \
                } \
            } \
        } while (0)
#else
    #define pltxt2htm_assert(condition, _) \
        do { \
            if constexpr (ndebug == false) { \
                if ((condition) == false) [[unlikely]] { \
                    ::exception::terminate(); \
                } \
            } \
        } while (0)
#endif
