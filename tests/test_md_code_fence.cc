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
        // The parser consumes the Markdown fence language and stores highlighting as
        // color nodes, so roundtrip HTML remains identical without a language class.
        auto const once = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"```cpp\nint x;\n```");
        auto const answer =
            ::fast_io::u8string_view{u8"<pre><code><span style=\"color:#cf222e;\">int</span>&nbsp;x;</code></pre>"};
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

    // C highlighting uses the C23 keyword set instead of treating C as C++.
    {
        auto const pltext = ::fast_io::u8string_view{
            u8"```c23\n#include <stdio.h>\nconstexpr _BitInt(16) add(typeof_unqual(int) left, int right) {\n"
            u8"    // C23\n    class value = nullptr;\n    return left + right;\n}\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:#0550ae;\">#include</span>&nbsp;&lt;stdio.h&gt;\n"
            u8"<span style=\"color:#cf222e;\">constexpr</span>&nbsp;<span "
            u8"style=\"color:#cf222e;\">_BitInt</span>(<span style=\"color:#0550ae;\">16</span>)&nbsp;<span "
            u8"style=\"color:#8250df;\">add</span>(<span style=\"color:#cf222e;\">typeof_unqual</span>(<span "
            u8"style=\"color:#cf222e;\">int</span>)&nbsp;left,&nbsp;<span "
            u8"style=\"color:#cf222e;\">int</span>&nbsp;right)&nbsp;{\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color:#6e7781;\">//&nbsp;C23</span>\n"
            u8"&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;value&nbsp;=&nbsp;<span "
            u8"style=\"color:#cf222e;\">nullptr</span>;\n&nbsp;&nbsp;&nbsp;&nbsp;<span "
            u8"style=\"color:#cf222e;\">return</span>&nbsp;left&nbsp;+&nbsp;right;\n}</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto const reparsed = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed, html);

        auto const richtext =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"```c\nconstexpr int main() {\n    return nullptr;\n}\n```");
        auto const richtext_answer = ::fast_io::u8string_view{
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>constexpr</color>\u00A0<color=#cf222e>"
            u8"int</color>\u00A0<color=#8250df>main</color>()\u00A0{\n\u00A0\u00A0\u00A0\u00A0<color=#cf222e>return</"
            u8"color>\u00A0<color=#cf222e>nullptr</color>;\n}\n</font>"};
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

    // Lua short strings, long-bracket strings/comments, keywords, numbers, and functions.
    {
        auto const pltext = ::fast_io::u8string_view{
            u8"```lua\nlocal function greet(name)\n    --[=[ first\nsecond ]=]\n    local message = [=[Hello ]=] .. "
            u8"name\n"
            u8"    print(message, 42, \"!\")\nend\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
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
            u8"style=\"color:#cf222e;\">end</span></code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto const reparsed = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed, html);

        auto const richtext = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"```lua\nlocal function greet()\n    return \"hello\"\nend\n```");
        auto const richtext_answer = ::fast_io::u8string_view{
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>local</color>\u00A0<color=#cf222e>"
            u8"function</color>\u00A0<color=#8250df>greet</color>()\n\u00A0\u00A0\u00A0\u00A0<color=#cf222e>"
            u8"return</color>\u00A0<color=#0a3069>\"hello\"</color>\n<color=#cf222e>end</color>\n</font>"};
        pltxt2htm_test_assert_equal(richtext, richtext_answer);
    }

    // Newlines stay direct CodeFence children instead of being nested in color nodes.
    {
        auto const pltext = ::fast_io::u8string_view{u8"```cpp\n/* first\nsecond */\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:#6e7781;\">/*&nbsp;first</span>\n<span "
            u8"style=\"color:#6e7781;\">second&nbsp;*/</span></code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto const reparsed = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed, html);
    }

    // Entity references, Markdown escapes, and UTF-8 code points are parsed by the
    // same cursor that drives syntax classification.
    {
        auto const pltext =
            ::fast_io::u8string_view{u8"```cpp\n\\#include &lt;vector&gt;\nauto text = \\\"你&amp;好\\\";\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:#0550ae;\">#include</span>&nbsp;&lt;vector&gt;\n<span "
            u8"style=\"color:#cf222e;\">auto</span>&nbsp;text&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">&quot;你&amp;好&quot;</span>;</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto const reparsed = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed, html);
    }

    // Rust nested comments and raw strings are emitted directly as colored AST nodes.
    {
        auto const pltext =
            ::fast_io::u8string_view{u8"```rust\n/* outer /* inner */ outer */\nlet text = r##\"a\"#b\"##;\n```"};
        auto const html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const answer = ::fast_io::u8string_view{
            u8"<pre><code><span "
            u8"style=\"color:#6e7781;\">/*&nbsp;outer&nbsp;/*&nbsp;inner&nbsp;*/&nbsp;outer&nbsp;*/</span>\n"
            u8"<span style=\"color:#cf222e;\">let</span>&nbsp;text&nbsp;=&nbsp;<span "
            u8"style=\"color:#0a3069;\">r##&quot;a&quot;#b&quot;##</span>;</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
