#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h2>text</h2>");
        auto answer = ::fast_io::u8string_view{u8"<h2>text</h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H2    >text</h2  >");
        auto answer = ::fast_io::u8string_view{u8"<h2>text</h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h2><color=red>text</color></h2>");
        auto answer = ::fast_io::u8string_view{u8"<h2><span style=\"color:red;\">text</span></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h2><color=red>text</h2></color>");
        auto answer = ::fast_io::u8string_view{u8"<h2><span style=\"color:red;\">text&lt;/h2&gt;</span></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h2>text<h2>text</h2></h2>");
        auto answer = ::fast_io::u8string_view{u8"<h2>text<h2>text</h2></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h2>");
        auto answer = ::fast_io::u8string_view{u8"<h2></h2>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
