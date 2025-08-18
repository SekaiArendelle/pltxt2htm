#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>text</i>");
    auto answer1 = ::fast_io::u8string_view{u8"<em>text</em>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<I    >text</I  >");
    auto answer2 = ::fast_io::u8string_view{u8"<em>text</em>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i><color=red>text</color></i>");
    auto answer3 = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text</span></em>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i><color=red>text</i></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text&lt;/i&gt;</span></em>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>text<i>text</i></i>");
    auto answer5 = ::fast_io::u8string_view{u8"<em>texttext</em>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>");
    auto answer6 = ::fast_io::u8string_view{u8""};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i");
    auto answer7 = ::fast_io::u8string_view{u8"&lt;i"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<i></i>t");
    auto answer8 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em>te<i>st</i></em>");
    auto answer9 = ::fast_io::u8string_view{u8"<em>test</em>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    return 0;
}