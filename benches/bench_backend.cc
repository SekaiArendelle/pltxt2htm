#include "bench_harness.hh"
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>
#include <pltxt2htm/details/backend/for_plweb_text.hh>
#include <pltxt2htm/details/backend/for_plweb_title.hh>
#include <pltxt2htm/details/backend/for_plunity_text.hh>

using namespace pltxt2htm_bench;
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

namespace {

struct PrebuiltAst {
    ::fast_io::u8string_view name;
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t input_bytes;
};

PrebuiltAst make_ast_plain_text(::std::size_t length = 50000) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < length / 40; ++i) {
        text.append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing.\n");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    return {u8"plain_text_50k", ::std::move(ast), text.size()};
}

PrebuiltAst make_ast_rich_html(::std::size_t repeat = 100) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(
            u8R"(<p>Paragraph with <em>emphasis</em> and <strong>bold</strong>.</p>
<h1>Heading</h1>
<ul><li>item 1</li><li>item 2</li></ul>
<table><tr><td>cell 1</td><td>cell 2</td></tr></table>
<a href="https://example.com">link</a>
<img src="image.png" alt="desc"/>
)");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    return {u8"rich_html_100x", ::std::move(ast), text.size()};
}

PrebuiltAst make_ast_pl_tags(::std::size_t repeat = 200) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(
            u8R"(<color=red><b>bold text</b></color>
<experiment=123>exp</experiment>
<discussion=456>disc</discussion>
<user=bob>user</user>
<size=20>big</size>
<external=url>ext</external>
)");
    }
    auto sv = ::fast_io::u8string_view{text.data(), text.size()};
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    return {u8"pl_tags_200x", ::std::move(ast), text.size()};
}

PrebuiltAst make_ast_markdown(::std::size_t repeat = 50) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(
            u8R"(# Header
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
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
    return {u8"markdown_50x", ::std::move(ast), text.size()};
}

} // anonymous namespace

int main() {
    ::fast_io::io::print(::fast_io::u8out(), u8"=== bench_backend: Backend Rendering Performance ===\n\n");

    auto asts = ::std::vector<PrebuiltAst>{};
    asts.push_back(make_ast_plain_text(50000));
    asts.push_back(make_ast_rich_html(100));
    asts.push_back(make_ast_pl_tags(200));
    asts.push_back(make_ast_markdown(50));

    print_csv_header();

    for (auto const& prebuilt : asts) {
        ::fast_io::io::print(::fast_io::u8out(), u8"--- Input: ", prebuilt.name, u8" (", prebuilt.ast.size(),
                             u8" nodes, ", prebuilt.input_bytes, u8" bytes) ---\n");

        {
            auto ast_copy = prebuilt.ast;
            auto res = benchmark([&] { return ::pltxt2htm::details::plweb_title_backend<ndebug>(ast_copy); },
                                 prebuilt.input_bytes, 0, 5, 20);
            print_result(prebuilt.name, u8"title_backend", res);
            print_result_csv(prebuilt.name, u8"title_backend", res);
        }

        {
            auto ast_copy = prebuilt.ast;
            auto res = benchmark(
                [&] {
                    return ::pltxt2htm::details::plweb_text_backend<ndebug, false>(
                        ast_copy, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
                },
                prebuilt.input_bytes, 0, 5, 20);
            print_result(prebuilt.name, u8"plweb_text_backend", res);
            print_result_csv(prebuilt.name, u8"plweb_text_backend", res);
        }

        {
            auto ast_copy = prebuilt.ast;
            auto res = benchmark(
                [&] {
                    return ::pltxt2htm::details::plunity_text_backend<ndebug>(ast_copy, u8"$PROJECT", u8"$VISITOR",
                                                                              u8"$AUTHOR", u8"$CO_AUTHORS");
                },
                prebuilt.input_bytes, 0, 5, 20);
            print_result(prebuilt.name, u8"plunity_text_backend", res);
            print_result_csv(prebuilt.name, u8"plunity_text_backend", res);
        }

        ::fast_io::io::print(::fast_io::u8out(), u8"\n");
    }

    return 0;
}
