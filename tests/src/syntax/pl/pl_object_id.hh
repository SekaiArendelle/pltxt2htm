#pragma once

#include "doctest_config.hh"

// Physics-Lab stores experiments, discussions, and users in MongoDB, so an id payload is a
// BSON ObjectId: exactly 24 hexadecimal digits, spelled in lowercase. These cases pin the
// identifier grammar at the syntax level; tests/src/ast/object_id.cc covers the type itself.
TEST_SUITE("pl_object_id") {
    TEST_CASE("uppercase-payload-is-folded") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=642CF37A494746375AAE306A>x</experiment>");
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-discussion-payload-is-folded") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<discussion=642CF37A494746375AAE306A>x</discussion>");
        auto const& answer = u8"<a href=\"localhost:5173/p/Discussion/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-user-payload-is-folded") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<user=642CF37A494746375AAE306A>x</user>");
        auto const& answer = u8"<span class='RUser' data-user='642cf37a494746375aae306a'>x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-tag-with-uppercase-payload") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<eXpEriMent=642CF37A494746375AAE306A>x</expERIMENT>");
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("all-digit-payload-parses") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=000000000000000000000000>x</experiment>");
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/000000000000000000000000\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("short-payload-stays-literal") {
        // ObjectId carries no check digits, so a payload that is not 24 digits is not an id at all.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=expid>x</experiment>");
        auto const& answer = u8"&lt;experiment=expid&gt;x&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("over-long-payload-stays-literal") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=642cf37a494746375aae306a0>x</experiment>");
        auto const& answer = u8"&lt;experiment=642cf37a494746375aae306a0&gt;x&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("non-hex-payload-stays-literal") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<experiment=642cf37a494746375aae306g>x</experiment>");
        auto const& answer = u8"&lt;experiment=642cf37a494746375aae306g&gt;x&lt;/experiment&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("user-short-payload-stays-literal") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<user=abc>x</user>");
        auto const& answer = u8"&lt;user=abc&gt;x&lt;/user&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("discussion-short-payload-stays-literal") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<discussion=abc>x</discussion>");
        auto const& answer = u8"&lt;discussion=abc&gt;x&lt;/discussion&gt;";
        CHECK(html == answer);
    }
}