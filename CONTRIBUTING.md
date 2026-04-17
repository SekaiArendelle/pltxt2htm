# Contributing to `pltxt2htm`

Thanks for your interest in contributing!

This project is a C++23 header-only library for converting Physics-Lab text to HTML, with bindings and distributions for multiple environments.

## Before You Start

- Read the main project overview in `README.md`.
- Check component-specific docs when needed:
  - CLI: `cmd/README.md`
  - Tests: `tests/README.md`
  - Python binding: `py/README.md`
  - C API: `c/README.md`
  - Rust binding: `rust/README.md`
  - C# binding: `csharp/README.md`
  - WASM: `wasm/README.md`

## Development Environment

You can use your local environment or Docker.

### Docker workflow

Use the repository `Dockerfile`:

```sh
docker build -t pltxt2htm .
docker run -d --name pltxt2htm-dev pltxt2htm
```

Enter the container:

```sh
docker exec -it pltxt2htm-dev bash
```

This repository primarily uses **xmake** for configuration, building, and testing.

Typical workflow:

```sh
xmake config
xmake build
```

You may pass options such as `--toolchain=`, `--sysroot=`, `--arch=`, and `--plat=` depending on your target.

## Code Formatting

Format project C++ code from the repository root with:

```sh
ninja -f format_cpp.ninja
```

## Running Tests

Run all tests from the repository root:

```sh
python ./tests/run_all_tests.py
```

Or with xmake:

```sh
xmake config
xmake test
```

Sanitizer examples:

```sh
# ASan
xmake config --policies=build.sanitizer.address
xmake test

# MSan
xmake config --policies=build.sanitizer.memory
xmake test
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
- Use fully qualified namespace style:
  - Prefer `::ns::fn_or_cls` for function/class references to avoid ADL-based calls.
- Prefer unambiguous initialization:
  - Prefer brace initialization (`T x{...}`) when constructing typed instances.
  - Avoid copy/equal-sign initialization (`T x = ...`) when it can hide implicit narrowing conversions.
  - Avoid initialization forms that look like declarations but actually construct objects (the "most vexing parse" style).
- Mark terminal error branches as cold paths:
  - For branches that call `exit`/`terminate`, mark the branch with `[[unlikely]]`.
  - For branches that end in `unreachable`, mark the branch with `[[unlikely]]` as well.
- Prefer guard-clause error handling:
  - Return early on failure (for example, `if (!ok) { return err; }`) and keep the normal path unindented below.
- Require `[[nodiscard]]` on meaningful return values:
  - For all non-`void`, non-`[[noreturn]]` functions/methods, add `[[nodiscard]]` to the declaration/definition (except `operator=`).
- Prefer `const` for non-mutating locals:
  - For local variables that are not reassigned after initialization, declare them as `const`.
  - In particular, when reviewing `include/pltxt2htm`, add missing `const` qualifiers whenever semantics allow.

## Commit and Pull Request Guidelines

- Write clear commit messages that explain *what* changed and *why*.
- Keep pull requests scoped to a single logical change.
- Include:
  - problem statement
  - summary of the solution
  - testing performed (commands + results)
- If output behavior changes, include before/after examples when possible.

## Reporting Issues

When opening an issue, please include:

- expected behavior
- actual behavior
- minimal reproduction input
- platform/toolchain details (OS, compiler, target, xmake config)

## Security

If you discover a security-sensitive issue, please avoid posting exploit details publicly before maintainers can respond.

## License

By contributing, you agree that your contributions are licensed under the project license in `LICENSE`.
