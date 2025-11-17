#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<a>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<A      >text
</color   >)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/color&nbsp;&nbsp;&nbsp;&gt;</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<A      >text</a   >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<a>text</a><color=red>text</color>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<A><a>text</a></A>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<a  $");
        auto answer = ::fast_io::u8string_view{u8"test&lt;a&nbsp;&nbsp;$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<a>text");
        auto answer = ::fast_io::u8string_view{u8"test<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<a></a>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
