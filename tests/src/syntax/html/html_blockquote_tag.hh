#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_blockquote_tag") {
    TEST_CASE("<blockquote>text</blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text</blockquote>");
        auto const& answer = u8"<blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<BLOCKQUOTE >text</BlockQuote >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<BLOCKQUOTE    >text</BlockQuote  >");
        auto const& answer = u8"<blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote><color=red>text</color></blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</color></blockquote>");
        auto const& answer = u8"<blockquote><span style=\"color:red;\">text</span></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote><color=red>text</blockquote></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</blockquote></color>");
        auto const& answer = u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote>text<blockquote>text</blockquote></blockquot...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto const& answer = u8"<blockquote>text&lt;blockquote&gt;text</blockquote>&lt;/blockquote&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>");
        auto const& answer = u8"<blockquote></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote");
        auto const& answer = u8"&lt;blockquote";
        CHECK(html == answer);
    }

    TEST_CASE("t<blockquote></blockquote>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<blockquote></blockquote>t");
        auto const& answer = u8"t&lt;blockquote&gt;&lt;/blockquote&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("ab<blockquote>test</blockquote>cd") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<blockquote>test</blockquote>cd");
        auto const& answer = u8"ab&lt;blockquote&gt;test&lt;/blockquote&gt;cd";
        CHECK(html == answer);
    }

    TEST_CASE("text <blockquote>text</blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n<blockquote>text</blockquote>");
        auto const& answer = u8"text<br><blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("text<br><blockquote>text</blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<br><blockquote>text</blockquote>");
        auto const& answer = u8"text<br><blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote><blockquote>text</blockquote></blockquote>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><blockquote>text</blockquote></blockquote>");
        auto const& answer = u8"<blockquote><blockquote>text</blockquote></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("<blockquote><blockquote>text</blockquote>text</blockquot...") {
        auto const& pltext = u8"<blockquote><blockquote>text</blockquote>text</blockquote>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<blockquote><blockquote>text</blockquote>text</blockquote>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em><margin left=2em>text</margin>\ntext</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<br> <blockquote>text</blockquote>") {
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<br>\n<blockquote>text</blockquote>");
        auto const& plunity_richtext_answer = u8"\n\n<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
}
