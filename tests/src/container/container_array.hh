#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <utility>
#include <pltxt2htm/container/array.hh>

#include "doctest_config.hh"
#include "details/concepts.hh"

template<::std::size_t extent>
concept can_form_array = requires { typename ::pltxt2htm::container::Array<int, extent>; };

using IntArray = ::pltxt2htm::container::Array<int, 3>;

static_assert(can_form_array<1>);
static_assert(!can_form_array<0>);
static_assert(::std::is_aggregate_v<IntArray>);
static_assert(::std::is_standard_layout_v<IntArray>);
static_assert(::std::is_trivially_copyable_v<IntArray>);
static_assert(!can_call_empty<IntArray>);
static_assert(!can_call_is_empty<IntArray>);
static_assert(requires(IntArray& values, IntArray const& const_values, ::std::size_t position) {
    values[position];
    values[position] = 1;
    const_values[position];
});

template<typename T>
concept can_compare_array_with = requires(::pltxt2htm::container::Array<T, 1> const& left,
                                          ::pltxt2htm::container::Array<T, 2> const& right) { left == right; };

template<typename T>
concept can_three_way_compare_array_with =
    requires(::pltxt2htm::container::Array<T, 1> const& left, ::pltxt2htm::container::Array<T, 2> const& right) {
        left <=> right;
    };

template<typename T>
concept nothrow_comparable_array_element = noexcept(::std::declval<::pltxt2htm::container::Array<T, 1> const&>() ==
                                                    ::std::declval<::pltxt2htm::container::Array<T, 2> const&>());

template<typename T>
concept nothrow_three_way_comparable_array_element =
    noexcept(::std::declval<::pltxt2htm::container::Array<T, 1> const&>() <=>
             ::std::declval<::pltxt2htm::container::Array<T, 2> const&>());

struct ArrayWithoutEquality {};

struct ArrayThrowingEquality {
    friend auto operator==(ArrayThrowingEquality const&, ArrayThrowingEquality const&) -> bool;
};

struct ArrayThrowingBooleanEquality {
    struct Result {
        operator bool() const;
    };

    friend constexpr auto operator==(ArrayThrowingBooleanEquality const&, ArrayThrowingBooleanEquality const&) noexcept
        -> Result {
        return {};
    }

    friend constexpr auto operator!=(ArrayThrowingBooleanEquality const&, ArrayThrowingBooleanEquality const&) noexcept
        -> Result {
        return {};
    }
};

struct ArrayThrowingThreeWay {
    friend constexpr auto operator==(ArrayThrowingThreeWay const&, ArrayThrowingThreeWay const&) noexcept -> bool {
        return true;
    }

    friend auto operator<=>(ArrayThrowingThreeWay const&, ArrayThrowingThreeWay const&) -> ::std::strong_ordering;
};

struct ArrayThrowingEqualityNothrowThreeWay {
    friend auto operator==(ArrayThrowingEqualityNothrowThreeWay const&, ArrayThrowingEqualityNothrowThreeWay const&)
        -> bool;

    friend constexpr auto operator<=>(ArrayThrowingEqualityNothrowThreeWay const&,
                                      ArrayThrowingEqualityNothrowThreeWay const&) noexcept -> ::std::strong_ordering {
        return ::std::strong_ordering::equal;
    }
};

static_assert(!can_compare_array_with<ArrayWithoutEquality>);
static_assert(!can_three_way_compare_array_with<ArrayWithoutEquality>);
static_assert(can_compare_array_with<int>);
static_assert(can_three_way_compare_array_with<int>);
static_assert(nothrow_comparable_array_element<int>);
static_assert(nothrow_three_way_comparable_array_element<int>);
static_assert(can_compare_array_with<ArrayThrowingEquality>);
static_assert(!nothrow_comparable_array_element<ArrayThrowingEquality>);
static_assert(can_compare_array_with<ArrayThrowingBooleanEquality>);
static_assert(!nothrow_comparable_array_element<ArrayThrowingBooleanEquality>);
static_assert(can_compare_array_with<ArrayThrowingThreeWay>);
static_assert(nothrow_comparable_array_element<ArrayThrowingThreeWay>);
static_assert(can_three_way_compare_array_with<ArrayThrowingThreeWay>);
static_assert(!nothrow_three_way_comparable_array_element<ArrayThrowingThreeWay>);
static_assert(can_compare_array_with<ArrayThrowingEqualityNothrowThreeWay>);
static_assert(!nothrow_comparable_array_element<ArrayThrowingEqualityNothrowThreeWay>);
static_assert(can_three_way_compare_array_with<ArrayThrowingEqualityNothrowThreeWay>);
static_assert(nothrow_three_way_comparable_array_element<ArrayThrowingEqualityNothrowThreeWay>);
static_assert(noexcept(::std::declval<IntArray const&>() == ::std::declval<IntArray const&>()));
static_assert(noexcept(::std::declval<IntArray const&>() <=> ::std::declval<IntArray const&>()));

TEST_SUITE("container_array") {
    consteval auto test_constexpr_array() noexcept -> bool {
        auto values = ::pltxt2htm::container::Array{1, 2, 3};
        static_assert(::std::same_as<decltype(values), IntArray>);
        if (values.size() != 3 || values.size_bytes() != 3 * sizeof(int) || values.max_size() != 3 ||
            values.max_size_bytes() != 3 * sizeof(int)) {
            return false;
        }
        if (values.data() != values.begin() || values.end() - values.begin() != 3 ||
            values.cend() - values.cbegin() != 3 || values.front() != 1 || values.back() != 3 ||
            *values.rbegin() != 3 || *values.crbegin() != 3) {
            return false;
        }

        values[1] = 4;
        if (values[1] != 4 || values[2] != 3) {
            return false;
        }

        auto other = ::pltxt2htm::container::Array{7, 8, 9};
        values.swap(other);
        if (values != ::pltxt2htm::container::Array{7, 8, 9} || other != ::pltxt2htm::container::Array{1, 4, 3}) {
            return false;
        }
        values.fill(5);
        if (values != ::pltxt2htm::container::Array{5, 5, 5}) {
            return false;
        }

        int source[]{10, 11, 12};
        auto copied = ::pltxt2htm::container::to_array(source);
        if (copied != ::pltxt2htm::container::Array{10, 11, 12}) {
            return false;
        }
        ::pltxt2htm::container::swap(copied, other);
        return copied == ::pltxt2htm::container::Array{1, 4, 3} && other == ::pltxt2htm::container::Array{10, 11, 12} &&
               (::pltxt2htm::container::Array{1, 2} <=> ::pltxt2htm::container::Array{1, 3}) ==
                   ::std::strong_ordering::less &&
               ::pltxt2htm::container::Array{1, 2} != ::pltxt2htm::container::Array{1, 2, 3};
    }

    static_assert(test_constexpr_array());

    TEST_CASE("heterogeneous-element-access") {
        auto const strings = ::pltxt2htm::container::Array{"first", "second", nullptr};
        static_assert(
            ::std::same_as<::std::remove_cvref_t<decltype(strings)>, ::pltxt2htm::container::Array<char const*, 3>>);
        CHECK(::std::string_view{strings[1]} == "second");
    }
}
