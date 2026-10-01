#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/parser.hh>

TEST_SUITE("md_code_fence") {
    TEST_CASE("basic") {
        auto const& pltext = u8"```\ntest\n```";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>test</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("single-line-backticks") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto const& answer = u8"<code>test</code>";
        CHECK(html == answer);
    }

    TEST_CASE("multiline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\nte\nst\n```");
        auto const& answer = u8"<pre><code>te\nst</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tilde-fence") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~\nte\nst\n~~~");
        auto const& answer = u8"<pre><code>te\nst</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-py") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\nprint(1)\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tilde-basic") {
        auto const& pltext = u8"~~~\ntest\n~~~";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>test</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("tilde-language-py") {
        auto const& pltext = u8"~~~py\nprint(1)\n~~~";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC "
            u8"SDF\">\n<color=#8250df>print</color>(<color=#0550ae>1</color>)\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("leading-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```\ntest\n```");
        auto const& answer = u8"<br><pre><code>test</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("br-prefixed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\ntest\n```");
        auto const& answer = u8"<br><pre><code>test</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-leading-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```py\nprint(1)\n```");
        auto const& answer =
            u8"<br><pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-br-prefixed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)\n```");
        auto const& answer =
            u8"<br><pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-unclosed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)");
        auto const& answer =
            u8"<br><pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("br-content-space") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\nte st\n```");
        auto const& answer = u8"<br><pre><code>te&nbsp;st</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("content-after-closing-fence") {
        // A line that starts with a fence but has content after it is NOT a valid
        // closing fence (CommonMark §4.5), so the block runs to the end of the input.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n```test");
        auto const& answer = u8"<pre><code>test\n```test</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("two-blocks") {
        auto const& data =
            u8R"(
```py
print("Hello World")
```
```py
print("Hello World")
```)";
        auto html = ::pltxt2htm_test::pltxt4unittest(data);
        auto const& answer =
            u8"<br><pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0a3069;\">&quot;Hello&nbsp;World&quot;</span>)</code></pre><br><pre><code><span "
            u8"style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0a3069;\">&quot;Hello&nbsp;World&quot;</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\n```");
        auto const& answer = u8"<pre><code></code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("inline-closing-fence-content") {
        // "```t" is not a valid closing fence (content after the fence on the same line),
        // so it is kept as code content and the block runs to the end of the input.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto const& answer = u8"t<br><pre><code>```t</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-closing-fence-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```py\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto const& answer = u8"t<br><pre><code>```t</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tilde-closing-fence-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n~~~py\n~~~t");
        // TODO reduce <br> tag before <pre> tag
        auto const& answer = u8"t<br><pre><code>~~~t</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tab-after-backticks") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\tpy\nprint(1)\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tab-after-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\t\nprint(1)\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#8250df;\">print</span>(<span "
            u8"style=\"color:#0550ae;\">1</span>)</code></pre>";
        CHECK(html == answer);
    }

    // Invalid language characters are rejected (only [a-zA-Z0-9+#._-] allowed)
    // "" in language → rejected, no language class
    TEST_CASE("invalid-quote-in-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo\"onmouseover=\"alert(1)\nprint(1)\n```");
        auto const& answer = u8"<pre><code>print(1)</code></pre>";
        CHECK(html == answer);
    }

    // "<" in language → rejected, no language class
    TEST_CASE("invalid-angle-in-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<svg/onload=alert(1)>\nprint(1)\n```");
        auto const& answer = u8"<pre><code>print(1)</code></pre>";
        CHECK(html == answer);
    }

    // "&" in language → rejected, no language class
    TEST_CASE("invalid-ampersand-in-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```&#xGG;\nprint(1)\n```");
        auto const& answer = u8"<pre><code>print(1)</code></pre>";
        CHECK(html == answer);
    }

    // "~" is not a valid language character → rejected, no language class
    TEST_CASE("tilde-in-language-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~~\n%'#");
        auto const& answer = u8"<pre><code>%&apos;#</code></pre>";
        CHECK(html == answer);
    }

    // "\" in language → rejected, no language class
    TEST_CASE("backslash-in-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\\\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }

    // "`" in language → rejected, no language class
    TEST_CASE("extra-backtick-in-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"````\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }

    // Valid special characters in language: "+", "#", ".", "_", "-"
    TEST_CASE("language-c-plus-plus") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c++\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-c-sharp") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c#\npublic\n```");
        auto const& answer = u8"<pre><code><span style=\"color:#cf222e;\">public</span></code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-dotted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo.bar\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-underscored") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo_bar\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-hyphenated") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo-bar\ncode\n```");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("five-backticks-literal") {
        // 5 backticks followed by content without a newline: the block-level fence parser
        // bails out, and the inline code-span branches only ever match empty content
        // (e.g. the first two backticks), which is rejected. The whole input stays literal.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"`````a bc");
        auto const& answer = u8"`````a&nbsp;bc";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-language-py") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"```py\ntest\n```");
        auto const& answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-tilde-language-py") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"~~~py\ntest\n~~~");
        auto const& answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-trailing-backslash") {
        // regression: roundtrip fuzzer crash. A fenced code block whose content ends in
        // "\&\" keeps both backslashes literally. The trailing backslash before
        // "</code></pre>" must not be parsed as an MD escape by the HTML parser either,
        // so re-parsing the first-pass HTML remains idempotent.
        auto const& pltext = u8"```\n\\&\\\n```";
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& once_answer = u8"<pre><code>\\&amp;\\</code></pre>";
        CHECK(once == once_answer);
        auto twice =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{once.data(), once.size()});
        CHECK(twice == once);
    }

    TEST_CASE("roundtrip-highlighted-cpp") {
        // The parser consumes the Markdown fence language and stores language-neutral
        // highlighting nodes, so roundtrip HTML remains identical without a language class.
        auto const once = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"```cpp\nint x;\n```");
        auto const& answer = u8"<pre><code><span style=\"color:#cf222e;\">int</span>&nbsp;x;</code></pre>";
        CHECK(once == answer);
        auto const twice =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{once.data(), once.size()});
        CHECK(twice == once);
    }

    TEST_CASE("nested-fence-stays-content") {
        // regression: a line that merely starts with a fence (e.g. a nested markdown fence)
        // inside the code content is NOT a valid closing fence, so it stays as content and
        // only a proper closing fence on its own line ends the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```md\n```js\ncode\n```");
        auto const& answer = u8"<pre><code>```js\ncode</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("closing-fence-trailing-spaces") {
        // regression: a closing fence may be followed by spaces/tabs on the same line
        // (CommonMark §4.5), and must still close the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \nrest");
        auto const& answer = u8"<pre><code>test</code></pre><br>rest";
        CHECK(html == answer);
    }

    TEST_CASE("four-backticks-not-closing") {
        // regression: this project only supports fixed 3-delimiter fences, so a longer
        // closing fence (4+ delimiters) is NOT a valid closing fence and stays as content.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n````\nrest");
        auto const& answer = u8"<pre><code>test\n````\nrest</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("closing-fence-trailing-tab") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \t");
        auto const& answer = u8"<pre><code>test</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("closing-fence-tab-then-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \t\nrest");
        auto const& answer = u8"<pre><code>test</code></pre><br>rest";
        CHECK(html == answer);
    }

    // Built-in C++ highlighting uses inline styles and emits no highlight.js language classes.
    TEST_CASE("cpp-highlighting") {
        auto const& pltext = u8"```cpp\nimport std;\n\nauto main() -> int {\n    std::println(\"Hello C++\");\n}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">import</span>&nbsp;std;\n\n<span style=\"color:#cf222e;\">"
            u8"auto</span>&nbsp;<span style=\"color:#8250df;\">main</span>()&nbsp;-&gt;&nbsp;<span "
            u8"style=\"color:#cf222e;\">"
            u8"int</span>&nbsp;{\n&nbsp;&nbsp;&nbsp;&nbsp;std::<span style=\"color:#8250df;\">println</span>(<span "
            u8"style=\"color:#0a3069;\">&quot;Hello&nbsp;C++&quot;</span>);\n}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>import</color>\u00A0std;\n\n<color=#cf222e>"
            u8"auto</color>\u00A0<color=#8250df>main</color>()\u00A0-<size=20>\uff1e</size>\u00A0<color=#cf222e>"
            u8"int</color>\u00A0{\n\u00A0\u00A0\u00A0\u00A0std::<color=#8250df>println</"
            u8"color>(<color=#0a3069>\"Hello\u00A0C++\"</color>);\n}\n</font>";
        CHECK(richtext == richtext_answer);
    }

    // C highlighting uses the C23 keyword set instead of treating C as C++.
    TEST_CASE("c23-highlighting") {
        auto const& pltext =
            u8"```c23\n#include <stdio.h>\nconstexpr _BitInt(16) add(typeof_unqual(int) left, int right) {\n"
            u8"    // C23\n    class value = nullptr;\n    return left + right;\n}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#0550ae;\">#include</span>&nbsp;&lt;stdio.h&gt;\n"
            u8"<span style=\"color:#cf222e;\">constexpr</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">_BitInt</span>(<span style=\"color:#0550ae;\">16</span>)&nbsp;<span "
            u8"style=\"color:#8250df;\">add</span>(<span style=\"color:#cf222e;\">typeof_unqual</span>(<span "
            u8"style=\"color:#cf222e;\">int</span>)&nbsp;left,&nbsp;<span "
            u8"style=\"color:#cf222e;\">int</span>&nbsp;right)&nbsp;{\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color:#6e7781;\">//&nbsp;C23</span>\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;value&nbsp;=&nbsp;<span "
            u8"style=\"color:#cf222e;\">nullptr</span>;\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;left&nbsp;+&nbsp;right;\n}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);

        auto const richtext =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"```c\nconstexpr int main() {\n    return nullptr;\n}\n```");
        auto const& richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>constexpr</color>\u00A0<color=#cf222e>"
            u8"int</color>\u00A0<color=#8250df>main</color>()\u00A0{\n\u00A0\u00A0\u00A0\u00A0<color=#cf222e>return</"
            u8"color>\u00A0<color=#cf222e>nullptr</color>;\n}\n</font>";
        CHECK(richtext == richtext_answer);
    }

    // Rust macros and strings use the same token stream in both backends.
    TEST_CASE("rust-highlighting") {
        auto const& pltext = u8"```rust\nfn main() {\n    println!(\"Hello Rust\");\n}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">fn</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">main</span>()&nbsp;{\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color:#cf222e;\">println</span>!(<span style=\"color:#0a3069;\">"
            u8"&quot;Hello&nbsp;Rust&quot;</span>);\n}</code></pre>";
        CHECK(html == answer);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC "
            u8"SDF\">\n<color=#cf222e>fn</color>\u00A0<color=#8250df>main</color>()\u00A0{\n"
            u8"\u00A0\u00A0\u00A0\u00A0<color=#cf222e>println</color>!(<color=#0a3069>\"Hello\u00A0Rust\"</"
            u8"color>);\n}\n</font>";
        CHECK(richtext == richtext_answer);
    }

    TEST_CASE("c-standard-typedefs") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(
            u8"```c\nsize_t a; ptrdiff_t b; max_align_t c; nullptr_t d; wchar_t e;\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">size_t</span>&nbsp;a;&nbsp;<span "
            u8"style=\"color:#cf222e;\">ptrdiff_t</span>&nbsp;b;&nbsp;<span "
            u8"style=\"color:#cf222e;\">max_align_t</span>&nbsp;c;&nbsp;<span "
            u8"style=\"color:#cf222e;\">nullptr_t</span>&nbsp;d;&nbsp;<span "
            u8"style=\"color:#cf222e;\">wchar_t</span>&nbsp;e;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("cpp-standard-typedefs") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(
            u8"```cpp\nstd::size_t a; std::ptrdiff_t b; std::max_align_t c; std::nullptr_t d; std::byte e;\n```");
        auto const& answer =
            u8"<pre><code>std::<span style=\"color:#cf222e;\">size_t</span>&nbsp;a;&nbsp;std::<span "
            u8"style=\"color:#cf222e;\">ptrdiff_t</span>&nbsp;b;&nbsp;std::<span "
            u8"style=\"color:#cf222e;\">max_align_t</span>&nbsp;c;&nbsp;std::<span "
            u8"style=\"color:#cf222e;\">nullptr_t</span>&nbsp;d;&nbsp;std::<span "
            u8"style=\"color:#cf222e;\">byte</span>&nbsp;e;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("c-fixed-width-integer-types") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(
            u8"```c\nuint16_t a; int_least32_t b; uint_fast64_t c; intptr_t d; uintmax_t e;\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">uint16_t</span>&nbsp;a;&nbsp;<span "
            u8"style=\"color:#cf222e;\">int_least32_t</span>&nbsp;b;&nbsp;<span "
            u8"style=\"color:#cf222e;\">uint_fast64_t</span>&nbsp;c;&nbsp;<span "
            u8"style=\"color:#cf222e;\">intptr_t</span>&nbsp;d;&nbsp;<span "
            u8"style=\"color:#cf222e;\">uintmax_t</span>&nbsp;e;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("cpp-fixed-width-integer-types") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(u8"```cpp\nuint16_t a; std::int32_t b;\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">uint16_t</span>&nbsp;a;&nbsp;std::<span "
            u8"style=\"color:#cf222e;\">int32_t</span>&nbsp;b;</code></pre>";
        CHECK(html == answer);
    }

    // Lua short strings, long-bracket strings/comments, keywords, numbers, and functions.
    TEST_CASE("lua-highlighting") {
        auto const& pltext =
            u8"```lua\nlocal function greet(name)\n    --[=[ first\nsecond ]=]\n    local message = [=[Hello ]=] .. "
            u8"name\n"
            u8"    print(message, 42, \"!\")\nend\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">local</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">function</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">greet</span>(name)\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#6e7781;\">--[=[&nbsp;first</span>\n<span "
            u8"style=\"color:#6e7781;\">second&nbsp;]=]</span>\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#cf222e;\">local</span>&nbsp;message&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">[=[Hello&nbsp;]=]</span>&nbsp;..&nbsp;name\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color:#8250df;\">print</span>(message,&nbsp;<span "
            u8"style=\"color:#0550ae;\">42</span>,&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;!&quot;</span>)\n<span "
            u8"style=\"color:#cf222e;\">end</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"```lua\nlocal function greet()\n    return \"hello\"\nend\n```");
        auto const& richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>local</color>\u00A0<color=#cf222e>"
            u8"function</color>\u00A0<color=#8250df>greet</color>()\n\u00A0\u00A0\u00A0\u00A0<color=#cf222e>"
            u8"return</color>\u00A0<color=#0a3069>\"hello\"</color>\n<color=#cf222e>end</color>\n</font>";
        CHECK(richtext == richtext_answer);
    }

    // Additional built-in languages share lexer families but retain their own keywords and comments.
    TEST_CASE("css-highlighting") {
        auto const& pltext = u8"```css\n@media screen { color: \"red\"; /* ok */ }\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">@media</span>&nbsp;screen&nbsp;{&nbsp;color:&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;red&quot;</span>;&nbsp;<span "
            u8"style=\"color:#6e7781;\">/*&nbsp;ok&nbsp;*/</span>&nbsp;}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("javascript-highlighting") {
        auto const& pltext = u8"```js\nfunction greet() { return \"hi\"; } // ok\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">function</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">greet</span>()&nbsp;{&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;hi&quot;</span>;&nbsp;}&nbsp;<span "
            u8"style=\"color:#6e7781;\">//&nbsp;ok</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("typescript-highlighting") {
        auto const& pltext = u8"```ts\nfunction greet(name: string): number { return 42; }\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">function</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">greet</span>(name:&nbsp;<span "
            u8"style=\"color:#cf222e;\">string</span>):&nbsp;<span "
            u8"style=\"color:#cf222e;\">number</span>&nbsp;{&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;<span "
            u8"style=\"color:#0550ae;\">42</span>;&nbsp;}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("python-highlighting") {
        auto const& pltext = u8"```py\ndef greet(name): # ok\n    return \"hi\"\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">def</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">greet</span>(name):&nbsp;<span "
            u8"style=\"color:#6e7781;\">#&nbsp;ok</span>\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;hi&quot;</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("python-multiline-string") {
        auto const html =
            ::pltxt2htm_test::pltxt4unittest(u8"```python\n\"\"\"before \" quote\nclass after\n\"\"\"\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#0a3069;\">&quot;&quot;&quot;before&nbsp;&quot;&nbsp;quote</span>\n"
            u8"<span style=\"color:#0a3069;\">class&nbsp;after</span>\n"
            u8"<span style=\"color:#0a3069;\">&quot;&quot;&quot;</span></code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("java-highlighting") {
        auto const& pltext = u8"```java\npublic static void main() { return; } // ok\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">public</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">static</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">void</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">main</span>()&nbsp;{&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>;&nbsp;}&nbsp;<span "
            u8"style=\"color:#6e7781;\">//&nbsp;ok</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("java-text-block") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(
            u8"```java\nString text = \"\"\"before \" quote\nclass after\n\"\"\";\n```");
        auto const& answer =
            u8"<pre><code>String&nbsp;text&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;&quot;&quot;before&nbsp;&quot;&nbsp;quote</span>\n"
            u8"<span style=\"color:#0a3069;\">class&nbsp;after</span>\n"
            u8"<span style=\"color:#0a3069;\">&quot;&quot;&quot;</span>;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("go-highlighting") {
        auto const& pltext = u8"```go\nfunc main() { return } // ok\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">func</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">main</span>()&nbsp;{&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;}&nbsp;<span "
            u8"style=\"color:#6e7781;\">//&nbsp;ok</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("bash-highlighting") {
        auto const& pltext = u8"```bash\nif true; then echo \"ok\"; fi # done\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">if</span>&nbsp;true;&nbsp;<span "
            u8"style=\"color:#cf222e;\">then</span>&nbsp;echo&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;ok&quot;</span>;&nbsp;<span "
            u8"style=\"color:#cf222e;\">fi</span>&nbsp;<span "
            u8"style=\"color:#6e7781;\">#&nbsp;done</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("json-highlighting") {
        auto const& pltext = u8"```json\n{\"ok\": true, \"n\": 42}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code>{<span style=\"color:#0a3069;\">&quot;ok&quot;</span>:&nbsp;<span "
            u8"style=\"color:#cf222e;\">true</span>,&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;n&quot;</span>:&nbsp;<span "
            u8"style=\"color:#0550ae;\">42</span>}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("yaml-highlighting") {
        auto const& pltext = u8"```yaml\nenabled: true # ok\nname: \"demo\"\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code>enabled:&nbsp;<span style=\"color:#cf222e;\">true</span>&nbsp;<span "
            u8"style=\"color:#6e7781;\">#&nbsp;ok</span>\nname:&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;demo&quot;</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("sql-highlighting") {
        auto const& pltext = u8"```sql\nSELECT count(*) FROM users WHERE id = 42; -- ok\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">SELECT</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">count</span>(*)&nbsp;<span "
            u8"style=\"color:#cf222e;\">FROM</span>&nbsp;users&nbsp;<span "
            u8"style=\"color:#cf222e;\">WHERE</span>&nbsp;id&nbsp;=&nbsp;<span "
            u8"style=\"color:#0550ae;\">42</span>;&nbsp;<span "
            u8"style=\"color:#6e7781;\">--&nbsp;ok</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("csharp-highlighting") {
        auto const& pltext =
            u8"```csharp\npublic class Demo {\n    static string Greet() => $@\"hello \"\"world\"\"\";\n}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">public</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">class</span>&nbsp;Demo&nbsp;{\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#cf222e;\">static</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">string</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">Greet</span>()&nbsp;=&gt;&nbsp;<span "
            u8"style=\"color:#0a3069;\">$@&quot;hello&nbsp;&quot;&quot;world&quot;&quot;&quot;</span>;\n}</code></"
            u8"pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("csharp-raw-string") {
        auto const& pltext = u8"```csharp\nstring text = \"\"\"\"before \" quote\"\"\"\";\nclass Next {}\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">string</span>&nbsp;text&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;&quot;&quot;&quot;before&nbsp;&quot;&nbsp;quote&quot;&quot;&quot;&quot;</"
            u8"span>;\n<span style=\"color:#cf222e;\">class</span>&nbsp;Next&nbsp;{}</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("kotlin-highlighting") {
        auto const& pltext = u8"```kotlin\nfun greet(name: String) = \"\"\"hello\n$name\"\"\"\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">fun</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">greet</span>(name:&nbsp;String)&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;&quot;&quot;hello</span>\n<span "
            u8"style=\"color:#0a3069;\">$name&quot;&quot;&quot;</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("toml-highlighting") {
        auto const& pltext = u8"```toml\nenabled = true # ok\ntitle = \"\"\"hello\nworld\"\"\"\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code>enabled&nbsp;=&nbsp;<span style=\"color:#cf222e;\">true</span>&nbsp;<span "
            u8"style=\"color:#6e7781;\">#&nbsp;ok</span>\ntitle&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;&quot;&quot;hello</span>\n<span "
            u8"style=\"color:#0a3069;\">world&quot;&quot;&quot;</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("html-highlighting") {
        auto const& pltext = u8"```html\n<div class=\"note\">Hello<!-- ok --></div>\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code>&lt;<span style=\"color:#cf222e;\">div</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">class</span>=<span "
            u8"style=\"color:#0a3069;\">&quot;note&quot;</span>&gt;Hello<span "
            u8"style=\"color:#6e7781;\">&lt;!--&nbsp;ok&nbsp;--&gt;</span>&lt;/<span "
            u8"style=\"color:#cf222e;\">div</span>&gt;</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    // Plain text between adjacent markup tags is emitted exactly once.
    TEST_CASE("html-adjacent-tags") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(u8"```html\n<div>Hello<span>world</span></div>\n```");
        auto const& answer =
            u8"<pre><code>&lt;<span style=\"color:#cf222e;\">div</span>&gt;Hello&lt;<span "
            u8"style=\"color:#cf222e;\">span</span>&gt;world&lt;/<span "
            u8"style=\"color:#cf222e;\">span</span>&gt;&lt;/<span "
            u8"style=\"color:#cf222e;\">div</span>&gt;</code></pre>";
        CHECK(html == answer);
    }

    // HTML raw-text elements do not treat less-than operators in their contents as tags.
    TEST_CASE("html-raw-text-elements") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(
            u8"```html\n<script>if (a<b) value</script><style>.x{width:1<2}</style>\n```");
        auto const& answer =
            u8"<pre><code>&lt;<span style=\"color:#cf222e;\">script</span>&gt;if&nbsp;(a&lt;b)&nbsp;value&lt;/<span "
            u8"style=\"color:#cf222e;\">script</span>&gt;&lt;<span "
            u8"style=\"color:#cf222e;\">style</span>&gt;.x{width:1&lt;2}"
            u8"&lt;/<span style=\"color:#cf222e;\">style</span>&gt;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("xml-highlighting") {
        auto const& pltext = u8"```xml\n<?xml version=\"1.0\"?><svg viewBox=\"0 0\"><path /></svg>\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#0550ae;\">&lt;?xml&nbsp;version=&quot;1.0&quot;?&gt;</span>&lt;<span "
            u8"style=\"color:#cf222e;\">svg</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">viewBox</span>=<span "
            u8"style=\"color:#0a3069;\">&quot;0&nbsp;0&quot;</span>&gt;&lt;<span "
            u8"style=\"color:#cf222e;\">path</span>&nbsp;/&gt;&lt;/<span "
            u8"style=\"color:#cf222e;\">svg</span>&gt;</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    TEST_CASE("xml-cdata") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(u8"```xml\n<![CDATA[a < b]]>\n```");
        auto const& answer =
            u8"<pre><code><span style=\"color:#0a3069;\">&lt;![CDATA[a&nbsp;&lt;&nbsp;b]]&gt;</span></code></pre>";
        CHECK(html == answer);
    }

    // A less-than operator followed by whitespace is not an HTML tag opener.
    TEST_CASE("html-less-than-operator") {
        auto const html = ::pltxt2htm_test::pltxt4unittest(u8"```html\nif (a < b) text\n```");
        auto const& answer = u8"<pre><code>if&nbsp;(a&nbsp;&lt;&nbsp;b)&nbsp;text</code></pre>";
        CHECK(html == answer);
    }

    // Newlines stay separate code AST ranges instead of being nested in highlighted nodes.
    TEST_CASE("multiline-highlight-ranges") {
        auto const& pltext = u8"```cpp\n/* first\nsecond */\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#6e7781;\">/*&nbsp;first</span>\n<span "
            u8"style=\"color:#6e7781;\">second&nbsp;*/</span></code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    // Markdown escapes and entity references stay literal in fenced code, while the
    // same cursor still performs syntax classification and parses UTF-8 code points.
    TEST_CASE("markdown-syntax-literal-in-code") {
        auto const& pltext = u8"```cpp\n\\#include &lt;vector&gt;\nauto text = &amp;value;\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code>\\<span style=\"color:#0550ae;\">#include</span>&nbsp;&amp;lt;vector&amp;gt;\n<span "
            u8"style=\"color:#cf222e;\">auto</span>&nbsp;text&nbsp;=&nbsp;&amp;amp;value;</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    // Rust nested comments and raw strings are emitted directly as colored AST nodes.
    TEST_CASE("rust-nested-comments-and-raw-string") {
        auto const& pltext = u8"```rust\n/* outer /* inner */ outer */\nlet text = r##\"a\"#b\"##;\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span "
            u8"style=\"color:#6e7781;\">/*&nbsp;outer&nbsp;/*&nbsp;inner&nbsp;*/&nbsp;outer&nbsp;*/</span>\n"
            u8"<span style=\"color:#cf222e;\">let</span>&nbsp;text&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">r##&quot;a&quot;#b&quot;##</span>;</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("rust-raw-identifiers-and-byte-strings") {
        auto const& pltext = u8"```rust\nlet r#type = br#\"a\"b\"#;\n```";
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<pre><code><span style=\"color:#cf222e;\">let</span>&nbsp;r#type&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">br#&quot;a&quot;b&quot;#</span>;</code></pre>";
        CHECK(html == answer);
        auto const reparsed =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed == html);
    }

    // CodeFence owns language-neutral highlighting IR. The source ranges still
    // reconstruct the original fence contents without retaining cpp metadata.
    TEST_CASE("language-neutral-highlight-ir") {
        auto const ast = ::pltxt2htm::parse_pltxt<::pltxt2htm::Contracts::quick_enforce>(u8"```cpp\nint f();\n```");
        CHECK(ast.size() == 1);
        auto const& root{ast.index(0)};
        CHECK(root.get_node_kind() == ::pltxt2htm::NodeKind::code_fence);
        auto const& code_ast{root.as_code_fence().get_highlighted_ast()};

        ::pltxt2htm::container::BasicString<char8_t> source{};
        bool has_keyword{};
        bool has_function_name{};
        for (auto const& node : code_ast.get_nodes()) {
            source.template append<::pltxt2htm::Contracts::quick_enforce>(code_ast.get_text(node));
            auto const kind{node.get_kind()};
            has_keyword = has_keyword || kind == ::pltxt2htm::CodeHighlightKind::keyword;
            has_function_name = has_function_name || kind == ::pltxt2htm::CodeHighlightKind::function;
        }
        auto const expected = ::pltxt2htm::container::BasicStringView{u8"int f();"};
        CHECK(source == expected);
        CHECK(has_keyword);
        CHECK(has_function_name);
    }
}
