#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h5>text</h5>");
        auto answer = ::fast_io::u8string_view{u8"<h5>text</h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<H5    >text</H5  >");
        auto answer = ::fast_io::u8string_view{u8"<h5>text</h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h5><color=red>text</color></h5>");
        auto answer = ::fast_io::u8string_view{u8"<h5><span style=\"color:red;\">text</span></h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h5><color=red>text</h5></color>");
        auto answer = ::fast_io::u8string_view{u8"<h5><span style=\"color:red;\">text&lt;/h5&gt;</span></h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h5>text<h5>text</h5></h5>");
        auto answer = ::fast_io::u8string_view{u8"<h5>text<h5>text</h5></h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<h5>");
        auto answer = ::fast_io::u8string_view{u8"<h5></h5>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
