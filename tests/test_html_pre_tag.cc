#include "precompile.hh"

int main() {
    // bare <pre> is literal escaped text now
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;text&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>text<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // <pre><code> renders as a code block (same as markdown code fence)
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // Attributes on <code> are not part of the roundtrip HTML subset.
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code class=\"language-cpp\">int x;</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;pre&gt;&lt;code&nbsp;class=&quot;language-cpp&quot;&gt;int&nbsp;x;&lt;/code&gt;&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size><size=20>\uff1c</size>code\u00A0class=\"language-cpp\""
            u8"<size=20>\uff1e</size>int\u00A0x;<size=20>\uff1c</size>/code<size=20>\uff1e</size><size=20>\uff1c</"
            u8"size>/"
            u8"pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\nline1\nline2\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // Canonical style spans emitted by the HTML backend remain markup inside code blocks.
    {
        auto const pltext =
            ::fast_io::u8string_view{u8"<pre><code><span style=\"color:#cf222e;\">int</span>&nbsp;x;</code></pre>"};
        auto const html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        pltxt2htm_test_assert_equal(html, pltext);
        auto const plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n<color=#cf222e>int</color>&nbsp;x;\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto const pltext = ::fast_io::u8string_view{
            u8"<pre><code><span style=\"color:red;font-size:12px;vertical-align:2px;\">x</span></code></pre>"};
        auto const html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        pltxt2htm_test_assert_equal(html, pltext);
    }

    // An unterminated style span remains literal code and does not invalidate the block.
    {
        auto const html =
            ::pltxt2htm_test::pltxt4htmlunittest(u8"<pre><code><span style=\"color:red;\">text</code></pre>");
        auto const answer =
            ::fast_io::u8string_view{u8"<pre><code>&lt;span&nbsp;style=&quot;color:red;&quot;&gt;text</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // <pre> wrapping anything other than <code> is literal escaped text
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><b>bold</b></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;<strong>bold</strong>&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size><b>bold</b>"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // <pre>\n<code> (newline before code) is NOT a code block: <pre> is literal, <code> is an inline code span
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre>\n<code>test</code>\n</pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;<br><code>test</code><br>&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>\n"
            u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> test </font>\n"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<PRE    >text</PRE  >");
        auto answer = ::fast_io::u8string_view{u8"&lt;PRE&nbsp;&nbsp;&nbsp;&nbsp;&gt;text&lt;/PRE&nbsp;&nbsp;&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</color></pre>");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;<span style=\"color:red;\">text</span>&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</pre></color>");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;<span style=\"color:red;\">text&lt;/pre&gt;</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>text<pre>text</pre></pre>");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;text&lt;pre&gt;text&lt;/pre&gt;&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<pre></pre>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;pre&gt;&lt;/pre&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside a bare <pre> becomes <br> (literal text)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>line1\nline2</pre>");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre&gt;line1<br>line2&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside <pre><code> is preserved as \n
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><code>line1\nline2</code></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside literal-escaped <pre><color> becomes <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>line1\nline2</color></pre>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;pre&gt;<span style=\"color:red;\">line1<br>line2</span>&lt;/pre&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n outside <pre> still becomes <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"line1\nline2");
        auto answer = ::fast_io::u8string_view{u8"line1<br>line2"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // inline <pre><code> mid-text: <pre> is literal, <code> is an inline code span
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"ab<pre><code>c</code></pre>de");
        auto answer = ::fast_io::u8string_view{u8"ab&lt;pre&gt;<code>c</code>&lt;/pre&gt;de"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // <pre><code> after a line break is a code block
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"x\n<pre><code>c</code></pre>");
        auto answer = ::fast_io::u8string_view{u8"x<br><pre><code>c</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // spaces/tabs between <pre> and <code> are allowed
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>  <code>c</code></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>c</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
