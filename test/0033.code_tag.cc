#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>text</code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<CODE    >text</CODE  >");
        auto answer = ::fast_io::u8string_view{u8"<code>text</code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code><color=red>text</color></code>");
        auto answer = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text</span></code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code><color=red>text</code></color>");
        auto answer = ::fast_io::u8string_view{u8"<code><span style=\"color:red;\">text&lt;/code&gt;</span></code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>text<code>text</code></code>");
        auto answer = ::fast_io::u8string_view{u8"<code>text<code>text</code></code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code>");
        auto answer = ::fast_io::u8string_view{u8"<code></code>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<code");
        auto answer = ::fast_io::u8string_view{u8"&lt;code"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<code></code>t");
        auto answer = ::fast_io::u8string_view{u8"t<code></code>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}