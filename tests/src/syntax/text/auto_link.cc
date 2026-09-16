#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">https://example.com</a>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<external=https://example.com>https://example.com</external>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"visit https://example.com now"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"visit&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;now"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"visit\u00A0<external=https://example.com>https://example.com</external>\u00A0now"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/path?query=1&foo=bar"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<a "
            u8"href=\"https://example.com/path?query=1&amp;foo=bar\">https://example.com/path?query=1&amp;foo=bar</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/?a=1&amp;b=2"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com/?a=1&amp;b=2\">https://example.com/?a=1&amp;b=2</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/path#fragment"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com/path#fragment\">https://example.com/path#fragment</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com:8080/path"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<a href=\"https://example.com:8080/path\">https://example.com:8080/path</a>"};
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
        auto answer = ::fast_io::u8string_view{
            u8"prefix&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;suffix"};
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
    {
        // Chinese chars after the path are excluded from the auto-link
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/\u4E2D\u6587"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<a href=\"https://example.com/\">https://example.com/</a>\u4E2D\u6587"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<external=https://example.com/>https://example.com/</external>\u4E2D\u6587"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        // Space in the URL is treated as end of the auto-link
        auto pltext = ::fast_io::u8string_view{u8"https://example.com/a b"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/a\">https://example.com/a</a>&nbsp;b"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<external=https://example.com/a>https://example.com/a</external>\u00A0b"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://localhost"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://localhost"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://localhost:8080"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://localhost:8080"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://127.0.0.1"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://127.0.0.1"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://127.0.0.1:8080"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://127.0.0.1:8080"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://www.example.wtf"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://www.example.wtf"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"javascript:alert(1)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"javascript:alert(1)"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com:"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://example.com:"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com:80javascript:alert(1)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://example.com:80javascript:alert(1)"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example.com@evil.invalid/path"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<a href=\"https://example.com\">https://example.com</a>@evil.invalid/path"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://www.bilibili.com/video/BV1df421f7WB"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<a "
            u8"href=\"https://www.bilibili.com/video/BV1df421f7WB\">https://www.bilibili.com/video/BV1df421f7WB</a>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<external=https://www.bilibili.com/video/BV1df421f7WB>https://www.bilibili.com/video/BV1df421f7WB</"
            u8"external>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://www.example.com:65535"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<a href=\"https://www.example.com:65535\">https://www.example.com:65535</a>"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{
            u8"<external=https://www.example.com:65535>https://www.example.com:65535</external>"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://www.example.com:65536"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://www.example.com:65536"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://www.example.com:123456"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://www.example.com:123456"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://-example.com/a/path"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://-example.com/a/path"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto pltext = ::fast_io::u8string_view{u8"https://example-.com/a/path"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"https://example-.com/a/path"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // non-ASCII is not auto-linked: only the ASCII prefix becomes a link
        auto pltext = ::fast_io::u8string_view{u8"x https://x.com/中文 y"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"x&nbsp;<a href=\"https://x.com/\">https://x.com/</a>中文&nbsp;y"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    return 0;
}
