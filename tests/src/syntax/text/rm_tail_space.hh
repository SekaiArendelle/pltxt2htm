#pragma once

#include "doctest_config.hh"

TEST_CASE("rm_tail_space") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"t<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  \t\t\n");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t t \n");
        auto answer = ::fast_io::u8string_view{u8"t&nbsp;t<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t  ");
        auto answer = ::fast_io::u8string_view{u8"t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t\t\t");
        auto answer = ::fast_io::u8string_view{u8"t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\t\t");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  ");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt \n");
        auto answer = ::fast_io::u8string_view{u8"t<br>t<br>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t \nt ");
        auto answer = ::fast_io::u8string_view{u8"t<br>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto answer = ::fast_io::u8string_view{u8"text\ntext"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<p><code>t \nt \n</code> </p>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\"><code>t<br>t<br></code></p>"};
        CHECK(html == answer);
    }
}
