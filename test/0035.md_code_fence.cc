#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```test```");
        auto answer = ::fast_io::u8string_view{u8"<code>test</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\nte\nst\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>te<br>st</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~\ntest\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~py\nprint(1)\n~~~");
        auto answer = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```\ntest\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```py\nprint(1)\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```py\nprint(1)");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```\nte st\n```");
        auto answer = ::fast_io::u8string_view{u8"<br><pre><code>te&nbsp;st</code></pre>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
