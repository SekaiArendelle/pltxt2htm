#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_thematic_break") {
    TEST_CASE("leading-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n---");
        auto const& answer = u8"<br><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-space-then-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n--- \nt");
        auto const& answer = u8"<br><hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("br-after-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n--- <Br />t");
        auto const& answer = u8"<br><hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("br-before-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />---");
        auto const& answer = u8"<br><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("br-before-and-text-after") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />--- \nt");
        auto const& answer = u8"<br><hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("br-both-sides") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />--- <Br />t");
        auto const& answer = u8"<br><hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("basic") {
        auto const& pltext = u8"---";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<hr>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"---\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text-after") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--- \nt");
        auto const& answer = u8"<hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--- <Br />t");
        auto const& answer = u8"<hr>t";
        CHECK(html == answer);
    }

    TEST_CASE("underscore-spaced") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"_ _  _");
        auto const& answer = u8"<hr>";
        CHECK(html == answer);
    }

    TEST_CASE("asterisk-spaced") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" ** * ***");
        auto const& answer = u8"<hr>";
        CHECK(html == answer);
    }

    TEST_CASE("after-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test\n ---");
        auto const& answer = u8"<h1>test</h1><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("after-heading-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<br> ---");
        auto const& answer = u8"<h1>test</h1><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("leading-newline-after-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# test\n ---");
        auto const& answer = u8"<br><h1>test</h1><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># test\n ---");
        auto const& answer = u8"<br><h1>test</h1><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("between-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---\n# test\n ---");
        auto const& answer = u8"<hr><h1>test</h1><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("three-breaks") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---\n---\n ---");
        auto const& answer = u8"<hr><hr><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-break-then-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> ---\n # test");
        auto const& answer = u8"<br><hr><h1>test</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("two-breaks-leading-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n---\n---");
        auto const& answer = u8"<br><hr><hr>";
        CHECK(html == answer);
    }

    TEST_CASE("break-br-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /> # test");
        auto const& answer = u8"<br><hr><h1>test</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("break-br-heading-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /> # test\ntest");
        auto const& answer = u8"<br><hr><h1>test</h1>test";
        CHECK(html == answer);
    }

    TEST_CASE("break-br-empty-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /># \ntest");
        auto const& answer = u8"<br><hr><h1></h1>test";
        CHECK(html == answer);
    }

    TEST_CASE("break-br-two-empty-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /># \n# \ntest");
        auto const& answer = u8"<br><hr><h1></h1><h1></h1>test";
        CHECK(html == answer);
    }

    TEST_CASE("br-break-br-empty-heading") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> --- <Br /># \ntest");
        auto const& answer = u8"<br><hr><h1></h1>test";
        CHECK(html == answer);
    }

    TEST_CASE("br-break-br-two-empty-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> --- <Br /># \n# \ntest");
        auto const& answer = u8"<br><hr><h1></h1><h1></h1>test";
        CHECK(html == answer);
    }

    TEST_CASE("two-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# t\n# t");
        auto const& answer = u8"<br><h1>t</h1><h1>t</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("br-then-two-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># t\n# t");
        auto const& answer = u8"<br><h1>t</h1><h1>t</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("three-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# t\n# t\n# t");
        auto const& answer = u8"<br><h1>t</h1><h1>t</h1><h1>t</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("hash-as-content-headings") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n#  #\n# #\n # #");
        auto const& answer = u8"<br><h1>#</h1><h1>#</h1><h1>#</h1>";
        CHECK(html == answer);
    }

    TEST_CASE("dash-tab-separated") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n-\t-\t-\ntext");
        auto const& answer = u8"<br><hr>text";
        CHECK(html == answer);
    }

    // negative cases: NOT thematic breaks

    TEST_CASE("single-dash-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - ");
        auto const& answer = u8"<ul><li></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("two-dashes-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--");
        auto const& answer = u8"--";
        CHECK(html == answer);
    }
    TEST_CASE("dash-text-dash-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"-a-");
        auto const& answer = u8"-a-";
        CHECK(html == answer);
    }
    TEST_CASE("single-asterisk-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*");
        auto const& answer = u8"*";
        CHECK(html == answer);
    }
    TEST_CASE("single-underscore-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" _ ");
        auto const& answer = u8"&nbsp;_";
        CHECK(html == answer);
    }
    TEST_CASE("two-dashes-spaced-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - -");
        auto const& answer = u8"<ul><li>-</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("trailing-char-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---a");
        auto const& answer = u8"---a";
        CHECK(html == answer);
    }
    TEST_CASE("leading-char-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a---");
        auto const& answer = u8"a---";
        CHECK(html == answer);
    }
    TEST_CASE("asterisks-with-char-not-break") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***a");
        auto const& answer = u8"***a";
        CHECK(html == answer);
    }
}
