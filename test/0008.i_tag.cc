#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<i>text</i>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<i>text</i>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<I    >text</I  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<i>text</i>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<i><color=red>text</color></i>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<i><span style=\"color:red;\">text</span></i>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<i><color=red>text</i></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<i><span style=\"color:red;\">text&lt;/i&gt;</span></i>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<i>text<i>text</i></i>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<i>texttext</i>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"<i>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
