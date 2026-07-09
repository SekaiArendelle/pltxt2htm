#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include "bench_fixtures.hh"

// -------------------------------------------------------------------
// Fixtures — direct AST construction (no parse overhead)
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
// Fixtures — parse-once then optimize (real-world pipeline)
// -------------------------------------------------------------------
struct PlainTextPipelineFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        input = make_plain_text(as_size(state));
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct RedundantColorPipelineFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        input = make_redundant_color_nesting(as_size(state));
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct MixedRedundantPipelineFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        input = make_mixed_redundant(as_size(state));
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct AdjacentTextPipelineFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        input = make_adjacent_text_nodes(as_size(state));
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct HtmlSpanAttrsPipelineFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;
    ::pltxt2htm::Ast<ndebug> ast;
    void SetUp(::benchmark::State& state) override {
        input = make_html_span_attrs(as_size(state));
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

// -------------------------------------------------------------------
// Pure optimizer benchmarks (direct AST construction)
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

// -------------------------------------------------------------------
// Pipeline benchmarks (parse once, optimize in loop)
// -------------------------------------------------------------------
BENCHMARK_DEFINE_F(PlainTextPipelineFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
    st.SetBytesProcessed(input.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainTextPipelineFixture, Optimize)->Arg(50)->Arg(200)->Arg(500);

BENCHMARK_DEFINE_F(RedundantColorPipelineFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
    st.SetBytesProcessed(input.size() * st.iterations());
}
BENCHMARK_REGISTER_F(RedundantColorPipelineFixture, Optimize)->Arg(100)->Arg(200)->Arg(500);

BENCHMARK_DEFINE_F(MixedRedundantPipelineFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
    st.SetBytesProcessed(input.size() * st.iterations());
}
BENCHMARK_REGISTER_F(MixedRedundantPipelineFixture, Optimize)->Arg(50)->Arg(100)->Arg(200);

BENCHMARK_DEFINE_F(AdjacentTextPipelineFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
    st.SetBytesProcessed(input.size() * st.iterations());
}
BENCHMARK_REGISTER_F(AdjacentTextPipelineFixture, Optimize)->Arg(200)->Arg(500)->Arg(1000);

BENCHMARK_DEFINE_F(HtmlSpanAttrsPipelineFixture, Optimize)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        ::benchmark::DoNotOptimize(copy);
    }
    st.SetBytesProcessed(input.size() * st.iterations());
}
BENCHMARK_REGISTER_F(HtmlSpanAttrsPipelineFixture, Optimize)->Arg(100)->Arg(300)->Arg(500);

BENCHMARK_MAIN();