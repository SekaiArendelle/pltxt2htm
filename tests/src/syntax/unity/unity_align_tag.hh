#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_align_tag") {
    TEST_CASE("center") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("left") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello</align>");
        auto const& answer = u8"<p style=\"text-align:left\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("right") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=right>hello</align>");
        auto const& answer = u8"<p style=\"text-align:right\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("justify-rejected") {
        // "justify" is not a valid TMP align value; only "justified" is
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justify>hello</align>");
        auto const& answer = u8"&lt;align=justify&gt;hello&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("justified") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justified>hello</align>");
        auto const& answer = u8"<p style=\"text-align:justify\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("case-insensitive-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Align=center>hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("quoted-value") {
        // double-quoted value (TMP allows <align="center">)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center\">hello</align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("quoted-value-with-spaces") {
        // quoted value with whitespace around the closing quote
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"right\" >hello</align  >");
        auto const& answer = u8"<p style=\"text-align:right\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("unmatched-quote-literal") {
        // unmatched opening quote renders as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=\"center>hello</align>");
        auto const& answer = u8"&lt;align=&quot;center&gt;hello&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("extra-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center   >hello</align  >");
        auto const& answer = u8"<p style=\"text-align:center\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("mid-line-literal") {
        // mid-line <align> sequences are literal text, so nothing collapses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<align=center></align>t");
        auto const& answer = u8"t&lt;align=center&gt;&lt;/align&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("mid-line-unclosed-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<align=center>");
        auto const& answer = u8"hello&lt;align=center&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-block-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello");
        auto const& answer = u8"<p style=\"text-align:left\">hello</p>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-inner-literal") {
        // inner inline <align=right> is literal text; only the outer block frame parses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello<align=right>world</align></align>");
        auto const& answer = u8"<p style=\"text-align:center\">hello&lt;align=right&gt;world</p>&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center><i>test</i></align>");
        auto const& answer = u8"<p style=\"text-align:center\"><em>test</em></p>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=>text");
        auto const& answer = u8"&lt;align=&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("typo-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=centerr>text");
        auto const& answer = u8"&lt;align=centerr&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=bogus>text</align>");
        auto const& answer = u8"&lt;align=bogus&gt;text&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("missing-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align>text</align>");
        auto const& answer = u8"&lt;align&gt;text&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-center") {
        // plunity backend emits TMP align tags
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=center>text</align>");
        auto const& answer = u8"<align=center>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-right") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right>text</align>");
        auto const& answer = u8"<align=right>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-justify-literal") {
        // "justify" is not a valid TMP align value; renders as literal text
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justify>text</align>");
        auto const& answer =
            u8"<size=20>＜</size>align=justify<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-justified") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justified>text</align>");
        auto const& answer = u8"<align=justified>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-left-verbatim") {
        // left is emitted verbatim for an explicit align tag
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=left>text</align>");
        auto const& answer = u8"<align=left>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-quoted-value") {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=\"right\">text</align>");
        auto const& answer = u8"<align=right>text</align>\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-unmatched-quote-literal") {
        // plunity backend emits unquoted TMP align tags for a quoted input
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right\">text</align>");
        auto const& answer =
            u8"<size=20>＜</size>align=right\"<size=20>＞</size>text<size=20>＜</size>/align<size=20>＞</size>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-line-break-block") {
        // plunity block: a line break forces block context, preserving <align>
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"before\n<align=right>text</align>");
        auto const& answer = u8"before\n<align=right>text</align>\n";
        CHECK(html == answer);
    }

    // newline separates two block-level <align> tags
    TEST_CASE("newline-separates-blocks") {
        auto const& pltext = u8"<align=center>a</align>\n<align=right>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><p style=\"text-align:right\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\n<align=right>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line between two <align> tags renders as two <br>
    TEST_CASE("blank-line-two-br") {
        auto const& pltext = u8"<align=center>a</align>\n\n<align=right>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><br><p style=\"text-align:right\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\n\n<align=right>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text before and after a block-level <align>
    TEST_CASE("text-around-block") {
        auto const& pltext = u8"a\n<align=center>b</align>\nc";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><p style=\"text-align:center\">b</p><br>c";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<align=center>b</align>\n\nc";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text on the same line after a closing </align> starts a new line
    TEST_CASE("text-after-close-new-line") {
        auto const& pltext = u8"<align=center>a</align>text\nb";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p>text<br>b";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\ntext\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a newline inside an <align> block renders as <br>
    TEST_CASE("newline-inside-block") {
        auto const& pltext = u8"<align=center>line1\nline2</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">line1<br>line2</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>line1\nline2</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line inside an <align> block renders as two <br>
    TEST_CASE("blank-line-inside-block") {
        auto const& pltext = u8"<align=center>line1\n\nline3</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">line1<br><br>line3</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>line1\n\nline3</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a <p> block followed by an <align> block on the next line
    TEST_CASE("p-then-align") {
        auto const& pltext = u8"<p>a</p>\n<align=center>b</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br><p style=\"text-align:center\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n<align=center>b</align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an <align> block followed by a <p> block on the next line
    TEST_CASE("align-then-p") {
        auto const& pltext = u8"<align=center>a</align>\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>a</align>\n\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a leading newline before a block-level <align>
    TEST_CASE("leading-newline") {
        auto const& pltext = u8"\n<align=center>a</align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<br><p style=\"text-align:center\">a</p>";
        CHECK(html == answer);
    }

    // a trailing newline after a closing </align>
    TEST_CASE("trailing-newline") {
        auto const& pltext = u8"<align=center>a</align>\n";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">a</p><br>";
        CHECK(html == answer);
    }

    // an empty <align> block is kept like an empty <p> (it carries the block
    // boundary), not erased by the optimizer
    TEST_CASE("empty-block-kept") {
        auto const& pltext = u8"<align=center></align>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\"></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center></align>\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // an empty <align> between two text lines preserves the empty paragraph
    TEST_CASE("empty-block-between-lines") {
        auto const& pltext = u8"a\n<align=center></align>\nb";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"a<br><p style=\"text-align:center\"></p><br>b";
        CHECK(html == answer);
    }
}
