#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"t<!--es-->t"};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<!--es-->t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<!-- est");
        auto answer = ::fast_io::u8string_view{u8"t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<!");
        auto answer = ::fast_io::u8string_view{u8"&lt;!"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
