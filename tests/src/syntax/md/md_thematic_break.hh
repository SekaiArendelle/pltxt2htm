#pragma once

#include "doctest_config.hh"

TEST_CASE("md_thematic_break") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Br />--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        CHECK(html == answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"---"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"---\n"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<hr>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<hr>t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"_ _  _");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" ** * ***");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"# test<br> ---");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---\n# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<hr><h1>test</h1><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---\n---\n ---");
        auto answer = ::fast_io::u8string_view{u8"<hr><hr><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> ---\n # test");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n---\n---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><hr>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /> # test");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /> # test\ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>test"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /># \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n --- <Br /># \n# \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> --- <Br /># \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br> --- <Br /># \n# \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<br># t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n# t\n# t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1><h1>t</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n#  #\n# #\n # #");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>#</h1><h1>#</h1><h1>#</h1>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n-\t-\t-\ntext");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>text"};
        CHECK(html == answer);
    }

    // negative cases: NOT thematic breaks

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - ");
        auto answer = ::fast_io::u8string_view{u8"<ul><li></li></ul>"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"--");
        auto answer = ::fast_io::u8string_view{u8"--"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"-a-");
        auto answer = ::fast_io::u8string_view{u8"-a-"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*");
        auto answer = ::fast_io::u8string_view{u8"*"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" _ ");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;_"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - -");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>-</li></ul>"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"---a");
        auto answer = ::fast_io::u8string_view{u8"---a"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a---");
        auto answer = ::fast_io::u8string_view{u8"a---"};
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"***a");
        auto answer = ::fast_io::u8string_view{u8"***a"};
        CHECK(html == answer);
    }
}
