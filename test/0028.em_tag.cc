#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<em>text</em>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<em>text</em>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<Em    >text</eM  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<em>text</em>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<em><color=red>text</color></em>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text</span></em>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<em><color=red>text</em></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text&lt;/em&gt;</span></em>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<em>text<em>text</em></em>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<em>texttext</em>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"text<em>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8"text"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"t<em></em>t", u8"localhost:5173");
    auto answer7 = ::fast_io::u8string_view{u8"tt"};
    ::exception::assert_true(html7 == answer7);

    return 0;
}
