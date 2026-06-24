#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<pre>text</pre>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<pre>text</pre>"};
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

    return 0;
}