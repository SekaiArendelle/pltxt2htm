#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text</u>");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<U    >text</U  >");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</color></u>");
        auto answer = ::fast_io::u8string_view{u8"<u><span style=\"color:red;\">text</span></u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</u></color>");
        auto answer = ::fast_io::u8string_view{u8"<u><span style=\"color:red;\">text&lt;/u&gt;</span></u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text<u>text</u></u>");
        auto answer = ::fast_io::u8string_view{u8"<u>texttext</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><u>text</u></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><u>text</u></strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u");
        auto answer = ::fast_io::u8string_view{u8"&lt;u"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<u></u>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unclosed <u> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <ul> and <user=...> must not be parsed as <u>
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>t</ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul>t</ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<user=123>name</user>");
        auto answer = ::fast_io::u8string_view{u8"<span class='RUser' data-user='123'>name</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
