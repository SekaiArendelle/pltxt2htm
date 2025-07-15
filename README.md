# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web](https://github.com/wsxiaolin/physics-lab-web)

This project aimed to provide a translater that compiles the Quantam-PhysicsLab's text to html (with markdown, minor html tags and latex support [as much as possible](#features)).

This project requires at least C++23, disables rtti and exceptions, use header-only style to supply api, has great cross platform ability and I strongly recommend you to use clang.

# How to use?
You can use `pltxt2htm` in [console](cmd/README.md), [browser](wasm/README.md) and [python](py/README.md).

C++ APIs is in [include/pltxt2htm](include/pltxt2htm/), here is a simple example:
```cpp
// example.cc
#include <fast_io/fast_io.h> // in include/fast_io
#include <pltxt2htm/pltxt2htm.hh> // in include/pltxt2htm

int main() noexcept {
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

And I strongly suggest you to add `-O2`, `-fno-ident`, `-fno-exceptions`, `-fno-rtti`, `-fno-unwind-tables`, `-fno-asynchronous-unwind-tables`, `-DNDEBUG`, `-fuse-ld=lld`, `-flto=thin`, `-stdlib=libc++`, `-rtlib=compiler-rt`, `--unwindlib=libunwind` and cross compiling flags(`--target=$TRIPLET` and `--sysroot=$YOUR_SYSROOT_PATH`) to your clang.

## Exported API
* `pltxt2htm::parse_pltxt`: Get AST of Quantum-Physics's text
  - only exported in C++ API (include/pltxt2htm/pltxt2htm.hh)
* `pltxt2htm::pltxt2advanced_html`: Render for Experiment's introduction text, all Quantum-Physics's Tag, minor HTML tag, most of the markdown and latex syntax is supported.
  - only exported in C++ API (include/pltxt2htm/pltxt2htm.hh)
* `pltxt2htm::advanced_parser`: C-Style pointer interface wrapper for pltxt2advanced_html
  - in include/pltxt2htm/pltxt2htm.h
  - Python API: `pltxt2htm.advanced_parser(text: str, host: str) -> str`
  - WASM API: `_advanced_parser(text: string, host: string) -> string`
* `pltxt2htm::pltxt2common_html`: Render for Experiment's title, very few syntax is enabled.
  - only exported in C++ API (include/pltxt2htm/pltxt2htm.hh)
* `pltxt2htm::common_parser`: C-Style pointer interface wrapper for pltxt2common_html
  - in include/pltxt2htm/pltxt2htm.h
  - Python API: `pltxt2htm.common_parser(text: str) -> str`
  - WASM API: `_common_parser(text: string) -> string`
* version
  - C++ API: `pltxt2htm::version::(major|minor|patch)`: Get version of pltxt2htm
  - Python API: `pltxt2htm.__version__`
  - WASM API: `_ver_major() -> number`, `_ver_minor() -> number`, `_ver_patch() -> number`

## features
I am ensure any new features (like markdown extension) will break old Quantum Physics's text. However, compatibility is not the reason that we should stop our steps. Here are some features why I (or why I not) support:

markdown extension based on [commonmark](https://spec.commonmark.org/0.31.2/)

* carriage return will be regarded as line break (unlike markdown, which requires 2 spaces before a carriage return to be considered a line break)
* HTML's br tag is supported (&lt;br&gt;, &lt;BR&gt;, &lt;br/&gt;, &lt;br&nbsp;&nbsp;&nbsp;&gt;, &lt;br&nbsp;&nbsp;&nbsp;/&gt; will be regarded as line break)
* Quantum-Physics's color tag is supported
* Quantum-Physics's a tag is supported
* Quantum-Physics's Discussion tag is supported
* Quantum-Physics's Experiment tag is supported
* Quantum-Physics's user tag is supported
* Quantum-Physics's size tag is supported
* Quantum-Physics&HTML's i tag is supported
* Quantum-Physics&HTML's b tag is supported
* HTML's &lt;h1&gt; tag is supported
* HTML's &lt;h2&gt; tag is supported
* HTML's &lt;h3&gt; tag is supported
* HTML's &lt;h4&gt; tag is supported
* HTML's &lt;h5&gt; tag is supported
* HTML's &lt;h6&gt; tag is supported
* HTML's &lt;p&gt; tag is supported
* HTML's &lt;del&gt; tag is supported
* most of the markdown ATX headers are supported
  - [commonmark example 69 ~ 76](https://spec.commonmark.org/0.31.2/#example-69) does not support
* MarkDown escape characters are supported
* [Indent code block](https://spec.commonmark.org/0.31.2/#indented-code-blocks) does not support because I hate it.

> Note: undocumented/untested features is undefined behavior

## distribution
`cmd/`, `wasm/` and `py/` distributions share the same version of `pltxt2htm::version`

`pltxt2htm` only maintains the trunk, and the release versions are only snapshots.

For linux users, please compile and install `pltxt2htm` (and I will never provide binaries for linux since there are tons of linux triplets caused by glibc, musl and architectures)

Users should also compile and install the Python C-extension for pltxt2htm because I don't want to build thousands of versions of Python.

## others

> Q: Is this the time to use C++20 module?

A: Not exactly. Despite clang, gcc and msvc all support C++20 modules, but the compiler crashes more frequently than hearder-only. At the same time, Header unit is not fullly supported.

> Q: Why not use NDEBUG macro in include/pltxt2htm

A: Conditional compilation in function body will cause [ODR violation](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same problem. therefore, to make function has different symbols in debug / release mode, I use `template<bool ndebug>` to achieve it.
