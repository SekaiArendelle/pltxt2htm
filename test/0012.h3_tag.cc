#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2html(u8"<h3>text</h3>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<h3>text</h3>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2html(u8"<H3    >text</h3  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<h3>text</h3>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2html(u8"<h3><color=red>text</color></h3>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text</span></h3>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2html(u8"<h3><color=red>text</h3></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text&lt;/h3&gt;</span></h3>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2html(u8"<h3>text<h3>text</h3></h3>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<h3>texttext</h3>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2html(u8"<h3>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
