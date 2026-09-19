#include <cstdint>
#include <limits>
#include <pltxt2htm/details/utils.hh>
#include "precompile.hh"

namespace {

/**
 * @brief Reference implementation mirroring the pre-refactor algorithm: extract
 *        digits least-significant-first, then reverse. Used as the oracle for the
 *        exhaustive cross-checks in main().
 */
[[nodiscard]]
auto oracle_uint2str(::std::uintmax_t value) noexcept -> ::fast_io::u8string {
    char8_t digits[::std::numeric_limits<::std::uintmax_t>::digits10 + 1];
    ::std::size_t count{};
    do {
        digits[count++] = static_cast<char8_t>(u8'0' + value % 10);
        value /= 10;
    } while (value != 0);
    ::fast_io::u8string result{};
    while (count != 0) {
        result.push_back(digits[--count]);
    }
    return result;
}

[[nodiscard]]
auto oracle_int2str(::std::intmax_t value) noexcept -> ::fast_io::u8string {
    auto magnitude = static_cast<::std::uintmax_t>(value);
    if (value < 0) {
        magnitude = ::std::uintmax_t{0} - magnitude;
    }
    ::fast_io::u8string result{};
    if (value < 0) {
        result.push_back(u8'-');
    }
    auto const digits = oracle_uint2str(magnitude);
    for (auto const ch : digits) {
        result.push_back(ch);
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
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{100}, end), end, u8"100")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{101}, end), end, u8"101")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{999}, end), end, u8"999")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{1000}, end), end, u8"1000")) {
        return false;
    }
    if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(::std::uintmax_t{12345}, end), end, u8"12345")) {
        return false;
    }
    if constexpr (::std::numeric_limits<::std::uintmax_t>::digits10 == 19) {
        if (!buffer_matches(::pltxt2htm::details::write_decimal_digits_backward(
                                (::std::numeric_limits<::std::uintmax_t>::max)(), end),
                            end, u8"18446744073709551615")) {
            return false;
        }
    }
    return true;
}
static_assert(test_write_decimal_digits_backward());

int main() noexcept {
    // size_t2str: boundary values around the single-digit / pair / loop transitions
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(0), ::fast_io::u8string_view{u8"0"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(1), ::fast_io::u8string_view{u8"1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(9), ::fast_io::u8string_view{u8"9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(10), ::fast_io::u8string_view{u8"10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(11), ::fast_io::u8string_view{u8"11"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(42), ::fast_io::u8string_view{u8"42"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(99), ::fast_io::u8string_view{u8"99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(100), ::fast_io::u8string_view{u8"100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(101), ::fast_io::u8string_view{u8"101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(110), ::fast_io::u8string_view{u8"110"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(999), ::fast_io::u8string_view{u8"999"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(1000), ::fast_io::u8string_view{u8"1000"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(12345), ::fast_io::u8string_view{u8"12345"});
    }

    // ptrdiff_t2str: sign handling around the same transitions
    {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(0), ::fast_io::u8string_view{u8"0"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(1), ::fast_io::u8string_view{u8"1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-1), ::fast_io::u8string_view{u8"-1"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(9), ::fast_io::u8string_view{u8"9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-9), ::fast_io::u8string_view{u8"-9"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(10), ::fast_io::u8string_view{u8"10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-10), ::fast_io::u8string_view{u8"-10"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(99), ::fast_io::u8string_view{u8"99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-99), ::fast_io::u8string_view{u8"-99"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(100), ::fast_io::u8string_view{u8"100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-100), ::fast_io::u8string_view{u8"-100"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(101), ::fast_io::u8string_view{u8"101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-101), ::fast_io::u8string_view{u8"-101"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(12345), ::fast_io::u8string_view{u8"12345"});
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(-12345), ::fast_io::u8string_view{u8"-12345"});
    }

    // extremes, including the PTRDIFF_MIN path that relies on unsigned negation
    {
        if constexpr (sizeof(::std::size_t) == 8) {
            pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str((::std::numeric_limits<::std::size_t>::max)()),
                                        ::fast_io::u8string_view{u8"18446744073709551615"});
            pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str((::std::numeric_limits<::std::ptrdiff_t>::max)()),
                                        ::fast_io::u8string_view{u8"9223372036854775807"});
            pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str((::std::numeric_limits<::std::ptrdiff_t>::min)()),
                                        ::fast_io::u8string_view{u8"-9223372036854775808"});
        }
        else {
            pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str((::std::numeric_limits<::std::size_t>::max)()),
                                        ::fast_io::u8string_view{u8"4294967295"});
            pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str((::std::numeric_limits<::std::ptrdiff_t>::max)()),
                                        ::fast_io::u8string_view{u8"2147483647"});
            pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str((::std::numeric_limits<::std::ptrdiff_t>::min)()),
                                        ::fast_io::u8string_view{u8"-2147483648"});
        }
    }

    // exhaustive cross-check against the pre-refactor algorithm
    for (::std::size_t i{}; i <= 100000; ++i) {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::size_t2str(i), oracle_uint2str(i));
    }
    for (::std::ptrdiff_t i{-100000}; i <= 100000; ++i) {
        pltxt2htm_test_assert_equal(::pltxt2htm::details::ptrdiff_t2str(i), oracle_int2str(i));
    }

    return 0;
}
