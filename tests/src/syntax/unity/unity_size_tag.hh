#pragma once

#include "doctest_config.hh"

TEST_CASE("unity_size_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello</size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=1>hello</size>");
        auto const& answer = u8"<span style=\"font-size:1px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0>hello</size>");
        auto const& answer = u8"&lt;size=0&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00>hello</size>");
        auto const& answer = u8"&lt;size=00&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=1>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=00>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello</size>");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=11   >hello</siZE  >");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=11>");
        auto const& answer = u8"hello";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=9><size=11>world");
        auto const& answer = u8"hello<span style=\"font-size:6px;\">world</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=9>hello<size=11>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">hello<span style=\"font-size:6px;\">world</span></span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello<size=10>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\">helloworld</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<size=10></size>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10></size");
        auto const& answer = u8"<span style=\"font-size:5px;\">&lt;/size</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10><i>test</i></size>");
        auto const& answer = u8"<span style=\"font-size:5px;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=>text");
        auto const& answer = u8"&lt;size=&gt;text";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size= >text");
        auto const& answer = u8"&lt;size=&nbsp;&gt;text";
        CHECK(html == answer);
    }

    if constexpr (sizeof(::std::size_t) <= 8) {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=999999999999999999999999>hello</size>");
        auto const& answer = u8"&lt;size=999999999999999999999999&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12>text</size>");
        auto const& answer = u8"<size=12>text</size>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=80%>hello</siZE>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=100%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:100%;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0%>hello</size>");
        auto const& answer = u8"&lt;size=0%&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00%>hello</size>");
        auto const& answer = u8"&lt;size=00%&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80%>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">helloworld</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80>world</size></size>");
        auto const& answer =
            u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:40px;\">world</span></span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%><i>test</i></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\"><em>test</em></span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=80%>text</size>");
        auto const& answer = u8"<size=80%>text</size>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"font-size:80%\">x</span>");
        auto const& answer = u8"<span style=\"font-size:80%;\">x</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=80%>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=2em   >hello</siZE>");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello");
        auto const& answer = u8"<span style=\"font-size:2em;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0em>hello</size>");
        auto const& answer = u8"&lt;size=0em&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00em>hello</size>");
        auto const& answer = u8"&lt;size=00em&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        // uppercase em unit rejected (lowercase "em" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2EM>hello</size>");
        auto const& answer = u8"&lt;size=2EM&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        // partial em rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2e>hello</size>");
        auto const& answer = u8"&lt;size=2e&gt;hello&lt;/size&gt;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=2em>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:2em;\">world</span></span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2em>text</size>");
        auto const& answer = u8"<size=2em>text</size>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=2em>hello</size>");
        auto reparsed_html =
            ::pltxt2htm_test::pltxt4htmlunittest(::pltxt2htm::container::U8StringView{html.data(), html.size()});
        CHECK(reparsed_html == html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:6px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.25>hello</size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12.5>text</size>");
        auto const& answer = u8"<size=12.5>text</size>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5%>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2.5%;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5em>hello</size>");
        auto const& answer = u8"<span style=\"font-size:2.5em;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2.5em>text</size>");
        auto const& answer = u8"<size=2.5em>text</size>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0.5>hello</size>");
        auto const& answer = u8"<span style=\"font-size:1px;\">hello</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0.8em>text</size>");
        auto const& answer = u8"<span style=\"font-size:0.8em;\">text</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5><size=12.5>world</size></size>");
        auto const& answer = u8"<span style=\"font-size:7px;\">world</span>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.>text");
        auto const& answer = u8"&lt;size=12.&gt;text";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=.5>text");
        auto const& answer = u8"&lt;size=.5&gt;text";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5.6>text");
        auto const& answer = u8"&lt;size=12.5.6&gt;text";
        CHECK(html == answer);
    }
}
