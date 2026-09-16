# pltxt2htm

![language](https://img.shields.io/badge/language-C++23-blue.svg)
![license](https://img.shields.io/badge/License-GPL-green.svg)

This repository is a sub-project of [Physics-Lab-Web2](https://github.com/NetLogo-Mobile/plweb2)

**header-only** C++23 library for converting Quantum-Physics (aka Physics-Lab) text format to HTML, with support for markdown and latex as much as possible (see the Features section below). Requires g++ >= 14, clang++ >= 20 or MSVC >= VS2026. Built without C++ RTTI and exceptions for maximum cross-platform compatibility.

> Note: MSVC support is still experimental

## Usage
C++ APIs are in [include/pltxt2htm](include/pltxt2htm/). [Click here](examples) to see a simple example.

You can also use `pltxt2htm` in console (cmd/README.md), browser (wasm/README.md), python (py/README.md), c (c/README.md), rust (rust/README.md) and csharp (csharp/README.md).

C++20 module example is in [cxxmodule/examples/example.cc](cxxmodule/examples/example.cc)

## Documentation
See [docsgen](./docsgen/README.md).

## Exported API
All C++ APIs have been listed in [pltxt2htm.cppm](cxxmodule/pltxt2htm/pltxt2htm.cppm)

## Compatibility

This library does not guarantee permanent backward compatibility for any facility. The function signatures of `pltxt2fixedadv_html`, `pltxt2plunity_introduction`, and `pltxt2common_html`, together with those of the corresponding functions in each language binding, are considered mostly stable. This is a statement of intent, not a compatibility guarantee.

All other public C++ APIs and all ABIs are unstable. Every update should be treated as potentially breaking for consumers of those interfaces.

## Features

Markdown extension based on [CommonMark](https://spec.commonmark.org/0.31.2/):

| Feature | Support | Details |
|---------|---------|---------|
| **Line breaks** | Supported | line break (no trailing-space requirement) |
| **ATX headers** | Supported | `#`–`######` → `<h1>`–`<h6>`. Trailing `#` not stripped. |
| **Setext headers** | Not supported | |
| **Escape characters** | Supported | Backslash escape for all ASCII punctuation |
| **Indented code blocks** | Not supported | |
| **Code fences** | Basic | ` ``` ` / `~~~` → `<pre><code>`. Optional language annotation. Built-in, dependency-free C++ and Rust syntax highlighting for web and PLUnity rich-text output. Fixed 3-delimiter only. |
| **Code spans** | Basic | Nested backticks not supported. |
| **Emphasis** | Basic | `*`/`_` → `<em>`, `**`/`__` → `<strong>`, `***`/`___` → `<em><strong>`. No intra-word emphasis or delimiter-run rules. |
| **Block quotes** | Basic | `>` lines → `<blockquote>`. Single-level only. No lazy continuation. |
| **Links** | Basic | `[text](url)` → `<a href="...">`. HTTP(S) only, TLD-restricted. No reference-style or title attribute. |
| **Images** | Basic | `![alt](url)` → `<img>`. Same URL restrictions as links. |
| **Strikethrough** | Supported | `~~text~~` → `<del>` |
| **Horizontal rules** | Supported | `---` / `***` / `___` (3+ chars) → `<hr>` |
| **Lists** | Supported | `-`/`+`/`*` → `<ul>`, `1.` → `<ol>`, with nesting and checkbox items (`- [ ]` / `- [x]`). No tight/loose distinction. |
| **Tables** | Supported | Pipe tables with alignment (`:---`, `:---:`, `---:`). Backslash-escaped pipes. |
| **LaTeX** | Basic | Inline `$...$` and block `$$...$$`. Raw passthrough, no delimiter escaping. |
| **Inline HTML** | Basic | Raw HTML tags. Generic attributes not supported. |

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
| `link` (Unity TextMeshPro link) | Supported |
| `i` | Supported |
| `b` | Supported |
| `u` (Unity TextMeshPro underline) | Supported |
| `s` (Unity TextMeshPro strikethrough) | Supported |
| `sup` (Unity TextMeshPro superscript) | Supported |
| `sub` (Unity TextMeshPro subscript) | Supported |
| `font` (Unity TextMeshPro font) | Not supported |
| `{Project}` / `{Visitor}` / `{Author}` / `{CoAuthors}` | Supported macros |

> Any new features (e.g. markdown extensions) may break existing Quantum-Physics texts, but compatibility concerns do not block progress.

See [tests](./tests/) for details on all supported features.

## Distribution

All distributions share the same version of `pltxt2htm::version`

`pltxt2htm` only maintains the trunk, and the release versions are only snapshots.

For Linux users, the `$ARCH-linux-musl-pltxt2htm-cmd-{debug|release}` distribution is statically linked against musl-libc. You can also use `wine pltxt2htm.exe`, `wavm run pltxt2htm.wasm` or compile and install `pltxt2htm`.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development setup, running tests, coding guidelines, and pull request workflow.

## Others

> Q: Is this the time to use C++20 module?

A: Not exactly. Compiler support has matured, but modules still require build-system cooperation, making them more painful to integrate than header-only. We ship a C++20 module interface as an optional alternative, yet header-only remains the primary distribution for maximum portability.

> Q: Why not use the NDEBUG macro in include/pltxt2htm?

A: Conditional compilation in a function body will cause [ODR violation](https://en.cppreference.com/w/cpp/language/definition) and [C++26 Contracts](https://en.cppreference.com/w/cpp/language/contracts) has the same problem. Therefore, to give functions different symbols in debug / release mode, I use `template<pltxt2htm::Contracts ndebug>` to achieve it.

> Q: Does using C++ mean this project is memory-unsafe?

A: Memory safety is a matter of engineering practice, not language choice alone. This project mitigates risks through:

* Extensive assertions in debug builds for memory and logic correctness.
* Over 95% test coverage.
* CI asan testing on every commit with both Clang and GCC.
* CI clang scan-build static analysis that fails the build on any reported bug.
* At least 6 hours of Clang fuzzing (with ASan or UBSan) before every release.

> Q: Why use C++ instead of a language with VM?

A: Only system languages have the strongest ability to not only run on different architectures and platforms but also provide bindings to other languages.
