#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_macro") {
    TEST_CASE("project-macro") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Project}test");
        auto const& answer = u8"test$PROJECTtest";
        CHECK(html == answer);
    }

    TEST_CASE("visitor-macro") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Visitor}test");
        auto const& answer = u8"test$VISITORtest";
        CHECK(html == answer);
    }

    TEST_CASE("author-macro") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Author}test");
        auto const& answer = u8"test$AUTHORtest";
        CHECK(html == answer);
    }

    TEST_CASE("coauthors-macro") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{CoAuthors}test");
        auto const& answer = u8"test$CO_AUTHORStest";
        CHECK(html == answer);
    }

    TEST_CASE("apostrophe-escaping") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{Project}", u8"localhost", u8"'", u8"", u8"", u8"");
        auto const& answer = u8"&apos;";
        CHECK(html == answer);
    }

    TEST_CASE("all-macros-mixed") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"{Project}{Visitor}{Author}{CoAuthors}", u8"project",
                                                                 u8"visitor", u8"author", u8"coauthors");
        auto const& answer = u8"projectvisitorauthorcoauthors";
        CHECK(html == answer);
    }
}
