#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"1. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        ::pltxt2htm_test::assert_true(html == answer);
    }
    // {
    //     auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test");
    //     auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul>");
    //     ::pltxt2htm_test::assert_true(html == answer);
    // }
    // {
    //     auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - text");
    //     auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>text</li></ul></ul>");
    //     ::pltxt2htm_test::assert_true(html == answer);
    // }
    // {
    //     auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - text\n     - test");
    //     auto answer = ::fast_io::u8string_view(
    //         u8"<ul><li>test</li><li>test</li><ul><li>text</li><ul><li>test</li></ul></ul></ul>");
    //     ::pltxt2htm_test::assert_true(html == answer);
    // }
    // {
    //     auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"- test\n - test\n   - t**ex**t");
    //     auto answer =
    //         ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>t<strong>ex</strong>t</li></ul></ul>");
    //     ::pltxt2htm_test::assert_true(html == answer);
    // }

    return 0;
}
