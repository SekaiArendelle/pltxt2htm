#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">https://example.com</a>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=https://example.com>https://example.com</external>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"visit https://example.com now"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"visit&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;now"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"visit <external=https://example.com>https://example.com</external> now"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/path?query=1&foo=bar"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path?query=1&amp;foo=bar\">https://example.com/path?query=1&amp;foo=bar</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/path#fragment"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path#fragment\">https://example.com/path#fragment</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com:8080/path"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com:8080/path\">https://example.com:8080/path</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"http://example.com"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"http://example.com\">http://example.com</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"prefix https://example.com suffix"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"prefix&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;suffix"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"not-a-url"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"not-a-url"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"`https://example.com`"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<code>https://example.com</code>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"[click](https://example.com)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">click</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    return 0;
}
