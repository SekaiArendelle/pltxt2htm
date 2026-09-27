#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_user_tag") {
    TEST_CASE("basic-user-span") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<user=642cf37a494746375aae306a>physicsLab</user>");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<USER=642cf37a494746375aae306a      >physicsLab</USER      >");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>";
        CHECK(html == answer);
    }

    TEST_CASE("newline-in-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<User=642cf37a494746375aae306a      >te
 xt</user      >
)");
        auto const& answer =
            u8"<br><span class='RUser' data-user=\'642cf37a494746375aae306a\'>te<br>&nbsp;xt</span><br>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-id-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<User=642cf37a494746375aae306a><User=642cf37a494746375aae306a>physicsLab</user></"
            u8"user>");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>";
        CHECK(html == answer);
    }

    TEST_CASE("inner-id-wins") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<User=123><user=642cf37a494746375aae306a>physicsLab</user></User>");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physicsLab</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<User=123>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    // test invalid tag
    TEST_CASE("incomplete-attribute-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<user=");
        auto const& answer = u8"test&lt;user=";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-id-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<User=642cf37a494746375aae306a>text<user=642cf37a494746375aae306a>text</user></user>");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>texttext</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-id-kept") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<user=642cf37a494746375aae306a>physics<user=123>L</user>ab</user>");
        auto const& answer =
            u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>physics<span class=\'RUser\' "
            u8"data-user=\'123\'>L</span>ab</span>";
        CHECK(html == answer);
    }

    // Optimization example: empty tag
    TEST_CASE("empty-tag-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<user=642cf37a494746375aae306a></user>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<user=642cf37a494746375aae306a></user>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<user=642cf37a494746375aae306a></user");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'642cf37a494746375aae306a\'>&lt;/user</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<user=xxx><i>test</i></user>");
        auto const& answer = u8"<span class=\'RUser\' data-user=\'xxx\'><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-italic-only") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<user=xxx><i>test</i></user>");
        auto const& answer = u8"<em>test</em>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<user=id>text</user>");
        auto const& answer = u8"<user=id>text</user>";
        CHECK(html == answer);
    }
}
