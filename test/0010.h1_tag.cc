#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h1>text</h1>");
        auto answer = ::fast_io::u8string_view{u8"<h1>text</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H1    >text</h1  >");
        auto answer = ::fast_io::u8string_view{u8"<h1>text</h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h1><color=red>text</color></h1>");
        auto answer = ::fast_io::u8string_view{u8"<h1><span style=\"color:red;\">text</span></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h1><color=red>text</h1></color>");
        auto answer = ::fast_io::u8string_view{u8"<h1><span style=\"color:red;\">text&lt;/h1&gt;</span></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h1>text<h1>text</h1></h1>");
        auto answer = ::fast_io::u8string_view{u8"<h1>text<h1>text</h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h1>");
        auto answer = ::fast_io::u8string_view{u8"<h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"# <h1>text");
        auto answer = ::fast_io::u8string_view{u8"<h1><h1>text</h1></h1>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
