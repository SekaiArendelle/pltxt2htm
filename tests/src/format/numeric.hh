#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

#include <pltxt2htm/numeric/uint192.hh>

#include "doctest_config.hh"

namespace pltxt2htm_test::numeric_details {

namespace numeric = ::pltxt2htm::numeric;

[[nodiscard]] constexpr auto make(::std::uint64_t const low, ::std::uint64_t const middle = 0,
                                  ::std::uint64_t const high = 0) -> numeric::UInt192 {
    return numeric::UInt192{low} | (numeric::UInt192{middle} << 64) | (numeric::UInt192{high} << 128);
}

[[nodiscard]] constexpr auto low64(numeric::UInt128 const value) -> ::std::uint64_t {
    return static_cast<::std::uint64_t>(value);
}

[[nodiscard]] constexpr auto high64(numeric::UInt128 const value) -> ::std::uint64_t {
    return static_cast<::std::uint64_t>(value >> 64);
}

#if defined(__SIZEOF_INT128__)
/// True when the 128-bit conversion keeps the low 128 bits.
template<typename T>
[[nodiscard]] constexpr auto low_128_bits_match(numeric::UInt192 const value) noexcept -> bool {
    T const low_bits = static_cast<T>(value);
    T const expected = (static_cast<T>(0x5555666677778888ull) << 64) | static_cast<T>(0x1111222233334444ull);
    return low_bits == expected;
}
#endif

} // namespace pltxt2htm_test::numeric_details

TEST_SUITE("numeric_integers") {
    TEST_CASE("multiply-128") {
        using ::pltxt2htm_test::numeric_details::high64;
        using ::pltxt2htm_test::numeric_details::low64;

        auto result = ::pltxt2htm::numeric::UInt128{1234567890123456789ull} * 9876543210987654321ull;
        CHECK(high64(result) == 660998117878163465ull);
        CHECK(low64(result) == 5547335688409725829ull);

        result = ::pltxt2htm::numeric::UInt128{9007199254740993ull} * 2;
        CHECK(high64(result) == 0);
        CHECK(low64(result) == 18014398509481986ull);

        result = ::pltxt2htm::numeric::UInt128{0x100000000ull} * 0x100000000ull;
        CHECK(high64(result) == 1);
        CHECK(low64(result) == 0);

        result = ::pltxt2htm::numeric::UInt128{0xFFFFFFFFFFFFFFFFull} * 0xFFFFFFFFFFFFFFFFull;
        CHECK(high64(result) == 0xFFFFFFFFFFFFFFFEull);
        CHECK(low64(result) == 1);

        result = ::pltxt2htm::numeric::UInt128{0xFFFFFFFFFFFFFFFFull} * 3;
        CHECK(high64(result) == 2);
        CHECK(low64(result) == 18446744073709551613ull);

        // Multiplication is modulo 2^128, like the UInt192 operators.
        result = (::pltxt2htm::numeric::UInt128{0xFFFFFFFFFFFFFFFFull} * 0xFFFFFFFFFFFFFFFFull) * 2;
        CHECK(high64(result) == 0xFFFFFFFFFFFFFFFCull);
        CHECK(low64(result) == 2);
    }

    TEST_CASE("uint128-operations") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::high64;
        using ::pltxt2htm_test::numeric_details::low64;

        auto const product = UInt128{0xFFFFFFFFFFFFFFFFull} * 0xFFFFFFFFFFFFFFFFull;
        CHECK(low64(product) == 1);
        CHECK(high64(product) == 0xFFFFFFFFFFFFFFFEull);
        CHECK(static_cast<bool>(product));
        CHECK_FALSE(static_cast<bool>(UInt128{}));
        CHECK_FALSE(static_cast<bool>(UInt128{0}));
        auto const high_only = UInt128{0x100000000ull} * 0x100000000ull;
        CHECK(low64(high_only) == 0);
        CHECK(high64(high_only) == 1);
        CHECK(static_cast<bool>(high_only));

        CHECK(high64(product >> 0) == 0xFFFFFFFFFFFFFFFEull);
        CHECK(low64(product >> 0) == 1);
        CHECK(high64(product >> 63) == 1);
        CHECK(low64(product >> 63) == 0xFFFFFFFFFFFFFFFCull);
        CHECK(high64(product >> 64) == 0);
        CHECK(low64(product >> 64) == 0xFFFFFFFFFFFFFFFEull);
        CHECK(high64(product >> 127) == 0);
        CHECK(low64(product >> 127) == 1);
#if !defined(__SIZEOF_INT128__)
        // The fallback defines shifts wider than the type as zero; native 128-bit shifts that
        // wide would be undefined behavior, so only the fallback checks them.
        CHECK(high64(product >> 128) == 0);
        CHECK(low64(product >> 128) == 0);
#endif
    }

    TEST_CASE("uint128-arithmetic") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::high64;
        using ::pltxt2htm_test::numeric_details::low64;

        auto const one = UInt128{1};
        auto const max = UInt128{0xFFFFFFFFFFFFFFFFull};

        auto const sum = max + one;
        CHECK(low64(sum) == 0);
        CHECK(high64(sum) == 1);

        auto const doubled = max + max;
        CHECK(low64(doubled) == 0xFFFFFFFFFFFFFFFEull);
        CHECK(high64(doubled) == 1);

        auto const difference = UInt128{} - one;
        CHECK(low64(difference) == 0xFFFFFFFFFFFFFFFFull);
        CHECK(high64(difference) == 0xFFFFFFFFFFFFFFFFull);

        auto const borrowed = (one << 64) - one;
        CHECK(low64(borrowed) == 0xFFFFFFFFFFFFFFFFull);
        CHECK(high64(borrowed) == 0);

        CHECK(low64(one << 0) == 1);
        CHECK(high64(one << 0) == 0);
        CHECK(low64(one << 63) == 0x8000000000000000ull);
        CHECK(high64(one << 63) == 0);
        CHECK(low64(one << 64) == 0);
        CHECK(high64(one << 64) == 1);
        CHECK(low64(one << 65) == 0);
        CHECK(high64(one << 65) == 2);
        CHECK(low64(one << 127) == 0);
        CHECK(high64(one << 127) == 0x8000000000000000ull);
#if !defined(__SIZEOF_INT128__)
        CHECK(low64(one << 128) == 0);
        CHECK(high64(one << 128) == 0);
#endif
    }

    TEST_CASE("bitwise-and") {
        using ::pltxt2htm::numeric::UInt192;
        using ::pltxt2htm_test::numeric_details::make;

        auto const value = make(0x1111222233334444ull, 0x5555666677778888ull, 0x9999AAAABBBBCCCCull);
        CHECK((value & make(0xFFFFFFFFFFFFFFFFull)) == make(0x1111222233334444ull));
        CHECK((value & make(0, 0xFFFFFFFFFFFFFFFFull)) == make(0, 0x5555666677778888ull));
        CHECK((value & make(0, 0, 0xFFFFFFFFFFFFFFFFull)) == make(0, 0, 0x9999AAAABBBBCCCCull));
        CHECK((value & make(0xFFFF0000FFFF0000ull, 0x00FF00FF00FF00FFull, 0xF0F0F0F0F0F0F0F0ull)) ==
              make(0x1111000033330000ull, 0x0055006600770088ull, 0x9090A0A0B0B0C0C0ull));
        CHECK((value & UInt192{}) == UInt192{});
    }

    TEST_CASE("bit-length") {
        using ::pltxt2htm_test::numeric_details::make;

        CHECK(make(0).bit_length() == 0);
        CHECK(make(1).bit_length() == 1);
        CHECK(make(2).bit_length() == 2);
        CHECK(make(0x8000000000000000ull).bit_length() == 64);
        CHECK(make(0, 1).bit_length() == 65);
        CHECK(make(0, 0x8000000000000000ull).bit_length() == 128);
        CHECK(make(0, 0, 1).bit_length() == 129);
        CHECK(make(0, 0, 0x8000000000000000ull).bit_length() == 192);
    }

    TEST_CASE("shifts") {
        using ::pltxt2htm_test::numeric_details::make;

        CHECK((make(1) << 0) == make(1));
        CHECK((make(1) << 63) == make(0x8000000000000000ull));
        CHECK((make(1) << 64) == make(0, 1));
        CHECK((make(1) << 65) == make(0, 2));
        CHECK((make(1) << 128) == make(0, 0, 1));
        CHECK((make(1) << 191) == make(0, 0, 0x8000000000000000ull));
        CHECK((make(1) << 192) == make(0));

        CHECK((make(0x8000000000000000ull) >> 63) == make(1));
        CHECK((make(0, 1) >> 64) == make(1));
        CHECK((make(0, 2) >> 65) == make(1));
        CHECK((make(0, 0, 1) >> 128) == make(1));
        CHECK((make(0, 0, 0x8000000000000000ull) >> 191) == make(1));
        CHECK((make(0, 0, 1) >> 192) == make(0));
    }

    TEST_CASE("add-subtract-compare") {
        using ::pltxt2htm_test::numeric_details::make;

        CHECK((make(0xFFFFFFFFFFFFFFFFull) + make(1)) == make(0, 1));
        CHECK((make(0, 0xFFFFFFFFFFFFFFFFull) + make(0, 1)) == make(0, 0, 1));
        CHECK((make(0, 1) - make(1)) == make(0xFFFFFFFFFFFFFFFFull));
        CHECK((make(0, 0, 1) - make(0, 1)) == make(0, 0xFFFFFFFFFFFFFFFFull));
        CHECK(make(0, 0, 1) > make(0, 1));
        CHECK(make(0, 1) < make(0, 0, 1));
        CHECK(make(1, 2, 3) == make(1, 2, 3));
    }

    TEST_CASE("multiply") {
        using ::pltxt2htm_test::numeric_details::make;

        CHECK((make(1, 2, 3) * 2) == make(2, 4, 6));
        CHECK((make(1, 2, 3) * 0) == make(0));
        CHECK((make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull) * 1) ==
              make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
    }

    TEST_CASE("signed-construction") {
        using ::pltxt2htm::numeric::UInt192;
        using ::pltxt2htm_test::numeric_details::make;

        auto const all_ones = make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
        UInt192 const from_unsigned = 5u;
        CHECK(from_unsigned == make(5));
        CHECK(UInt192{-1} == all_ones);
        CHECK(UInt192{-2} == (make(0) - make(2)));
        CHECK(UInt192{5} == make(5));
        CHECK(UInt192{static_cast<::std::int64_t>(-1)} == all_ones);

        using ::pltxt2htm::numeric::UInt128;
        UInt128 const from_unsigned_128 = 5u;
        CHECK(static_cast<::std::uint64_t>(from_unsigned_128) == 5);

#if !defined(__SIZEOF_INT128__)
        auto const negative = UInt128{-1};
        CHECK(static_cast<::std::uint64_t>(negative) == 0xFFFFFFFFFFFFFFFFull);
        CHECK(static_cast<::std::uint64_t>(negative >> 64) == 0xFFFFFFFFFFFFFFFFull);
#endif
    }

    TEST_CASE("explicit-conversion") {
        using ::pltxt2htm_test::numeric_details::make;

        auto const value = make(0x1111222233334444ull, 0x5555666677778888ull, 0xFEDCBA9876543210ull);
        CHECK(static_cast<::std::uint64_t>(value) == 0x1111222233334444ull);
        CHECK(static_cast<::std::uint32_t>(value) == 0x33334444u);
        CHECK(static_cast<::std::uint8_t>(value) == 0x44u);
        CHECK(static_cast<bool>(value));
        CHECK_FALSE(static_cast<bool>(make(0)));
        CHECK(static_cast<bool>(make(0, 1)));
        CHECK(static_cast<bool>(make(0, 0, 1)));
#if defined(__SIZEOF_INT128__)
        CHECK(::pltxt2htm_test::numeric_details::low_128_bits_match<::pltxt2htm::numeric::UInt128>(value));
#endif
    }

    TEST_CASE("powers-and-divide") {
        using ::pltxt2htm::numeric::divide;
        using ::pltxt2htm::numeric::pow5;
        using ::pltxt2htm::numeric::pow10;
        using ::pltxt2htm_test::numeric_details::make;

        CHECK(pow5(0) == make(1));
        CHECK(pow5(3) == make(125));
        CHECK(pow10(0) == make(1));
        CHECK(pow10(19) == make(10000000000000000000ull));
        CHECK((pow10(19) * 10) == pow10(20));
        CHECK((pow10(38) * 10) == pow10(39));

        // q * d + r == n and r < d for a large numerator and a 64-bit divisor.
        auto const numerator = pow10(38);
        auto const divisor = pow5(19);
        auto const division = divide(numerator, divisor);
        CHECK(((division.quotient * static_cast<::std::uint64_t>(divisor)) + division.remainder) == numerator);
        CHECK(division.remainder < divisor);
        CHECK(divide(divisor, divisor).quotient == make(1));
        CHECK(divide(divisor, divisor).remainder == make(0));
        CHECK(divide(make(5), make(7)).quotient == make(0));
        CHECK(divide(make(5), make(7)).remainder == make(5));

        // A full-width numerator with a 64-bit divisor exercises every limb of the quotient.
        auto const wide_numerator = make(0xFFFFFFFFFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull);
        auto const wide_divisor = make(0x9E3779B97F4A7C15ull);
        auto const wide_division = divide(wide_numerator, wide_divisor);
        CHECK(((wide_division.quotient * static_cast<::std::uint64_t>(wide_divisor)) + wide_division.remainder) ==
              wide_numerator);
        CHECK(wide_division.remainder < wide_divisor);
    }

    TEST_CASE("ratio-to-double") {
        using ::pltxt2htm::numeric::decimal_ratio_to_double;
        using ::pltxt2htm::numeric::pow10;
        using ::pltxt2htm_test::numeric_details::make;

        CHECK(decimal_ratio_to_double(make(0), 5) == 0.0);
        CHECK(decimal_ratio_to_double(make(1), 0) == 1.0);
        CHECK(decimal_ratio_to_double(make(125), 1) == 12.5);
        CHECK(decimal_ratio_to_double(make(1), 1) == 0.1);
        CHECK(decimal_ratio_to_double(make(1), 18) == 1e-18);
        CHECK(decimal_ratio_to_double(pow10(38), 38) == 1.0);
        CHECK(decimal_ratio_to_double(pow10(38) + make(1), 38) == 1.0);
        CHECK(decimal_ratio_to_double(pow10(19) * 3, 19) == 3.0);
        CHECK(decimal_ratio_to_double(make(0xFFFFFFFFFFFFFFFFull), 0) == 18446744073709551616.0);

        // 2^53 + 1 is exactly halfway between two doubles: ties-to-even goes down, and a sticky
        // tail above the midpoint goes up.
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0) == 9007199254740992.0);
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0, true) == 9007199254740994.0);
    }
}