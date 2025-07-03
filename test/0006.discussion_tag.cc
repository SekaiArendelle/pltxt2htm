#include <cstring>
#include <pltxt2htm/pltxt2htm.hh>
#include <exception/exception.hh>

int main() noexcept {
    auto html1 =
        ::pltxt2htm::pltxt2html(u8"<discussion=642cf37a494746375aae306a>physicsLab</discussion>", u8"localhost:5173");
    auto answer1 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>";
    ::exception::assert_true(::std::memcmp(html1.data(), answer1, html1.size()) == 0);

    auto html2 = ::pltxt2htm::pltxt2html(u8"<dIscUssion=642cf37a494746375aae306a      >physicsLab</DISCUSSION      >",
                                         u8"localhost:5173");
    auto answer2 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>";
    ::exception::assert_true(::std::memcmp(html2.data(), answer2, html2.size()) == 0);

    auto html3 = ::pltxt2htm::pltxt2html(
        u8R"(
<dIscUssion=642cf37a494746375aae306a      >te
 xt</DISCUSSION      >
)",
        u8"localhost:5173");
    auto answer3 =
        u8"<br><a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
        u8"internal>te<br>&nbsp;xt</a><br>";
    ::exception::assert_true(::std::memcmp(html3.data(), answer3, html3.size()) == 0);

    auto html4 = ::pltxt2htm::pltxt2html(
        u8"<Discussion=642cf37a494746375aae306a><discussion=642cf37a494746375aae306a>physicsLab</discussion></"
        u8"Discussion>",
        u8"localhost:5173");
    auto answer4 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>";
    ;
    ::exception::assert_true(::std::memcmp(html4.data(), answer4, html4.size()) == 0);

    auto html5 = ::pltxt2htm::pltxt2html(
        u8"<Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>",
        u8"localhost:5173");
    auto answer5 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physicsLab</a>";
    ::exception::assert_true(::std::memcmp(html5.data(), answer5, html5.size()) == 0);

    auto html6 = ::pltxt2htm::pltxt2html(u8"test<DIScussion=>", u8"localhost:5173");
    auto answer6 = u8"test";
    ::exception::assert_true(::std::memcmp(html6.data(), answer6, html6.size()) == 0);

    // test invalid tag
    auto html8 = ::pltxt2htm::pltxt2html(u8"test<Discussion=", u8"localhost:5173");
    auto answer8 = u8"test&lt;Discussion=";
    ::exception::assert_true(::std::memcmp(html8.data(), answer8, html8.size()) == 0);

    auto html9 = ::pltxt2htm::pltxt2html(
        u8"<discussion=642cf37a494746375aae306a>text<discussion=642cf37a494746375aae306a>text</discussion></"
        u8"Discussion>",
        u8"localhost:5173");
    auto answer9 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>texttext</a>";
    ::exception::assert_true(::std::memcmp(html9.data(), answer9, html9.size()) == 0);

    auto html10 = ::pltxt2htm::pltxt2html(
        u8"<discussion=642cf37a494746375aae306a>physics<discussion=123>L</Discussion>ab</discussion>",
        u8"localhost:5173");
    auto answer10 =
        u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" internal>physics<a "
        u8"href=\"localhost:5173/ExperimentSummary/Discussion/123\" internal>L</a>ab</a>";
    ::exception::assert_true(::std::memcmp(html10.data(), answer10, html10.size()) == 0);

    return 0;
}
