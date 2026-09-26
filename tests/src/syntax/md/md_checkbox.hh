#pragma once

#include "doctest_config.hh"

TEST_CASE("md_checkbox") {
    // ---- unchecked checkbox ----
    {
        auto const& pltext = u8"- [ ] task";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<ul><li><input type=\"checkbox\" disabled>task</li></ul>";
        CHECK(html == answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_answer = u8"• ☐ task\n";
        CHECK(plunity == plunity_answer);
    }
    // ---- checked checkbox ----
    {
        auto const& pltext = u8"- [x] done";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>";
        CHECK(html == answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_answer = u8"• ☑ done\n";
        CHECK(plunity == plunity_answer);
    }
    // ---- uppercase X ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [X] done");
        auto const& answer = u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>";
        CHECK(html == answer);
    }
    // ---- nested checkboxes ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] parent\n  - [x] child");
        auto const& answer =
            u8"<ul><li><input type=\"checkbox\" disabled>parent<ul><li><input type=\"checkbox\" disabled "
            u8"checked>child</li></ul></li></ul>";
        CHECK(html == answer);
    }
    // ---- checkbox in mixed list ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] task\n- normal");
        auto const& answer = u8"<ul><li><input type=\"checkbox\" disabled>task</li><li>normal</li></ul>";
        CHECK(html == answer);
    }
    // ---- [ ] without following space is not a checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [] not a checkbox");
        auto const& answer = u8"<ul><li>[]&nbsp;not&nbsp;a&nbsp;checkbox</li></ul>";
        CHECK(html == answer);
    }
    // ---- [x] without following space is not a checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x]not a checkbox");
        auto const& answer = u8"<ul><li>[x]not&nbsp;a&nbsp;checkbox</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. [x] item");
        auto const& answer = u8"1. ☑ item\n";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. [ ] item");
        auto const& answer = u8"1. ☐ item\n";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- parent\n  - [x] child");
        auto const& answer = u8"• parent\n  ∘ ☑ child\n";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x] # **done**");
        auto const& answer =
            u8"<ul><li><input type=\"checkbox\" disabled checked>#&nbsp;<strong>done</strong></li></ul>";
        CHECK(html == answer);
    }
}
