#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"*test*");
        auto answer = ::fast_io::u8string_view{u8"<em>test</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t*e*st");
        auto answer = ::fast_io::u8string_view{u8"t<em>e</em>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t**e**st");
        auto answer = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // Test triple emphasis with asterisks ***text***
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"***test***");
        auto answer = ::fast_io::u8string{u8"<em><strong>test</strong></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t***e***st");
        auto answer = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t_e_st");
        auto answer = ::fast_io::u8string_view{u8"t<em>e</em>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t__e__st");
        auto answer = ::fast_io::u8string_view{u8"t<strong>e</strong>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t___e___st");
        auto answer = ::fast_io::u8string_view{u8"t<em><strong>e</strong></em>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t_e*st");
        auto answer = ::fast_io::u8string_view{u8"t_e*st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"*test\n*");
        auto answer = ::fast_io::u8string_view{u8"*test<br>*"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"**test\n**");
        auto answer = ::fast_io::u8string_view{u8"**test<br>**"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te***st\n***");
        auto answer = ::fast_io::u8string_view{u8"te***st<br><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // Test triple emphasis with underscores ___text___
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"___test___");
        auto answer = ::fast_io::u8string{u8"<em><strong>test</strong></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t****t");
        auto answer = ::fast_io::u8string{u8"t****t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t***t");
        auto answer = ::fast_io::u8string{u8"t***t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t*****t");
        auto answer = ::fast_io::u8string{u8"t*****t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
