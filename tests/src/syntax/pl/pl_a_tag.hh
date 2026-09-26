#pragma once

#include "doctest_config.hh"

TEST_CASE("pl_a_tag") {
    {
        auto const& pltext = u8"<a>text</a>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=#0000AA>text</color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<A      >text
</Discussion   >)");
        auto const& answer =
            u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/Discussion&nbsp;&nbsp;&nbsp;&gt;</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A      >text</a   >");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>text</a><color=red>text</color>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A><a>text</a></A>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A><color=red>text</color></A>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><a>text</a></color>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#0000AA>t<a>ex</a>t</color>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>t<color=#0000AA>ex</color>t</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>t<color=red>ex</color>t</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">t<span style=\"color:red;\">ex</span>t</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<A>t<a>ex</a>t</A>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<a  $");
        auto const& answer = u8"test&lt;a&nbsp;&nbsp;$";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<a>text");
        auto const& answer = u8"test<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<a></a>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<a>es</color>t");
        auto const& answer = u8"t<span style=\"color:#0000AA;\">es</span>t";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a><i>test</i></a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\"><em>test</em></span>";
        CHECK(html == answer);
    }
}
