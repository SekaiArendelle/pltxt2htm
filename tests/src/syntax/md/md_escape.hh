#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_escape") {
    TEST_CASE("backslash") {
        auto const& pltext = u8"\\\\";
        auto const& answer = u8"\\";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("exclamation") {
        auto const& pltext = u8"\\!";
        auto const& answer = u8"!";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("double-quote") {
        auto const& pltext = u8"\\\"";
        auto const& answer = u8"&quot;";
        auto const& plunity_answer = u8"\"";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("hash") {
        auto const& pltext = u8"\\#";
        auto const& answer = u8"#";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("dollar") {
        auto const& pltext = u8"\\$";
        auto const& answer = u8"$";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("percent") {
        auto const& pltext = u8"\\%";
        auto const& answer = u8"%";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("ampersand") {
        auto const& pltext = u8"\\&";
        auto const& answer = u8"&amp;";
        auto const& plunity_answer = u8"&";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("single-quote") {
        auto const& pltext = u8"\\\'";
        auto const& answer = u8"&apos;";
        auto const& plunity_answer = u8"'";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("left-paren") {
        auto const& pltext = u8"\\(";
        auto const& answer = u8"(";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("right-paren") {
        auto const& pltext = u8"\\)";
        auto const& answer = u8")";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("asterisk") {
        auto const& pltext = u8"\\*";
        auto const& answer = u8"*";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("plus") {
        auto const& pltext = u8"\\+";
        auto const& answer = u8"+";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("comma") {
        auto const& pltext = u8"\\,";
        auto const& answer = u8",";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("hyphen") {
        auto const& pltext = u8"\\-";
        auto const& answer = u8"-";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("dot") {
        auto const& pltext = u8"\\.";
        auto const& answer = u8".";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("slash") {
        auto const& pltext = u8"\\/";
        auto const& answer = u8"/";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("colon") {
        auto const& pltext = u8"\\:";
        auto const& answer = u8":";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("semicolon") {
        auto const& pltext = u8"\\;";
        auto const& answer = u8";";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("less-than") {
        auto const& pltext = u8"\\<";
        auto const& answer = u8"&lt;";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
    }
    TEST_CASE("equals") {
        auto const& pltext = u8"\\=";
        auto const& answer = u8"=";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("greater-than") {
        auto const& pltext = u8"\\>";
        auto const& answer = u8"&gt;";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
    }
    TEST_CASE("question") {
        auto const& pltext = u8"\\?";
        auto const& answer = u8"?";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("at-sign") {
        auto const& pltext = u8"\\@";
        auto const& answer = u8"@";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("left-bracket") {
        auto const& pltext = u8"\\[";
        auto const& answer = u8"[";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("right-bracket") {
        auto const& pltext = u8"\\]";
        auto const& answer = u8"]";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("caret") {
        auto const& pltext = u8"\\^";
        auto const& answer = u8"^";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("underscore") {
        auto const& pltext = u8"\\_";
        auto const& answer = u8"_";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("backtick") {
        auto const& pltext = u8"\\`";
        auto const& answer = u8"`";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("left-brace") {
        auto const& pltext = u8"\\{";
        auto const& answer = u8"{";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("pipe") {
        auto const& pltext = u8"\\|";
        auto const& answer = u8"|";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("right-brace") {
        auto const& pltext = u8"\\}";
        auto const& answer = u8"}";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("tilde") {
        auto const& pltext = u8"\\~";
        auto const& answer = u8"~";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("non-punctuation-kept") {
        auto const& pltext = u8"\\a";
        auto const& answer = u8"\\a";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == pltext);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == pltext);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
}
