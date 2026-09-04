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
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::Group<ndebug>{::std::move(sub)}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_Text);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_PlColor)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::pltxt2htm::container::U8String{u8"red"}}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_PlColor);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_HtmlSpan)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::HtmlSpan<ndebug>{
            ::std::move(sub), ::pltxt2htm::container::U8String{u8"color:red;"},
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>>{::pltxt2htm::container::nullopt},
            ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>{
                ::pltxt2htm::container::nullopt}}};
        ::benchmark::DoNotOptimize(node);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, NodeCreate_HtmlSpan);

BENCHMARK_DEFINE_F(MicroFixture, NodeCreate_MdLink)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::Ast<ndebug> text_sub;
        text_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'L'}});
        ::pltxt2htm::Url url{::pltxt2htm::container::U8String{u8"/"}};
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
    ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::Group<ndebug>{::std::move(nested)}};
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

BENCHMARK_DEFINE_F(MicroFixture, BasicStringAppend_Reserved)(benchmark::State& st) {
    constexpr ::std::size_t fragment_size{sizeof(u8"<span style=\"color:red;\">text</span>") / sizeof(char8_t) - 1};
    for (auto _ : st) {
        ::pltxt2htm::container::U8String s;
        s.reserve<ndebug>(as_size(st) * fragment_size);
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringAppend_Reserved)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringAppend_Reserved)(benchmark::State& st) {
    constexpr ::std::size_t fragment_size{sizeof(u8"<span style=\"color:red;\">text</span>") / sizeof(char8_t) - 1};
    for (auto _ : st) {
        ::fast_io::u8string s;
        s.reserve(as_size(st) * fragment_size);
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringAppend_Reserved)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_DEFINE_F(MicroFixture, BasicStringAppend_NoReserve)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::container::U8String s;
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringAppend_NoReserve)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringAppend_NoReserve)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s;
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.append(u8"<span style=\"color:red;\">text</span>");
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringAppend_NoReserve)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_DEFINE_F(MicroFixture, BasicStringPushBack_Reserved)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::container::U8String s;
        s.reserve<ndebug>(as_size(st));
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.push_back(u8'x');
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringPushBack_Reserved)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringPushBack_Reserved)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s;
        s.reserve(as_size(st));
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.push_back(u8'x');
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringPushBack_Reserved)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, BasicStringPushBack_NoReserve)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::container::U8String s;
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.push_back(u8'x');
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringPushBack_NoReserve)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringPushBack_NoReserve)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s;
        for (::std::int64_t i = 0; i < st.range(0); ++i) {
            s.push_back(u8'x');
        }
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringPushBack_NoReserve)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, BasicStringAssign)(benchmark::State& st) {
    ::pltxt2htm::container::U8String const source{as_size(st), u8'x'};
    ::pltxt2htm::container::U8String target;
    target.reserve<ndebug>(as_size(st));
    for (auto _ : st) {
        target.assign(::pltxt2htm::container::U8StringView{source});
        ::benchmark::DoNotOptimize(target);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringAssign)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringAssign)(benchmark::State& st) {
    ::fast_io::u8string const source{as_size(st), u8'x'};
    ::fast_io::u8string target;
    target.reserve(as_size(st));
    for (auto _ : st) {
        target.assign(::fast_io::u8string_view{source.data(), source.size()});
        ::benchmark::DoNotOptimize(target);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringAssign)->Arg(1024)->Arg(4096)->Arg(16384);

BENCHMARK_DEFINE_F(MicroFixture, BasicStringZeroConstruct)(benchmark::State& st) {
    for (auto _ : st) {
        ::pltxt2htm::container::U8String s{as_size(st)};
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, BasicStringZeroConstruct)->Arg(1024)->Arg(1 << 20);

BENCHMARK_DEFINE_F(MicroFixture, FastIoStringZeroConstruct)(benchmark::State& st) {
    for (auto _ : st) {
        ::fast_io::u8string s{as_size(st)};
        ::benchmark::DoNotOptimize(s);
    }
}

BENCHMARK_REGISTER_F(MicroFixture, FastIoStringZeroConstruct)->Arg(1024)->Arg(1 << 20);

BENCHMARK_MAIN();
