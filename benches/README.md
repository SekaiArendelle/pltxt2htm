# pltxt2htm Benchmarks

Microbenchmark suite for quantitative performance analysis of `pltxt2htm`.

## Prerequisites

- C++23 compiler (Clang/GCC/MSVC)
- CMake >= 3.20
- Ninja (recommended) or other build system

## Build

```sh
cmake -S benches -B benches/build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build benches/build
```

This produces Release-optimized executables (`-O3 -flto` on GCC/Clang, `/O2 /GL /LTCG` on MSVC).

## Benchmarks

| Executable | Measures | Inputs |
|-----------|----------|--------|
| `bench_parse` | `parse_pltxt` + `optimize_ast` individually | plain text, rich markdown, PL tags (nested/mixed), LaTeX, stress (long line, many lines) |
| `bench_optimize` | `optimize_ast` vs `parse_pltxt` | redundant color nesting, mixed formatting, adjacent text, HTML spans |
| `bench_backend` | 3 backends: `plweb_title`, `plweb_text`, `plunity_text` | plain text, rich HTML, PL tags, markdown ASTs |
| `bench_end2end` | All 4 public APIs (`common_html`, `pltxt4unittest`, `fixedadv_html`, `plunity_introduction`) | full document, terse document, plain document |
| `bench_micro` | Micro-operations (node creation/move, AST append, string ops) | N/A |

## Run

```sh
# Run a single benchmark
./benches/build/bench_parse.exe

# Run all benchmarks
./benches/build/bench_parse.exe
./benches/build/bench_optimize.exe
./benches/build/bench_backend.exe
./benches/build/bench_end2end.exe
./benches/build/bench_micro.exe
```

Each benchmark outputs both human-readable tables and CSV lines (prefixed with header row `bench,config,...`).

## Profiling

### Simple timing (PowerShell)

```powershell
Measure-Command { .\benches\build\bench_end2end.exe }
```

Or use the built-in script:

```powershell
.\benches\profile.ps1 -Mode simple -Binary .\benches\build\bench_parse.exe -Iterations 10
```

### CPU hotspot analysis (Windows Performance Recorder)

Requires [Windows Performance Toolkit](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/) (part of Windows ADK).

```powershell
# Run as Administrator
.\benches\profile.ps1 -Mode wpr -Binary .\benches\build\bench_parse.exe
```

Produces a `.etl` file openable with Windows Performance Analyzer (WPA).

### ETW kernel tracing (xperf)

```powershell
.\benches\profile.ps1 -Mode etw -Binary .\benches\build\bench_end2end.exe
```

## Output Format

Human-readable:

```
  plain_text [parse (quick_enforce)]
    Iterations: 20
    Input:      38400 bytes
    Throughput: 11.3 MB/s
    Median:     3115 us
    Min:        2565 us
    Max:        4751 us
```

CSV (for plotting or further analysis):

```
bench,config,iterations,input_bytes,output_bytes,total_ms,median_us,min_us,max_us,throughput_mbs
plain_text,parse,20,38400,0,72.73,3351,2781,5246,10.07
```

## Adding a New Benchmark

1. Create `bench_<name>.cc` that includes `"bench_harness.hh"` and the required library headers
2. Use `pltxt2htm_bench::benchmark()` (for functions returning a value) or `benchmark_void()`
3. Call `print_result()` / `print_result_csv()` to display results
4. Rebuild — `CMakeLists.txt` auto-globs `bench_*.cc` files
