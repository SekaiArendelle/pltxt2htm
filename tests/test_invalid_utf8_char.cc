#include <fast_io/fast_io_dsal/array.h>
#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"Hello");
        auto answer = ::fast_io::u8string_view{u8"Hello"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"café");
        auto answer = ::fast_io::u8string_view{u8"café"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"中文");
        auto answer = ::fast_io::u8string_view{u8"中文"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"😊😊");
        auto answer = ::fast_io::u8string_view{u8"😊😊"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xc3)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"\uFFFD"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xc3), char8_t(0xa)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xe0), char8_t(0x80), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xc0), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xe2), char8_t(0x82)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf4), char8_t(0x90), char8_t(0x80), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // invalid 4-byte sequence: 3rd byte is not a continuation byte (0x28)
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t(0x90), char8_t('('), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�(�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xed), char8_t(0xa0), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0x9f)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t('A'), char8_t(0x80), char8_t('B')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"A�B"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Control chars now produce InvalidU8Char (U+FFFD) instead of being silently dropped
        constexpr auto data = ::fast_io::array{char8_t('A'), char8_t(0x1f), char8_t('B')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"A\uFFFDB"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xe2)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xe2), char8_t('A')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xe2), char8_t(0x82), char8_t('A')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t('A')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t(0x90)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t(0x90), char8_t('A')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t(0x90), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t('A'), char8_t(0x80), char8_t(0x80)};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A��"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        constexpr auto data = ::fast_io::array{char8_t(0xf0), char8_t(0x90), char8_t(0x80), char8_t('A')};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(::fast_io::u8string_view(data.data(), data.size()));
        auto answer = ::fast_io::u8string_view{u8"�A"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
