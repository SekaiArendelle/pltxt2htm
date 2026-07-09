#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include "bench_fixtures.hh"

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
