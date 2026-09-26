#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_internal_tag") {
    // ---- plunity backend: <internal=value> is output verbatim ----
    TEST_CASE("---- plunity backend: <internal=value> is output verbati...") {
        auto const& pltext = u8"<internal=run>运行</internal>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<internal=run>运行</internal>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<internal=English>点击这里</internal>") {
        auto const& pltext = u8"<internal=English>点击这里</internal>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=English&gt;点击这里&lt;/internal&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<internal=English>点击这里</internal>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Comma-separated value (legacy color suffix) is part of t...") {
        // Comma-separated value (legacy color suffix) is part of the value.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=run,FF0000>运行</internal>");
        auto const& answer = u8"<internal=run,FF0000>运行</internal>";
        CHECK(html == answer);
    }

    TEST_CASE("Nested inline formatting inside the internal content is...") {
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
    TEST_CASE("---- web backend: <internal=value> is escaped to literal...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<INTERNAL=run>运行</INTERNAL>");
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("Value characters are HTML-escaped to prevent injection.") {
        // Value characters are HTML-escaped to prevent injection.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=a<b>c</internal>");
        auto const& answer = u8"&lt;internal=a&lt;b&gt;c&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("fixedadv web backend escapes identically.") {
        // fixedadv web backend escapes identically.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<internal=run>运行</internal>");
        auto const& answer = u8"&lt;internal=run&gt;运行&lt;/internal&gt;";
        CHECK(html == answer);
    }

    // ---- optimizer: empty internal tag is omitted ----
    TEST_CASE("---- optimizer: empty internal tag is omitted ----") {
        auto const& pltext = u8"t<internal=run></internal>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ---- malformed / non-matching input falls back to literal text ----
    TEST_CASE("---- malformed / non-matching input falls back to litera...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<internal=");
        auto const& answer = u8"test&lt;internal=";
        CHECK(html == answer);
    }
}
