#include "precompile.hh"

int main() noexcept {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<user=642cf37a494746375aae306a>physicsLab</user>");
    auto answer1 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 =
        ::pltxt2htm_test::pltxt2advanced_htmld(u8"<USER=642cf37a494746375aae306a      >physicsLab</USER      >");
    auto answer2 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8R"(
<User=642cf37a494746375aae306a      >te
 xt</user      >
)");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><span class='RUser' data-user=\'642cf37a494746375aae306a\'>te<br>&nbsp;xt</span><br>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<User=642cf37a494746375aae306a><User=642cf37a494746375aae306a>physicsLab</user></"
        u8"user>");
    auto answer4 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 =
        ::pltxt2htm_test::pltxt2advanced_htmld(u8"<User=123><user=642cf37a494746375aae306a>physicsLab</user></User>");
    auto answer5 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<User=123>");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<user=");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;user="};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<User=642cf37a494746375aae306a>text<user=642cf37a494746375aae306a>text</user></user>");
    auto answer9 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>texttext</span>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 =
        ::pltxt2htm_test::pltxt2advanced_htmld(u8"<user=642cf37a494746375aae306a>physics<user=123>L</user>ab</user>");
    auto answer10 = ::fast_io::u8string_view{
        u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physics<span class=\'RUser\' "
        u8"data-user=\'123\'>L</span>ab</span>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    // Optimization example: empty tag
    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<user=642cf37a494746375aae306a></user>t");
    auto answer11 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<user=642cf37a494746375aae306a></user>t");
    auto answer12 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    return 0;
}
