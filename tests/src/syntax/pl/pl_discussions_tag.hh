#pragma once

#include "doctest_config.hh"

TEST_CASE("pl_discussions_tag") {
    // web backend escapes the list-entry tag (per feature request, escaping is acceptable)
    {
        auto const& pltext =
            u8"<discussions=UserID/123/UserName/\u5C0F\u660E/Sort/Popularity>\u66F4\u591A\u8BA8\u8BBA"
            u8"</discussions>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"&lt;discussions=UserID/123/UserName/\u5C0F\u660E/Sort/Popularity&gt;\u66F4\u591A\u8BA8\u8BBA"
            u8"&lt;/discussions&gt;";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<discussions=UserID/123/UserName/\u5C0F\u660E/Sort/Popularity>\u66F4\u591A\u8BA8\u8BBA"
            u8"</discussions>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<discussions=UserID/abc/UserName/\u5C0F\u660E>\u66F4\u591A"
            u8"\u8BA8\u8BBA</discussions>");
        auto const& answer =
            u8"&lt;discussions=UserID/abc/UserName/\u5C0F\u660E&gt;\u66F4\u591A\u8BA8\u8BBA"
            u8"&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // comma-separated multi-values are allowed
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussions=Tags/a,b,c>list</discussions>");
        auto const& answer = u8"&lt;discussions=Tags/a,b,c&gt;list&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // case-insensitive tag name
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<DiScUsSiOnS=abc      >list</DISCUSSIONS      >");
        auto const& answer = u8"&lt;discussions=abc&gt;list&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // empty content is removed by the optimizer
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<discussions=abc></discussions>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    // unmatched closing tag is tolerated
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussions=abc>list");
        auto const& answer = u8"&lt;discussions=abc&gt;list&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // value characters that could enable XSS are escaped on output
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussions=<>>test</discussions>");
        auto const& answer = u8"&lt;discussions=&lt;&gt;&gt;test&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // non-nestable mutual exclusion with singular tags: inner equal-sign tag is literal
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussions=a>t<discussion=b>ex</discussion>t</discussions>");
        auto const& answer = u8"&lt;discussions=a&gt;t&lt;discussion=b&gt;ex&lt;/discussion&gt;t&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<discussions=a>t<experiment=b>ex</experiment>t</discussions>");
        auto const& answer = u8"&lt;discussions=a&gt;t&lt;experiment=b&gt;ex&lt;/experiment&gt;t&lt;/discussions&gt;";
        CHECK(html == answer);
    }

    // plunity backend keeps the list-entry tag verbatim (see first block)
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<discussions=abc>list</discussions>");
        auto const& answer = u8"<discussions=abc>list</discussions>";
        CHECK(html == answer);
    }
}
