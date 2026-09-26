#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_color_tag") {
    TEST_CASE("case-insensitive-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<cOLOr=red>text</color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("extra-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#66CcFf      >text</color   >");
        auto const& answer = u8"<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("newline-in-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<color=#66CcFf>te
 xt</color>
)");
        auto const& answer = u8"<br><span style=\"color:#66CcFf;\">te<br>&nbsp;xt</span><br>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-color-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><Color=red>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("inner-color-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><Color=#66CcFf>text</color></color>");
        auto const& answer = u8"<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf>text");
        auto const& answer = u8"test<span style=\"color:#66CcFf;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("malformed-tag-literal") {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<Color=#66CcFf  $");
        auto const& answer = u8"test&lt;Color=#66CcFf&nbsp;&nbsp;$";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-color-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>text<Color=red>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">texttext</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-color-split") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>t<Color=red>ex</color>t</color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("different-color-nested") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>text<Color=#66ccff>text</color></color>");
        auto const& answer = u8"<span style=\"color:red;\">text<span style=\"color:#66ccff;\">text</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=");
        auto const& answer = u8"&lt;color=";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>test</color");
        auto const& answer = u8"<span style=\"color:red;\">test&lt;/color</span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<color=red></color>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("invalid-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=$>test");
        auto const& answer = u8"&lt;color=$&gt;test";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-close-still-renders") {
        // https://github.com/SekaiArendelle/pltxt2htm/issues/20
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>test</a>");
        auto const& answer = u8"<span style=\"color:red;\">test</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-a-tag-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><a>test</a></color>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">test</span>";
        CHECK(html == answer);
    }

    TEST_CASE("invalid-hex-length") {
        // invalid hex length should be rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=#12345>test</color>");
        auto const& answer = u8"&lt;color=#12345&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("hex-lengths-accepted") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<color=#123>a</color><color=#1234>b</color><color=#12345678>c</color>");
        auto const& answer =
            u8"<span style=\"color:#123;\">a</span><span style=\"color:#1234;\">b</span><span "
            u8"style=\"color:#12345678;\">c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-dollars-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red$>test</color>");
        auto const& answer = u8"&lt;color=red$&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("style-injection-rejected") {
        // block style injection / XSS payload in color value
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red;background:urljavascriptalert1>test</color>");
        auto const& answer = u8"&lt;color=red;background:urljavascriptalert1&gt;test&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red><i>test</i></color>");
        auto const& answer = u8"<span style=\"color:red;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<color=red>text</color>");
        auto const& answer = u8"<color=red>text</color>";
        CHECK(html == answer);
    }
}
