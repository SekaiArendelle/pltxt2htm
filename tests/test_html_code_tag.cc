#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&gt;text&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<CODE    >text</CODE  >");
        auto answer = ::fast_io::u8string_view{u8"&lt;CODE&nbsp;&nbsp;&nbsp;&nbsp;&gt;text&lt;/CODE&nbsp;&nbsp;&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</color></code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&gt;<span style=\"color:red;\">text</span>&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</code></color>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&gt;<span style=\"color:red;\">text&lt;/code&gt;</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text<code>text</code></code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&gt;text&lt;code&gt;text&lt;/code&gt;&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code");
        auto answer = ::fast_io::u8string_view{u8"&lt;code"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<code></code>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;code&gt;&lt;/code&gt;t"};
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
        auto answer = ::fast_io::u8string_view{
            u8"ab<size=20>\uff1c</size>code<size=20>\uff1e</size>test<size=20>\uff1c</size>/code<size=20>\uff1e</"
            u8"size>cd"};
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
