#include <fast_io/fast_io_dsal/array.h>
#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p>text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p style=\"text-align:center\">text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p><align=center>text</align></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p style=\"text-align:right\">text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p><align=right>text</align></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p style=\"text-align:justify\">text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p><align=justified>text</align></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p style=\"text-align:left\">text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<p style=\"text-align:center\"><b>text</b></p>");
        auto answer = ::fast_io::u8string_view{u8"<p><align=center><b>text</b></align></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
