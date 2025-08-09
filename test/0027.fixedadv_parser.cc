#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a>text</a>");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a href=\"https://plweb.turtlesim.com\">text</a>");
    auto answer2 = ::fast_io::u8string_view{u8"<a&nbsp;href=&quot;https://plweb.turtlesim.com&quot;&gt;text</a&gt;"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    return 0;
}