#pragma once

#include "doctest_config.hh"

TEST_SUITE("common_parser") {
    TEST_CASE("empty-input") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }
    TEST_CASE("a-tag-without-href") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
        auto const& answer = u8"<span style=\"color:#0000AA;\">test</span>";
        CHECK(html == answer);
    }
    TEST_CASE("h3-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
        auto const& answer = u8"&lt;h3&gt;test";
        CHECK(html == answer);
    }
    TEST_CASE("b-tag-becomes-strong") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
        auto const& answer = u8"<strong>test</strong>";
        CHECK(html == answer);
    }
    TEST_CASE("i-tag-becomes-em") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
        auto const& answer = u8"<em>test</em>";
        CHECK(html == answer);
    }
    TEST_CASE("nested-color-tags") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\"><span style=\"color:#66CcFf;\">text</span></span>";
        CHECK(html == answer);
    }
    TEST_CASE("tag-inside-text") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
        auto const& answer = u8"t<strong>ex</strong>t";
        CHECK(html == answer);
    }
    TEST_CASE("bold-italic-asterisks") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"***test***");
        auto const& answer = u8"<em><strong>test</strong></em>";
        CHECK(html == answer);
    }
    TEST_CASE("bold-italic-underscores") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"___test___");
        auto const& answer = u8"<em><strong>test</strong></em>";
        CHECK(html == answer);
    }
    TEST_CASE("braced-placeholders-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"{Project}{Visitor}{Author}{CoAuthors}");
        auto const& answer = u8"{Project}{Visitor}{Author}{CoAuthors}";
        CHECK(html == answer);
    }
    TEST_CASE("markdown-link") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com)");
        auto const& answer = u8"<a href=\"https://example.com\">text</a>";
        CHECK(html == answer);
    }
    TEST_CASE("bare-url") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com");
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>";
        CHECK(html == answer);
    }
    TEST_CASE("url-with-entity") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com/?a=1&amp;b=2");
        auto const& answer = u8"<a href=\"https://example.com/?a=1&amp;b=2\">https://example.com/?a=1&amp;b=2</a>";
        CHECK(html == answer);
    }
    TEST_CASE("link-with-escaped-quote") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com/?q=&quot;) ");
        auto const& answer = u8"<a href=\"https://example.com/?q=%22\">text</a>&nbsp;";
        CHECK(html == answer);
    }
    TEST_CASE("link-with-bold-text") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[**bold**](https://example.com)");
        auto const& answer = u8"<a href=\"https://example.com\"><strong>bold</strong></a>";
        CHECK(html == answer);
    }
    TEST_CASE("external-tag") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com>text</external>");
        auto const& answer = u8"<a href=\"https://example.com\">text</a>";
        CHECK(html == answer);
    }
    TEST_CASE("external-with-bold") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com><b>bold</b></external>");
        auto const& answer = u8"<a href=\"https://example.com\"><strong>bold</strong></a>";
        CHECK(html == answer);
    }
    TEST_CASE("image-syntax-dropped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"![alt](https://example.com/image.png)");
        auto const& answer = u8"";
        CHECK(html == answer);
    }
    TEST_CASE("empty-link-brackets") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[](3.tw&)");
        auto const& answer = u8"[](3.tw&amp;)";
        CHECK(html == answer);
    }
    TEST_CASE("entities-and-whitespace") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"&amp;&'\"<>\t<br>\n");
        auto const& answer = u8"&amp;&amp;&apos;&quot;&lt;&gt;&nbsp;&nbsp;&nbsp;&nbsp;";
        CHECK(html == answer);
    }
    TEST_CASE("backslash-escapes") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8R"(\!\#\$\%\(\)\*\+\,\-\.\/\:\;\=\?\@\[\]\^\_\`\{\|\}\~\\\'\"\<\>\&)"
            u8"escaped");
        auto const& answer = u8"!#$%()*+,-./:;=?@[]^_`{|}~\\&apos;&quot;&lt;&gt;&amp;escaped";
        CHECK(html == answer);
    }
    TEST_CASE("span-color-gets-semicolon") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:red\">text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }
    TEST_CASE("span-font-size-gets-semicolon") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"font-size:12px\">text</span>");
        auto const& answer = u8"<span style=\"font-size:12px;\">text</span>";
        CHECK(html == answer);
    }
    TEST_CASE("span-multiple-declarations") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        auto const& answer = u8"<span style=\"color:blue;font-size:16px;\">text</span>";
        CHECK(html == answer);
    }
    TEST_CASE("internal-attribute-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://example.com\" internal>text</a>");
        auto const& answer = u8"<a href=\"https://example.com\" internal>text</a>";
        CHECK(html == answer);
    }
    TEST_CASE("bold-double-underscore") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"__text__");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }
    TEST_CASE("italic-single-underscore") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"_text_");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }
    TEST_CASE("italic-single-asterisk") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"*text*");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }
    // This suite runs the production default (`optimize = false`), so nested markup is preserved as written; the
    // optimizer's merging of identical spans is exercised by the fixedadv suites, which optimize.
    TEST_CASE("identical-spans-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<span style=\"font-size:12px\"><span style=\"font-size:12px\">text</span></span>");
        auto const& answer = u8"<span style=\"font-size:12px;\"><span style=\"font-size:12px;\">text</span></span>";
        CHECK(html == answer);
    }
    TEST_CASE("span-and-color-kept") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"font-size:12px\"><color=red>text</color></span>");
        auto const& answer = u8"<span style=\"font-size:12px;\"><span style=\"color:red;\">text</span></span>";
        CHECK(html == answer);
    }
    TEST_CASE("strong-tag-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<strong>text</strong>");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }
    TEST_CASE("em-tag-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<em>text</em>");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }
    TEST_CASE("experiment-tag-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<experiment=id>text</experiment>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("discussion-tag-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<discussion=id>text</discussion>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("user-tag-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<user=id>text</user>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("size-tag-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<size=12>text</size>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("p-tag-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<p>text</p>");
        auto const& answer = u8"&lt;p&gt;text&lt;/p&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("heading-tags-escaped") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<h1>1</h1><h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        auto const& answer =
            u8"&lt;h1&gt;1&lt;/h1&gt;&lt;h2&gt;2&lt;/h2&gt;&lt;h3&gt;3&lt;/h3&gt;"
            u8"&lt;h4&gt;4&lt;/h4&gt;&lt;h5&gt;5&lt;/h5&gt;&lt;h6&gt;6&lt;/h6&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("del-tag-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<del>text</del>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("list-tags-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<ul><li>u</li></ul><ol><li>o</li></ol>");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;u&lt;/li&gt;&lt;/ul&gt;&lt;ol&gt;&lt;li&gt;o&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("code-pre-blockquote-escaped") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<code>code</code><pre>pre</pre><blockquote>quote</blockquote>");
        auto const& answer = u8"code&lt;pre&gt;pre&lt;/pre&gt;&lt;blockquote&gt;quote&lt;/blockquote&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("table-tags-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<table><caption>c</caption><colgroup><col></colgroup><thead><tr><th>h</th></tr></"
            u8"thead><tbody><tr><td>d</"
            u8"td></tr></tbody><tfoot><tr><td>f</td></tr></tfoot></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;caption&gt;c&lt;/caption&gt;&lt;colgroup&gt;&lt;col&gt;&lt;/"
            u8"colgroup&gt;&lt;thead&gt;&lt;"
            u8"tr&gt;&lt;th&gt;h&lt;/th&gt;&lt;/tr&gt;&lt;/thead&gt;&lt;tbody&gt;&lt;tr&gt;&lt;td&gt;d&lt;/td&gt;&lt;/"
            u8"tr&gt;&"
            u8"lt;/tbody&gt;&lt;tfoot&gt;&lt;tr&gt;&lt;td&gt;f&lt;/td&gt;&lt;/tr&gt;&lt;/tfoot&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("void-tags-and-comment") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<hr><input type=\"checkbox\" disabled><img src=\"a.png\" alt=\"a\"><!-- note -->");
        auto const& answer = u8"&lt;hr&gt;&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("strikethrough-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"~~text~~");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }
    TEST_CASE("dash-list-marker") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- item");
        auto const& answer = u8"-&nbsp;item";
        CHECK(html == answer);
    }
    TEST_CASE("ordered-list-marker") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"1. item");
        auto const& answer = u8"1.&nbsp;item";
        CHECK(html == answer);
    }
    TEST_CASE("checkbox-list-marker") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- [x] item");
        auto const& answer = u8"-&nbsp;[x]&nbsp;item";
        CHECK(html == answer);
    }
    TEST_CASE("heading-markers-without-space") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"# 1\n## 2\n### 3\n#### 4\n##### 5\n###### 6");
        auto const& answer = u8"#&nbsp;1##&nbsp;2###&nbsp;3####&nbsp;4#####&nbsp;5######&nbsp;6";
        CHECK(html == answer);
    }
    TEST_CASE("quote-marker-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"> quote");
        auto const& answer = u8"&gt;&nbsp;quote";
        CHECK(html == answer);
    }
    TEST_CASE("table-marker-escaped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"| h |\n|-|\n| d |");
        auto const& answer = u8"|&nbsp;h&nbsp;||-||&nbsp;d&nbsp;|";
        CHECK(html == answer);
    }
    TEST_CASE("inline-code-runs") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"`one```two`````three```");
        auto const& answer = u8"onetwothree";
        CHECK(html == answer);
    }
    TEST_CASE("fenced-code-blocks") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"```\ncode\n```\n~~~\nmore\n~~~");
        auto const& answer = u8"```code```~~~more~~~";
        CHECK(html == answer);
    }
    TEST_CASE("inline-math-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$inline$");
        auto const& answer = u8"inline";
        CHECK(html == answer);
    }
    TEST_CASE("block-math-unwrapped") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$$block$$");
        auto const& answer = u8"block";
        CHECK(html == answer);
    }
    TEST_CASE("triple-dash-kept") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"---");
        auto const& answer = u8"---";
        CHECK(html == answer);
    }
}
