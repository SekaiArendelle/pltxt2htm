#pragma once

#include "doctest_config.hh"

TEST_CASE("html_sub_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text</sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SUB    >text</SUB  >");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        CHECK(html == answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"H<sub>2</sub>O"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"H<sub>2</sub>O"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"H<sub>2</sub>O"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub><color=red>text</color></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub><span style=\"color:red;\">text</span></sub>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub><color=red>text</sub></color>");
        auto answer = ::fast_io::u8string_view{u8"<sub><span style=\"color:red;\">text&lt;/sub&gt;</span></sub>"};
        CHECK(html == answer);
    }

    {
        // nested <sub> must NOT be flattened: the inner text shifts the baseline further
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text<sub>text</sub></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text<sub>text</sub></sub>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text<sub>text</sub>text</sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>text<sub>text</sub>text</sub>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><sub>text</sub></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><sub>text</sub></strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub");
        auto answer = ::fast_io::u8string_view{u8"&lt;sub"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<sub></sub>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
    }

    {
        // unclosed <sub> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<sub>text");
        auto answer = ::fast_io::u8string_view{u8"<sub>text</sub>"};
        CHECK(html == answer);
    }

    {
        // <sub> can nest with other formatting tags
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>t1<sub>t2</sub>t3</u>");
        auto answer = ::fast_io::u8string_view{u8"<u>t1<sub>t2</sub>t3</u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<sub>t1<sub>t2</sub></sub>");
        auto answer = ::fast_io::u8string_view{u8"<sub>t1<sub>t2</sub></sub>"};
        CHECK(html == answer);
    }
}
