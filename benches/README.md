# pltxt2htm Benchmarks

Microbenchmark suite using [Google Benchmark](https://github.com/google/benchmark) for quantitative performance analysis of `pltxt2htm`.

Google Benchmark is fetched and built automatically by CMake via `FetchContent` (v1.9.5 from GitHub).

## Prerequisites

- C++23 compiler (Clang/GCC/MSVC)
- CMake >= 3.20
- Ninja (recommended) or other build system
- Git (for `FetchContent`)

## Build

```sh
cmake -S benches -B benches/build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build benches/build
```

First build fetches and compiles Google Benchmark automatically. Subsequent builds reuse the cached source.

## Benchmarks

| Executable | Measures | Inputs |
|-----------|----------|--------|
| `bench_parse` | `parse_pltxt` only | plain text, rich markdown, PL tags, LaTeX, stress, optimizer-related inputs |
| `bench_optimize` | `optimize_ast` on directly constructed ASTs (no parse overhead) | nested color, adjacent color, mixed redundant, HTML spans |
| `bench_pipeline` | Parse-once then `optimize_ast` in loop (real-world pipeline) | plain text, redundant color, mixed redundant, adjacent text, HTML spans |
| `bench_backend` | 3 backends: `plweb_title`, `plweb_text`, `plunity_text` | plain text, rich HTML, PL tags, markdown ASTs |
| `bench_end2end` | All 4 public APIs (`common_html`, `pltxt4unittest`, `fixedadv_html`, `plunity_introduction`) | full document, terse document, plain document |
| `bench_micro` | Micro-operations (node creation/move, AST append, string ops) | N/A |

Data preparation is handled by Google Benchmark's `SetUp` in dedicated Fixture classes
(see `include/bench_fixtures.hh`). Each fixture generates and parses its input once
per benchmark invocation, before the measured loop.

## Run

```sh
.\benches\build\bench_end2end.exe
.\benches\build\bench_parse.exe --benchmark_format=csv
.\benches\build\bench_parse.exe --benchmark_format=json --benchmark_out=results.json
```

## Options

```
--benchmark_format=<console|csv|json>
--benchmark_out=<filename>
--benchmark_counters_tabular=true
--benchmark_filter=<regex>
--benchmark_min_time=<seconds>
```

See [Google Benchmark docs](https://github.com/google/benchmark/blob/main/docs/user_guide.md) for all options.

## Profiling

### Windows Performance Recorder

```powershell
wpr -start CPU -filemode
.\benches\build\bench_parse.exe
wpr -stop profile.etl
```

Open `profile.etl` in Windows Performance Analyzer (WPA).

### PowerShell simple timing

```powershell
Measure-Command { .\benches\build\bench_end2end.exe }
```

## Docker (Alpine / musl)

Containerized build without host toolchain requirements:

```bash
docker build -f benches/docker/unknown-linux-musl/Dockerfile \
    -t x86_64-linux-musl-pltxt2htm-benches .

# Interactive shell
docker run -it --rm x86_64-linux-musl-pltxt2htm-benches

# Single command
docker run --rm x86_64-linux-musl-pltxt2htm-benches \
    ./benches/build/bench_micro

# With filter and CSV output
docker run --rm x86_64-linux-musl-pltxt2htm-benches \
    ./benches/build/bench_parse --benchmark_filter=RichMarkdown --benchmark_format=csv
```

## Adding a New Benchmark

1. Create `src/bench_<name>.cc` including `<benchmark/benchmark.h>` and required library headers
2. Write standard Google Benchmark functions using `benchmark::State`
3. Register with `BENCHMARK(FuncName)->Arg(N);`
4. End with `BENCHMARK_MAIN();`
5. Rebuild — `CMakeLists.txt` auto-globs `bench_*.cc`
