#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_s_tag") {
    TEST_CASE("basic-strikethrough") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text</s>");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("case-insensitive-extra-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<S    >text</S  >");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-color-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</color></s>");
        auto const& answer = u8"<s><span style=\"color:red;\">text</span></s>";
        CHECK(html == answer);
    }

    TEST_CASE("close-order-mismatch") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</s></color>");
        auto const& answer = u8"<s><span style=\"color:red;\">text&lt;/s&gt;</span></s>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-tag-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text<s>text</s></s>");
        auto const& answer = u8"<s>texttext</s>";
        CHECK(html == answer);
    }

    TEST_CASE("inside-bold") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><s>text</s></b>");
        auto const& answer = u8"<strong><s>text</s></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-open-tag-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s");
        auto const& answer = u8"&lt;s";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<s></s>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        // unclosed <s> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text");
        auto const& answer = u8"<s>text</s>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-markdown-del") {
        // strikethrough tags from different syntaxes can nest
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>t1<del>t2</del>t3</s>");
        auto const& answer = u8"<s>t1<del>t2</del>t3</s>";
        CHECK(html == answer);
    }

    TEST_CASE("del-wrapping-s-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~t1<s>t2</s>t3~~");
        auto const& answer = u8"<del>t1<s>t2</s>t3</del>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<s>test</s>cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-nested-flattened") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<s>t1<s>t2</s></s>");
        auto const& answer = u8"<s>t1t2</s>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-del-becomes-s") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<del>test</del>cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-markdown-del") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab~~test~~cd");
        auto const& answer = u8"ab<s>test</s>cd";
        CHECK(html == answer);
    }
}
