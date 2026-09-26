#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_p_tag") {
    TEST_CASE("<p>text</p>") {
        auto const& pltext = u8"<p>text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<P >text</P >") {
        auto const& pltext = u8"<P    >text</P  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p><color=red>text</color></p>") {
        auto const& pltext = u8"<p><color=red>text</color></p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\"><span style=\"color:red;\">text</span></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=red>text</color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p><color=red>text</p></color>") {
        auto const& pltext = u8"<p><color=red>text</p></color>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\"><span style=\"color:red;\">text&lt;/p&gt;</span></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=red>text<size=20>\uFF1C</size>/p<size=20>\uFF1E</size></color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p>text<p>text</p></p>") {
        auto const& pltext = u8"<p>text<p>text</p></p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">text&lt;p&gt;text</p>&lt;/p&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"text<size=20>\uFF1C</size>p<size=20>\uFF1E</size>text\n<size=20>\uFF1C</size>/p<size=20>\uFF1E</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p>") {
        auto const& pltext = u8"<p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\"></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t<p></p>t") {
        auto const& pltext = u8"t<p></p>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;p&gt;&lt;/p&gt;t";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>p<size=20>\uFF1E</size><size=20>\uFF1C</size>/p<size=20>\uFF1E</size>t";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t<p></p") {
        auto const& pltext = u8"t<p></p";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"t&lt;p&gt;&lt;/p";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"t<size=20>\uFF1C</size>p<size=20>\uFF1E</size><size=20>\uFF1C</size>/p";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text <p>text</p>") {
        auto const& pltext = u8"text\n<p>text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\ntext";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text<br><p>text</p>") {
        auto const& pltext = u8"text<br><p>text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<br><p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\ntext";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text<br><p>text</p> (11)") {
        auto const& pltext = u8"text<br><p>text</p>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto const& answer = u8"text<br><p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
    }

    // newline separates two block-level <p> tags
    TEST_CASE("newline separates two block-level <p> tags") {
        auto const& pltext = u8"<p>a</p>\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line between two <p> tags renders as two <br>
    TEST_CASE("a blank line between two <p> tags renders as two <br>") {
        auto const& pltext = u8"<p>a</p>\n\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br><br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\n\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // multiple blank lines render as multiple <br>
    TEST_CASE("multiple blank lines render as multiple <br>") {
        auto const& pltext = u8"<p>a</p>\n\n\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br><br><br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
    }

    // trailing text after a closing </p> starts a new line
    TEST_CASE("trailing text after a closing </p> starts a new line") {
        auto const& pltext = u8"<p>a</p>text\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p>text<br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\ntext\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a newline inside a <p> block renders as <br>
    TEST_CASE("a newline inside a <p> block renders as <br>") {
        auto const& pltext = u8"<p>line1\nline2</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">line1<br>line2</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"line1\nline2";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // a blank line inside a <p> block renders as two <br>
    TEST_CASE("a blank line inside a <p> block renders as two <br>") {
        auto const& pltext = u8"<p>line1\n\nline3</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">line1<br><br>line3</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"line1\n\nline3";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // text after a newline followed by a <p> block
    TEST_CASE("text after a newline followed by a <p> block") {
        auto const& pltext = u8"<p>a</p>\ntext\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br>text<br><p style=\"text-align:left\">b</p>";
        CHECK(html == answer);
    }

    // a leading newline before a <p> block
    TEST_CASE("a leading newline before a <p> block") {
        auto const& pltext = u8"\n<p>a</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<br><p style=\"text-align:left\">a</p>";
        CHECK(html == answer);
    }

    // a trailing newline after a closing </p>
    TEST_CASE("a trailing newline after a closing </p>") {
        auto const& pltext = u8"<p>a</p>\n";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a</p><br>";
        CHECK(html == answer);
    }

    // an unclosed <p> at a line start still forms a block containing the newline
    TEST_CASE("an unclosed <p> at a line start still forms a block cont...") {
        auto const& pltext = u8"<p>a\n<p>b</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">a<br><p style=\"text-align:left\">b</p></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"a\nb";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p>text</p>text") {
        auto const& pltext = u8"<p>text</p>text";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">text</p>text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text\ntext";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p style=/'text-align:center/'>text</p>") {
        auto const& pltext = u8"<p style=\"text-align:center\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center>text</align>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p style=/'text-align:right/'>text</p>") {
        auto const& pltext = u8"<p style=\"text-align:right\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:right\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=right>text</align>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p style=/'text-align:justify/'>text</p>") {
        auto const& pltext = u8"<p style=\"text-align:justify\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:justify\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=justified>text</align>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("a style with an extra CSS property is not allowed; the t...") {
        // a style with an extra CSS property is not allowed; the tag is rejected
        auto const& pltext = u8"<p style=\"text-align:center;color:red\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;p&nbsp;style=&quot;text-align:center;color:red&quot;&gt;text&lt;/p&gt;";
        CHECK(html == answer);
    }

    // an extra non-style attribute is not allowed; the tag is rejected
    TEST_CASE("an extra non-style attribute is not allowed; the tag is...") {
        auto const& pltext = u8"<p id=\"x\" style=\"text-align:center\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;p&nbsp;id=&quot;x&quot;&nbsp;style=&quot;text-align:center&quot;&gt;text&lt;/p&gt;";
        CHECK(html == answer);
    }

    // a non-text-align style is rejected
    TEST_CASE("a non-text-align style is rejected") {
        auto const& pltext = u8"<p style=\"color:red\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;p&nbsp;style=&quot;color:red&quot;&gt;text&lt;/p&gt;";
        CHECK(html == answer);
    }

    // left is the default, so no style attribute is emitted
    TEST_CASE("left is the default; so no style attribute is emitted") {
        auto const& pltext = u8"<p style=\"text-align:left\">text</p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<p style=/'text-align:center/'><b>text</b></p>") {
        auto const& pltext = u8"<p style=\"text-align:center\"><b>text</b></p>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<p style=\"text-align:center\"><strong>text</strong></p>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<align=center><b>text</b></align>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
}
