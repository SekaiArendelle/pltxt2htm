#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<cOLOr=red>text</color>", u8"localhost:5173");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<color=#66CcFf      >text</color   >", u8"localhost:5173");
    auto answer2 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<color=#66CcFf>te
 xt</color>
)",
        u8"localhost:5173");
    auto answer3 = ::fast_io::u8string_view{u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(u8"<color=red><Color=red>text</color></color>", u8"localhost:5173");
    auto answer4 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">text</span>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 =
        ::pltxt2htm::pltxt2advanced_html(u8"<color=red><Color=#66CcFf>text</color></color>", u8"localhost:5173");
    auto answer5 = ::fast_io::u8string_view{u8"<span style=\"color:#66CcFf;\">text</span>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<Color=#66CcFf>", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::exception::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm::pltxt2advanced_html(u8"test<Color=#66CcFf>text", u8"localhost:5173");
    auto answer7 = ::fast_io::u8string_view{u8"test<span style=\"color:#66CcFf;\">text</span>"};
    ::exception::assert_true(html7 == answer7);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"test<Color=#66CcFf  $", u8"localhost:5173");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$"};
    ::exception::assert_true(html8 == answer8);

    auto html9 =
        ::pltxt2htm::pltxt2advanced_html(u8"<color=red>text<Color=red>text</color></color>", u8"localhost:5173");
    auto answer9 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">texttext</span>"};
    ::exception::assert_true(html9 == answer9);

    auto html10 =
        ::pltxt2htm::pltxt2advanced_html(u8"<color=red>text<Color=#66ccff>text</color></color>", u8"localhost:5173");
    auto answer10 =
        ::fast_io::u8string_view{u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>"};
    ::exception::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm::pltxt2advanced_html(u8"<color=", u8"localhost:5173");
    auto answer11 = ::fast_io::u8string_view{u8"&lt;color="};
    ::exception::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm::pltxt2advanced_html(u8"<color=red>test</color", u8"localhost:5173");
    auto answer12 = ::fast_io::u8string_view{u8"<span style=\"color:red;\">test&lt;/color</span>"};
    ::exception::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm::pltxt2advanced_html(u8"t<color=red></color>t", u8"localhost:5173");
    auto answer13 = ::fast_io::u8string_view{u8"tt"};
    ::exception::assert_true(html13 == answer13);

    auto html14 = ::pltxt2htm::pltxt2advanced_html(u8"<color=$>test", u8"localhost:5173");
    auto answer14 = ::fast_io::u8string_view{u8"&lt;color=$&gt;test"};
    ::exception::assert_true(html14 == answer14);

    return 0;
}
