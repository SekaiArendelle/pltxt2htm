#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 =
        ::pltxt2htm::pltxt2advanced_html(u8"<user=642cf37a494746375aae306a>physicsLab</user>", u8"localhost:5173");
    auto answer1 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(u8"<USER=642cf37a494746375aae306a      >physicsLab</USER      >",
                                                  u8"localhost:5173");
    auto answer2 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<User=642cf37a494746375aae306a      >te
 xt</user      >
)",
        u8"localhost:5173");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><span class='RUser' data-user=\'642cf37a494746375aae306a\'>te<br>&nbsp;xt</span><br>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(
        u8"<User=642cf37a494746375aae306a><User=642cf37a494746375aae306a>physicsLab</user></"
        u8"user>",
        u8"localhost:5173");
    auto answer4 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(u8"<User=123><user=642cf37a494746375aae306a>physicsLab</user></User>",
                                                  u8"localhost:5173");
    auto answer5 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<User=123>", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::exception::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"test<user=", u8"localhost:5173");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;user="};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2advanced_html(
        u8"<User=642cf37a494746375aae306a>text<user=642cf37a494746375aae306a>text</user></user>", u8"localhost:5173");
    auto answer9 =
        ::fast_io::u8string_view{u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>texttext</span>"};
    ::exception::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm::pltxt2advanced_html(
        u8"<user=642cf37a494746375aae306a>physics<user=123>L</user>ab</user>", u8"localhost:5173");
    auto answer10 = ::fast_io::u8string_view{
        u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physics<span class=\'RUser\' "
        u8"data-user=\'123\'>L</span>ab</span>"};
    ::exception::assert_true(html10 == answer10);

    return 0;
}
