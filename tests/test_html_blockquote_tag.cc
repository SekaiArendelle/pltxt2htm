#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<blockquote>text</blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
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
        auto htmlu = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote><color=red>text</color></blockquote>");
        auto htmlu_answer = ::fast_io::u8string_view{u8"<blockquote>&lt;color=red&gt;text&lt;/color&gt;</blockquote>"};
        pltxt2htm_test_assert_equal(htmlu, htmlu_answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<blockquote><color=red>text</color></blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<blockquote><color=red>text</color></blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</blockquote></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto htmlu = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote><color=red>text</blockquote></color>");
        auto htmlu_answer = ::fast_io::u8string_view{u8"<blockquote>&lt;color=red&gt;text</blockquote>&lt;/color&gt;"};
        pltxt2htm_test_assert_equal(htmlu, htmlu_answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<blockquote><color=red>text</blockquote></color>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<blockquote><color=red>text<size=20>\uFF1C</size>/blockquote<size=20>\uFF1E</size></color></blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text&lt;blockquote&gt;text</blockquote>&lt;/blockquote&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<blockquote>text<size=20>\uFF1C</size>blockquote<size=20>\uFF1E</size>text</blockquote>"
            u8"<size=20>\uFF1C</size>/blockquote<size=20>\uFF1E</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
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
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<blockquote></blockquote>t");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"t<size=20>\uFF1C</size>blockquote<size=20>\uFF1E</size><size=20>\uFF1C</size>/blockquote<size=20>\uFF1E</size>t"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<blockquote></blockquote");
        auto answer = ::fast_io::u8string_view{u8"t&lt;blockquote&gt;&lt;/blockquote"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<blockquote></blockquote");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"t<size=20>\uFF1C</size>blockquote<size=20>\uFF1E</size><size=20>\uFF1C</size>/blockquote"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<blockquote>test</blockquote>cd");
        auto answer = ::fast_io::u8string_view{u8"ab&lt;blockquote&gt;test&lt;/blockquote&gt;cd"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<blockquote>test</blockquote>cd");
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"ab<size=20>\uFF1C</size>blockquote<size=20>\uFF1E</size>test<size=20>\uFF1C</size>/blockquote<size=20>\uFF1E</size>cd"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text\n<blockquote>text</blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<br><blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"text<br><blockquote>text</blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"text<br><blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
