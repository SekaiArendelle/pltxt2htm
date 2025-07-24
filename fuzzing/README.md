Clang Fuzzer for `pltxt2htm::pltxt2advanced_html` and `pltxt2htm::pltxt2common_html`

## Build
chdir to fuzzing/

```sh
xmake config --toolchain=clang --policies=build.sanitizer.address
# Asan costs a lot, and you can switch to other sanitizers
# xmake config --toolchain=clang --policies=build.sanitizer.undefined
xmake build
```
