# Contributing to `pltxt2htm`

Thanks for your interest in contributing!

This project is a C++23 header-only library for converting Physics-Lab text to HTML, with bindings and distributions for multiple environments.

## Development

For the development workflow (build, format, static analysis, tests) and the coding conventions, see [AGENTS.md](./AGENTS.md).

## Commit messages

Commits follow the shape:

```
<type>(<scope>): <subject>

<body>

<footer>
```

- **type** — `feat`, `fix`, `refactor`, `perf`, `style`, `docs`, `test`, `build`, `ci`, `chore`, `revert`. Use `chore` when nothing else fits; it is the catch-all.
- **scope** — optional, names the sub-project or area touched: `include`, `cmd`, `tests`, `py`, `c`, `rust`, `csharp`, `wasm`, `benches`, `fuzzing`, `scripts`, `docsgen`, `ci`.
- **subject** — lowercase, imperative mood, no trailing period, at most 72 columns.
- **body** — optional, wrapped at 72 columns. Explain *why*; the diff already shows *what*.
- **footer** — optional. Issue references (`Closes #123`) and attribution trailers.

A template covering all of the above lives in [`.gitmessage`](./.gitmessage). Enable it once per clone:

```sh
git config commit.template .gitmessage
```

### Attribution

When a model produced the work being committed, record which one, instead of leaving it implicit:

```
fix(rust): free strings through C API

The binding leaked the converted buffer whenever conversion returned an
error, because the error path skipped the deallocation helper.

Closes #399
Co-authored-by: DeepSeek V3.2 <deepseek-v3.2@users.noreply.github.com>
```

Use one `Co-authored-by:` trailer per participating model, and a noreply address that belongs to nobody else's GitHub account — the trailer is plain text in the commit message, so a stand-in address keeps the attribution readable without mis-crediting a real account. `git commit` (git >= 2.32) can append trailers for you:

```sh
git commit --trailer "Co-authored-by: DeepSeek V3.2 <deepseek-v3.2@users.noreply.github.com>"
```

Add the trailer only when a model actually wrote the code. Commits written by hand carry no attribution trailer.

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
