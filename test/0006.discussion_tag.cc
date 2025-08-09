#include "precompile.hh"

int main() noexcept {
    auto html1 =
        ::pltxt2htm_test::pltxt2advanced_htmld(u8"<discussion=642cf37a494746375aae306a>physicsLab</discussion>");
    auto answer1 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<dIscUssion=642cf37a494746375aae306a      >physicsLab</DISCUSSION      >");
    auto answer2 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8R"(
<dIscUssion=642cf37a494746375aae306a      >te
 xt</DISCUSSION      >
)");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
        u8"internal>te<br>&nbsp;xt</a><br>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<Discussion=642cf37a494746375aae306a><discussion=642cf37a494746375aae306a>physicsLab</discussion></"
        u8"Discussion>");
    auto answer4 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>");
    auto answer5 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<DIScussion=>");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Discussion=");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;Discussion="};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<discussion=642cf37a494746375aae306a>text<discussion=642cf37a494746375aae306a>text</discussion></"
        u8"Discussion>");
    auto answer9 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>texttext</a>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<discussion=642cf37a494746375aae306a>physics<discussion=123>L</Discussion>ab</discussion>");
    auto answer10 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physics<a "
        u8"href=\"localhost:5173/ExperimentSummary/Discussion/123\" internal>L</a>ab</a>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<discussion=642cf37a494746375aae306a></discussion>t");
    auto answer11 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    return 0;
}
