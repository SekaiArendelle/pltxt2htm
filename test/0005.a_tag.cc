#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<a>text</a>");
    auto answer1 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8R"(
<A      >text
</color   >)");
    auto answer2 = ::fast_io::u8string_view{
        u8"<br><span style=\"color:#0000AA;\">text<br>&lt;/color&nbsp;&nbsp;&nbsp;&gt;</span>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<A      >text</a   >");
    auto answer3 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<a>text</a><color=red>text</color>");
    auto answer4 =
        ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span><span style=\"color:red;\">text</span>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<A><a>text</a></A>");
    auto answer5 = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    // test invalid tag
    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<a  $");
    auto answer6 = ::fast_io::u8string_view{u8"test&lt;a&nbsp;&nbsp;$"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<a>text");
    auto answer7 = ::fast_io::u8string_view{u8"test<span style=\"color:#0000AA;\">text</span>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<a></a>t");
    auto answer8 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    return 0;
}
