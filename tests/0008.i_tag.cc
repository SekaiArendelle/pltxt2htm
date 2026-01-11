#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>text</i>");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<I    >text</I  >");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i><color=red>text</color></i>");
        auto answer = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text</span></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i><color=red>text</i></color>");
        auto answer = ::fast_io::u8string_view{u8"<em><span style=\"color:red;\">text&lt;/i&gt;</span></em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>text<i>text</i></i>");
        auto answer = ::fast_io::u8string_view{u8"<em>texttext</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i>");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<i");
        auto answer = ::fast_io::u8string_view{u8"&lt;i"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<i></i>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<em>te<i>st</i></em>");
        auto answer = ::fast_io::u8string_view{u8"<em>test</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}