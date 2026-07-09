#include "bench_harness.hh"
#include <pltxt2htm/pltxt2htm.hh>

using namespace pltxt2htm_bench;
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

namespace {

::fast_io::u8string make_full_document(::std::size_t repeat = 30) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < repeat; ++i) {
        input.append(
            u8R"(# Chapter Title

This is a paragraph with **bold** and *italic* text.
It also contains `inline code` and a [link](https://example.com).

<color=red>This text is red</color> and <color=blue>this is blue</color>.

## Section with List

- First item with <b>bold</b> formatting
- Second item with <i>italic</i> formatting
  - Nested <color=green>green</color> item
  - Another nested item
- Third item with <external=https://example.com>external link</external>

## Experiment Section

<experiment=1001>Open Experiment 1</experiment>
<discussion=2001>Join Discussion</discussion>
<user=teacher>Teacher profile</user>

## Table

| Name | Value | Description |
|------|-------|-------------|
| Alpha | 1.0   | First value |
| Beta  | 2.5   | Second value |
| Gamma | 3.14  | Third value  |

## Code Block

```python
def hello(name: str) -> str:
    return f"Hello, {name}!"
```

## LaTeX

Inline: $E = mc^2$

Block:
$$
\frac{d}{dx} \int_{a}^{x} f(t)\,dt = f(x)
$$

## Block Quote

> This is a block quote.
> It spans multiple lines.
>
> -- Attribution

---
<size=24>Large text at the end</size>
)");
    }
    return input;
}

::fast_io::u8string make_terse_document(::std::size_t repeat = 200) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < repeat; ++i) {
        input.append(
            u8R"(<color=red>item</color> <b>bold</b> <i>italic</i>
<experiment=1>exp</experiment>
[link](https://x.com)
---
)");
    }
    return input;
}

::fast_io::u8string make_plain_document(::std::size_t chars = 100000) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < chars / 50; ++i) {
        input.append(u8"Plain text line with no special formatting at all.\n");
    }
    return input;
}

} // anonymous namespace

int main() {
    ::fast_io::io::print(::fast_io::u8out(), u8"=== bench_end2end: End-to-End Pipeline ===\n\n");

    auto inputs = ::std::vector<::std::pair<::fast_io::u8string_view, ::fast_io::u8string>>{
        {u8"full_document_30x", make_full_document(30)},
        {u8"terse_document_200x", make_terse_document(200)},
        {u8"plain_document_100k", make_plain_document(100000)},
    };

    print_csv_header();

    for (auto const& inp : inputs) {
        auto sv = ::fast_io::u8string_view{inp.second.data(), inp.second.size()};

        ::fast_io::io::print(::fast_io::u8out(), u8"--- Input: ", inp.first, u8" (", sv.size(), u8" bytes) ---\n");

        {
            auto res = benchmark([&] { return ::pltxt2htm::pltxt2common_html<ndebug>(sv); }, sv.size(), 0, 5, 20);
            print_result(inp.first, u8"common_html (opt=false)", res);
            print_result_csv(inp.first, u8"common_html", res);
        }

        {
            auto res = benchmark([&] { return ::pltxt2htm::pltxt4unittest<ndebug>(sv); }, sv.size(), 0, 5, 20);
            print_result(inp.first, u8"pltxt4unittest (opt=true)", res);
            print_result_csv(inp.first, u8"pltxt4unittest", res);
        }

        {
            auto res = benchmark(
                [&] {
                    return ::pltxt2htm::pltxt2fixedadv_html<ndebug>(sv, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR",
                                                                    u8"$AUTHOR", u8"$CO_AUTHORS");
                },
                sv.size(), 0, 5, 20);
            print_result(inp.first, u8"fixedadv_html (opt=true)", res);
            print_result_csv(inp.first, u8"fixedadv_html", res);
        }

        {
            auto res = benchmark(
                [&] {
                    return ::pltxt2htm::pltxt2plunity_introduction<ndebug>(sv, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR",
                                                                           u8"$CO_AUTHORS");
                },
                sv.size(), 0, 5, 20);
            print_result(inp.first, u8"plunity_introduction (opt=true)", res);
            print_result_csv(inp.first, u8"plunity_introduction", res);
        }

        ::fast_io::io::print(::fast_io::u8out(), u8"\n");
    }

    return 0;
}
