#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"");
        auto answer = ::fast_io::u8string_view{u8""};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a>test");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">test</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<h3>test");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b>test");
        auto answer = ::fast_io::u8string_view{u8"<strong>test</strong>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<i>test");
        auto answer = ::fast_io::u8string_view{u8"<em>test</em>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_html(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"t<b>ex</b>t");
        auto answer = ::fast_io::u8string_view{u8"t<strong>ex</strong>t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
