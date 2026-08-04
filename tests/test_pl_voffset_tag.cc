#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:-5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5px>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5em>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:-5em;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=0>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=0&gt;hello&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=00>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=00&gt;hello&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-0>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=-0&gt;hello&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5>hello</voffset>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=-5>hello</voffset>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<voffset=5em>hello</voffset>");
        auto reparsed_html = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{html.data(), html.size()});
        pltxt2htm_test_assert_equal(reparsed_html, html);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Voffset=5>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5   >hello</voffset  >");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">hello</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=5>");
        auto answer = ::fast_io::u8string_view{u8"hello"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<voffset=3><voffset=5>world");
        auto answer = ::fast_io::u8string_view{u8"hello<span style=\"vertical-align:5px;\">world</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=3>hello<voffset=5>world</voffset></voffset>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"vertical-align:3px;\">hello<span style=\"vertical-align:5px;\">world</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5>world</voffset></voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">helloworld</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unitless defaults to px, so 5 and 5px merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5>hello<voffset=5px>world</voffset></voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">helloworld</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // em and px are distinct units: no merge
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5em>hello<voffset=5>world</voffset></voffset>");
        auto answer = ::fast_io::u8string_view{
            u8"<span style=\"vertical-align:5em;\">hello<span style=\"vertical-align:5px;\">world</span></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-5>hello<voffset=-5>world</voffset></voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:-5px;\">helloworld</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<voffset=5></voffset>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5></voffset");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">&lt;/voffset</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><i>test</i></voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\"><em>test</em></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset= >text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=&nbsp;&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=em>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=em&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=px>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=px&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // units are case-sensitive: 5EM is not a valid length
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5EM>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=5EM&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // trailing junk after a valid unit is rejected
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5pxx>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=5pxx&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    if constexpr (sizeof(::std::ptrdiff_t) <= 8) {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=999999999999999999999999>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=999999999999999999999999&gt;hello&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // negative overflow rejected (min - 1)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=-999999999999999999999999>hello</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=-999999999999999999999999&gt;hello&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5>text</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<voffset=5>text</voffset>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5>text</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<voffset=-5>text</voffset>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5px>text</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<voffset=5>text</voffset>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=5em>text</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<voffset=5em>text</voffset>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<voffset=-5em>text</voffset>");
        auto answer = ::fast_io::u8string_view{u8"<voffset=-5em>text</voffset>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // HTML roundtrip: <span style="vertical-align:Npx;"> maps back to <voffset=N>
        auto html = ::pltxt2htm_test::pltxt4htmlunittest(u8"<span style=\"vertical-align:5px\">x</span>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\">x</span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // tag content can contain HTML which is not interpreted
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5><b>test</b></voffset>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"vertical-align:5px;\"><strong>test</strong></span>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<voffset=5%>test</voffset>");
        auto answer = ::fast_io::u8string_view{u8"&lt;voffset=5%&gt;test&lt;/voffset&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
