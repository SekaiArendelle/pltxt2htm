#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<user=642cf37a494746375aae306a>physicsLab</user>");
        auto answer = ::fast_io::u8string_view{
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<USER=642cf37a494746375aae306a      >physicsLab</USER      >");
        auto answer = ::fast_io::u8string_view{
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<User=642cf37a494746375aae306a      >te
 xt</user      >
)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><span class='RUser' data-user=\'642cf37a494746375aae306a\'>te<br>&nbsp;xt</span><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<User=642cf37a494746375aae306a><User=642cf37a494746375aae306a>physicsLab</user></"
            u8"user>");
        auto answer = ::fast_io::u8string_view{
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<User=123><user=642cf37a494746375aae306a>physicsLab</user></User>");
        auto answer = ::fast_io::u8string_view{
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<User=123>");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // test invalid tag
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<user=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;user="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<User=642cf37a494746375aae306a>text<user=642cf37a494746375aae306a>text</user></user>");
        auto answer =
            ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>texttext</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<user=642cf37a494746375aae306a>physics<user=123>L</user>ab</user>");
        auto answer = ::fast_io::u8string_view{
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physics<span class=\'RUser\' "
            u8"data-user=\'123\'>L</span>ab</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // Optimization example: empty tag
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<user=642cf37a494746375aae306a></user>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<user=642cf37a494746375aae306a></user>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
