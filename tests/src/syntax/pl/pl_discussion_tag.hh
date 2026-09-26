#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_discussion_tag") {
    TEST_CASE("<discussion=642cf37a494746375aae306a>physicsLab</discuss...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussion=642cf37a494746375aae306a>physicsLab</discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<dIscUssion=642cf37a494746375aae306a >physicsLab</DISCUS...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<dIscUssion=642cf37a494746375aae306a      >physicsLab</DISCUSSION      >");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<dIscUssion=642cf37a494746375aae306a >te") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<dIscUssion=642cf37a494746375aae306a      >te
 xt</DISCUSSION      >
)");
        auto const& answer =
            u8"<br><a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>te<br>&nbsp;xt</a><br>";
        CHECK(html == answer);
    }

    TEST_CASE("<Discussion=642cf37a494746375aae306a><discussion=642cf37...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Discussion=642cf37a494746375aae306a><discussion=642cf37a494746375aae306a>physicsLab</discussion></"
            u8"Discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>&lt;discussion=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Discussion&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<Discussion=123><discussion=642cf37a494746375aae306a>phy...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/123\" "
            u8"internal>&lt;discussion=642cf37a494746375aae306a&gt;physicsLab</a>&lt;/Discussion&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("test<DIScussion=123>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<DIScussion=123>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("test invalid tag") {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Discussion=");
        auto const& answer = u8"test&lt;Discussion=";
        CHECK(html == answer);
    }

    TEST_CASE("<discussion=642cf37a494746375aae306a>text<discussion=642...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<discussion=642cf37a494746375aae306a>text<discussion=642cf37a494746375aae306a>text</discussion></"
            u8"Discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>text&lt;discussion=642cf37a494746375aae306a&gt;text</a>&lt;/Discussion&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<discussion=642cf37a494746375aae306a>physics<discussion=...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<discussion=642cf37a494746375aae306a>physics<discussion=123>L</Discussion>ab</discussion>");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\" "
            u8"internal>physics&lt;discussion=123&gt;L</a>ab&lt;/discussion&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("t<discussion=642cf37a494746375aae306a></discussion>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<discussion=642cf37a494746375aae306a></discussion>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("<discussion=>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussion=>t");
        auto const& answer = u8"&lt;discussion=&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("<discussion=123></discussion") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussion=123></discussion");
        auto const& answer =
            u8"<a href=\"localhost:5173/ExperimentSummary/Discussion/123\" internal>&lt;/discussion</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<discussion=id>text</discussion>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<discussion=id>text</discussion>");
        auto const& answer = u8"<discussion=id>text</discussion>";
        CHECK(html == answer);
    }
}
