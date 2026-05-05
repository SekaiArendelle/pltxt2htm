#include <stdio.h>
#include <stdlib.h>
#include "../include/pltxt2htm.h"

int main(void) {
    char const* html = PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)("<a>example</a>", "localhost:5173", "$PROJECT", "$VISITOR", "$AUTHOR", "$CO_AUTHORS");
    printf("%s\n", html);
    free((void*)html);

    return 0;
}
