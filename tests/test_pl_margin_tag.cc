#include "precompile.hh"

int main() {
    // web backend renders <margin...> as a block-level div with CSS margins
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-right=1em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-right:1em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a newline inside a margin block still renders as <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>line1\nline2</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">line1<br>line2</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text after the closing </margin> stays on its own content line
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>a</margin>\nb");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">a</div><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mid-line <margin> sequences are literal text (block-level only)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<margin-left=2em>x</margin>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;margin-left=2em&gt;x&lt;/margin&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a non-numeric value renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=abc>x</margin>");
        auto answer = ::fast_io::u8string_view{u8"&lt;margin-left=abc&gt;x&lt;/margin&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an empty margin block is kept, not erased by the optimizer
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em></margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\"></div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unclosed margin block still parses
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin-left=2em>text");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // plunity backend emits TMP margin tags
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-right=1em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin right=1em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em right=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // px is the default unit and is emitted without a suffix
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=10px>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=10 right=10>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // percent unit is preserved
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=5%>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=5% right=5%>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // web backend emits px and percent CSS units explicitly
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin=10px>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:10px;margin-right:10px;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin=5%>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:5%;margin-right:5%;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // zero is a valid margin value
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=0>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=0 right=0>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // tag name matching is case-insensitive (TMP accepts <MARGIN-LEFT>)
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<MARGIN-LEFT=2em>text</MARGIN>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unclosed margin block still emits a closing tag in the plunity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em>text");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // text after the closing </margin> starts on a new line in the plunity backend
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em>a</margin>text");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>a</margin>\ntext"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // two adjacent block-level margin tags are separated by a line break
    {
        auto pltext = ::fast_io::u8string_view{u8"<margin-left=1em>a</margin>\n<margin-right=2em>b</margin>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<div style=\"margin-left:1em;\">a</div><br><div style=\"margin-right:2em;\">b</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<margin left=1em>a</margin>\n<margin right=2em>b</margin>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // text before and after a block-level margin
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<margin-left=2em>b</margin>\nc"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<br><div style=\"margin-left:2em;\">b</div><br>c"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n<margin left=2em>b</margin>\nc"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // an empty margin between two lines keeps its block boundary
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<margin-left=2em></margin>\nb"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<br><div style=\"margin-left:2em;\"></div><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n<margin left=2em></margin>\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // the attribute form <margin left=V right=W> round-trips through the plunity backend
    {
        auto pltext = ::fast_io::u8string_view{u8"<margin left=2em right=2em>text</margin>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:2em;\">text</div>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<margin left=2em right=2em>text</margin>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // left and right margins can differ
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em right=3em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em right=3em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // the attribute form may specify only one side
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin right=1em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin right=1em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // attribute names are case-insensitive (TMP uppercases the tag buffer)
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin LEFT=2em RIGHT=3em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em right=3em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // <margin=V> and <margin left=V right=V> are equivalent
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em right=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto html_attributes =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=2em right=2em>text</margin>");
        pltxt2htm_test_assert_equal(html_attributes, answer);
    }

    // repeated attributes keep the last value
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin left=1em left=2em>text</margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em>text</margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a newline inside an attribute-form margin block still renders as <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=2em right=3em>line1\nline2</margin>");
        auto answer =
            ::fast_io::u8string_view{u8"<div style=\"margin-left:2em;margin-right:3em;\">line1<br>line2</div>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // mid-line attribute-form <margin> sequences are literal text (block-level only)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<margin left=2em>x</margin>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;margin&nbsp;left=2em&gt;x&lt;/margin&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a malformed attribute value renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=abc right=2em>x</margin>");
        auto answer = ::fast_io::u8string_view{u8"&lt;margin&nbsp;left=abc&nbsp;right=2em&gt;x&lt;/margin&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an unknown attribute name renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin foo=2em>x</margin>");
        auto answer = ::fast_io::u8string_view{u8"&lt;margin&nbsp;foo=2em&gt;x&lt;/margin&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a missing value renders as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<margin left=2em right>x</margin>");
        auto answer = ::fast_io::u8string_view{u8"&lt;margin&nbsp;left=2em&nbsp;right&gt;x&lt;/margin&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<margin-left=2em><margin-right=2em>x</margin></margin>");
        auto answer = ::fast_io::u8string_view{u8"<margin left=2em><margin right=2em>x</margin></margin>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
