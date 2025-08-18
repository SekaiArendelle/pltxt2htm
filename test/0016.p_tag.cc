#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>text</p>");
    auto answer1 = ::fast_io::u8string_view{u8"<p>text</p>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<P    >text</P  >");
    auto answer2 = ::fast_io::u8string_view{u8"<p>text</p>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p><color=red>text</color></p>");
    auto answer3 = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text</span></p>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p><color=red>text</p></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<p><span style=\"color:red;\">text&lt;/p&gt;</span></p>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>text<p>text</p></p>");
    auto answer5 = ::fast_io::u8string_view{u8"<p>text<p>text</p></p>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<p>");
    auto answer6 = ::fast_io::u8string_view{u8"<p></p>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<p></p>t");
    auto answer7 = ::fast_io::u8string_view{u8"t<p></p>t"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}
