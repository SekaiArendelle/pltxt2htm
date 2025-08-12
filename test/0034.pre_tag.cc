#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre>text</pre>");
    auto answer1 = ::fast_io::u8string_view{u8"<pre>text</pre>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<PRE    >text</PRE  >");
    auto answer2 = ::fast_io::u8string_view{u8"<pre>text</pre>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre><color=red>text</color></pre>");
    auto answer3 = ::fast_io::u8string_view{u8"<pre><span style=\"color:red;\">text</span></pre>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre><color=red>text</pre></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<pre><span style=\"color:red;\">text&lt;/pre&gt;</span></pre>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre>text<pre>text</pre></pre>");
    auto answer5 = ::fast_io::u8string_view{u8"<pre>text<pre>text</pre></pre>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre>");
    auto answer6 = ::fast_io::u8string_view{u8"<pre></pre>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<pre");
    auto answer7 = ::fast_io::u8string_view{u8"&lt;pre"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<pre></pre>t");
    auto answer8 = ::fast_io::u8string_view{u8"t<pre></pre>t"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    return 0;
}