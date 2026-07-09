#include <benchmark/benchmark.h>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>

#ifdef NDEBUG
constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif

// --- AST builder helpers ---
// Construct ASTs directly without parsing, so benchmarks measure
// optimize_ast in isolation with zero parse overhead.

// Build: <color=red>(<color=red>(<color=red>(...text...)))
static auto build_nested_color_ast(::std::size_t depth) {
    ::pltxt2htm::PlTxtNode<ndebug> inner{::pltxt2htm::U8Char{u8't'}};
    for (::std::size_t i = 0; i < depth; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::std::move(inner));
        inner = ::pltxt2htm::PlTxtNode<ndebug>{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
    }
    ::pltxt2htm::Ast<ndebug> ast;
    ast.push_back(::std::move(inner));
    return ast;
}

// Build: <color=red>t</color><color=red>t</color><color=red>t</color>...
static auto build_adjacent_color_ast(::std::size_t count) {
    ::pltxt2htm::Ast<ndebug> ast;
    for (::std::size_t i = 0; i < count; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}});
    }
    return ast;
}

// Build: <color=red><b><i>t</i></b></color> repeated depth times
static auto build_mixed_redundant_ast(::std::size_t depth) {
    ::pltxt2htm::PlTxtNode<ndebug> inner{::pltxt2htm::U8Char{u8't'}};
    for (::std::size_t i = 0; i < depth; ++i) {
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{
                ::pltxt2htm::PlI<ndebug>{::std::move(sub)}};
        }
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{
                ::pltxt2htm::PlB<ndebug>{::std::move(sub)}};
        }
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{
                ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
        }
    }
    ::pltxt2htm::Ast<ndebug> ast;
    ast.push_back(::std::move(inner));
    return ast;
}

// Build: repeated <span style="...">t</span>
static auto build_html_span_ast(::std::size_t count) {
    ::pltxt2htm::Ast<ndebug> ast;
    for (::std::size_t i = 0; i < count; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::HtmlSpan<ndebug>{
            ::std::move(sub), ::fast_io::u8string{u8"color:red;font-size:16px"},
            ::exception::optional<::std::size_t>{}}});
    }
    return ast;
}

// --- Benchmarks ---

static void BM_Optimize_NestedColor(benchmark::State& state) {
    auto ast = build_nested_color_ast(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_Optimize_AdjacentColor(benchmark::State& state) {
    auto ast = build_adjacent_color_ast(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_Optimize_MixedRedundant(benchmark::State& state) {
    auto ast = build_mixed_redundant_ast(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_Optimize_HtmlSpan(benchmark::State& state) {
    auto ast = build_html_span_ast(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = ast;
        ::pltxt2htm::optimize_ast<ndebug>(copy);
        benchmark::DoNotOptimize(copy);
    }
}

BENCHMARK(BM_Optimize_NestedColor)->Arg(100)->Arg(200)->Arg(500);
BENCHMARK(BM_Optimize_AdjacentColor)->Arg(200)->Arg(500)->Arg(1000);
BENCHMARK(BM_Optimize_MixedRedundant)->Arg(50)->Arg(100)->Arg(200);
BENCHMARK(BM_Optimize_HtmlSpan)->Arg(100)->Arg(300)->Arg(500);

BENCHMARK_MAIN();
