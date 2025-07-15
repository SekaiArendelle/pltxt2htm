#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 = ::pltxt2htm::pltxt2advanced_html(u8"<experiment=642cf37a494746375aae306a>physicsLab</experiment>",
                                                  u8"localhost:5173");
    auto answer1 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm::pltxt2advanced_html(
        u8"<expEriMent=642cf37a494746375aae306a      >physicsLab</EXPERIMENT      >", u8"localhost:5173");
    auto answer2 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm::pltxt2advanced_html(
        u8R"(
<Experiment=642cf37a494746375aae306a      >te
 xt</EXPERIMENT      >
)",
        u8"localhost:5173");
    auto answer3 = ::fast_io::u8string_view{
        u8"<br><a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
        u8"internal>te<br>&nbsp;xt</a><br>"};
    ::exception::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm::pltxt2advanced_html(
        u8"<Experiment=642cf37a494746375aae306a><experiment=642cf37a494746375aae306a>physicsLab</experiment></"
        u8"experiment>",
        u8"localhost:5173");
    auto answer4 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm::pltxt2advanced_html(
        u8"<Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment>",
        u8"localhost:5173");
    auto answer5 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physicsLab</a>"};
    ::exception::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm::pltxt2advanced_html(u8"test<eXperiment=>", u8"localhost:5173");
    auto answer6 = ::fast_io::u8string_view{u8"test"};
    ::exception::assert_true(html6 == answer6);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2advanced_html(u8"test<experiment=", u8"localhost:5173");
    auto answer8 = ::fast_io::u8string_view{u8"test&lt;experiment="};
    ::exception::assert_true(html8 == answer8);

    auto html9 = ::pltxt2htm::pltxt2advanced_html(
        u8"<experiment=642cf37a494746375aae306a>text<experiment=642cf37a494746375aae306a>text</experiment></"
        u8"experiment>",
        u8"localhost:5173");
    auto answer9 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>texttext</a>"};
    ::exception::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm::pltxt2advanced_html(
        u8"<experiment=642cf37a494746375aae306a>physics<experiment=123>L</experiment>ab</experiment>",
        u8"localhost:5173");
    auto answer10 = ::fast_io::u8string_view{
        u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physics<a "
        u8"href=\"localhost:5173/ExperimentSummary/Experiment/123\" internal>L</a>ab</a>"};
    ::exception::assert_true(html10 == answer10);

    return 0;
}