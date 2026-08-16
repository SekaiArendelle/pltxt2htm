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
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
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
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
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
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
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
            u8"<br><pre><code class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre><br><pre><code "
            u8"class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre>"};
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
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code class=\"language-py\">```t</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n~~~py\n~~~t");
        // TODO reduce <br> tag before <pre> tag
        auto answer = ::fast_io::u8string_view{u8"t<br><pre><code class=\"language-py\">~~~t</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\tpy\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\t\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
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
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-c++\">code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c#\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-c#\">code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo.bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-foo.bar\">code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo_bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-foo_bar\">code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo-bar\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-foo-bar\">code</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // 5 backticks: first 3 open an inline code span, remaining 2 are content.
        // A space after some "language-like" text, followed by content without a newline,
        // causes the block-level fence parser to bail out, falling through to
        // inline code span parsing.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"`````a bc");
        auto answer = ::fast_io::u8string_view{u8"<code>``a&nbsp;bc</code>"};
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
        // regression: a line that merely starts with a fence (e.g. a nested markdown fence)
        // inside the code content is NOT a valid closing fence, so it stays as content and
        // only a proper closing fence on its own line ends the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```md\n```js\ncode\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-md\">```js\ncode</code></pre>"};
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

    return 0;
}
