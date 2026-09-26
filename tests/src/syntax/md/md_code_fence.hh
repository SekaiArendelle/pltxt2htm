#pragma once

#include "doctest_config.hh"

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
        auto const& answer = u8"<pre><code class=\"language-py\">print(1)</code></pre>";
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
        auto const& answer = u8"<pre><code class=\"language-py\">print(1)</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\nprint(1)\n</font>";
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
        auto const& answer = u8"<br><pre><code class=\"language-py\">print(1)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-br-prefixed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)\n```");
        auto const& answer = u8"<br><pre><code class=\"language-py\">print(1)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("language-unclosed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br>```py\nprint(1)");
        auto const& answer = u8"<br><pre><code class=\"language-py\">print(1)</code></pre>";
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
            u8"<br><pre><code class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre><br><pre><code "
            u8"class=\"language-py\">print(&quot;Hello&nbsp;World&quot;)</code></pre>";
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
        auto const& answer = u8"t<br><pre><code class=\"language-py\">```t</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tilde-closing-fence-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\n~~~py\n~~~t");
        // TODO reduce <br> tag before <pre> tag
        auto const& answer = u8"t<br><pre><code class=\"language-py\">~~~t</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tab-after-backticks") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```\tpy\nprint(1)\n```");
        auto const& answer = u8"<pre><code class=\"language-py\">print(1)</code></pre>";
        CHECK(html == answer);
    }

    TEST_CASE("tab-after-language") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```py\t\nprint(1)\n```");
        auto const& answer = u8"<pre><code class=\"language-py\">print(1)</code></pre>";
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
        auto const& answer = u8"<pre><code class=\"language-c++\">code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-c-sharp") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```c#\ncode\n```");
        auto const& answer = u8"<pre><code class=\"language-c#\">code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-dotted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo.bar\ncode\n```");
        auto const& answer = u8"<pre><code class=\"language-foo.bar\">code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-underscored") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo_bar\ncode\n```");
        auto const& answer = u8"<pre><code class=\"language-foo_bar\">code</code></pre>";
        CHECK(html == answer);
    }
    TEST_CASE("language-hyphenated") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```foo-bar\ncode\n```");
        auto const& answer = u8"<pre><code class=\"language-foo-bar\">code</code></pre>";
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
        // "\&\" yields "<pre><code>...&amp;\</code></pre>". The trailing backslash before
        // "</code></pre>" must not be parsed as an MD escape by the HTML parser, so
        // re-parsing the first-pass HTML must be idempotent.
        auto const& pltext = u8"```\n\\&\\\n```";
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& once_answer = u8"<pre><code>&amp;\\</code></pre>";
        CHECK(once == once_answer);
        auto twice =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{once.data(), once.size()});
        CHECK(twice == once);
    }

    TEST_CASE("nested-fence-stays-content") {
        // regression: a line that merely starts with a fence (e.g. a nested markdown fence)
        // inside the code content is NOT a valid closing fence, so it stays as content and
        // only a proper closing fence on its own line ends the block.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```md\n```js\ncode\n```");
        auto const& answer = u8"<pre><code class=\"language-md\">```js\ncode</code></pre>";
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
}
