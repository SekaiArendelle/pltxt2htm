#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<!--es-->t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<!--es-->t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<!-- est");
        auto answer = ::fast_io::u8string_view{u8"t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<!");
        auto answer = ::fast_io::u8string_view{u8"&lt;!"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
