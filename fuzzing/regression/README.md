# Regression corpus

This directory holds crash inputs from past fuzzing runs, kept as regression
seeds. Layout:

```
regression/<fuzzer-target>/<descriptive-name or crash hash>
```

Workflow when libFuzzer reports a crash (artifact lands in `fuzzing/artifacts/`
as `<target>-crash-<sha1>` and similar):

1. Minimize the input if needed (see fuzzing/README.md).
2. Move the minimized artifact into `regression/<target>/` and commit it.
3. Add a regression test under `tests/src/syntax/` referencing the artifact
   (existing examples: `tests/src/syntax/html/html_div_tag.hh` and
   `tests/src/syntax/md/md_code_span.hh`).

Every fuzz target also loads its `seeds/` and `corpus/` directories, and
libFuzzer runs each file passed as an argument on its own, so these files
double as a regression test suite:

```sh
pixi run -e clang fuzzing common_parser asan "../regression/common_parser/*"
```

Note: pass the files via a glob, not the directory itself - libFuzzer treats
an extra directory argument as an additional (persistent) corpus dir and
starts fuzzing instead of running the inputs once. The glob requires the
directory to be non-empty; an unmatched glob aborts the task.
