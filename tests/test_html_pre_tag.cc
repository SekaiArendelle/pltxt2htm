#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>text<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // <pre><code> renders as a NerdFont code block (same as markdown code fence)
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<font=\"PhysicsLab-NerdFont SDF\">\ntest\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code class=\"language-cpp\">int x;</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-cpp\">int&nbsp;x;</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-NerdFont SDF\">\nint\u00A0x;\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<font=\"PhysicsLab-NerdFont SDF\">\nline1\nline2\n</font>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // <pre> wrapping anything other than a lone <code> keeps the escaped passthrough form
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre><b>bold</b></pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre><strong>bold</strong></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size><b>bold</b>"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    // <pre>\n<code> (whitespace before code) is NOT a lone-code pre -> escaped form
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre>\n<code>test</code>\n</pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre>\n<code>test</code>\n</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>\n"
            u8"<size=20>\uff1c</size>code<size=20>\uff1e</size>test<size=20>\uff1c</size>/code<size=20>\uff1e</size>\n"
            u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<PRE    >text</PRE  >");
        auto answer = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</color></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre><span style=\"color:red;\">text</span></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>text</pre></color>");
        auto answer = ::fast_io::u8string_view{u8"<pre><span style=\"color:red;\">text&lt;/pre&gt;</span></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>text<pre>text</pre></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre>text<pre>text</pre></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre");
        auto answer = ::fast_io::u8string_view{u8"&lt;pre"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<pre></pre>t");
        auto answer = ::fast_io::u8string_view{u8"t<pre></pre>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside <pre> should be preserved as \n, not converted to <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre>line1\nline2</pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre>line1\nline2</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside <pre><code> should also preserve \n
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><code>line1\nline2</code></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>line1\nline2</code></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n inside <pre><color> should preserve \n
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<pre><color=red>line1\nline2</color></pre>");
        auto answer = ::fast_io::u8string_view{u8"<pre><span style=\"color:red;\">line1\nline2</span></pre>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // \n outside <pre> should still become <br>
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"line1\nline2");
        auto answer = ::fast_io::u8string_view{u8"line1<br>line2"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}