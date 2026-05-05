#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"~~test~~"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<del>test</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st~~");
        auto answer = ::fast_io::u8string_view{u8"te<del>st</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st\n~~");
        auto answer = ::fast_io::u8string_view{u8"te~~st<br>~~"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t~~e~~st");
        auto answer = ::fast_io::u8string_view{u8"t<del>e</del>st"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*t~~es~~t*");
        auto answer = ::fast_io::u8string_view{u8"<em>t<del>es</del>t</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
