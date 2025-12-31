#include "precompile.hh"

int main() {
    // simple inline math
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$x^2$");
        auto answer = ::fast_io::u8string_view{u8"$x^2$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // inline math with spaces
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$x + y$");
        auto answer = ::fast_io::u8string_view{u8"$x + y$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // block math on single line $$...$$
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$$x^2$$");
        auto answer = ::fast_io::u8string_view{u8"$$x^2$$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$$");
        auto answer = ::fast_io::u8string_view{u8"$$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$$x^2");
        auto answer = ::fast_io::u8string_view{u8"$$x^2"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // block math with newlines
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$$\na+b\nc$$");
        auto answer = ::fast_io::u8string_view{u8"$$\na+b\nc$$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // mixed with regular text
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"A $x$ B");
        auto answer = ::fast_io::u8string_view{u8"A&nbsp;$x$&nbsp;B"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"$$\na+b\nc$$test");
        auto answer = ::fast_io::u8string_view{u8"$$\na+b\nc$$test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
