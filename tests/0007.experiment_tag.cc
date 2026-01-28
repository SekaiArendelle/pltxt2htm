#include "precompile.hh"

int main() {
    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=642cf37a494746375aae306a>physicsLab</experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<expEriMent=642cf37a494746375aae306a      >physicsLab</EXPERIMENT      >");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<Experiment=642cf37a494746375aae306a      >te
 xt</EXPERIMENT      >
)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>te<br>&nbsp;xt</a><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<Experiment=642cf37a494746375aae306a><experiment=642cf37a494746375aae306a>physicsLab</experiment></"
            u8"experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>&lt;experiment=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/experiment&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/123\" "
            u8"internal>&lt;experiment=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Experiment&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<eXperiment=123>");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te<eXperiment=123></experiment>st");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // test invalid tag
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<experiment=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;experiment="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<experiment=642cf37a494746375aae306a>text<experiment=642cf37a494746375aae306a>text</experiment></"
            u8"experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>text&lt;experiment=642cf37a494746375aae306a&gt;text</a>&lt;/experiment&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<experiment=642cf37a494746375aae306a>physics<experiment=123>L</experiment>ab</experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physics"
            u8"&lt;experiment=123&gt;L</a>ab&lt;/experiment&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=642cf37a494746375aae306a>physicsLab");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<experiment=642cf37a494746375aae306a></experiment>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=>t");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiment=&gt;t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=642cf37a494746375aae306a></experiment");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>&lt;/experiment</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<experiment=a><i><experiment=b>c</experiment></i></experiment>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/a\" "
            u8"internal><em>&lt;experiment=b&gt;c&lt;/experiment&gt;</em></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
