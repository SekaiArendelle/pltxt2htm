#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_voffset_tag") {
    TEST_CASE("basic-5px") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("negative-value") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("px-unit") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5px>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-unit") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("negative-em") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5em>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("zero-value") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=0>hello</voffset>");
        auto const& answer = u8"&lt;voffset=0&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("leading-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=00>hello</voffset>");
        auto const& answer = u8"&lt;voffset=00&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("negative-zero-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-0>hello</voffset>");
        auto const& answer = u8"&lt;voffset=-0&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-basic") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("roundtrip-negative") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=-5>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("roundtrip-em") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5em>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("case-insensitive-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Voffset=5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("extra-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5   >hello</voffset  >");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=5>");
        auto const& answer = u8"hello";
        CHECK(html == answer);
    }

    TEST_CASE("two-open-tags-inner-wins") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=3><voffset=5>world");
        auto const& answer = u8"hello<span style=\"vertical-align:5px;\">world</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-values") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=3>hello<voffset=5>world</voffset></voffset>");
        auto const& answer =
            u8"<span style=\"vertical-align:3px;\">hello<span style=\"vertical-align:5px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-value-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unitless-merges-with-px") {
        // unitless defaults to px, so 5 and 5px merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5px>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em-and-px-distinct") {
        // em and px are distinct units: no merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello<voffset=5>world</voffset></voffset>");
        auto const& answer =
            u8"<span style=\"vertical-align:5em;\">hello<span style=\"vertical-align:5px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-negative-flattened") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello<voffset=-5>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<voffset=5></voffset>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5></voffset");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">&lt;/voffset</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-italic") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><i>test</i></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=>text");
        auto const& answer = u8"&lt;voffset=&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("space-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset= >text");
        auto const& answer = u8"&lt;voffset=&nbsp;&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("unit-only-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=em>text");
        auto const& answer = u8"&lt;voffset=em&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("unit-only-px-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=px>text");
        auto const& answer = u8"&lt;voffset=px&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-unit-rejected") {
        // units are case-sensitive: 5EM is not a valid length
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5EM>text");
        auto const& answer = u8"&lt;voffset=5EM&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("trailing-junk-rejected") {
        // trailing junk after a valid unit is rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5pxx>text");
        auto const& answer = u8"&lt;voffset=5pxx&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("overflow-rejected") {
        if constexpr (sizeof(::std::ptrdiff_t) <= 8) {
            auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=999999999999999999999999>hello</voffset>");
            auto const& answer = u8"&lt;voffset=999999999999999999999999&gt;hello&lt;/voffset&gt;";
            CHECK(html == answer);
        }
    }

    TEST_CASE("negative-overflow-rejected") {
        // negative overflow rejected (min - 1)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-999999999999999999999999>hello</voffset>");
        auto const& answer = u8"&lt;voffset=-999999999999999999999999&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-px-suffix") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5>text</voffset>");
        auto const& answer = u8"<voffset=5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-negative-px") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5>text</voffset>");
        auto const& answer = u8"<voffset=-5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-explicit-px") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5px>text</voffset>");
        auto const& answer = u8"<voffset=5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-em") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5em>text</voffset>");
        auto const& answer = u8"<voffset=5em>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-negative-em") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5em>text</voffset>");
        auto const& answer = u8"<voffset=-5em>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("html-span-roundtrip") {
        // HTML roundtrip: <span style="vertical-align:Npx;"> maps back to <voffset=N>
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"vertical-align:5px\">x</span>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-bold") {
        // tag content can contain HTML which is not interpreted
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><b>test</b></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\"><strong>test</strong></span>";
        CHECK(html == answer);
    }

    TEST_CASE("percent-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5%>test</voffset>");
        auto const& answer = u8"&lt;voffset=5%&gt;test&lt;/voffset&gt;";
        CHECK(html == answer);
    }
}
