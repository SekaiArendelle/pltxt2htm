#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"");
        pltxt2htm_test_assert_equal(html, u8"");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:#0000AA;\">test</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
        pltxt2htm_test_assert_equal(html, u8"&lt;h3&gt;test");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
        pltxt2htm_test_assert_equal(html, u8"<strong>test</strong>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
        pltxt2htm_test_assert_equal(html, u8"<em>test</em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_html(u8"<color=red><Color=#66CcFf>text</color></color>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:#66CcFf;\">text</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
        pltxt2htm_test_assert_equal(html, u8"t<strong>ex</strong>t");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"***test***");
        pltxt2htm_test_assert_equal(html, u8"<em><strong>test</strong></em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"___test___");
        pltxt2htm_test_assert_equal(html, u8"<em><strong>test</strong></em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"{Project}{Visitor}{Author}{CoAuthors}");
        pltxt2htm_test_assert_equal(html, u8"{Project}{Visitor}{Author}{CoAuthors}");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com)");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\">text</a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\">https://example.com</a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com/?a=1&amp;b=2");
        pltxt2htm_test_assert_equal(
            html, u8"<a href=\"https://example.com/?a=1&amp;b=2\">https://example.com/?a=1&amp;b=2</a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com/?q=&quot;) ");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com/?q=%22\">text</a>&nbsp;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[**bold**](https://example.com)");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\"><strong>bold</strong></a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com>text</external>");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\">text</a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com><b>bold</b></external>");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\"><strong>bold</strong></a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"![alt](https://example.com/image.png)");
        pltxt2htm_test_assert_equal(html, u8"");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[](3.tw&)");
        pltxt2htm_test_assert_equal(html, u8"[](3.tw&amp;)");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"&amp;&'\"<>\t<br>\n");
        pltxt2htm_test_assert_equal(html, u8"&amp;&amp;&apos;&quot;&lt;&gt;&nbsp;&nbsp;&nbsp;&nbsp;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8R"(\!\#\$\%\(\)\*\+\,\-\.\/\:\;\=\?\@\[\]\^\_\`\{\|\}\~\\\'\"\<\>\&)"
            u8"escaped");
        pltxt2htm_test_assert_equal(html, u8"!#$%()*+,-./:;=?@[]^_`{|}~\\&apos;&quot;&lt;&gt;&amp;escaped");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:red\">text</span>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:red;\">text</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"font-size:12px\">text</span>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"font-size:12px;\">text</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:blue;font-size:16px;\">text</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://example.com\" internal>text</a>");
        pltxt2htm_test_assert_equal(html, u8"<a href=\"https://example.com\" internal>text</a>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"__text__");
        pltxt2htm_test_assert_equal(html, u8"<strong>text</strong>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"_text_");
        pltxt2htm_test_assert_equal(html, u8"<em>text</em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"*text*");
        pltxt2htm_test_assert_equal(html, u8"<em>text</em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_html(
            u8"<span style=\"font-size:12px\"><span style=\"font-size:12px\">text</span></span>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"font-size:12px;\">text</span>");
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2common_html(u8"<span style=\"font-size:12px\"><color=red>text</color></span>");
        pltxt2htm_test_assert_equal(html, u8"<span style=\"color:red;font-size:12px;\">text</span>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<strong>text</strong>");
        pltxt2htm_test_assert_equal(html, u8"<strong>text</strong>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<em>text</em>");
        pltxt2htm_test_assert_equal(html, u8"<em>text</em>");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<experiment=id>text</experiment>");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<discussion=id>text</discussion>");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<user=id>text</user>");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<size=12>text</size>");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<p>text</p>");
        pltxt2htm_test_assert_equal(html, u8"&lt;p&gt;text&lt;/p&gt;");
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<h1>1</h1><h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        pltxt2htm_test_assert_equal(html,
                                    u8"&lt;h1&gt;1&lt;/h1&gt;&lt;h2&gt;2&lt;/h2&gt;&lt;h3&gt;3&lt;/h3&gt;"
                                    u8"&lt;h4&gt;4&lt;/h4&gt;&lt;h5&gt;5&lt;/h5&gt;&lt;h6&gt;6&lt;/h6&gt;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<del>text</del>");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<ul><li>u</li></ul><ol><li>o</li></ol>");
        pltxt2htm_test_assert_equal(
            html, u8"&lt;ul&gt;&lt;li&gt;u&lt;/li&gt;&lt;/ul&gt;&lt;ol&gt;&lt;li&gt;o&lt;/li&gt;&lt;/ol&gt;");
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<code>code</code><pre>pre</pre><blockquote>quote</blockquote>");
        pltxt2htm_test_assert_equal(html, u8"code&lt;pre&gt;pre&lt;/pre&gt;&lt;blockquote&gt;quote&lt;/blockquote&gt;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<table><caption>c</caption><colgroup><col></colgroup><thead><tr><th>h</th></tr></"
            u8"thead><tbody><tr><td>d</"
            u8"td></tr></tbody><tfoot><tr><td>f</td></tr></tfoot></table>");
        pltxt2htm_test_assert_equal(
            html,
            u8"&lt;table&gt;&lt;caption&gt;c&lt;/caption&gt;&lt;colgroup&gt;&lt;col&gt;&lt;/"
            u8"colgroup&gt;&lt;thead&gt;&lt;"
            u8"tr&gt;&lt;th&gt;h&lt;/th&gt;&lt;/tr&gt;&lt;/thead&gt;&lt;tbody&gt;&lt;tr&gt;&lt;td&gt;d&lt;/td&gt;&lt;/"
            u8"tr&gt;&"
            u8"lt;/tbody&gt;&lt;tfoot&gt;&lt;tr&gt;&lt;td&gt;f&lt;/td&gt;&lt;/tr&gt;&lt;/tfoot&gt;&lt;/table&gt;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<hr><input type=\"checkbox\" disabled><img src=\"a.png\" alt=\"a\"><!-- note -->");
        pltxt2htm_test_assert_equal(html, u8"&lt;hr&gt;&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"~~text~~");
        pltxt2htm_test_assert_equal(html, u8"text");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- item");
        pltxt2htm_test_assert_equal(html, u8"-&nbsp;item");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"1. item");
        pltxt2htm_test_assert_equal(html, u8"1.&nbsp;item");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- [x] item");
        pltxt2htm_test_assert_equal(html, u8"-&nbsp;[x]&nbsp;item");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"# 1\n## 2\n### 3\n#### 4\n##### 5\n###### 6");
        pltxt2htm_test_assert_equal(html, u8"#&nbsp;1##&nbsp;2###&nbsp;3####&nbsp;4#####&nbsp;5######&nbsp;6");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"> quote");
        pltxt2htm_test_assert_equal(html, u8"&gt;&nbsp;quote");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"| h |\n|-|\n| d |");
        pltxt2htm_test_assert_equal(html, u8"|&nbsp;h&nbsp;||-||&nbsp;d&nbsp;|");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"`one```two`````three```");
        pltxt2htm_test_assert_equal(html, u8"onetwothree");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"```\ncode\n```\n~~~\nmore\n~~~");
        pltxt2htm_test_assert_equal(html, u8"```code```~~~more~~~");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$inline$");
        pltxt2htm_test_assert_equal(html, u8"inline");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$$block$$");
        pltxt2htm_test_assert_equal(html, u8"block");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"---");
        pltxt2htm_test_assert_equal(html, u8"---");
    }

    return 0;
}
