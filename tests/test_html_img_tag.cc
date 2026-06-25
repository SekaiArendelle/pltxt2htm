#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img alt=\"description\" src=\"https://example.com/image.png\">"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\"/>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"alt&amp;text\">"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"alt&amp;text\">"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"前<img src=\"a.jpg\" alt=\"中间\">后"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"前<img src=\"a.jpg\" alt=\"中间\">后"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;img&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img src=\"a.jpg\">"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"&lt;img&nbsp;src=&quot;a.jpg&quot;&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\">"});
        auto answer = ::fast_io::u8string_view{u8""};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"description\" class=\"cls\"/>"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"&lt;img&nbsp;src=&quot;<a href=\"https://example.com/image.png\">https://example.com/image.png</a>&quot;&nbsp;alt=&quot;description&quot;&nbsp;class=&quot;cls&quot;/&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
