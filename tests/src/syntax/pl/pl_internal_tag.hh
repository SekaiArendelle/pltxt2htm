#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_internal_tag") {
    // ---- plunity backend: <internal=value> is output verbatim ----
    TEST_CASE("basic-backend-difference") {
        auto const& pltext = u8"<internal=run>运行</internal>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<internal=run>运行</internal>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("english-value") {
        auto const& pltext = u8"<internal=English>点击这里</internal>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=English&gt;点击这里&lt;/internal&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<internal=English>点击这里</internal>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("comma-value-kept") {
        // Comma-separated value (legacy color suffix) is part of the value.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=run,FF0000>运行</internal>");
        auto const& answer = u8"<internal=run,FF0000>运行</internal>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic-kept") {
        // Nested inline formatting inside the internal content is kept.
        auto const& pltext = u8"<internal=run><i>运行</i></internal>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=run&gt;<em>运行</em>&lt;/internal&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<internal=run><i>运行</i></internal>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ---- web backend: <internal=value> is escaped to literal &lt;internal&gt; ----
    TEST_CASE("uppercase-escaped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<INTERNAL=run>运行</INTERNAL>");
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("value-injection-escaped") {
        // Value characters are HTML-escaped to prevent injection.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=a<b>c</internal>");
        auto const& answer = u8"&lt;internal=a&lt;b&gt;c&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("fixedadv-escapes-same") {
        // fixedadv web backend escapes identically.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<internal=run>运行</internal>");
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
    }

    // ---- optimizer: empty internal tag is omitted ----
    TEST_CASE("empty-tag-dropped") {
        auto const& pltext = u8"t<internal=run></internal>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ---- malformed / non-matching input falls back to literal text ----
    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<internal=");
        auto const& answer = u8"test&lt;internal=";
        CHECK(html == answer);
    }
}
