#pragma once

#include "doctest_config.hh"

TEST_CASE("pl_s_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text</s>");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<S    >text</S  >");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</color></s>");
        auto answer = ::fast_io::u8string_view{u8"<s><span style=\"color:red;\">text</span></s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s><color=red>text</s></color>");
        auto answer = ::fast_io::u8string_view{u8"<s><span style=\"color:red;\">text&lt;/s&gt;</span></s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text<s>text</s></s>");
        auto answer = ::fast_io::u8string_view{u8"<s>texttext</s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><s>text</s></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><s>text</s></strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s");
        auto answer = ::fast_io::u8string_view{u8"&lt;s"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<s></s>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
    }

    {
        // unclosed <s> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>text");
        auto answer = ::fast_io::u8string_view{u8"<s>text</s>"};
        CHECK(html == answer);
    }

    {
        // strikethrough tags from different syntaxes can nest
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<s>t1<del>t2</del>t3</s>");
        auto answer = ::fast_io::u8string_view{u8"<s>t1<del>t2</del>t3</s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~t1<s>t2</s>t3~~");
        auto answer = ::fast_io::u8string_view{u8"<del>t1<s>t2</s>t3</del>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<s>test</s>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<s>test</s>cd"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<s>t1<s>t2</s></s>");
        auto answer = ::fast_io::u8string_view{u8"<s>t1t2</s>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<del>test</del>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<s>test</s>cd"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab~~test~~cd");
        auto answer = ::fast_io::u8string_view{u8"ab<s>test</s>cd"};
        CHECK(html == answer);
    }
}
