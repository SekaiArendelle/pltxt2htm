#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_mark_tag") {
    TEST_CASE("default-yellow-highlight") {
        auto const& pltext = u8"<mark>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FFFF00>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-tag-whitespace") {
        auto const& pltext = u8"<MARK    >text</Mark  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FFFF00>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-color-inside") {
        auto const& pltext = u8"<mark><color=red>text</color></mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<mark style=\"background-color:#FFFF00;\"><span style=\"color:red;\">text</span></mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FFFF00><color=red>text</color></mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("misnested-close-escaped") {
        auto const& pltext = u8"<mark><color=red>text</mark></color>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<mark style=\"background-color:#FFFF00;\"><span style=\"color:red;\">text&lt;/mark&gt;</span></mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<mark=#FFFF00><color=red>text<size=20>\uFF1C</size>/mark<size=20>\uFF1E</size></color></mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-same-tag-collapse") {
        auto const& pltext = u8"<Mark>text<mark>text</mark></Mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">texttext</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FFFF00>texttext</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-tag-dropped") {
        auto const& pltext = u8"text<mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-tag-removed") {
        auto const& pltext = u8"t<mark></mark>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto const& pltext = u8"<mark></mark";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">&lt;/mark</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FFFF00><size=20>\uFF1C</size>/mark</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("style-named-color") {
        auto const& pltext = u8"<mark style=\"background-color:red\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("style-hex-color") {
        auto const& pltext = u8"<mark style=\"background-color:#FF0000\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FF0000;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FF0000>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("style-attribute-spacing") {
        auto const& pltext = u8"<mark  style=\"background-color:red\"  >text</mark  >";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-equal-color-merge") {
        auto const& pltext =
            u8"<mark style=\"background-color:yellow\">a<mark style=\"background-color:yellow\">b</mark>c</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:yellow;\">abc</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=yellow>abc</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-distinct-colors") {
        auto const& pltext =
            u8"<mark style=\"background-color:red\">a<mark style=\"background-color:blue\">b</mark>c</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<mark style=\"background-color:red;\">a<mark style=\"background-color:blue;\">b</mark>c</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>a<mark=blue>b</mark>c</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("other-css-props-rejected") {
        // XSS: other CSS properties are rejected and the tag degrades to literal text
        auto const& pltext = u8"<mark style=\"color:red\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;style=&quot;color:red&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-style-attr") {
        // XSS: uppercase STYLE attribute is rejected
        auto const& pltext = u8"<mark STYLE=\"background-color:red\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;STYLE=&quot;background-color:red&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-css-property") {
        // XSS: uppercase CSS property is rejected
        auto const& pltext = u8"<mark style=\"BACKGROUND-COLOR:red\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;style=&quot;BACKGROUND-COLOR:red&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("multi-prop-style-rejected") {
        // XSS: extra CSS property after background-color is rejected
        auto const& pltext = u8"<mark style=\"background-color:red;color:blue\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;style=&quot;background-color:red;color:blue&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate-css-rejected") {
        // XSS: duplicate background-color property is rejected
        auto const& pltext = u8"<mark style=\"background-color:red;background-color:blue\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"&lt;mark&nbsp;style=&quot;background-color:red;background-color:blue&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("url-color-rejected") {
        // XSS: url(...) color value is rejected
        auto const& pltext = u8"<mark style=\"background-color:url(javascript:alert(1))\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"&lt;mark&nbsp;style=&quot;background-color:url(javascript:alert(1))&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("invalid-hex-rejected") {
        // XSS: invalid hex color value is rejected
        auto const& pltext = u8"<mark style=\"background-color:#GGG\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;style=&quot;background-color:#GGG&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-attribute-rejected") {
        // XSS: unknown attribute is rejected
        auto const& pltext = u8"<mark class=\"foo\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark&nbsp;class=&quot;foo&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("empty-styled-tag-removed") {
        auto const& pltext = u8"t<mark style=\"background-color:red\"></mark>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("shorthand-named-color") {
        auto const& pltext = u8"<mark=red>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("shorthand-hex-color") {
        auto const& pltext = u8"<mark=#FF0000>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FF0000;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=#FF0000>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase-shorthand-color") {
        auto const& pltext = u8"<MARK=red>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("shorthand-trailing-space") {
        auto const& pltext = u8"<mark=red >text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:red;\">text</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>text</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-shorthand-merge") {
        auto const& pltext = u8"<mark=yellow>a<mark=yellow>b</mark>c</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:yellow;\">abc</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=yellow>abc</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-shorthand-distinct") {
        auto const& pltext = u8"<mark=red>a<mark=blue>b</mark>c</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<mark style=\"background-color:red;\">a<mark style=\"background-color:blue;\">b</mark>c</mark>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<mark=red>a<mark=blue>b</mark>c</mark>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-shorthand-removed") {
        auto const& pltext = u8"t<mark=red></mark>t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("title-backend-strips-highlight") {
        // title backend mirrors html_mark title behavior
        auto const& pltext = u8"<mark=red>text</mark>";
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto const& answer = u8"text";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-rejected") {
        // XSS: empty value is rejected and the tag degrades to escaped literal text
        auto const& pltext = u8"<mark=>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("numeric-value-rejected") {
        // XSS: numeric value is rejected
        auto const& pltext = u8"<mark=123>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=123&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-css-rejected") {
        // XSS: extra CSS after the color value is rejected
        auto const& pltext = u8"<mark=red;color:blue>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=red;color:blue&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("shorthand-url-rejected") {
        // XSS: url(...) color value is rejected
        auto const& pltext = u8"<mark=url(javascript:alert(1))>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=url(javascript:alert(1))&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("shorthand-invalid-hex") {
        // XSS: invalid hex color value is rejected
        auto const& pltext = u8"<mark=#GGG>text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=#GGG&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-attr-rejected") {
        // XSS: extra attribute after the color value is rejected
        auto const& pltext = u8"<mark=red class=\"x\">text</mark>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=red&nbsp;class=&quot;x&quot;&gt;text&lt;/mark&gt;";
        CHECK(html == answer);
    }
}
