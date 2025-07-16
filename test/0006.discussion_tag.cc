#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<discussion=642cf37a494746375aae306a>physicsLab</discussion>",
                                                  u8"localhost:5173");
    auto answer1 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(
        u8"<dIscUssion=642cf37a494746375aae306a      >physicsLab</DISCUSSION      >", u8"localhost:5173");
    auto answer2 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<dIscUssion=642cf37a494746375aae306a      >te
 xt</DISCUSSION      >
)",
        u8"localhost:5173");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
        u8"internal>te<br>&nbsp;xt</a><br>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(
        u8"<Discussion=642cf37a494746375aae306a><discussion=642cf37a494746375aae306a>physicsLab</discussion></"
        u8"Discussion>",
        u8"localhost:5173");
    auto answer4 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(
        u8"<Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>",
        u8"localhost:5173");
    auto answer5 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<DIScussion=>", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::exception::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"test<Discussion=", u8"localhost:5173");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;Discussion="};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2advanced_html(
        u8"<discussion=642cf37a494746375aae306a>text<discussion=642cf37a494746375aae306a>text</discussion></"
        u8"Discussion>",
        u8"localhost:5173");
    auto answer9 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>texttext</a>"};
    ::exception::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm::pltxt2advanced_html(
        u8"<discussion=642cf37a494746375aae306a>physics<discussion=123>L</Discussion>ab</discussion>",
        u8"localhost:5173");
    auto answer10 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physics<a "
        u8"href=\"localhost:5173/ExperimentSummary/Discussion/123\" internal>L</a>ab</a>"};
    ::exception::assert_true(html10 == answer10);

    return 0;
}
