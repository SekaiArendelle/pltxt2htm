#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_trigger_tag") {
    // ---- plunity backend: <trigger=value> is output verbatim ----
    TEST_CASE("---- plunity backend: <trigger=value> is output verbatim...") {
        auto const& pltext = u8"<trigger=run>运行</trigger>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<trigger=run>运行</trigger>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<trigger=English>点击这里</trigger>") {
        auto const& pltext = u8"<trigger=English>点击这里</trigger>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=English&gt;点击这里&lt;/trigger&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<trigger=English>点击这里</trigger>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Comma-separated value (legacy color suffix) is part of t...") {
        // Comma-separated value (legacy color suffix) is part of the value.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=run,FF0000>运行</trigger>");
        auto const& answer = u8"<trigger=run,FF0000>运行</trigger>";
        CHECK(html == answer);
    }

    TEST_CASE("Nested inline formatting inside the trigger content is k...") {
        // Nested inline formatting inside the trigger content is kept.
        auto const& pltext = u8"<trigger=run><i>运行</i></trigger>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=run&gt;<em>运行</em>&lt;/trigger&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<trigger=run><i>运行</i></trigger>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ---- web backend: <trigger=value> is escaped to literal &lt;trigger&gt; ----
    TEST_CASE("---- web backend: <trigger=value> is escaped to literal...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TRIGGER=run>运行</TRIGGER>");
        auto const& answer = u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("Value characters are HTML-escaped to prevent injection.") {
        // Value characters are HTML-escaped to prevent injection.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=a<b>c</trigger>");
        auto const& answer = u8"&lt;trigger=a&lt;b&gt;c&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("Character references in values are decoded into the AST...") {
        // Character references in values are decoded into the AST and escaped exactly once by the web backend.
        auto const& pltext = u8"<trigger=a&amp;b>c</trigger>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=a&amp;b&gt;c&lt;/trigger&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<trigger=a&b>c</trigger>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Unknown references remain literal semantic text and thei...") {
        // Unknown references remain literal semantic text and their ampersand is escaped.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=a&unknown;b>c</trigger>");
        auto const& answer = u8"&lt;trigger=a&amp;unknown;b&gt;c&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("Character references cannot introduce raw control bytes...") {
        // Character references cannot introduce raw control bytes into tag values.
        auto const& pltext = u8"<trigger=a&#1;&#13;&#127;b>c</trigger>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=a���b&gt;c&lt;/trigger&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<trigger=a���b>c</trigger>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("fixedadv web backend escapes identically.") {
        // fixedadv web backend escapes identically.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<trigger=run>运行</trigger>");
        auto const& answer = u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    // ---- optimizer: empty trigger tag is omitted ----
    TEST_CASE("---- optimizer: empty trigger tag is omitted ----") {
        auto const& pltext = u8"t<trigger=run></trigger>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    // ---- malformed / non-matching input falls back to literal text ----
    TEST_CASE("---- malformed / non-matching input falls back to litera...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<trigger=");
        auto const& answer = u8"test&lt;trigger=";
        CHECK(html == answer);
    }

    TEST_CASE("<table> (HTML) must still parse as a table; not a trigge...") {
        // <table> (HTML) must still parse as a table, not a trigger.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>x</td></tr></table>");
        auto const& answer = u8"<table><tr><td>x</td></tr></table>";
        CHECK(html == answer);
    }
}
