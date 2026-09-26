#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_h3_tag") {
    TEST_CASE("wellformed-tag-preserved") {
        auto const& pltext = u8"<h3>text</h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3>text</h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=36><b>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-whitespace-normalize") {
        auto const& pltext = u8"<H3    >text</h3  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3>text</h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=36><b>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-color-in-heading") {
        auto const& pltext = u8"<h3><color=red>text</color></h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3><span style=\"color:red;\">text</span></h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=36><b><color=red>text</color></b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("misnested-close-escaped") {
        auto const& pltext = u8"<h3><color=red>text</h3></color>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3><span style=\"color:red;\">text&lt;/h3&gt;</span></h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=36><b><color=red>text<size=20>\uFF1C</size>/h3<size=20>\uFF1E</size></color></b></"
            u8"size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-same-tag-escaped") {
        auto const& pltext = u8"<h3>text<h3>text</h3></h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3>text&lt;h3&gt;text</h3>&lt;/h3&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=36><b>text<size=20>\uFF1C</size>h3<size=20>\uFF1E</size>text</b></size>\n"
            u8"<size=20>\uFF1C</size>/h3<size=20>\uFF1E</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-heading-autoclose") {
        auto const& pltext = u8"<h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3></h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=36><b></b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unterminated-close-escaped") {
        auto const& pltext = u8"<h3></h3";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h3>&lt;/h3</h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=36><b><size=20>\uFF1C</size>/h3</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("hash-heading-literal-tag") {
        auto const& pltext = u8"# <h3>text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<h1>&lt;h3&gt;text</h1>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=38><b><size=20>\uFF1C</size>h3<size=20>\uFF1E</size>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("same-line-tags-literal") {
        auto const& pltext = u8"t<h3></h3>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;h3&gt;&lt;/h3&gt;t";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>h3<size=20>\uFF1E</size><size=20>\uFF1C</size>/h3<size=20>\uFF1E</size>t";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("same-line-unclosed-literal") {
        auto const& pltext = u8"t<h3></h3";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;h3&gt;&lt;/h3";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>h3<size=20>\uFF1E</size><size=20>\uFF1C</size>/h3";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("newline-before-heading") {
        auto const& pltext = u8"text\n<h3>text</h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><h3>text</h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\n<size=36><b>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("br-before-heading") {
        auto const& pltext = u8"text<br><h3>text</h3>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><h3>text</h3>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\n<size=36><b>text</b></size>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("experimental-parser-parity") {
        auto const& pltext = u8"text<br><h3>text</h3>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto const& answer = u8"text<br><h3>text</h3>";
        CHECK(html == answer);
    }
}
