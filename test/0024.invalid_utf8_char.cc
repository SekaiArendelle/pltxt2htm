#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2common_htmld(u8"Hello");
    auto answer1 = ::fast_io::u8string_view{u8"Hello"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2common_htmld(u8"café");
    auto answer2 = ::fast_io::u8string_view{u8"café"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2common_htmld(u8"中文");
    auto answer3 = ::fast_io::u8string_view{u8"中文"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2common_htmld(u8"😊😊");
    auto answer4 = ::fast_io::u8string_view{u8"😊😊"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc3");
    auto answer5 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc3\xa");
    auto answer6 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xe0\x80\x80");
    auto answer7 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc0\x80");
    auto answer8 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xe2\x82");
    auto answer9 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf4\x90\x80\x80");
    auto answer10 = ::fast_io::u8string_view{u8"�"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2common_htmld(u8"\xed\xa0\x80");
    auto answer11 = ::fast_io::u8string_view{u8"��"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    return 0;
}
