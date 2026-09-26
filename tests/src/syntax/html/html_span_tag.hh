#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_span_tag") {
    TEST_CASE("color-style-conversion") {
        auto const& pltext = u8"<span style=\"color:red\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=red>text</color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("font-size-px-richtext") {
        auto const& pltext = u8"<span style=\"font-size:20px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=40>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("combined-color-font-size") {
        auto const& pltext = u8"<span style=\"color:blue;font-size:16px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:blue;font-size:16px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=blue><size=32>text</size></color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("style-whitespace-normalize") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\" ; color \t : \t red \t ; ; font-size : 20px ; \">text</span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-style-attr-rejected") {
        // uppercase STYLE attribute is rejected (only lowercase "style" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span STYLE=\"color:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;STYLE=&quot;color:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-color-property-rejected") {
        // uppercase COLOR property is rejected (only lowercase "color" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"COLOR:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;COLOR:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("tag-internal-whitespace") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span  style=\"color:red\"  >text</span  >");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("important-flag-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red !important\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red&nbsp;!important&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("hex-color-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#FF0000\">text</span>");
        auto const& answer = u8"<span style=\"color:#FF0000;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unitless-font-size-px") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20\">text</span>");
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-semicolon-normalized") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;\">text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("class-attribute-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span class=\"foo\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;class=&quot;foo&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unsupported-background-property") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"background:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;background:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("invalid-hex-color") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#GGG\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:#GGG&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em-unit-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1em\">text</span>");
        auto const& answer = u8"<span style=\"font-size:1em;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate-color-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;color:blue\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red;color:blue&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate-font-size-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20px;font-size:30px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20px;font-size:30px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("percent-font-size-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:80%\">text</span>");
        auto const& answer = u8"<span style=\"font-size:80%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color-with-percent-size") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:blue;font-size:80%\">text</span>");
        auto const& answer = u8"<span style=\"color:blue;font-size:80%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-unit-duplicate-size") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:80%;font-size:30px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:80%;font-size:30px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("styled-empty-tag-removed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<span style=\"color:red\"></span>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-color-flatten") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("same-color-siblings-merge") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:red\">b</span>c</span>");
        auto const& answer = u8"<span style=\"color:red;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("inner-color-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:blue\">text</span></span>");
        auto const& answer = u8"<span style=\"color:blue;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-distinct-colors") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:blue\">b</span></span>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"color:blue;\">b</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("strong-preserves-inner-color") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><span style=\"color:red\">text</span></strong>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text</span></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-without-url-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a><span style=\"color:red\">text</span></a>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("outer-color-fully-overridden") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=green><span style=\"color:red\">text</span></color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("matching-color-flattens") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"color:red\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-absorbed-into-color") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#0000AA;\">a<a>b</a>c</span>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-color-merges-inner") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-outer-inner-differ") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:red;\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">a<span style=\"color:red;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-color-overrides-wrap") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><a>text</a></span>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("inner-color-overrides-outer") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><color=blue>text</color></span>");
        auto const& answer = u8"<span style=\"color:blue;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("matched-font-size-flatten") {
        // font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"font-size:20px\">text</span></span>");
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("matched-combined-style-flatten") {
        // color+font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:20px\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("differing-size-inner-wins") {
        // color+font-size, font-size different: should NOT flatten, inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:16px\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:16px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unmatched-font-size-preserved") {
        // color=red wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"font-size:20px\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("size-blocks-color-flatten") {
        // color=red wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<color=red>a<span style=\"color:red;font-size:20px\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"color:red;font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unmatched-size-in-anchor") {
        // anchor wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"font-size:20px\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">a<span style=\"font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("disjoint-attrs-merge") {
        // non-overlapping nested attrs: merge outer font-size with inner color
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"color:red\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("triple-nested-flatten") {
        // triple nested same color: should flatten to single span
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\"><span "
            u8"style=\"color:red\">text</span></span></span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-nested-style-removed") {
        // empty same-color nested span: should remove both (nothing left)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><span style=\"color:red\"></span></span>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-size-blocks-flatten") {
        // anchor wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;font-size:20px\">b</span>c</a>");
        auto const& answer =
            u8"<span style=\"color:#0000AA;\">a<span style=\"color:#0000AA;font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-autoclose") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">text");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red&quot;";
        CHECK(html == answer);
    }

    TEST_CASE("font-size-overflow") {
        // overflow font-size: span tag rejected, treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:99999999999999999999px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:99999999999999999999px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("single-quoted-attribute") {
        // single-quoted style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style='color:red'>text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("zero-font-size-rejected") {
        // font-size:0 rejected (zero is not a valid font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:0\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:0&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("empty-style-rejected") {
        // empty style value rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-px-unit-rejected") {
        // uppercase px unit rejected (lowercase "px" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20PX\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20PX&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-unit-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20p\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20p&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unit-extra-char-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20pxx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20pxx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-color-accepted") {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:Red\">text</span>");
        auto const& answer = u8"<span style=\"color:Red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("all-caps-color-accepted") {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:BLUE\">text</span>");
        auto const& answer = u8"<span style=\"color:BLUE;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("negative-font-size-rejected") {
        // negative font-size rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:-20px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:-20px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unrepresentable-size-literal") {
        // font-size that cannot be represented in double/std::size_t stays literal
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:18446744073709551614px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:18446744073709551614px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-px-accepted") {
        // fractional font-size accepted and round-tripped
        auto const& pltext = u8"<span style=\"font-size:12.5px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:12.5px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=25>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("fractional-em-accepted") {
        // fractional font-size with em unit
        auto const& pltext = u8"<span style=\"font-size:1.5em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:1.5em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=1.5em>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("fractional-percent-accepted") {
        // fractional font-size with percent unit
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:87.5%\">text</span>");
        auto const& answer = u8"<span style=\"font-size:87.5%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("leading-dot-rejected") {
        // leading-dot font-size rejected (stays literal)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:.5px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:.5px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-dot-rejected") {
        // font-size with empty fractional part rejected (stays literal)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:12.px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:12.px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("super-dropped-in-richtext") {
        auto const& pltext = u8"<span style=\"vertical-align:super\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:super;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("keyword-sub-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:sub\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:sub;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-baseline-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:baseline\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:baseline;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-text-top-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:text-top\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:text-top;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-text-bottom-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:text-bottom\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:text-bottom;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-middle-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:middle\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:middle;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-top-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:top\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:top;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-bottom-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:bottom\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:bottom;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("px-offset-maps-voffset") {
        auto const& pltext = u8"<span style=\"vertical-align:5px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:5px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<voffset=5>text</voffset>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("percent-offset-dropped") {
        auto const& pltext = u8"<span style=\"vertical-align:20%\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:20%;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("combined-color-size-align") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px;vertical-align:super\">text</span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-offset-dropped") {
        auto const& pltext = u8"<span style=\"vertical-align:10em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:10em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-vertical-align-rejected") {
        // uppercase vertical-align property rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"VERTICAL-ALIGN:super\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;VERTICAL-ALIGN:super&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-keyword-rejected") {
        // uppercase keyword rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:SUPER\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:SUPER&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("keyword-trailing-char-rejected") {
        // keyword with trailing alphanumeric rejected ("superx" is not a keyword)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:superx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:superx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-keyword-rejected") {
        // unknown keyword rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:foo\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:foo&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("zero-offset-rejected") {
        // zero length rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:0\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:0&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("negative-offset-accepted") {
        // negative length supported (vertical-align: -5px)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:-5px\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate-align-rejected") {
        // duplicate vertical-align rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:super;vertical-align:sub\">text</span>");
        auto const& answer =
            u8"&lt;span&nbsp;style=&quot;vertical-align:super;vertical-align:sub&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("matched-align-flatten") {
        // matched nested vertical-align: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:super\"><span style=\"vertical-align:super\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("differing-align-inner-wins") {
        // differing nested vertical-align: inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:super\"><span style=\"vertical-align:sub\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:sub;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("differing-offset-inner-wins") {
        // differing nested vertical-align lengths: inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:5px\"><span style=\"vertical-align:10px\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:10px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("disjoint-color-align-merge") {
        // non-overlapping nested attrs: merge outer color with inner vertical-align
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"vertical-align:super\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("align-blocks-color-flatten") {
        // span with matching color AND vertical-align inside <color=red> should NOT flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<color=red>a<span style=\"color:red;vertical-align:super\">b</span>c</color>");
        auto const& answer =
            u8"<span style=\"color:red;\">a<span style=\"color:red;vertical-align:super;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor-align-blocks-flatten") {
        // span with matching color AND vertical-align inside <a> should NOT flatten
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;vertical-align:super\">b</span>c</a>");
        auto const& answer =
            u8"<span style=\"color:#0000AA;\">a<span style=\"color:#0000AA;vertical-align:super;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unity-combined-style-tags") {
        // color+font-size+px vertical-align combined in Unity output
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<span style=\"color:red;font-size:16px;vertical-align:5px\">text</span>");
        auto const& answer = u8"<color=red><size=32><voffset=5>text</voffset></size></color>";
        CHECK(html == answer);
    }

    TEST_CASE("em-font-size-supported") {
        // em font-size is supported
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20em\">text</span>");
        auto const& answer = u8"<span style=\"font-size:20em;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("combined-color-em-size") {
        auto const& pltext = u8"<span style=\"color:blue;font-size:16em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:blue;font-size:16em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=blue><size=16em>text</size></color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-em-unit-rejected") {
        // uppercase em unit rejected (lowercase "em" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20EM\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20EM&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-em-rejected") {
        // partial em rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1e\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1e&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em-trailing-char-rejected") {
        // em with trailing character rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1emx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1emx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate-m-suffix-rejected") {
        // duplicate 'm' rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1emm\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1emm&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("zero-em-size-rejected") {
        // font-size:0em rejected (zero is not a valid font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:0em\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:0em&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unity-em-size-mapping") {
        // em font-size maps to <size=Nem> in Unity output
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<span style=\"font-size:16em\">text</span>");
        auto const& answer = u8"<size=16em>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("em-font-size-roundtrip") {
        // em font-size roundtrip through the HTML parser
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<span style=\"font-size:16em\">text</span>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("em-offset-roundtrip") {
        // em vertical-align roundtrip through the HTML parser
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<span style=\"vertical-align:10em\">text</span>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("matched-em-size-flatten") {
        // matched nested em font-size: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:1em\"><span style=\"font-size:1em\">text</span></span>");
        auto const& answer = u8"<span style=\"font-size:1em;\">text</span>";
        CHECK(html == answer);
    }
}
