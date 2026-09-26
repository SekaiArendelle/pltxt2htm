#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_br_tag") {
    auto const& text =
        u8R"(
文本1<br   />文本2<bR/>文本3<br   >文本4<br    /
文本5<Br<br>
)";

    TEST_CASE("br-normalization") {
        auto html = ::pltxt2htm_test::pltxt4unittest(text);
        auto const& answer =
            u8R"(<br>文本1<br>文本2<br>文本3<br>文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/<br>文本5&lt;Br<br><br>)";
        CHECK(html == answer);
    }

    TEST_CASE("common-htmld-strips-br") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(text);
        auto const& answer = u8R"(文本1文本2文本3文本4&lt;br&nbsp;&nbsp;&nbsp;&nbsp;/文本5&lt;Br)";
        CHECK(html == answer);
    }

    TEST_CASE("tab-before-self-close") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br\t/>text");
        auto const& answer = u8"<br>text";
        CHECK(html == answer);
    }
}
