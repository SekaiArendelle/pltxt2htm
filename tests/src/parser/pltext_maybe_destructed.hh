#pragma once

#include "doctest_config.hh"

TEST_SUITE("pltext_maybe_destructed") {
    TEST_CASE("blockquote-with-link") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> [a](example.com)");
        CHECK(html == u8"<blockquote><a href=\"example.com\">a</a></blockquote>");
    }
    TEST_CASE("blockquote-with-color") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> <color=red>test</color>");
        CHECK(html == u8"<blockquote><span style=\"color:red;\">test</span></blockquote>");
    }
    TEST_CASE("blockquote-with-experiment") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"> <experiment=642cf37a494746375aae306a>physicsLab</experiment>");
        CHECK(html ==
              u8"<blockquote><a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" "
              u8"internal>physicsLab</a></blockquote>");
    }
    TEST_CASE("blockquote-with-external") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"> <external=https://example.com/discussion>physicsLab</external>");
        CHECK(html == u8"<blockquote><a href=\"https://example.com/discussion\">physicsLab</a></blockquote>");
    }
}
