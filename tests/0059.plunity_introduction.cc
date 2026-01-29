#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto answer = ::fast_io::u8string_view{u8"text\ntext"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<b>test</b>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<strong>test</strong>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab**test**cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<i>test</i>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<em>test</em>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab*test*cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<external=https://example.com>example</external>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<external=https://example.com>example</external>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    {
        auto text = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab[example](https://example.com)cd");
        auto answer = ::fast_io::u8string_view{u8"ab<external=https://example.com>example</external>cd"};
        ::pltxt2htm_test::assert_true(text == answer);
    }

    return 0;
}
