#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<cOLOr=red>text</color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=#66CcFf      >text</color   >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<color=#66CcFf>te
 xt</color>
)");
        auto answer = ::fast_io::u8string_view{u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red><Color=red>text</color></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf>");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf>text");
        auto answer = ::fast_io::u8string_view{u8"test<span style=\"color:#66CcFf;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf  $");
        auto answer = ::fast_io::u8string_view{u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>text<Color=red>text</color></color>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">texttext</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>text<Color=#66ccff>text</color></color>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=");
        auto answer = ::fast_io::u8string_view{u8"&lt;color="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>test</color");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:red;\">test&lt;/color</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<color=red></color>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=$>test");
        auto answer = ::fast_io::u8string_view{u8"&lt;color=$&gt;test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
