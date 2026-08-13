#include "precompile.hh"

int main() {
    // ---- plunity backend: <internal=value> is output verbatim ----
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=run>运行</internal>");
        auto answer = ::fast_io::u8string_view{u8"<internal=run>运行</internal>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=English>点击这里</internal>");
        auto answer = ::fast_io::u8string_view{u8"<internal=English>点击这里</internal>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Comma-separated value (legacy color suffix) is part of the value.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=run,FF0000>运行</internal>");
        auto answer = ::fast_io::u8string_view{u8"<internal=run,FF0000>运行</internal>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Nested inline formatting inside the internal content is kept.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<internal=run><i>运行</i></internal>");
        auto answer = ::fast_io::u8string_view{u8"<internal=run><i>运行</i></internal>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- web backend: <internal=value> is escaped to literal &lt;internal&gt; ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=run>运行</internal>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=run&gt;运行&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=English>点击这里</internal>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=English&gt;点击这里&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<INTERNAL=run>运行</INTERNAL>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=run&gt;运行&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Value characters are HTML-escaped to prevent injection.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=a<b>c</internal>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=a&lt;b&gt;c&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Nested inline formatting is rendered inside the escaped wrapper.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<internal=run><i>运行</i></internal>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=run&gt;<em>运行</em>&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // fixedadv web backend escapes identically.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<internal=run>运行</internal>");
        auto answer = ::fast_io::u8string_view{u8"&lt;internal=run&gt;运行&lt;/internal&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- optimizer: empty internal tag is omitted ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<internal=run></internal>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<internal=run></internal>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- malformed / non-matching input falls back to literal text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<internal=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;internal="};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
