#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_experiment_tag") {
    TEST_CASE("basic-experiment-link") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=642cf37a494746375aae306a>physicsLab</experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<expEriMent=642cf37a494746375aae306a      >physicsLab</EXPERIMENT      >");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("newline-in-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<Experiment=642cf37a494746375aae306a      >te
 xt</EXPERIMENT      >
)");
        auto const& answer =
            u8"<br><a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>te<br>&nbsp;xt</a><br>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Experiment=642cf37a494746375aae306a><experiment=642cf37a494746375aae306a>physicsLab</experiment></"
            u8"experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>&lt;experiment=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("outer-id-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/123\" "
            u8"internal>&lt;experiment=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<eXperiment=123>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("empty-tag-spliced-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te<eXperiment=123></experiment>st");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    // test invalid tag
    TEST_CASE("incomplete-attribute-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<experiment=");
        auto const& answer = u8"test&lt;experiment=";
        CHECK(html == answer);
    }

    TEST_CASE("nested-subtag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<experiment=642cf37a494746375aae306a>text<experiment=642cf37a494746375aae306a>text</experiment></"
            u8"experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>text&lt;experiment=642cf37a494746375aae306a&gt;text</a>&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-id-mismatch-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<experiment=642cf37a494746375aae306a>physics<experiment=123>L</experiment>ab</experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>physics"
            u8"&lt;experiment=123&gt;L</a>ab&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=642cf37a494746375aae306a>physicsLab");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<experiment=642cf37a494746375aae306a></experiment>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=>t");
        auto const& answer = u8"&lt;experiment=&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=642cf37a494746375aae306a></experiment");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\" "
            u8"internal>&lt;/experiment</a>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-id-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=a>t<experiment=b>ex</experiment>t</experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/a\" "
            u8"internal>t&lt;experiment=b&gt;ex</a>t&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-inside-italic-literal") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<experiment=a><i><experiment=b>c</experiment></i></experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/a\" "
            u8"internal><em>&lt;experiment=b&gt;c&lt;/experiment&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("angle-bracket-value-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=<>test</experiment>");
        auto const& answer = u8"&lt;experiment=&lt;&gt;test&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("discussion-inside-experiment-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiment=a>t<discussion=b>ex</discussion>t</experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/a\" "
            u8"internal>t&lt;discussion=b&gt;ex&lt;/discussion&gt;t</a>";
        CHECK(html == answer);
    }

    TEST_CASE("discussion-inside-italic-literal") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<experiment=a><i>t<discussion=b>ex</discussion>t</i></experiment>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Experiment/a\" "
            u8"internal><em>t&lt;discussion=b&gt;ex&lt;/discussion&gt;t</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("experiment-inside-discussion-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussion=a>t<experiment=b>ex</experiment>t</discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/a\" "
            u8"internal>t&lt;experiment=b&gt;ex&lt;/experiment&gt;t</a>";
        CHECK(html == answer);
    }

    TEST_CASE("experiment-inside-italic-literal") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<discussion=a><i>t<experiment=b>ex</experiment>t</i></discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/a\" "
            u8"internal><em>t&lt;experiment=b&gt;ex&lt;/experiment&gt;t</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<experiment=id>text</experiment>");
        auto const& answer = u8"<experiment=id>text</experiment>";
        CHECK(html == answer);
    }
}
