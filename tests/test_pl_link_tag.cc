#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com/discussion\">physicslab</link>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/discussion\">physicslab</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<lInK=\"https://another-example.org/path\">physicslab</LINK>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://another-example.org/path\">physicslab</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<LINK="https://site.com/item"      >te
 xt</link      >
)");
        auto answer = ::fast_io::u8string_view{u8"<br><a href=\"https://site.com/item\">te<br>&nbsp;xt</a><br>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Link=\"https://main.com\"><link=\"https://nested.com\">physicslab</link></Link>");
        // The rejected nested tag becomes literal text; auto-link is suppressed because the
        // URL is inside a URL-link frame (pl_link), matching pl_external's behavior.
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\">&lt;link=&quot;https://nested.com&quot;&gt;physicslab</a>&lt;/Link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // A bare URL in a link tag's text is not auto-linked (no nested <a> inside <a>).
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://a.com\">visit https://b.com here</link>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://a.com\">visit&nbsp;https://b.com&nbsp;here</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Same suppression applies inside pl_external.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://a.com>see https://b.com</external>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://a.com\">see&nbsp;https://b.com</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Control: outside any URL-link frame auto-link still works.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"x https://a.com y");
        auto answer = ::fast_io::u8string_view{u8"x&nbsp;<a href=\"https://a.com\">https://a.com</a>&nbsp;y"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"ab<Link=\"https://first.com\">te<link=\"https://second.com\">physicslab</link>st</Link>cd");
        auto answer = ::fast_io::u8string_view{
            u8"ab<a href=\"https://first.com\">te&lt;link=&quot;https://second.com&quot;&gt;physicslab</a>st&lt;/"
            u8"Link&gt;cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<lInK=\"https://example.com\">");
        auto answer = ::fast_io::u8string_view{u8"test"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te<lInK=\"https://example.com\"></link>st");
        auto answer = ::fast_io::u8string_view{u8"test"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<link=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;link="};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // unquoted value is not a Unity TextMeshPro link tag, so it stays plain text
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=https://example.com>text</link>");
        auto answer = ::fast_io::u8string_view{u8"&lt;link=https://example.com&gt;text&lt;/link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://example.com\">text<link=\"https://another-example.com\">text</link></link>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com\">text&lt;link=&quot;https://another-example.com&quot;&gt;text</a>&lt;/"
            u8"link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://example.com\">physics<link=\"https://another-site.org\">L</link>ab</link>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com\">physics&lt;link=&quot;https://another-site.org&quot;&gt;L</a>ab&lt;/"
            u8"link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com\">physicslab");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">physicslab</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<link=\"https://example.com\"></link>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"\">t");
        auto answer = ::fast_io::u8string_view{u8"&lt;link=&quot;&quot;&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com\"></link");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">&lt;/link</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://main.com\"><i><link=\"https://nested.com\">c</link></i></link>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\"><em>&lt;link=&quot;https://nested.com&quot;&gt;c&lt;/link&gt;</em></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert(1)\">x</link>");
        auto answer = ::fast_io::u8string_view{u8"&lt;link=&quot;javascript:alert(1)&quot;&gt;x&lt;/link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // At the root there is no URL-link outer frame, so the URL inside this rejected
        // tag is still auto-linked.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://a.com\"onclick=\"alert(1)\">x</link>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;link=&quot;<a href=\"https://a.com\">https://a.com</a>&quot;onclick=&quot;alert(1)&quot;&gt;x&lt;/"
            u8"link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://main.com\"><i><experiment=a>c</experiment></i></link>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\"><em>&lt;experiment=a&gt;c&lt;/experiment&gt;</em></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://main.com\"><i><discussion=a>c</discussion></i></link>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\"><em>&lt;discussion=a&gt;c&lt;/discussion&gt;</em></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Same as above: rejected at the root, so the URL is auto-linked.
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://main.com\" onmouseover=\"alert('XSS')\">content</link>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;link=&quot;<a "
            u8"href=\"https://main.com\">https://main.com</a>&quot;&nbsp;onmouseover=&quot;alert(&apos;"
            u8"XSS&apos;)&quot;&gt;content&lt;/link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert('XSS')\">clickme</link>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;link=&quot;javascript:alert(&apos;XSS&apos;)&quot;&gt;clickme&lt;/link&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"example.com\">text</link>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<link=\"https://example.com\">example</link>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<link=\"https://example.com\">example</link>cd"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // non-ASCII (CJK) in the link path is accepted and percent-encoded
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://x.com/中文\">x</link>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://x.com/%E4%B8%AD%E6%96%87\">x</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}