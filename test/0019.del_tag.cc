#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<del>text</del>");
    auto answer1 = ::fast_io::u8string_view{u8"<del>text</del>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<DEL    >text</DEL  >");
    auto answer2 = ::fast_io::u8string_view{u8"<del>text</del>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Del><color=red>text</color></Del>");
    auto answer3 = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text</span></del>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<del><color=red>text</del></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text&lt;/del&gt;</span></del>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Del>text<del>text</del></Del>");
    auto answer5 = ::fast_io::u8string_view{u8"<del>texttext</del>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"text<del>");
    auto answer6 = ::fast_io::u8string_view{u8"text"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<del></del>t");
    auto answer7 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}
