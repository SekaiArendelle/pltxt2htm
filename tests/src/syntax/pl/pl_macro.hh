#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_macro") {
    TEST_CASE("project-macro") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"test{Project}test");
        auto const& answer = u8"test$PROJECTtest";
        CHECK(html == answer);
    }

    TEST_CASE("visitor-macro") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"test{Visitor}test");
        auto const& answer = u8"test$VISITORtest";
        CHECK(html == answer);
    }

    TEST_CASE("author-macro") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"test{Author}test");
        auto const& answer = u8"test$AUTHORtest";
        CHECK(html == answer);
    }

    TEST_CASE("coauthors-macro") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"test{CoAuthors}test");
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

    TEST_CASE("project-placeholder-escaped") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{project}", u8"localhost:5173", u8"<img src=x onerror=alert(1)>",
                                                   u8"visitor", u8"author", u8"coauthors");
        auto const& answer = u8"&lt;img src=x onerror=alert(1)&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("author-placeholders-escaped") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{visitor}{author}{coauthors}", u8"localhost:5173", u8"project", u8"<svg/onload=alert(2)>",
            u8"<script>alert(3)</script>", u8"<iframe src=javascript:alert(4)></iframe>");
        auto const& answer =
            u8"&lt;svg/onload=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;iframe "
            u8"src=javascript:alert(4)&gt;&lt;/iframe&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("all-placeholders-escaped") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{project}{visitor}{author}{coauthors}", u8"localhost:5173", u8"<svg/onload=alert(1)>",
            u8"<img src=x onerror=alert(2)>", u8"<script>alert(3)</script>", u8"<a href=javascript:alert(4)>x</a>");
        auto const& answer =
            u8"&lt;svg/onload=alert(1)&gt;&lt;img src=x onerror=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;a "
            u8"href=javascript:alert(4)&gt;x&lt;/a&gt;";
        CHECK(html == answer);
    }
}
