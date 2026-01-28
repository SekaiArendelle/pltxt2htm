#include "precompile.hh"

int main() {
    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<discussion=642cf37a494746375aae306a>physicsLab</discussion>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<dIscUssion=642cf37a494746375aae306a      >physicsLab</DISCUSSION      >");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<dIscUssion=642cf37a494746375aae306a      >te
 xt</DISCUSSION      >
)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>te<br>&nbsp;xt</a><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<Discussion=642cf37a494746375aae306a><discussion=642cf37a494746375aae306a>physicsLab</discussion></"
            u8"Discussion>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>&lt;discussion=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Discussion&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/123\" "
            u8"internal>&lt;discussion=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Discussion&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<DIScussion=123>");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<Discussion=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;Discussion="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<discussion=642cf37a494746375aae306a>text<discussion=642cf37a494746375aae306a>text</discussion></"
            u8"Discussion>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>text&lt;discussion=642cf37a494746375aae306a&gt;text</a>&lt;/Discussion&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<discussion=642cf37a494746375aae306a>physics<discussion=123>L</Discussion>ab</discussion>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physics&lt;discussion=123&gt;L</a>ab&lt;/discussion&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<discussion=642cf37a494746375aae306a></discussion>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<discussion=>t");
        auto answer = ::fast_io::u8string_view{u8"&lt;discussion=&gt;t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
