#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_sup_tag") {
    TEST_CASE("basic-tag-passthrough") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text</sup>");
        auto const& answer = u8"<sup>text</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-spacing-normalized") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SUP    >text</SUP  >");
        auto const& answer = u8"<sup>text</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("math-exponent-inline") {
        auto const& pltext = u8"x<sup>2</sup>+y<sup>2</sup>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"x<sup>2</sup>+y<sup>2</sup>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"x<sup>2</sup>+y<sup>2</sup>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("properly-nested-color") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup><color=red>text</color></sup>");
        auto const& answer = u8"<sup><span style=\"color:red;\">text</span></sup>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-close-order") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup><color=red>text</sup></color>");
        auto const& answer = u8"<sup><span style=\"color:red;\">text&lt;/sup&gt;</span></sup>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-not-flattened") {
        // nested <sup> must NOT be flattened: the inner text shifts the baseline further
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text<sup>text</sup></sup>");
        auto const& answer = u8"<sup>text<sup>text</sup></sup>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-with-trailing-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text<sup>text</sup>text</sup>");
        auto const& answer = u8"<sup>text<sup>text</sup>text</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("bold-to-strong-wrapper") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><sup>text</sup></b>");
        auto const& answer = u8"<strong><sup>text</sup></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("bare-open-tag-empty") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup");
        auto const& answer = u8"&lt;sup";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-removed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<sup></sup>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-auto-closed") {
        // unclosed <sup> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sup>text");
        auto const& answer = u8"<sup>text</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("nesting-with-underline") {
        // <sup> can nest with other formatting tags
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>t1<sup>t2</sup>t3</u>");
        auto const& answer = u8"<u>t1<sup>t2</sup>t3</u>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-nested-passthrough") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<sup>t1<sup>t2</sup></sup>");
        auto const& answer = u8"<sup>t1<sup>t2</sup></sup>";
        CHECK(html == answer);
    }
}
