#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"< >&\'\"");
    ::pltxt2htm_test::assert_true(html1 == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"");
    ::pltxt2htm_test::assert_true(html2 == u8"");

    auto html3 = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
    ::pltxt2htm_test::assert_true(html3 == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");

    return 0;
}
