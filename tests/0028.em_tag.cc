#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em>text</em>");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Em    >text</eM  >");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em><color=red>text</color></em>");
        auto answer = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text</span></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em><color=red>text</em></color>");
        auto answer = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text&lt;/em&gt;</span></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em>text<em>text</em></em>");
        auto answer = ::fast_io::u8string_view{u8"<em>texttext</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"text<em>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<em></em>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
