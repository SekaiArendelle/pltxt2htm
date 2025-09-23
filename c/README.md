# Use `pltxt2htm` in Programming language C

This folder supply C's header-source style interface. (build source code to library and include [pltxt2htm.h](./pltxt2htm.h) in a C project)

## build

> Note: building library still requires C++ toolchains

```sh
xmake config -k [static|shared]
xmake build
xmake install -o $PREFIX
```

## Usage
```c
// example.c
#include <stdio.h>
#include <stdlib.h>
#include "pltxt2htm.h"

int main(void) {
    char const* html = advanced_parserd("<a>example</a>", "_");
    printf("%s\n", html);
    free((void*)html);

    return 0;
}
```

compile `example.c`:
```sh
gcc example.c -o example -L ./build/linux/x64/release -lpltxt2htm
./example
```
