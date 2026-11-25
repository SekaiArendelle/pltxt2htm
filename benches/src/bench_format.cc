#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

#include <benchmark/benchmark.h>

#include <pltxt2htm/format/floating.hh>

// -------------------------------------------------------------------
// Fixed-point formatting: pltxt2htm::format::to_chars against
// std::to_chars(chars_format::fixed). Group 0 is typical font sizes, group 1
// is fractional values, and group 2 covers both ends of the binary64 domain.
// -------------------------------------------------------------------

namespace {

constexpr ::std::array<double, 8> font_size_values{0.5, 7.0, 12.0, 12.5, 16.0, 20.25, 175.002439047, 214748364.75};

constexpr ::std::array<double, 5> fractional_values{0.1, 1.0 / 3.0, 0.000024198805311133413, 1e-9, 1e-18};

constexpr ::std::array<double, 5> extreme_values{::std::numeric_limits<double>::denorm_min(),
                                                 ::std::numeric_limits<double>::min(), 1e20, 1e300,
                                                 ::std::numeric_limits<double>::max()};

[[nodiscard]] constexpr auto value_group(int const group) noexcept -> ::std::span<double const> {
    switch (group) {
    case 0: {
        return font_size_values;
    }
    case 1: {
        return fractional_values;
    }
    default: {
        return extreme_values;
    }
    }
}

struct FormatFixture : ::benchmark::Fixture {};

} // namespace

BENCHMARK_DEFINE_F(FormatFixture, PLTXT2HTM_ToChars)(benchmark::State& st) {
    char8_t buffer[::pltxt2htm::format::details::max_fixed_point_length];
    auto const values = value_group(static_cast<int>(st.range(0)));
    ::std::size_t index{};
    for (auto _ : st) {
        auto const value = values[index % values.size()];
        auto* const end = ::pltxt2htm::format::to_chars(buffer, buffer + sizeof(buffer), value);
        ::benchmark::DoNotOptimize(static_cast<::std::size_t>(end - buffer));
        ::benchmark::ClobberMemory();
        ++index;
    }
}

BENCHMARK_REGISTER_F(FormatFixture, PLTXT2HTM_ToChars)->Arg(0)->Arg(1)->Arg(2)->ArgName("group");

BENCHMARK_DEFINE_F(FormatFixture, StdToChars)(benchmark::State& st) {
    char buffer[512];
    auto const values = value_group(static_cast<int>(st.range(0)));
    ::std::size_t index{};
    for (auto _ : st) {
        auto const value = values[index % values.size()];
        auto const result = ::std::to_chars(buffer, buffer + sizeof(buffer), value, ::std::chars_format::fixed);
        ::benchmark::DoNotOptimize(static_cast<::std::size_t>(result.ptr - buffer));
        ::benchmark::ClobberMemory();
        ++index;
    }
}

BENCHMARK_REGISTER_F(FormatFixture, StdToChars)->Arg(0)->Arg(1)->Arg(2)->ArgName("group");