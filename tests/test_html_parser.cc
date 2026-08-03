#include "precompile.hh"

int main() {
    // HTML elements that work identically to the original parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p>text</p>");
        auto answer = ::fast_io::u8string_view{u8"<p>text</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<em>text</em>");
        auto answer = ::fast_io::u8string_view{u8"<em>text</em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<strong>text</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<mark>text</mark>");
        auto answer = ::fast_io::u8string_view{u8"<mark>text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<MARK  >text</MaRk>");
        auto answer = ::fast_io::u8string_view{u8"<mark>text</mark>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h1>Title</h1>");
        auto answer = ::fast_io::u8string_view{u8"<h1>Title</h1>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<a href=\"http://example.com\">link</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"http://example.com\">link</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<a href=\"invalid-url\">link</a>");
        auto answer = ::fast_io::u8string_view{u8"&lt;a&nbsp;href=&quot;invalid-url&quot;&gt;link&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<img src=\"pic.png\" alt=\"pic\">");
        auto answer = ::fast_io::u8string_view{u8"<img src=\"pic.png\" alt=\"pic\">"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<br>");
        auto answer = ::fast_io::u8string_view{u8"<br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<hr>");
        auto answer = ::fast_io::u8string_view{u8"<hr>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li>item</li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li>item</li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ol><li>item</li></ol>");
        auto answer = ::fast_io::u8string_view{u8"<ol><li>item</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tr><td>cell</td></tr></table>");
        auto answer = ::fast_io::u8string_view{u8"<table><tr><td>cell</td></tr></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"color:red;\">text</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<code>code</code>");
        auto answer = ::fast_io::u8string_view{u8"<code>code</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre>pre</pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre>pre</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote>quote</blockquote>");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>quote</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<del>deleted</del>");
        auto answer = ::fast_io::u8string_view{u8"<del>deleted</del>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<!-- comment -->");
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<input type=\"checkbox\" disabled>");
        auto answer = ::fast_io::u8string_view{u8"<input type=\"checkbox\" disabled>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // <b> and <i> are NOT parsed (PL-only syntax, excluded from HTML parser)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<b>bold</b>");
        auto answer = ::fast_io::u8string_view{u8"&lt;b&gt;bold&lt;/b&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<i>italic</i>");
        auto answer = ::fast_io::u8string_view{u8"&lt;i&gt;italic&lt;/i&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Markdown syntax is NOT parsed
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"**bold**");
        auto answer = ::fast_io::u8string_view{u8"**bold**"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"~~text~~");
        auto answer = ::fast_io::u8string_view{u8"~~text~~"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"`code`");
        auto answer = ::fast_io::u8string_view{u8"`code`"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // PL syntax is NOT parsed
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<color=red>text</color>");
        auto answer = ::fast_io::u8string_view{u8"&lt;color=red&gt;text&lt;/color&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<size=20>text</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=20&gt;text&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"{project}");
        auto answer = ::fast_io::u8string_view{u8"{project}"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"{visitor}");
        auto answer = ::fast_io::u8string_view{u8"{visitor}"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<user=123>");
        auto answer = ::fast_io::u8string_view{u8"&lt;user=123&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<experiment=42>");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiment=42&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Backslash is treated as literal character (no MD escape semantics)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"\\\\n");
        auto answer = ::fast_io::u8string_view{u8"\\\\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"a\\\\tb");
        auto answer = ::fast_io::u8string_view{u8"a\\\\tb"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Entity references still work
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&amp;");
        auto answer = ::fast_io::u8string_view{u8"&amp;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&lt;text&gt;");
        auto answer = ::fast_io::u8string_view{u8"&lt;text&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Special characters
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&");
        auto answer = ::fast_io::u8string_view{u8"&amp;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<");
        auto answer = ::fast_io::u8string_view{u8"&lt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8">");
        auto answer = ::fast_io::u8string_view{u8"&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&quot;");
        auto answer = ::fast_io::u8string_view{u8"&quot;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Nested HTML tags
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p><em>text</em></p>");
        auto answer = ::fast_io::u8string_view{u8"<p><em>text</em></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li><strong>bold</strong></li></ul>");
        auto answer = ::fast_io::u8string_view{u8"<ul><li><strong>bold</strong></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Mixed: PL/MD tag nested inside HTML tag → PL/MD treated as text
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p><color=red>text</color></p>");
        auto answer = ::fast_io::u8string_view{u8"<p>&lt;color=red&gt;text&lt;/color&gt;</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Empty tags
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p></p>");
        auto answer = ::fast_io::u8string_view{u8"<p></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<em></em>");
        auto answer = ::fast_io::u8string_view{u8"<em></em>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Remaining heading levels
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        auto answer = ::fast_io::u8string_view{u8"<h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Complete table structure
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Scalar nodes handled directly by the HTML-only parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"\n\t'\"");
        auto answer = ::fast_io::u8string_view{u8"<br>&nbsp;&nbsp;&nbsp;&nbsp;&apos;&quot;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Nested tags without explicit closing tags are closed once at end of input.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote><p><em><strong>text");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><p><em><strong>text</strong></em></p></blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<p><span style=\"color:red\"><a href=\"https://example.com\"><code>text");
        auto answer = ::fast_io::u8string_view{
            u8"<p><span style=\"color:red;\"><a href=\"https://example.com\"><code>text</code></a></span></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><thead><tr><th>head");
        auto answer = ::fast_io::u8string_view{u8"<table><thead><tr><th>head</th></tr></thead></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h1><h2><h3><h4><h5><h6>heading");
        auto answer = ::fast_io::u8string_view{u8"<h1><h2><h3><h4><h5><h6>heading</h6></h5></h4></h3></h2></h1>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<del><pre>text");
        auto answer = ::fast_io::u8string_view{u8"<del><pre>text</pre></del>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li><ol><li>item");
        auto answer = ::fast_io::u8string_view{u8"<ul><li><ol><li>item</li></ol></li></ul>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tbody><tr><td>body");
        auto answer = ::fast_io::u8string_view{u8"<table><tbody><tr><td>body</td></tr></tbody></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tfoot><tr><td>foot");
        auto answer = ::fast_io::u8string_view{u8"<table><tfoot><tr><td>foot</td></tr></tfoot></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><caption>caption");
        auto answer = ::fast_io::u8string_view{u8"<table><caption>caption</caption></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><colgroup><col>");
        auto answer = ::fast_io::u8string_view{u8"<table><colgroup><col></colgroup></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Invalid opening tags are preserved as escaped text.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<del invalid><hr invalid><li><ol invalid><pre invalid><table invalid><!invalid>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;del&nbsp;invalid&gt;&lt;hr&nbsp;invalid&gt;&lt;li&gt;&lt;ol&nbsp;invalid&gt;&lt;pre&nbsp;invalid&gt;"
            u8"&lt;table&nbsp;invalid&gt;&lt;!invalid&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Mismatched closing tags are preserved inside otherwise valid elements.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<span style=\"color:red\"></x></span><a href=\"https://example.com\"></x></a><p></x></p>"
            u8"<h1></x></h1><h2></x></h2><h3></x></h3><h4></x></h4><h5></x></h5><h6></x></h6>"
            u8"<del></x></del><em></x></em><strong></x></strong><code></x></code><pre></x></pre>"
            u8"<blockquote></x></blockquote>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:red;\">&lt;/x&gt;</span><a href=\"https://example.com\">&lt;/x&gt;</a>"
            u8"<p>&lt;/x&gt;</p><h1>&lt;/x&gt;</h1><h2>&lt;/x&gt;</h2><h3>&lt;/x&gt;</h3><h4>&lt;/x&gt;</h4>"
            u8"<h5>&lt;/x&gt;</h5><h6>&lt;/x&gt;</h6><del>&lt;/x&gt;</del><em>&lt;/x&gt;</em>"
            u8"<strong>&lt;/x&gt;</strong><code>&lt;/x&gt;</code><pre>&lt;/x&gt;</pre>"
            u8"<blockquote>&lt;/x&gt;</blockquote>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<ul></x></ul><ol></x></ol><ul><li></x></li></ul><ol><li></x></li></ol>");
        auto answer = ::fast_io::u8string_view{
            u8"<ul>&lt;/x&gt;</ul><ol>&lt;/x&gt;</ol><ul><li>&lt;/x&gt;</li></ul><ol><li>&lt;/x&gt;</li></ol>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<table></x></table><table><tr></x></tr></table><table><tr><td></x></td></tr></table>"
            u8"<table><tr><th></x></th></tr></table><table><thead></x></thead></table>"
            u8"<table><tbody></x></tbody></table><table><tfoot></x></tfoot></table>"
            u8"<table><caption></x></caption></table><table><colgroup></x></colgroup></table>");
        auto answer = ::fast_io::u8string_view{
            u8"<table>&lt;/x&gt;</table><table><tr>&lt;/x&gt;</tr></table>"
            u8"<table><tr><td>&lt;/x&gt;</td></tr></table><table><tr><th>&lt;/x&gt;</th></tr></table>"
            u8"<table><thead>&lt;/x&gt;</thead></table><table><tbody>&lt;/x&gt;</tbody></table>"
            u8"<table><tfoot>&lt;/x&gt;</tfoot></table><table><caption>&lt;/x&gt;</caption></table>"
            u8"<table><colgroup>&lt;/x&gt;</colgroup></table>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
