#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell1</td><td>cell2</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>h1</th><th>h2</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tr><td>a</td><td>b</td></tr><tr><td>c</td><td>d</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TABLE><TR><TD>CELL</TD></TR></TABLE>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>CELL</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><caption>caption</caption><tr><th>header</th></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><caption>caption</caption><tr><th>header</th></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><colgroup><col></colgroup><tr><td>text</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td><color=red>red</color></td></tr></table>");
        auto answer =
            ::fast_io::u8string_view{u8"<table><tr><td><span style=\"color:red;\">red</span></td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tbody><tr><td>body</td></tr></tbody></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tbody><tr><td>body</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table>");
        auto answer = ::fast_io::u8string_view{u8"<table></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<table></table>t");
        auto answer = ::fast_io::u8string_view{u8"t<table></table>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<col>");
        auto answer = ::fast_io::u8string_view{u8"<col>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<table><tr><td>cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
