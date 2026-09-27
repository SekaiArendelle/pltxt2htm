#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_h5_tag") {
    TEST_CASE("wellformed-tag-preserved") {
        auto const& pltext = u8"<h5>text</h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5>text</h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b>text</b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-whitespace-normalize") {
        auto const& pltext = u8"<H5    >text</H5  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5>text</h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b>text</b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-color-in-heading") {
        auto const& pltext = u8"<h5><color=red>text</color></h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5><span style=\"color:red;\">text</span></h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b><color=red>text</color></b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("misnested-close-escaped") {
        auto const& pltext = u8"<h5><color=red>text</h5></color>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5><span style=\"color:red;\">text&lt;/h5&gt;</span></h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<b><color=red>text<size=20>\uFF1C</size>/h5<size=20>\uFF1E</size></color></b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-same-tag-escaped") {
        auto const& pltext = u8"<h5>text<h5>text</h5></h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5>text&lt;h5&gt;text</h5>&lt;/h5&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<b>text<size=20>\uFF1C</size>h5<size=20>\uFF1E</size>text</b>\n"
            u8"<size=20>\uFF1C</size>/h5<size=20>\uFF1E</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-heading-autoclose") {
        auto const& pltext = u8"<h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5></h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b></b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unterminated-close-escaped") {
        auto const& pltext = u8"<h5></h5";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h5>&lt;/h5</h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<b><size=20>\uFF1C</size>/h5</b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("hash-heading-literal-tag") {
        auto const& pltext = u8"# <h5>text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>&lt;h5&gt;text</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=38><b><size=20>\uFF1C</size>h5<size=20>\uFF1E</size>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("same-line-tags-literal") {
        auto const& pltext = u8"t<h5></h5>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;h5&gt;&lt;/h5&gt;t";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>h5<size=20>\uFF1E</size><size=20>\uFF1C</size>/h5<size=20>\uFF1E</size>t";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("same-line-unclosed-literal") {
        auto const& pltext = u8"t<h5></h5";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;h5&gt;&lt;/h5";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>h5<size=20>\uFF1E</size><size=20>\uFF1C</size>/h5";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("newline-before-heading") {
        auto const& pltext = u8"text\n<h5>text</h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><h5>text</h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\n<b>text</b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("br-before-heading") {
        auto const& pltext = u8"text<br><h5>text</h5>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><h5>text</h5>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\n<b>text</b>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("experimental-parser-parity") {
        auto const& pltext = u8"text<br><h5>text</h5>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto const& answer = u8"text<br><h5>text</h5>";
        CHECK(html == answer);
    }
}
