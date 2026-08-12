#include "precompile.hh"

int main() {
    // web backend renders <div style="margin-left:..."> as a block-level div with CSS margins
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-right:1em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-right:1em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:3em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // px is emitted explicitly with a px suffix in the web backend
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:10px\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:10px;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // percent unit is preserved
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:5%\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:5%;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an empty div is kept, not erased by the optimizer
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\"></div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\"></div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unclosed div still parses
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">text");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a newline inside a div still renders as <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">line1\nline2</div>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">line1<br>line2</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text after the closing </div> stays on its own content line
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em\">a</div>\nb");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">a</div><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text before and after a block-level div
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<div style=\"margin-left:2em\">b</div>\nc"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<div style=\"margin-left:2em;\">b</div><br>c"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mid-line <div> sequences are literal text (block-level only)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<div style=\"margin-left:2em\">x</div>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // non-numeric margin value renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:abc\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:abc&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unknown style declaration (e.g. color) makes the whole tag literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"color:red\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;color:red&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a style mix with a non-margin declaration is rejected entirely
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em;color:red\">x</div>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:2em;color:red&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a div without the style attribute is literal text (not parsed as a margin block)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div>plain</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&gt;plain&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a script tag is not an allowed attribute and renders as literal text (XSS guard)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div onclick=\"alert(1)\">x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;onclick=&quot;alert(1)&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // malformed quote (unterminated style value) renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left:2em>x</div>");
        auto answer = ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left:2em&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a missing ':' separator renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<div style=\"margin-left 2em\">x</div>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;div&nbsp;style=&quot;margin-left&nbsp;2em&quot;&gt;x&lt;/div&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // plunity backend maps the div back to a TMP margin tag
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-right:1em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin right=1em>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<div style=\"margin-left:2em;margin-right:3em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em right=3em>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // px is the default unit and is emitted without a suffix in the unity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:10px\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=10>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // percent unit is preserved in the unity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:5%\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=5%>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unclosed div still emits a closing margin tag in the unity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:2em\">text");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an empty div round-trips in the unity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:2em\"></div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em></margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // nested divs are handled and map to nested margin tags
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<div style=\"margin-left:2em\"><div style=\"margin-right:1em\">x</div></div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em><margin right=1em>x</margin>\n</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // the plunity front except for unity maps to plain text (title/plain frontends drop the tag)
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // the title backend (pltxt2common_html) keeps the div content but drops the tag itself
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<div style=\"margin-left:2em\">text</div>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- plunity newline rendering around a block-level div ----

    // plunity: a newline before a div puts the margin block on its own line
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<div style=\"margin-left:2em\">b</div>\nc"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n<margin left=2em>b</margin>\n\nc"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: two div blocks separated by a single newline
    {
        auto pltext = ::fast_io::u8string_view{
            u8"<div style=\"margin-left:2em\">a</div>\n<div style=\"margin-left:3em\">b</div>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\n<margin left=3em>b</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a blank line between two div blocks keeps an extra blank line
    {
        auto pltext = ::fast_io::u8string_view{
            u8"<div style=\"margin-left:2em\">a</div>\n\n<div style=\"margin-left:3em\">b</div>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\n\n<margin left=3em>b</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: text followed by a div on the next line
    {
        auto pltext = ::fast_io::u8string_view{u8"text\n<div style=\"margin-left:2em\">b</div>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"text\n<margin left=2em>b</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a leading newline before a div is swallowed
    {
        auto pltext = ::fast_io::u8string_view{u8"\n<div style=\"margin-left:2em\">b</div>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>b</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a newline after the closing </div> starts a new text line
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\">a</div>\nb"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a blank line after a div then text keeps the blank line
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\">a</div>\n\nb"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\n\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // plunity: a newline inside a div renders as a line break
    {
        auto pltext = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em\">line1\nline2</div>"};
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em>line1\nline2</margin>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    return 0;
}