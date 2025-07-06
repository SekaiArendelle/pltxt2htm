#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<h4>text</h4>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<h4>text</h4>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<H4    >text</H4  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<h4>text</h4>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(u8"<h4><color=red>text</color></h4>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text</span></h4>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<h4><color=red>text</h4></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text&lt;/h4&gt;</span></h4>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<h4>text<h4>text</h4></h4>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<h4>texttext</h4>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"<h4>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
