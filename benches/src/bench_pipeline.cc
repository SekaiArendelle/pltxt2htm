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

// Each benchmark parses once, then measures optimize_ast in the loop.
// This reflects real-world usage (parse once, then optimize the parsed AST).

static void BM_Pipeline_PlainText(benchmark::State& state) {
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

static void BM_Pipeline_RedundantColor(benchmark::State& state) {
    auto input = make_redundant_color_nesting(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Pipeline_MixedRedundant(benchmark::State& state) {
    auto input = make_mixed_redundant(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Pipeline_AdjacentText(benchmark::State& state) {
    auto input = make_adjacent_text_nodes(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

static void BM_Pipeline_HtmlSpanAttrs(benchmark::State& state) {
    auto input = make_html_span_attrs(static_cast<::std::size_t>(state.range(0)));
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
    state.SetBytesProcessed(sv.size() * state.iterations());
}

BENCHMARK(BM_Pipeline_PlainText)->Arg(50)->Arg(200)->Arg(500);
BENCHMARK(BM_Pipeline_RedundantColor)->Arg(100)->Arg(200)->Arg(500);
BENCHMARK(BM_Pipeline_MixedRedundant)->Arg(50)->Arg(100)->Arg(200);
BENCHMARK(BM_Pipeline_AdjacentText)->Arg(200)->Arg(500)->Arg(1000);
BENCHMARK(BM_Pipeline_HtmlSpanAttrs)->Arg(100)->Arg(300)->Arg(500);

BENCHMARK_MAIN();
