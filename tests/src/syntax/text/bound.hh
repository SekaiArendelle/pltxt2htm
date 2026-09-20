#pragma once

#include "doctest_config.hh"

TEST_CASE("bound") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<");
        auto answer = ::fast_io::u8string_view{u8"test&lt;"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test\\");
        auto answer = ::fast_io::u8string_view{u8"test\\"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test</");
        auto answer = ::fast_io::u8string_view{u8"test&lt;/"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        CHECK(html == answer);
    }
}
