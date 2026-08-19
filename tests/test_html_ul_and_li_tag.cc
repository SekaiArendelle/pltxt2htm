#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><li>item2</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<UL    ><LI>item1</LI><LI>item2</LI></UL  >");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1</li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</color></li><li>item2</li></ul>");
        auto answer =
            ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1</span></li><li>item2</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</li></ul></color>");
        auto answer =
            ::fast_io::u8string_view{u8"<ul><li><span style=\"color:red;\">item1</span></li></ul>&lt;/color&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ul></ul>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;ul&gt;&lt;/ul&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto answer = ::fast_io::u8string_view{u8"t&lt;li"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><del>test</del></ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;&lt;li&gt;item1&lt;/li&gt;<del>test</del>&lt;/ul&gt;"};
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
        auto answer = ::fast_io::u8string_view{u8"\u2022 \n  \u2218 xxx\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>text<ul><li>sub</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"\u2022 text\n  \u2218 sub\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <ul> without <li> must not be parsed as the <u> underline tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>t</ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;t&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- block-level lists: whitespace/newlines inside <ul> are ignored ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>\n<li>a</li>\n<li>b</li>\n</ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>a</li><li>b</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul> <li>a</li>\t<li>b</li> </ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>a</li><li>b</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- a list after a line break is still a block ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a\n<ul><li>b</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"a<br><ul><li>b</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- non-<li> interior content makes the whole list literal ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a</li> text <li>b</li></ul>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;ul&gt;&lt;li&gt;a&lt;/li&gt;&nbsp;text&nbsp;&lt;li&gt;b&lt;/li&gt;&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <li> outside a list context is literal ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<li>a</li>");
        auto answer = ::fast_io::u8string_view{u8"&lt;li&gt;a&lt;/li&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- whitespace/newlines inside items are HTML formatting, not content ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n<ol>\n  <li>text</li></ol></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li><ol><li>text</li></ol></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n  text</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>text</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // a newline in the middle of the item text is real content
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>first\nsecond</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>first<br>second</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>text\n</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>text</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>text<ul><li>text</li></ul><ul><li>text</li></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>text<ul><li>text</li></ul><ul><li>text</li></ul></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- list-item text is parsed with the inline-only parser ----
    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>before\n# **heading**</li><li>after\n- *item*</li></ul>");
        auto answer = ::fast_io::u8string_view{
            u8"<ul><li>before<br>#&nbsp;<strong>heading</strong></li><li>after<br>-&nbsp;<em>item</em></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // An unclosed inline tag must stop at the current list-item frame.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><b>first</li><li>second</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li><strong>first</strong></li><li>second</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- mismatched closing tag makes the list literal ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- empty lists (no <li> items) are malformed and fall back to literal text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul></ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>\n</ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;<br>&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a<ul></ul></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"&lt;ul&gt;&lt;li&gt;a&lt;ul&gt;&lt;/ul&gt;&lt;/li&gt;&lt;/ul&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // an empty <li> item is still a valid list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- regression: empty lists must not panic the backends (fuzzer crash) ----
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul></ul>");
        auto answer =
            ::fast_io::u8string_view{u8"<size=20>＜</size>ul<size=20>＞</size><size=20>＜</size>/ul<size=20>＞</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul>\n</ul>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>＜</size>ul<size=20>＞</size>\n<size=20>＜</size>/ul<size=20>＞</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
