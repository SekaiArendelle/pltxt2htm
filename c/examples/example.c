#include <stdio.h>
#include <stdlib.h>
#include "../include/pltxt2htm.h"

int main(void) {
    char const* html = PLTXT2HTM_C_NAME_MANGLING(advanced_parser)("<a>example</a>", "_");
    printf("%s\n", html);
    free((void*)html);

    return 0;
}
