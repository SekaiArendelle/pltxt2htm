#pragma once

#include "doctest_config.hh"

TEST_CASE("latex") {
    // simple inline math
    {
        auto const& pltext = u8"$x^2$";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"$x^2$";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"$x^2$";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // inline math with spaces
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$x + y$");
        auto const& answer = u8"$x + y$";
        CHECK(html == answer);
    }

    // block math on single line $$...$$
    {
        auto const& pltext = u8"$$x^2$$";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"$$x^2$$";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"$$x^2$$";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$");
        auto const& answer = u8"$$";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$x^2");
        auto const& answer = u8"$$x^2";
        CHECK(html == answer);
    }

    // block math with newlines
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$");
        auto const& answer = u8"$$\na+b\nc$$";
        CHECK(html == answer);
    }

    // mixed with regular text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A $x$ B");
        auto const& answer = u8"A&nbsp;$x$&nbsp;B";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$test");
        auto const& answer = u8"$$\na+b\nc$$test";
        CHECK(html == answer);
    }

    // roundtrip mode must not double-skip nodes after $...$
    // regression test for roundtrip fuzzer finding
    {
        auto const& pltext = u8"$a$b";
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& answer = u8"b";
        CHECK(html == answer);
    }
    // same for md_latex_block
    {
        auto const& pltext = u8"$$a$$b";
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto const& answer = u8"b";
        CHECK(html == answer);
    }
}
