#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<BLOCKQUOTE    >text</BlockQuote  >");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote><color=red>text</color></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><span style=\"color:red;\">text</span></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote><color=red>text</blockquote></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text<blockquote>text</blockquote></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<blockquote");
        auto answer = ::fast_io::u8string_view{u8"&lt;blockquote"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<blockquote></blockquote>t");
        auto answer = ::fast_io::u8string_view{u8"t<blockquote></blockquote>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}