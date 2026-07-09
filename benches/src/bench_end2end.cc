#include <pltxt2htm/pltxt2htm.hh>
#include "bench_fixtures.hh"

// Full document (~32KB)
BENCHMARK_DEFINE_F(FullDocE2EFixture, CommonHtml)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2common_html<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(FullDocE2EFixture, CommonHtml)->Arg(30);

BENCHMARK_DEFINE_F(FullDocE2EFixture, UnitTest)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt4unittest<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(FullDocE2EFixture, UnitTest)->Arg(30);

BENCHMARK_DEFINE_F(FullDocE2EFixture, FixedAdv)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
            sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(FullDocE2EFixture, FixedAdv)->Arg(30);

BENCHMARK_DEFINE_F(FullDocE2EFixture, Plunity)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
            sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(FullDocE2EFixture, Plunity)->Arg(30);

// Terse document (~21KB)
BENCHMARK_DEFINE_F(TerseDocE2EFixture, CommonHtml)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2common_html<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(TerseDocE2EFixture, CommonHtml)->Arg(200);

BENCHMARK_DEFINE_F(TerseDocE2EFixture, UnitTest)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt4unittest<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(TerseDocE2EFixture, UnitTest)->Arg(200);

BENCHMARK_DEFINE_F(TerseDocE2EFixture, FixedAdv)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
            sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(TerseDocE2EFixture, FixedAdv)->Arg(200);

BENCHMARK_DEFINE_F(TerseDocE2EFixture, Plunity)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
            sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(TerseDocE2EFixture, Plunity)->Arg(200);

// Plain document (100KB)
BENCHMARK_DEFINE_F(PlainDocE2EFixture, CommonHtml)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2common_html<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainDocE2EFixture, CommonHtml)->Arg(100000);

BENCHMARK_DEFINE_F(PlainDocE2EFixture, UnitTest)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt4unittest<ndebug>(sv);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainDocE2EFixture, UnitTest)->Arg(100000);

BENCHMARK_DEFINE_F(PlainDocE2EFixture, FixedAdv)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
            sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainDocE2EFixture, FixedAdv)->Arg(100000);

BENCHMARK_DEFINE_F(PlainDocE2EFixture, Plunity)(benchmark::State& st) {
    auto sv = ::fast_io::u8string_view{input.data(), input.size()};
    for (auto _ : st) {
        auto result = ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
            sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(sv.size() * st.iterations());
}
BENCHMARK_REGISTER_F(PlainDocE2EFixture, Plunity)->Arg(100000);

BENCHMARK_MAIN();
