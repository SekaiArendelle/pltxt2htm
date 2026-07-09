#include <benchmark/benchmark.h>
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>
#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include <pltxt2htm/details/backend/for_plweb_title.hh>
#include <pltxt2htm/details/backend/for_plunity_text.hh>
#include "bench_inputs.hh"

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

struct AstFixture {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;

    explicit AstFixture(::pltxt2htm::Ast<ndebug>&& a, ::std::size_t b)
        : ast{::std::move(a)}, input_bytes{b} {}
};

static AstFixture prepare_ast_plain_text(::std::size_t length) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < length / 40; ++i) {
        text.append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing.\n");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    return AstFixture{::pltxt2htm::parse_pltxt<ndebug>(sv), text.size()};
}

static AstFixture prepare_ast_rich_html(::std::size_t repeat) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(u8R"(<p>Paragraph with <em>emphasis</em> and <strong>bold</strong>.</p>
<h1>Heading</h1>
<ul><li>item 1</li><li>item 2</li></ul>
<table><tr><td>cell 1</td><td>cell 2</td></tr></table>
<a href="https://example.com">link</a>
<img src="image.png" alt="desc"/>
)");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    return AstFixture{::pltxt2htm::parse_pltxt<ndebug>(sv), text.size()};
}

static AstFixture prepare_ast_pl_tags(::std::size_t repeat) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(u8R"(<color=red><b>bold text</b></color>
<experiment=123>exp</experiment>
<discussion=456>disc</discussion>
<user=bob>user</user>
<size=20>big</size>
<external=url>ext</external>
)");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    return AstFixture{::pltxt2htm::parse_pltxt<ndebug>(sv), text.size()};
}

static AstFixture prepare_ast_markdown(::std::size_t repeat) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
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
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    return AstFixture{::pltxt2htm::parse_pltxt<ndebug>(sv), text.size()};
}

// title_backend

static void BM_Title_PlainText(benchmark::State& state) {
    auto f = prepare_ast_plain_text(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Title_RichHtml(benchmark::State& state) {
    auto f = prepare_ast_rich_html(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Title_PlTags(benchmark::State& state) {
    auto f = prepare_ast_pl_tags(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Title_Markdown(benchmark::State& state) {
    auto f = prepare_ast_markdown(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_title_backend<ndebug>(copy);
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

// plweb_text_backend

static void BM_PlwebText_PlainText(benchmark::State& state) {
    auto f = prepare_ast_plain_text(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_PlwebText_RichHtml(benchmark::State& state) {
    auto f = prepare_ast_rich_html(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_PlwebText_PlTags(benchmark::State& state) {
    auto f = prepare_ast_pl_tags(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_PlwebText_Markdown(benchmark::State& state) {
    auto f = prepare_ast_markdown(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
            copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

// plunity_text_backend

static void BM_Plunity_PlainText(benchmark::State& state) {
    auto f = prepare_ast_plain_text(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Plunity_RichHtml(benchmark::State& state) {
    auto f = prepare_ast_rich_html(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Plunity_PlTags(benchmark::State& state) {
    auto f = prepare_ast_pl_tags(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

static void BM_Plunity_Markdown(benchmark::State& state) {
    auto f = prepare_ast_markdown(static_cast<::std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto copy = f.ast;
        auto result = ::pltxt2htm::details::plunity_text_backend<ndebug>(
            copy, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(f.input_bytes * state.iterations());
}

BENCHMARK(BM_Title_PlainText)->Arg(50000);
BENCHMARK(BM_Title_RichHtml)->Arg(100);
BENCHMARK(BM_Title_PlTags)->Arg(200);
BENCHMARK(BM_Title_Markdown)->Arg(50);

BENCHMARK(BM_PlwebText_PlainText)->Arg(50000);
BENCHMARK(BM_PlwebText_RichHtml)->Arg(100);
BENCHMARK(BM_PlwebText_PlTags)->Arg(200);
BENCHMARK(BM_PlwebText_Markdown)->Arg(50);

BENCHMARK(BM_Plunity_PlainText)->Arg(50000);
BENCHMARK(BM_Plunity_RichHtml)->Arg(100);
BENCHMARK(BM_Plunity_PlTags)->Arg(200);
BENCHMARK(BM_Plunity_Markdown)->Arg(50);

BENCHMARK_MAIN();
