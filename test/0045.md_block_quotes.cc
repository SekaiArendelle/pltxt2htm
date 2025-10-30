#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text");
    auto answer1 = ::fast_io::u8string_view{u8"<blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test");
    auto answer2 = ::fast_io::u8string_view{u8"<blockquote>text<br>test</blockquote>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test\n");
    auto answer3 = ::fast_io::u8string_view{u8"<blockquote>text<br>test<br></blockquote>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> text\n > test\ntest\n");
    auto answer4 = ::fast_io::u8string_view{u8"<blockquote>text<br>test<br></blockquote>test<br>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> # hello");
    auto answer5 = ::fast_io::u8string_view{u8"<blockquote><h1>hello</h1></blockquote>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> hello\n\n > text");
    auto answer6 = ::fast_io::u8string_view{u8"<blockquote>hello<br></blockquote><br><blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> # hello\n > # test\n > # text");
    auto answer7 = ::fast_io::u8string_view{u8"<blockquote><h1>hello</h1><h1>test</h1><h1>text</h1></blockquote>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html8 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"\n> text");
    auto answer8 = ::fast_io::u8string_view{u8"<br><blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html8 == answer8);

    // empty block quotes is undocumented in spec.commonmark.org, this is an implement defined behavior
    auto html9 = ::pltxt2htm_test::pltxt2advanced_htmld(u8">");
    auto answer9 = ::fast_io::u8string_view{u8"&gt;"};
    ::pltxt2htm_test::assert_true(html9 == answer9);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8" > ");
    auto answer10 = ::fast_io::u8string_view{u8"&nbsp;&gt;&nbsp;"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    return 0;
}
