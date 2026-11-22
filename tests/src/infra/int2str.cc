#include <charconv>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <pltxt2htm/details/utils.hh>
#include "precompile.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this file.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace {

/**
 * @brief Convert an integer through the standard library for use as an independent
 *        oracle in the exhaustive cross-checks in main().
 */
template<typename Integer>
    requires (::std::is_integral_v<Integer>)
[[nodiscard]]
auto oracle_int2str(Integer value) noexcept -> ::pltxt2htm::container::U8String {
    // digits10 + 1 covers the largest magnitude; one more character covers a sign.
    char buffer[::std::numeric_limits<Integer>::digits10 + 3];
    auto const conversion = ::std::to_chars(buffer, buffer + sizeof(buffer), value);

    ::pltxt2htm::container::U8String result{};
    for (auto ptr = buffer; ptr != conversion.ptr; ++ptr) {
        result.push_back(static_cast<char8_t>(*ptr));
    }
    return result;
}

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

} // namespace

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

static_assert(::pltxt2htm::details::decimal_digit_pairs.size() == 200);
static_assert(test_decimal_digit_pairs());

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
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{9}, end), end, u8"9")) {
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

static_assert(test_write_decimal_digits_backward());

int main() noexcept {
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(0.0), u8"0");
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(12.0), u8"12");
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(12.5), u8"12.5");
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(0.125), u8"0.125");
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(0.03125), u8"0.03125");
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(1.0625), u8"1.0625");
        // 2^63 sits exactly on double2str's fixed-point bound, so it produces no scaled
        // candidate; its exact digits must still come out of the truncated-integer fallback.
        if constexpr (sizeof(::std::size_t) == 8) {
            pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(9223372036854775808.0),
                                        ::pltxt2htm::container::U8StringView{u8"9223372036854775808"});
            // 2^64 - 2048 is the largest double below 2^64, one ulp below the parser's
            // limit; it is outside the fixed-point range too, so it hits the same fallback.
            pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(18446744073709549568.0),
                                        ::pltxt2htm::container::U8StringView{u8"18446744073709549568"});
        }
        // Regression: on a 32-bit target, 214748364.75 * 10 is 2147483647.5, so rounding up
        // needed 2147483648, one past ptrdiff_t max, and the two-digit candidate needs
        // 21474836475, which no 32-bit integer holds; the value therefore fell back to the
        // truncated integer "214748364". The scaled value is rounded in 64-bit unsigned
        // arithmetic against a platform-independent bound, so 32-bit targets now keep the
        // two-digit form just like 64-bit ones do.
        pltxt2htm_test_assert_equal(::pltxt2htm::details::double2str(214748364.75),
                                    ::pltxt2htm::container::U8StringView{u8"214748364.75"});
    }

    // size_t2str: boundary values around the single-digit / pair / loop transitions
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(0), ::pltxt2htm::container::U8StringView{u8"0"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(1), ::pltxt2htm::container::U8StringView{u8"1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(9), ::pltxt2htm::container::U8StringView{u8"9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(10), ::pltxt2htm::container::U8StringView{u8"10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(11), ::pltxt2htm::container::U8StringView{u8"11"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(42), ::pltxt2htm::container::U8StringView{u8"42"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(99), ::pltxt2htm::container::U8StringView{u8"99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(100),
                                    ::pltxt2htm::container::U8StringView{u8"100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(101),
                                    ::pltxt2htm::container::U8StringView{u8"101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(110),
                                    ::pltxt2htm::container::U8StringView{u8"110"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(999),
                                    ::pltxt2htm::container::U8StringView{u8"999"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(1000),
                                    ::pltxt2htm::container::U8StringView{u8"1000"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(12345),
                                    ::pltxt2htm::container::U8StringView{u8"12345"});
    }

    // uint64_t2str: same conversion for the 64-bit scaled values, including values above a
    // 32-bit size_t maximum
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::uint64_t2str(0), ::pltxt2htm::container::U8StringView{u8"0"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::uint64_t2str(::std::uint64_t{1} << 32),
                                    ::pltxt2htm::container::U8StringView{u8"4294967296"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::uint64_t2str(::std::numeric_limits<::std::uint64_t>::max()),
                                    ::pltxt2htm::container::U8StringView{u8"18446744073709551615"});
    }

    // ptrdiff_t2str: sign handling around the same transitions
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(0),
                                    ::pltxt2htm::container::U8StringView{u8"0"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(1),
                                    ::pltxt2htm::container::U8StringView{u8"1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-1),
                                    ::pltxt2htm::container::U8StringView{u8"-1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(9),
                                    ::pltxt2htm::container::U8StringView{u8"9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-9),
                                    ::pltxt2htm::container::U8StringView{u8"-9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(10),
                                    ::pltxt2htm::container::U8StringView{u8"10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-10),
                                    ::pltxt2htm::container::U8StringView{u8"-10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(99),
                                    ::pltxt2htm::container::U8StringView{u8"99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-99),
                                    ::pltxt2htm::container::U8StringView{u8"-99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(100),
                                    ::pltxt2htm::container::U8StringView{u8"100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-100),
                                    ::pltxt2htm::container::U8StringView{u8"-100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(101),
                                    ::pltxt2htm::container::U8StringView{u8"101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-101),
                                    ::pltxt2htm::container::U8StringView{u8"-101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(12345),
                                    ::pltxt2htm::container::U8StringView{u8"12345"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-12345),
                                    ::pltxt2htm::container::U8StringView{u8"-12345"});
    }

    // extremes, including the PTRDIFF_MIN path that relies on unsigned negation
    {
        if constexpr (sizeof(::std::size_t) == 8) {
            pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(::std::numeric_limits<::std::size_t>::max()),
                                        ::pltxt2htm::container::U8StringView{u8"18446744073709551615"});
            pltxt2htm_test_assert_equal(
                ::pltxt2htm::details::ptrdiff_t2str(::std::numeric_limits<::std::ptrdiff_t>::max()),
                ::pltxt2htm::container::U8StringView{u8"9223372036854775807"});
            pltxt2htm_test_assert_equal(
                ::pltxt2htm::details::ptrdiff_t2str(::std::numeric_limits<::std::ptrdiff_t>::min()),
                ::pltxt2htm::container::U8StringView{u8"-9223372036854775808"});
        }
        else {
            pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(::std::numeric_limits<::std::size_t>::max()),
                                        ::pltxt2htm::container::U8StringView{u8"4294967295"});
            pltxt2htm_test_assert_equal(
                ::pltxt2htm::details::ptrdiff_t2str(::std::numeric_limits<::std::ptrdiff_t>::max()),
                ::pltxt2htm::container::U8StringView{u8"2147483647"});
            pltxt2htm_test_assert_equal(
                ::pltxt2htm::details::ptrdiff_t2str(::std::numeric_limits<::std::ptrdiff_t>::min()),
                ::pltxt2htm::container::U8StringView{u8"-2147483648"});
        }
    }

    // exhaustive cross-check against the pre-refactor algorithm
    for (::std::size_t i{}; i <= 100000; ++i) {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(i), oracle_int2str(i));
    }
    for (::std::uint64_t i{}; i <= 100000; ++i) {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::uint64_t2str(i), oracle_int2str(i));
    }
    for (::std::ptrdiff_t i{-100000}; i <= 100000; ++i) {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(i), oracle_int2str(i));
    }

    return 0;
}

#pragma pop_macro("max")
#pragma pop_macro("min")
