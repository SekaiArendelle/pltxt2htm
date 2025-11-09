#include "precompile.hh"

int main() {
    auto html1 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
    auto answer1 = ::fast_io::u8string_view{u8"<a href=\"url\">text</a>"};
    ::pltxt2htm_test::assert_true(html1 == answer1);

    auto html2 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[hello](https://example.com)");
    auto answer2 = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">hello</a>"};
    ::pltxt2htm_test::assert_true(html2 == answer2);

    auto html3 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[link](#anchor)");
    auto answer3 = ::fast_io::u8string_view{u8"<a href=\"#anchor\">link</a>"};
    ::pltxt2htm_test::assert_true(html3 == answer3);

    auto html4 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text with spaces](https://example.com/path)");
    auto answer4 = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path\">text&nbsp;with&nbsp;spaces</a>"};
    ::pltxt2htm_test::assert_true(html4 == answer4);

    auto html5 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[escaped \\[brackets\\]](url)");
    auto answer5 = ::fast_io::u8string_view{u8"<a href=\"url\">escaped&nbsp;[brackets]</a>"};
    ::pltxt2htm_test::assert_true(html5 == answer5);

    auto html6 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[nested [link]](url)");
    auto answer6 = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](url)"};
    ::pltxt2htm_test::assert_true(html6 == answer6);

    auto html7 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[t**ex**t](example.com)");
    auto answer7 = ::fast_io::u8string_view{u8"<a href=\"example.com\">t<strong>ex</strong>t</a>"};
    ::pltxt2htm_test::assert_true(html7 == answer7);

    auto html10 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
    auto answer10 = ::fast_io::u8string_view{u8"<a href=\"url\">text</a>"};
    ::pltxt2htm_test::assert_true(html10 == answer10);

    auto html11 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text");
    auto answer11 = ::fast_io::u8string_view{u8"[text"};
    ::pltxt2htm_test::assert_true(html11 == answer11);

    auto html12 = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url");
    auto answer12 = ::fast_io::u8string_view{u8"[text](url"};
    ::pltxt2htm_test::assert_true(html12 == answer12);

    return 0;
}
