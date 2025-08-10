#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<cOLOr=red>text</color>");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=#66CcFf      >text</color   >");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8R"(
<color=#66CcFf>te
 xt</color>
)");
    auto answer3 = ::fast_io::u8string_view{u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red><Color=red>text</color></color>");
    auto answer4 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red><Color=#66CcFf>text</color></color>");
    auto answer5 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf>");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf>text");
    auto answer7 = ::fast_io::u8string_view{u8"test<span style=\"color:#66CcFf;\">text</span>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    // test invalid tag
    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Color=#66CcFf  $");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>text<Color=red>text</color></color>");
    auto answer9 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">texttext</span>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>text<Color=#66ccff>text</color></color>");
    auto answer10 =
        ::fast_io::u8string_view{u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=");
    auto answer11 = ::fast_io::u8string_view{u8"&lt;color="};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=red>test</color");
    auto answer12 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">test&lt;/color</span>"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<color=red></color>t");
    auto answer13 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html13 == answer13);

    auto html14 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<color=$>test");
    auto answer14 = ::fast_io::u8string_view{u8"&lt;color=$&gt;test"};
    ::pltxt2htm_test::assert_true(html14 == answer14);

    return 0;
}
