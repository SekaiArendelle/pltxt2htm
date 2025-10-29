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
    auto answer5 = ::fast_io::u8string_view{u8"<blockquote><h1>hello<h1></blockquote>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"> hello\n\n > text");
    auto answer6 = ::fast_io::u8string_view{u8"<blockquote>hello</blockquote><blockquote>text</blockquote>"};
    ::pltxt2htm_test::assert_true(html6 == answer6);
}
