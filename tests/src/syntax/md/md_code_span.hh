#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_code_span") {
    TEST_CASE("basic") {
        auto const& pltext = u8"`test`";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<code>test</code>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("inner-space") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t t`");
        auto const& answer = u8"<code>t&nbsp;t</code>";
        CHECK(html == answer);
    }

    TEST_CASE("surrounded-by-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t`e`st");
        auto const& answer = u8"t<code>e</code>st";
        CHECK(html == answer);
    }

    TEST_CASE("double-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``test``");
        auto const& answer = u8"<code>test</code>";
        CHECK(html == answer);
    }

    TEST_CASE("triple-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```test```");
        auto const& answer = u8"<code>test</code>";
        CHECK(html == answer);
    }

    TEST_CASE("escaped-backtick-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t\\`t`");
        auto const& answer = u8"<code>t`t</code>";
        CHECK(html == answer);
    }

    TEST_CASE("double-backtick-surrounded") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t``e``st");
        auto const& answer = u8"t<code>e</code>st";
        CHECK(html == answer);
    }

    TEST_CASE("triple-backtick-surrounded") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t```e```st");
        auto const& answer = u8"t<code>e</code>st";
        CHECK(html == answer);
    }

    // An opening backtick without a matching closing backtick is literal text,
    // not an unterminated code span.
    TEST_CASE("unmatched-backtick-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t");
        auto const& answer = u8"`t";
        CHECK(html == answer);
    }

    TEST_CASE("html-inside-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```<br>```");
        auto const& answer = u8"<code>&lt;br&gt;</code>";
        CHECK(html == answer);
    }

    TEST_CASE("tags-inside-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`<em>t</em>`");
        auto const& answer = u8"<code>&lt;em&gt;t&lt;/em&gt;</code>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-mono-font") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab`test`cd");
        auto const& answer = u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-double-backtick") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab``test``cd");
        auto const& answer = u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-triple-backtick") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab```test```cd");
        auto const& answer = u8"ab<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>cd";
        CHECK(html == answer);
    }

    // Regression: an opening delimiter without a matching closing delimiter must not be
    // accepted as a code span. Before the fix, the 2-backtick branch in the inline parser
    // subtracted the delimiters from the consumed count and underflowed to a huge size_t,
    // terminating (quick_enforce) or forming an out-of-bounds subview (ignore). See
    // fixedadv_fuzzer_crash_analysis.md. Delimiter lengths 1, 2, 3 at top level.
    TEST_CASE("lone-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`");
        auto const& answer = u8"`";
        CHECK(html == answer);
    }
    // A delimiter run with no content is NOT a code span and stays literal text.
    TEST_CASE("lone-double-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``");
        auto const& answer = u8"``";
        CHECK(html == answer);
    }
    TEST_CASE("lone-triple-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```");
        auto const& answer = u8"```";
        CHECK(html == answer);
    }
    // Content long enough to fill an unclosed span still must not be consumed by it.
    TEST_CASE("unclosed-with-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`t");
        auto const& answer = u8"`t";
        CHECK(html == answer);
    }
    TEST_CASE("unclosed-double-with-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``ab");
        auto const& answer = u8"``ab";
        CHECK(html == answer);
    }
    TEST_CASE("unclosed-triple-with-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"```x");
        auto const& answer = u8"```x";
        CHECK(html == answer);
    }
    // A backslash-escaped backtick at the end is consumed as content, not a closing delimiter.
    TEST_CASE("escaped-backtick-at-end") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"`a\\`");
        auto const& answer = u8"`a`";
        CHECK(html == answer);
    }
    // Even balanced delimiter runs with no content stay literal.
    TEST_CASE("lone-quadruple-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"````");
        auto const& answer = u8"````";
        CHECK(html == answer);
    }
    TEST_CASE("lone-sextuple-backtick") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"``````");
        auto const& answer = u8"``````";
        CHECK(html == answer);
    }
    // The fuzzer crash input: an unclosed code span inside a Markdown list item.
    TEST_CASE("unclosed-in-list-item") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* `");
        auto const& answer = u8"<ul><li>`</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("unclosed-double-in-list-item") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* ``");
        auto const& answer = u8"<ul><li>``</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("unclosed-triple-in-list-item") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* ```");
        auto const& answer = u8"<ul><li>```</li></ul>";
        CHECK(html == answer);
    }
    // Unclosed code span inside a table cell.
    TEST_CASE("unclosed-in-table-cell") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"| a |\n|---|\n| `` |");
        auto const& answer = u8"<table><thead><tr><th>a</th></tr></thead><tbody><tr><td>``</td></tr></tbody></table>";
        CHECK(html == answer);
    }
}
