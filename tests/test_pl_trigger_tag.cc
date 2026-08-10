#include "precompile.hh"

int main() {
#ifdef PLTXT2HTM_ENABLE_TRIGGER_TAG
    // ---- plunity backend: <trigger=value> is output verbatim ----
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=run>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"<trigger=run>运行</trigger>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=English>点击这里</trigger>");
        auto answer = ::fast_io::u8string_view{u8"<trigger=English>点击这里</trigger>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Comma-separated value (legacy color suffix) is part of the value.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=run,FF0000>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"<trigger=run,FF0000>运行</trigger>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Nested inline formatting inside the trigger content is kept.
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=run><i>运行</i></trigger>");
        auto answer = ::fast_io::u8string_view{u8"<trigger=run><i>运行</i></trigger>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- web backend: <trigger=value> is escaped to literal &lt;trigger&gt; ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=run>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=English>点击这里</trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=English&gt;点击这里&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<TRIGGER=run>运行</TRIGGER>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Value characters are HTML-escaped to prevent injection.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=a<b>c</trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=a&lt;b&gt;c&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Nested inline formatting is rendered inside the escaped wrapper.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=run><i>运行</i></trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=run&gt;<em>运行</em>&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // fixedadv web backend escapes identically.
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<trigger=run>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- optimizer: empty trigger tag is omitted ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<trigger=run></trigger>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<trigger=run></trigger>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        pltxt2htm_test_assert_equal(html, answer);
    }
#else
    // ---- feature off: <trigger> is not a reserved tag, output as literal text ----

    // plunity backend: literal <trigger=value> is output with fullwidth angle brackets.
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<trigger=run>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"<size=20>\uff1c</size>trigger=run<size=20>\uff1e</size>运行"
                                               u8"<size=20>\uff1c</size>/trigger<size=20>\uff1e</size>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // web backend: <trigger=value> is escaped to literal &lt;trigger&gt;.
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<trigger=run>运行</trigger>");
        auto answer = ::fast_io::u8string_view{u8"&lt;trigger=run&gt;运行&lt;/trigger&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // Empty <trigger></trigger> is NOT omitted (no trigger node is produced).
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<trigger=run></trigger>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;trigger=run&gt;&lt;/trigger&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"t<trigger=run></trigger>t");
        auto answer = ::fast_io::u8string_view{u8"t<size=20>\uff1c</size>trigger=run<size=20>\uff1e</size>"
                                               u8"<size=20>\uff1c</size>/trigger<size=20>\uff1e</size>t"};
        pltxt2htm_test_assert_equal(html, answer);
    }
#endif

    // ---- malformed / non-matching input falls back to literal text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<trigger=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;trigger="};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // <table> (HTML) must still parse as a table, not a trigger.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<table><tr><td>x</td></tr></table>");
        pltxt2htm_test_assert_true(html == ::fast_io::u8string_view{u8"<table><tr><td>x</td></tr></table>"});
    }

    return 0;
}
