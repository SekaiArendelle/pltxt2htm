#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~test~~");
    auto answer1 = ::fast_io::u8string_view{u8"<del>test</del>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te~~st~~");
    auto answer2 = ::fast_io::u8string_view{u8"te<del>st</del>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te~~st\n~~");
    auto answer3 = ::fast_io::u8string_view{u8"te~~st<br>~~"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t~~e~~st");
    auto answer4 = ::fast_io::u8string_view{u8"t<del>e</del>st"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"*t~~es~~t*");
    auto answer5 = ::fast_io::u8string_view{u8"<em>t<del>es</del>t</em>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    return 0;
}
