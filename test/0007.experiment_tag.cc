#include "precompile.hh"

int main() {
    auto html1 =
        ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=642cf37a494746375aae306a>physicsLab</experiment>");
    auto answer1 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<expEriMent=642cf37a494746375aae306a      >physicsLab</EXPERIMENT      >");
    auto answer2 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8R"(
<Experiment=642cf37a494746375aae306a      >te
 xt</EXPERIMENT      >
)");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
        u8"internal>te<br>&nbsp;xt</a><br>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<Experiment=642cf37a494746375aae306a><experiment=642cf37a494746375aae306a>physicsLab</experiment></"
        u8"experiment>");
    auto answer4 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment>");
    auto answer5 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<eXperiment=123>");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<experiment=");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;experiment="};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<experiment=642cf37a494746375aae306a>text<experiment=642cf37a494746375aae306a>text</experiment></"
        u8"experiment>");
    auto answer9 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>texttext</a>"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(
        u8"<experiment=642cf37a494746375aae306a>physics<experiment=123>L</experiment>ab</experiment>");
    auto answer10 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physics<a "
        u8"href=\"localhost:5173/ExperimentSummary/Experiment/123\" internal>L</a>ab</a>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=642cf37a494746375aae306a>physicsLab");
    auto answer11 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<experiment=642cf37a494746375aae306a></experiment>t");
    auto answer12 = ::fast_io::u8string_view{u8"tt"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    auto html13 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=>t");
    auto answer13 = ::fast_io::u8string_view{u8"&lt;experiment=&gt;t"};
    ::pltxt2htm_test::assert_true(html13 == answer13);

    return 0;
}