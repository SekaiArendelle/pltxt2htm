#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=right>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:right\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justify>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:justify\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=justified>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:justify\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Align=center>hello</align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center   >hello</align  >");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // mid-line <align> sequences are literal text, so nothing collapses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<align=center></align>t");
        auto answer = ::fast_io::u8string_view{u8"t&lt;align=center&gt;&lt;/align&gt;t"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello<align=center>");
        auto answer = ::fast_io::u8string_view{u8"hello&lt;align=center&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=left>hello");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:left\">hello</p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // inner inline <align=right> is literal text; only the outer block frame parses
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center>hello<align=right>world</align></align>");
        auto answer = ::fast_io::u8string_view{
            u8"<p style=\"text-align:center\">hello&lt;align=right&gt;world</p>&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=center><i>test</i></align>");
        auto answer = ::fast_io::u8string_view{u8"<p style=\"text-align:center\"><em>test</em></p>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=centerr>text");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=centerr&gt;text"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align=bogus>text</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align=bogus&gt;text&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<align>text</align>");
        auto answer = ::fast_io::u8string_view{u8"&lt;align&gt;text&lt;/align&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity backend emits TMP align tags
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=center>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=center>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=right>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=right>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justify>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=justified>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=justified>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=justified>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // left is emitted verbatim for an explicit align tag
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<align=left>text</align>");
        auto answer = ::fast_io::u8string_view{u8"<align=left>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // plunity block: a line break forces block context, preserving <align>
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"before\n<align=right>text</align>");
        auto answer = ::fast_io::u8string_view{u8"before\n<align=right>text</align>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}