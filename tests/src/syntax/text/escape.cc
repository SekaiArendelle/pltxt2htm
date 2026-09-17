#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"< >&\'\"");
        pltxt2htm_test_assert_equal(html, u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        pltxt2htm_test_assert_equal(html, u8"");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        pltxt2htm_test_assert_equal(html, u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }

    return 0;
}
