#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=1>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:1px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=0&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=00&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=1>hello</size>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=00>hello</size>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:6px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=11   >hello</siZE  >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:6px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11>hello");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:6px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=11>");
        auto answer = ::fast_io::u8string_view{u8"hello"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<size=9><size=11>world");
        auto answer = ::fast_io::u8string_view{u8"hello<span style=\"font-size:6px;\">world</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=9>hello<size=11>world</size></size>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"font-size:5px;\">hello<span style=\"font-size:6px;\">world</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10>hello<size=10>world</size></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px;\">helloworld</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<size=10></size>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10></size");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px;\">&lt;/size</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=10><i>test</i></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:5px;\"><em>test</em></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size= >text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=&nbsp;&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    if constexpr (sizeof(::std::size_t) <= 8) {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=999999999999999999999999>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=999999999999999999999999&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=12>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=80%>hello</siZE>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=100%>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:100%;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0%>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=0%&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00%>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=00%&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80%>world</size></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\">helloworld</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=80>world</size></size>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:40px;\">world</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%><i>test</i></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\"><em>test</em></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=80%>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=80%>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"font-size:80%\">x</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:80%;\">x</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=80%>hello</size>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<SIZE=2em   >hello</siZE>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2em>hello");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0em>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=0em&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=00em>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=00em&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // uppercase em unit rejected (lowercase "em" only)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2EM>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=2EM&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // partial em rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2e>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=2e&gt;hello&lt;/size&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=80%>hello<size=2em>world</size></size>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"font-size:80%;\">hello<span style=\"font-size:2em;\">world</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2em>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=2em>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<size=2em>hello</size>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:7px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=11.5>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:6px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.25>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:7px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=12.5>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=12.5>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5%>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2.5%;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=2.5em>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:2.5em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<size=2.5em>text</size>");
        auto answer = ::fast_io::u8string_view{u8"<size=2.5em>text</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=0.5>hello</size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:1px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5><size=12.5>world</size></size>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"font-size:7px;\">world</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=12.&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=.5>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=.5&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<size=12.5.6>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;size=12.5.6&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
