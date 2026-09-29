#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_size_tag") {
    TEST_CASE("size-ten-becomes-5px") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello</size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("size-one-becomes-1px") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=1>hello</size>");
        auto const& answer = u8"<span style=\"font-size:1px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0>hello</size>");
        auto const& answer = u8"&lt;size=0&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("leading-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00>hello</size>");
        auto const& answer = u8"&lt;size=00&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-size-one") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=1>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("roundtrip-leading-zero") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=00>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("size-eleven-becomes-6px") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello</size>");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("case-insensitive-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=11   >hello</siZE  >");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=11>");
        auto const& answer = u8"hello";
        CHECK(html == answer);
    }

    TEST_CASE("two-open-tags-inner-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=9><size=11>world");
        auto const& answer = u8"hello<span style=\"font-size:6px;\">world</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-size") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=9>hello<size=11>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">hello<span style=\"font-size:6px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-size-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello<size=10>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<size=10></size>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10></size");
        auto const& answer = u8"<span style=\"font-size:5px;\">&lt;/size</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10><i>test</i></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=>text");
        auto const& answer = u8"&lt;size=&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("space-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size= >text");
        auto const& answer = u8"&lt;size=&nbsp;&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("overflow-rejected") {
        if constexpr (sizeof(::std::size_t) <= 8) {
            auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=999999999999999999999999>hello</size>");
            auto const& answer = u8"&lt;size=999999999999999999999999&gt;hello&lt;/size&gt;";
            CHECK(html == answer);
        }
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12>text</size>");
        auto const& answer = u8"<size=12>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-unit") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-case-insensitive") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=80%>hello</siZE>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-hundred") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=100%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:100%;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-unclosed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0%>hello</size>");
        auto const& answer = u8"&lt;size=0%&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("percent-leading-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00%>hello</size>");
        auto const& answer = u8"&lt;size=00%&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("percent-nested-same-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80%>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-nested-different-kept") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80>world</size></size>");
        auto const& answer =
            u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:40px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%><i>test</i></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-percent") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=80%>text</size>");
        auto const& answer = u8"<size=80%>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("html-span-missing-semicolon") {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"font-size:80%\">x</span>");
        auto const& answer = u8"<span style=\"font-size:80%;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-percent") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=80%>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("em-unit") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-case-insensitive") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=2em   >hello</siZE>");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-unclosed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0em>hello</size>");
        auto const& answer = u8"&lt;size=0em&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em-leading-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00em>hello</size>");
        auto const& answer = u8"&lt;size=00em&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em-uppercase-rejected") {
        // uppercase em unit rejected (lowercase "em" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2EM>hello</size>");
        auto const& answer = u8"&lt;size=2EM&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("em-partial-rejected") {
        // partial em rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2e>hello</size>");
        auto const& answer = u8"&lt;size=2e&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-percent-em-nested") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=2em>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:2em;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-em") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2em>text</size>");
        auto const& answer = u8"<size=2em>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-em") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=2em>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("fractional-size") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-rounding-down") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-multi-digit") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.25>hello</size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-fractional") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12.5>text</size>");
        auto const& answer = u8"<size=12.5>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-percent") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2.5%;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-em") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5em>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2.5em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-fractional-em") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2.5em>text</size>");
        auto const& answer = u8"<size=2.5em>text</size>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-below-one") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:1px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-em-below-one") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0.8em>text</size>");
        auto const& answer = u8"<span style=\"font-size:0.8em;\">text</span>";
        CHECK(html == answer);
    }

    TEST_CASE("fractional-nested-same-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5><size=12.5>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">world</span>";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-dot-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.>text");
        auto const& answer = u8"&lt;size=12.&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("leading-dot-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=.5>text");
        auto const& answer = u8"&lt;size=.5&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("two-dots-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5.6>text");
        auto const& answer = u8"&lt;size=12.5.6&gt;text";
        CHECK(html == answer);
    }
}
