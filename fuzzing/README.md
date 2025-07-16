Clang Fuzzer for `pltxt2htm::pltxt2advanced_html` and `pltxt2htm::pltxt2common_html`

## Compile it
chdir to fuzzing/

advanced_parser.cc
```sh
mkdir build
clang++ -g ./advanced_parser.cc -O0 -o build/advanced_parser -std=c++23 -I ../include -fsanitize=fuzzer,undefined -fuse-ld=lld -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -fno-omit-frame-pointer -fno-exceptions -fno-rtti
```
