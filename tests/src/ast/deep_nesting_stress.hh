#pragma once

#include "doctest_config.hh"

TEST_SUITE("deep_nesting_stress") {
    TEST_CASE("deeply nested color tags") {
        ::pltxt2htm::container::U8String input;
        for (::std::size_t i{}; i < 500; ++i) {
            input.append(u8"<color=red>");
        }
        input.append(u8"hello");
        for (::std::size_t i{}; i < 500; ++i) {
            input.append(u8"</color>");
        }
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(::pltxt2htm::container::U8StringView{input});
        auto const& answer = u8"<span style=\"color:red;\">hello</span>";
        CHECK(html == answer);
    }
}
