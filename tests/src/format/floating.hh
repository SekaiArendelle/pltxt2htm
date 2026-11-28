#pragma once

#include <bit>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>

#include <pltxt2htm/format/floating.hh>

#include "doctest_config.hh"
#include "format/details/support.hh"

namespace pltxt2htm_test::format_floating_details {

/**
 * @brief Format through std::to_chars(chars_format::fixed), the semantic oracle.
 * @details The library promises the same text as std::to_chars for finite
 *          values, so this is an independent implementation to compare against.
 */
[[nodiscard]] inline auto std_fixed(double const value) -> ::std::string {
    char buffer[512];
    auto const result = ::std::to_chars(buffer, buffer + sizeof(buffer), value, ::std::chars_format::fixed);
    CHECK(result.ec == ::std::errc{});
    return ::std::string{buffer, result.ptr};
}

[[nodiscard]]
inline auto format_to_string(double const value) -> ::pltxt2htm::container::U8String {
    ::pltxt2htm::container::U8String result{};
    ::pltxt2htm::format::append_decimal(result, value);
    return result;
}

/// Whether the library produces exactly the std::to_chars(fixed) text.
inline void check_matches_std(double const value) {
    auto const text = format_to_string(value);
    CHECK(::pltxt2htm_test::format_details::to_std_string(text) == std_fixed(value));
}

inline void check_font_size_syntax_round_trip(::pltxt2htm::container::U8StringView const decimal,
                                              ::pltxt2htm::container::U8StringView const expected_decimal) {
    auto pltext = ::pltxt2htm::container::U8String{u8"<span style=\"font-size:"};
    pltext.append(decimal);
    pltext.append(u8"px\">text</span>");

    auto html =
        ::pltxt2htm_test::pltxt2fixedadv_htmld(::pltxt2htm::container::U8StringView{pltext.data(), pltext.size()});
    auto expected = ::pltxt2htm::container::U8String{u8"<span style=\"font-size:"};
    expected.append(expected_decimal);
    expected.append(u8"px;\">text</span>");
    CHECK(html == expected);
}

} // namespace pltxt2htm_test::format_floating_details

TEST_SUITE("format_floating") {
    TEST_CASE("fixed-point-values") {
        using ::pltxt2htm_test::format_floating_details::format_to_string;

        CHECK(format_to_string(0.0) == u8"0");
        CHECK(format_to_string(-0.0) == u8"-0");
        CHECK(format_to_string(12.0) == u8"12");
        CHECK(format_to_string(12.5) == u8"12.5");
        CHECK(format_to_string(-1.5) == u8"-1.5");
        CHECK(format_to_string(0.1) == u8"0.1");
        CHECK(format_to_string(1.0 / 3.0) == u8"0.3333333333333333");
        CHECK(format_to_string(1e-19) == u8"0.0000000000000000001");
        CHECK(format_to_string(175.002439047) == u8"175.002439047");
        CHECK(format_to_string(214748364.75) == u8"214748364.75");
        CHECK(format_to_string(123456789.25) == u8"123456789.25");
        // Integral values keep their exact integer digits.
        CHECK(format_to_string(7173733806442602179.0) == u8"7173733806442602496");
        CHECK(format_to_string(9223372036854775808.0) == u8"9223372036854775808");
        CHECK(format_to_string(18446744073709549568.0) == u8"18446744073709549568");
    }

    TEST_CASE("full-domain") {
        using ::pltxt2htm_test::format_floating_details::format_to_string;
        using ::std::numeric_limits;

        // Both ends of the domain are supported now; the limits only constrain
        // the parser, not the formatter.
        auto const smallest_normal = numeric_limits<double>::min();
        auto const largest = numeric_limits<double>::max();
        CHECK(format_to_string(smallest_normal).size() == 326);
        CHECK(format_to_string(-smallest_normal).size() == 327);
        CHECK(format_to_string(largest).size() == 309);
        CHECK(format_to_string(numeric_limits<double>::denorm_min()).size() == 326);
        CHECK(format_to_string(1e-320).size() == 322);
        CHECK(format_to_string(1e308).size() == 309);
    }

    TEST_CASE("large-integral-values") {
        // Integral values are emitted as their exact expansion, matching
        // std::to_chars, even when that needs more than 64 bits.
        using ::pltxt2htm_test::format_floating_details::check_matches_std;

        for (double const value :
             {0.0, -0.0, 1.0, 100.0, 18446744073709551616.0, ::std::ldexp(1.0, 100), ::std::ldexp(1.0, 200), 1e300,
              12345678901234567890.0, ::std::numeric_limits<double>::max()}) {
            check_matches_std(value);
        }
    }

    TEST_CASE("non-finite-and-negative-zero") {
        using ::pltxt2htm_test::format_floating_details::format_to_string;
        using ::std::numeric_limits;

        CHECK(format_to_string(numeric_limits<double>::quiet_NaN()) == u8"nan");
        CHECK(format_to_string(-numeric_limits<double>::quiet_NaN()) == u8"-nan");
        CHECK(format_to_string(numeric_limits<double>::infinity()) == u8"inf");
        CHECK(format_to_string(-numeric_limits<double>::infinity()) == u8"-inf");
        CHECK(format_to_string(-0.0) == u8"-0");
    }

    TEST_CASE("append-decimal") {
        ::pltxt2htm::container::U8String out{};
        ::pltxt2htm::format::append_decimal(out, 12.5);
        ::pltxt2htm::format::append_decimal(out, 7);
        ::pltxt2htm::format::append_decimal(out, -3);
        ::pltxt2htm::format::append_decimal(out, 0.0);
        ::pltxt2htm::format::append_decimal(out, -0.0);
        CHECK(out == u8"12.57-30-0");
    }

    TEST_CASE("append-decimal-out-of-font-size-domain") {
        // The formatter is no longer clamped: every double is safe to append.
        using ::pltxt2htm_test::format_floating_details::std_fixed;
        using ::std::numeric_limits;

        ::pltxt2htm::container::U8String out{};
        ::pltxt2htm::format::append_decimal(out, 1e-47);
        ::pltxt2htm::format::append_decimal(out, 1e20);
        ::pltxt2htm::format::append_decimal(out, numeric_limits<double>::infinity());
        ::pltxt2htm::format::append_decimal(out, -numeric_limits<double>::infinity());
        ::pltxt2htm::format::append_decimal(out, numeric_limits<double>::quiet_NaN());

        ::pltxt2htm::container::U8String expected{};
        for (::std::string const& part :
             {std_fixed(1e-47), std_fixed(1e20), ::std::string{"inf"}, ::std::string{"-inf"}, ::std::string{"nan"}}) {
            expected.append(reinterpret_cast<char8_t const*>(part.data()),
                            reinterpret_cast<char8_t const*>(part.data()) + part.size());
        }
        CHECK(out == expected);
    }

    TEST_CASE("matches-std-to-chars-fixed") {
        using ::pltxt2htm_test::format_floating_details::check_matches_std;
        using ::std::isfinite;
        using ::std::ldexp;
        using ::std::nextafter;
        using ::std::pow;

        ::std::uint64_t state{0x9e3779b97f4a7c15ull};
        for (::std::size_t i{}; i < 4000; ++i) {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            double const value = ::std::bit_cast<double>(state);
            if (isfinite(value) == false) {
                continue;
            }
            check_matches_std(value);
        }

        // Every power of two and its neighbours, including subnormals.
        for (int exponent = -1074; exponent <= 1023; ++exponent) {
            auto const value = ldexp(1.0, exponent);
            check_matches_std(value);
            check_matches_std(nextafter(value, 0.0));
            if (isfinite(nextafter(value, ::std::numeric_limits<double>::infinity()))) {
                check_matches_std(nextafter(value, ::std::numeric_limits<double>::infinity()));
            }
        }

        // Every decimal power across the finite range and its neighbours.
        for (int exponent = -323; exponent <= 308; ++exponent) {
            auto const value = pow(10.0, exponent);
            check_matches_std(value);
            check_matches_std(nextafter(value, 0.0));
            if (isfinite(nextafter(value, ::std::numeric_limits<double>::infinity()))) {
                check_matches_std(nextafter(value, ::std::numeric_limits<double>::infinity()));
            }
        }
    }

    TEST_CASE("formatted-values-round-trip-through-font-size-syntax") {
        using ::pltxt2htm_test::format_floating_details::check_font_size_syntax_round_trip;
        using ::pltxt2htm_test::format_floating_details::format_to_string;

        ::std::uint64_t state{0x243f6a8885a308d3ull};
        ::std::size_t checked{};
        for (::std::size_t i{}; i < 2000; ++i) {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            double value{};
            if (i % 2 == 0) {
                value = static_cast<double>(state % 1000000000ull) / static_cast<double>(1 + state % 1000000ull);
            }
            else {
                value = ::std::ldexp(static_cast<double>(1 + state % 9000000ull), -static_cast<int>(state % 60));
            }
            if (value == 0 || value > 1000000.0) {
                continue;
            }
            auto const text = format_to_string(value);
            ::pltxt2htm::container::U8StringView const view{text.data(), text.size()};
            check_font_size_syntax_round_trip(view, view);
            ++checked;
        }
        CHECK(checked > 0);
    }

    TEST_CASE("generated-decimals-round-trip-through-font-size-syntax") {
        using ::pltxt2htm_test::format_details::as_view;
        using ::pltxt2htm_test::format_floating_details::check_font_size_syntax_round_trip;
        using ::pltxt2htm_test::format_floating_details::std_fixed;

        ::std::uint64_t state{0x243f6a8885a308d3ull};
        ::std::size_t checked{};
        for (::std::size_t i{}; i < 2000; ++i) {
            state = state * 6364136223846793005ull + 1442695040888963407ull;
            ::std::uint64_t const integer_part = state % 1000000;
            char integer_buffer[32];
            auto const integer_result =
                ::std::to_chars(integer_buffer, integer_buffer + sizeof(integer_buffer), integer_part);
            ::std::string text{integer_buffer, integer_result.ptr};
            if (i % 5 != 0) {
                text.push_back('.');
                // Long fractions exercise both the retained significand and the discarded-tail rounding path.
                auto const digits = 1 + static_cast<::std::size_t>(state % 40);
                for (::std::size_t d{}; d < digits; ++d) {
                    text.push_back(static_cast<char>('0' + static_cast<int>((state >> (d + 1)) % 10)));
                }
            }

            double oracle{};
            auto const oracle_result = ::std::from_chars(text.data(), text.data() + text.size(), oracle);
            REQUIRE(oracle_result.ec == ::std::errc{});
            REQUIRE(oracle_result.ptr == text.data() + text.size());
            if (oracle == 0 || oracle < 0.000000000000000001 || oracle > 1000000.0) {
                continue;
            }

            INFO("input: ", text.c_str());
            auto const expected = std_fixed(oracle);
            check_font_size_syntax_round_trip(as_view(text), as_view(expected));
            ++checked;
        }
        CHECK(checked > 0);
    }
}
