#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Br />--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<br><hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"--- \nt");
        auto answer = ::fast_io::u8string_view{u8"<hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"--- <Br />t");
        auto answer = ::fast_io::u8string_view{u8"<hr>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"_ _  _");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" ** * ***");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# test<br> ---");
        auto answer = ::fast_io::u8string_view{u8"<h1>test</h1><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>test</h1><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n# test\n ---");
        auto answer = ::fast_io::u8string_view{u8"<hr><h1>test</h1><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"---\n---\n ---");
        auto answer = ::fast_io::u8string_view{u8"<hr><hr><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> ---\n # test");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n---\n---");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><hr>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /> # test");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /> # test\ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1>test</h1>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /># \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n --- <Br /># \n# \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> --- <Br /># \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br> --- <Br /># \n# \ntest");
        auto answer = ::fast_io::u8string_view{u8"<br><hr><h1></h1><h1></h1>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br># t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n# t\n# t\n# t");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>t</h1><h1>t</h1><h1>t</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n#  #\n# #\n # #");
        auto answer = ::fast_io::u8string_view{u8"<br><h1>#</h1><h1>#</h1><h1>#</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
