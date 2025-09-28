#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2common_htmld(u8"t<!--es-->t");
    auto answer1 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<!--es-->t");
    auto answer2 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<!-- est");
    auto answer3 = ::fast_io::u8string_view{u8"t"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<!");
    auto answer4 = ::fast_io::u8string_view{u8"&lt;!"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    return 0;
}
