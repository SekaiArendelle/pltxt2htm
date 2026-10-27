## Use pltxt2htm in C++20 module

Requires: clang++ >= 22, g++ >= 16

## Tests

`cxxmodule/tests/check_public_symbols` compares, via reflection, the public symbol surface visible through the classic header (`#include <pltxt2htm/pltxt2htm.hh>`) and through the module (`import pltxt2htm;`). The two views must be identical after excluding the namespaces that are not part of the module surface on both sides: the private `pltxt2htm::details` namespace and the `pltxt2htm::sys_*` platform namespaces (`sys_nt` / `sys_win32` / `sys_linux`), which remain plain headers usable alongside `import pltxt2htm;`.

From the repository root:

```sh
pixi run -e gcc test-cxxmodule
```

## Build and run example.cc

From the repository root:

```sh
pixi run -e clang run-cxxmodule-example
```
