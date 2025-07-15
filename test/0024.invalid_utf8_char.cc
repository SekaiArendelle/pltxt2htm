#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2common_html(u8"Hello");
    auto answer1 = ::fast_io::u8string_view{u8"Hello"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2common_html(u8"café");
    auto answer2 = ::fast_io::u8string_view{u8"café"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2common_html(u8"中文");
    auto answer3 = ::fast_io::u8string_view{u8"中文"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2common_html(u8"😊😊");
    auto answer4 = ::fast_io::u8string_view{u8"😊😊"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2common_html(u8"\xc3");
    auto answer5 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2common_html(u8"\xc3\xa");
    auto answer6 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2common_html(u8"\xe0\x80\x80");
    auto answer7 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm::pltxt2common_html(u8"\xc0\x80");
    auto answer8 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2common_html(u8"\xe2\x82");
    auto answer9 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm::pltxt2common_html(u8"\xf4\x90\x80\x80");
    auto answer10 = ::fast_io::u8string_view{u8"�"};
    ::exception::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm::pltxt2common_html(u8"\xed\xa0\x80");
    auto answer11 = ::fast_io::u8string_view{u8"��"};
    ::exception::assert_true(html11 == answer11);

    return 0;
}
