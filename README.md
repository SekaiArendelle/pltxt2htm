# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web](https://github.com/wsxiaolin/physics-lab-web)

This project aimed to provide a translater that compiles the Quantam-PhysicsLab's text to html (with markdown, minor html tags and latex support).

This project requires at least C++23, and I strongly recommend you to use clang.

This project disables rtti and exceptions, use header-only style to supply api and has strong cross-platform support.

# How to use?
You can build `pltxt2htm` to [wasm](js/README.md) and [python c extensions]() by using [xmake](https://xmake.io/#/getting_started)

For cpp users, you should just include [pltxt2htm.hh](include/pltxt2htm/pltxt2htm.hh)
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

And I strongly suggest you to add `-O2`, `-fno-ident`, `-DNDEBUG`, `-fuse-ld=lld`, `-flto=thin`, `-stdlib=libc++`, `-rtlib=compiler-rt`, `--unwindlib=libunwind` and cross compiling flags(`--target=$TRIPLET` and `--sysroot=$YOUR_SYSROOT_PATH`) to your compiler.

## run tests
```sh
cd test
xmake test
```

## markdown extension
There are some differents between the *origin markdown* and the *markdown extension* in this project, which are listed below:
1. Atx style headers must be at the beginning of a line
2. Setext style headers are not supported
3. It is not necessary to add 2 spaces before entering the carriage to be considered a line break
4. Indented code blocks are not supported

## others

### Q: Is this the time to use C++20 module?
A: Not exactly. Despite clang, gcc and msvc all support C++20 modules, but the compiler crashes more frequently than hearder-only. At the same time, Header unit is not fullly supported.
