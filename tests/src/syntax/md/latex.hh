#pragma once

#include "doctest_config.hh"

TEST_SUITE("latex") {
    // simple inline math
    TEST_CASE("inline-basic") {
        auto const& pltext = u8"$x^2$";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"$x^2$";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"$x^2$";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // inline math with spaces
    TEST_CASE("inline-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$x + y$");
        auto const& answer = u8"$x + y$";
        CHECK(html == answer);
    }

    // block math on single line $$...$$
    TEST_CASE("block-single-line") {
        auto const& pltext = u8"$$x^2$$";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"$$x^2$$";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"$$x^2$$";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("lone-delimiters") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$");
        auto const& answer = u8"$$";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-block") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$x^2");
        auto const& answer = u8"$$x^2";
        CHECK(html == answer);
    }

    // block math with newlines
    TEST_CASE("block-multiline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$");
        auto const& answer = u8"$$\na+b\nc$$";
        CHECK(html == answer);
    }

    // mixed with regular text
    TEST_CASE("mixed-with-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A $x$ B");
        auto const& answer = u8"A&nbsp;$x$&nbsp;B";
        CHECK(html == answer);
    }

    TEST_CASE("block-followed-by-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$test");
        auto const& answer = u8"$$\na+b\nc$$test";
        CHECK(html == answer);
    }

    // roundtrip mode must not double-skip nodes after $...$
    // regression test for roundtrip fuzzer finding
    TEST_CASE("roundtrip-inline-no-double-skip") {
        auto const& pltext = u8"$a$b";
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& answer = u8"b";
        CHECK(html == answer);
    }
    // same for md_latex_block
    TEST_CASE("roundtrip-block-no-double-skip") {
        auto const& pltext = u8"$$a$$b";
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& answer = u8"b";
        CHECK(html == answer);
    }
}
