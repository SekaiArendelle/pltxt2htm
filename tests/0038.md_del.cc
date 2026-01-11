#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~test~~");
        auto answer = ::fast_io::u8string_view{u8"<del>test</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te~~st~~");
        auto answer = ::fast_io::u8string_view{u8"te<del>st</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te~~st\n~~");
        auto answer = ::fast_io::u8string_view{u8"te~~st<br>~~"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t~~e~~st");
        auto answer = ::fast_io::u8string_view{u8"t<del>e</del>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"*t~~es~~t*");
        auto answer = ::fast_io::u8string_view{u8"<em>t<del>es</del>t</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
