#pragma once

#include <cstddef>
#include <cstdint>

#include <pltxt2htm/details/font_size_domain.hh>
#include <pltxt2htm/details/wide_decimal.hh>

#include "doctest_config.hh"

namespace pltxt2htm_test::format_wide_details {

namespace wide = ::pltxt2htm::details::wide;

[[nodiscard]] constexpr auto make(::std::uint64_t const low, ::std::uint64_t const middle = 0,
                                  ::std::uint64_t const high = 0) -> wide::UInt192 {
    return wide::UInt192{.low = low, .middle = middle, .high = high};
}

[[nodiscard]] constexpr auto equals(wide::UInt192 const left, wide::UInt192 const right) -> bool {
    return wide::compare(left, right) == 0;
}

} // namespace pltxt2htm_test::format_wide_details

TEST_SUITE("format_wide_decimal") {
    TEST_CASE("multiply-64x64") {
        auto result = ::pltxt2htm::details::wide::multiply_64x64(1234567890123456789ull, 9876543210987654321ull);
        CHECK(result.high == 660998117878163465ull);
        CHECK(result.low == 5547335688409725829ull);

        result = ::pltxt2htm::details::wide::multiply_64x64(9007199254740993ull, 2);
        CHECK(result.high == 0);
        CHECK(result.low == 18014398509481986ull);

        result = ::pltxt2htm::details::wide::multiply_64x64(0x100000000ull, 0x100000000ull);
        CHECK(result.high == 1);
        CHECK(result.low == 0);

        result = ::pltxt2htm::details::wide::multiply_64x64(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
        CHECK(result.high == 0xFFFFFFFFFFFFFFFEull);
        CHECK(result.low == 1);

        result = ::pltxt2htm::details::wide::multiply_64x64(0xFFFFFFFFFFFFFFFFull, 3);
        CHECK(result.high == 2);
        CHECK(result.low == 18446744073709551613ull);
    }

    TEST_CASE("bit-length") {
        using ::pltxt2htm::details::wide::bit_length;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(bit_length(make(0)) == 0);
        CHECK(bit_length(make(1)) == 1);
        CHECK(bit_length(make(2)) == 2);
        CHECK(bit_length(make(0x8000000000000000ull)) == 64);
        CHECK(bit_length(make(0, 1)) == 65);
        CHECK(bit_length(make(0, 0x8000000000000000ull)) == 128);
        CHECK(bit_length(make(0, 0, 1)) == 129);
        CHECK(bit_length(make(0, 0, 0x8000000000000000ull)) == 192);
    }

    TEST_CASE("shifts") {
        using ::pltxt2htm::details::wide::shift_left;
        using ::pltxt2htm::details::wide::shift_right;
        using ::pltxt2htm_test::format_wide_details::equals;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(equals(shift_left(make(1), 0), make(1)));
        CHECK(equals(shift_left(make(1), 63), make(0x8000000000000000ull)));
        CHECK(equals(shift_left(make(1), 64), make(0, 1)));
        CHECK(equals(shift_left(make(1), 65), make(0, 2)));
        CHECK(equals(shift_left(make(1), 128), make(0, 0, 1)));
        CHECK(equals(shift_left(make(1), 191), make(0, 0, 0x8000000000000000ull)));
        CHECK(equals(shift_left(make(1), 192), make(0)));

        CHECK(equals(shift_right(make(0x8000000000000000ull), 63), make(1)));
        CHECK(equals(shift_right(make(0, 1), 64), make(1)));
        CHECK(equals(shift_right(make(0, 2), 65), make(1)));
        CHECK(equals(shift_right(make(0, 0, 1), 128), make(1)));
        CHECK(equals(shift_right(make(0, 0, 0x8000000000000000ull), 191), make(1)));
        CHECK(equals(shift_right(make(0, 0, 1), 192), make(0)));
    }

    TEST_CASE("add-subtract-compare") {
        using ::pltxt2htm::details::wide::add;
        using ::pltxt2htm::details::wide::compare;
        using ::pltxt2htm::details::wide::subtract;
        using ::pltxt2htm_test::format_wide_details::equals;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(equals(add(make(0xFFFFFFFFFFFFFFFFull), make(1)), make(0, 1)));
        CHECK(equals(add(make(0, 0xFFFFFFFFFFFFFFFFull), make(0, 1)), make(0, 0, 1)));
        CHECK(equals(subtract(make(0, 1), make(1)), make(0xFFFFFFFFFFFFFFFFull)));
        CHECK(equals(subtract(make(0, 0, 1), make(0, 1)), make(0, 0xFFFFFFFFFFFFFFFFull)));
        CHECK(compare(make(0, 0, 1), make(0, 1)) > 0);
        CHECK(compare(make(0, 1), make(0, 0, 1)) < 0);
        CHECK(compare(make(1, 2, 3), make(1, 2, 3)) == 0);
    }

    TEST_CASE("multiply") {
        using ::pltxt2htm::details::wide::multiply;
        using ::pltxt2htm_test::format_wide_details::equals;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(equals(multiply(make(1, 2, 3), 2), make(2, 4, 6)));
        CHECK(equals(multiply(make(1, 2, 3), 0), make(0)));
        CHECK(equals(multiply(make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull), 1),
                     make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull)));
    }

    TEST_CASE("powers-and-divide") {
        using ::pltxt2htm::details::wide::add;
        using ::pltxt2htm::details::wide::compare;
        using ::pltxt2htm::details::wide::divide;
        using ::pltxt2htm::details::wide::is_zero;
        using ::pltxt2htm::details::wide::multiply;
        using ::pltxt2htm::details::wide::pow5;
        using ::pltxt2htm::details::wide::pow10;
        using ::pltxt2htm_test::format_wide_details::equals;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(equals(pow5(0), make(1)));
        CHECK(equals(pow5(3), make(125)));
        CHECK(equals(pow10(0), make(1)));
        CHECK(equals(pow10(19), make(10000000000000000000ull)));
        CHECK(equals(multiply(pow10(19), 10), pow10(20)));
        CHECK(equals(multiply(pow10(38), 10), pow10(39)));

        // q * d + r == n and r < d for a large numerator and a 64-bit divisor.
        auto const numerator = pow10(38);
        auto const divisor = pow5(19);
        auto const division = divide(numerator, divisor);
        CHECK(equals(add(multiply(division.quotient, divisor.low), division.remainder), numerator));
        CHECK(compare(division.remainder, divisor) < 0);
        CHECK(equals(divide(divisor, divisor).quotient, make(1)));
        CHECK(is_zero(divide(divisor, divisor).remainder));
        CHECK(is_zero(divide(make(5), make(7)).quotient));
        CHECK(equals(divide(make(5), make(7)).remainder, make(5)));

        // A full-width numerator with a 64-bit divisor exercises every limb of the quotient.
        auto const wide_numerator = make(0xFFFFFFFFFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull);
        auto const wide_divisor = make(0x9E3779B97F4A7C15ull);
        auto const wide_division = divide(wide_numerator, wide_divisor);
        CHECK(equals(add(multiply(wide_division.quotient, wide_divisor.low), wide_division.remainder), wide_numerator));
        CHECK(compare(wide_division.remainder, wide_divisor) < 0);
    }

    TEST_CASE("ratio-to-double") {
        using ::pltxt2htm::details::min_font_size_value;
        using ::pltxt2htm::details::wide::add;
        using ::pltxt2htm::details::wide::decimal_ratio_to_double;
        using ::pltxt2htm::details::wide::multiply;
        using ::pltxt2htm::details::wide::pow10;
        using ::pltxt2htm_test::format_wide_details::make;

        CHECK(decimal_ratio_to_double(make(0), 5) == 0.0);
        CHECK(decimal_ratio_to_double(make(1), 0) == 1.0);
        CHECK(decimal_ratio_to_double(make(125), 1) == 12.5);
        CHECK(decimal_ratio_to_double(make(1), 1) == 0.1);
        CHECK(decimal_ratio_to_double(make(1), 18) == min_font_size_value);
        CHECK(decimal_ratio_to_double(pow10(38), 38) == 1.0);
        CHECK(decimal_ratio_to_double(add(pow10(38), make(1)), 38) == 1.0);
        CHECK(decimal_ratio_to_double(multiply(pow10(19), 3), 19) == 3.0);
        CHECK(decimal_ratio_to_double(make(0xFFFFFFFFFFFFFFFFull), 0) == 18446744073709551616.0);

        // 2^53 + 1 is exactly halfway between two doubles: ties-to-even goes down, and a sticky
        // tail above the midpoint goes up.
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0) == 9007199254740992.0);
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0, true) == 9007199254740994.0);
    }
}