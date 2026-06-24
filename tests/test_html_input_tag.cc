#include "precompile.hh"

int main() {
    // ---- unchecked checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled>");
        auto answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<input type=\"checkbox\" disabled>");
        auto plunity_answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled>");
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    // ---- checked checkbox ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled checked>");
        auto answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled checked>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<input type=\"checkbox\" disabled checked>");
        auto plunity_answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled checked>");
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    // ---- uppercase INPUT (tag name case-insensitive, attributes case-sensitive) ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<INPUT TYPE=\"checkbox\" DISABLED CHECKED>");
        auto answer = ::fast_io::u8string_view(
            u8"&lt;INPUT&nbsp;TYPE=&quot;checkbox&quot;&nbsp;DISABLED&nbsp;CHECKED&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- self-closing syntax <input ... /> ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled />");
        auto answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- attribute order: checked before disabled ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input disabled checked type=\"checkbox\">");
        auto answer = ::fast_io::u8string_view(u8"<input type=\"checkbox\" disabled checked>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- text directly adjacent (no spaces) ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a<input type=\"checkbox\" disabled>b");
        auto answer = ::fast_io::u8string_view(u8"a<input type=\"checkbox\" disabled>b");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <input> without type="checkbox" should be escaped ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"text\" disabled>");
        auto answer = ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;text&quot;&nbsp;disabled&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <input> without disabled should be escaped ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\">");
        auto answer = ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;checkbox&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <input> with extra unknown attribute should be escaped ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled unknown=\"x\">");
        auto answer = ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;unknown=&quot;x&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <input> with event handler should be escaped ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled onclick=\"alert(1)\">");
        auto answer = ::fast_io::u8string_view(
            u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;onclick=&quot;alert(1)&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    return 0;
}
