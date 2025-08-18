#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li>item1</li><li>item2</li></ul>");
    auto answer1 = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<UL    ><LI>item1</LI><LI>item2</LI></UL  >");
    auto answer2 = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li><color=red>item1</color></li><li>item2</li></ul>");
    auto answer3 =
        ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1</span></li><li>item2</li></ul>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li><color=red>item1</li></ul></color>");
    auto answer4 =
        ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1&lt;/li&gt;&lt;/ul&gt;</span></li></ul>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
    auto answer5 = ::fast_io::u8string_view{u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<ul>");
    auto answer6 = ::fast_io::u8string_view{u8"<ul></ul>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<ul></ul>t");
    auto answer7 = ::fast_io::u8string_view{u8"t<ul></ul>t"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    return 0;
}