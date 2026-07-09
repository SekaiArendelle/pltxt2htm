#include <benchmark/benchmark.h>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/ast/node_kind.hh>
#include <pltxt2htm/contracts.hh>

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

static void BM_NodeCreate_U8Char(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::U8Char{u8'A'}};
        benchmark::DoNotOptimize(node);
    }
}

static void BM_NodeCreate_Text(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::Text<ndebug>{::std::move(sub)}};
        benchmark::DoNotOptimize(node);
    }
}

static void BM_NodeCreate_PlColor(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
        benchmark::DoNotOptimize(node);
    }
}

static void BM_NodeCreate_HtmlSpan(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::HtmlSpan<ndebug>{
            ::std::move(sub), ::fast_io::u8string{u8"color:red;"}, ::exception::optional<::std::size_t>{}}};
        benchmark::DoNotOptimize(node);
    }
}

static void BM_NodeCreate_MdLink(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::Ast<ndebug> text_sub, url_sub;
        text_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'L'}});
        url_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'/'}});
        ::pltxt2htm::Url<ndebug> url{::std::move(url_sub)};
        ::pltxt2htm::PlTxtNode<ndebug> node{
            ::pltxt2htm::MdLink<ndebug>{::std::move(text_sub), ::std::move(url)}};
        benchmark::DoNotOptimize(node);
    }
}

static void BM_NodeMove_Trivial(benchmark::State& state) {
    ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::U8Char{u8'A'}};
    for (auto _ : state) {
        ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
        src = ::std::move(dst);
        benchmark::DoNotOptimize(src);
    }
}

static void BM_NodeMove_WithSubAst(benchmark::State& state) {
    ::pltxt2htm::Ast<ndebug> nested;
    nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
    nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'B'}});
    ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::Text<ndebug>{::std::move(nested)}};
    for (auto _ : state) {
        ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
        src = ::std::move(dst);
        benchmark::DoNotOptimize(src);
    }
}

static void BM_AstAppend_1000(benchmark::State& state) {
    for (auto _ : state) {
        ::pltxt2htm::Ast<ndebug> ast;
        for (int i = 0; i < 1000; ++i) {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
        }
        benchmark::DoNotOptimize(ast);
    }
}

static void BM_StringAppend_Reserved(benchmark::State& state) {
    for (auto _ : state) {
        ::fast_io::u8string s;
        s.reserve(10000);
        for (int i = 0; i < 100; ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        benchmark::DoNotOptimize(s);
    }
}

static void BM_StringAppend_NoReserve(benchmark::State& state) {
    for (auto _ : state) {
        ::fast_io::u8string s;
        for (int i = 0; i < 100; ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(BM_NodeCreate_U8Char);
BENCHMARK(BM_NodeCreate_Text);
BENCHMARK(BM_NodeCreate_PlColor);
BENCHMARK(BM_NodeCreate_HtmlSpan);
BENCHMARK(BM_NodeCreate_MdLink);

BENCHMARK(BM_NodeMove_Trivial);
BENCHMARK(BM_NodeMove_WithSubAst);

BENCHMARK(BM_AstAppend_1000);

BENCHMARK(BM_StringAppend_Reserved);
BENCHMARK(BM_StringAppend_NoReserve);

BENCHMARK_MAIN();
