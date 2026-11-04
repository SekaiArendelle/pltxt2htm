#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>

#include <pltxt2htm/container/array.hh>
#include <pltxt2htm/container/deque.hh>
#include <pltxt2htm/container/span.hh>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/vector.hh>

#include "doctest_config.hh"
#include "details/concepts.hh"

using IntSpan = ::pltxt2htm::container::Span<int>;
using StaticIntSpan = ::pltxt2htm::container::Span<int, 3>;
using ConstIntSpan = ::pltxt2htm::container::Span<int const>;
using U8Span = ::pltxt2htm::container::Span<char8_t const>;

struct SpanTestBase {
    int base_value;
};

struct SpanTestDerived : SpanTestBase {
    int derived_value;
};

static_assert(::pltxt2htm::container::dynamic_extent == ::std::dynamic_extent);
static_assert(::std::is_trivially_copyable_v<IntSpan>);
static_assert(::std::is_standard_layout_v<IntSpan>);
static_assert(::std::same_as<IntSpan::element_type, int>);
static_assert(::std::same_as<IntSpan::value_type, int>);
static_assert(::std::same_as<IntSpan::pointer, int*>);
static_assert(::std::same_as<IntSpan::iterator, int*>);
static_assert(::std::same_as<IntSpan::const_iterator, int const*>);
static_assert(::std::same_as<ConstIntSpan::element_type, int const>);
static_assert(::std::same_as<ConstIntSpan::value_type, int>);
static_assert(::std::same_as<ConstIntSpan::reference, int const&>);
static_assert(IntSpan::extent == ::pltxt2htm::container::dynamic_extent);
static_assert(StaticIntSpan::extent == 3);
static_assert(::pltxt2htm::container::is_span<IntSpan>);
static_assert(::pltxt2htm::container::is_span<StaticIntSpan const&>);
static_assert(!::pltxt2htm::container::is_span<int>);
static_assert(!::pltxt2htm::container::is_span<int*>);
// A derived-to-base element conversion would make end() and pointer arithmetic
// over a Derived[] array invalid, so no constructor may accept it.
static_assert(
    !::std::is_constructible_v<::pltxt2htm::container::Span<SpanTestBase>, SpanTestDerived*, SpanTestDerived*>);
static_assert(!::std::is_constructible_v<::pltxt2htm::container::Span<SpanTestBase>,
                                         ::pltxt2htm::container::Array<SpanTestDerived, 2>&>);
static_assert(!::std::is_constructible_v<::pltxt2htm::container::Span<SpanTestBase>,
                                         ::pltxt2htm::container::Vector<SpanTestDerived>&>);
static_assert(!::std::is_constructible_v<::pltxt2htm::container::Span<SpanTestBase>, SpanTestDerived (&)[2]>);
static_assert(::std::ranges::contiguous_range<IntSpan&>);
static_assert(::std::ranges::sized_range<IntSpan&>);
static_assert(::std::ranges::contiguous_range<IntSpan const&>);
static_assert(::std::equality_comparable<IntSpan>);

using DeducedArraySpan = decltype(::pltxt2htm::container::Span{::std::declval<int (&)[3]>()});
using DeducedArrayAndSizeSpan = decltype(::pltxt2htm::container::Span{::std::declval<int (&)[4]>(), 4});
using DeducedIteratorSpan = decltype(::pltxt2htm::container::Span{::std::declval<int*>(), ::std::declval<int*>()});
using DeducedPointerSizeSpan =
    decltype(::pltxt2htm::container::Span{::std::declval<int*>(), ::std::declval<::std::size_t>()});
using DeducedRangeSpan = decltype(::pltxt2htm::container::Span{::std::declval<::pltxt2htm::container::Vector<int>&>()});
using DeducedConstRangeSpan =
    decltype(::pltxt2htm::container::Span{::std::declval<::pltxt2htm::container::Vector<int> const&>()});

static_assert(::std::same_as<DeducedArraySpan, StaticIntSpan>);
static_assert(::std::same_as<DeducedArrayAndSizeSpan, IntSpan>);
static_assert(::std::same_as<DeducedIteratorSpan, IntSpan>);
static_assert(::std::same_as<DeducedPointerSizeSpan, IntSpan>);
static_assert(::std::same_as<DeducedRangeSpan, IntSpan>);
static_assert(::std::same_as<DeducedConstRangeSpan, ConstIntSpan>);
static_assert(::std::is_default_constructible_v<IntSpan>);
static_assert(::std::is_default_constructible_v<::pltxt2htm::container::Span<int, 0>>);
static_assert(!::std::is_default_constructible_v<StaticIntSpan>);
static_assert(!can_call_empty<IntSpan>);
static_assert(can_call_is_empty<IntSpan>);
static_assert(::std::is_constructible_v<IntSpan, int*, ::std::size_t>);
static_assert(::std::is_constructible_v<IntSpan, int*, int*>);
static_assert(::std::is_constructible_v<IntSpan, int (&)[4]>);
static_assert(::std::is_constructible_v<StaticIntSpan, int (&)[3]>);
static_assert(!::std::is_constructible_v<StaticIntSpan, int (&)[4]>);
static_assert(::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Array<int, 3>&>);
static_assert(!::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Array<int, 3>>);
static_assert(::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Vector<int>&>);
static_assert(!::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Vector<int> const&>);
static_assert(::std::is_constructible_v<ConstIntSpan, ::pltxt2htm::container::Vector<int> const&>);
static_assert(::std::is_constructible_v<U8Span, ::pltxt2htm::container::U8String&>);
static_assert(!::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Deque<int>&>);
static_assert(::std::is_constructible_v<ConstIntSpan, IntSpan>);
static_assert(::std::is_convertible_v<IntSpan, ConstIntSpan>);
static_assert(::std::is_convertible_v<StaticIntSpan, IntSpan>);
static_assert(!::std::is_convertible_v<IntSpan, StaticIntSpan>);
static_assert(::std::is_constructible_v<StaticIntSpan, IntSpan>);
static_assert(!::std::is_constructible_v<StaticIntSpan, ::pltxt2htm::container::Span<int, 4>>);
static_assert(!::std::is_constructible_v<IntSpan, ::pltxt2htm::container::Span<long>>);

TEST_SUITE("container_span") {
    consteval auto test_constexpr_span() noexcept -> bool {
        IntSpan const empty{};
        if (!empty.is_empty() || empty.size() != 0 || empty.data() != nullptr || empty.begin() != empty.end() ||
            empty.size_bytes() != 0) {
            return false;
        }

        if (!empty.subspan(0).is_empty() || !empty.first(0).is_empty() || !empty.last(0).is_empty()) {
            return false;
        }

        int three_values[]{1, 2, 3};
        StaticIntSpan const static_view{three_values};
        if (static_view.data() != three_values || static_view.size() != 3 ||
            static_view.size_bytes() != 3 * sizeof(int) || static_view[1] != 2 || static_view.front() != 1 ||
            static_view.back() != 3) {
            return false;
        }

        int values[]{1, 2, 3, 4};
        IntSpan const view{values, 4};
        if (view.size() != 4 || view.end() - view.begin() != 4) {
            return false;
        }

        IntSpan const from_iterators{values + 1, values + 3};
        if (from_iterators.size() != 2 || from_iterators.data() != values + 1 || from_iterators[0] != 2 ||
            from_iterators[1] != 3) {
            return false;
        }

        StaticIntSpan const from_dynamic{view.first(3)};
        if (from_dynamic.size() != 3 || from_dynamic.data() != values || from_dynamic.back() != 3) {
            return false;
        }

        // Const-qualifying the view does not make the referenced elements read-only.
        view[0] = 10;
        if (values[0] != 10) {
            return false;
        }

        auto const middle = view.subspan(1, 2);
        if (middle.size() != 2 || middle.data() != values + 1 || middle[0] != 2 || middle[1] != 3) {
            return false;
        }

        auto const tail = view.subspan(2);
        if (tail.size() != 2 || tail.data() != values + 2) {
            return false;
        }

        auto const head = view.first(2);
        auto const last_two = view.last(2);
        if (head.size() != 2 || head.data() != values || last_two.size() != 2 || last_two.data() != values + 2) {
            return false;
        }

        ConstIntSpan const const_view{view};
        if (const_view.data() != values || const_view.size() != 4) {
            return false;
        }

        IntSpan const same{values, 4};
        auto const reversed = view.rbegin();
        if (!(view == same) || view != same || view == view.first(2) || reversed == view.rend() || *reversed != 4) {
            return false;
        }

        if (view.cbegin() != view.begin() || view.cend() != view.end() || view.crbegin() == view.crend() ||
            *view.crbegin() != 4) {
            return false;
        }

        auto const empty_tail = view.subspan(4);
        auto const empty_tail_zero = view.subspan(4, 0);
        auto const empty_head = view.first(0);
        auto const empty_last = view.last(0);
        if (!empty_tail.is_empty() || !empty_tail_zero.is_empty() || !empty_head.is_empty() || !empty_last.is_empty() ||
            empty_head.data() != values || empty_tail.data() != values + 4 || empty_last.data() != values + 4) {
            return false;
        }

        ::pltxt2htm::container::Span<int, 3> const first_three{three_values};
        ::pltxt2htm::container::Span<int, 4> const first_four{values};
        if (first_three == first_four) {
            return false;
        }

        ::pltxt2htm::container::Array<int, 3> array_values{{7, 8, 9}};
        IntSpan const from_array{array_values};
        if (from_array.data() != array_values.data() || from_array.size() != 3 || from_array[2] != 9) {
            return false;
        }

        U8Span const literal{u8"abc"};
        if (literal.size() != 4 || literal[0] != u8'a' || literal[3] != char8_t{0}) {
            return false;
        }

        int const const_values[]{5, 6};
        ConstIntSpan const read_only{const_values};
        return read_only.size() == 2 && read_only.front() == 5 && read_only.back() == 6;
    }

    static_assert(test_constexpr_span());

    TEST_CASE("construct-from-vector") {
        ::pltxt2htm::container::Vector<int> values{};
        values.push_back(1);
        values.push_back(2);
        values.push_back(3);

        IntSpan const view{values};
        CHECK(view.data() == values.data());
        CHECK(view.size() == values.size());
        CHECK(view[0] == 1);
        CHECK(view.back() == 3);

        view[1] = 20;
        CHECK(values[1] == 20);
    }

    TEST_CASE("construct-from-u8-string") {
        ::pltxt2htm::container::U8String text{u8"pltxt2htm"};
        U8Span const view{text};
        CHECK(view.data() == text.data());
        CHECK(view.size() == text.size());
        CHECK(view.size_bytes() == text.size() * sizeof(char8_t));
        CHECK(view[0] == u8'p');
        CHECK(view.back() == u8'm');
    }

    TEST_CASE("static-extent-view-from-array") {
        int values[]{4, 5, 6};
        StaticIntSpan const view{values};
        CHECK(view.size() == 3);
        CHECK(view.front() == 4);
        CHECK(view.back() == 6);
        CHECK(view.subspan(1).size() == 2);
    }

    TEST_CASE("copy-and-convert") {
        int values[]{7, 8, 9};
        StaticIntSpan const source{values};
        IntSpan const copy{source};
        ConstIntSpan const read_only{copy};
        CHECK(copy.data() == source.data());
        CHECK(read_only.data() == source.data());
        CHECK((copy == source));
        CHECK((read_only == copy));
    }
}