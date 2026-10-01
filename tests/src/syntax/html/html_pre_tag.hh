#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/parser.hh>

TEST_SUITE("html_pre_tag") {
    // bare <pre> is literal escaped text now
    TEST_CASE("bare-tag-escaped") {
        auto const& pltext = u8"<pre>text</pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;pre&gt;text&lt;/pre&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>text<size=20>\uff1c</size>/pre<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // <pre><code> renders as a code block (same as markdown code fence)
    TEST_CASE("code-block-renders") {
        auto const& pltext = u8"<pre><code>test</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>test</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // Attributes on <code> are not part of the roundtrip HTML subset.
    TEST_CASE("code-language-class") {
        auto const& pltext = u8"<pre><code class=\"language-cpp\">int x;</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"&lt;pre&gt;&lt;code&nbsp;class=&quot;language-cpp&quot;&gt;int&nbsp;x;&lt;/code&gt;&lt;/pre&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size><size=20>\uff1c</size>code\u00A0class=\"language-cpp\""
            u8"<size=20>\uff1e</size>int\u00A0x;<size=20>\uff1c</size>/code<size=20>\uff1e</size><size=20>\uff1c</"
            u8"size>/"
            u8"pre<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("multiline-code-preserved") {
        auto const& pltext = u8"<pre><code>line1\nline2</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>line1\nline2</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\nline1\nline2\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // HTML code content is literal in the PL text frontend as well as in the HTML frontend.
    TEST_CASE("code-content-literal") {
        auto const& pltext = u8"<pre><code>a\\&b\\*</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>a\\&amp;b\\*</code></pre>";
        CHECK(html == answer);
    }

    // Canonical style spans emitted by the HTML backend remain markup inside code blocks.
    TEST_CASE("style-span-roundtrip") {
        auto const& pltext = u8"<pre><code><span style=\"color:#cf222e;\">int</span>&nbsp;x;</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        CHECK(html == pltext);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>int</color>\u00A0x;\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // Rendered-code character references use HTML replacement rules before Unity escaping.
    TEST_CASE("rendered-code-character-references") {
        auto const& pltext = u8"<pre><code>&#0;&#128;&#xD800;&lt;&gt;&nbsp;</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        CHECK(html == pltext);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n\uFFFD\u20AC\uFFFD<size=20>\uff1c</size>"
            u8"<size=20>\uff1e</size>\u00A0\n</font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // Style spans keep their declarations even when they are not part of the canonical subset.
    TEST_CASE("style-span-extra-declarations") {
        auto const& pltext =
            u8"<pre><code><span style=\"color:red;font-size:12px;vertical-align:2px;\">x</span></code></pre>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        CHECK(html == pltext);
    }

    // An unterminated style span is closed with the surrounding code block.
    TEST_CASE("unterminated-style-span-closed") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code><span style=\"color:red;\">text</code></pre>");
        auto const& answer = u8"<pre><code><span style=\"color:red;\">text</span></code></pre>";
        CHECK(html == answer);
    }

    // Once the opening tags match, an unterminated code block extends to EOF.
    TEST_CASE("unterminated-code-block-extends-to-eof") {
        auto const& pltext = u8"<pre><code>a\n<pre><code>b";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code>a\n&lt;pre&gt;&lt;code&gt;b</code></pre>";
        CHECK(html == answer);
        auto html_parser_result = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        CHECK(html_parser_result == answer);
    }

    // Unterminated style spans and their surrounding code block are both closed at EOF.
    TEST_CASE("unterminated-style-span-and-code-block-closed") {
        auto const& pltext = u8"<pre><code><span style=\"color:red;\">text";
        auto const& answer = u8"<pre><code><span style=\"color:red;\">text</span></code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        CHECK(html == answer);
        auto html_parser_result = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        CHECK(html_parser_result == answer);
    }

    // <pre> wrapping anything other than <code> is literal escaped text
    TEST_CASE("non-code-content-escaped") {
        auto const& pltext = u8"<pre><b>bold</b></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;pre&gt;<strong>bold</strong>&lt;/pre&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size><b>bold</b>"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // <pre>\n<code> (newline before code) is NOT a code block: <pre> is literal, <code> is an inline code span
    TEST_CASE("newline-breaks-code-block") {
        auto const& pltext = u8"<pre>\n<code>test</code>\n</pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;pre&gt;<br><code>test</code><br>&lt;/pre&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>\n"
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>\n"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<PRE    >text</PRE  >");
        auto const& answer = u8"&lt;PRE&nbsp;&nbsp;&nbsp;&nbsp;&gt;text&lt;/PRE&nbsp;&nbsp;&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-color-renders") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</color></pre>");
        auto const& answer = u8"&lt;pre&gt;<span style=\"color:red;\">text</span>&lt;/pre&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("misnested-inner-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</pre></color>");
        auto const& answer = u8"&lt;pre&gt;<span style=\"color:red;\">text&lt;/pre&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-tags-all-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>text<pre>text</pre></pre>");
        auto const& answer = u8"&lt;pre&gt;text&lt;pre&gt;text&lt;/pre&gt;&lt;/pre&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("lone-open-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>");
        auto const& answer = u8"&lt;pre&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-open-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre");
        auto const& answer = u8"&lt;pre";
        CHECK(html == answer);
    }

    TEST_CASE("empty-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<pre></pre>t");
        auto const& answer = u8"t&lt;pre&gt;&lt;/pre&gt;t";
        CHECK(html == answer);
    }

    // \n inside a bare <pre> becomes <br> (literal text)
    TEST_CASE("newline-becomes-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>line1\nline2</pre>");
        auto const& answer = u8"&lt;pre&gt;line1<br>line2&lt;/pre&gt;";
        CHECK(html == answer);
    }

    // \n inside <pre><code> is preserved as \n
    TEST_CASE("newline-preserved-in-code") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><code>line1\nline2</code></pre>");
        auto const& answer = u8"<pre><code>line1\nline2</code></pre>";
        CHECK(html == answer);
    }

    // \n inside literal-escaped <pre><color> becomes <br>
    TEST_CASE("newline-br-in-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>line1\nline2</color></pre>");
        auto const& answer = u8"&lt;pre&gt;<span style=\"color:red;\">line1<br>line2</span>&lt;/pre&gt;";
        CHECK(html == answer);
    }

    // \n outside <pre> still becomes <br>
    TEST_CASE("outside-newline-becomes-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"line1\nline2");
        auto const& answer = u8"line1<br>line2";
        CHECK(html == answer);
    }

    // inline <pre><code> mid-text: <pre> is literal, <code> is an inline code span
    TEST_CASE("inline-code-not-a-block") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<pre><code>c</code></pre>de");
        auto const& answer = u8"ab&lt;pre&gt;<code>c</code>&lt;/pre&gt;de";
        CHECK(html == answer);
    }

    // <pre><code> after a line break is a code block
    TEST_CASE("code-block-after-line-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"x\n<pre><code>c</code></pre>");
        auto const& answer = u8"x<br><pre><code>c</code></pre>";
        CHECK(html == answer);
    }

    // spaces/tabs between <pre> and <code> are allowed
    TEST_CASE("code-block-allows-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>  <code>c</code></pre>");
        auto const& answer = u8"<pre><code>c</code></pre>";
        CHECK(html == answer);
    }
}
