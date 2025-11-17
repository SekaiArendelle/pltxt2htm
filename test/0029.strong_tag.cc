#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong>text</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Strong    >text</sTrong  >");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong><color=red>text</color></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong><color=red>text</strong></color>");
        auto answer =
            ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text&lt;/strong&gt;</span></strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<strong>text<strong>text</strong></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"text<strong>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<strong></strong>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
