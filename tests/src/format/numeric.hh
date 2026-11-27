#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

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

/// Checks both operand orders against independently specified result limbs.
template<typename T>
[[nodiscard]] constexpr auto product_128_matches(numeric::UInt128 const left, T const right, ::std::uint64_t const low,
                                                 ::std::uint64_t const high) noexcept -> bool {
    auto const forward = left * right;
    auto const reverse = right * left;
    static_assert(::std::same_as<decltype(forward), numeric::UInt128 const>);
    static_assert(::std::same_as<decltype(reverse), numeric::UInt128 const>);
    return low64(forward) == low && high64(forward) == high && low64(reverse) == low && high64(reverse) == high;
}

/// True when the 128-bit conversion keeps the low 128 bits.
[[nodiscard]] constexpr auto low_128_bits_match(numeric::UInt192 const value) noexcept -> bool {
    auto const low_bits = static_cast<numeric::UInt128>(value);
    return low64(low_bits) == 0x1111222233334444ull && high64(low_bits) == 0x5555666677778888ull;
}

#if defined(__SIZEOF_INT128__)
__extension__ using NativeUInt128 = unsigned __int128;
__extension__ using NativeInt128 = __int128;

[[nodiscard]] constexpr auto native_128_matches(numeric::UInt128 const actual, NativeUInt128 const expected) noexcept
    -> bool {
    return low64(actual) == static_cast<::std::uint64_t>(expected) &&
           high64(actual) == static_cast<::std::uint64_t>(expected >> 64);
}

[[nodiscard]] constexpr auto next_numeric_random(::std::uint64_t& state) noexcept -> ::std::uint64_t {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}
#endif

} // namespace pltxt2htm_test::numeric_details

TEST_SUITE("numeric_integers") {
    TEST_CASE("uint128-class-contract") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::high64;
        using ::pltxt2htm_test::numeric_details::low64;

        static_assert(::std::is_class_v<UInt128>);
        static_assert(sizeof(UInt128) == 16);
        static_assert(::std::is_trivially_copyable_v<UInt128>);
        static_assert(::std::is_constructible_v<UInt128, int>);
        static_assert(!::std::is_constructible_v<UInt128, double>);
        static_assert(::std::is_convertible_v<unsigned, UInt128>);
        static_assert(!::std::is_convertible_v<int, UInt128>);
        static_assert(!::std::is_assignable_v<UInt128&, int>);
        static_assert(!::std::is_convertible_v<UInt128, ::std::uint64_t>);
        static_assert(!::std::is_convertible_v<UInt128, bool>);

        constexpr UInt128 zero;
        static_assert(!static_cast<bool>(zero));
        UInt128 default_initialized;
        CHECK(low64(default_initialized) == 0);
        CHECK(high64(default_initialized) == 0);

        int signed_source = 1;
        UInt128 const direct{signed_source};
        UInt128 const implicit = 5u;
        CHECK(low64(direct) == 1);
        CHECK(high64(direct) == 0);
        CHECK(low64(implicit) == 5);
        CHECK(high64(implicit) == 0);

        auto const wide = (UInt128{0x123456789ABCDEF0ull} << 64) + UInt128{0xFEDCBA9876543210ull};
        CHECK(static_cast<::std::uint32_t>(wide) == 0x76543210u);
        CHECK(static_cast<::std::uint16_t>(wide) == 0x3210u);
        CHECK(static_cast<::std::uint8_t>(wide) == 0x10u);
    }

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

    TEST_CASE("multiply-128-signed-operands") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::product_128_matches;

        auto const high_bit = UInt128{1} << 64;
        auto const all_ones = UInt128{0xFFFFFFFFFFFFFFFFull} + (UInt128{0xFFFFFFFFFFFFFFFFull} << 64);

        CHECK(product_128_matches(UInt128{}, -1, 0, 0));
        CHECK(product_128_matches(UInt128{1}, -1, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(UInt128{3}, -2, 0xFFFFFFFFFFFFFFFAull, 0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(high_bit, -1, 0, 0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(all_ones, -1, 1, 0));

        CHECK(product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int8_t>::min)(), 0xFFFFFFFFFFFFFF00ull,
                                  0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int16_t>::min)(), 0xFFFFFFFFFFFF0000ull,
                                  0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int32_t>::min)(), 0xFFFFFFFF00000000ull,
                                  0xFFFFFFFFFFFFFFFFull));
        CHECK(
            product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int64_t>::min)(), 0, 0xFFFFFFFFFFFFFFFFull));
        CHECK(product_128_matches(high_bit, (::std::numeric_limits<::std::int64_t>::min)(), 0, 0x8000000000000000ull));
        CHECK(product_128_matches(all_ones, (::std::numeric_limits<::std::int64_t>::min)(), 0x8000000000000000ull, 0));
        CHECK(
            product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int64_t>::max)(), 0xFFFFFFFFFFFFFFFEull, 0));

        CHECK(product_128_matches(high_bit, 0, 0, 0));
        CHECK(product_128_matches(high_bit, 1, 0, 1));
        CHECK(product_128_matches(high_bit, false, 0, 0));
        CHECK(product_128_matches(high_bit, true, 0, 1));
        CHECK(
            product_128_matches(UInt128{2}, (::std::numeric_limits<::std::uint64_t>::max)(), 0xFFFFFFFFFFFFFFFEull, 1));
    }

    TEST_CASE("multiply-128-wide-operands") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::product_128_matches;

        auto const high_bit = UInt128{1} << 64;
        auto const both_limbs = high_bit + UInt128{1};
        auto const low_ones = UInt128{0xFFFFFFFFFFFFFFFFull};
        auto const all_ones = low_ones + (low_ones << 64);

        CHECK(product_128_matches(both_limbs, UInt128{}, 0, 0));
        CHECK(product_128_matches(both_limbs, UInt128{1}, 1, 1));
        CHECK(product_128_matches(high_bit, high_bit, 0, 0));
        CHECK(product_128_matches(both_limbs, both_limbs, 1, 2));
        CHECK(product_128_matches(low_ones, low_ones, 1, 0xFFFFFFFFFFFFFFFEull));
        CHECK(product_128_matches(all_ones, all_ones, 1, 0));
        CHECK(product_128_matches(high_bit + low_ones, high_bit + low_ones, 1, 0xFFFFFFFFFFFFFFFCull));
        CHECK(product_128_matches(all_ones, UInt128{2}, 0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFFull));
    }

    TEST_CASE("multiply-128-constexpr") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::product_128_matches;

        constexpr auto high_bit = UInt128{1} << 64;
        static_assert(product_128_matches(UInt128{1}, -1, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
        static_assert(
            product_128_matches(UInt128{2}, (::std::numeric_limits<::std::int64_t>::min)(), 0, 0xFFFFFFFFFFFFFFFFull));
        static_assert(product_128_matches(high_bit + UInt128{1}, high_bit + UInt128{1}, 1, 2));
        static_assert(product_128_matches(high_bit, high_bit, 0, 0));
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
        CHECK(high64(product >> 128) == 0);
        CHECK(low64(product >> 128) == 0);
        CHECK_FALSE(static_cast<bool>(product >> 129));
        CHECK_FALSE(static_cast<bool>(product >> (::std::numeric_limits<unsigned>::max)()));
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
        CHECK(low64(one << 128) == 0);
        CHECK(high64(one << 128) == 0);
        CHECK_FALSE(static_cast<bool>(one << 129));
        CHECK_FALSE(static_cast<bool>(one << (::std::numeric_limits<unsigned>::max)()));
        static_assert(!static_cast<bool>(UInt128{1} << 128));
        static_assert(!static_cast<bool>(UInt128{1} >> 128));
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

    TEST_CASE("wide-operands") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::make;

        auto const all_ones = make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);

        CHECK((make(0) + (-1)) == all_ones);
        CHECK(((-1) + make(0)) == all_ones);
        CHECK((make(1) - 2) == all_ones);
        CHECK(((-1) - make(0)) == all_ones);
        CHECK((make(3) * (-2)) == (make(0) - make(6)));
        CHECK(((-2) * make(3)) == (make(0) - make(6)));

        CHECK((make(0, 1) * make(0, 1)) == make(0, 0, 1));
        CHECK((make(0xFFFFFFFFFFFFFFFFull) * make(0xFFFFFFFFFFFFFFFFull)) == make(1, 0xFFFFFFFFFFFFFFFEull, 0));
        CHECK((make(0, 0, 1) * make(0xFFFFFFFFFFFFFFFFull)) == make(0, 0, 0xFFFFFFFFFFFFFFFFull));
        CHECK((make(1, 1, 1) * make(1, 1, 1)) == make(1, 2, 3));

        auto const high_bit = UInt128{1} << 64;
        CHECK((make(1) + high_bit) == make(1, 1));
        CHECK((high_bit + make(1)) == make(1, 1));
        CHECK((make(1) * high_bit) == make(0, 1));
        CHECK((high_bit * make(1)) == make(0, 1));

        auto const both_limbs = UInt128{1} + (UInt128{1} << 64);
        CHECK((make(1) * both_limbs) == make(1, 1));
        CHECK((both_limbs * make(1)) == make(1, 1));

        CHECK((make(0xFFFFFFFFFFFFFFFFull) & (-1)) == make(0xFFFFFFFFFFFFFFFFull));
        CHECK((make(1) | (-1)) == all_ones);
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
        auto const wide_source = UInt128{1} << 64;
        UInt192 const from_wide = wide_source;
        CHECK(from_wide == make(0, 1));
        CHECK(UInt192{wide_source} == make(0, 1));
        auto const wide_product = UInt128{0xFFFFFFFFFFFFFFFFull} * 0xFFFFFFFFFFFFFFFFull;
        CHECK(UInt192{wide_product} == make(1, 0xFFFFFFFFFFFFFFFEull, 0));

        auto const negative = UInt128{-1};
        CHECK(static_cast<::std::uint64_t>(negative) == 0xFFFFFFFFFFFFFFFFull);
        CHECK(static_cast<::std::uint64_t>(negative >> 64) == 0xFFFFFFFFFFFFFFFFull);
        CHECK(UInt192{negative} == make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0));
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
        CHECK(::pltxt2htm_test::numeric_details::low_128_bits_match(value));
        constexpr auto wide = make(0x1111222233334444ull, 0x5555666677778888ull, 0xFEDCBA9876543210ull);
        static_assert(::pltxt2htm_test::numeric_details::low_128_bits_match(wide));
        CHECK(::pltxt2htm::numeric::UInt192{static_cast<::pltxt2htm::numeric::UInt128>(value)} ==
              make(0x1111222233334444ull, 0x5555666677778888ull));
    }

#if defined(__SIZEOF_INT128__)
    TEST_CASE("uint128-native-interoperability") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm::numeric::UInt192;
        using ::pltxt2htm_test::numeric_details::make;
        using ::pltxt2htm_test::numeric_details::native_128_matches;
        using ::pltxt2htm_test::numeric_details::NativeInt128;
        using ::pltxt2htm_test::numeric_details::NativeUInt128;

        static_assert(::std::is_convertible_v<NativeUInt128, UInt128>);
        static_assert(!::std::is_convertible_v<NativeInt128, UInt128>);
        static_assert(!::std::is_convertible_v<UInt128, NativeUInt128>);

        constexpr auto source = (NativeUInt128{0xFEDCBA9876543210ull} << 64) | NativeUInt128{0x123456789ABCDEF0ull};
        constexpr UInt128 from_unsigned = source;
        static_assert(native_128_matches(from_unsigned, source));
        static_assert(static_cast<NativeUInt128>(from_unsigned) == source);
        static_assert(noexcept(static_cast<NativeUInt128>(from_unsigned)));
        CHECK(native_128_matches(from_unsigned, source));
        CHECK(static_cast<NativeUInt128>(from_unsigned) == source);

        constexpr auto negative = -(NativeInt128{1} << 100) + 7;
        static_assert(native_128_matches(UInt128{negative}, static_cast<NativeUInt128>(negative)));
        CHECK(native_128_matches(UInt128{negative}, static_cast<NativeUInt128>(negative)));
        constexpr auto signed_min = -(NativeInt128{1} << 126) - (NativeInt128{1} << 126);
        static_assert(native_128_matches(UInt128{signed_min}, NativeUInt128{1} << 127));
        static_assert(native_128_matches(UInt128{2} * signed_min, 0));

        auto const high_bit = NativeUInt128{1} << 64;
        CHECK(native_128_matches(UInt128{3} * high_bit, NativeUInt128{3} << 64));
        CHECK(native_128_matches(high_bit * UInt128{3}, NativeUInt128{3} << 64));
        CHECK(native_128_matches(UInt128{3} * negative, NativeUInt128{3} * static_cast<NativeUInt128>(negative)));
        CHECK(native_128_matches(negative * UInt128{3}, NativeUInt128{3} * static_cast<NativeUInt128>(negative)));

        CHECK(UInt192{source} == make(0x123456789ABCDEF0ull, 0xFEDCBA9876543210ull));
        CHECK(UInt192{NativeInt128{-1}} == make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
        CHECK((make(1) * high_bit) == make(0, 1));
        CHECK((high_bit * make(1)) == make(0, 1));
        CHECK((make(3) * NativeInt128{-2}) == (make(0) - make(6)));
        CHECK((NativeInt128{-2} * make(3)) == (make(0) - make(6)));
        CHECK(static_cast<NativeUInt128>(make(0x123456789ABCDEF0ull, 0xFEDCBA9876543210ull, 1)) == source);
    }

    TEST_CASE("uint128-native-arithmetic-oracle") {
        using ::pltxt2htm::numeric::UInt128;
        using ::pltxt2htm_test::numeric_details::native_128_matches;
        using ::pltxt2htm_test::numeric_details::NativeUInt128;
        using ::pltxt2htm_test::numeric_details::next_numeric_random;

        ::std::uint64_t state{0x9E3779B97F4A7C15ull};
        for (unsigned i{}; i < 1024; ++i) {
            auto const left_low = next_numeric_random(state);
            auto const left_high = next_numeric_random(state);
            auto const right_low = next_numeric_random(state);
            auto const right_high = next_numeric_random(state);
            auto const left = UInt128{left_low} + (UInt128{left_high} << 64);
            auto const right = UInt128{right_low} + (UInt128{right_high} << 64);
            auto const native_left = NativeUInt128{left_low} | (NativeUInt128{left_high} << 64);
            auto const native_right = NativeUInt128{right_low} | (NativeUInt128{right_high} << 64);
            auto const signed_factor = static_cast<::std::int64_t>(right_low);
            auto const shift = static_cast<unsigned>(right_high % 160);

            CHECK(native_128_matches(left + right, native_left + native_right));
            CHECK(native_128_matches(left - right, native_left - native_right));
            CHECK(native_128_matches(left * right, native_left * native_right));
            CHECK(native_128_matches(left * right_low, native_left * right_low));
            CHECK(native_128_matches(left * signed_factor, native_left * static_cast<NativeUInt128>(signed_factor)));
            CHECK(native_128_matches(signed_factor * left, native_left * static_cast<NativeUInt128>(signed_factor)));
            CHECK(native_128_matches(left << shift, shift < 128 ? native_left << shift : NativeUInt128{}));
            CHECK(native_128_matches(left >> shift, shift < 128 ? native_left >> shift : NativeUInt128{}));
        }
    }
#endif

    TEST_CASE("divide-operators") {
        using ::pltxt2htm_test::numeric_details::make;

        CHECK((make(100) / make(7)) == make(14));
        CHECK((make(100) % make(7)) == make(2));
        CHECK((make(7) / make(100)) == make(0));
        CHECK((make(7) % make(100)) == make(7));

        CHECK((make(100) / 7u) == make(14));
        CHECK((make(100) % 7u) == make(2));
        CHECK((100u / make(7)) == make(14));
        CHECK((100u % make(7)) == make(2));

        auto const numerator = make(0xFFFFFFFFFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull);
        auto const divisor = make(0x9E3779B97F4A7C15ull);
        // Precomputed quotient and remainder are an independent oracle; dividing and comparing
        // against divide()'s own result would be a tautology.
        auto const expected_quotient = make(0xC4871CA04BA57AD1ull, 0x9C60305DA68C2512ull, 0x1ull);
        auto const expected_remainder = make(0x60A682697485B0DAull);
        CHECK((numerator / divisor) == expected_quotient);
        CHECK((numerator % divisor) == expected_remainder);
        CHECK(((expected_quotient * divisor) + expected_remainder) == numerator);
        CHECK(expected_remainder < divisor);
    }

    TEST_CASE("powers-and-divide") {
        using ::pltxt2htm::numeric::divide;
        using ::pltxt2htm::numeric::pow;
        using ::pltxt2htm::numeric::UInt192;
        using ::pltxt2htm_test::numeric_details::make;

        CHECK(pow(UInt192{5}, 0) == make(1));
        CHECK(pow(UInt192{5}, 3) == make(125));
        CHECK(pow(UInt192{10}, 0) == make(1));
        CHECK(pow(UInt192{10}, 19) == make(10000000000000000000ull));
        CHECK((pow(UInt192{10}, 19) * 10) == pow(UInt192{10}, 20));
        CHECK((pow(UInt192{10}, 38) * 10) == pow(UInt192{10}, 39));

        // q * d + r == n and r < d for a large numerator and a 64-bit divisor.
        auto const numerator = pow(UInt192{10}, 38);
        auto const divisor = pow(UInt192{5}, 19);
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

        // Divisors spanning all three limbs, including one just above 2^191 where the running
        // remainder grows largest, pin the invariant that shifting the remainder left stays in range.
        auto const all_ones = make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
        auto const three_limb_divisor = make(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0x7FFFFFFFFFFFFFFFull);
        auto const three_limb_division = divide(all_ones, three_limb_divisor);
        CHECK(three_limb_division.quotient == make(2));
        CHECK(three_limb_division.remainder == make(1));

        auto const above_2_pow_191_divisor = make(0x0000000000000001ull, 0, 0x8000000000000000ull);
        auto const above_2_pow_191_division = divide(all_ones, above_2_pow_191_divisor);
        CHECK(above_2_pow_191_division.quotient == make(1));
        CHECK(above_2_pow_191_division.remainder ==
              make(0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFFull, 0x7FFFFFFFFFFFFFFFull));
        CHECK(((above_2_pow_191_division.quotient * above_2_pow_191_divisor) + above_2_pow_191_division.remainder) ==
              all_ones);
        CHECK(above_2_pow_191_division.remainder < above_2_pow_191_divisor);
    }

    TEST_CASE("ratio-to-double") {
        using ::pltxt2htm::numeric::decimal_ratio_to_double;
        using ::pltxt2htm::numeric::pow;
        using ::pltxt2htm::numeric::UInt192;
        using ::pltxt2htm_test::numeric_details::make;

        CHECK(decimal_ratio_to_double(make(0), 5) == 0.0);
        CHECK(decimal_ratio_to_double(make(1), 0) == 1.0);
        CHECK(decimal_ratio_to_double(make(125), 1) == 12.5);
        CHECK(decimal_ratio_to_double(make(1), 1) == 0.1);
        CHECK(decimal_ratio_to_double(make(1), 18) == 1e-18);
        CHECK(decimal_ratio_to_double(pow(UInt192{10}, 38), 38) == 1.0);
        CHECK(decimal_ratio_to_double(pow(UInt192{10}, 38) + make(1), 38) == 1.0);
        CHECK(decimal_ratio_to_double(pow(UInt192{10}, 19) * 3, 19) == 3.0);
        CHECK(decimal_ratio_to_double(make(0xFFFFFFFFFFFFFFFFull), 0) == 18446744073709551616.0);

        // 2^53 + 1 is exactly halfway between two doubles and rounds down (ties-to-even), while a
        // sticky tail above the midpoint goes up.
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0) == 9007199254740992.0);
        CHECK(decimal_ratio_to_double(make(9007199254740993ull), 0, true) == 9007199254740994.0);
        // 2^53 + 3 is exactly halfway and the lower candidate has an odd significand, so
        // ties-to-even rounds up.
        CHECK(decimal_ratio_to_double(make(9007199254740995ull), 0) == 9007199254740996.0);
    }

    TEST_CASE("constexpr-evaluation") {
        using ::pltxt2htm::numeric::decimal_ratio_to_double;
        using ::pltxt2htm::numeric::divide;
        using ::pltxt2htm_test::numeric_details::make;

        static_assert((make(1) << 191) == make(0, 0, 0x8000000000000000ull));
        static_assert((make(0, 0, 0x8000000000000000ull) >> 191) == make(1));
        static_assert(divide(make(0xFFFFFFFFFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull),
                             make(0x9E3779B97F4A7C15ull))
                          .quotient == make(0xC4871CA04BA57AD1ull, 0x9C60305DA68C2512ull, 0x1ull));
        static_assert(divide(make(0xFFFFFFFFFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull),
                             make(0x9E3779B97F4A7C15ull))
                          .remainder == make(0x60A682697485B0DAull));
        static_assert(decimal_ratio_to_double(make(9007199254740995ull), 0) == 9007199254740996.0);
    }
}
