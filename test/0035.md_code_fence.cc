#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\ntest\n```");
    auto answer1 = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```test```");
    auto answer2 = ::fast_io::u8string_view{u8"<code>test</code>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\nte\nst\n```");
    auto answer3 = ::fast_io::u8string_view{u8"<pre><code>te<br>st</code></pre>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```py\nprint(1)\n```");
    auto answer4 = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~\ntest\n~~~");
    auto answer5 = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~py\nprint(1)\n~~~");
    auto answer6 = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n```\ntest\n```");
    auto answer7 = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```\ntest\n```");
    auto answer8 = ::fast_io::u8string_view{u8"<br><pre><code>test</code></pre>"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n```py\nprint(1)\n```");
    auto answer9 = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```py\nprint(1)\n```");
    auto answer10 = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```py\nprint(1)");
    auto answer11 = ::fast_io::u8string_view{u8"<br><pre><code class=\"language-py\">print(1)</code></pre>"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<br>```\nte st\n```");
    auto answer12 = ::fast_io::u8string_view{u8"<br><pre><code>te&nbsp;st</code></pre>"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    return 0;
}
