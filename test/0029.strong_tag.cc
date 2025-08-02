#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong>text</strong>");
    auto answer1 = ::fast_io::u8string_view{u8"<strong>text</strong>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Strong    >text</sTrong  >");
    auto answer2 = ::fast_io::u8string_view{u8"<strong>text</strong>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong><color=red>text</color></strong>");
    auto answer3 = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong><color=red>text</strong></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text&lt;/strong&gt;</span></strong>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong>text<strong>text</strong></strong>");
    auto answer5 = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"text<strong>");
    auto answer6 = ::fast_io::u8string_view{u8"text"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<strong></strong>t");
    auto answer7 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}
