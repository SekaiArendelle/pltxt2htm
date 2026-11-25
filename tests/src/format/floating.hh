#pragma once

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

#include <pltxt2htm/format/floating.hh>
#include <pltxt2htm/details/parser/decimal.hh>
#include <pltxt2htm/details/utils.hh>

#include "doctest_config.hh"
#include "format/details/support.hh"

namespace pltxt2htm_test::format_floating_details {

[[nodiscard]] inline auto parse(::pltxt2htm::container::U8String const& text)
    -> ::pltxt2htm::container::Optional<::pltxt2htm::details::TryParseDoubleDecimalValueResult> {
    return ::pltxt2htm::details::try_parse_double_decimal_value(
        ::pltxt2htm::container::U8StringView{text.data(), text.size()});
}

/// Format, parse back, and format again: checks round-trip and idempotence in one place.
inline void check_round_trip(double const value) {
    auto const text = ::pltxt2htm::format::to_decimal_string(value);
    REQUIRE(text.has_value());
    auto const& fixed = text.value();
    auto const parsed = ::pltxt2htm_test::format_floating_details::parse(fixed);
    REQUIRE(parsed.has_value());
    CHECK(parsed.value().value == value);

    auto const again = ::pltxt2htm::format::to_decimal_string(parsed.value().value);
    REQUIRE(again.has_value());
    CHECK(again.value() == fixed);
}

} // namespace pltxt2htm_test::format_floating_details

TEST_SUITE("format_floating") {
    TEST_CASE("fixed-point-values") {
        CHECK(::pltxt2htm::format::to_decimal_string(0.0).value() == u8"0");
        CHECK(::pltxt2htm::format::to_decimal_string(12.0).value() == u8"12");
        CHECK(::pltxt2htm::format::to_decimal_string(12.5).value() == u8"12.5");
        CHECK(::pltxt2htm::format::to_decimal_string(0.1).value() == u8"0.1");
        CHECK(::pltxt2htm::format::to_decimal_string(1.0 / 3.0).value() == u8"0.3333333333333333");
        CHECK(::pltxt2htm::format::to_decimal_string(1e-18).value() == u8"0.000000000000000001");
        CHECK(::pltxt2htm::format::to_decimal_string(175.002439047).value() == u8"175.002439047");
        CHECK(::pltxt2htm::format::to_decimal_string(214748364.75).value() == u8"214748364.75");
        CHECK(::pltxt2htm::format::to_decimal_string(123456789.25).value() == u8"123456789.25");
        CHECK(::pltxt2htm::format::to_decimal_string(0.000024198805311133413).value() == u8"0.000024198805311133413");
        // Integral values keep their exact integer digits.
        CHECK(::pltxt2htm::format::to_decimal_string(7173733806442602179.0).value() == u8"7173733806442602496");
        CHECK(::pltxt2htm::format::to_decimal_string(9223372036854775808.0).value() == u8"9223372036854775808");
        CHECK(::pltxt2htm::format::to_decimal_string(18446744073709549568.0).value() == u8"18446744073709549568");
    }

    TEST_CASE("domain") {
        using ::pltxt2htm::format::details::can_format_fixed_point;
        using ::pltxt2htm::format::details::max_fixed_point_value;
        using ::pltxt2htm::format::details::min_fixed_point_value;

        CHECK_FALSE(::pltxt2htm::format::to_decimal_string(max_fixed_point_value).has_value());
        CHECK_FALSE(::pltxt2htm::format::to_decimal_string(1e-19).has_value());
        CHECK_FALSE(::pltxt2htm::format::to_decimal_string(-1.0).has_value());
        CHECK_FALSE(::pltxt2htm::format::to_decimal_string(::std::numeric_limits<double>::quiet_NaN()).has_value());
        CHECK(::pltxt2htm::format::to_decimal_string(-0.0).value() == u8"0");
        CHECK(::pltxt2htm::format::to_decimal_string(min_fixed_point_value).value() == u8"0.000000000000000001");

        CHECK(can_format_fixed_point(0.0));
        CHECK(can_format_fixed_point(min_fixed_point_value));
        CHECK_FALSE(can_format_fixed_point(1e-19));
        CHECK_FALSE(can_format_fixed_point(max_fixed_point_value));
        CHECK_FALSE(can_format_fixed_point(-1.0));
        CHECK_FALSE(can_format_fixed_point(::std::numeric_limits<double>::quiet_NaN()));
    }

    TEST_CASE("round-trip-and-idempotence") {
        for (double const value : {0.0, 1.0, 12.5, 0.1, 1e-18, 175.002439047, 214748364.75, 0.010924504404784173,
                                   7173733806442602179.0, 18446744073709549568.0}) {
            ::pltxt2htm_test::format_floating_details::check_round_trip(value);
        }
    }

    TEST_CASE("to-chars") {
        char8_t buffer[8];
        auto* end = ::pltxt2htm::format::to_chars(buffer, buffer + sizeof(buffer), 12.5);
        REQUIRE(end != nullptr);
        CHECK(::pltxt2htm::container::U8StringView{buffer, static_cast<::std::size_t>(end - buffer)} ==
              ::pltxt2htm::container::U8StringView{u8"12.5"});

        char8_t exact[4];
        CHECK(::pltxt2htm::format::to_chars(exact, exact + sizeof(exact), 12.5) == exact + sizeof(exact));
        CHECK(::pltxt2htm::format::to_chars(exact, exact + 3, 12.5) == nullptr);
        CHECK(::pltxt2htm::format::to_chars(buffer, buffer + sizeof(buffer), 1e-19) == nullptr);
        CHECK(::pltxt2htm::format::to_chars(buffer, buffer + sizeof(buffer),
                                            ::pltxt2htm::format::details::max_fixed_point_value) == nullptr);
    }

    TEST_CASE("round-up") {
        using ::pltxt2htm::details::double_to_uint64_ceil;
        using ::pltxt2htm::details::max_size_tag_value;
        using ::pltxt2htm::details::min_font_size_value;

        CHECK(double_to_uint64_ceil(0.0) == 0);
        CHECK(double_to_uint64_ceil(1.0) == 1);
        CHECK(double_to_uint64_ceil(1.2) == 2);
        CHECK(double_to_uint64_ceil(0.5) == 1);
        CHECK(double_to_uint64_ceil(214748364.75 / 2) == 107374183);
        CHECK(double_to_uint64_ceil(max_size_tag_value) == 18446744073709549568ull);
        CHECK(double_to_uint64_ceil(min_font_size_value) == 1);
    }

    TEST_CASE("append-decimal") {
        ::pltxt2htm::container::U8String out{};
        ::pltxt2htm::format::append_decimal(out, 12.5);
        ::pltxt2htm::format::append_decimal(out, 7);
        ::pltxt2htm::format::append_decimal(out, -3);
        ::pltxt2htm::format::append_decimal(out, 0.0);
        CHECK(out == u8"12.57-30");
    }

    TEST_CASE("deterministic-sweep") {
        ::std::uint64_t state{0x9e3779b97f4a7c15ull};
        for (::std::size_t i{}; i < 3000; ++i) {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            double value{};
            if (i % 2 == 0) {
                value = static_cast<double>(state % 1000000000ull) / static_cast<double>(1 + state % 1000000ull);
            }
            else {
                value = ::std::fabs(::std::bit_cast<double>(state));
            }
            if (::std::isfinite(value) == false) {
                continue;
            }
            if (::pltxt2htm::format::details::can_format_fixed_point(value) == false) {
                continue;
            }
            ::pltxt2htm_test::format_floating_details::check_round_trip(value);
        }
    }
}