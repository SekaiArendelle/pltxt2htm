#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h3>text</h3>");
        auto answer = ::fast_io::u8string_view{u8"<h3>text</h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H3    >text</h3  >");
        auto answer = ::fast_io::u8string_view{u8"<h3>text</h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h3><color=red>text</color></h3>");
        auto answer = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text</span></h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h3><color=red>text</h3></color>");
        auto answer = ::fast_io::u8string_view{u8"<h3><span style=\"color:red;\">text&lt;/h3&gt;</span></h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h3>text<h3>text</h3></h3>");
        auto answer = ::fast_io::u8string_view{u8"<h3>text<h3>text</h3></h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h3>");
        auto answer = ::fast_io::u8string_view{u8"<h3></h3>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
