#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>text</h4>");
        auto answer = ::fast_io::u8string_view{u8"<h4>text</h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H4    >text</H4  >");
        auto answer = ::fast_io::u8string_view{u8"<h4>text</h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4><color=red>text</color></h4>");
        auto answer = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text</span></h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4><color=red>text</h4></color>");
        auto answer = ::fast_io::u8string_view{u8"<h4><span style=\"color:red;\">text&lt;/h4&gt;</span></h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>text<h4>text</h4></h4>");
        auto answer = ::fast_io::u8string_view{u8"<h4>text<h4>text</h4></h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h4>");
        auto answer = ::fast_io::u8string_view{u8"<h4></h4>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
