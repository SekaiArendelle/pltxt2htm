#include <pltxt2htm/parser.hh>
#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include <pltxt2htm/details/backend/for_plweb_title.hh>
#include <pltxt2htm/details/backend/for_plunity_text.hh>
#include "bench_fixtures.hh"

// -------------------------------------------------------------------
// Backend fixtures — custom data generators (not from bench_inputs.hh)
// -------------------------------------------------------------------
struct PlainTextBackendFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;
    void SetUp(::benchmark::State& state) override {
        ::fast_io::u8string text;
        auto n = as_size(state);
        for (::std::size_t i = 0; i < n / 40; ++i)
            text.append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing.\n");
        input_bytes = text.size();
        auto sv = ::fast_io::u8string_view{text.data(), text.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct RichHtmlBackendFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;
    void SetUp(::benchmark::State& state) override {
        ::fast_io::u8string text;
        auto n = as_size(state);
        for (::std::size_t i = 0; i < n; ++i)
            text.append(u8R"(<p>Paragraph with <em>emphasis</em> and <strong>bold</strong>.</p>
<h1>Heading</h1>
<ul><li>item 1</li><li>item 2</li></ul>
<table><tr><td>cell 1</td><td>cell 2</td></tr></table>
<a href="https://example.com">link</a>
<img src="image.png" alt="desc"/>
)");
        input_bytes = text.size();
        auto sv = ::fast_io::u8string_view{text.data(), text.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct PlTagsBackendFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;
    void SetUp(::benchmark::State& state) override {
        ::fast_io::u8string text;
        auto n = as_size(state);
        for (::std::size_t i = 0; i < n; ++i)
            text.append(u8R"(<color=red><b>bold text</b></color>
<experiment=123>exp</experiment>
<discussion=456>disc</discussion>
<user=bob>user</user>
<size=20>big</size>
<external=url>ext</external>
)");
        input_bytes = text.size();
        auto sv = ::fast_io::u8string_view{text.data(), text.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

struct MarkdownBackendFixture : ::benchmark::Fixture {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;
    void SetUp(::benchmark::State& state) override {
        ::fast_io::u8string text;
        auto n = as_size(state);
        for (::std::size_t i = 0; i < n; ++i)
            text.append(u8R"(# Header
**bold** and *italic*
- list item
- another item

| A | B |
|---|---|
| 1 | 2 |

> blockquote

```code
int x = 1;
```
)");
        input_bytes = text.size();
        auto sv = ::fast_io::u8string_view{text.data(), text.size()};
        ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    }
};

// -------------------------------------------------------------------
// plweb_title_backend
// -------------------------------------------------------------------
BENCHMARK_DEFINE_F(PlainTextBackendFixture, Title)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlainTextBackendFixture, Title)->Arg(50000);

BENCHMARK_DEFINE_F(RichHtmlBackendFixture, Title)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(RichHtmlBackendFixture, Title)->Arg(100);

BENCHMARK_DEFINE_F(PlTagsBackendFixture, Title)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlTagsBackendFixture, Title)->Arg(200);

BENCHMARK_DEFINE_F(MarkdownBackendFixture, Title)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(MarkdownBackendFixture, Title)->Arg(50);

// -------------------------------------------------------------------
// plweb_text_backend
// -------------------------------------------------------------------
BENCHMARK_DEFINE_F(PlainTextBackendFixture, PlwebText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlainTextBackendFixture, PlwebText)->Arg(50000);

BENCHMARK_DEFINE_F(RichHtmlBackendFixture, PlwebText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(RichHtmlBackendFixture, PlwebText)->Arg(100);

BENCHMARK_DEFINE_F(PlTagsBackendFixture, PlwebText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlTagsBackendFixture, PlwebText)->Arg(200);

BENCHMARK_DEFINE_F(MarkdownBackendFixture, PlwebText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(MarkdownBackendFixture, PlwebText)->Arg(50);

// -------------------------------------------------------------------
// plunity_text_backend
// -------------------------------------------------------------------
BENCHMARK_DEFINE_F(PlainTextBackendFixture, PlunityText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlainTextBackendFixture, PlunityText)->Arg(50000);

BENCHMARK_DEFINE_F(RichHtmlBackendFixture, PlunityText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(RichHtmlBackendFixture, PlunityText)->Arg(100);

BENCHMARK_DEFINE_F(PlTagsBackendFixture, PlunityText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(PlTagsBackendFixture, PlunityText)->Arg(200);

BENCHMARK_DEFINE_F(MarkdownBackendFixture, PlunityText)(benchmark::State& st) {
    for (auto _ : st) {
        auto copy = ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        ::benchmark::DoNotOptimize(result);
    }
    st.SetBytesProcessed(input_bytes * st.iterations());
}
BENCHMARK_REGISTER_F(MarkdownBackendFixture, PlunityText)->Arg(50);

BENCHMARK_MAIN();
