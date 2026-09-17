#pragma once

#include "doctest_config.hh"

TEST_CASE("tab") {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\ta");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&nbsp;&nbsp;&nbsp;a"};
        CHECK(html == answer);
    }
}

