#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2html(u8"<del>text</del>", u8"localhost");
    auto answer1 = ::fast_io::u8string_view{u8"<del>text</del>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2html(u8"<DEL    >text</DEL  >", u8"localhost");
    auto answer2 = ::fast_io::u8string_view{u8"<del>text</del>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2html(u8"<Del><color=red>text</color></Del>", u8"localhost");
    auto answer3 = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text</span></del>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2html(u8"<del><color=red>text</del></color>", u8"localhost");
    auto answer4 = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text&lt;/del&gt;</span></del>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2html(u8"<Del>text<del>text</del></Del>", u8"localhost");
    auto answer5 = ::fast_io::u8string_view{u8"<del>texttext</del>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2html(u8"text<del>", u8"localhost");
    auto answer6 = ::fast_io::u8string_view{u8"text"};
    ::exception::assert_true(html6 == answer6);

    return 0;
}
