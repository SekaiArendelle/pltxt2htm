#pragma once

// Windows headers may define min/max as function-like macros, which would break
// std::numeric_limits<...>::max() in the assertions below.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <pltxt2htm/details/format/integer.hh>

#include "doctest_config.hh"
#include "format/details/support.hh"

namespace pltxt2htm_test::format_integer_details {

template<::std::size_t N>
[[nodiscard]]
constexpr auto buffer_matches(char8_t const* begin, char8_t const* end, char8_t const (&expected)[N]) noexcept -> bool {
    if (static_cast<::std::size_t>(end - begin) != N - 1) {
        return false;
    }
    for (::std::size_t i{}; i + 1 < N; ++i) {
        if (begin[i] != expected[i]) {
            return false;
        }
    }
    return true;
}

[[nodiscard]]
consteval auto test_decimal_digit_pairs() noexcept -> bool {
    for (::std::size_t i{}; i < 100; ++i) {
        if (::pltxt2htm::details::decimal_digit_pairs[i * 2] != static_cast<char8_t>(u8'0' + i / 10) ||
            ::pltxt2htm::details::decimal_digit_pairs[i * 2 + 1] != static_cast<char8_t>(u8'0' + i % 10)) {
            return false;
        }
    }
    return true;
}

[[nodiscard]]
consteval auto test_write_decimal_digits_backward() noexcept -> bool {
    constexpr ::std::size_t buffer_size = ::std::numeric_limits<::std::uintmax_t>::digits10 + 2;
    char8_t buffer[buffer_size]{};
    char8_t* const end = buffer + buffer_size;

    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{0}, end), end, u8"0")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{7}, end), end, u8"7")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{10}, end), end, u8"10")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{42}, end), end, u8"42")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{99}, end), end, u8"99")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{100}, end), end,
                        u8"100")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{101}, end), end,
                        u8"101")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{999}, end), end,
                        u8"999")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{1000}, end), end,
                        u8"1000")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{12345}, end), end,
                        u8"12345")) {
        return false;
    }
    if constexpr (::std::numeric_limits<::std::uintmax_t>::digits10 == 19) {
        if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(
                                ::std::numeric_limits<::std::uintmax_t>::max(), end),
                            end, u8"18446744073709551615")) {
            return false;
        }
    }
    return true;
}

} // namespace pltxt2htm_test::format_integer_details

static_assert(::pltxt2htm::details::decimal_digit_pairs.size() == 200);
static_assert(::pltxt2htm_test::format_integer_details::test_decimal_digit_pairs());
static_assert(::pltxt2htm_test::format_integer_details::test_write_decimal_digits_backward());

TEST_SUITE("format_integer") {
    TEST_CASE("unsigned-boundaries") {
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{0}) == u8"0");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{1}) == u8"1");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{9}) == u8"9");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{10}) == u8"10");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{11}) == u8"11");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{42}) == u8"42");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{99}) == u8"99");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{100}) == u8"100");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{101}) == u8"101");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{110}) == u8"110");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{999}) == u8"999");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{1000}) == u8"1000");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{12345}) == u8"12345");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::uint64_t{1} << 32) == u8"4294967296");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::numeric_limits<::std::uint64_t>::max()) ==
              u8"18446744073709551615");
    }

    TEST_CASE("signed-boundaries") {
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{0}) == u8"0");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{1}) == u8"1");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-1}) == u8"-1");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{9}) == u8"9");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-9}) == u8"-9");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{10}) == u8"10");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-10}) == u8"-10");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{99}) == u8"99");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-99}) == u8"-99");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{100}) == u8"100");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-100}) == u8"-100");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{101}) == u8"101");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-101}) == u8"-101");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{12345}) == u8"12345");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::ptrdiff_t{-12345}) == u8"-12345");
        CHECK(::pltxt2htm::details::to_decimal_string(::std::numeric_limits<::std::ptrdiff_t>::max()) ==
              ::pltxt2htm_test::format_details::oracle_int2str(::std::numeric_limits<::std::ptrdiff_t>::max()));
        CHECK(::pltxt2htm::details::to_decimal_string(::std::numeric_limits<::std::ptrdiff_t>::min()) ==
              ::pltxt2htm_test::format_details::oracle_int2str(::std::numeric_limits<::std::ptrdiff_t>::min()));
    }

    TEST_CASE("exhaustive-oracle") {
        for (::std::uint64_t i{}; i <= 100000; ++i) {
            CHECK(::pltxt2htm::details::to_decimal_string(i) == ::pltxt2htm_test::format_details::oracle_int2str(i));
        }
        for (::std::ptrdiff_t i{-100000}; i <= 100000; ++i) {
            CHECK(::pltxt2htm::details::to_decimal_string(i) == ::pltxt2htm_test::format_details::oracle_int2str(i));
        }
    }

    TEST_CASE("to-chars") {
        char8_t buffer[24];
        auto* end = ::pltxt2htm::details::to_chars(buffer, buffer + sizeof(buffer), ::std::uint64_t{12345});
        REQUIRE(end != nullptr);
        CHECK(::pltxt2htm_test::format_integer_details::buffer_matches(buffer, end, u8"12345"));

        end = ::pltxt2htm::details::to_chars(buffer, buffer + sizeof(buffer), ::std::ptrdiff_t{-42});
        REQUIRE(end != nullptr);
        CHECK(::pltxt2htm_test::format_integer_details::buffer_matches(buffer, end, u8"-42"));

        end = ::pltxt2htm::details::to_chars(buffer, buffer + sizeof(buffer),
                                             ::std::numeric_limits<::std::int64_t>::min());
        REQUIRE(end != nullptr);
        CHECK(::pltxt2htm::details::to_chars(buffer, buffer + sizeof(buffer),
                                             ::std::numeric_limits<::std::int64_t>::min()) != nullptr);

        char8_t short_buffer[3];
        CHECK(::pltxt2htm::details::to_chars(short_buffer, short_buffer + sizeof(short_buffer),
                                             ::std::uint64_t{12345}) == nullptr);
        CHECK(::pltxt2htm::details::to_chars(short_buffer, short_buffer + sizeof(short_buffer), ::std::uint64_t{0}) !=
              nullptr);
    }

    TEST_CASE("append-decimal") {
        ::pltxt2htm::container::U8String out{};
        ::pltxt2htm::details::append_decimal(out, ::std::uint64_t{12});
        ::pltxt2htm::details::append_decimal(out, ::std::ptrdiff_t{-3});
        ::pltxt2htm::details::append_decimal(out, ::std::uint64_t{0});
        CHECK(out == u8"12-30");
    }
}

#pragma pop_macro("max")
#pragma pop_macro("min")