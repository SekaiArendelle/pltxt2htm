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

A concise reference template covering this structure lives in [`.gitmessage`](./.gitmessage).

### Attribution

When a model materially contributed code, documentation, tests, or other content to a commit, record that provenance with this Git trailer:

```
Assisted-by: <model name> [<version if known>]
```

Replace the placeholders with the exact identity provided by the environment. Include a version only when it is actually known; otherwise, omit it rather than guessing. Use one trailer per participating model.

The trailer applies when model-produced content is included in the commit. Do not add it merely because a model was used for brainstorming, searching, or review when none of its produced content was incorporated. Commits whose material was written entirely by humans carry no model-attribution trailer.

The trailer is disclosure metadata supplied by the contributor, not a mechanism for detecting undisclosed model use, and does not identify a GitHub co-author. Reserve `Co-authored-by` for human collaborators, using an email address associated with the collaborator's GitHub account. Git (git >= 2.32) can append the trailer:

```sh
git commit --trailer "Assisted-by: <model name> [<version if known>]"
```

## Pull Requests

Pull requests are welcome as reviewable implementation proposals. Keeping a change in a pull request makes its diff and discussion easy to inspect even when the submitted commits will not be merged as-is. I generally merge implementation commits directly only from contributors with whom I already have an established working relationship. For other contributions, I may reimplement the change instead, while preserving credit in the resulting commit message.

An **Issue** is useful for discussing an approach before code exists, but it is not required before opening a pull request.

Pull requests submitted directly by automated or AI agents will be closed. A human-submitted pull request carrying an `Assisted-by` trailer is not considered agent-submitted merely because it discloses model assistance.

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
