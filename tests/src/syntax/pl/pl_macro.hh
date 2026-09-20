#pragma once

#include "doctest_config.hh"

TEST_CASE("pl_macro") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Project}test");
        auto answer = ::fast_io::u8string_view{u8"test$PROJECTtest"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Visitor}test");
        auto answer = ::fast_io::u8string_view{u8"test$VISITORtest"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{Author}test");
        auto answer = ::fast_io::u8string_view{u8"test$AUTHORtest"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test{CoAuthors}test");
        auto answer = ::fast_io::u8string_view{u8"test$CO_AUTHORStest"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{Project}", u8"localhost", u8"'", u8"", u8"", u8"");
        auto answer = ::fast_io::u8string_view{u8"&apos;"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"{Project}{Visitor}{Author}{CoAuthors}", u8"project",
                                                                 u8"visitor", u8"author", u8"coauthors");
        auto answer = ::fast_io::u8string_view{u8"projectvisitorauthorcoauthors"};
        CHECK(html == answer);
    }
}
