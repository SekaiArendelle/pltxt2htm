#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_align_tag") {
    TEST_CASE("<align=center>hello</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("<align=left>hello</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello</align>");
        auto const& answer = u8"<p style=\"text-align:left\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("<align=right>hello</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=right>hello</align>");
        auto const& answer = u8"<p style=\"text-align:right\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("'justify' is not a valid TMP align value; only 'justifie...") {
        // "justify" is not a valid TMP align value; only "justified" is
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justify>hello</align>");
        auto const& answer = u8"&lt;align=justify&gt;hello&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<align=justified>hello</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justified>hello</align>");
        auto const& answer = u8"<p style=\"text-align:justify\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("<Align=center>hello</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Align=center>hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("double-quoted value (TMP allows <align='center'>)") {
        // double-quoted value (TMP allows <align="center">)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center\">hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("quoted value with whitespace around the closing quote") {
        // quoted value with whitespace around the closing quote
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"right\" >hello</align  >");
        auto const& answer = u8"<p style=\"text-align:right\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("unmatched opening quote renders as literal text") {
        // unmatched opening quote renders as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center>hello</align>");
        auto const& answer = u8"&lt;align=&quot;center&gt;hello&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<align=center >hello</align >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center   >hello</align  >");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("mid-line <align> sequences are literal text; so nothing...") {
        // mid-line <align> sequences are literal text, so nothing collapses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<align=center></align>t");
        auto const& answer = u8"t&lt;align=center&gt;&lt;/align&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("hello<align=center>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<align=center>");
        auto const& answer = u8"hello&lt;align=center&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<align=left>hello") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello");
        auto const& answer = u8"<p style=\"text-align:left\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("inner inline <align=right> is literal text; only the out...") {
        // inner inline <align=right> is literal text; only the outer block frame parses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello<align=right>world</align></align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello&lt;align=right&gt;world</p>&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<align=center><i>test</i></align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center><i>test</i></align>");
        auto const& answer = u8"<p style=\"text-align:center\"><em>test</em></p>";
        CHECK(html == answer);
    }

    TEST_CASE("<align=>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=>text");
        auto const& answer = u8"&lt;align=&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<align=centerr>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=centerr>text");
        auto const& answer = u8"&lt;align=centerr&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<align=bogus>text</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=bogus>text</align>");
        auto const& answer = u8"&lt;align=bogus&gt;text&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<align>text</align>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align>text</align>");
        auto const& answer = u8"&lt;align&gt;text&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("plunity backend emits TMP align tags") {
        // plunity backend emits TMP align tags
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=center>text</align>");
        auto const& answer = u8"<align=center>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("<align=right>text</align>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right>text</align>");
        auto const& answer = u8"<align=right>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("'justify' is not a valid TMP align value; renders as lit...") {
        // "justify" is not a valid TMP align value; renders as literal text
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justify>text</align>");
        auto const& answer =
            u8"<size=20>＜</size>align=justify<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>";
        CHECK(html == answer);
    }

    TEST_CASE("<align=justified>text</align>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justified>text</align>");
        auto const& answer = u8"<align=justified>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("left is emitted verbatim for an explicit align tag") {
        // left is emitted verbatim for an explicit align tag
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=left>text</align>");
        auto const& answer = u8"<align=left>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity backend emits unquoted TMP align tags for a quot...") {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=\"right\">text</align>");
        auto const& answer = u8"<align=right>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity backend emits unquoted TMP align tags for a quot... (26)") {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right\">text</align>");
        auto const& answer =
            u8"<size=20>＜</size>align=right\"<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity block: a line break forces block context; preser...") {
        // plunity block: a line break forces block context, preserving <align>
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"before\n<align=right>text</align>");
        auto const& answer = u8"before\n<align=right>text</align>\n";
        CHECK(html == answer);
    }

    // newline separates two block-level <align> tags
    TEST_CASE("newline separates two block-level <align> tags") {
        auto const& pltext = u8"<align=center>a</align>\n<align=right>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><p style=\"text-align:right\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\n<align=right>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line between two <align> tags renders as two <br>
    TEST_CASE("a blank line between two <align> tags renders as two <br...") {
        auto const& pltext = u8"<align=center>a</align>\n\n<align=right>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><br><p style=\"text-align:right\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\n\n<align=right>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text before and after a block-level <align>
    TEST_CASE("text before and after a block-level <align>") {
        auto const& pltext = u8"a\n<align=center>b</align>\nc";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><p style=\"text-align:center\">b</p><br>c";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<align=center>b</align>\n\nc";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text on the same line after a closing </align> starts a new line
    TEST_CASE("text on the same line after a closing </align> starts a...") {
        auto const& pltext = u8"<align=center>a</align>text\nb";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p>text<br>b";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\ntext\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a newline inside an <align> block renders as <br>
    TEST_CASE("a newline inside an <align> block renders as <br>") {
        auto const& pltext = u8"<align=center>line1\nline2</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">line1<br>line2</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>line1\nline2</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line inside an <align> block renders as two <br>
    TEST_CASE("a blank line inside an <align> block renders as two <br>") {
        auto const& pltext = u8"<align=center>line1\n\nline3</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">line1<br><br>line3</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>line1\n\nline3</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a <p> block followed by an <align> block on the next line
    TEST_CASE("a <p> block followed by an <align> block on the next lin...") {
        auto const& pltext = u8"<p>a</p>\n<align=center>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br><p style=\"text-align:center\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<align=center>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an <align> block followed by a <p> block on the next line
    TEST_CASE("an <align> block followed by a <p> block on the next lin...") {
        auto const& pltext = u8"<align=center>a</align>\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a leading newline before a block-level <align>
    TEST_CASE("a leading newline before a block-level <align>") {
        auto const& pltext = u8"\n<align=center>a</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<br><p style=\"text-align:center\">a</p>";
        CHECK(html == answer);
    }

    // a trailing newline after a closing </align>
    TEST_CASE("a trailing newline after a closing </align>") {
        auto const& pltext = u8"<align=center>a</align>\n";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br>";
        CHECK(html == answer);
    }

    // an empty <align> block is kept like an empty <p> (it carries the block
    // boundary), not erased by the optimizer
    TEST_CASE("boundary); not erased by the optimizer") {
        auto const& pltext = u8"<align=center></align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\"></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center></align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an empty <align> between two text lines preserves the empty paragraph
    TEST_CASE("an empty <align> between two text lines preserves the em...") {
        auto const& pltext = u8"a\n<align=center></align>\nb";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><p style=\"text-align:center\"></p><br>b";
        CHECK(html == answer);
    }
}
