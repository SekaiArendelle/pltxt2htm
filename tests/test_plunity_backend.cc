#include <fast_io/fast_io_dsal/array.h>
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

    {
        constexpr auto data = ::fast_io::array{char8_t(0xc3)};
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(::fast_io::u8string_view{data.data(), data.size()});
        auto answer = ::fast_io::u8string_view{u8"\uFFFD"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"&amp;\t'\"");
        auto answer = ::fast_io::u8string_view{u8"&amp;\t'\""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<color=red>text</color>");
        auto answer = ::fast_io::u8string_view{u8"<color=red>text</color>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<a>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<color=#0000AA>text</color>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<experiment=id>text</experiment>");
        auto answer = ::fast_io::u8string_view{u8"<experiment=id>text</experiment>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<discussion=id>text</discussion>");
        auto answer = ::fast_io::u8string_view{u8"<discussion=id>text</discussion>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<user=id>text</user>");
        auto answer = ::fast_io::u8string_view{u8"<user=id>text</user>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=12>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"__bold__");
        auto answer = ::fast_io::u8string_view{u8"<b>bold</b>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"_italic_");
        auto answer = ::fast_io::u8string_view{u8"<i>italic</i>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"## heading");
        auto answer = ::fast_io::u8string_view{u8"<size=37><b>heading</b></size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"### heading");
        auto answer = ::fast_io::u8string_view{u8"<size=36><b>heading</b></size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"#### heading");
        auto answer = ::fast_io::u8string_view{u8"<size=35><b>heading</b></size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"##### heading");
        auto answer = ::fast_io::u8string_view{u8"<b>heading</b>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"###### heading");
        auto answer = ::fast_io::u8string_view{u8"<b>heading</b>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"| left | center | right |\n"
            u8"|:-----|:------:|------:|\n"
            u8"| a | b | c |");
        auto answer = ::fast_io::u8string_view{
            u8"<table><thead><tr><th>left</th><th style=\"text-align:center\">center</th><th "
            u8"style=\"text-align:right\">right</th></tr></thead><tbody><tr><td>a</td><td "
            u8"style=\"text-align:center\">b</td><td style=\"text-align:right\">c</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<code class=\"language-cpp\">code</code>");
        auto answer = ::fast_io::u8string_view{u8"<code class=\"language-cpp\">code</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. [x] item");
        auto answer = ::fast_io::u8string_view{u8"1. item\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"{Project}{Visitor}{Author}{CoAuthors}", u8"project",
                                                                 u8"visitor", u8"author", u8"coauthors");
        auto answer = ::fast_io::u8string_view{u8"projectvisitorauthorcoauthors"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"![&amp;](https://example.com/image.png)");
        auto answer = ::fast_io::u8string_view{u8"![&amp;](https://example.com/image.png)"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<!-- note -->");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<table><tr><th style=\"text-align:right\">head</th><td "
            u8"style=\"text-align:center\">data</td></tr></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><tr><th style=\"text-align:right\">head</th><td "
            u8"style=\"text-align:center\">data</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- parent\n  - [x] child");
        auto answer = ::fast_io::u8string_view{u8"• parent\n  [x] child\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
