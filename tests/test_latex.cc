#include "precompile.hh"

int main() {
    // simple inline math
    {
        auto pltext = ::fast_io::u8string_view{u8"$x^2$"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"$x^2$"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"$x^2$"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // inline math with spaces
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$x + y$");
        auto answer = ::fast_io::u8string_view{u8"$x + y$"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // block math on single line $$...$$
    {
        auto pltext = ::fast_io::u8string_view{u8"$$x^2$$"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"$$x^2$$"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"$$x^2$$"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$");
        auto answer = ::fast_io::u8string_view{u8"$$"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$x^2");
        auto answer = ::fast_io::u8string_view{u8"$$x^2"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // block math with newlines
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$");
        auto answer = ::fast_io::u8string_view{u8"$$\na+b\nc$$"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mixed with regular text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"A $x$ B");
        auto answer = ::fast_io::u8string_view{u8"A&nbsp;$x$&nbsp;B"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"$$\na+b\nc$$test");
        auto answer = ::fast_io::u8string_view{u8"$$\na+b\nc$$test"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
