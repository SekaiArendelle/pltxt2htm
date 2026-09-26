#pragma once

#include "doctest_config.hh"

TEST_SUITE("rm_tail_space") {
    TEST_CASE("trailing-spaces-before-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \n");
        auto const& answer = u8"t<br>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-tabs-before-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t\n");
        auto const& answer = u8"t<br>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-trailing-whitespace") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \t\t\n");
        auto const& answer = u8"t<br>";
        CHECK(html == answer);
    }

    TEST_CASE("only-whitespace-before-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  \t\t\n");
        auto const& answer = u8"<br>";
        CHECK(html == answer);
    }

    TEST_CASE("inner-space-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t t \n");
        auto const& answer = u8"t&nbsp;t<br>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-spaces-at-eof") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  ");
        auto const& answer = u8"t";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-tabs-at-eof") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t");
        auto const& answer = u8"t";
        CHECK(html == answer);
    }

    TEST_CASE("only-tabs-at-eof") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\t\t");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("only-spaces-at-eof") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  ");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("two-lines-trailing-space") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt \n");
        auto const& answer = u8"t<br>t<br>";
        CHECK(html == answer);
    }

    TEST_CASE("second-line-trailing-space") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt ");
        auto const& answer = u8"t<br>t";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-plain-text") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-trailing-space-trimmed") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto const& answer = u8"text\ntext";
        CHECK(html == answer);
    }

    TEST_CASE("code-block-trailing-space") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<p><code>t \nt \n</code> </p>");
        auto const& answer = u8"<p style=\"text-align:left\"><code>t<br>t<br></code></p>";
        CHECK(html == answer);
    }
}
