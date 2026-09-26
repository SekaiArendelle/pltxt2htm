#pragma once

#include "doctest_config.hh"

TEST_CASE("html_blockquote_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<BLOCKQUOTE    >text</BlockQuote  >");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</color></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><span style=\"color:red;\">text</span></blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><color=red>text</blockquote></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<blockquote><span style=\"color:red;\">text&lt;/blockquote&gt;</span></blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>text<blockquote>text</blockquote></blockquote>");
        auto answer =
            ::fast_io::u8string_view{u8"<blockquote>text&lt;blockquote&gt;text</blockquote>&lt;/blockquote&gt;"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote></blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote");
        auto answer = ::fast_io::u8string_view{u8"&lt;blockquote"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<blockquote></blockquote>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;blockquote&gt;&lt;/blockquote&gt;t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<blockquote>test</blockquote>cd");
        auto answer = ::fast_io::u8string_view{u8"ab&lt;blockquote&gt;test&lt;/blockquote&gt;cd"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n<blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<br><blockquote>text</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"text<br><blockquote>text</blockquote>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<blockquote><blockquote>text</blockquote></blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><blockquote>text</blockquote></blockquote>"};
        CHECK(html == answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<blockquote><blockquote>text</blockquote>text</blockquote>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<blockquote><blockquote>text</blockquote>text</blockquote>"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<margin left=2em><margin left=2em>text</margin>\ntext</margin>\n"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<br>\n<blockquote>text</blockquote>");
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"\n\n<margin left=2em>text</margin>\n"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
}
