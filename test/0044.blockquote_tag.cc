#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>text</blockquote>");
    auto answer1 = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<BLOCKQUOTE    >text</BlockQuote  >");
    auto answer2 = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote><color=red>text</color></blockquote>");
    auto answer3 = ::fast_io::u8string_view{u8"<blockquote><span style=\"color:red;\">text</span></blockquote>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote><color=red>text</blockquote></color>");
    auto answer4 = ::fast_io::u8string_view{
        u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
    auto answer5 = ::fast_io::u8string_view{u8"<blockquote>text<blockquote>text</blockquote></blockquote>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>");
    auto answer6 = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote");
    auto answer7 = ::fast_io::u8string_view{u8"&lt;blockquote"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<blockquote></blockquote>t");
    auto answer8 = ::fast_io::u8string_view{u8"t<blockquote></blockquote>t"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    return 0;
}