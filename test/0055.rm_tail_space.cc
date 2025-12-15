#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t  \n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t\t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t t \n");
        auto answer = ::fast_io::u8string_view{u8"t&nbsp;t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
