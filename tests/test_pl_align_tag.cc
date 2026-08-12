#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=right>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:right\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // "justify" is not a valid TMP align value; only "justified" is
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justify>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=justify&gt;hello&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justified>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:justify\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Align=center>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // double-quoted value (TMP allows <align="center">)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center\">hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // quoted value with whitespace around the closing quote
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"right\" >hello</align  >");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:right\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unmatched opening quote renders as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=&quot;center&gt;hello&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center   >hello</align  >");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // mid-line <align> sequences are literal text, so nothing collapses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<align=center></align>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;align=center&gt;&lt;/align&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<align=center>");
        auto answer = ::fast_io::u8string_view{u8"hello&lt;align=center&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // inner inline <align=right> is literal text; only the outer block frame parses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello<align=right>world</align></align>");
        auto answer = ::fast_io::u8string_view{
            u8"<p style=\"text-align:center\">hello&lt;align=right&gt;world</p>&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center><i>test</i></align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\"><em>test</em></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=centerr>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=centerr&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=bogus>text</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=bogus&gt;text&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align>text</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align&gt;text&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity backend emits TMP align tags
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=center>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=center>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=right>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // "justify" is not a valid TMP align value; renders as literal text
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justify>text</align>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>＜</size>align=justify<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justified>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=justified>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // left is emitted verbatim for an explicit align tag
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=left>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=left>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=\"right\">text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=right>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right\">text</align>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>＜</size>align=right\"<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity block: a line break forces block context, preserving <align>
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"before\n<align=right>text</align>");
        auto answer = ::fast_io::u8string_view{u8"before\n<align=right>text</align>\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // newline separates two block-level <align> tags
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>a</align>\n<align=right>b</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:center\">a</p><p style=\"text-align:right\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<align=center>a</align>\n\n<align=right>b</align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a blank line between two <align> tags renders as two <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>a</align>\n\n<align=right>b</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<p style=\"text-align:center\">a</p><br><p style=\"text-align:right\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<align=center>a</align>\n\n\n<align=right>b</align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // text before and after a block-level <align>
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<align=center>b</align>\nc"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<p style=\"text-align:center\">b</p><br>c"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n<align=center>b</align>\n\nc"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // text on the same line after a closing </align> starts a new line
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>a</align>text\nb"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">a</p>text<br>b"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center>a</align>\ntext\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a newline inside an <align> block renders as <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>line1\nline2</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">line1<br>line2</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center>line1\nline2</align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a blank line inside an <align> block renders as two <br>
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>line1\n\nline3</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">line1<br><br>line3</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center>line1\n\nline3</align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a <p> block followed by an <align> block on the next line
    {
        auto pltext = ::fast_io::u8string_view{u8"<p>a</p>\n<align=center>b</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:left\">a</p><p style=\"text-align:center\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"a\n<align=center>b</align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // an <align> block followed by a <p> block on the next line
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>a</align>\n<p>b</p>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<p style=\"text-align:center\">a</p><p style=\"text-align:left\">b</p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center>a</align>\n\nb"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // a leading newline before a block-level <align>
    {
        auto pltext = ::fast_io::u8string_view{u8"\n<align=center>a</align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">a</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // a trailing newline after a closing </align>
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center>a</align>\n"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">a</p><br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // an empty <align> block is kept like an empty <p> (it carries the block
    // boundary), not erased by the optimizer
    {
        auto pltext = ::fast_io::u8string_view{u8"<align=center></align>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\"></p>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<align=center></align>\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // an empty <align> between two text lines preserves the empty paragraph
    {
        auto pltext = ::fast_io::u8string_view{u8"a\n<align=center></align>\nb"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"a<p style=\"text-align:center\"></p><br>b"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
