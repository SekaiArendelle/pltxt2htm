#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_escape") {
    TEST_CASE("//") {
        auto const& pltext = u8"\\\\";
        auto const& answer = u8"\\";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/!") {
        auto const& pltext = u8"\\!";
        auto const& answer = u8"!";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("//'") {
        auto const& pltext = u8"\\\"";
        auto const& answer = u8"&quot;";
        auto const& plunity_answer = u8"\"";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("/#") {
        auto const& pltext = u8"\\#";
        auto const& answer = u8"#";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/$") {
        auto const& pltext = u8"\\$";
        auto const& answer = u8"$";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/%") {
        auto const& pltext = u8"\\%";
        auto const& answer = u8"%";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/&") {
        auto const& pltext = u8"\\&";
        auto const& answer = u8"&amp;";
        auto const& plunity_answer = u8"&";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("//' (8)") {
        auto const& pltext = u8"\\\'";
        auto const& answer = u8"&apos;";
        auto const& plunity_answer = u8"'";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == plunity_answer);
    }
    TEST_CASE("/(") {
        auto const& pltext = u8"\\(";
        auto const& answer = u8"(";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/)") {
        auto const& pltext = u8"\\)";
        auto const& answer = u8")";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/*") {
        auto const& pltext = u8"\\*";
        auto const& answer = u8"*";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/+") {
        auto const& pltext = u8"\\+";
        auto const& answer = u8"+";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("case 13") {
        auto const& pltext = u8"\\,";
        auto const& answer = u8",";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/-") {
        auto const& pltext = u8"\\-";
        auto const& answer = u8"-";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/.") {
        auto const& pltext = u8"\\.";
        auto const& answer = u8".";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("// (16)") {
        auto const& pltext = u8"\\/";
        auto const& answer = u8"/";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("case 17") {
        auto const& pltext = u8"\\:";
        auto const& answer = u8":";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("case 18") {
        auto const& pltext = u8"\\;";
        auto const& answer = u8";";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/<") {
        auto const& pltext = u8"\\<";
        auto const& answer = u8"&lt;";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
    }
    TEST_CASE("/=") {
        auto const& pltext = u8"\\=";
        auto const& answer = u8"=";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/>") {
        auto const& pltext = u8"\\>";
        auto const& answer = u8"&gt;";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
    }
    TEST_CASE("/?") {
        auto const& pltext = u8"\\?";
        auto const& answer = u8"?";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/@") {
        auto const& pltext = u8"\\@";
        auto const& answer = u8"@";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/[") {
        auto const& pltext = u8"\\[";
        auto const& answer = u8"[";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/]") {
        auto const& pltext = u8"\\]";
        auto const& answer = u8"]";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/^") {
        auto const& pltext = u8"\\^";
        auto const& answer = u8"^";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/_") {
        auto const& pltext = u8"\\_";
        auto const& answer = u8"_";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/`") {
        auto const& pltext = u8"\\`";
        auto const& answer = u8"`";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/{") {
        auto const& pltext = u8"\\{";
        auto const& answer = u8"{";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/|") {
        auto const& pltext = u8"\\|";
        auto const& answer = u8"|";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/}") {
        auto const& pltext = u8"\\}";
        auto const& answer = u8"}";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/~") {
        auto const& pltext = u8"\\~";
        auto const& answer = u8"~";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == answer);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
    TEST_CASE("/a") {
        auto const& pltext = u8"\\a";
        auto const& answer = u8"\\a";
        CHECK(::pltxt2htm_test::pltxt4unittest(pltext) == pltext);
        CHECK(::pltxt2htm_test::pltxt2common_htmld(pltext) == pltext);
        CHECK(::pltxt2htm_test::pltxt2plunity_introduction(pltext) == answer);
    }
}
