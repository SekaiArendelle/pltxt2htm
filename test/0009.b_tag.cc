#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>text</b>");
    auto answer1 = ::fast_io::u8string_view{u8"<strong>text</strong>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<B    >text</b  >");
    auto answer2 = ::fast_io::u8string_view{u8"<strong>text</strong>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b><color=red>text</color></B>");
    auto answer3 = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b><color=red>text</b></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text&lt;/b&gt;</span></strong>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>text<b>text</b></b>");
    auto answer5 = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    // Optimization example: empty tag
    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<b></b>t");
    auto answer7 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}