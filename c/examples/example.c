#include <stdio.h>
#include <stdlib.h>
#include "../include/pltxt2htm.h"

int main(void) {
    char const* html =
        pltxt2htm_fixedadv_parser("<a>example</a>", "localhost:5173", "$PROJECT", "$VISITOR", "$AUTHOR", "$CO_AUTHORS");
    printf("%s\n", html);
    pltxt2htm_free_u8string(html);

    return 0;
}
