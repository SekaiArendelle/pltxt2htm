#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2html(u8"<p>text</p>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<p>text</p>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2html(u8"<P    >text</P  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<p>text</p>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2html(u8"<p><color=red>text</color></p>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text</span></p>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2html(u8"<p><color=red>text</p></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text&lt;/p&gt;</span></p>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2html(u8"<p>text<p>text</p></p>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<p>texttext</p>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2html(u8"<p>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
