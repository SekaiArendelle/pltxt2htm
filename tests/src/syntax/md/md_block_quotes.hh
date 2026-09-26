#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_block_quotes") {
    TEST_CASE("> text") {
        auto const& pltext = u8"> text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<blockquote>text</blockquote>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<margin left=2em>text</margin>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("> text > test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> text\n > test");
        auto const& answer = u8"<blockquote>text<br>test</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> text > test (3)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> text\n > test\n");
        auto const& answer = u8"<blockquote>text<br>test</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> text > test test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> text\n > test\ntest\n");
        auto const& answer = u8"<blockquote>text<br>test</blockquote>test<br>";
        CHECK(html == answer);
    }

    TEST_CASE("> # hello") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> # hello");
        auto const& answer = u8"<blockquote><h1>hello</h1></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> hello > text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> hello\n\n > text");
        auto const& answer = u8"<blockquote>hello</blockquote><br><blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> # hello > # test > # text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"> # hello\n > # test\n > # text");
        auto const& answer = u8"<blockquote><h1>hello</h1><h1>test</h1><h1>text</h1></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> text (8)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n> text");
        auto const& answer = u8"<br><blockquote>text</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("# > quote") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# \n> quote");
        auto const& answer = u8"<h1></h1><blockquote>quote</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("--- > quote") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---\n> quote");
        auto const& answer = u8"<hr><blockquote>quote</blockquote>";
        CHECK(html == answer);
    }

    // empty block quotes is undocumented in spec.commonmark.org, this is an implement defined behavior
    TEST_CASE("empty block quotes is undocumented in spec.commonmark.or...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8">");
        auto const& answer = u8"&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("case 12") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" > ");
        auto const& answer = u8"&nbsp;&gt;";
        CHECK(html == answer);
    }

    TEST_CASE(">> test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" >> test");
        auto const& answer = u8"<blockquote><blockquote>test</blockquote></blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" \t> \ttest");
        auto const& answer = u8"<blockquote>test</blockquote>";
        CHECK(html == answer);
    }

    TEST_CASE("> test test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" > test\ntest");
        auto const& answer = u8"<blockquote>test</blockquote>test";
        CHECK(html == answer);
    }

    TEST_CASE("> test (16)") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"> test");
        auto const& answer = u8"<margin left=2em>test</margin>\n";
        CHECK(html == answer);
    }
}
