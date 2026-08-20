# AGENTS.md — Developer Guide for `pltxt2htm`

This file is the entry point for AI coding agents. It contains the development workflow, build/format/lint/test commands, and coding conventions. Human-facing contribution policy (pull requests, issues, license) lives in [CONTRIBUTING.md](./CONTRIBUTING.md).

## Mandatory rules

- **Do NOT run git write operations without explicit human instruction.** An agent must not run `git add`, `git commit`, `git push`, open a **Pull Request**, open an **Issue**, or perform any other write operation to the repository or remote unless the human explicitly asks for it.
- After changing code, run formatting, static analysis, and tests (commands below).

## Project layout

Header-only C++23 library converting Physics-Lab text to HTML, with bindings and distributions.

| Path | Purpose |
|------|---------|
| `include/pltxt2htm/` | Core headers (the library) |
| `cmd/` | CLI — see [cmd/README.md](./cmd/README.md) |
| `tests/` | Test suites — see [tests/README.md](./tests/README.md) |
| `py/` | Python binding — see [py/README.md](./py/README.md) |
| `c/` | C API — see [c/README.md](./c/README.md) |
| `rust/` | Rust binding — see [rust/README.md](./rust/README.md) |
| `csharp/` | C# binding — see [csharp/README.md](./csharp/README.md) |
| `wasm/` | WASM — see [wasm/README.md](./wasm/README.md) |
| `benches/` | Benchmarks — see [benches/README.md](./benches/README.md) |
| `fuzzing/` | Fuzzing — see [fuzzing/README.md](./fuzzing/README.md) |
| `scripts/` | Dev tooling scripts |
| `Dockerfile` | Dev container |

Read [README.md](./README.md) for the project overview.

## Development environment

Local tools:

- C++23 compiler (clang / gcc / MSVC)
- CMake
- Python 3
- clang-format and clang-tidy (for formatting / static analysis)

Docker alternative:

```sh
docker build -t pltxt2htm .
docker run -d --name pltxt2htm-dev -v .:/pltxt2htm pltxt2htm
docker exec -it pltxt2htm-dev bash
```

The source directory is mounted into the container, so edits are shared both ways.

Each sub-project is independently built with CMake — see the respective `README.md`.

## Workflow

1. **Locate** – Read the relevant sub-project README to understand which module to modify.
2. **Code** – Follow the [Coding conventions](#coding-conventions) below.
3. **Format & analyze** – Run the [formatting](#code-formatting) and [static analysis](#static-analysis) commands.
4. **Test** – Run the tests for the module you touched, then the full suite.
5. **Submit** – Do NOT run any git write operations (such as `git add`, `git commit`, `git push`) or open a PR/Issue without explicit human instruction. Present a patch file or a sketch of the approach instead (see [CONTRIBUTING.md](./CONTRIBUTING.md)).

## Quick commands (run from repository root)

### Code formatting

```sh
ninja -f format_cpp.ninja
```

Configuration is in `.clang-format` at the project root. `format_cpp.ninja` is a **generated artifact** — never edit it by hand. After adding new source files, regenerate it:

```sh
python scripts/gen_format_ninja.py
```

### Static analysis

```sh
python scripts/run_clang_tidy.py
```

Configuration is in `.clang-tidy` at the project root. The script configures each sub-project (`examples`, `cmd`, `c`, or as given) into `<sub>/build-tidy` with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` and runs clang-tidy on every translation unit in the resulting compile database, so the flags match the real build. Re-run after adding source files; pass `--refresh` to force a re-configure (e.g. after editing `CMakeLists.txt`). Machine-specific flags go through the standard `CXXFLAGS` environment variable, for example:

```sh
CXXFLAGS=-stdlib=libc++ python scripts/run_clang_tidy.py
```

### Running tests

Full suite:

```sh
python ./tests/run_all_tests.py
```

Or with CMake:

```sh
cmake -S tests -B tests/build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build tests/build
ctest --test-dir tests/build
```

Sanitizer examples:

```sh
# ASan
cmake -S tests -B tests/build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=address
cmake --build tests/build
ctest --test-dir tests/build

# MSan
cmake -S tests -B tests/build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=memory
cmake --build tests/build
ctest --test-dir tests/build
```

Coverage:

```sh
python ./tests/codecov.py
```

**Command cost note:** `run_all_tests.py` and `codecov.py` are the full, slowest checks. For quick iteration, build and test only the module you changed via its CMake config or README.

## Coding conventions

General:

- Keep changes focused and minimal.
- Follow existing style and structure in the touched module.
- Preserve compatibility with current behavior unless the change intentionally updates behavior.
- Add or update tests when behavior changes.

### C++ Style Conventions (`include/pltxt2htm`)

Follow the existing low-runtime, cross-platform style used in core headers:

- **Do not name lambdas:**
  - Never write `auto name = [](...) { ... };` and call by name later.
  - If a callable needs a name, extract it as a normal `constexpr` function.
  - Immediately invoked lambda expressions (`[]() { ... }()`) are acceptable only when necessary to compute a `constexpr` value in a context where `if constexpr` is not directly usable (e.g., inside a function with non-`constexpr` scope rules).
- **Avoid the redundant `(string_view, offset)` parameter pair:**
  - A `string_view` already carries both a pointer and a length, so a signature like `parse_value(::fast_io::u8string_view s, ::std::size_t pos)` is redundant — the offset is implicit in the view.
  - When a helper must skip a prefix, pass a pre-subviewed view (via `::pltxt2htm::details::u8string_view_subview<ndebug>(...)`) and let the function operate from index `0`. Have the returned `end` be relative to that subview and let the caller re-add the offset when absolute coordinates are needed.
  - This keeps each parser "parse the given view from the start", avoids offset arithmetic and empty-check (`pos == 0`) inside helpers, and keeps the call sites' intent explicit.
- **Keep side effects separated from algorithms:**
  - Put pure algorithmic logic in headers under `include/` whenever practical.
  - Keep I/O and other side-effectful operations (file access, console output, process exits, etc.) in implementation files in module directories.
- **Hoist loop-invariant expressions manually:**
  - Cache expressions that are unchanged for the full loop instead of relying on the compiler to move repeated work out of the loop.
  - Only hoist an expression when loop-body mutations and aliasing cannot change its result, and do not turn conditionally evaluated work into unconditional work.
- **Prefer compile-time evaluation:**
  - Write functions as `constexpr` (or `consteval` when required) whenever semantics allow.
  - Keep parsing/helpers friendly to compile-time checking where practical.
- **Prefer `constexpr` function definitions by default:**
  - Prefix function/method definitions with `constexpr` whenever the language permits (the entrypoint `main` is the exception).
  - Do not write `inline constexpr`; use `constexpr` directly (it is already inline).
- **Avoid leading `const` (use postfix `const`):**
  - Write the cv-qualifier after the type it qualifies (`int const`, `T const&`, `auto const`) rather than before it (`const int`, `const T&`, `const auto`).
  - `const` always binds to the declaration to its left, so postfix placement makes `int const*` (pointer to const int) vs `int* const` (const pointer to int) unambiguous at a glance.
- **Avoid C++ runtime-heavy standard library components:**
  - Do not introduce dependencies such as iostream/locale/RTTI-driven facilities for core logic.
  - Prefer existing project choices (`fast_io` containers/string types and `exception` utilities).
- **No exception-based control flow:**
  - Do not throw/catch exceptions in core code paths.
  - Use existing assertion + terminate/panic patterns for unrecoverable states.
- **No RTTI / dynamic polymorphism patterns:**
  - Do not use `dynamic_cast` or virtual dispatch for new logic.
- **Avoid macros:**
  - Do not introduce new macros for regular logic, constants, or API design.
  - Prefer `constexpr`/`consteval`, templates, and inline functions.
  - Keep macro usage only for tightly scoped compatibility/assertion infrastructure when unavoidable.
- **Forbid `volatile` and `register`:**
  - Do not introduce `volatile` or `register` in new code.
- **Prefer C++23 deducing-`this`:**
  - For member functions, prefer explicit object parameters (deducing-`this`) over implicit `this` pointer style when practical.
  - Keep const/ref-qualified overload behavior explicit via the object parameter form.
  - Pick the object-parameter form that matches the access the body needs; do not default to `this auto&&`:
    - `this Class& self` — mutating functions and accessors returning a non-const reference.
    - `this Class const& self` — read-only functions returning by value (including `string_view`, enums, `ValueWithUnit`).
    - `this auto&& self` (with `decltype(auto)` + `::std::forward_like`) — only when the function must forward the object's value category and constness, e.g. returning a reference to a member on both const and non-const objects.
  - `this auto&&` is a template: every distinct deduced object type produces a separate instantiation. On read-only getters it both bloats code (const/non-const/rvalue variants) and lets the member be called on rvalues, so prefer a concrete `&`/`const&` form unless forwarding is genuinely needed.
- **Use deducing-`this` for `operator=`:**
  - All non-`= delete` `operator=` overloads must use the C++23 deducing-`this` form with an lvalue reference object parameter (`this X& self`) to prevent assignment to temporaries.
  - `= delete` overloads are exempt since they already prevent any use.
- **Prefer fully qualified namespace style for function calls:**
  - Prefer `::ns::fn` when calling free functions to avoid accidental ADL-based calls.
  - Types and classes may be referenced with or without leading `::`; the fully qualified form is optional there and not required.
- **Prefer unambiguous initialization:**
  - Prefer brace initialization (`T x{...}`) when constructing typed instances.
  - Avoid initialization forms that look like declarations but actually construct objects (the "most vexing parse" style).
- **Mark terminal error branches as cold paths:**
  - For branches that call `exit`/`terminate`, mark the branch with `[[unlikely]]`.
  - For branches that end in `unreachable`, mark the branch with `[[unlikely]]` as well.
- **Prefer guard-clause error handling:**
  - Return early on failure (for example, `if (!ok) { return err; }`) and keep the normal path unindented below.
- **Prefer `::std::addressof` over `operator&`:**
  - Do not use the built-in `&` operator to obtain the address of an object, because `operator&` can be overloaded.
  - Use `::std::addressof(...)` instead, which correctly returns the address even for types with overloaded `operator&`.
