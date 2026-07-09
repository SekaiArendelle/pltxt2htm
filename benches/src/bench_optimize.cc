#include <pltxt2htm/optimizer.hh>
#include "bench_fixtures.hh"

// -------------------------------------------------------------------
// Fixtures — AST built once in SetUp, measured in BenchmarkCase
// -------------------------------------------------------------------
struct NestedColorOptFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        ast = build_nested_color_ast(as_size(state));
    }
};

struct AdjacentColorOptFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        ast = build_adjacent_color_ast(as_size(state));
    }
};

struct MixedRedundantOptFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        ast = build_mixed_redundant_ast(as_size(state));
    }
};

struct HtmlSpanOptFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        ast = build_html_span_ast(as_size(state));
    }
};

// -------------------------------------------------------------------
// Benchmarks
// -------------------------------------------------------------------
BENCHMARK_DEFINE_F(NestedColorOptFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK_REGISTER_F(NestedColorOptFixture, Optimize)->Arg(100)->Arg(200)->Arg(500);

BENCHMARK_DEFINE_F(AdjacentColorOptFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK_REGISTER_F(AdjacentColorOptFixture, Optimize)->Arg(200)->Arg(500)->Arg(1000);

BENCHMARK_DEFINE_F(MixedRedundantOptFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK_REGISTER_F(MixedRedundantOptFixture, Optimize)->Arg(50)->Arg(100)->Arg(200);

BENCHMARK_DEFINE_F(HtmlSpanOptFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK_REGISTER_F(HtmlSpanOptFixture, Optimize)->Arg(100)->Arg(300)->Arg(500);

BENCHMARK_MAIN();