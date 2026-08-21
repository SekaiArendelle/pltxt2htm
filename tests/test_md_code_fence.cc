#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"```\ntest\n```"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\nte\nst\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>te\nst</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~\nte\nst\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>te\nst</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"~~~\ntest\n~~~"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"~~~py\nprint(1)\n~~~"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\nprint(1)\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```\nte st\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>te&nbsp;st</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // A line that starts with a fence but has content after it is NOT a valid
        // closing fence (CommonMark §4.5), so the block runs to the end of the input.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n```test");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test\n```test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto data = ::fast_io::u8string_view{
            u8R"(
```py
print("Hello World")
```
```py
print("Hello World")
```)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(data);
        auto answer = ::fast_io::u8string_view{
            u8"<br><pre><code>print(&quot;Hello&nbsp;World&quot;)</code></"
            u8"pre><br><pre><code>print(&quot;Hello&nbsp;World&quot;)"
            u8"</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code></code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // "```t" is not a valid closing fence (content after the fence on the same line),
        // so it is kept as code content and the block runs to the end of the input.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code>```t</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n```py\n```t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code>```t</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n~~~py\n~~~t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code>~~~t</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\tpy\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\t\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Invalid language characters are rejected (only [a-zA-Z0-9+#._-] allowed)
    // "" in language → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo\"onmouseover=\"alert(1)\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // "<" in language → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<svg/onload=alert(1)>\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // "&" in language → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```&#xGG;\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // "~" is not a valid language character → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~~\n%'#");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>%&apos;#</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // "\" in language → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\\\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // "`" in language → rejected, no language class
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"````\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Valid special characters in language: "+", "#", ".", "_", "-"
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c++\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c#\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo.bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo_bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo-bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // 5 backticks followed by content without a newline: the block-level fence parser
        // bails out, and the inline code-span branches only ever match empty content
        // (e.g. the first two backticks), which is rejected. The whole input stays literal.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"`````a bc");
        auto answer = ::fast_io::u8string_view{u8"`````a&nbsp;bc"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"```py\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"~~~py\ntest\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // regression: roundtrip fuzzer crash. A fenced code block whose content ends in
        // "\&\" yields "<pre><code>...&amp;\</code></pre>". The trailing backslash before
        // "</code></pre>" must not be parsed as an MD escape by the HTML parser, so
        // re-parsing the first-pass HTML must be idempotent.
        auto pltext = ::fast_io::u8string_view{u8"```\n\\&\\\n```"};
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto once_answer = ::fast_io::u8string_view{u8"<pre><code>&amp;\\</code></pre>"};
        pltxt2htm_test_assert_equal(once, once_answer);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{once.data(), once.size()});
        pltxt2htm_test_assert_equal(twice, once);
    }

    {
        // The roundtrip backend intentionally discards the Markdown fence language.
        // Its HTML subset only accepts bare <pre><code>, so both passes stay identical
        // without relying on a highlight.js language class.
        auto const once = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"```cpp\nint x;\n```");
        auto const answer = ::fast_io::u8string_view{u8"<pre><code>int&nbsp;x;</code></pre>"};
        pltxt2htm_test_assert_equal(once, answer);
        auto const twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{once.data(), once.size()});
        pltxt2htm_test_assert_equal(twice, once);
    }

    {
        // regression: a line that merely starts with a fence (e.g. a nested markdown fence)
        // inside the code content is NOT a valid closing fence, so it stays as content and
        // only a proper closing fence on its own line ends the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```md\n```js\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>```js\ncode</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // regression: a closing fence may be followed by spaces/tabs on the same line
        // (CommonMark §4.5), and must still close the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \nrest");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre><br>rest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // regression: this project only supports fixed 3-delimiter fences, so a longer
        // closing fence (4+ delimiters) is NOT a valid closing fence and stays as content.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n````\nrest");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test\n````\nrest</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \t");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\ntest\n``` \t\nrest");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre><br>rest"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Built-in C++ highlighting uses inline styles and emits no highlight.js language classes.
    {
        auto const pltext = ::fast_io::u8string_view{
            u8"```cpp\nimport std;\n\nauto main() -> int {\n    std::println(\"Hello C++\");\n}\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:#cf222e;\">import</span>&nbsp;std;\n\n<span style=\"color:#cf222e;\">"
            u8"auto</span>&nbsp;<span style=\"color:#8250df;\">main</span>()&nbsp;-&gt;&nbsp;<span "
            u8"style=\"color:#cf222e;\">"
            u8"int</span>&nbsp;{\n&nbsp;&nbsp;&nbsp;&nbsp;std::<span style=\"color:#8250df;\">println</span>(<span "
            u8"style=\"color:#0a3069;\">&quot;Hello&nbsp;C++&quot;</span>);\n}</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto const reparsed = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed, html);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const richtext_answer = ::fast_io::u8string_view{
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>import</color>\u00A0std;\n\n<color=#cf222e>"
            u8"auto</color>\u00A0<color=#8250df>main</color>()\u00A0-<size=20>\uff1e</size>\u00A0<color=#cf222e>"
            u8"int</color>\u00A0{\n\u00A0\u00A0\u00A0\u00A0std::<color=#8250df>println</"
            u8"color>(<color=#0a3069>\"Hello\u00A0C++\"</color>);\n}\n</font>"};
        pltxt2htm_test_assert_equal(richtext, richtext_answer);
    }

    // Rust macros and strings use the same token stream in both backends.
    {
        auto const pltext = ::fast_io::u8string_view{u8"```rust\nfn main() {\n    println!(\"Hello Rust\");\n}\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:#cf222e;\">fn</span>&nbsp;<span "
            u8"style=\"color:#8250df;\">main</span>()&nbsp;{\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color:#cf222e;\">println</span>!(<span style=\"color:#0a3069;\">"
            u8"&quot;Hello&nbsp;Rust&quot;</span>);\n}</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const richtext_answer = ::fast_io::u8string_view{
            u8"<font=\"PhysicsLab-SarasaMonoSC "
            u8"SDF\">\n<color=#cf222e>fn</color>\u00A0<color=#8250df>main</color>()\u00A0{\n"
            u8"\u00A0\u00A0\u00A0\u00A0<color=#cf222e>println</color>!(<color=#0a3069>\"Hello\u00A0Rust\"</"
            u8"color>);\n}\n</font>"};
        pltxt2htm_test_assert_equal(richtext, richtext_answer);
    }

    return 0;
}
