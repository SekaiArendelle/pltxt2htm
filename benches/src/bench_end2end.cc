#include <benchmark/benchmark.h>
#include <pltxt2htm/pltxt2htm.hh>
#include "bench_inputs.hh"

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

#define DEFINE_E2E_BENCH(Name, DocGen, ApiCall)                          \
    static void Name(benchmark::State& state) {                          \
        auto input = DocGen(static_cast<::std::size_t>(state.range(0))); \
        auto sv = ::fast_io::u8string_view{input.data(), input.size()};  \
        for (auto _ : state) {                                           \
            auto result = ApiCall;                                       \
            benchmark::DoNotOptimize(result);                            \
        }                                                                \
        state.SetBytesProcessed(sv.size() * state.iterations());         \
    }

DEFINE_E2E_BENCH(BM_CommonHtml_Full,   make_full_document,
                 ::pltxt2htm::pltxt2common_html<ndebug>(sv))
DEFINE_E2E_BENCH(BM_CommonHtml_Terse,  make_terse_document,
                 ::pltxt2htm::pltxt2common_html<ndebug>(sv))
DEFINE_E2E_BENCH(BM_CommonHtml_Plain,  make_plain_document,
                 ::pltxt2htm::pltxt2common_html<ndebug>(sv))

DEFINE_E2E_BENCH(BM_UnitTest_Full,     make_full_document,
                 ::pltxt2htm::pltxt4unittest<ndebug>(sv))
DEFINE_E2E_BENCH(BM_UnitTest_Terse,    make_terse_document,
                 ::pltxt2htm::pltxt4unittest<ndebug>(sv))
DEFINE_E2E_BENCH(BM_UnitTest_Plain,    make_plain_document,
                 ::pltxt2htm::pltxt4unittest<ndebug>(sv))

DEFINE_E2E_BENCH(BM_FixedAdv_Full,     make_full_document,
                 ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
                     sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))
DEFINE_E2E_BENCH(BM_FixedAdv_Terse,    make_terse_document,
                 ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
                     sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))
DEFINE_E2E_BENCH(BM_FixedAdv_Plain,    make_plain_document,
                 ::pltxt2htm::pltxt2fixedadv_html<ndebug>(
                     sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))

DEFINE_E2E_BENCH(BM_Plunity_Full,      make_full_document,
                 ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
                     sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))
DEFINE_E2E_BENCH(BM_Plunity_Terse,     make_terse_document,
                 ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
                     sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))
DEFINE_E2E_BENCH(BM_Plunity_Plain,     make_plain_document,
                 ::pltxt2htm::pltxt2plunity_introduction<ndebug>(
                     sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS"))

// Full document: 30 repeats ~32KB
BENCHMARK(BM_CommonHtml_Full)->Arg(30);
BENCHMARK(BM_UnitTest_Full)->Arg(30);
BENCHMARK(BM_FixedAdv_Full)->Arg(30);
BENCHMARK(BM_Plunity_Full)->Arg(30);

// Terse document: 200 repeats ~21KB
BENCHMARK(BM_CommonHtml_Terse)->Arg(200);
BENCHMARK(BM_UnitTest_Terse)->Arg(200);
BENCHMARK(BM_FixedAdv_Terse)->Arg(200);
BENCHMARK(BM_Plunity_Terse)->Arg(200);

// Plain document: 100KB
BENCHMARK(BM_CommonHtml_Plain)->Arg(100000);
BENCHMARK(BM_UnitTest_Plain)->Arg(100000);
BENCHMARK(BM_FixedAdv_Plain)->Arg(100000);
BENCHMARK(BM_Plunity_Plain)->Arg(100000);

BENCHMARK_MAIN();
