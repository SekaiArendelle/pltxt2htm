#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<h1>text</h1>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<h1>text</h1>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<H1    >text</h1  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<h1>text</h1>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<h1><color=red>text</color></h1>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<h1><span style=\"color:red;\">text</span></h1>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<h1><color=red>text</h1></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<h1><span style=\"color:red;\">text&lt;/h1&gt;</span></h1>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<h1>text<h1>text</h1></h1>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<h1>texttext</h1>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"<h1>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8"<h1></h1>"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"# <h1>text", u8"localhost");
    auto answer7 = ::fast_io::u8string_view{u8"<h1>text</h1>"};
    ::exception::assert_true(html7 == answer7);

    return 0;
}
