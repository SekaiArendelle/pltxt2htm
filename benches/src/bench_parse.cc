#include <benchmark/benchmark.h>
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>
#include "bench_inputs.hh"

#ifdef NDEBUG
constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif

static void BM_Parse_PlainText(benchmark::State& state) {
    auto input = make_plain_text(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Optimize_PlainText(benchmark::State& state) {
    auto input = make_plain_text(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_RichMarkdown(benchmark::State& state) {
    auto input = make_rich_markdown(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_PlTagsNested(benchmark::State& state) {
    auto input = make_pl_tags_nested(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_PlTagsMixed(benchmark::State& state) {
    auto input = make_pl_tags_mixed(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_LatexHeavy(benchmark::State& state) {
    auto input = make_latex_heavy(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_StressLongLine(benchmark::State& state) {
    auto input = make_stress_long_line(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Parse_StressManyLines(benchmark::State& state) {
    auto input = make_stress_many_lines(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : state) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        benchmark::DoNotOptimize(ast);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

// Plain text sizes
BENCHMARK(BM_Parse_PlainText)->Arg(50)->Arg(200)->Arg(500);
BENCHMARK(BM_Optimize_PlainText)->Arg(50)->Arg(200)->Arg(500);

// Rich markdown
BENCHMARK(BM_Parse_RichMarkdown)->Arg(10)->Arg(50)->Arg(100);

// PL tags
BENCHMARK(BM_Parse_PlTagsNested)->Arg(100)->Arg(500)->Arg(1000);
BENCHMARK(BM_Parse_PlTagsMixed)->Arg(20)->Arg(50)->Arg(100);

// LaTeX
BENCHMARK(BM_Parse_LatexHeavy)->Arg(50)->Arg(100)->Arg(200);

// Stress tests (reduced iterations for long line)
BENCHMARK(BM_Parse_StressLongLine)->Arg(10000)->Arg(50000)->Arg(100000)->Iterations(3);
BENCHMARK(BM_Parse_StressManyLines)->Arg(5000)->Arg(10000);

BENCHMARK_MAIN();
