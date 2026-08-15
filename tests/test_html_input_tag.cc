#include "precompile.hh"

int main() {
    // ---- inline <input> is always literal; checkbox inputs are only recognized
    //      inside a block-level <ul>/<ol> item (see the list tests below) ----
    {
        auto pltext = ::fast_io::u8string_view{u8"<input type=\"checkbox\" disabled>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>input\u00a0type=\"checkbox\"\u00a0disabled<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"<input type=\"checkbox\" disabled checked>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;checked&gt;");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>input\u00a0type=\"checkbox\"\u00a0disabled\u00a0checked<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    // ---- uppercase INPUT (tag name case-insensitive) is still literal inline ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<INPUT TYPE=\"checkbox\" DISABLED CHECKED>");
        auto answer =
            ::fast_io::u8string_view(u8"&lt;INPUT&nbsp;TYPE=&quot;checkbox&quot;&nbsp;DISABLED&nbsp;CHECKED&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- self-closing syntax <input ... /> is literal inline ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled />");
        auto answer = ::fast_io::u8string_view(u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;/&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- attribute order: checked before disabled ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input disabled checked type=\"checkbox\">");
        auto answer =
            ::fast_io::u8string_view(u8"&lt;input&nbsp;disabled&nbsp;checked&nbsp;type=&quot;checkbox&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- text directly adjacent (no spaces) ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a<input type=\"checkbox\" disabled>b");
        auto answer = ::fast_io::u8string_view(u8"a&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;b");
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
        auto answer = ::fast_io::u8string_view(
            u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;unknown=&quot;x&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- <input> with event handler should be escaped ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<input type=\"checkbox\" disabled onclick=\"alert(1)\">");
        auto answer = ::fast_io::u8string_view(
            u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&nbsp;onclick=&quot;alert(1)&quot;&gt;");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- checkbox <input> at the start of a <ul>/<ol> item is valid ----
    {
        auto pltext = ::fast_io::u8string_view{u8"<ul><li><input type=\"checkbox\" disabled>task</li></ul>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled>task</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_answer = ::fast_io::u8string_view{u8"\u2022 \u2610 task\n"};
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>"};
        auto html =
            ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ul><li><input type=\"checkbox\" disabled checked>done</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_answer = ::fast_io::u8string_view{u8"\u2022 \u2611 done\n"};
        pltxt2htm_test_assert_equal(plunity, plunity_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<ul>\n<li><input type=\"checkbox\" disabled>a</li>\n<li><input type=\"checkbox\" disabled "
            u8"checked>b</li>\n</ul>");
        auto answer = ::fast_io::u8string_view(
            u8"<ul><li><input type=\"checkbox\" disabled>a</li><li><input type=\"checkbox\" disabled "
            u8"checked>b</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><input type=\"checkbox\" disabled>item</li></ol>");
        auto answer = ::fast_io::u8string_view(u8"<ol><li><input type=\"checkbox\" disabled>item</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- a non-checkbox <input> inside an item is literal text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><input type=\"text\" disabled>t</li></ul>");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>&lt;input&nbsp;type=&quot;text&quot;&nbsp;disabled&gt;t</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    // ---- a checkbox <input> not at the start of the item is literal text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a<input type=\"checkbox\" disabled></li></ul>");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>a&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
