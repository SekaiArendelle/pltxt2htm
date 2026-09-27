#pragma once

#include "doctest_config.hh"

TEST_SUITE("common_parser") {
    TEST_CASE("(empty input)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"");
        CHECK(html == u8"");
    }
    TEST_CASE("<a>test") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
        CHECK(html == u8"<span style=\"color:#0000AA;\">test</span>");
    }
    TEST_CASE("<h3>test") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
        CHECK(html == u8"&lt;h3&gt;test");
    }
    TEST_CASE("<b>test") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
        CHECK(html == u8"<strong>test</strong>");
    }
    TEST_CASE("<i>test") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
        CHECK(html == u8"<em>test</em>");
    }
    TEST_CASE("<color=red><Color=#66CcFf>text</color></color>") {
        auto html = ::pltxt2htm_test::pltxt2common_html(u8"<color=red><Color=#66CcFf>text</color></color>");
        CHECK(html == u8"<span style=\"color:#66CcFf;\">text</span>");
    }
    TEST_CASE("t<b>ex</b>t") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
        CHECK(html == u8"t<strong>ex</strong>t");
    }
    TEST_CASE("***test***") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"***test***");
        CHECK(html == u8"<em><strong>test</strong></em>");
    }
    TEST_CASE("___test___") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"___test___");
        CHECK(html == u8"<em><strong>test</strong></em>");
    }
    TEST_CASE("{Project}{Visitor}{Author}{CoAuthors}") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"{Project}{Visitor}{Author}{CoAuthors}");
        CHECK(html == u8"{Project}{Visitor}{Author}{CoAuthors}");
    }
    TEST_CASE("[text](https://example.com)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com)");
        CHECK(html == u8"<a href=\"https://example.com\">text</a>");
    }
    TEST_CASE("https://example.com") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com");
        CHECK(html == u8"<a href=\"https://example.com\">https://example.com</a>");
    }
    TEST_CASE("https://example.com/?a=1&amp;b=2") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com/?a=1&amp;b=2");
        CHECK(html == u8"<a href=\"https://example.com/?a=1&amp;b=2\">https://example.com/?a=1&amp;b=2</a>");
    }
    TEST_CASE("[text](https://example.com/?q=&quot;) ") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com/?q=&quot;) ");
        CHECK(html == u8"<a href=\"https://example.com/?q=%22\">text</a>&nbsp;");
    }
    TEST_CASE("[**bold**](https://example.com)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[**bold**](https://example.com)");
        CHECK(html == u8"<a href=\"https://example.com\"><strong>bold</strong></a>");
    }
    TEST_CASE("<external=https://example.com>text</external>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com>text</external>");
        CHECK(html == u8"<a href=\"https://example.com\">text</a>");
    }
    TEST_CASE("<external=https://example.com><b>bold</b></external>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com><b>bold</b></external>");
        CHECK(html == u8"<a href=\"https://example.com\"><strong>bold</strong></a>");
    }
    TEST_CASE("![alt](https://example.com/image.png)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"![alt](https://example.com/image.png)");
        CHECK(html == u8"");
    }
    TEST_CASE("[](3.tw&)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[](3.tw&)");
        CHECK(html == u8"[](3.tw&amp;)");
    }
    TEST_CASE("&amp;&'\"<>\\t<br>\\n") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"&amp;&'\"<>\t<br>\n");
        CHECK(html == u8"&amp;&amp;&apos;&quot;&lt;&gt;&nbsp;&nbsp;&nbsp;&nbsp;");
    }
    TEST_CASE("\\!\\#\\$\\%\\(\\)\\*\\+\\,\\-\\.\\/\\:\\;\\=\\?\\@\\[\\]\\^\\_\\`\\{\\|\\}\\~\\\\\\'...") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8R"(\!\#\$\%\(\)\*\+\,\-\.\/\:\;\=\?\@\[\]\^\_\`\{\|\}\~\\\'\"\<\>\&)"
            u8"escaped");
        CHECK(html == u8"!#$%()*+,-./:;=?@[]^_`{|}~\\&apos;&quot;&lt;&gt;&amp;escaped");
    }
    TEST_CASE("<span style=\"color:red\">text</span>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:red\">text</span>");
        CHECK(html == u8"<span style=\"color:red;\">text</span>");
    }
    TEST_CASE("<span style=\"font-size:12px\">text</span>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"font-size:12px\">text</span>");
        CHECK(html == u8"<span style=\"font-size:12px;\">text</span>");
    }
    TEST_CASE("<span style=\"color:blue;font-size:16px\">text</span>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        CHECK(html == u8"<span style=\"color:blue;font-size:16px;\">text</span>");
    }
    TEST_CASE("<a href=\"https://example.com\" internal>text</a>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://example.com\" internal>text</a>");
        CHECK(html == u8"<a href=\"https://example.com\" internal>text</a>");
    }
    TEST_CASE("__text__") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"__text__");
        CHECK(html == u8"<strong>text</strong>");
    }
    TEST_CASE("_text_") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"_text_");
        CHECK(html == u8"<em>text</em>");
    }
    TEST_CASE("*text*") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"*text*");
        CHECK(html == u8"<em>text</em>");
    }
    TEST_CASE("<span style=\"font-size:12px\"><span style=\"font-size:12px\"...") {
        auto html = ::pltxt2htm_test::pltxt2common_html(
            u8"<span style=\"font-size:12px\"><span style=\"font-size:12px\">text</span></span>");
        CHECK(html == u8"<span style=\"font-size:12px;\">text</span>");
    }
    TEST_CASE("<span style=\"font-size:12px\"><color=red>text</color></span>") {
        auto html =
            ::pltxt2htm_test::pltxt2common_html(u8"<span style=\"font-size:12px\"><color=red>text</color></span>");
        CHECK(html == u8"<span style=\"color:red;font-size:12px;\">text</span>");
    }
    TEST_CASE("<strong>text</strong>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<strong>text</strong>");
        CHECK(html == u8"<strong>text</strong>");
    }
    TEST_CASE("<em>text</em>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<em>text</em>");
        CHECK(html == u8"<em>text</em>");
    }
    TEST_CASE("<experiment=id>text</experiment>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<experiment=id>text</experiment>");
        CHECK(html == u8"text");
    }
    TEST_CASE("<discussion=id>text</discussion>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<discussion=id>text</discussion>");
        CHECK(html == u8"text");
    }
    TEST_CASE("<user=id>text</user>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<user=id>text</user>");
        CHECK(html == u8"text");
    }
    TEST_CASE("<size=12>text</size>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<size=12>text</size>");
        CHECK(html == u8"text");
    }
    TEST_CASE("<p>text</p>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<p>text</p>");
        CHECK(html == u8"&lt;p&gt;text&lt;/p&gt;");
    }
    TEST_CASE("<h1>1</h1><h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<h1>1</h1><h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        CHECK(html ==
              u8"&lt;h1&gt;1&lt;/h1&gt;&lt;h2&gt;2&lt;/h2&gt;&lt;h3&gt;3&lt;/h3&gt;"
              u8"&lt;h4&gt;4&lt;/h4&gt;&lt;h5&gt;5&lt;/h5&gt;&lt;h6&gt;6&lt;/h6&gt;");
    }
    TEST_CASE("<del>text</del>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<del>text</del>");
        CHECK(html == u8"text");
    }
    TEST_CASE("<ul><li>u</li></ul><ol><li>o</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<ul><li>u</li></ul><ol><li>o</li></ol>");
        CHECK(html == u8"&lt;ul&gt;&lt;li&gt;u&lt;/li&gt;&lt;/ul&gt;&lt;ol&gt;&lt;li&gt;o&lt;/li&gt;&lt;/ol&gt;");
    }
    TEST_CASE("<code>code</code><pre>pre</pre><blockquote>quote</blockqu...") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<code>code</code><pre>pre</pre><blockquote>quote</blockquote>");
        CHECK(html == u8"code&lt;pre&gt;pre&lt;/pre&gt;&lt;blockquote&gt;quote&lt;/blockquote&gt;");
    }
    TEST_CASE("<table><caption>c</caption><colgroup><col></colgroup><the...") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<table><caption>c</caption><colgroup><col></colgroup><thead><tr><th>h</th></tr></"
            u8"thead><tbody><tr><td>d</"
            u8"td></tr></tbody><tfoot><tr><td>f</td></tr></tfoot></table>");
        CHECK(
            html ==
            u8"&lt;table&gt;&lt;caption&gt;c&lt;/caption&gt;&lt;colgroup&gt;&lt;col&gt;&lt;/"
            u8"colgroup&gt;&lt;thead&gt;&lt;"
            u8"tr&gt;&lt;th&gt;h&lt;/th&gt;&lt;/tr&gt;&lt;/thead&gt;&lt;tbody&gt;&lt;tr&gt;&lt;td&gt;d&lt;/td&gt;&lt;/"
            u8"tr&gt;&"
            u8"lt;/tbody&gt;&lt;tfoot&gt;&lt;tr&gt;&lt;td&gt;f&lt;/td&gt;&lt;/tr&gt;&lt;/tfoot&gt;&lt;/table&gt;");
    }
    TEST_CASE("<hr><input type=\"checkbox\" disabled><img src=\"a.png\" alt=...") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<hr><input type=\"checkbox\" disabled><img src=\"a.png\" alt=\"a\"><!-- note -->");
        CHECK(html == u8"&lt;hr&gt;&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;");
    }
    TEST_CASE("~~text~~") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"~~text~~");
        CHECK(html == u8"text");
    }
    TEST_CASE("- item") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- item");
        CHECK(html == u8"-&nbsp;item");
    }
    TEST_CASE("1. item") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"1. item");
        CHECK(html == u8"1.&nbsp;item");
    }
    TEST_CASE("- [x] item") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- [x] item");
        CHECK(html == u8"-&nbsp;[x]&nbsp;item");
    }
    TEST_CASE("# 1\\n## 2\\n### 3\\n#### 4\\n##### 5\\n###### 6") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"# 1\n## 2\n### 3\n#### 4\n##### 5\n###### 6");
        CHECK(html == u8"#&nbsp;1##&nbsp;2###&nbsp;3####&nbsp;4#####&nbsp;5######&nbsp;6");
    }
    TEST_CASE("> quote") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"> quote");
        CHECK(html == u8"&gt;&nbsp;quote");
    }
    TEST_CASE("| h |\\n|-|\\n| d |") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"| h |\n|-|\n| d |");
        CHECK(html == u8"|&nbsp;h&nbsp;||-||&nbsp;d&nbsp;|");
    }
    TEST_CASE("`one```two`````three```") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"`one```two`````three```");
        CHECK(html == u8"onetwothree");
    }
    TEST_CASE("```\\ncode\\n```\\n~~~\\nmore\\n~~~") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"```\ncode\n```\n~~~\nmore\n~~~");
        CHECK(html == u8"```code```~~~more~~~");
    }
    TEST_CASE("$inline$") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$inline$");
        CHECK(html == u8"inline");
    }
    TEST_CASE("$$block$$") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$$block$$");
        CHECK(html == u8"block");
    }
    TEST_CASE("---") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"---");
        CHECK(html == u8"---");
    }
}
