#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_voffset_tag") {
    TEST_CASE("<voffset=5>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-5>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5px>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5px>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5em>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-5em>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5em>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5em;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=0>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=0>hello</voffset>");
        auto const& answer = u8"&lt;voffset=0&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=00>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=00>hello</voffset>");
        auto const& answer = u8"&lt;voffset=00&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-0>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-0>hello</voffset>");
        auto const& answer = u8"&lt;voffset=-0&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5>hello</voffset> (9)") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("<voffset=-5>hello</voffset> (10)") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=-5>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("<voffset=5em>hello</voffset> (11)") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5em>hello</voffset>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    TEST_CASE("<Voffset=5>hello</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Voffset=5>hello</voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5 >hello</voffset >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5   >hello</voffset  >");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5>hello") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">hello</span>";
        CHECK(html == answer);
    }

    TEST_CASE("hello<voffset=5>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=5>");
        auto const& answer = u8"hello";
        CHECK(html == answer);
    }

    TEST_CASE("hello<voffset=3><voffset=5>world") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=3><voffset=5>world");
        auto const& answer = u8"hello<span style=\"vertical-align:5px;\">world</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=3>hello<voffset=5>world</voffset></voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=3>hello<voffset=5>world</voffset></voffset>");
        auto const& answer =
            u8"<span style=\"vertical-align:3px;\">hello<span style=\"vertical-align:5px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5>hello<voffset=5>world</voffset></voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("unitless defaults to px; so 5 and 5px merge") {
        // unitless defaults to px, so 5 and 5px merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5px>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("em and px are distinct units: no merge") {
        // em and px are distinct units: no merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello<voffset=5>world</voffset></voffset>");
        auto const& answer =
            u8"<span style=\"vertical-align:5em;\">hello<span style=\"vertical-align:5px;\">world</span></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-5>hello<voffset=-5>world</voffset></voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello<voffset=-5>world</voffset></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:-5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    TEST_CASE("t<voffset=5></voffset>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<voffset=5></voffset>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5></voffset") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5></voffset");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">&lt;/voffset</span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5><i>test</i></voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><i>test</i></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=>text");
        auto const& answer = u8"&lt;voffset=&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset= >text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset= >text");
        auto const& answer = u8"&lt;voffset=&nbsp;&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=em>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=em>text");
        auto const& answer = u8"&lt;voffset=em&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=px>text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=px>text");
        auto const& answer = u8"&lt;voffset=px&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("units are case-sensitive: 5EM is not a valid length") {
        // units are case-sensitive: 5EM is not a valid length
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5EM>text");
        auto const& answer = u8"&lt;voffset=5EM&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("trailing junk after a valid unit is rejected") {
        // trailing junk after a valid unit is rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5pxx>text");
        auto const& answer = u8"&lt;voffset=5pxx&gt;text";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=999999999999999999999999>hello</voffset>") {
        if constexpr (sizeof(::std::ptrdiff_t) <= 8) {
            auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=999999999999999999999999>hello</voffset>");
            auto const& answer = u8"&lt;voffset=999999999999999999999999&gt;hello&lt;/voffset&gt;";
            CHECK(html == answer);
        }
    }

    TEST_CASE("negative overflow rejected (min - 1)") {
        // negative overflow rejected (min - 1)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-999999999999999999999999>hello</voffset>");
        auto const& answer = u8"&lt;voffset=-999999999999999999999999&gt;hello&lt;/voffset&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5>text</voffset>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5>text</voffset>");
        auto const& answer = u8"<voffset=5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-5>text</voffset>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5>text</voffset>");
        auto const& answer = u8"<voffset=-5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5px>text</voffset>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5px>text</voffset>");
        auto const& answer = u8"<voffset=5px>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5em>text</voffset>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5em>text</voffset>");
        auto const& answer = u8"<voffset=5em>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=-5em>text</voffset>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5em>text</voffset>");
        auto const& answer = u8"<voffset=-5em>text</voffset>";
        CHECK(html == answer);
    }

    TEST_CASE("HTML roundtrip: <span style='vertical-align:Npx;'> maps...") {
        // HTML roundtrip: <span style="vertical-align:Npx;"> maps back to <voffset=N>
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"vertical-align:5px\">x</span>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\">x</span>";
        CHECK(html == answer);
    }

    TEST_CASE("tag content can contain HTML which is not interpreted") {
        // tag content can contain HTML which is not interpreted
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><b>test</b></voffset>");
        auto const& answer = u8"<span style=\"vertical-align:5px;\"><strong>test</strong></span>";
        CHECK(html == answer);
    }

    TEST_CASE("<voffset=5%>test</voffset>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5%>test</voffset>");
        auto const& answer = u8"&lt;voffset=5%&gt;test&lt;/voffset&gt;";
        CHECK(html == answer);
    }
}
