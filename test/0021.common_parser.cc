#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2common_htmld(u8"");
    auto answer1 = ::fast_io::u8string_view{u8""};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">test</span>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
    auto answer3 = ::fast_io::u8string_view{u8"test"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
    auto answer4 = ::fast_io::u8string_view{u8"<strong>test</strong>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
    auto answer5 = ::fast_io::u8string_view{u8"<em>test</em>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2common_htmld(u8"<color=red><Color=#66CcFf>text</color></color>");
    auto answer6 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
    auto answer7 = ::fast_io::u8string_view{u8"t<strong>ex</strong>t"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}
