#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_color_tag") {
    TEST_CASE("<cOLOr=red>text</color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<cOLOr=red>text</color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=#66CcFf >text</color >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#66CcFf      >text</color   >");
        auto const& answer = u8"<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=#66CcFf>te") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<color=#66CcFf>te
 xt</color>
)");
        auto const& answer = u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red><Color=red>text</color></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><Color=red>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red><Color=#66CcFf>text</color></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto const& answer = u8"<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("test<Color=#66CcFf>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("test<Color=#66CcFf>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf>text");
        auto const& answer = u8"test<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("test invalid tag") {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf  $");
        auto const& answer = u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>text<Color=red>text</color></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>text<Color=red>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">texttext</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>t<Color=red>ex</color>t</color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>t<Color=red>ex</color>t</color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>text<Color=#66ccff>text</color></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>text<Color=#66ccff>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=");
        auto const& answer = u8"&lt;color=";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>test</color") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>test</color");
        auto const& answer = u8"<span style=\"color:red;\">test&lt;/color</span>";
        CHECK(html == answer);
    }

    TEST_CASE("t<color=red></color>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<color=red></color>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("<color=$>test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=$>test");
        auto const& answer = u8"&lt;color=$&gt;test";
        CHECK(html == answer);
    }

    TEST_CASE("https://github.com/SekaiArendelle/pltxt2htm/issues/20") {
        // https://github.com/SekaiArendelle/pltxt2htm/issues/20
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>test</a>");
        auto const& answer = u8"<span style=\"color:red;\">test</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red><a>test</a></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><a>test</a></color>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">test</span>";
        CHECK(html == answer);
    }

    TEST_CASE("invalid hex length should be rejected") {
        // invalid hex length should be rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#12345>test</color>");
        auto const& answer = u8"&lt;color=#12345&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<color=#123>a</color><color=#1234>b</color><color=#12345...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<color=#123>a</color><color=#1234>b</color><color=#12345678>c</color>");
        auto const& answer =
            u8"<span style=\"color:#123;\">a</span><span style=\"color:#1234;\">b</span><span "
            u8"style=\"color:#12345678;\">c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red$>test</color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red$>test</color>");
        auto const& answer = u8"&lt;color=red$&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block style injection / XSS payload in color value") {
        // block style injection / XSS payload in color value
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red;background:urljavascriptalert1>test</color>");
        auto const& answer = u8"&lt;color=red;background:urljavascriptalert1&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red><i>test</i></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><i>test</i></color>");
        auto const& answer = u8"<span style=\"color:red;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>text</color>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<color=red>text</color>");
        auto const& answer = u8"<color=red>text</color>";
        CHECK(html == answer);
    }
}
