Clang libFuzzer harnesses for the exported APIs in pltxt2htm.

The layout mostly follows the conventions used by OSS-Fuzz-style projects:
one harness binary per fuzz target, a small checked-in seed corpus per
target, a shared dictionary, and crash artifacts collected into a gitignored
directory. Accumulated units are kept out of git and folded back into the
seed corpus via `libFuzzer -merge` minimization.

## Layout

| Path | Purpose | Tracked |
|------|---------|---------|
| `src/<target>.cc` | `LLVMFuzzerTestOneInput` harnesses | yes |
| `seeds/<target>/` | Curated seed corpus, grown via merge minimization | yes |
| `corpus/<target>/` | Runtime corpus; new coverage units accumulate here | no |
| `dictionaries/pltxt.dict` | Shared libFuzzer dictionary for all targets | yes |
| `artifacts/` | Crash/oom/leak/timeout/slow-unit artifacts | no |
| `logs/` | libFuzzer job logs and scratch space | no |

Targets:

| Target | Exercises |
|--------|-----------|
| `common_parser` | `pltxt2common_html` |
| `fixedadv_parser` | `pltxt2fixedadv_html` |
| `fixedadv_roundtrip` | Roundtrip/idempotency: `f(x) == f(f(x))`, aborts on mismatch |
| `plrichtext_parser` | `pltxt2plunity_introduction` |

## Usage (pixi tasks)

All orchestration lives in the `fuzzing` tasks of the `clang` environment in
the root `pixi.toml`; builds are incremental.

```sh
pixi run -e clang fuzzing common_parser # run until Ctrl-C
pixi run -e clang fuzzing common_parser "" "-max_total_time=600" # 10 minutes, no sanitizer
pixi run -e clang fuzzing common_parser address "-jobs=8 -workers=8" # 8 parallel workers
pixi run -e clang fuzzing common_parser address,undefined # multi-sanitizer build
```

Arguments are positional: `fuzzing <target> [sanitizer] [fuzzer_args]`, where
`sanitizer` (default: empty, i.e. none) is a comma-separated list passed
verbatim to `-fsanitize=` - `address`, `undefined`, `memory` - and
`fuzzer_args` is passed through to libFuzzer verbatim (shell-expanded, so
values containing `*` or `?` are glob-expanded by the task shell).

Notes:

- Requires clang (libFuzzer is incompatible with other compilers here) and is
  unsupported on Windows (`CMakeLists.txt` enforces both). The `memory`
  sanitizer additionally needs instrumented dependencies.
- Long unattended sessions should run inside a terminal multiplexer or
  container rather than being daemonized - the crash artifacts and corpus
  updates are the valuable output, not a detached process.

New coverage units are written into the gitignored `fuzzing/corpus/<target>/`
and never touch git; the checked-in `fuzzing/seeds/<target>/` is loaded as a
read-only source of seeds during runs. Fold the useful units into the seed
corpus with the merge task below (see `seeds/common_parser/comprehensive.md`
for provenance: it is a snapshot of `tests/data/comprehensive.md`, to be
refreshed when that fixture changes).

### Minimizing the corpus

```sh
pixi run -e clang fuzzing-merge common_parser
rm -rf fuzzing/corpus/common_parser   # start the next session from a clean slate
```

The merge writes a minimal set of `fuzzing/corpus/<target>/` units that add coverage features over the current seeds into
`fuzzing/seeds/<target>/` - usually a few hundred small files instead of tens of thousands of raw units.

### After a crash

The artifact lands in `fuzzing/artifacts/` as `<target>-crash-<sha1>` (also
`-oom-`, `-leak-`, `-timeout-`, `-slow-unit-`). Reproduce it by passing the
file to the target. Once the bug is fixed, move the minimized input into
`fuzzing/seeds/<target>/` and add a regression test under `tests/src/syntax/`
so future fuzzing and test runs both cover it.

