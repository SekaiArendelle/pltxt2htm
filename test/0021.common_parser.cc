#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2common_html(u8"");
    auto answer1 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2common_html(u8"<a>test");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">test</span>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2common_html(u8"<h3>test");
    auto answer3 = ::fast_io::u8string_view{u8"test"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2common_html(u8"<b>test");
    auto answer4 = ::fast_io::u8string_view{u8"<b>test</b>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2common_html(u8"<i>test");
    auto answer5 = ::fast_io::u8string_view{u8"<i>test</i>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 =
        ::pltxt2htm::pltxt2advanced_html(u8"<color=red><Color=#66CcFf>text</color></color>", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
