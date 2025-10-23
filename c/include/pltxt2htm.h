/* alias char is char8_t */

#ifndef PLTXT2HTM_H
#define PLTXT2HTM_H // This macro is an exported api (only available in c/)

#include <stddef.h>
#include "push_macros.h"

#if defined(__cplusplus)
extern "C" {
#endif

char const* PLTXT2HTM_C_NAME_MANGLING(common_parser)(char const* text)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

char const* PLTXT2HTM_C_NAME_MANGLING(advanced_parser)(char const* text, char const* host)
#if __cpp_noexcept_function_type >= 201510L
    noexcept
#endif
    ;

char const* PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)(char const* text, char const* host)
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
