#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"1. test"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"1. test\n"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\ntest");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>test");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  1. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><ol><li>text</li></ol></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   - text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><ul><li>text</li></ul></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   1. text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ol><li>text</li></ol></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test\n   1. text\n     1. test");
        auto answer = ::fast_io::u8string_view(
            u8"<ol><li>test</li><li>test</li><ol><li>text</li><ol><li>test</li></ol></ol></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n 1. text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul><ol><li>text</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n - text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol><ul><li>text</li></ul>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. t**ex**t");
        auto answer =
            ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><ol><li>t<strong>ex</strong>t</li></ol></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1 ");
        auto answer = ::fast_io::u8string_view(u8"1");
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1.\ttest");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
