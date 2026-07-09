#include <pltxt2htm/parser.hh>
#include "bench_fixtures.hh"

BENCHMARK_DEFINE_F(PlainTextParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainTextParseFixture, Parse)->Arg(50)->Arg(200)->Arg(500);

BENCHMARK_DEFINE_F(RichMarkdownParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(RichMarkdownParseFixture, Parse)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_DEFINE_F(PlTagsNestedParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlTagsNestedParseFixture, Parse)->Arg(100)->Arg(500)->Arg(1000);

BENCHMARK_DEFINE_F(PlTagsMixedParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlTagsMixedParseFixture, Parse)->Arg(20)->Arg(50)->Arg(100);

BENCHMARK_DEFINE_F(LatexHeavyParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(LatexHeavyParseFixture, Parse)->Arg(50)->Arg(100)->Arg(200);

BENCHMARK_DEFINE_F(StressLongLineParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(StressLongLineParseFixture, Parse)->Arg(10000)->Arg(50000)->Arg(100000)->Iterations(3);

BENCHMARK_DEFINE_F(StressManyLinesParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(StressManyLinesParseFixture, Parse)->Arg(5000)->Arg(10000);

BENCHMARK_DEFINE_F(RedundantColorParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(RedundantColorParseFixture, Parse)->Arg(200);

BENCHMARK_DEFINE_F(AdjacentTextParseFixture, Parse)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
        ::benchmark::DoNotOptimize(ast);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(AdjacentTextParseFixture, Parse)->Arg(500);

BENCHMARK_MAIN();
