#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text \ntext ");
        auto answer = ::fast_io::u8string_view{u8"text\ntext"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<b>test</b>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<strong>test</strong>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab**test**cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<i>test</i>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<em>test</em>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab*test*cd");
        auto answer = ::fast_io::u8string_view{u8"ab<i>test</i>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<external=https://example.com>example</external>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<external=https://example.com>example</external>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab[example](https://example.com)cd");
        auto answer = ::fast_io::u8string_view{u8"ab<external=https://example.com>example</external>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- test\n- text");
        // TODO maybe the tail linebreak should be removed
        auto answer = ::fast_io::u8string_view{u8"• test\n• text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"* test\n  - text\n    + test");
        // TODO maybe the tail linebreak should be removed
        auto answer = ::fast_io::u8string_view{u8"• test\n  ∘ text\n    ▫ test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n3. test");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>test</li><li>text</li><li>test</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  1. test\n  2. text");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n  1. test\n  2. text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test\n\n1. test\n1. text\n1. test\n");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n\n1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto plrichtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<li>test</li>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>＜</size>li<size=20>＞</size>test<size=20>＜</size>/li<size=20>＞</size>"};
        pltxt2htm_test_assert_equal(plrichtext, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li><ul><li>xxx</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 \n  \u2218 xxx\n\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>text<ul><li>sub</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 text\n  \u2218 sub\n\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li><ol><li>xxx</li></ol></li></ol>");
        auto answer = ::fast_io::u8string_view{u8"1. \n  1. xxx\n\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ul><li>b<ul><li>c</li></ul></li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 a\n  \u2218 b\n    \u25ab c\n\n\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ol><li>b</li></ol></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 a\n  1. b\n\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<code>test</code>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<code>test</code>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab`test`cd");
        auto answer = ::fast_io::u8string_view{u8"ab test cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab``test``cd");
        auto answer = ::fast_io::u8string_view{u8"ab test cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab```test```cd");
        auto answer = ::fast_io::u8string_view{u8"ab test cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<del>test</del>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<del>test</del>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab~~test~~cd");
        auto answer = ::fast_io::u8string_view{u8"ab~~test~~cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<blockquote>test</blockquote>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<blockquote>test</blockquote>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"> test");
        auto answer = ::fast_io::u8string_view{u8"test"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"```\ntest\n```"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"```py\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"```py\ntest\n```"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"~~~\ntest\n~~~");
        auto answer = ::fast_io::u8string_view{u8"```\ntest\n```"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"~~~py\ntest\n~~~");
        auto answer = ::fast_io::u8string_view{u8"```py\ntest\n```"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
