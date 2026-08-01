#include "bench_fixtures.hh"

// -------------------------------------------------------------------
// Micro-benchmarks — no data to prepare, everything created inline.
// Fixture provides ndebug and consistent pattern with other bench files.
// -------------------------------------------------------------------
struct MicroFixture : ::benchmark::Fixture {};

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_U8Char)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::U8Char{u8'A'}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_U8Char);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_Text)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::Text<ndebug>{::std::move(sub)}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_Text);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_PlColor)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_PlColor);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_HtmlSpan)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{
            ::pltxt2htm::HtmlSpan<ndebug>{::std::move(sub), ::fast_io::u8string{u8"color:red;"},
                                          ::exception::optional<::std::size_t>{}, ::pltxt2htm::SizeUnit::px}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_HtmlSpan);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_MdLink)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> text_sub;
        text_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'L'}});
        ::pltxt2htm::Url url{::fast_io::u8string{u8"/"}};
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::MdLink<ndebug>{::std::move(text_sub), ::std::move(url)}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_MdLink);

BENCHMARK_DEFINE_F(MicroFixture, NodeMove_Trivial)(benchmark::State& st) {
    ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::U8Char{u8'A'}};
    for (auto _ : st) {
        ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
        src = ::std::move(dst);
        ::benchmark::DoNotOptimize(src);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeMove_Trivial);

BENCHMARK_DEFINE_F(MicroFixture, NodeMove_WithSubAst)(benchmark::State& st) {
    ::pltxt2htm::Ast<ndebug> nested;
    nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
    nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'B'}});
    ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::Text<ndebug>{::std::move(nested)}};
    for (auto _ : st) {
        ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
        src = ::std::move(dst);
        ::benchmark::DoNotOptimize(src);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeMove_WithSubAst);

BENCHMARK_DEFINE_F(MicroFixture, AstAppend_1000)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> ast;
        for (int i = 0; i < 1000; ++i) {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
        }
        ::benchmark::DoNotOptimize(ast);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, AstAppend_1000);

BENCHMARK_DEFINE_F(MicroFixture, StringAppend_Reserved)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s;
        s.reserve(10000);
        for (int i = 0; i < 100; ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, StringAppend_Reserved);

BENCHMARK_DEFINE_F(MicroFixture, StringAppend_NoReserve)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s;
        for (int i = 0; i < 100; ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, StringAppend_NoReserve);

BENCHMARK_MAIN();