#pragma once

#include <array>
#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

#include <pltxt2htm/container/inplace_vector.hh>

#include "doctest_config.hh"
#include "details/concepts.hh"
#include "details/structs.hh"

using IntInplaceVector = ::pltxt2htm::container::InplaceVector<int, 8>;

template<typename Vector>
concept has_at = requires(Vector& vector) { vector.at(0); };

struct NonTrivialInplaceValue {
    ~NonTrivialInplaceValue() noexcept {
    }
};

struct TrivialWithoutDefaultConstructorInplace {
    TrivialWithoutDefaultConstructorInplace() = delete;
    int value;
};

struct TriviallyMovableOnlyInplace {
    TriviallyMovableOnlyInplace() = default;
    TriviallyMovableOnlyInplace(TriviallyMovableOnlyInplace const&) = delete;
    TriviallyMovableOnlyInplace(TriviallyMovableOnlyInplace&&) = default;
    auto operator=(this TriviallyMovableOnlyInplace&, TriviallyMovableOnlyInplace const&)
        -> TriviallyMovableOnlyInplace& = delete;
    auto operator=(this TriviallyMovableOnlyInplace&, TriviallyMovableOnlyInplace&&)
        -> TriviallyMovableOnlyInplace& = default;
    int value;
};

struct TrivialWithoutAssignmentInplace {
    auto operator=(this TrivialWithoutAssignmentInplace&, TrivialWithoutAssignmentInplace const&)
        -> TrivialWithoutAssignmentInplace& = delete;
    int value;
};

struct ImmobileInplace {
    ImmobileInplace() = delete;
    ImmobileInplace(ImmobileInplace const&) = delete;
    ImmobileInplace(ImmobileInplace&&) = delete;
    auto operator=(this ImmobileInplace&, ImmobileInplace const&) -> ImmobileInplace& = delete;
    auto operator=(this ImmobileInplace&, ImmobileInplace&&) -> ImmobileInplace& = delete;
};

struct PotentiallyThrowingComparisonInplace {
    int value;

    [[nodiscard]] friend constexpr auto operator==(PotentiallyThrowingComparisonInplace const& left,
                                                   PotentiallyThrowingComparisonInplace const& right) -> bool {
        return left.value == right.value;
    }

    [[nodiscard]] friend constexpr auto operator<=>(PotentiallyThrowingComparisonInplace const& left,
                                                    PotentiallyThrowingComparisonInplace const& right) noexcept(false) {
        return left.value <=> right.value;
    }
};

using ZeroInplaceVector = ::pltxt2htm::container::InplaceVector<NonTrivialInplaceValue, 0>;

static_assert(::std::same_as<IntInplaceVector::value_type, int>);
static_assert(::std::same_as<IntInplaceVector::iterator, int*>);
static_assert(::std::same_as<IntInplaceVector::const_iterator, int const*>);
static_assert(::std::ranges::contiguous_range<IntInplaceVector>);
static_assert(::std::ranges::sized_range<IntInplaceVector>);
static_assert(::std::default_initializable<IntInplaceVector>);
static_assert(can_subscript<IntInplaceVector>);
static_assert(can_subscript<IntInplaceVector const>);
static_assert(!has_at<IntInplaceVector>);
static_assert(::std::is_trivially_copy_constructible_v<IntInplaceVector>);
static_assert(::std::is_trivially_move_constructible_v<IntInplaceVector>);
static_assert(::std::is_trivially_copy_assignable_v<IntInplaceVector>);
static_assert(::std::is_trivially_move_assignable_v<IntInplaceVector>);
static_assert(::std::is_empty_v<ZeroInplaceVector>);
static_assert(::std::is_trivially_default_constructible_v<ZeroInplaceVector>);
static_assert(::std::is_trivially_copyable_v<ZeroInplaceVector>);
static_assert(::std::is_trivially_copyable_v<TrivialWithoutDefaultConstructorInplace>);
static_assert(
    ::std::default_initializable<::pltxt2htm::container::InplaceVector<TrivialWithoutDefaultConstructorInplace, 2>>);
static_assert(::std::is_trivially_move_constructible_v<TriviallyMovableOnlyInplace>);
static_assert(::std::is_trivially_move_assignable_v<TriviallyMovableOnlyInplace>);
static_assert(
    ::std::is_trivially_move_constructible_v<::pltxt2htm::container::InplaceVector<TriviallyMovableOnlyInplace, 2>>);
static_assert(
    ::std::is_trivially_move_assignable_v<::pltxt2htm::container::InplaceVector<TriviallyMovableOnlyInplace, 2>>);
static_assert(::std::is_trivially_default_constructible_v<TrivialWithoutAssignmentInplace>);
static_assert(::std::is_trivially_copyable_v<TrivialWithoutAssignmentInplace>);
static_assert(::std::default_initializable<::pltxt2htm::container::InplaceVector<TrivialWithoutAssignmentInplace, 2>>);
using ImmobileZeroInplaceVector = ::pltxt2htm::container::InplaceVector<ImmobileInplace, 0>;
static_assert(
    noexcept(::std::declval<ImmobileZeroInplaceVector&>().swap(::std::declval<ImmobileZeroInplaceVector&>())));
static_assert(noexcept(::pltxt2htm::container::swap(::std::declval<ImmobileZeroInplaceVector&>(),
                                                    ::std::declval<ImmobileZeroInplaceVector&>())));
static_assert(::std::same_as<decltype(::std::declval<IntInplaceVector&>().try_emplace_back(1)),
                             ::pltxt2htm::container::Optional<int&>>);
static_assert(::std::same_as<decltype(::std::declval<IntInplaceVector&>().emplace_back(1)), int&>);
static_assert(noexcept(::std::declval<IntInplaceVector const&>() <=> ::std::declval<IntInplaceVector const&>()));
using PotentiallyThrowingComparisonInplaceVector =
    ::pltxt2htm::container::InplaceVector<PotentiallyThrowingComparisonInplace, 1>;
using PotentiallyThrowingComparisonEmptyInplaceVector =
    ::pltxt2htm::container::InplaceVector<PotentiallyThrowingComparisonInplace, 0>;
static_assert(!noexcept(::std::declval<PotentiallyThrowingComparisonInplaceVector const&>() <=>
                        ::std::declval<PotentiallyThrowingComparisonInplaceVector const&>()));
static_assert(noexcept(::std::declval<PotentiallyThrowingComparisonEmptyInplaceVector const&>() <=>
                       ::std::declval<PotentiallyThrowingComparisonInplaceVector const&>()));
static_assert(noexcept(::std::declval<PotentiallyThrowingComparisonInplaceVector const&>() <=>
                       ::std::declval<PotentiallyThrowingComparisonEmptyInplaceVector const&>()));

using ThrowingBeginRange = ::pltxt2htm_test::ThrowingBeginRange;
using ThrowingEndRange = ::pltxt2htm_test::ThrowingEndRange;

// Range wrappers must include both begin() and end() in their noexcept
// specification; otherwise a throwing accessor would terminate.
static_assert(!noexcept(::std::declval<IntInplaceVector&>().append_range(::std::declval<ThrowingBeginRange&>())));
static_assert(!noexcept(::std::declval<IntInplaceVector&>().append_range(::std::declval<ThrowingEndRange&>())));
static_assert(!noexcept(::std::declval<IntInplaceVector&>().assign_range(::std::declval<ThrowingBeginRange&>())));
static_assert(!noexcept(::std::declval<IntInplaceVector&>().assign_range(::std::declval<ThrowingEndRange&>())));
static_assert(!noexcept(::std::declval<IntInplaceVector&>().insert_range(
    ::std::declval<IntInplaceVector::const_iterator>(), ::std::declval<ThrowingBeginRange&>())));
static_assert(!noexcept(::std::declval<IntInplaceVector&>().insert_range(
    ::std::declval<IntInplaceVector::const_iterator>(), ::std::declval<ThrowingEndRange&>())));
// A range with nothrow access keeps the wrappers noexcept.
static_assert(noexcept(::std::declval<IntInplaceVector&>().append_range(::std::declval<::std::array<int, 2>&>())));

consteval auto inplace_vector_constexpr_operations_work() -> bool {
    IntInplaceVector values;
    if (!values.is_empty() || values.size() != 0 || values.size_bytes() != 0 || values.capacity() != 8 ||
        values.max_size() != 8 || values.max_size_bytes() != 8 * sizeof(int) || values.begin() != values.end()) {
        return false;
    }

    values.push_back(1);
    values.emplace_back(3);
    auto const inserted = values.emplace(values.begin() + 1, 2);
    if (inserted != values.begin() + 1 || values.size() != 3 || values.front() != 1 || values.back() != 3 ||
        values[1] != 2 || values[2] != 3 || *values.rbegin() != 3) {
        return false;
    }
    IntInplaceVector const& as_const = values;
    if (as_const[0] != 1 || as_const[1] != 2 || as_const[2] != 3) {
        return false;
    }

    (void)values.insert(values.begin() + 1, 2, 7);
    int const tail[]{4, 5};
    values.append(tail, tail + 2);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 7, 7, 2, 3, 4, 5}) {
        return false;
    }
    auto copy = values;
    auto moved = ::std::move(copy);
    if (moved != values) {
        return false;
    }

    (void)values.erase(values.begin() + 1, values.begin() + 3);
    (void)values.erase(values.begin() + 2);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }
    (void)values.erase(values.begin() + 1, values.begin() + 1);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }

    values.resize(6, 9);
    values.resize(5);
    values.pop_back();
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }

    // The fill value may refer to a tail element that shrinking destroys; the
    // shrink path must not read it. Constant evaluation makes a stray read fail.
    IntInplaceVector self_reference_shrink{1, 2, 3, 4, 5};
    self_reference_shrink.resize(2, self_reference_shrink[4]);
    if (self_reference_shrink != ::pltxt2htm::container::InplaceVector<int, 2>{1, 2}) {
        return false;
    }

    // Growth appends at the tail without relocating the source element.
    IntInplaceVector self_reference_grow{7};
    self_reference_grow.resize(4, self_reference_grow[0]);
    if (self_reference_grow != ::pltxt2htm::container::InplaceVector<int, 4>{7, 7, 7, 7}) {
        return false;
    }

    ::std::array<int, 3> const replacement{8, 6, 7};
    values.assign_range(replacement);
    (void)values.insert_range(values.begin() + 1, ::std::array<int, 2>{3, 4});
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{8, 3, 4, 6, 7}) {
        return false;
    }

    ::pltxt2htm::container::InplaceVector<int, 8> other{10, 11};
    values.swap(other);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{10, 11} ||
        other != ::pltxt2htm::container::InplaceVector<int, 8>{8, 3, 4, 6, 7}) {
        return false;
    }

    other.assign(3, 1);
    other = {2, 3};
    other.reserve(8);
    other.shrink_to_fit();
    return other == ::pltxt2htm::container::InplaceVector<int, 2>{2, 3} &&
           (other <=> ::pltxt2htm::container::InplaceVector<int, 3>{2, 4}) == ::std::strong_ordering::less;
}

static_assert(inplace_vector_constexpr_operations_work());

consteval auto inplace_vector_constexpr_special_members_work_after_destroy() -> bool {
    ::pltxt2htm::container::InplaceVector<int, 4> after_pop{1, 2, 3};
    after_pop.pop_back();
    auto copied = after_pop;
    if (copied != ::pltxt2htm::container::InplaceVector<int, 2>{1, 2}) {
        return false;
    }

    (void)copied.erase(copied.begin());
    auto moved = ::std::move(copied);
    if (moved != ::pltxt2htm::container::InplaceVector<int, 1>{2}) {
        return false;
    }

    moved.clear();
    auto cleared = moved;
    return cleared.is_empty();
}

static_assert(inplace_vector_constexpr_special_members_work_after_destroy());

consteval auto zero_capacity_inplace_vector_works() -> bool {
    ::pltxt2htm::container::InplaceVector<int, 0> values;
    auto result = values.try_push_back(1);
    auto const erased = values.erase(values.begin(), values.end());
    auto const inserted = values.insert(values.begin(), 0, 1);
    return values.is_empty() && values.size() == 0 && values.capacity() == 0 && values.data() == nullptr &&
           values.begin() == values.end() && erased == values.end() && inserted == values.end() && !result.has_value();
}

static_assert(zero_capacity_inplace_vector_works());

namespace pltxt2htm_test {

struct TrackedInplaceVectorValue {
    static inline int alive{};
    int value{};

    explicit TrackedInplaceVectorValue(int value_) noexcept
        : value{value_} {
        ++alive;
    }

    TrackedInplaceVectorValue(TrackedInplaceVectorValue const& other) noexcept
        : value{other.value} {
        ++alive;
    }

    TrackedInplaceVectorValue(TrackedInplaceVectorValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
        ++alive;
    }

    auto operator=(this TrackedInplaceVectorValue& self, TrackedInplaceVectorValue const& other) noexcept
        -> TrackedInplaceVectorValue& {
        self.value = other.value;
        return self;
    }

    auto operator=(this TrackedInplaceVectorValue& self, TrackedInplaceVectorValue&& other) noexcept
        -> TrackedInplaceVectorValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    ~TrackedInplaceVectorValue() noexcept {
        --alive;
    }
};

struct CopyTrackedInplaceVectorValue {
    static inline int copies{};
    static inline bool throw_on_copy{};
    static inline int throw_after{-1};
    static inline CopyTrackedInplaceVectorValue const* copy_source{};
    int value{};

    explicit CopyTrackedInplaceVectorValue(int value_) noexcept
        : value{value_} {
    }

    CopyTrackedInplaceVectorValue(CopyTrackedInplaceVectorValue const& other)
        : value{other.value} {
        if (throw_on_copy || copies == throw_after) {
            throw 1;
        }
        copy_source = ::std::addressof(other);
        ++copies;
    }

    CopyTrackedInplaceVectorValue(CopyTrackedInplaceVectorValue&&) noexcept = default;

    ~CopyTrackedInplaceVectorValue() noexcept {
    }
};

} // namespace pltxt2htm_test

TEST_SUITE("container_inplace_vector") {
    TEST_CASE("tracked-lifetime") {
        using TrackedVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::TrackedInplaceVectorValue, 4>;

        {
            TrackedVector values;
            auto first = values.try_emplace_back(1);
            values.emplace_back(2);
            (void)values.emplace(values.begin() + 1, 3);
            CHECK(first.has_value());
            CHECK(first.value().value == 1);
            CHECK(values.size() == 3);
            CHECK(::pltxt2htm_test::TrackedInplaceVectorValue::alive == 3);

            auto const empty_erase_result = values.erase(values.begin() + 1, values.begin() + 1);
            CHECK(empty_erase_result == values.begin() + 1);
            CHECK(values[0].value == 1);
            CHECK(values[1].value == 3);
            CHECK(values[2].value == 2);

            (void)values.erase(values.begin());
            CHECK(values.size() == 2);
            CHECK(::pltxt2htm_test::TrackedInplaceVectorValue::alive == 2);
            values.clear();
            CHECK(::pltxt2htm_test::TrackedInplaceVectorValue::alive == 0);

            values.emplace_back(4);
            values.emplace_back(5);
            values.emplace_back(6);
            values.emplace_back(7);
            auto full_result = values.try_emplace_back(8);
            CHECK_FALSE(full_result.has_value());
            CHECK(values.size() == values.capacity());
        }

        CHECK(::pltxt2htm_test::TrackedInplaceVectorValue::alive == 0);
    }

    TEST_CASE("comparison-operators") {
        ::pltxt2htm::container::InplaceVector<int, 4> left{1, 2, 3};
        ::pltxt2htm::container::InplaceVector<int, 4> right{1, 2, 3};
        ::pltxt2htm::container::InplaceVector<int, 4> different{1, 2, 4};

        CHECK(left == right);
        CHECK(left != different);
        CHECK((left <=> right) == ::std::strong_ordering::equal);
        CHECK((left <=> different) == ::std::strong_ordering::less);
    }

    TEST_CASE("free-swap") {
        ::pltxt2htm::container::InplaceVector<int, 4> left{1, 2};
        ::pltxt2htm::container::InplaceVector<int, 4> right{3, 4, 5};
        ::pltxt2htm::container::swap(left, right);
        CHECK(left == ::pltxt2htm::container::InplaceVector<int, 4>{3, 4, 5});
        CHECK(right == ::pltxt2htm::container::InplaceVector<int, 4>{1, 2});
    }

    TEST_CASE("resize-with-self-referencing-value") {
        // Shrinking destroys the element that value references, but that path
        // never uses the fill value. Growth appends at the tail, so the source
        // element stays in place while copies are appended.
        ::pltxt2htm::container::InplaceVector<int, 8> values{1, 2, 3, 4, 5};
        values.resize(2, values[4]);
        CHECK(values.size() == 2);
        CHECK(values[0] == 1);
        CHECK(values[1] == 2);
        // Now grow; the source element is not relocated.
        values.resize(5, values[0]);
        CHECK(values.size() == 5);
        CHECK(values[2] == 1);
        CHECK(values[3] == 1);
        CHECK(values[4] == 1);
    }

    TEST_CASE("resize-with-value-copy-count") {
        using CopyVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::CopyTrackedInplaceVectorValue, 8>;
        CopyVector values;
        values.emplace_back(7);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = false;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        values.resize(4, values.front());
        CHECK(values.size() == 4);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 3);
        CHECK(values[0].value == 7);
        CHECK(values[3].value == 7);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        values.resize(2, values.front());
        CHECK(values.size() == 2);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 0);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        values.resize(values.size(), values.front());
        CHECK(values.size() == 2);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 0);
    }

    TEST_CASE("resize-with-value-growth-keeps-source-valid") {
        using CopyVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::CopyTrackedInplaceVectorValue, 8>;
        CopyVector values;
        values.emplace_back(1);
        values.emplace_back(2);
        auto const* const source = ::std::addressof(values.back());

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = false;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copy_source = nullptr;
        values.resize(4, values.back());
        CHECK(values.size() == 4);
        CHECK(values[2].value == 2);
        CHECK(values[3].value == 2);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 2);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copy_source == source);
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copy_source = nullptr;
    }

    TEST_CASE("resize-with-value-shrink-does-not-copy") {
        using CopyVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::CopyTrackedInplaceVectorValue, 8>;
        CopyVector values;
        values.emplace_back(1);
        values.emplace_back(2);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = true;
        CHECK_NOTHROW(values.resize(1, values.back()));
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = false;
        CHECK(values.size() == 1);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 0);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = true;
        CHECK_NOTHROW(values.resize(values.size(), values.front()));
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = false;
        CHECK(values.size() == 1);
        CHECK(::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies == 0);
    }

    TEST_CASE("resize-with-value-throwing-copy-rolls-back") {
        using CopyVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::CopyTrackedInplaceVectorValue, 8>;
        CopyVector values;
        values.emplace_back(1);
        values.emplace_back(2);

        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::copies = 0;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_on_copy = false;
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_after = 1;
        CHECK_THROWS(values.resize(5, values.front()));
        ::pltxt2htm_test::CopyTrackedInplaceVectorValue::throw_after = -1;
        CHECK(values.size() == 2);
        CHECK(values[0].value == 1);
        CHECK(values[1].value == 2);
    }

    TEST_CASE("assign-with-self-referencing-value") {
        ::pltxt2htm::container::InplaceVector<int, 8> values{1, 2, 3};
        values.assign(5, values[0]);
        CHECK(values.size() == 5);
        CHECK(values[0] == 1);
        CHECK(values[4] == 1);
    }

    TEST_CASE("non-trivial-swap") {
        using TrackedVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::TrackedInplaceVectorValue, 4>;
        TrackedVector left;
        left.emplace_back(1);
        left.emplace_back(2);
        TrackedVector right;
        right.emplace_back(3);
        right.emplace_back(4);
        right.emplace_back(5);
        left.swap(right);
        CHECK(left.size() == 3);
        CHECK(right.size() == 2);
        CHECK(left[0].value == 3);
        CHECK(left[1].value == 4);
        CHECK(left[2].value == 5);
        CHECK(right[0].value == 1);
        CHECK(right[1].value == 2);
        CHECK(::pltxt2htm_test::TrackedInplaceVectorValue::alive == 5);
    }

    TEST_CASE("move-assignment-non-trivial") {
        // Test move assignment with a non-trivial type to avoid clang's union
        // default-move-assignment bug (which does nothing for trivially copyable
        // unions on some platforms).
        using TrackedVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::TrackedInplaceVectorValue, 4>;
        TrackedVector left;
        left.emplace_back(1);
        left.emplace_back(2);
        left.emplace_back(3);
        TrackedVector right;
        right.emplace_back(4);
        right.emplace_back(5);
        left = ::std::move(right);
        CHECK(left.size() == 2);
        CHECK(left[0].value == 4);
        CHECK(left[1].value == 5);
    }

    TEST_CASE("insert-at-end") {
        ::pltxt2htm::container::InplaceVector<int, 4> values{1, 2};
        auto const result = values.insert(values.end(), 3);
        CHECK(result == values.begin() + 2);
        CHECK(values.size() == 3);
        CHECK(values.back() == 3);
    }

    TEST_CASE("erase-range-return-iterator") {
        ::pltxt2htm::container::InplaceVector<int, 4> values{1, 2, 3, 4};
        auto const result = values.erase(values.begin() + 1, values.begin() + 3);
        CHECK(result == values.begin() + 1);
        CHECK(values.size() == 2);
        CHECK(values[0] == 1);
        CHECK(values[1] == 4);
    }

    TEST_CASE("throwing-range-access-propagates") {
        IntInplaceVector values;
        ::pltxt2htm_test::ThrowingBeginRange begin_range{nullptr, nullptr, true};
        CHECK_THROWS(values.append_range(begin_range));
        ::pltxt2htm_test::ThrowingEndRange end_range{nullptr, nullptr, true};
        CHECK_THROWS(values.assign_range(end_range));
        CHECK_THROWS(static_cast<void>(values.insert_range(values.begin(), begin_range)));
    }
}
