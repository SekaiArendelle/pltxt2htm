#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">test</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
        auto answer = ::fast_io::u8string_view{u8"test"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
        auto answer = ::fast_io::u8string_view{u8"<strong>test</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
        auto answer = ::fast_io::u8string_view{u8"<em>test</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_html(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
        auto answer = ::fast_io::u8string_view{u8"t<strong>ex</strong>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"***test***");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>test</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"___test___");
        auto answer = ::fast_io::u8string_view{u8"<em><strong>test</strong></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"{Project}{Visitor}{Author}{CoAuthors}");
        auto answer = ::fast_io::u8string_view{u8"{Project}{Visitor}{Author}{CoAuthors}"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[text](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"https://example.com");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">https://example.com</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[**bold**](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\"><strong>bold</strong></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com>text</external>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<external=https://example.com><b>bold</b></external>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\"><strong>bold</strong></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"![alt](https://example.com/image.png)");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"[](3.tw&)");
        auto answer = ::fast_io::u8string_view{u8"[](3.tw&amp;)"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"&amp;&'\"<>\t<br>\n");
        auto answer = ::fast_io::u8string_view{u8"&amp;&amp;&apos;&quot;&lt;&gt;&nbsp;&nbsp;&nbsp;&nbsp;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8R"(\!\#\$\%\(\)\*\+\,\-\.\/\:\;\=\?\@\[\]\^\_\`\{\|\}\~\\\'\"\<\>\&)"
            u8"escaped");
        auto answer = ::fast_io::u8string_view{u8"!#$%()*+,-./:;=?@[]^_`{|}~\\&apos;&quot;&lt;&gt;&amp;escaped"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:red\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"font-size:12px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:12px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<span style=\"color:blue;font-size:16px\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:blue;font-size:16px;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://example.com\" internal>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\" internal>text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"__text__");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"_text_");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<strong>text</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<em>text</em>");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<experiment=id>text</experiment>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<discussion=id>text</discussion>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<user=id>text</user>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<size=12>text</size>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<p>text</p>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<h1>1</h1><h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        auto answer = ::fast_io::u8string_view{u8"123456"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<del>text</del>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<ul><li>u</li></ul><ol><li>o</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"uo"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<code>code</code><pre>pre</pre><blockquote>quote</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"codeprequote"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<table><caption>c</caption><colgroup><col></colgroup><thead><tr><th>h</th></tr></"
            u8"thead><tbody><tr><td>d</"
            u8"td></tr></tbody><tfoot><tr><td>f</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{u8"chdf"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"<hr><input type=\"checkbox\" disabled><img src=\"a.png\" alt=\"a\"><!-- note -->");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"~~text~~");
        auto answer = ::fast_io::u8string_view{u8"text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- item");
        auto answer = ::fast_io::u8string_view{u8"item"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"1. item");
        auto answer = ::fast_io::u8string_view{u8"item"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"- [x] item");
        auto answer = ::fast_io::u8string_view{u8"item"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"# 1\n## 2\n### 3\n#### 4\n##### 5\n###### 6");
        auto answer = ::fast_io::u8string_view{u8"123456"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"> quote");
        auto answer = ::fast_io::u8string_view{u8"quote"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"| h |\n|-|\n| d |");
        auto answer = ::fast_io::u8string_view{u8"hd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"`one```two`````three```");
        auto answer = ::fast_io::u8string_view{u8"onetwothree"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"```\ncode\n```\n~~~\nmore\n~~~");
        auto answer = ::fast_io::u8string_view{u8"codemore"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$inline$");
        auto answer = ::fast_io::u8string_view{u8"inline"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"$$block$$");
        auto answer = ::fast_io::u8string_view{u8"block"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"---");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
