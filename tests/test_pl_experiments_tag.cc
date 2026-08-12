#include "precompile.hh"

int main() {
    // web backend escapes the list-entry tag (per feature request, escaping is acceptable)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<experiments=UserID/123/UserName/\u5C0F\u660E/Tags/\u7CBE\u9009>\u66F4\u591A\u5B9E\u9A8C</experiments>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;experiments=UserID/123/UserName/\u5C0F\u660E/Tags/\u7CBE\u9009&gt;\u66F4\u591A\u5B9E\u9A8C"
            u8"&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiments=UserID/abc/UserName/\u5C0F\u660E>\u66F4\u591A"
                                                     u8"\u5B9E\u9A8C</experiments>");
        auto answer =
            ::fast_io::u8string_view{u8"&lt;experiments=UserID/abc/UserName/\u5C0F\u660E&gt;\u66F4\u591A\u5B9E\u9A8C"
                                     u8"&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // comma-separated multi-values are allowed
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiments=Tags/a,b,c>list</experiments>");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiments=Tags/a,b,c&gt;list&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // case-insensitive tag name
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ExpErImEnTs=abc      >list</EXPERIMENTS      >");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiments=abc&gt;list&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // empty content is removed by the optimizer
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<experiments=abc></experiments>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // unmatched closing tag is tolerated
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiments=abc>list");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiments=abc&gt;list&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // value characters that could enable XSS are escaped on output
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<experiments=<>>test</experiments>");
        auto answer = ::fast_io::u8string_view{u8"&lt;experiments=&lt;&gt;&gt;test&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // non-nestable mutual exclusion with singular tags: inner equal-sign tag is literal
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<experiments=a>t<experiment=b>ex</experiment>t</experiments>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;experiments=a&gt;t&lt;experiment=b&gt;ex&lt;/experiment&gt;t&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<experiments=a>t<discussion=b>ex</discussion>t</experiments>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;experiments=a&gt;t&lt;discussion=b&gt;ex&lt;/discussion&gt;t&lt;/experiments&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // plunity backend keeps the list-entry tag verbatim
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            u8"<experiments=UserID/123/UserName/\u5C0F\u660E>\u66F4\u591A\u5B9E\u9A8C</experiments>");
        auto answer = ::fast_io::u8string_view{
            u8"<experiments=UserID/123/UserName/\u5C0F\u660E>\u66F4\u591A\u5B9E\u9A8C</experiments>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<experiments=abc>list</experiments>");
        auto answer = ::fast_io::u8string_view{u8"<experiments=abc>list</experiments>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
