#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_div_tag") {
    // web backend renders <div style="margin-left:..."> as a block-level div with CSS margins
    TEST_CASE("margin-left-block-div") {
        auto const& pltext = u8"<div style=\"margin-left:2em\">text</div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("margin-right-block-div") {
        auto const& pltext = u8"<div style=\"margin-right:1em\">text</div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-right:1em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin right=1em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("left-right-margins") {
        auto const& pltext = u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;margin-right:3em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em right=3em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // px is emitted explicitly with a px suffix in the web backend
    TEST_CASE("pixel-unit-suffix") {
        auto const& pltext = u8"<div style=\"margin-left:10px\">text</div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:10px;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=10>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // percent unit is preserved
    TEST_CASE("percent-unit-preserved") {
        auto const& pltext = u8"<div style=\"margin-left:5%\">text</div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:5%;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=5%>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an empty div is kept, not erased by the optimizer
    TEST_CASE("empty-div-preserved") {
        auto const& pltext = u8"<div style=\"margin-left:2em\"></div>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;\"></div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em></margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an unclosed div still parses
    TEST_CASE("unclosed-div-parsed") {
        auto const& pltext = u8"<div style=\"margin-left:2em\">text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a newline inside a div still renders as <br>
    TEST_CASE("newline-to-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">line1\nline2</div>");
        auto const& answer = u8"<div style=\"margin-left:2em;\">line1<br>line2</div>";
        CHECK(html == answer);
    }

    // text after the closing </div> stays on its own content line
    TEST_CASE("newline-after-div-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">a</div>\nb");
        auto const& answer = u8"<div style=\"margin-left:2em;\">a</div><br>b";
        CHECK(html == answer);
    }

    // text before and after a block-level div
    TEST_CASE("text-around-block-div") {
        auto const& pltext = u8"a\n<div style=\"margin-left:2em\">b</div>\nc";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><div style=\"margin-left:2em;\">b</div><br>c";
        CHECK(html == answer);
    }

    // mid-line <div> sequences are literal text (block-level only)
    TEST_CASE("midline-div-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<div style=\"margin-left:2em\">x</div>t");
        auto const& answer = u8"t&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;t";
        CHECK(html == answer);
    }

    // non-numeric margin value renders as literal text
    TEST_CASE("nonnumeric-margin-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:abc\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left:abc&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // an unknown style declaration (e.g. color) makes the whole tag literal text
    TEST_CASE("unknown-style-declaration") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"color:red\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;color:red&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // a style mix with a non-margin declaration is rejected entirely
    TEST_CASE("mixed-style-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em;color:red\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left:2em;color:red&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // a div without the style attribute is literal text (not parsed as a margin block)
    TEST_CASE("missing-style-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div>plain</div>");
        auto const& answer = u8"&lt;div&gt;plain&lt;/div&gt;";
        CHECK(html == answer);
    }

    // a script tag is not an allowed attribute and renders as literal text (XSS guard)
    TEST_CASE("disallowed-attribute-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div onclick=\"alert(1)\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;onclick=&quot;alert(1)&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // malformed quote (unterminated style value) renders as literal text
    TEST_CASE("unterminated-quote-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em>x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left:2em&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // a missing ':' separator renders as literal text
    TEST_CASE("missing-colon-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left 2em\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left&nbsp;2em&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // plunity backend maps the div back to a TMP margin tag (see the blocks above)

    // nested divs are handled and map to nested margin tags
    TEST_CASE("nested-div-margin-tags") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<div style=\"margin-left:2em\"><div style=\"margin-right:1em\">x</div></div>");
        auto const& answer = u8"<margin left=2em><margin right=1em>x</margin>\n</margin>\n";
        CHECK(html == answer);
    }

    // the title backend (pltxt2common_html) renders the block-level div literally
    TEST_CASE("title-backend-literal-div") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<div style=\"margin-left:2em\">text</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;text&lt;/div&gt;";
        CHECK(html == answer);
    }

    // regression: roundtrip fuzzer crash (fixedadv_roundtrip, crash-6250118fd3e7f955112700997404af6e9d791b89).
    // The web backend emits a unity_margin block as <div style="margin-left:...;margin-right:...">, but the
    // experimental HTML parser (parse_pltxt_html) used by the second roundtrip pass does not understand <div>,
    // so on the second pass the emitted <div> is re-parsed as literal text and escaped. The two passes diverge:
    //     first pass : x<br><div style="margin-left:2px;margin-right:2px;"></div>
    //     second pass: x<br>&lt;div&nbsp;style=&quot;margin-left:2px;margin-right:2px;&quot;&gt;&lt;/div&gt;
    // The roundtrip is therefore not idempotent. The test below asserts the idempotency the fuzzer checks and
    // currently reproduces the crash; it should pass once the HTML parser learns <div>.
    TEST_CASE("roundtrip-idempotency-crash") {
        auto const& pltext = u8"x\n<MARgin=2>";
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::mnp::os_c_str(once));
        CHECK(twice == once);
    }
    // the exact fuzzer input (span-wrapped margin block), without the libFuzzer trailing-junk bytes
    TEST_CASE("fuzzer-margin-roundtrip") {
        auto const& pltext = u8",><sIzE=4>[8t<sIzE\n<MARgin=2>";
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::mnp::os_c_str(once));
        CHECK(twice == once);
    }

    // the experimental HTML parser (parse_pltxt_html) understands <div style="margin-..."> too,
    // matching the main parser's behaviour (block-level only, margin-left/right only)
    TEST_CASE("html-parser-margin-block") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em\">text</div>");
        auto const& answer = u8"<div style=\"margin-left:2em;\">text</div>";
        CHECK(html == answer);
    }

    TEST_CASE("html-parser-both-margins") {
        auto html =
            ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>");
        auto const& answer = u8"<div style=\"margin-left:2em;margin-right:3em;\">text</div>";
        CHECK(html == answer);
    }

    TEST_CASE("html-parser-newline-br") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"margin-left:2em\">a</div>\nb");
        auto const& answer = u8"<div style=\"margin-left:2em;\">a</div><br>b";
        CHECK(html == answer);
    }

    TEST_CASE("html-parser-surrounding-text") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"a\n<div style=\"margin-left:2em\">b</div>\nc");
        auto const& answer = u8"a<br><div style=\"margin-left:2em;\">b</div><br>c";
        CHECK(html == answer);
    }

    // mid-line and unknown-style <div> stay literal text in the experimental parser
    TEST_CASE("html-parser-midline-div") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"t<div style=\"margin-left:2em\">x</div>t");
        auto const& answer = u8"t&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("html-parser-unknown-style") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<div style=\"color:red\">x</div>");
        auto const& answer = u8"&lt;div&nbsp;style=&quot;color:red&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    // a div nested inside a span (emitted by the roundtrip backend for an open margin scope)
    TEST_CASE("span-wrapped-div-preserved") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<span style=\"font-size:2px;\">a<br><div style=\"margin-left:2px;\"></div></span>");
        auto const& answer = u8"<span style=\"font-size:2px;\">a<br><div style=\"margin-left:2px;\"></div></span>";
        CHECK(html == answer);
    }

    // nested divs map to nested margin scopes in the experimental parser
    TEST_CASE("nested-div-experimental") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<div style=\"margin-left:2em\"><div style=\"margin-right:1em\">x</div></div>");
        auto const& answer = u8"<div style=\"margin-left:2em;\"><div style=\"margin-right:1em;\">x</div></div>";
        CHECK(html == answer);
    }
}
