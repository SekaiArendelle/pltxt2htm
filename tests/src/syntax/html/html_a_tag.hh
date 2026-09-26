#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_a_tag") {
    TEST_CASE("<a href=/'https://www.example.com/'>click here</a>") {
        auto const& pltext = u8"<a href=\"https://www.example.com\">click here</a>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://www.example.com\">click&nbsp;here</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://www.example.com>click\u00A0here</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<a href='https://www.example.com'>click here</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href='https://www.example.com'>click here</a>");
        auto const& answer = u8"<a href=\"https://www.example.com\">click&nbsp;here</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://www.example.com/'>a<b>b</b>c</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\">a<b>b</b>c</a>");
        auto const& answer = u8"<a href=\"https://www.example.com\">a<strong>b</strong>c</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://www.example.com/'><color=red>text</col...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\"><color=red>text</color></a>");
        auto const& answer = u8"<a href=\"https://www.example.com\"><span style=\"color:red;\">text</span></a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://www.example.com/'>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\">text");
        auto const& answer = u8"<a href=\"https://www.example.com\">text</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://www.example.com/'>click here</a> (6)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://www.example.com\">click here</a>");
        auto const& answer = u8"<a href=\"https://www.example.com\">click&nbsp;here</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://www.example.com/' style=/'color:red;/'...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\" style=\"color:red;\"></a>");
        auto const& answer =
            u8"&lt;a&nbsp;href=&quot;<a "
            u8"href=\"https://www.example.com\">https://www.example.com</"
            u8"a>&quot;&nbsp;style=&quot;color:red;&quot;&gt;&lt;/a&gt;";
        CHECK(html == answer);
    }

    // illegal URLs (not http/https) are rejected — the tag is output as literal escaped text
    TEST_CASE("illegal URLs (not http/https) are rejected — the tag is...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"javascript:alert(1)\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;javascript:alert(1)&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'data:text/html;<script>alert(1)</script>/'>cli...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"data:text/html,<script>alert(1)</script>\">click</a>");
        auto const& answer =
            u8"&lt;a&nbsp;href=&quot;data:text/html,&lt;script&gt;alert(1)&lt;/script&gt;&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'vbscript:msgbox(1)/'>click</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"vbscript:msgbox(1)\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;vbscript:msgbox(1)&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'https://invalid.host/'>click</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://invalid.host\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://invalid.host&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("The opening tag is recognized but its URL fails validati...") {
        // The opening tag is recognized but its URL fails validation, so the whole span
        // becomes literal text (no auto-link inside).
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://example.com@evil.invalid/path\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://example.com@evil.invalid/path&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    // scheme-less URLs are accepted (validated by try_parse_url)
    TEST_CASE("scheme-less URLs are accepted (validated by try_parse_ur...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"example.com\">text</a>");
        auto const& answer = u8"<a href=\"example.com\">text</a>";
        CHECK(html == answer);
    }

    TEST_CASE("<a href=/'sub.example.com/path/'>text</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"sub.example.com/path\">text</a>");
        auto const& answer = u8"<a href=\"sub.example.com/path\">text</a>";
        CHECK(html == answer);
    }

    // internal attribute
    TEST_CASE("internal attribute") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\" internal>text</a>");
        auto const& answer = u8"<a href=\"https://www.example.com\" internal>text</a>";
        CHECK(html == answer);
    }

    // percent-encoding: ' in auto-link URL → %27
    TEST_CASE("percent-encoding: ' in auto-link URL → %27") {
        auto const& pltext = u8"https://example.com/path'with'quote";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<a "
            u8"href=\"https://example.com/path%27with%27quote\">https://example.com/path%27with%27quote</a>";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com/path'with'quote") {
        auto const& pltext = u8"https://example.com/path'with'quote";
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto const& answer =
            u8"<a "
            u8"href=\"https://example.com/path%27with%27quote\">https://example.com/path%27with%27quote</a>";
        CHECK(html == answer);
    }

    // percent-encoding: ' in markdown link URL → %27
    TEST_CASE("percent-encoding: ' in markdown link URL → %27") {
        auto const& pltext = u8"[text](https://example.com/pa'th)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/pa%27th\">text</a>";
        CHECK(html == answer);
    }

    // percent-encoding: ' in HTML <a> tag href → %27
    TEST_CASE("percent-encoding: ' in HTML <a> tag href → %27") {
        auto const& pltext = u8"<a href=\"https://example.com/pa'th\">text</a>";
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/pa%27th\">text</a>";
        CHECK(html == answer);
    }

    // roundtrip idempotency for ' in auto-link URL
    TEST_CASE("roundtrip idempotency for ' in auto-link URL") {
        auto pass1 = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"https://example.com/path'with'quote");
        auto pass2 = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::mnp::os_c_str(pass1));
        CHECK(pass2 == pass1);
    }

    // --- rejected <a href="..."> tags (recognized structure, invalid URL) become one literal span ---

    TEST_CASE("--- rejected <a href='...'> tags (recognized structure...") {
        // a '>' inside the quoted value: the span ends at the real closing '>' found
        // by the parser, not the first '>'
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://x.com/a>b\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://x.com/a&gt;b&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("single-quoted href value") {
        // single-quoted href value
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href='javascript:x'>click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&apos;javascript:x&apos;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("a newline inside the rejected span becomes a line break...") {
        // a newline inside the rejected span becomes a line break; no block re-scan
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://x\n.com\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://x<br>.com&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("non-ASCII (CJK) in the path is accepted and percent-enco...") {
        // non-ASCII (CJK) in the path is accepted and percent-encoded
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://x.com/中文\">click</a>");
        auto const& answer = u8"<a href=\"https://x.com/%E4%B8%AD%E6%96%87\">click</a>";
        CHECK(html == answer);
    }

    TEST_CASE("non-ASCII in the authority (domain) is still rejected; t...") {
        // non-ASCII in the authority (domain) is still rejected; the span stays literal
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://中文.com/path\">click</a>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://中文.com/path&quot;&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("a valid tag right after the rejected span still parses n...") {
        // a valid tag right after the rejected span still parses normally
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://example.com@evil.invalid/path\"><i>ok</i>");
        auto const& answer = u8"&lt;a&nbsp;href=&quot;https://example.com@evil.invalid/path&quot;&gt;<em>ok</em>";
        CHECK(html == answer);
    }

    TEST_CASE("the boolean 'internal' attribute is part of the span whe...") {
        // the boolean "internal" attribute is part of the span when the URL is invalid
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://example.com@evil.invalid/path\" internal>click</a>");
        auto const& answer =
            u8"&lt;a&nbsp;href=&quot;https://example.com@evil.invalid/path&quot;&nbsp;internal&gt;click&lt;/a&gt;";
        CHECK(html == answer);
    }
}
