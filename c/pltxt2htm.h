/* alias char = char8_t */

#ifndef PLTXT2HTM_H
#define PLTXT2HTM_H

#if defined(__cplusplus)
extern "C"
#endif
    char const* common_parser(char const* text)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#if defined(__cplusplus)
extern "C"
#endif
    char const* common_parserd(char const* text)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#if defined(__cplusplus)
extern "C"
#endif
    char const* advanced_parser(char const* text, char const* host)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#if defined(__cplusplus)
extern "C"
#endif
    char const* advanced_parserd(char const* text, char const* host)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#if defined(__cplusplus)
extern "C"
#endif
    char const* fixedadv_parser(char const* text, char const* host)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#if defined(__cplusplus)
extern "C"
#endif
    char const* fixedadv_parserd(char const* text, char const* host)
#if __cpp_noexcept_function_type >= 201501L
        noexcept
#endif
    ;

#endif
