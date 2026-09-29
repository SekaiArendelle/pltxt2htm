#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_del_tag") {
    TEST_CASE("basic-del-passthrough") {
        auto const& pltext = u8"<del>text</del>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del>text</del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s>text</s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("case-insensitive-spacing") {
        auto const& pltext = u8"<DEL    >text</DEL  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del>text</del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s>text</s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-color-inside") {
        auto const& pltext = u8"<Del><color=red>text</color></Del>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del><span style=\"color:red;\">text</span></del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s><color=red>text</color></s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("close-order-mismatch") {
        auto const& pltext = u8"<del><color=red>text</del></color>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del><span style=\"color:red;\">text&lt;/del&gt;</span></del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<s><color=red>text<size=20>\uFF1C</size>/del<size=20>\uFF1E</size></color></s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("same-tag-flattened") {
        auto const& pltext = u8"<Del>text<del>text</del></Del>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del>texttext</del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s>texttext</s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-empty-dropped") {
        auto const& pltext = u8"text<del>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto const& pltext = u8"t<del></del>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("incomplete-close-literal") {
        auto const& pltext = u8"<del></del";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del>&lt;/del</del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s><size=20>\uFF1C</size>/del</s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
}
