# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web](https://github.com/NetLogo-Mobile/Physics-Lab-Web)

This project is **header-only**, aimed to provide a translater from Quantam-PhysicsLab's text to html (with markdown, minor html tags and latex support [as much as possible](#features)). Requires at least C++23 (g++ >= 14 or clang++ >= 20), disables C++ rtti and exception, has great cross platform ability.

# Usage
You can use `pltxt2htm` in [console](cmd/README.md), [browser](wasm/README.md), [python](py/README.md) and [c](c/README.md).

C++ APIs is in [include/pltxt2htm](include/pltxt2htm/), here is a simple example:
```cpp
// example.cc
#include <fast_io/fast_io.h> // in include/fast_io
#include <pltxt2htm/pltxt2htm.hh> // in include/pltxt2htm

int main() {
    auto html = ::pltxt2htm::pltxt2advanced_html(u8R"(
# Hello Quantum PhysicsLab
With Markdown supports
)", u8"localhost:5173");
    ::fast_io::io::println(::fast_io::u8c_stdout(), html);

    return 0;
}
```

Compile it: (Windows)
```sh
clang++ example.cc -o example -std=c++23 -I include -lntdll
```

Other platforms:
```sh
clang++ example.cc -o example -std=c++23 -I include
```

flag `-O2`, `-fno-ident`, `-fno-exceptions`, `-fno-rtti`, `-fno-unwind-tables`, `-fno-asynchronous-unwind-tables`, `-DNDEBUG` is recommended in release mode.

C++20 module example is in [module/example.cc](./cxxmodule/example.cc)

## Exported API
All C++ APIs have been listed in [pltxt2htm.cppm](./cxxmodule/pltxt2htm/pltxt2htm.cppm)

## features
I am ensure any new features (like markdown extension) will break old Quantum Physics's text. However, compatibility is not the reason that we should stop our steps. Here are some features why I (or why I not) support:

markdown extension based on [commonmark](https://spec.commonmark.org/0.31.2/)

* carriage return will be regarded as line break (unlike markdown, which requires 2 spaces before a carriage return to be considered a line break)
* Quantum-Physics's color tag is supported
* Quantum-Physics's a tag is supported
* Quantum-Physics's Discussion tag is supported
* Quantum-Physics's Experiment tag is supported
* Quantum-Physics's user tag is supported
* Quantum-Physics's size tag is supported
* Quantum-Physics&HTML's i tag is supported
* Quantum-Physics&HTML's b tag is supported
* Partial supports of [HTML blocks](https://spec.commonmark.org/0.31.2/#html-blocks)
* most of the markdown ATX headers are supported
  - [example 69 ~ 76](https://spec.commonmark.org/0.31.2/#example-69) sucks thus I ignore it
* MarkDown escape characters are supported
* I hate [Indent code block](https://spec.commonmark.org/0.31.2/#indented-code-blocks).
* Rough support for markdown [code fences](https://spec.commonmark.org/0.31.2/#code-fence) but common usage is well tested.
* Rough support for markdown [code span](https://spec.commonmark.org/0.31.2/#code-spans) but common usage is well tested.
* Rough support for markdown [emphasis](https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis) but common usage is well tested.

Show all supported features in [test](./test/)

> Note: undocumented/untested features is undefined behavior

## Setup development environment
Easy to setup development environment by my [Dockerfile](./Dockerfile)

### Setup docker image and container
```sh
docker build -t pltxt2htm .
docker run -d --name pltxt2htm-dev pltxt2htm
```

### Enter container
```sh
docker exec -it pltxt2htm-dev sh
```

## distribution
`cmd/`, `wasm/` and `py/` distributions share the same version of `pltxt2htm::version`

`pltxt2htm` only maintains the trunk, and the release versions are only snapshots.

For linux users, please use `wine` or compile and install `pltxt2htm` (and I will never provide binaries for linux since there are tons of linux triplets caused by arch and libc)

## others

> Q: Is this the time to use C++20 module?

A: Not exactly. Despite clang, gcc and msvc all support C++20 modules, but the compiler crashes more frequently than hearder-only. At the same time, Header unit is not fullly supported.

> Q: Why not use NDEBUG macro in include/pltxt2htm

A: Conditional compilation in function body will cause [ODR violation](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same problem. Therefore, to make function has different symbols in debug / release mode, I use `template<bool ndebug>` to achieve it.
