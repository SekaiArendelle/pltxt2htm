# Contributing to `pltxt2htm`

Thanks for your interest in contributing!

This project is a C++23 header-only library for converting Physics-Lab text to HTML, with bindings and distributions for multiple environments.

## Typical workflow

1. **Locate** – Read the relevant sub-project README listed under
   [Before You Start](#before-you-start) to understand which module to modify.
2. **Code & Analyze** – Follow the [Coding Guidelines](#coding-guidelines) while writing your changes, then run the [formatting](#code-formatting) and static analysis tools.
3. **Test** – [Run all tests](#running-tests) as described in Running Tests.
4. **Submit** – Only ever submit an Issue (see [Pull Requests](#pull-requests)).

## Before You Start

- Read the main project overview in [README.md](./README.md).
- Check component-specific docs when needed:
  - CLI: [cmd/README.md](./cmd/README.md)
  - Tests: [tests/README.md](./tests/README.md)
  - Python binding: [py/README.md](./py/README.md)
  - C API: [c/README.md](./c/README.md)
  - Rust binding: [rust/README.md](./rust/README.md)
  - C# binding: [csharp/README.md](./csharp/README.md)
  - WASM: [wasm/README.md](./wasm/README.md)
  - Benchmarks: [benches/README.md](./benches/README.md)

## Development Environment

You can use your local environment or Docker.

### Docker workflow

Use the repository [Dockerfile](./Dockerfile):

```sh
docker build -t pltxt2htm .
docker run -d --name pltxt2htm-dev -v .:/pltxt2htm pltxt2htm
```

Enter the container:

```sh
docker exec -it pltxt2htm-dev bash
```

The source directory is mounted into the container, so edits made inside are reflected on the host and vice versa.

Each sub-project (`c/`, `cmd/`, `tests/`, etc.) is independently built with CMake. See the respective `README.md` for build instructions.

## Code Formatting

Format project C++ code from the repository root with:

```sh
ninja -f format_cpp.ninja
```

Configuration is in `.clang-format` at the project root. Re-generate `format_cpp.ninja` after adding new source files:

```sh
python scripts/gen_format_ninja.py
```

## Static Analysis

Run clang-tidy on all C/C++ source files:

```sh
ninja -f tidy_cpp.ninja
```

Configuration is in `.clang-tidy` at the project root. Re-generate `tidy_cpp.ninja` after adding new source files:

```sh
python scripts/gen_tidy_ninja.py
```

## Running Tests

Run all tests from the repository root:

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

For coverage:

```sh
python ./tests/codecov.py
```

## Coding Guidelines

- Keep changes focused and minimal.
- Follow existing style and structure in the touched module.
- Preserve compatibility with current behavior unless the change intentionally updates behavior.
- Add or update tests when behavior changes.

### C++ Style Conventions (`include/pltxt2htm`)

Please follow the existing low-runtime, cross-platform style used in core headers:

- Prefer compile-time evaluation:
  - Write functions as `constexpr` (or `consteval` when required) whenever semantics allow.
  - Keep parsing/helpers friendly to compile-time checking where practical.
- Avoid C++ runtime-heavy standard library components:
  - Do not introduce dependencies such as iostream/locale/RTTI-driven facilities for core logic.
  - Prefer existing project choices (`fast_io` containers/string types and `exception` utilities).
- No exception-based control flow:
  - Do not throw/catch exceptions in core code paths.
  - Use existing assertion + terminate/panic patterns for unrecoverable states.
  - Since exceptions are not used, prefer marking functions/methods `noexcept` whenever valid.
- No RTTI / dynamic polymorphism patterns:
  - Do not use `typeid`, `dynamic_cast`, or virtual dispatch for new logic.
- Avoid macros:
  - Do not introduce new macros for regular logic, constants, or API design.
  - Prefer `constexpr`/`consteval`, templates, and inline functions.
  - Keep macro usage only for tightly scoped compatibility/assertion infrastructure when unavoidable.
- Forbid `volatile` and `register`:
  - Do not introduce `volatile` or `register` in new code.
- Prefer C++23 deducing-this:
  - For member functions, prefer explicit object parameters (deducing-`this`) over implicit `this` pointer style when practical.
  - Keep const/ref-qualified overload behavior explicit via the object parameter form.
- Avoid mutable global state:
  - Do not add global variables or other shared mutable state that can create hidden side effects.
  - Compile-time constants (`constexpr`/`consteval`, including class static constants) are acceptable.
- Keep side effects separated from algorithms:
  - Put pure algorithmic logic in headers under `include/` whenever practical.
  - Keep I/O and other side-effectful operations (file access, console output, process exits, etc.) in implementation files in module directories.
- Prefer `constexpr` function definitions by default:
  - Prefix function/method definitions with `constexpr` whenever the language permits (the entrypoint `main` is the exception).
  - Do not write `inline constexpr`; use `constexpr` directly (it is already inline).
- Do not name lambdas:
  - Never write `auto name = [](...) { ... };` and call by name later.
  - If a callable needs a name, extract it as a normal `constexpr` function.
  - Immediately invoked lambda expressions (`[]() { ... }()`) are acceptable only when necessary to compute a `constexpr` value in a context where `if constexpr` is not directly usable (e.g., inside a function with non-`constexpr` scope rules).
- Use fully qualified namespace style:
  - Prefer `::ns::fn_or_cls` for function/class references to avoid ADL-based calls.
- Prefer unambiguous initialization:
  - Prefer brace initialization (`T x{...}`) when constructing typed instances.
  - Avoid copy-initialization (`T x = ...`) when it can hide implicit narrowing conversions.
  - Avoid initialization forms that look like declarations but actually construct objects (the "most vexing parse" style).
- Mark terminal error branches as cold paths:
  - For branches that call `exit`/`terminate`, mark the branch with `[[unlikely]]`.
  - For branches that end in `unreachable`, mark the branch with `[[unlikely]]` as well.
- Prefer guard-clause error handling:
  - Return early on failure (for example, `if (!ok) { return err; }`) and keep the normal path unindented below.
- No `else` after `return`/`continue`/`break`/`goto`:
  - When a branch ends unconditionally with a jump statement, the `else` is unnecessary dead indentation; drop it and unindent the subsequent block.
- Require `[[nodiscard]]` on meaningful return values:
  - For all non-`void`, non-`[[noreturn]]` functions/methods, add `[[nodiscard]]` to the declaration/definition (except `operator=`).
- Prefer `const` for non-mutating locals:
  - For local variables that are not reassigned after initialization, declare them as `const`.
  - In particular, when reviewing `include/pltxt2htm`, add missing `const` qualifiers whenever semantics allow.
- Prefer `::std::addressof` over `operator&`:
  - Do not use the built-in `&` operator to obtain the address of an object, because `operator&` can be overloaded.
  - Use `::std::addressof(...)` instead, which correctly returns the address even for types with overloaded `operator&`.
- Use deducing-`this` for `operator=`:
  - All non-`= delete` `operator=` overloads must use the C++23 deducing-`this` form with an lvalue reference object parameter (`this X& self`) to prevent assignment to temporaries.
  - `= delete` overloads are exempt since they already prevent any use.

## Pull Requests

Pull requests from contributors I am not personally familiar with, or from AI agents, will be closed — not because the contribution isn't appreciated, but because handling external PRs is less efficient than writing the fix myself. Discussion and review of proposed approaches are always welcome.
If you have a solution in mind, please open an **Issue** with reference code or a sketch of the approach or a patch file. I will write the actual patch myself and credit you in the commit message.

## Reporting Issues

When opening an issue, please include:

- expected behavior
- actual behavior
- minimal reproduction input
- platform/toolchain details (OS, compiler, target, cmake config)

## Security

If you discover a security-sensitive issue, please avoid posting exploit details publicly before maintainers can respond.

## License

By contributing, you agree that your contributions are licensed under the project license in `LICENSE`.
