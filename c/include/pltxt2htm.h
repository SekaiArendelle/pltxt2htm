/* alias char is char8_t */

#ifndef PLTXT2HTM_H
#define PLTXT2HTM_H // This macro is an exported api (only available in c/)

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

char const* pltxt2htm_common_parser(char const* text)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

char const* pltxt2htm_fixedadv_parser(char const* const pltext, char const* const host, char const* const project,
                                      char const* const visitor, char const* const author, char const* const coauthors)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

char const* pltxt2htm_plrichtext_parser(char const* const text, char const* const project, char const* const visitor,
                                        char const* const author, char const* const coauthors)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

void pltxt2htm_free_u8string(char const* str)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

size_t pltxt2htm_version_major(void)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

size_t pltxt2htm_version_minor(void)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

size_t pltxt2htm_version_patch(void)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

#if defined(__cplusplus)
}
#endif

#endif // PLTXT2HTM_H
