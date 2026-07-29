#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text</code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<CODE    >text</CODE  >");
        auto answer = ::fast_io::u8string_view{u8"<code>text</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</color></code>");
        auto answer = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text</span></code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code><color=red>text</code></color>");
        auto answer = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text&lt;/code&gt;</span></code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>text<code>text</code></code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text<code>text</code></code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code>");
        auto answer = ::fast_io::u8string_view{u8"<code></code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

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

    // --- <code class="language-..."> tests ---
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-bash\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"<code class=\"language-bash\">echo</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-cpp\">fn()</code>");
        auto answer = ::fast_io::u8string_view{u8"<code class=\"language-cpp\">fn()</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-c++'>fn()</code>");
        auto answer = ::fast_io::u8string_view{u8"<code class=\"language-c++\">fn()</code>"};
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
    // unknown attribute (style) — should fall back
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code style=\"color:red\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;style=&quot;color:red&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // language- with empty language suffix — should fall back
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class=\"language-\">echo</code>");
        auto answer = ::fast_io::u8string_view{u8"&lt;code&nbsp;class=&quot;language-&quot;&gt;echo&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    // reject language suffix characters that can break out of the class attribute
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<code class='language-\" onmouseover=\"alert(1)'>x</code>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;code&nbsp;class=&apos;language-&quot;&nbsp;onmouseover=&quot;alert(1)&apos;&gt;x&lt;/code&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<code>test</code>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<code>test</code>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<code class=\"language-cpp\">code</code>");
        auto answer = ::fast_io::u8string_view{u8"<code class=\"language-cpp\">code</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
