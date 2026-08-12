#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<BLOCKQUOTE    >text</BlockQuote  >");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</color></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><span style=\"color:red;\">text</span></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</blockquote></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto answer =
            ::fast_io::u8string_view{u8"<blockquote>text&lt;blockquote&gt;text</blockquote>&lt;/blockquote&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote");
        auto answer = ::fast_io::u8string_view{u8"&lt;blockquote"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<blockquote></blockquote>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;blockquote&gt;&lt;/blockquote&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<blockquote>test</blockquote>cd");
        auto answer = ::fast_io::u8string_view{u8"ab&lt;blockquote&gt;test&lt;/blockquote&gt;cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<br><blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><blockquote>text</blockquote></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><blockquote>text</blockquote></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><blockquote>text</blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><blockquote>text</blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<blockquote><blockquote>text</blockquote>text</blockquote>");
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<margin left=2em><margin left=2em>text</margin>\ntext</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<br>\n<blockquote>text</blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"\n\n<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // ---- plunity newline rendering around a block-level <blockquote> ----

    // plunity: a newline before a blockquote puts the margin block on its own line
    {
        auto pltext = ::fast_io::u8string_view{u8"text\n<blockquote>text</blockquote>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"text\n<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: two blockquotes separated by a single newline
    {
        auto pltext = ::fast_io::u8string_view{u8"<blockquote>a</blockquote>\n<blockquote>b</blockquote>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<margin left=2em>a</margin>\n\n<margin left=2em>b</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a newline after the closing </blockquote> starts a new text line
    {
        auto pltext = ::fast_io::u8string_view{u8"<blockquote>a</blockquote>\nb"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a leading newline before a blockquote is swallowed
    {
        auto pltext = ::fast_io::u8string_view{u8"\n<blockquote>text</blockquote>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a blank line after a blockquote then text keeps the blank line
    {
        auto pltext = ::fast_io::u8string_view{u8"<blockquote>a</blockquote>\n\nb"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    return 0;
}