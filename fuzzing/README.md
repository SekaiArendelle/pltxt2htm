Clang Fuzzer for `pltxt2htm::advanced_parser` and `pltxt2htm::common_parser`

## Compile it
chdir to fuzzing/

advanced_parser.cc
```sh
mkdir build
clang++ -g ./advanced_parser.cc -O0 -o build/advanced_parser -std=c++23 -I ../include -fsanitize=fuzzer,undefined -fuse-ld=lld -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -fno-omit-frame-pointer -fno-exceptions -fno-rtti
```
