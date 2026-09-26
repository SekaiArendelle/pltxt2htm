#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_u_tag") {
    TEST_CASE("basic-underline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text</u>");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<U    >text</U  >");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-color-inside") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</color></u>");
        auto const& answer = u8"<u><span style=\"color:red;\">text</span></u>";
        CHECK(html == answer);
    }

    TEST_CASE("close-order-mismatch") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</u></color>");
        auto const& answer = u8"<u><span style=\"color:red;\">text&lt;/u&gt;</span></u>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-tag-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text<u>text</u></u>");
        auto const& answer = u8"<u>texttext</u>";
        CHECK(html == answer);
    }

    TEST_CASE("inside-bold") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><u>text</u></b>");
        auto const& answer = u8"<strong><u>text</u></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-open-tag-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u");
        auto const& answer = u8"&lt;u";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<u></u>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        // unclosed <u> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<u>test</u>cd");
        auto const& answer = u8"ab<u>test</u>cd";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-nested-flattened") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<u>t1<u>t2</u></u>");
        auto const& answer = u8"<u>t1t2</u>";
        CHECK(html == answer);
    }
}
