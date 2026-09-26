#pragma once

#include "doctest_config.hh"

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

    TEST_CASE("code-language-class") {
        auto const& pltext = u8"<pre><code class=\"language-cpp\">int x;</code></pre>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<pre><code class=\"language-cpp\">int&nbsp;x;</code></pre>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\nint\u00A0x;\n</font>";
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
