#include "bench_harness.hh"
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>

using namespace pltxt2htm_bench;
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

namespace {

::fast_io::u8string make_plain_text(::std::size_t repeat = 200) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n");
        text.append(u8"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n");
        text.append(u8"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris.\n");
    }
    return text;
}

::fast_io::u8string make_rich_markdown(::std::size_t repeat = 50) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(
            u8R"(# Heading 1
## Heading 2
### Heading 3

- List item 1
- List item 2
  - Nested item
  - Another nested
- List item 3

1. Ordered item 1
2. Ordered item 2
3. Ordered item 3

**bold text** and *italic text* and ***bold italic***
~~strikethrough~~

[link text](https://example.com)
![image alt](https://example.com/img.png)

| Col1 | Col2 | Col3 |
|------|------|------|
| A1   | B1   | C1   |
| A2   | B2   | C2   |

`inline code`

```cpp
auto x = 42;
std::cout << x;
```

> Block quote line 1
> Block quote line 2

---
)");
    }
    return text;
}

::fast_io::u8string make_pl_tags_nested(::std::size_t depth = 500) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"<color=red>");
    }
    input.append(u8"deeply nested text");
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"</color>");
    }
    return input;
}

::fast_io::u8string make_pl_tags_mixed(::std::size_t repeat = 50) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < repeat; ++i) {
        input.append(
            u8R"(<color=red><b>bold text</b> and <i>italic</i></color>
<experiment=12345>experiment link</experiment>
<discussion=67890>discussion link</discussion>
<user=alice>user link</user>
<size=16>large text</size>
<external=https://example.com>external link</external>
<color=#3366CC><b><i>nested formatting</i></b></color>
)");
    }
    return input;
}

::fast_io::u8string make_latex_heavy(::std::size_t repeat = 100) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < repeat; ++i) {
        input.append(
            u8R"(Inline equation: $E = mc^2$
Block equation:
$$
\int_{-\infty}^{\infty} e^{-x^2}\,dx = \sqrt{\pi}
$$
Mixed: $\alpha + \beta = \gamma$ and $\sum_{i=1}^{n} i = \frac{n(n+1)}{2}$
)");
    }
    return input;
}

::fast_io::u8string make_stress_long_line(::std::size_t length = 100000) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < length; ++i) {
        input.push_back(u8'a' + static_cast<char8_t>(i % 26));
    }
    return input;
}

::fast_io::u8string make_stress_many_lines(::std::size_t lines = 10000) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < lines; ++i) {
        input.append(u8"line 1234567890\n");
    }
    return input;
}

} // anonymous namespace

int main() {
    ::fast_io::io::print(::fast_io::u8out(), u8"=== bench_parse: Parser Performance ===\n\n");

    auto inputs = ::std::vector<::std::pair<::fast_io::u8string_view, ::fast_io::u8string>>{
        {u8"plain_text", make_plain_text(200)},
        {u8"rich_markdown", make_rich_markdown(50)},
        {u8"pl_tags_nested_500", make_pl_tags_nested(500)},
        {u8"pl_tags_mixed_50x", make_pl_tags_mixed(50)},
        {u8"latex_heavy_100x", make_latex_heavy(100)},
        {u8"stress_long_line", make_stress_long_line(100000)},
        {u8"stress_many_lines", make_stress_many_lines(10000)},
    };

    bool first_csv = true;
    for (auto const& inp : inputs) {
        auto sv = ::fast_io::u8string_view{inp.second.data(), inp.second.size()};

        int parse_iter = (inp.first == u8"stress_long_line") ? 3 : 20;
        int opt_iter = parse_iter;

        {
            auto res = benchmark([&] { return ::pltxt2htm::parse_pltxt<ndebug>(sv); }, sv.size(), 0, 2, parse_iter);
            print_result(inp.first, u8"parse (quick_enforce)", res);
        }

        {
            auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
            auto res = benchmark_void([&] { ::pltxt2htm::optimize_ast<ndebug>(ast); }, sv.size(), 2, opt_iter);
            print_result(inp.first, u8"optimize (quick_enforce)", res);
        }

        if (first_csv) {
            print_csv_header();
            first_csv = false;
        }
        {
            auto res = benchmark([&] { return ::pltxt2htm::parse_pltxt<ndebug>(sv); }, sv.size(), 0, 2, parse_iter);
            print_result_csv(inp.first, u8"parse", res);
        }
        {
            auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);
            auto res = benchmark_void([&] { ::pltxt2htm::optimize_ast<ndebug>(ast); }, sv.size(), 2, opt_iter);
            print_result_csv(inp.first, u8"optimize", res);
        }
    }

    return 0;
}
