#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_code_tag") {
    // bare <code> renders as an inline <code> element (like other inline HTML tags)
    TEST_CASE("bare-inline-code") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text</code>");
        auto const& answer = u8"<code>text</code>";
        CHECK(html == answer);
    }

    // opening and closing tags allow spaces before '>' (case-insensitive)
    TEST_CASE("case-insensitive-spacing") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<CODE    >text</CODE  >");
        auto const& answer = u8"<code>text</code>";
        CHECK(html == answer);
    }

    // content is parsed as inline markup (nested tags are processed normally)
    TEST_CASE("nested-color-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</color></code>");
        auto const& answer = u8"<code><span style=\"color:red;\">text</span></code>";
        CHECK(html == answer);
    }

    TEST_CASE("close-order-mismatch") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</code></color>");
        auto const& answer = u8"<code><span style=\"color:red;\">text&lt;/code&gt;</span></code>";
        CHECK(html == answer);
    }

    // nested <code> is flattened (same-tag optimization, like other inline HTML tags)
    TEST_CASE("same-tag-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text<code>text</code></code>");
        auto const& answer = u8"<code>texttext</code>";
        CHECK(html == answer);
    }

    // unclosed <code> auto-closes at end of input; empty content is dropped
    TEST_CASE("unclosed-empty-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    // incomplete <code (no '>') stays literal escaped text
    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code");
        auto const& answer = u8"&lt;code";
        CHECK(html == answer);
    }

    // empty <code></code> is dropped
    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<code></code>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    // --- standalone <code class="language-..."> stays literal escaped text ---
    TEST_CASE("language-class-stays-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-bash\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;language-bash&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("class-attr-literal-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-cpp\">fn()</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;language-cpp&quot;&gt;fn()&lt;/code&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("single-quote-class-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-c++'>fn()</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&apos;language-c++&apos;&gt;fn()&lt;/code&gt;";
        CHECK(html == answer);
    }
    // case-sensitive: Language- vs language-
    TEST_CASE("language-class-case-sensitive") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"Language-bash\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;Language-bash&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    // non-language- prefix
    TEST_CASE("non-language-class-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"xxx\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;xxx&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    // empty class value
    TEST_CASE("empty-class-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    // unknown attribute (style)
    TEST_CASE("unknown-attribute-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code style=\"color:red\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;style=&quot;color:red&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    // language- with empty language suffix
    TEST_CASE("empty-language-suffix-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-\">echo</code>");
        auto const& answer = u8"&lt;code&nbsp;class=&quot;language-&quot;&gt;echo&lt;/code&gt;";
        CHECK(html == answer);
    }
    // attribute injection attempts stay escaped
    TEST_CASE("attribute-injection-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-\" onmouseover=\"alert(1)'>x</code>");
        auto const& answer =
            u8"&lt;code&nbsp;class=&apos;language-&quot;&nbsp;onmouseover=&quot;alert(1)&apos;&gt;x&lt;/code&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-mono-font") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<code>test</code>cd");
        auto const& answer = u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-class-literal-escaped") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<code class=\"language-cpp\">code</code>");
        auto const& answer =
            u8"<size=20>\uff1c</size>code\u00A0class=\"language-cpp\"<size=20>\uff1e</size>code<size=20>\uff1c</size>/"
            u8"code<size=20>\uff1e</size>";
        CHECK(html == answer);
    }
}
