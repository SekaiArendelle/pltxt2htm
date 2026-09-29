#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_margin_tag") {
    // web backend renders <margin...> as a block-level div with CSS margins
    TEST_CASE("left-margin-div") {
        auto const& pltext = u8"<margin-left=2em>text</margin>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("right-margin-div") {
        auto const& pltext = u8"<margin-right=1em>text</margin>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-right:1em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin right=1em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("both-sides") {
        auto const& pltext = u8"<margin=2em>text</margin>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;margin-right:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em right=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a newline inside a margin block still renders as <br>
    TEST_CASE("newline-inside-block") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>line1\nline2</margin>");
        auto const& answer = u8"<div style=\"margin-left:2em;\">line1<br>line2</div>";
        CHECK(html == answer);
    }

    // text after the closing </margin> stays on its own content line
    TEST_CASE("text-after-close") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>a</margin>\nb");
        auto const& answer = u8"<div style=\"margin-left:2em;\">a</div><br>b";
        CHECK(html == answer);
    }

    // mid-line <margin> sequences are literal text (block-level only)
    TEST_CASE("mid-line-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<margin-left=2em>x</margin>t");
        auto const& answer = u8"t&lt;margin-left=2em&gt;x&lt;/margin&gt;t";
        CHECK(html == answer);
    }

    // a non-numeric value renders as literal text
    TEST_CASE("non-numeric-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=abc>x</margin>");
        auto const& answer = u8"&lt;margin-left=abc&gt;x&lt;/margin&gt;";
        CHECK(html == answer);
    }

    // an empty margin block is kept, not erased by the optimizer
    TEST_CASE("empty-block-kept") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em></margin>");
        auto const& answer = u8"<div style=\"margin-left:2em;\"></div>";
        CHECK(html == answer);
    }

    // an unclosed margin block still parses
    TEST_CASE("unclosed-tag-auto-closed") {
        auto const& pltext = u8"<margin-left=2em>text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // plunity backend emits TMP margin tags (see the blocks above)

    // px is the default unit and is emitted without a suffix
    TEST_CASE("px-default-unit") {
        auto const& pltext = u8"<margin=10px>text</margin>";
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& answer = u8"<margin left=10 right=10>text</margin>\n";
        CHECK(html == answer);
        auto web_html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& web_answer = u8"<div style=\"margin-left:10px;margin-right:10px;\">text</div>";
        CHECK(web_html == web_answer);
    }

    // percent unit is preserved
    TEST_CASE("percent-unit-preserved") {
        auto const& pltext = u8"<margin=5%>text</margin>";
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& answer = u8"<margin left=5% right=5%>text</margin>\n";
        CHECK(html == answer);
        auto web_html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& web_answer = u8"<div style=\"margin-left:5%;margin-right:5%;\">text</div>";
        CHECK(web_html == web_answer);
    }

    // web backend emits px and percent CSS units explicitly (see the blocks above)

    // zero is a valid margin value
    TEST_CASE("zero-value") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=0>text</margin>");
        auto const& answer = u8"<margin left=0 right=0>text</margin>\n";
        CHECK(html == answer);
    }

    // tag name matching is case-insensitive (TMP accepts <MARGIN-LEFT>)
    TEST_CASE("case-insensitive-tag") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<MARGIN-LEFT=2em>text</MARGIN>");
        auto const& answer = u8"<margin left=2em>text</margin>\n";
        CHECK(html == answer);
    }

    // text after the closing </margin> starts on a new line in the plunity backend
    TEST_CASE("plunity-text-after-close") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em>a</margin>text");
        auto const& answer = u8"<margin left=2em>a</margin>\ntext";
        CHECK(html == answer);
    }

    // two adjacent block-level margin tags are separated by a line break
    TEST_CASE("adjacent-blocks-separated") {
        auto const& pltext = u8"<margin-left=1em>a</margin>\n<margin-right=2em>b</margin>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:1em;\">a</div><br><div style=\"margin-right:2em;\">b</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=1em>a</margin>\n\n<margin right=2em>b</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text before and after a block-level margin
    TEST_CASE("text-around-block") {
        auto const& pltext = u8"a\n<margin-left=2em>b</margin>\nc";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><div style=\"margin-left:2em;\">b</div><br>c";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<margin left=2em>b</margin>\n\nc";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an empty margin between two lines keeps its block boundary
    TEST_CASE("empty-block-between-lines") {
        auto const& pltext = u8"a\n<margin-left=2em></margin>\nb";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><div style=\"margin-left:2em;\"></div><br>b";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<margin left=2em></margin>\n\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // the attribute form <margin left=V right=W> round-trips through the plunity backend
    TEST_CASE("attribute-form-roundtrip") {
        auto const& pltext = u8"<margin left=2em right=2em>text</margin>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<div style=\"margin-left:2em;margin-right:2em;\">text</div>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em right=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // left and right margins can differ
    TEST_CASE("different-sides") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em right=3em>text</margin>");
        auto const& answer = u8"<margin left=2em right=3em>text</margin>\n";
        CHECK(html == answer);
    }

    // the attribute form may specify only one side
    TEST_CASE("single-side") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em>text</margin>");
        auto const& answer = u8"<margin left=2em>text</margin>\n";
        CHECK(html == answer);
    }

    TEST_CASE("right-only-attribute") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin right=1em>text</margin>");
        auto const& answer = u8"<margin right=1em>text</margin>\n";
        CHECK(html == answer);
    }

    // attribute names are case-insensitive (TMP uppercases the tag buffer)
    TEST_CASE("case-insensitive-attribute") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin LEFT=2em RIGHT=3em>text</margin>");
        auto const& answer = u8"<margin left=2em right=3em>text</margin>\n";
        CHECK(html == answer);
    }

    // <margin=V> and <margin left=V right=V> are equivalent
    TEST_CASE("value-form-equals-attribute-form") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=2em>text</margin>");
        auto const& answer = u8"<margin left=2em right=2em>text</margin>\n";
        CHECK(html == answer);
        auto html_attributes =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em right=2em>text</margin>");
        CHECK(html_attributes == answer);
    }

    // a repeated attribute is malformed and renders as literal text
    TEST_CASE("repeated-attribute-literal") {
        auto const& pltext = u8"<margin left=1em left=2em>text</margin>";
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& answer =
            u8"<size=20>\uff1c</size>margin\u00A0left=1em\u00A0left=2em<size=20>\uff1e</size>text"
            u8"<size=20>\uff1c</size>/margin<size=20>\uff1e</size>";
        CHECK(html == answer);
        auto web_html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& web_answer = u8"&lt;margin&nbsp;left=1em&nbsp;left=2em&gt;text&lt;/margin&gt;";
        CHECK(web_html == web_answer);
    }

    // a newline inside an attribute-form margin block still renders as <br>
    TEST_CASE("attribute-form-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=2em right=3em>line1\nline2</margin>");
        auto const& answer = u8"<div style=\"margin-left:2em;margin-right:3em;\">line1<br>line2</div>";
        CHECK(html == answer);
    }

    // mid-line attribute-form <margin> sequences are literal text (block-level only)
    TEST_CASE("mid-line-attribute-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<margin left=2em>x</margin>t");
        auto const& answer = u8"t&lt;margin&nbsp;left=2em&gt;x&lt;/margin&gt;t";
        CHECK(html == answer);
    }

    // a malformed attribute value renders as literal text
    TEST_CASE("malformed-attribute-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=abc right=2em>x</margin>");
        auto const& answer = u8"&lt;margin&nbsp;left=abc&nbsp;right=2em&gt;x&lt;/margin&gt;";
        CHECK(html == answer);
    }

    // an unknown attribute name renders as literal text
    TEST_CASE("unknown-attribute-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin foo=2em>x</margin>");
        auto const& answer = u8"&lt;margin&nbsp;foo=2em&gt;x&lt;/margin&gt;";
        CHECK(html == answer);
    }

    // a missing value renders as literal text
    TEST_CASE("missing-attribute-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=2em right>x</margin>");
        auto const& answer = u8"&lt;margin&nbsp;left=2em&nbsp;right&gt;x&lt;/margin&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-margin-blocks") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em><margin-right=2em>x</margin></margin>");
        auto const& answer = u8"<margin left=2em><margin right=2em>x</margin>\n</margin>\n";
        CHECK(html == answer);
    }
}
