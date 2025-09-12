#include "exception/exception.hh"
#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\ntest\n```");
    auto answer1 = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```test```");
    auto answer2 = ::fast_io::u8string_view{u8"<code>test</code>"};
    // ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```\nte\nst\n```");
    auto answer3 = ::fast_io::u8string_view{u8"<pre><code>te<br>st</code></pre>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"```py\nprint(1)\n```");
    auto answer4 = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~\ntest\n~~~");
    auto answer5 = ::fast_io::u8string_view{u8"<pre><code>test</code></pre>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"~~~py\nprint(1)\n~~~");
    auto answer6 = ::fast_io::u8string_view{u8"<pre><code class=\"language-py\">print(1)</code></pre>"};
    ::exception::assert_true(html6 == answer6);
}
