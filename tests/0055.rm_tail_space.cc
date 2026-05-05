#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t t \n");
        auto answer = ::fast_io::u8string_view{u8"t&nbsp;t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  ");
        auto answer = ::fast_io::u8string_view{u8"t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t");
        auto answer = ::fast_io::u8string_view{u8"t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\t\t");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  ");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt \n");
        auto answer = ::fast_io::u8string_view{u8"t<br>t<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt ");
        auto answer = ::fast_io::u8string_view{u8"t<br>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<p><code>t \nt \n</code> </p>");
        auto answer = ::fast_io::u8string_view{u8"<p><code>t<br>t<br></code></p>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
