# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web2](https://github.com/NetLogo-Mobile/plweb2)

**header-only** C++23 library for converting Quantum-Physics(aka. Physics-Lab)'s text format to HTML, with supports for markdown and latex [as much as possible](#features). Requires g++ >= 14, clang++ >= 20 or MSVC >= VS2026. Build without C++ rtti and exceptions for maximum cross-platform compatibility.

> Note: MSVC support is still experimental

## Usage
C++ APIs is in [include/pltxt2htm](include/pltxt2htm/), [Click here](examples/example.cc) to see a simple example.

You can also use `pltxt2htm` in [console](cmd/README.md), [browser](wasm/README.md), [python](py/README.md), [c](c/README.md), [rust](rust/README.md) and [csharp](csharp/README.md).

C++20 module example is in [cxxmodule/example.cc](cxxmodule/example.cc)

## Exported API
All C++ APIs have been listed in [pltxt2htm.cppm](cxxmodule/pltxt2htm/pltxt2htm.cppm)

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
* [setext headers](https://spec.commonmark.org/0.31.2/#setext-headings) is not planed
* MarkDown escape characters are supported
* I hate [Indent code block](https://spec.commonmark.org/0.31.2/#indented-code-blocks).
* Rough support for markdown [code fences](https://spec.commonmark.org/0.31.2/#code-fence) but common usage is well tested.
* Rough support for markdown [code span](https://spec.commonmark.org/0.31.2/#code-spans) but common usage is well tested.
* Rough support for markdown [emphasis](https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis) but common usage is well tested.
* Rough support for markdown [block quotes](https://spec.commonmark.org/0.31.2/#block-quotes) but common usage is well tested.
* Rough support for markdown [links](https://spec.commonmark.org/0.31.2/#links) but common usage is well tested.
* Rough support for markdown [lists](https://spec.commonmark.org/0.31.2/#lists) but common usage is well tested.
* `Checkbox`, `markdown table` and `mermaid` is not planed yet.

Show all supported features in [test](./test/)

## Setup development environment
Easy to setup development environment by my [Dockerfile](./Dockerfile)

### Setup docker image and container
```sh
docker build -t pltxt2htm .
docker run -d --name pltxt2htm-dev pltxt2htm
```

### Enter container
```sh
docker exec -it pltxt2htm-dev bash
```

## distribution
All distributions share the same version of `pltxt2htm::version`

`pltxt2htm` only maintains the trunk, and the release versions are only snapshots.

For linux users, the `$ARCH-linux-musl-pltxt2htm-cmd-{debug|release}` distribution have static-linked musl-libc. you can also use `wine pltxt2htm.exe`, `wavm run pltxt2htm.wasm` or compile and install `pltxt2htm`.

## others

> Q: Is this the time to use C++20 module?

A: Not exactly. Despite clang, gcc and msvc all support C++20 module, but the compiler crashes more frequently than hearder-only. At the same time, C++20 module heavily rely on build system, which makes it more painful than header-only.

> Q: Why not use NDEBUG macro in include/pltxt2htm

A: Conditional compilation in function body will cause [ODR violation](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same problem. Therefore, to make function has different symbols in debug / release mode, I use `template<bool ndebug>` to achieve it.

> Q: Why use C++ instead of a "memory safe" language like Rust?

A: I appreciate Rust but not for its "safety". Basically, safety should always be guaranteed by programmer, because most of the logic error can't be detected at compile time, not even type safety can be handled, thats why rtti exists, while Rust is not 100% memory safe.

At the same time, `pltxt2htm` is absolutely safe:
* Lots of assertions are enabled in debug mode to assure memory safety and logic correctness.
* Over 90% (with a future target of 95%) test coverage.
* Every commit in master undergose testing with asan under both clang and gcc in ci.
* Every release undergose at least 6 hours of fuzzing with clang, with asan or ubsan enabled.

> Q: Why use C++ instead of a language with VM?

A: Only system languages have the strongest ability to not only run in different arch and platform but provide binds to other language.
