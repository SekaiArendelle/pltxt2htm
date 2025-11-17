#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"< >&\'\"");
        ::pltxt2htm_test::assert_true(html == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"");
        ::pltxt2htm_test::assert_true(html == u8"");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        ::pltxt2htm_test::assert_true(html == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }

    return 0;
}
