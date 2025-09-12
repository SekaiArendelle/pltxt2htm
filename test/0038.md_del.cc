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

    return 0;
}
