## install smoke test

Checks that the header-only library can be consumed through CMake, from two points
of view:

- [`find_package/`](./find_package/) — against an installation
  (`find_package(pltxt2htm CONFIG)`), which also covers the version file;
- [`add_subdirectory/`](./add_subdirectory/) — against the source tree
  (`add_subdirectory()`, the code path `FetchContent` uses).
- [`platform_options/`](./platform_options/) — checks that the installed target
  supplies the Windows/MSVC options in the consumer's context.

Both build [`main.cc`](./main.cc) with the exported `pltxt2htm::pltxt2htm` target.
The program pins the version reported by its build system against
`pltxt2htm::version`, then converts a heading and fails unless the generated HTML
contains it.

```sh
# install the library ...
cmake -S . -B build-install -DCMAKE_INSTALL_PREFIX=/tmp/pltxt2htm
cmake --install build-install

# ... and consume that installation
cmake -S tests/install/find_package -B build-consumer -DCMAKE_PREFIX_PATH=/tmp/pltxt2htm
cmake --build build-consumer
./build-consumer/install_find_package
```

`tests/docker/x86_64-linux-gnu-install/Dockerfile` installs without a compiler,
then runs both consumers in CI. It also deletes the source tree before building
the `find_package` consumer, so an export that accidentally points back into the
source tree fails there.
