#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"*test*");
    auto answer1 = ::fast_io::u8string_view{u8"<em>test</em>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t*e*st");
    auto answer2 = ::fast_io::u8string_view{u8"t<em>e</em>st"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t**e**st");
    auto answer3 = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t***e***st");
    auto answer4 = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t_e_st");
    auto answer5 = ::fast_io::u8string_view{u8"t<em>e</em>st"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t__e__st");
    auto answer6 = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t___e___st");
    auto answer7 = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t_e*st");
    auto answer8 = ::fast_io::u8string_view{u8"t<em>est</em>"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    return 0;
}
