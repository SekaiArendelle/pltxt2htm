#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"Hello");
        auto answer = ::fast_io::u8string_view{u8"Hello"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"café");
        auto answer = ::fast_io::u8string_view{u8"café"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"中文");
        auto answer = ::fast_io::u8string_view{u8"中文"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"😊😊");
        auto answer = ::fast_io::u8string_view{u8"😊😊"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc3");
        auto answer = ::fast_io::u8string_view{u8"\xef\xbf\xbd"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc3\xa");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xe0\x80\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xc0\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xe2\x82");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf4\x90\x80\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }
    {
        // invalid 4-byte sequence: 3rd byte is not a continuation byte (0x28)
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf0\x90\x28\x80");
        auto answer = ::fast_io::u8string_view{u8"�(�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xed\xa0\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\x9f");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"A\x80"
            u8"B");
        auto answer = ::fast_io::u8string_view{u8"A�B"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"A\x1f"
            u8"B");
        auto answer = ::fast_io::u8string_view{u8"AB"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xe2");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xe2"
            u8"A");
        auto answer = ::fast_io::u8string_view{u8"�A"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xe2\x82"
            u8"A");
        auto answer = ::fast_io::u8string_view{u8"�A"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf0");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xf0"
            u8"A");
        auto answer = ::fast_io::u8string_view{u8"�A"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf0\x90");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xf0\x90"
            u8"A");
        auto answer = ::fast_io::u8string_view{u8"�A"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\xf0\x90\x80");
        auto answer = ::fast_io::u8string_view{u8"�"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xf0"
            u8"A\x80\x80");
        auto answer = ::fast_io::u8string_view{u8"�A��"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(
            u8"\xf0\x90\x80"
            u8"A");
        auto answer = ::fast_io::u8string_view{u8"�A"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
