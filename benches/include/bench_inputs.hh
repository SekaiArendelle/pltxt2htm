#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>

struct BenchInput {
    ::fast_io::u8string data;
    ::std::size_t bytes;
};

inline ::fast_io::u8string make_plain_text(::std::size_t repeat) {
    ::fast_io::u8string text;
    for (::std::size_t i = 0; i < repeat; ++i) {
        text.append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n");
        text.append(u8"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n");
        text.append(u8"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris.\n");
    }
    return text;
}

inline ::fast_io::u8string make_rich_markdown(::std::size_t repeat) {
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

inline ::fast_io::u8string make_pl_tags_nested(::std::size_t depth) {
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

inline ::fast_io::u8string make_pl_tags_mixed(::std::size_t repeat) {
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

inline ::fast_io::u8string make_latex_heavy(::std::size_t repeat) {
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

inline ::fast_io::u8string make_stress_long_line(::std::size_t length) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < length; ++i) {
        input.push_back(u8'a' + static_cast<char8_t>(i % 26));
    }
    return input;
}

inline ::fast_io::u8string make_stress_many_lines(::std::size_t lines) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < lines; ++i) {
        input.append(u8"line 1234567890\n");
    }
    return input;
}

// Optimizer-specific inputs

inline ::fast_io::u8string make_redundant_color_nesting(::std::size_t depth) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"<color=red>");
    }
    input.append(u8"redundant nested text");
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"</color>");
    }
    return input;
}

inline ::fast_io::u8string make_mixed_redundant(::std::size_t depth) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"<color=red><b><i>");
    }
    input.append(u8"deep mixed formatting");
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"</i></b></color>");
    }
    return input;
}

inline ::fast_io::u8string make_adjacent_text_nodes(::std::size_t segments) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < segments; ++i) {
        input.append(u8"<color=red>segment_");
        input.push_back(u8'0' + static_cast<char8_t>(i % 10));
        if (i % 2 == 0) {
            input.append(u8"</color><color=red>");
        }
        else {
            input.append(u8"</color>");
        }
    }
    return input;
}

inline ::fast_io::u8string make_html_span_attrs(::std::size_t spans) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < spans; ++i) {
        input.append(
            u8R"(<span style="color:red;font-size:16px">text</span>
)");
    }
    return input;
}

// End-to-end documents

inline ::fast_io::u8string make_full_document(::std::size_t repeat) {
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

inline ::fast_io::u8string make_terse_document(::std::size_t repeat) {
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

inline ::fast_io::u8string make_plain_document(::std::size_t chars) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < chars / 50; ++i) {
        input.append(u8"Plain text line with no special formatting at all.\n");
    }
    return input;
}
