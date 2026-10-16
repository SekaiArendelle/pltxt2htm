#pragma once

#include "doctest_config.hh"

// The inline parser turns inline pl-text into an AST; `inline_pltxt4unittest` renders that
// AST with the unit-test backend. These cases pin the inline frame lifecycle end to end -
// a tag closed by its own closing tag, a tag still open when the input ends, a closing tag
// that matches no frame, and a `<` that starts no tag - without looking at parser internals.

TEST_SUITE("inline_parser") {
    // Each inline tag, closed by its own closing tag.
    TEST_CASE("closed-a-tag") {
        auto const& pltext = u8"<a>x</a>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:#0000AA;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-a-tag-with-href") {
        auto const& pltext = u8"<a href=\"https://example.com\">x</a>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-b-tag") {
        auto const& pltext = u8"<b>x</b>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-code-tag") {
        auto const& pltext = u8"<code>x</code>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x</code>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-color-tag") {
        auto const& pltext = u8"<color=red>x</color>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-del-tag") {
        auto const& pltext = u8"<del>x</del>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<del>x</del>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-discussion-tag") {
        auto const& pltext = u8"<discussion=642cf37a494746375aae306a>x</discussion>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer =
            u8"<a href=\"localhost:5173/p/Discussion/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-discussions-tag") {
        auto const& pltext = u8"<discussions=abc>x</discussions>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;discussions=abc&gt;x&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("closed-em-tag") {
        auto const& pltext = u8"<em>x</em>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-experiment-tag") {
        auto const& pltext = u8"<experiment=642cf37a494746375aae306a>x</experiment>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-experiments-tag") {
        auto const& pltext = u8"<experiments=abc>x</experiments>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;experiments=abc&gt;x&lt;/experiments&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("closed-external-tag") {
        auto const& pltext = u8"<external=https://example.com>x</external>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-i-tag") {
        auto const& pltext = u8"<i>x</i>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-internal-tag") {
        auto const& pltext = u8"<internal=456>x</internal>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=456&gt;x&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("closed-link-tag") {
        auto const& pltext = u8"<link=\"https://example.com\">x</link>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-mark-tag") {
        auto const& pltext = u8"<mark>x</mark>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">x</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-mark-tag-with-color") {
        auto const& pltext = u8"<mark=#FF0000>x</mark>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FF0000;\">x</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-s-tag") {
        auto const& pltext = u8"<s>x</s>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<s>x</s>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-size-tag") {
        auto const& pltext = u8"<size=20>x</size>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:10px;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-span-tag") {
        auto const& pltext = u8"<span style=\"color:red\">x</span>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-strong-tag") {
        auto const& pltext = u8"<strong>x</strong>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-sub-tag") {
        auto const& pltext = u8"<sub>x</sub>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sub>x</sub>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-sup-tag") {
        auto const& pltext = u8"<sup>x</sup>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sup>x</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-trigger-tag") {
        auto const& pltext = u8"<trigger=abc>x</trigger>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=abc&gt;x&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("closed-u-tag") {
        auto const& pltext = u8"<u>x</u>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<u>x</u>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-user-tag") {
        auto const& pltext = u8"<user=642cf37a494746375aae306a>x</user>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span class='RUser' data-user='642cf37a494746375aae306a'>x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("closed-voffset-tag") {
        auto const& pltext = u8"<voffset=5>x</voffset>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:5px;\">x</span>";
        CHECK(html == answer);
    }

    // The same frames left open: the trailing input must still close them.
    TEST_CASE("unclosed-a-tag") {
        auto const& pltext = u8"<a>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:#0000AA;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-a-tag-with-href") {
        auto const& pltext = u8"<a href=\"https://example.com\">x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-b-tag") {
        auto const& pltext = u8"<b>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-code-tag") {
        auto const& pltext = u8"<code>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x</code>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-color-tag") {
        auto const& pltext = u8"<color=red>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-del-tag") {
        auto const& pltext = u8"<del>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<del>x</del>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-discussion-tag") {
        auto const& pltext = u8"<discussion=642cf37a494746375aae306a>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer =
            u8"<a href=\"localhost:5173/p/Discussion/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-discussions-tag") {
        auto const& pltext = u8"<discussions=abc>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;discussions=abc&gt;x&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-em-tag") {
        auto const& pltext = u8"<em>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-experiment-tag") {
        auto const& pltext = u8"<experiment=642cf37a494746375aae306a>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" internal>x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-experiments-tag") {
        auto const& pltext = u8"<experiments=abc>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;experiments=abc&gt;x&lt;/experiments&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-external-tag") {
        auto const& pltext = u8"<external=https://example.com>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-i-tag") {
        auto const& pltext = u8"<i>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-internal-tag") {
        auto const& pltext = u8"<internal=456>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=456&gt;x&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-link-tag") {
        auto const& pltext = u8"<link=\"https://example.com\">x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x</a>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-mark-tag") {
        auto const& pltext = u8"<mark>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">x</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-mark-tag-with-color") {
        auto const& pltext = u8"<mark=#FF0000>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FF0000;\">x</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-s-tag") {
        auto const& pltext = u8"<s>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<s>x</s>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-size-tag") {
        auto const& pltext = u8"<size=20>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:10px;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-span-tag") {
        auto const& pltext = u8"<span style=\"color:red\">x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-strong-tag") {
        auto const& pltext = u8"<strong>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-sub-tag") {
        auto const& pltext = u8"<sub>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sub>x</sub>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-sup-tag") {
        auto const& pltext = u8"<sup>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sup>x</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-trigger-tag") {
        auto const& pltext = u8"<trigger=abc>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=abc&gt;x&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-u-tag") {
        auto const& pltext = u8"<u>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<u>x</u>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-user-tag") {
        auto const& pltext = u8"<user=642cf37a494746375aae306a>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span class='RUser' data-user='642cf37a494746375aae306a'>x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-voffset-tag") {
        auto const& pltext = u8"<voffset=5>x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:5px;\">x</span>";
        CHECK(html == answer);
    }

    // A closing tag that names no open frame stays literal, and the frame it failed to close is still closed at the end
    // of the input.
    TEST_CASE("mismatched-a-tag") {
        auto const& pltext = u8"<a>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:#0000AA;\">x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-a-tag-with-href") {
        auto const& pltext = u8"<a href=\"https://example.com\">x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x&lt;/q&gt;</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-b-tag") {
        auto const& pltext = u8"<b>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x&lt;/q&gt;</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-code-tag") {
        auto const& pltext = u8"<code>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x&lt;/q&gt;</code>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-color-tag") {
        auto const& pltext = u8"<color=red>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-del-tag") {
        auto const& pltext = u8"<del>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<del>x&lt;/q&gt;</del>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-discussion-tag") {
        auto const& pltext = u8"<discussion=642cf37a494746375aae306a>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer =
            u8"<a href=\"localhost:5173/p/Discussion/642cf37a494746375aae306a\" "
            u8"internal>x&lt;/q&gt;</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-discussions-tag") {
        auto const& pltext = u8"<discussions=abc>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;discussions=abc&gt;x&lt;/q&gt;&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-em-tag") {
        auto const& pltext = u8"<em>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x&lt;/q&gt;</em>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-experiment-tag") {
        auto const& pltext = u8"<experiment=642cf37a494746375aae306a>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"localhost:5173/p/Experiment/642cf37a494746375aae306a\" internal>x&lt;/q&gt;</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-experiments-tag") {
        auto const& pltext = u8"<experiments=abc>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;experiments=abc&gt;x&lt;/q&gt;&lt;/experiments&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-external-tag") {
        auto const& pltext = u8"<external=https://example.com>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x&lt;/q&gt;</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-i-tag") {
        auto const& pltext = u8"<i>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x&lt;/q&gt;</em>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-internal-tag") {
        auto const& pltext = u8"<internal=456>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;internal=456&gt;x&lt;/q&gt;&lt;/internal&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-link-tag") {
        auto const& pltext = u8"<link=\"https://example.com\">x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">x&lt;/q&gt;</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-mark-tag") {
        auto const& pltext = u8"<mark>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FFFF00;\">x&lt;/q&gt;</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-mark-tag-with-color") {
        auto const& pltext = u8"<mark=#FF0000>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<mark style=\"background-color:#FF0000;\">x&lt;/q&gt;</mark>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-s-tag") {
        auto const& pltext = u8"<s>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<s>x&lt;/q&gt;</s>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-size-tag") {
        auto const& pltext = u8"<size=20>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:10px;\">x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-span-tag") {
        auto const& pltext = u8"<span style=\"color:red\">x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-strong-tag") {
        auto const& pltext = u8"<strong>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x&lt;/q&gt;</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-sub-tag") {
        auto const& pltext = u8"<sub>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sub>x&lt;/q&gt;</sub>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-sup-tag") {
        auto const& pltext = u8"<sup>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<sup>x&lt;/q&gt;</sup>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-trigger-tag") {
        auto const& pltext = u8"<trigger=abc>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;trigger=abc&gt;x&lt;/q&gt;&lt;/trigger&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-u-tag") {
        auto const& pltext = u8"<u>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<u>x&lt;/q&gt;</u>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-user-tag") {
        auto const& pltext = u8"<user=642cf37a494746375aae306a>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span class='RUser' data-user='642cf37a494746375aae306a'>x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    TEST_CASE("mismatched-voffset-tag") {
        auto const& pltext = u8"<voffset=5>x</q>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:5px;\">x&lt;/q&gt;</span>";
        CHECK(html == answer);
    }

    // Inline constructs that carry no tag: the inline parser is what produces them.
    TEST_CASE("md-single-emphasis-asterisk") {
        auto const& pltext = u8"*x*";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("md-double-emphasis-asterisk") {
        auto const& pltext = u8"**x**";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("md-triple-emphasis-asterisk") {
        auto const& pltext = u8"***x***";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em><strong>x</strong></em>";
        CHECK(html == answer);
    }

    TEST_CASE("md-single-emphasis-underscore") {
        auto const& pltext = u8"_x_";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em>x</em>";
        CHECK(html == answer);
    }

    TEST_CASE("md-double-emphasis-underscore") {
        auto const& pltext = u8"__x__";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("md-triple-emphasis-underscore") {
        auto const& pltext = u8"___x___";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<em><strong>x</strong></em>";
        CHECK(html == answer);
    }

    TEST_CASE("md-del") {
        auto const& pltext = u8"~~x~~";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<del>x</del>";
        CHECK(html == answer);
    }

    TEST_CASE("md-code-span-one-backtick") {
        auto const& pltext = u8"`x`";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x</code>";
        CHECK(html == answer);
    }

    TEST_CASE("md-code-span-two-backticks") {
        auto const& pltext = u8"``x``";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x</code>";
        CHECK(html == answer);
    }

    TEST_CASE("md-code-span-three-backticks") {
        auto const& pltext = u8"```x```";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<code>x</code>";
        CHECK(html == answer);
    }

    TEST_CASE("md-latex-inline") {
        auto const& pltext = u8"$x$";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$x$";
        CHECK(html == answer);
    }

    TEST_CASE("md-latex-block") {
        auto const& pltext = u8"$$x$$";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$$x$$";
        CHECK(html == answer);
    }

    TEST_CASE("md-link") {
        auto const& pltext = u8"[t](https://example.com)";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">t</a>";
        CHECK(html == answer);
    }

    TEST_CASE("md-image") {
        auto const& pltext = u8"![t](https://example.com/a.png)";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/a.png\" alt=\"t\">";
        CHECK(html == answer);
    }

    TEST_CASE("bare-url-becomes-a-link") {
        auto const& pltext = u8"https://example.com";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>";
        CHECK(html == answer);
    }

    TEST_CASE("project-placeholder") {
        auto const& pltext = u8"{project}";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$PROJECT";
        CHECK(html == answer);
    }

    TEST_CASE("visitor-placeholder") {
        auto const& pltext = u8"{visitor}";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$VISITOR";
        CHECK(html == answer);
    }

    TEST_CASE("author-placeholder") {
        auto const& pltext = u8"{author}";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$AUTHOR";
        CHECK(html == answer);
    }

    TEST_CASE("coauthors-placeholder") {
        auto const& pltext = u8"{coauthors}";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"$CO_AUTHORS";
        CHECK(html == answer);
    }

    // Tags that carry no content of their own.
    TEST_CASE("br-tag") {
        auto const& pltext = u8"<br>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<br>";
        CHECK(html == answer);
    }

    TEST_CASE("self-closing-br-tag") {
        auto const& pltext = u8"<br/>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<br>";
        CHECK(html == answer);
    }

    TEST_CASE("img-tag") {
        auto const& pltext = u8"<img src=\"https://example.com/a.png\" alt=\"a\">";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/a.png\" alt=\"a\">";
        CHECK(html == answer);
    }

    TEST_CASE("note-comment-renders-nothing") {
        auto const& pltext = u8"<!-- note -->";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    // Frames that accept the legacy alternative closing tag of their pair.
    TEST_CASE("color-tag-closed-by-the-a-closing-tag") {
        auto const& pltext = u8"<color=red>x</a>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("a-tag-closed-by-the-color-closing-tag") {
        auto const& pltext = u8"<a>x</color>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:#0000AA;\">x</span>";
        CHECK(html == answer);
    }

    // A nested inline frame closes into its parent.
    TEST_CASE("nested-b-and-i-tags") {
        auto const& pltext = u8"<b><i>x</i></b>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<strong><em>x</em></strong>";
        CHECK(html == answer);
    }

    // Block-level tags are literal in the inline context of this entry point.
    TEST_CASE("block-p-tag-stays-literal") {
        auto const& pltext = u8"<p>x</p>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;p&gt;x&lt;/p&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h1-tag-stays-literal") {
        auto const& pltext = u8"<h1>x</h1>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h1&gt;x&lt;/h1&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h2-tag-stays-literal") {
        auto const& pltext = u8"<h2>x</h2>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h2&gt;x&lt;/h2&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h3-tag-stays-literal") {
        auto const& pltext = u8"<h3>x</h3>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h3&gt;x&lt;/h3&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h4-tag-stays-literal") {
        auto const& pltext = u8"<h4>x</h4>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h4&gt;x&lt;/h4&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h5-tag-stays-literal") {
        auto const& pltext = u8"<h5>x</h5>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h5&gt;x&lt;/h5&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-h6-tag-stays-literal") {
        auto const& pltext = u8"<h6>x</h6>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;h6&gt;x&lt;/h6&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-blockquote-tag-stays-literal") {
        auto const& pltext = u8"<blockquote>x</blockquote>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;blockquote&gt;x&lt;/blockquote&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-div-tag-stays-literal") {
        auto const& pltext = u8"<div style=\"margin-left:2em\">x</div>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;div&nbsp;style=&quot;margin-left:2em&quot;&gt;x&lt;/div&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-align-tag-stays-literal") {
        auto const& pltext = u8"<align=center>x</align>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;align=center&gt;x&lt;/align&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("block-margin-tag-stays-literal") {
        auto const& pltext = u8"<margin left=2em>x</margin>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;margin&nbsp;left=2em&gt;x&lt;/margin&gt;";
        CHECK(html == answer);
    }

    // A `<` that starts no known tag must survive as escaped text.
    TEST_CASE("stray-less-than") {
        auto const& pltext = u8"<";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;";
        CHECK(html == answer);
    }

    TEST_CASE("less-than-at-end-of-input") {
        auto const& pltext = u8"a<";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"a&lt;";
        CHECK(html == answer);
    }

    TEST_CASE("digit-after-less-than") {
        auto const& pltext = u8"<9";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;9";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-name-after-less-than") {
        auto const& pltext = u8"<z";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;z";
        CHECK(html == answer);
    }

    TEST_CASE("closing-tag-that-opened-nothing") {
        auto const& pltext = u8"</z";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;/z";
        CHECK(html == answer);
    }

    TEST_CASE("bang-that-starts-no-comment") {
        auto const& pltext = u8"<!x";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;!x";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-b-tag") {
        auto const& pltext = u8"<b";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;b";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-a-tag") {
        auto const& pltext = u8"<a ";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;a&nbsp;";
        CHECK(html == answer);
    }

    TEST_CASE("e-name-that-matches-no-tag") {
        auto const& pltext = u8"<ez";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;ez";
        CHECK(html == answer);
    }

    TEST_CASE("zero-size-tag") {
        auto const& pltext = u8"<size=0>x</size>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;size=0&gt;x&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("tag-without-a-value") {
        auto const& pltext = u8"<user=";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;user=";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-single-letter-tag") {
        auto const& pltext = u8"<v";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;v";
        CHECK(html == answer);
    }

    TEST_CASE("mark-without-a-value") {
        auto const& pltext = u8"<mark=";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;mark=";
        CHECK(html == answer);
    }

    TEST_CASE("size-without-a-value") {
        auto const& pltext = u8"<size=";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;size=";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-sup-tag") {
        auto const& pltext = u8"<sup";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;sup";
        CHECK(html == answer);
    }

    TEST_CASE("block-tag-in-inline-context") {
        auto const& pltext = u8"<div";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;div";
        CHECK(html == answer);
    }

    // A recognized opening tag whose URL is rejected consumes the span as literal text.
    TEST_CASE("javascript-url-in-a-tag") {
        auto const& pltext = u8"<a href=\"javascript:alert(1)\">x</a>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;a&nbsp;href=&quot;javascript:alert(1)&quot;&gt;x&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-url-in-external-tag") {
        auto const& pltext = u8"<external=javascript:alert(1)>x</external>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;external=javascript:alert(1)&gt;x&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-url-in-link-tag") {
        auto const& pltext = u8"<link=\"javascript:alert(1)\">x</link>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"&lt;link=&quot;javascript:alert(1)&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    // A code span needs non-empty, single-line content.
    TEST_CASE("double-backtick-without-content") {
        auto const& pltext = u8"``";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"``";
        CHECK(html == answer);
    }

    TEST_CASE("quadruple-backtick-without-content") {
        auto const& pltext = u8"````";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"````";
        CHECK(html == answer);
    }

    TEST_CASE("code-span-with-a-newline") {
        auto const& pltext = u8"`a\nb`";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"`a<br>b`";
        CHECK(html == answer);
    }

    // A bare URL inside a URL frame must not become a nested link.
    TEST_CASE("auto-link-suppressed-in-link-tag") {
        auto const& pltext = u8"<link=\"https://example.com\">https://example.com</link>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>";
        CHECK(html == answer);
    }

    TEST_CASE("auto-link-suppressed-in-a-tag") {
        auto const& pltext = u8"<a href=\"https://example.com\">https://example.com</a>";
        auto html = ::pltxt2htm_test::inline_pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>";
        CHECK(html == answer);
    }
}
