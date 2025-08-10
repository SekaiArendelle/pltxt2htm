#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>text</h4>");
    auto answer1 = ::fast_io::u8string_view{u8"<h4>text</h4>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H4    >text</H4  >");
    auto answer2 = ::fast_io::u8string_view{u8"<h4>text</h4>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4><color=red>text</color></h4>");
    auto answer3 = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text</span></h4>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4><color=red>text</h4></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text&lt;/h4&gt;</span></h4>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>text<h4>text</h4></h4>");
    auto answer5 = ::fast_io::u8string_view{u8"<h4>texttext</h4>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>");
    auto answer6 = ::fast_io::u8string_view{u8"<h4></h4>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    return 0;
}
