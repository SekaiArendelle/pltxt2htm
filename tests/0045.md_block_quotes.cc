#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text<br>test</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test\n");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text<br>test</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test\ntest\n");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>text<br>test</blockquote>test<br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> # hello");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><h1>hello</h1></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> hello\n\n > text");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>hello</blockquote><br><blockquote>text</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> # hello\n > # test\n > # text");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><h1>hello</h1><h1>test</h1><h1>text</h1></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n> text");
        auto answer = ::fast_io::u8string_view{u8"<br><blockquote>text</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    // empty block quotes is undocumented in spec.commonmark.org, this is an implement defined behavior
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8">");
        auto answer = ::fast_io::u8string_view{u8"&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" > ");
        auto answer = ::fast_io::u8string_view{u8"&nbsp;&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" >> test");
        auto answer = ::fast_io::u8string_view{u8"<blockquote><blockquote>test</blockquote></blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" \t> \ttest");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>test</blockquote>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8" > test\ntest");
        auto answer = ::fast_io::u8string_view{u8"<blockquote>test</blockquote>test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
