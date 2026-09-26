#pragma once

#include "doctest_config.hh"

TEST_CASE("html_parser") {
    // HTML elements that work identically to the original parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p>text</p>");
        auto const& answer = u8"<p style=\"text-align:left\">text</p>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<em>text</em>");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<strong>text</strong>");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<mark>text</mark>");
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">text</mark>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<MARK  >text</MaRk>");
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">text</mark>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<mark style=\"background-color:red\">text</mark>");
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<mark style=\"color:red\">text</mark>");
        auto const& answer = u8"&lt;mark&nbsp;style=&quot;color:red&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h1>Title</h1>");
        auto const& answer = u8"<h1>Title</h1>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<a href=\"http://example.com\">link</a>");
        auto const& answer = u8"<a href=\"http://example.com\">link</a>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<a href=\"invalid-url\">link</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;invalid-url&quot;&gt;link&lt;/a&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<img src=\"pic.png\" alt=\"pic\">");
        auto const& answer = u8"<img src=\"pic.png\" alt=\"pic\">";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<br>");
        auto const& answer = u8"<br>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<hr>");
        auto const& answer = u8"<hr>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<hr><h1>Title</h1>");
        auto const& answer = u8"<hr><h1>Title</h1>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h1>Title</h1><hr><h2>Section</h2>");
        auto const& answer = u8"<h1>Title</h1><hr><h2>Section</h2>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<hr><h1>Section</h1><hr><p>Paragraph</p>");
        auto const& answer = u8"<hr><h1>Section</h1><hr><p style=\"text-align:left\">Paragraph</p>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li>item</li></ul>");
        auto const& answer = u8"<ul><li>item</li></ul>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ol><li>item</li></ol>");
        auto const& answer = u8"<ol><li>item</li></ol>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ol start=\"5\"><li>item</li></ol>");
        auto const& answer = u8"<ol start=\"5\"><li>item</li></ol>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tr><td>cell</td></tr></table>");
        auto const& answer = u8"<table><tr><td>cell</td></tr></table>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"color:red;\">text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<code>code</code>");
        auto const& answer = u8"<code>code</code>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre>pre</pre>");
        auto const& answer = u8"&lt;pre&gt;pre&lt;/pre&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code>code</code></pre>");
        auto const& answer = u8"<pre><code>code</code></pre>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code class=\"language-cpp\">int x;</code></pre>");
        auto const& answer = u8"<pre><code class=\"language-cpp\">int&nbsp;x;</code></pre>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"ab<pre><code>c</code></pre>de");
        auto const& answer = u8"ab&lt;pre&gt;<code>c</code>&lt;/pre&gt;de";
        CHECK(html == answer);
    }
    // Backslash is literal inside <pre><code>; \ before </code></pre> must not swallow the
    // closing tag (regression: shared try_parse infra re-introduced MD-escape parsing here).
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code>&amp;\\</code></pre>");
        auto const& answer = u8"<pre><code>&amp;\\</code></pre>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code>a\\&b\\</code></pre>");
        auto const& answer = u8"<pre><code>a\\&amp;b\\</code></pre>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote>quote</blockquote>");
        auto const& answer = u8"<blockquote>quote</blockquote>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<del>deleted</del>");
        auto const& answer = u8"<del>deleted</del>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<!-- comment -->");
        auto const& answer = u8"";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<input type=\"checkbox\" disabled>");
        auto const& answer = u8"&lt;input&nbsp;type=&quot;checkbox&quot;&nbsp;disabled&gt;";
        CHECK(html == answer);
    }

    // <b> and <i> are NOT parsed (PL-only syntax, excluded from HTML parser)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<b>bold</b>");
        auto const& answer = u8"&lt;b&gt;bold&lt;/b&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<i>italic</i>");
        auto const& answer = u8"&lt;i&gt;italic&lt;/i&gt;";
        CHECK(html == answer);
    }

    // Markdown syntax is NOT parsed
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"**bold**");
        auto const& answer = u8"**bold**";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"~~text~~");
        auto const& answer = u8"~~text~~";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"`code`");
        auto const& answer = u8"`code`";
        CHECK(html == answer);
    }

    // PL syntax is NOT parsed
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<color=red>text</color>");
        auto const& answer = u8"&lt;color=red&gt;text&lt;/color&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<size=20>text</size>");
        auto const& answer = u8"&lt;size=20&gt;text&lt;/size&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<mark=red>text</mark>");
        auto const& answer = u8"&lt;mark=red&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"{project}");
        auto const& answer = u8"{project}";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"{visitor}");
        auto const& answer = u8"{visitor}";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<user=123>");
        auto const& answer = u8"&lt;user=123&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<experiment=42>");
        auto const& answer = u8"&lt;experiment=42&gt;";
        CHECK(html == answer);
    }

    // Backslash is treated as literal character (no MD escape semantics)
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"\\\\n");
        auto const& answer = u8"\\\\n";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"a\\\\tb");
        auto const& answer = u8"a\\\\tb";
        CHECK(html == answer);
    }

    // Entity references still work
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&amp;");
        auto const& answer = u8"&amp;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&lt;text&gt;");
        auto const& answer = u8"&lt;text&gt;";
        CHECK(html == answer);
    }

    // Special characters
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&");
        auto const& answer = u8"&amp;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<");
        auto const& answer = u8"&lt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8">");
        auto const& answer = u8"&gt;";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"&quot;");
        auto const& answer = u8"&quot;";
        CHECK(html == answer);
    }

    // Nested HTML tags
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p><em>text</em></p>");
        auto const& answer = u8"<p style=\"text-align:left\"><em>text</em></p>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li><strong>bold</strong></li></ul>");
        auto const& answer = u8"<ul><li><strong>bold</strong></li></ul>";
        CHECK(html == answer);
    }

    // Mixed: PL/MD tag nested inside HTML tag → PL/MD treated as text
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p><color=red>text</color></p>");
        auto const& answer = u8"<p style=\"text-align:left\">&lt;color=red&gt;text&lt;/color&gt;</p>";
        CHECK(html == answer);
    }

    // Empty tags
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<p></p>");
        auto const& answer = u8"<p style=\"text-align:left\"></p>";
        CHECK(html == answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<em></em>");
        auto const& answer = u8"<em></em>";
        CHECK(html == answer);
    }

    // Remaining heading levels
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>");
        auto const& answer = u8"<h2>2</h2><h3>3</h3><h4>4</h4><h5>5</h5><h6>6</h6>";
        CHECK(html == answer);
    }

    // Complete table structure
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>");
        auto const& answer =
            u8"<table><caption>caption</caption><colgroup><col></colgroup><thead><tr><th "
            u8"style=\"text-align:center\">head</th></tr></thead><tbody><tr><td "
            u8"style=\"text-align:right\">body</td></tr></tbody><tfoot><tr><td>foot</td></tr></tfoot></table>";
        CHECK(html == answer);
    }

    // Scalar nodes handled directly by the HTML-only parser
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"\n\t'\"");
        auto const& answer = u8"<br>&nbsp;&nbsp;&nbsp;&nbsp;&apos;&quot;";
        CHECK(html == answer);
    }

    // Nested tags without explicit closing tags are closed once at end of input.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<blockquote><p><em><strong>text");
        auto const& answer =
            u8"<blockquote><p style=\"text-align:left\"><em><strong>text</strong></em></p></blockquote>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<p><span style=\"color:red\"><a href=\"https://example.com\"><code>text");
        auto const& answer =
            u8"<p style=\"text-align:left\"><span style=\"color:red;\"><a "
            u8"href=\"https://example.com\"><code>text</code></a></span></p>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><thead><tr><th>head");
        auto const& answer = u8"&lt;table&gt;&lt;thead&gt;&lt;tr&gt;&lt;th&gt;head";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<h1><h2><h3><h4><h5><h6>heading");
        auto const& answer = u8"<h1><h2><h3><h4><h5><h6>heading</h6></h5></h4></h3></h2></h1>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<del><pre>text");
        auto const& answer = u8"<del>&lt;pre&gt;text</del>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<ul><li><ol><li>item");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;&lt;ol&gt;&lt;li&gt;item";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tbody><tr><td>body");
        auto const& answer = u8"&lt;table&gt;&lt;tbody&gt;&lt;tr&gt;&lt;td&gt;body";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><tfoot><tr><td>foot");
        auto const& answer = u8"&lt;table&gt;&lt;tfoot&gt;&lt;tr&gt;&lt;td&gt;foot";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><caption>caption");
        auto const& answer = u8"&lt;table&gt;&lt;caption&gt;caption";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<table><colgroup><col>");
        auto const& answer = u8"&lt;table&gt;&lt;colgroup&gt;&lt;col&gt;";
        CHECK(html == answer);
    }

    // Invalid opening tags are preserved as escaped text.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<del invalid><hr invalid><li><ol invalid><pre invalid><table invalid><!invalid>");
        auto const& answer =
            u8"&lt;del&nbsp;invalid&gt;&lt;hr&nbsp;invalid&gt;&lt;li&gt;&lt;ol&nbsp;invalid&gt;&lt;pre&nbsp;invalid&gt;"
            u8"&lt;table&nbsp;invalid&gt;&lt;!invalid&gt;";
        CHECK(html == answer);
    }

    // Mismatched closing tags are preserved inside otherwise valid elements.
    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<span style=\"color:red\"></x></span><a href=\"https://example.com\"></x></a><p></x></p>"
            u8"<h1></x></h1><h2></x></h2><h3></x></h3><h4></x></h4><h5></x></h5><h6></x></h6>"
            u8"<del></x></del><em></x></em><strong></x></strong><code></x></code><pre></x></pre>"
            u8"<blockquote></x></blockquote>");
        auto const& answer =
            u8"<span style=\"color:red;\">&lt;/x&gt;</span><a href=\"https://example.com\">&lt;/x&gt;</a>"
            u8"<p "
            u8"style=\"text-align:left\">&lt;/x&gt;</p><h1>&lt;/x&gt;</h1><h2>&lt;/x&gt;</h2><h3>&lt;/x&gt;</"
            u8"h3><h4>&lt;/x&gt;</h4>"
            u8"<h5>&lt;/x&gt;</h5><h6>&lt;/x&gt;</h6><del>&lt;/x&gt;</del><em>&lt;/x&gt;</em>"
            u8"<strong>&lt;/x&gt;</strong><code>&lt;/x&gt;</code>&lt;pre&gt;&lt;/x&gt;&lt;/pre&gt;"
            u8"&lt;blockquote&gt;&lt;/x&gt;&lt;/blockquote&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<ul></x></ul><ol></x></ol><ul><li></x></li></ul><ol><li></x></li></ol>");
        auto const& answer =
            u8"&lt;ul&gt;&lt;/x&gt;&lt;/ul&gt;&lt;ol&gt;&lt;/x&gt;&lt;/ol&gt;"
            u8"&lt;ul&gt;&lt;li&gt;&lt;/x&gt;&lt;/li&gt;&lt;/ul&gt;&lt;ol&gt;&lt;li&gt;&lt;/x&gt;&lt;/li&gt;&lt;/"
            u8"ol&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(
            u8"<table></x></table><table><tr></x></tr></table><table><tr><td></x></td></tr></table>"
            u8"<table><tr><th></x></th></tr></table><table><thead></x></thead></table>"
            u8"<table><tbody></x></tbody></table><table><tfoot></x></tfoot></table>"
            u8"<table><caption></x></caption></table><table><colgroup></x></colgroup></table>");
        auto const& answer =
            u8"&lt;table&gt;&lt;/x&gt;&lt;/table&gt;&lt;table&gt;&lt;tr&gt;&lt;/x&gt;&lt;/tr&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;tr&gt;&lt;td&gt;&lt;/x&gt;&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;tr&gt;&lt;th&gt;&lt;/x&gt;&lt;/th&gt;&lt;/tr&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;thead&gt;&lt;/x&gt;&lt;/thead&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;tbody&gt;&lt;/x&gt;&lt;/tbody&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;tfoot&gt;&lt;/x&gt;&lt;/tfoot&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;caption&gt;&lt;/x&gt;&lt;/caption&gt;&lt;/table&gt;"
            u8"&lt;table&gt;&lt;colgroup&gt;&lt;/x&gt;&lt;/colgroup&gt;&lt;/table&gt;";
        CHECK(html == answer);
    }
}
