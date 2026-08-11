#include "precompile.hh"

int main() {
    // bare <code> renders as an inline code span (same as Markdown code span)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text</code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // opening tag allows spaces before '>'; the closing tag must be exactly </code> (case-insensitive)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<CODE    >text</CODE  >");
        auto answer = ::fast_io::u8string_view{u8"<code>text&lt;/CODE&nbsp;&nbsp;&gt;</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // content is parsed as plain text (no nested inline tags, like a code span)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</color></code>");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;color=red&gt;text&lt;/color&gt;</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</code></color>");
        auto answer = ::fast_io::u8string_view{u8"<code>&lt;color=red&gt;text</code>&lt;/color&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // the first </code> closes the span; nested <code> is treated as literal text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text<code>text</code></code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text&lt;code&gt;text</code>&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // unclosed <code> auto-closes at end of input
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>");
        auto answer = ::fast_io::u8string_view{u8"<code></code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // incomplete <code (no '>') stays literal escaped text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code");
        auto answer = ::fast_io::u8string_view{u8"&lt;code"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<code></code>t");
        auto answer = ::fast_io::u8string_view{u8"t<code></code>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // --- standalone <code class="language-..."> stays literal escaped text ---
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-bash\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;language-bash&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-cpp\">fn()</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;language-cpp&quot;&gt;fn()&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-c++'>fn()</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&apos;language-c++&apos;&gt;fn()&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // case-sensitive: Language- vs language-
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"Language-bash\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;Language-bash&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // non-language- prefix
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"xxx\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;xxx&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // empty class value
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // unknown attribute (style)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code style=\"color:red\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;style=&quot;color:red&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // language- with empty language suffix
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;language-&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // attribute injection attempts stay escaped
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-\" onmouseover=\"alert(1)'>x</code>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;code&nbsp;class=&apos;language-&quot;&nbsp;onmouseover=&quot;alert(1)&apos;&gt;x&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<code>test</code>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<font=\"PhysicsLab-NerdFont SDF\"> test </font>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<code class=\"language-cpp\">code</code>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>\uff1c</size>code\u00A0class=\"language-cpp\"<size=20>\uff1e</size>code<size=20>\uff1c</size>/"
            u8"code<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
