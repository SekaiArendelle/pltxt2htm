#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"- test"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"• test\n"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>text</li></ul></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - text\n     - test");
        auto answer = ::fast_io::u8string_view(
            u8"<ul><li>test</li><li>test</li><ul><li>text</li><ul><li>test</li></ul></ul></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - t**ex**t");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>t<strong>ex</strong>t</li></ul></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"-\ttest");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
