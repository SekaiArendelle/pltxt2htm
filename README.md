# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web](https://github.com/wsxiaolin/physics-lab-web)

This project aimed to provide a translater that compiles the Quantam-PhysicsLab's text to html (with markdown, minor html tags and latex support [as much as possible](#markdown-extensions-for-quantum-physicss-text)).

This project requires at least C++23, disables rtti and exceptions, use header-only style to supply api, has great cross platform ability and I strongly recommend you to use clang.

# How to use?
You can use `pltxt2htm` in [cmd line](cmd/README.md), [js](js/README.md) and [python]().

C++ APIs is in [include/pltxt2htm](include/pltxt2htm/), here is a simple example:
```cpp
// example.cc
#include <fast_io/fast_io.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html = pltxt2htm::pltxt2html(R"(
# Hello Quantum PhysicsLab
With Markdown supports
)");
    ::fast_io::io::println(html);

    return 0;
}
```

Compile it: (Windows)
```sh
clang++ example.cc -o example -std=c++23 -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -I include -lntdll
```

Other platforms:
```sh
clang++ example.cc -o example -std=c++23 -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -I include
```

And I strongly suggest you to add `-O2`, `-fno-ident`, `-DNDEBUG`, `-fuse-ld=lld`, `-flto=thin`, `-stdlib=libc++`, `-rtlib=compiler-rt`, `--unwindlib=libunwind` and cross compiling flags(`--target=$TRIPLET` and `--sysroot=$YOUR_SYSROOT_PATH`) to your clang.

## run tests
```sh
cd test
python ./run_tests.py
```

## markdown extensions for Quantum Physics's text
I was ensure any new extensions will break old Quantum Physics's text. However, compatibility is not the reason that we should stop our steps. Here are some extensions why I (not) support:

<!-- 1. Atx style headers must be at the beginning of a line
2. Setext style headers are not supported
3. It is not necessary to add 2 spaces before entering the carriage to be considered a line break
4. Indented code blocks are not supported -->

## others

### Q: Is this the time to use C++20 module?
A: Not exactly. Despite clang, gcc and msvc all support C++20 modules, but the compiler crashes more frequently than hearder-only. At the same time, Header unit is not fullly supported.

### Q: Why not use NDEBUG macro in include/pltxt2htm
A: Conditional compilation in function body will cause [ODR against](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same prolem. therefore, to make debug / release mode has different symbols, I use `template<bool ndebug>` to achieve it.
