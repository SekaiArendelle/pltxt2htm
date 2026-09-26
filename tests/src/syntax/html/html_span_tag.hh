#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_span_tag") {
    TEST_CASE("<span style=/'color:red/'>text</span>") {
        auto const& pltext = u8"<span style=\"color:red\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=red>text</color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/'font-size:20px/'>text</span>") {
        auto const& pltext = u8"<span style=\"font-size:20px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=40>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/'color:blue;font-size:16px/'>text</span>") {
        auto const& pltext = u8"<span style=\"color:blue;font-size:16px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:blue;font-size:16px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=blue><size=32>text</size></color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/' ; color : red ; ; font-size : 20px ; /'>t...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\" ; color \t : \t red \t ; ; font-size : 20px ; \">text</span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase STYLE attribute is rejected (only lowercase 's...") {
        // uppercase STYLE attribute is rejected (only lowercase "style" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span STYLE=\"color:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;STYLE=&quot;color:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase COLOR property is rejected (only lowercase 'co...") {
        // uppercase COLOR property is rejected (only lowercase "color" is allowed)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"COLOR:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;COLOR:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/' >text</span >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span  style=\"color:red\"  >text</span  >");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red !important/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red !important\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red&nbsp;!important&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:#FF0000/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#FF0000\">text</span>");
        auto const& answer = u8"<span style=\"color:#FF0000;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:20/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20\">text</span>");
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red;/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;\">text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span class=/'foo/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span class=\"foo\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;class=&quot;foo&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'background:red/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"background:red\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;background:red&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:#GGG/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#GGG\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:#GGG&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:1em/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1em\">text</span>");
        auto const& answer = u8"<span style=\"font-size:1em;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red;color:blue/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red;color:blue\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red;color:blue&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:20px;font-size:30px/'>text</span...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20px;font-size:30px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20px;font-size:30px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:80%/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:80%\">text</span>");
        auto const& answer = u8"<span style=\"font-size:80%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:blue;font-size:80%/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:blue;font-size:80%\">text</span>");
        auto const& answer = u8"<span style=\"color:blue;font-size:80%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:80%;font-size:30px/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:80%;font-size:30px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:80%;font-size:30px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("t<span style=/'color:red/'></span>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<span style=\"color:red\"></span>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'><span style=/'color:red/'>text...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'>a<span style=/'color:red/'>b</...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:red\">b</span>c</span>");
        auto const& answer = u8"<span style=\"color:red;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'><span style=/'color:blue/'>tex...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:blue\">text</span></span>");
        auto const& answer = u8"<span style=\"color:blue;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'>a<span style=/'color:blue/'>b<...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">a<span style=\"color:blue\">b</span></span>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"color:blue;\">b</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<strong><span style=/'color:red/'>text</span></strong>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><span style=\"color:red\">text</span></strong>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text</span></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("<a><span style=/'color:red/'>text</span></a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a><span style=\"color:red\">text</span></a>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=green><span style=/'color:red/'>text</span></colo...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=green><span style=\"color:red\">text</span></color>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<color=red>a<span style=/'color:red/'>b</span>c</color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"color:red\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:#0000AA;/'>a<a>b</a>c</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:#0000AA;\">a<a>b</a>c</span>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<a>a<span style=/'color:#0000AA;/'>b</span>c</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">abc</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<a>a<span style=/'color:red;/'>b</span>c</a>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:red;\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">a<span style=\"color:red;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'><a>text</a></span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><a>text</a></span>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'><color=blue>text</color></span...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><color=blue>text</color></span>");
        auto const& answer = u8"<span style=\"color:blue;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("font-size matched nesting: should flatten") {
        // font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"font-size:20px\">text</span></span>");
        auto const& answer = u8"<span style=\"font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color+font-size matched nesting: should flatten") {
        // color+font-size matched nesting: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:20px\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color+font-size; font-size different: should NOT flatten...") {
        // color+font-size, font-size different: should NOT flatten, inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px\"><span style=\"color:red;font-size:16px\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:16px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color=red wrapping font-size-only span with siblings: sp...") {
        // color=red wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<color=red>a<span style=\"font-size:20px\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color=red wrapping span with matching color AND font-siz...") {
        // color=red wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<color=red>a<span style=\"color:red;font-size:20px\">b</span>c</color>");
        auto const& answer = u8"<span style=\"color:red;\">a<span style=\"color:red;font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("anchor wrapping font-size-only span with siblings: span...") {
        // anchor wrapping font-size-only span with siblings: span preserved (no color to match)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"font-size:20px\">b</span>c</a>");
        auto const& answer = u8"<span style=\"color:#0000AA;\">a<span style=\"font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("non-overlapping nested attrs: merge outer font-size with...") {
        // non-overlapping nested attrs: merge outer font-size with inner color
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:20px\"><span style=\"color:red\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("triple nested same color: should flatten to single span") {
        // triple nested same color: should flatten to single span
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"color:red\"><span "
            u8"style=\"color:red\">text</span></span></span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty same-color nested span: should remove both (nothin...") {
        // empty same-color nested span: should remove both (nothing left)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"><span style=\"color:red\"></span></span>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("anchor wrapping span with matching color AND font-size...") {
        // anchor wrapping span with matching color AND font-size: should NOT flatten
        // (flattening would lose the font-size)
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;font-size:20px\">b</span>c</a>");
        auto const& answer =
            u8"<span style=\"color:#0000AA;\">a<span style=\"color:#0000AA;font-size:20px;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\">text");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:red/'") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:red\"");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;color:red&quot;";
        CHECK(html == answer);
    }

    TEST_CASE("overflow font-size: span tag rejected; treated as litera...") {
        // overflow font-size: span tag rejected, treated as literal text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:99999999999999999999px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:99999999999999999999px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("single-quoted style attribute") {
        // single-quoted style attribute
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style='color:red'>text</span>");
        auto const& answer = u8"<span style=\"color:red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("font-size:0 rejected (zero is not a valid font-size)") {
        // font-size:0 rejected (zero is not a valid font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:0\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:0&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("empty style value rejected") {
        // empty style value rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase px unit rejected (lowercase 'px' only)") {
        // uppercase px unit rejected (lowercase "px" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20PX\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20PX&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:20p/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20p\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20p&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'font-size:20pxx/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20pxx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20pxx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase color name rejected (lowercase only)") {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:Red\">text</span>");
        auto const& answer = u8"<span style=\"color:Red;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase color name rejected (lowercase only) (55)") {
        // uppercase color name rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"color:BLUE\">text</span>");
        auto const& answer = u8"<span style=\"color:BLUE;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("negative font-size rejected") {
        // negative font-size rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:-20px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:-20px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("font-size that cannot be represented in double/std::size...") {
        // font-size that cannot be represented in double/std::size_t stays literal
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:18446744073709551614px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:18446744073709551614px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("fractional font-size accepted and round-tripped") {
        // fractional font-size accepted and round-tripped
        auto const& pltext = u8"<span style=\"font-size:12.5px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:12.5px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=25>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("fractional font-size with em unit") {
        // fractional font-size with em unit
        auto const& pltext = u8"<span style=\"font-size:1.5em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"font-size:1.5em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<size=1.5em>text</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("fractional font-size with percent unit") {
        // fractional font-size with percent unit
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:87.5%\">text</span>");
        auto const& answer = u8"<span style=\"font-size:87.5%;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("leading-dot font-size rejected (stays literal)") {
        // leading-dot font-size rejected (stays literal)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:.5px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:.5px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("font-size with empty fractional part rejected (stays lit...") {
        // font-size with empty fractional part rejected (stays literal)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:12.px\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:12.px&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:super/'>text</span>") {
        auto const& pltext = u8"<span style=\"vertical-align:super\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:super;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/'vertical-align:sub/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:sub\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:sub;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:baseline/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:baseline\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:baseline;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:text-top/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:text-top\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:text-top;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:text-bottom/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:text-bottom\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:text-bottom;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:middle/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:middle\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:middle;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:top/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:top\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:top;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:bottom/'>text</span>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:bottom\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:bottom;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:5px/'>text</span>") {
        auto const& pltext = u8"<span style=\"vertical-align:5px\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:5px;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<voffset=5>text</voffset>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/'vertical-align:20%/'>text</span>") {
        auto const& pltext = u8"<span style=\"vertical-align:20%\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:20%;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("<span style=/'color:red;font-size:20px;vertical-align:su...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red;font-size:20px;vertical-align:super\">text</span>");
        auto const& answer = u8"<span style=\"color:red;font-size:20px;vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'vertical-align:10em/'>text</span>") {
        auto const& pltext = u8"<span style=\"vertical-align:10em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"vertical-align:10em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"text";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase vertical-align property rejected (lowercase on...") {
        // uppercase vertical-align property rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"VERTICAL-ALIGN:super\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;VERTICAL-ALIGN:super&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase keyword rejected (lowercase only)") {
        // uppercase keyword rejected (lowercase only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:SUPER\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:SUPER&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("keyword with trailing alphanumeric rejected ('superx' is...") {
        // keyword with trailing alphanumeric rejected ("superx" is not a keyword)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:superx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:superx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unknown keyword rejected") {
        // unknown keyword rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:foo\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:foo&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("zero length rejected") {
        // zero length rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:0\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;vertical-align:0&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("negative length supported (vertical-align: -5px)") {
        // negative length supported (vertical-align: -5px)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:-5px\">text</span>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate vertical-align rejected") {
        // duplicate vertical-align rejected
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"vertical-align:super;vertical-align:sub\">text</span>");
        auto const& answer =
            u8"&lt;span&nbsp;style=&quot;vertical-align:super;vertical-align:sub&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("matched nested vertical-align: should flatten") {
        // matched nested vertical-align: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:super\"><span style=\"vertical-align:super\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("differing nested vertical-align: inner wins") {
        // differing nested vertical-align: inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:super\"><span style=\"vertical-align:sub\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:sub;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("differing nested vertical-align lengths: inner wins") {
        // differing nested vertical-align lengths: inner wins
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"vertical-align:5px\"><span style=\"vertical-align:10px\">text</span></span>");
        auto const& answer = u8"<span style=\"vertical-align:10px;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("non-overlapping nested attrs: merge outer color with inn...") {
        // non-overlapping nested attrs: merge outer color with inner vertical-align
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"color:red\"><span style=\"vertical-align:super\">text</span></span>");
        auto const& answer = u8"<span style=\"color:red;vertical-align:super;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("span with matching color AND vertical-align inside <colo...") {
        // span with matching color AND vertical-align inside <color=red> should NOT flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<color=red>a<span style=\"color:red;vertical-align:super\">b</span>c</color>");
        auto const& answer =
            u8"<span style=\"color:red;\">a<span style=\"color:red;vertical-align:super;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("span with matching color AND vertical-align inside <a> s...") {
        // span with matching color AND vertical-align inside <a> should NOT flatten
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a>a<span style=\"color:#0000AA;vertical-align:super\">b</span>c</a>");
        auto const& answer =
            u8"<span style=\"color:#0000AA;\">a<span style=\"color:#0000AA;vertical-align:super;\">b</span>c</span>";
        CHECK(html == answer);
    }

    TEST_CASE("color+font-size+px vertical-align combined in Unity outp...") {
        // color+font-size+px vertical-align combined in Unity output
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<span style=\"color:red;font-size:16px;vertical-align:5px\">text</span>");
        auto const& answer = u8"<color=red><size=32><voffset=5>text</voffset></size></color>";
        CHECK(html == answer);
    }

    TEST_CASE("em font-size is supported") {
        // em font-size is supported
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20em\">text</span>");
        auto const& answer = u8"<span style=\"font-size:20em;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<span style=/'color:blue;font-size:16em/'>text</span>") {
        auto const& pltext = u8"<span style=\"color:blue;font-size:16em\">text</span>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<span style=\"color:blue;font-size:16em;\">text</span>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<color=blue><size=16em>text</size></color>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("uppercase em unit rejected (lowercase 'em' only)") {
        // uppercase em unit rejected (lowercase "em" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:20EM\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:20EM&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("partial em rejected") {
        // partial em rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1e\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1e&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em with trailing character rejected") {
        // em with trailing character rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1emx\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1emx&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("duplicate 'm' rejected") {
        // duplicate 'm' rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:1emm\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:1emm&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("font-size:0em rejected (zero is not a valid font-size)") {
        // font-size:0em rejected (zero is not a valid font-size)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<span style=\"font-size:0em\">text</span>");
        auto const& answer = u8"&lt;span&nbsp;style=&quot;font-size:0em&quot;&gt;text&lt;/span&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em font-size maps to <size=Nem> in Unity output") {
        // em font-size maps to <size=Nem> in Unity output
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<span style=\"font-size:16em\">text</span>");
        auto const& answer = u8"<size=16em>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("em font-size roundtrip through the HTML parser") {
        // em font-size roundtrip through the HTML parser
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<span style=\"font-size:16em\">text</span>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("em vertical-align roundtrip through the HTML parser") {
        // em vertical-align roundtrip through the HTML parser
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<span style=\"vertical-align:10em\">text</span>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("matched nested em font-size: should flatten") {
        // matched nested em font-size: should flatten
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<span style=\"font-size:1em\"><span style=\"font-size:1em\">text</span></span>");
        auto const& answer = u8"<span style=\"font-size:1em;\">text</span>";
        CHECK(html == answer);
    }
}
