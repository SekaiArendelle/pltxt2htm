#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_s_tag") {
    TEST_CASE("<s>text</s>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text</s>");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("<S >text</S >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<S    >text</S  >");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("<s><color=red>text</color></s>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</color></s>");
        auto const& answer = u8"<s><span style=\"color:red;\">text</span></s>";
        CHECK(html == answer);
    }

    TEST_CASE("<s><color=red>text</s></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</s></color>");
        auto const& answer = u8"<s><span style=\"color:red;\">text&lt;/s&gt;</span></s>";
        CHECK(html == answer);
    }

    TEST_CASE("<s>text<s>text</s></s>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text<s>text</s></s>");
        auto const& answer = u8"<s>texttext</s>";
        CHECK(html == answer);
    }

    TEST_CASE("<b><s>text</s></b>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><s>text</s></b>");
        auto const& answer = u8"<strong><s>text</s></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("<s>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("<s") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s");
        auto const& answer = u8"&lt;s";
        CHECK(html == answer);
    }

    TEST_CASE("t<s></s>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<s></s>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed <s> tag") {
        // unclosed <s> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("strikethrough tags from different syntaxes can nest") {
        // strikethrough tags from different syntaxes can nest
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>t1<del>t2</del>t3</s>");
        auto const& answer = u8"<s>t1<del>t2</del>t3</s>";
        CHECK(html == answer);
    }

    TEST_CASE("~~t1<s>t2</s>t3~~") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~t1<s>t2</s>t3~~");
        auto const& answer = u8"<del>t1<s>t2</s>t3</del>";
        CHECK(html == answer);
    }

    TEST_CASE("ab<s>test</s>cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<s>test</s>cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }

    TEST_CASE("<s>t1<s>t2</s></s>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<s>t1<s>t2</s></s>");
        auto const& answer = u8"<s>t1t2</s>";
        CHECK(html == answer);
    }

    TEST_CASE("ab<del>test</del>cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<del>test</del>cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }

    TEST_CASE("ab~~test~~cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab~~test~~cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }
}
