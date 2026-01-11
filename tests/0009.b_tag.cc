#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>text</b>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<B    >text</b  >");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b><color=red>text</color></B>");
        auto answer = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b><color=red>text</b></color>");
        auto answer = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text&lt;/b&gt;</span></strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>text<b>text</b></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<b>");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // Optimization example: empty tag
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<b></b>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong>text<b>text</b></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}