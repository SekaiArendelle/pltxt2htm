#include "precompile.hh"

int main() {
    // ---- unchecked checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] task");
        auto answer = ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled>task</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- [ ] task");
        auto plunity_answer = ::fast_io::u8string_view(u8"[ ] task\n");
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    // ---- checked checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x] done");
        auto answer = ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- [x] done");
        auto plunity_answer = ::fast_io::u8string_view(u8"[x] done\n");
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    // ---- uppercase X ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [X] done");
        auto answer = ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- nested checkboxes ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] parent\n  - [x] child");
        auto answer = ::fast_io::u8string_view(
            u8"<ul><li><input type=\"checkbox\" disabled>parent</li><ul><li><input type=\"checkbox\" disabled "
            u8"checked>child</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- checkbox in mixed list ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [ ] task\n- normal");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled>task</li><li>normal</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- [ ] without following space is not a checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [] not a checkbox");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>[]&nbsp;not&nbsp;a&nbsp;checkbox</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- [x] without following space is not a checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- [x]not a checkbox");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>[x]not&nbsp;a&nbsp;checkbox</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    return 0;
}
