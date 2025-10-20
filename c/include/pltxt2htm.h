/* alias char is char8_t */

#ifndef PLTXT2HTM_H
#define PLTXT2HTM_H // This macro is an exported api (only available in c/)

#include <stddef.h>
#include "push_macros.h"

#if defined(__cplusplus)
extern "C" {
#endif

char const* PLTXT2HTM_C_NAME_MANGLING(common_parser)(char const* text) /* throws */;
char const* PLTXT2HTM_C_NAME_MANGLING(advanced_parser)(char const* text, char const* host) /* throws */;
char const* PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)(char const* text, char const* host) /* throws */;

size_t get_version_major(void) PLTXT2HTM_C_NOEXCEPT;
size_t get_version_minor(void) PLTXT2HTM_C_NOEXCEPT;
size_t get_version_patch(void) PLTXT2HTM_C_NOEXCEPT;

#if defined(__cplusplus)
}
#endif

#include "pop_macros.h"

#endif // PLTXT2HTM_H
