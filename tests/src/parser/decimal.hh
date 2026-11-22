#pragma once

#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include <pltxt2htm/details/parser/decimal.hh>

#include "doctest_config.hh"
#include "format/details/support.hh"

namespace pltxt2htm_test::parser_decimal_details {

using Result = ::pltxt2htm::details::TryParseDoubleDecimalValueResult;

[[nodiscard]] inline auto parse(::std::string const& text) -> ::pltxt2htm::container::Optional<Result> {
    return ::pltxt2htm::details::try_parse_double_decimal_value(::pltxt2htm_test::format_details::as_view(text));
}

/// Expand a scientific decimal (e.g. "1.25e+02") into plain fixed-point notation.
[[nodiscard]] inline auto expand_scientific(::std::string const& text) -> ::std::string {
    auto const exponent_position = text.find_first_of("eE");
    ::std::string mantissa = text.substr(0, exponent_position);
    int exponent{};
    if (exponent_position != ::std::string::npos) {
        exponent = ::std::atoi(text.c_str() + exponent_position + 1);
    }
    auto const dot = mantissa.find('.');
    ::std::string integer_part = dot == ::std::string::npos ? mantissa : mantissa.substr(0, dot);
    ::std::string fraction_part = dot == ::std::string::npos ? ::std::string{} : mantissa.substr(dot + 1);
    if (fraction_part.empty() == false) {
        exponent -= static_cast<int>(fraction_part.size());
    }
    ::std::string digits = integer_part + fraction_part;
    auto const first_nonzero = digits.find_first_not_of('0');
    if (first_nonzero == ::std::string::npos) {
        return "0";
    }
    digits.erase(0, first_nonzero);
    if (exponent >= 0) {
        digits.append(static_cast<::std::size_t>(exponent), '0');
        return digits;
    }
    auto const fraction_digits = static_cast<::std::size_t>(-exponent);
    if (fraction_digits < digits.size()) {
        digits.insert(digits.size() - fraction_digits, 1, '.');
        return digits;
    }
    return "0." + ::std::string(fraction_digits - digits.size(), '0') + digits;
}

[[nodiscard]] inline auto oracle_value(::std::string const& text) -> double {
    double value{};
    auto const result = ::std::from_chars(text.data(), text.data() + text.size(), value);
    REQUIRE(result.ec == ::std::errc{});
    REQUIRE(result.ptr == text.data() + text.size());
    return value;
}

} // namespace pltxt2htm_test::parser_decimal_details

TEST_SUITE("parser_decimal") {
    TEST_CASE("syntax") {
        using ::pltxt2htm_test::parser_decimal_details::parse;
        CHECK_FALSE(parse("").has_value());
        CHECK_FALSE(parse(".").has_value());
        CHECK_FALSE(parse(".5").has_value());
        CHECK_FALSE(parse("5.").has_value());
        CHECK_FALSE(parse(" 5").has_value());
        CHECK_FALSE(parse("+5").has_value());
        CHECK_FALSE(parse("-5").has_value());

        auto parsed = parse("0");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 0);
        CHECK(parsed.value().end == 1);

        parsed = parse("00");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 0);
        CHECK(parsed.value().end == 2);

        parsed = parse("12.5");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 12.5);
        CHECK(parsed.value().end == 4);

        parsed = parse("12.5x");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 12.5);
        CHECK(parsed.value().end == 4);

        parsed = parse("1.2.3");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 1.2);
        CHECK(parsed.value().end == 3);

        // `e` is not an exponent marker here; the value ends before it.
        parsed = parse("1e5");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 1);
        CHECK(parsed.value().end == 1);
    }

    TEST_CASE("domain") {
        using ::pltxt2htm::details::max_font_size_value;
        using ::pltxt2htm::details::max_size_tag_value;
        using ::pltxt2htm::details::min_font_size_value;
        using ::pltxt2htm_test::parser_decimal_details::parse;

        auto parsed = parse("0.000000000000000001");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == min_font_size_value);

        CHECK_FALSE(parse("0.0000000000000000001").has_value());
        CHECK_FALSE(parse("0.00000000000000000001").has_value());

        parsed = parse("0.0000000000000000000");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == 0);

        parsed = parse("18446744073709549568");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == max_size_tag_value);

        CHECK_FALSE(parse("18446744073709551615").has_value());
        CHECK_FALSE(parse("18446744073709551616").has_value());
        CHECK_FALSE(parse("18446744073709551616.5").has_value());
        CHECK_FALSE(parse("99999999999999999999999999").has_value());
        // The fractional part is far below the double spacing here, so the value rounds down to
        // the largest in-domain double instead of crossing the exclusive upper bound.
        parsed = parse("18446744073709549568.1");
        REQUIRE(parsed.has_value());
        CHECK(parsed.value().value == max_size_tag_value);
        CHECK(max_font_size_value > max_size_tag_value);
    }

    TEST_CASE("exact-rounding") {
        using ::pltxt2htm_test::parser_decimal_details::oracle_value;
        using ::pltxt2htm_test::parser_decimal_details::parse;

        for (char const* const text :
             {"0.010924504404784173", "175.002439047", "6502438387485576253.18963", "0.0000000000000010855935740152218",
              "0.00000000000011875472369152510", "10184670014847297.72941", "34612617606248738.81690909232107",
              "214748364.75", "0.1", "0.33333333333333331483", "9223372036854775808", "18446744073709549568",
              // Exact double midpoints with a 39-digit fraction: the nonzero tail must break the
              // tie upward instead of rounding to even.
              "9007199254740993.000000000000000000000000000000000000001",
              "4611686018427388416.000000000000000000000000000000000000005",
              "14044493974566121.0000000000000000000000000000000000000005",
              // The same midpoint with an all-zero tail keeps ties-to-even.
              "9007199254740993.000000000000000000000000000000000000000"}) {
            auto const parsed = parse(text);
            REQUIRE(parsed.has_value());
            CHECK(parsed.value().value == oracle_value(text));
        }
    }

    TEST_CASE("generated-differential") {
        using ::pltxt2htm_test::parser_decimal_details::parse;

        ::std::uint64_t state{0x243f6a8885a308d3ull};
        for (::std::size_t i{}; i < 2000; ++i) {
            state = state * 6364136223846793005ull + 1442695040888963407ull;
            ::std::uint64_t const integer_part = i % 4 == 0 ? state % 1000 : state % 18446744073709551615ull;
            char integer_buffer[32];
            auto const integer_result =
                ::std::to_chars(integer_buffer, integer_buffer + sizeof(integer_buffer), integer_part);
            ::std::string text{integer_buffer, integer_result.ptr};
            if (i % 5 != 0) {
                text.push_back('.');
                // Occasionally exceed the 38-digit exact scale so the dropped-tail sticky path
                // is exercised as well.
                auto const digits = i % 7 == 0 ? 39 + static_cast<::std::size_t>(state % 22)
                                               : 1 + static_cast<::std::size_t>(state % 20);
                for (::std::size_t d{}; d < digits; ++d) {
                    text.push_back(static_cast<char>('0' + static_cast<int>((state >> (d + 1)) % 10)));
                }
            }
            double oracle{};
            auto const oracle_result = ::std::from_chars(text.data(), text.data() + text.size(), oracle);
            bool const oracle_ok = oracle_result.ec == ::std::errc{} && oracle_result.ptr == text.data() + text.size();
            auto const parsed = parse(text);
            if (oracle_ok == false) {
                CHECK_FALSE(parsed.has_value());
                continue;
            }
            bool const in_domain = oracle >= ::pltxt2htm::details::min_font_size_value &&
                                   oracle < ::pltxt2htm::details::max_font_size_value;
            INFO("input: ", text.c_str());
            if (in_domain) {
                REQUIRE(parsed.has_value());
                CHECK(parsed.value().value == oracle);
            }
            else {
                CHECK_FALSE(parsed.has_value());
            }
        }
    }

    TEST_CASE("seventeen-digit-recovery") {
        using ::pltxt2htm_test::parser_decimal_details::expand_scientific;
        using ::pltxt2htm_test::parser_decimal_details::parse;

        ::std::uint64_t state{0x9e3779b97f4a7c15ull};
        ::std::size_t checked{};
        for (::std::size_t i{}; i < 1000; ++i) {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            double value{};
            ::std::memcpy(&value, &state, sizeof value);
            if (::std::isfinite(value) == false) {
                continue;
            }
            value = ::std::fabs(value);
            if (value < ::pltxt2htm::details::min_font_size_value ||
                value >= ::pltxt2htm::details::max_font_size_value) {
                continue;
            }
            char buffer[64];
            auto const result =
                ::std::to_chars(buffer, buffer + sizeof(buffer), value, ::std::chars_format::scientific, 16);
            REQUIRE(result.ec == ::std::errc{});
            auto const fixed = expand_scientific(::std::string{buffer, result.ptr});
            auto const parsed = parse(fixed);
            INFO("input: ", fixed.c_str());
            REQUIRE(parsed.has_value());
            CHECK(parsed.value().value == value);
            ++checked;
        }
        CHECK(checked > 0);
    }
}