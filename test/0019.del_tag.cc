#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<del>text</del>");
        auto answer = ::fast_io::u8string_view{u8"<del>text</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<DEL    >text</DEL  >");
        auto answer = ::fast_io::u8string_view{u8"<del>text</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Del><color=red>text</color></Del>");
        auto answer = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text</span></del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<del><color=red>text</del></color>");
        auto answer = ::fast_io::u8string_view{u8"<del><span style=\"color:red;\">text&lt;/del&gt;</span></del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<Del>text<del>text</del></Del>");
        auto answer = ::fast_io::u8string_view{u8"<del>texttext</del>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"text<del>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<del></del>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
