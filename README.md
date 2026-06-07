# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web2](https://github.com/NetLogo-Mobile/plweb2)

**header-only** C++23 library for converting Quantum-Physics(aka. Physics-Lab)'s text format to HTML, with supports for markdown and latex as much as possible (see the Features section below). Requires g++ >= 14, clang++ >= 20 or MSVC >= VS2026. Build without C++ rtti and exceptions for maximum cross-platform compatibility.

> Note: MSVC support is still experimental

## Usage
C++ APIs is in [include/pltxt2htm](include/pltxt2htm/), [Click here](examples) to see a simple example.

You can also use `pltxt2htm` in console (cmd/README.md), browser (wasm/README.md), python (py/README.md), c (c/README.md), rust (rust/README.md) and csharp (csharp/README.md).

C++20 module example is in [cxxmodule/examples/example.cc](cxxmodule/examples/example.cc)

## Documentation
See [docsgen](./docsgen/README.md).

## Exported API
All C++ APIs have been listed in [pltxt2htm.cppm](cxxmodule/pltxt2htm/pltxt2htm.cppm)

## Features

Markdown extension based on [CommonMark](https://spec.commonmark.org/0.31.2/):

| Feature | Support | Details |
|---------|---------|---------|
| **Line breaks** | Supported | Carriage return → line break (no trailing-space requirement) |
| **ATX headers** | Supported | `#`–`######` → `<h1>`–`<h6>`. Trailing `#` not stripped. |
| **Setext headers** | Not planned | |
| **Escape characters** | Supported | Backslash escape for all ASCII punctuation |
| **Indented code blocks** | Not planned | |
| **Code fences** | Basic | ` ``` ` / `~~~` → `<pre><code>`. Optional language annotation. Fixed 3-delimiter only. |
| **Code spans** | Basic | No intra-span backtick balancing. |
| **Emphasis** | Basic | `*`/`_` → `<em>`, `**`/`__` → `<strong>`, `***`/`___` → `<em><strong>`. No intra-word emphasis or delimiter-run rules. |
| **Block quotes** | Basic | `>` lines → `<blockquote>`. Single-level only. No lazy continuation. |
| **Links** | Basic | `[text](url)` → `<a href="...">`. HTTP(S) only, TLD-restricted. No reference-style or title attribute. |
| **Images** | Basic | `![alt](url)` → `<img>`. Same URL restrictions as links. |
| **Strikethrough** | Supported | `~~text~~` → `<del>` |
| **Horizontal rules** | Supported | `---` / `***` / `___` (3+ chars) → `<hr>` |
| **Lists** | Supported | `-`/`+`/`*` → `<ul>`, `1.` → `<ol>`, with nesting and checkbox items (`- [ ]` / `- [x]`). No tight/loose distinction. |
| **Tables** | Supported | Pipe tables with alignment (`:---`, `:---:`, `---:`). Backslash-escaped pipes. |
| **LaTeX** | Basic | Inline `$...$` and block `$$...$$`. Raw passthrough, no delimiter escaping. |
| **HTML blocks** | Partial | Individual HTML tags parsed inline (bare tags, no generic attributes). PL-specific key=value attributes supported. |

Quantum-Physics tags:

| Tag | Status |
|-----|--------|
| `color` | Supported |
| `a` | Supported |
| `discussion` | Supported |
| `experiment` | Supported |
| `user` | Supported |
| `size` | Supported |
| `external` | Supported |
| `i` | Supported |
| `b` | Supported |
| `{Project}` / `{Visitor}` / `{Author}` / `{CoAuthors}` | Supported macros |

> Any new features (e.g. markdown extensions) may break existing Quantum-Physics texts, but compatibility concerns do not block progress.

See [tests](./tests/) for details on all supported features.

## distribution

All distributions share the same version of `pltxt2htm::version`

`pltxt2htm` only maintains the trunk, and the release versions are only snapshots.

For linux users, the `$ARCH-linux-musl-pltxt2htm-cmd-{debug|release}` distribution have static-linked musl-libc. you can also use `wine pltxt2htm.exe`, `wavm run pltxt2htm.wasm` or compile and install `pltxt2htm`.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development setup, running tests, coding guidelines, and pull request workflow.

## others

> Q: Is this the time to use C++20 module?

A: Not exactly. Despite clang, gcc and msvc all support C++20 module, but the compiler crashes more frequently than hearder-only. At the same time, C++20 module heavily rely on build system, which makes it more painful than header-only.

> Q: Why not use NDEBUG macro in include/pltxt2htm

A: Conditional compilation in function body will cause [ODR violation](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same problem. Therefore, to make function has different symbols in debug / release mode, I use `template<pltxt2htm::Contracts ndebug>` to achieve it.

> Q: Why use C++ instead of a "memory safe" language like Rust?

A: I appreciate Rust but not for its "safety". Basically, safety should always be guaranteed by programmer, because most of the logic error can't be detected at compile time, not even type safety can be handled, thats why rtti exists, while Rust rely on `unsafe` block.

At the same time, `pltxt2htm` is absolutely safe:
* Lots of assertions are enabled in debug mode to assure memory safety and logic correctness.
* Over 95% test coverage.
* Every commit in main undergose testing with asan under both clang and gcc in ci.
* Every release undergose at least 6 hours of fuzzing with clang, with asan or ubsan enabled.

> Q: Why use C++ instead of a language with VM?

A: Only system languages have the strongest ability to not only run in different arch and platform but provide binds to other language.
