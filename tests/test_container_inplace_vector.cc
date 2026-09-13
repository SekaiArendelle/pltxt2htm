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

#include "precompile.hh"

using IntVector = ::pltxt2htm::container::InplaceVector<int, 8>;

template<typename Vector>
concept can_subscript = requires(Vector& vector) { vector[0]; };

template<typename Vector>
concept has_at = requires(Vector& vector) { vector.at(0); };

template<typename Vector>
concept has_unchecked_emplace_back = requires(Vector& vector) { vector.unchecked_emplace_back(0); };

template<typename Vector>
concept has_unchecked_push_back = requires(Vector& vector) { vector.unchecked_push_back(0); };

template<typename Vector>
concept has_default_emplace_back = requires(Vector& vector) { vector.emplace_back(0); };

template<typename Vector>
concept has_default_copy_push_back =
    requires(Vector& vector, typename Vector::const_reference value) { vector.push_back(value); };

template<typename Vector>
concept has_default_move_push_back =
    requires(Vector& vector, typename Vector::value_type&& value) { vector.push_back(::std::move(value)); };

struct NonTrivialValue {
    ~NonTrivialValue() noexcept {
    }
};

struct TrivialWithoutDefaultConstructor {
    TrivialWithoutDefaultConstructor() = delete;
    int value;
};

struct TriviallyMovableOnly {
    TriviallyMovableOnly() = default;
    TriviallyMovableOnly(TriviallyMovableOnly const&) = delete;
    TriviallyMovableOnly(TriviallyMovableOnly&&) = default;
    auto operator=(this TriviallyMovableOnly&, TriviallyMovableOnly const&) -> TriviallyMovableOnly& = delete;
    auto operator=(this TriviallyMovableOnly&, TriviallyMovableOnly&&) -> TriviallyMovableOnly& = default;
    int value;
};

struct TrivialWithoutAssignment {
    auto operator=(this TrivialWithoutAssignment&, TrivialWithoutAssignment const&)
        -> TrivialWithoutAssignment& = delete;
    int value;
};

struct TriviallyCopyableWithNontrivialSelectedAssignment {
    auto operator=(this TriviallyCopyableWithNontrivialSelectedAssignment&,
                   TriviallyCopyableWithNontrivialSelectedAssignment const&)
        -> TriviallyCopyableWithNontrivialSelectedAssignment& = delete;

    template<typename U>
    constexpr auto operator=(this TriviallyCopyableWithNontrivialSelectedAssignment& self, U&&)
        -> TriviallyCopyableWithNontrivialSelectedAssignment& {
        return self;
    }
};

struct Immobile {
    Immobile() = delete;
    Immobile(Immobile const&) = delete;
    Immobile(Immobile&&) = delete;
    auto operator=(this Immobile&, Immobile const&) -> Immobile& = delete;
    auto operator=(this Immobile&, Immobile&&) -> Immobile& = delete;
};

struct PotentiallyThrowingComparison {
    int value;

    [[nodiscard]] friend constexpr auto operator==(PotentiallyThrowingComparison const& left,
                                                   PotentiallyThrowingComparison const& right) -> bool {
        return left.value == right.value;
    }

    [[nodiscard]] friend constexpr auto operator<=>(PotentiallyThrowingComparison const& left,
                                                    PotentiallyThrowingComparison const& right) noexcept(false) {
        return left.value <=> right.value;
    }
};

using ZeroVector = ::pltxt2htm::container::InplaceVector<NonTrivialValue, 0>;

static_assert(::std::same_as<IntVector::value_type, int>);
static_assert(::std::same_as<IntVector::iterator, int*>);
static_assert(::std::same_as<IntVector::const_iterator, int const*>);
static_assert(::std::ranges::contiguous_range<IntVector>);
static_assert(::std::ranges::sized_range<IntVector>);
static_assert(::std::default_initializable<IntVector>);
static_assert(!can_subscript<IntVector>);
static_assert(!has_at<IntVector>);
static_assert(!has_unchecked_emplace_back<IntVector>);
static_assert(!has_unchecked_push_back<IntVector>);
static_assert(!has_default_emplace_back<IntVector>);
static_assert(!has_default_copy_push_back<IntVector>);
static_assert(!has_default_move_push_back<IntVector>);
static_assert(::std::is_trivially_copy_constructible_v<IntVector>);
static_assert(::std::is_trivially_move_constructible_v<IntVector>);
static_assert(::std::is_trivially_copy_assignable_v<IntVector>);
static_assert(::std::is_trivially_move_assignable_v<IntVector>);
static_assert(::std::is_empty_v<ZeroVector>);
static_assert(::std::is_trivially_default_constructible_v<ZeroVector>);
static_assert(::std::is_trivially_copyable_v<ZeroVector>);
static_assert(::std::is_trivially_copyable_v<TrivialWithoutDefaultConstructor>);
static_assert(::std::default_initializable<::pltxt2htm::container::InplaceVector<TrivialWithoutDefaultConstructor, 2>>);
static_assert(::std::is_trivially_move_constructible_v<TriviallyMovableOnly>);
static_assert(::std::is_trivially_move_assignable_v<TriviallyMovableOnly>);
static_assert(::std::is_trivially_move_constructible_v<::pltxt2htm::container::InplaceVector<TriviallyMovableOnly, 2>>);
static_assert(::std::is_trivially_move_assignable_v<::pltxt2htm::container::InplaceVector<TriviallyMovableOnly, 2>>);
static_assert(::std::is_trivially_default_constructible_v<TrivialWithoutAssignment>);
static_assert(::std::is_trivially_copyable_v<TrivialWithoutAssignment>);
static_assert(::std::default_initializable<::pltxt2htm::container::InplaceVector<TrivialWithoutAssignment, 2>>);
static_assert(::std::is_trivially_default_constructible_v<TriviallyCopyableWithNontrivialSelectedAssignment>);
static_assert(::std::is_trivially_copyable_v<TriviallyCopyableWithNontrivialSelectedAssignment>);
static_assert(::std::is_assignable_v<TriviallyCopyableWithNontrivialSelectedAssignment&,
                                     TriviallyCopyableWithNontrivialSelectedAssignment>);
static_assert(!::std::is_trivially_assignable_v<TriviallyCopyableWithNontrivialSelectedAssignment&,
                                                TriviallyCopyableWithNontrivialSelectedAssignment>);
using ImmobileZeroVector = ::pltxt2htm::container::InplaceVector<Immobile, 0>;
static_assert(noexcept(::std::declval<ImmobileZeroVector&>().swap(::std::declval<ImmobileZeroVector&>())));
static_assert(noexcept(::pltxt2htm::container::swap(::std::declval<ImmobileZeroVector&>(),
                                                    ::std::declval<ImmobileZeroVector&>())));
static_assert(
    ::std::same_as<decltype(::std::declval<IntVector&>().try_emplace_back(1)), ::pltxt2htm::container::Optional<int&>>);
static_assert(
    ::std::same_as<
        decltype(::std::declval<IntVector&>().template emplace_back<::pltxt2htm::Contracts::quick_enforce>(1)), int&>);
static_assert(noexcept(::std::declval<IntVector const&>() <=> ::std::declval<IntVector const&>()));
using PotentiallyThrowingComparisonVector = ::pltxt2htm::container::InplaceVector<PotentiallyThrowingComparison, 1>;
using PotentiallyThrowingComparisonEmptyVector =
    ::pltxt2htm::container::InplaceVector<PotentiallyThrowingComparison, 0>;
static_assert(!noexcept(::std::declval<PotentiallyThrowingComparisonVector const&>() <=>
                        ::std::declval<PotentiallyThrowingComparisonVector const&>()));
static_assert(noexcept(::std::declval<PotentiallyThrowingComparisonEmptyVector const&>() <=>
                       ::std::declval<PotentiallyThrowingComparisonVector const&>()));
static_assert(noexcept(::std::declval<PotentiallyThrowingComparisonVector const&>() <=>
                       ::std::declval<PotentiallyThrowingComparisonEmptyVector const&>()));

consteval auto inplace_vector_constexpr_operations_work() -> bool {
    IntVector values;
    if (!values.empty() || values.size() != 0 || values.size_bytes() != 0 || values.capacity() != 8 ||
        values.max_size() != 8 || values.max_size_bytes() != 8 * sizeof(int) || values.begin() != values.end()) {
        return false;
    }

    values.template push_back<::pltxt2htm::Contracts::quick_enforce>(1);
    values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(3);
    auto const inserted = values.template emplace<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, 2);
    if (inserted != values.begin() + 1 || values.size() != 3 ||
        values.template front<::pltxt2htm::Contracts::quick_enforce>() != 1 ||
        values.template back<::pltxt2htm::Contracts::quick_enforce>() != 3 ||
        values.template index<::pltxt2htm::Contracts::quick_enforce>(1) != 2 ||
        values.template index<::pltxt2htm::Contracts::ignore>(2) != 3 || *values.rbegin() != 3) {
        return false;
    }

    (void)values.template insert<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, 2, 7);
    int const tail[]{4, 5};
    values.template append<::pltxt2htm::Contracts::quick_enforce>(tail, tail + 2);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 7, 7, 2, 3, 4, 5}) {
        return false;
    }
    auto copy = values;
    auto moved = ::std::move(copy);
    if (moved != values) {
        return false;
    }

    (void)values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, values.begin() + 3);
    (void)values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 2);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }
    (void)values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, values.begin() + 1);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }

    values.template resize<::pltxt2htm::Contracts::quick_enforce>(6, 9);
    values.template resize<::pltxt2htm::Contracts::quick_enforce>(5);
    values.template pop_back<::pltxt2htm::Contracts::quick_enforce>();
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{1, 2, 4, 5}) {
        return false;
    }

    ::std::array<int, 3> const replacement{8, 6, 7};
    values.template assign_range<::pltxt2htm::Contracts::quick_enforce>(replacement);
    (void)values.template insert_range<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1,
                                                                              ::std::array<int, 2>{3, 4});
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{8, 3, 4, 6, 7}) {
        return false;
    }

    ::pltxt2htm::container::InplaceVector<int, 8> other{10, 11};
    values.swap(other);
    if (values != ::pltxt2htm::container::InplaceVector<int, 8>{10, 11} ||
        other != ::pltxt2htm::container::InplaceVector<int, 8>{8, 3, 4, 6, 7}) {
        return false;
    }

    other.template assign<::pltxt2htm::Contracts::quick_enforce>(3, 1);
    other = {2, 3};
    other.template reserve<::pltxt2htm::Contracts::quick_enforce>(8);
    other.shrink_to_fit();
    return other == ::pltxt2htm::container::InplaceVector<int, 2>{2, 3} &&
           (other <=> ::pltxt2htm::container::InplaceVector<int, 3>{2, 4}) == ::std::strong_ordering::less;
}

static_assert(inplace_vector_constexpr_operations_work());

consteval auto inplace_vector_constexpr_special_members_work_after_destroy() -> bool {
    ::pltxt2htm::container::InplaceVector<int, 4> after_pop{1, 2, 3};
    after_pop.template pop_back<::pltxt2htm::Contracts::quick_enforce>();
    auto copied = after_pop;
    if (copied != ::pltxt2htm::container::InplaceVector<int, 2>{1, 2}) {
        return false;
    }

    (void)copied.template erase<::pltxt2htm::Contracts::quick_enforce>(copied.begin());
    auto moved = ::std::move(copied);
    if (moved != ::pltxt2htm::container::InplaceVector<int, 1>{2}) {
        return false;
    }

    moved.clear();
    auto cleared = moved;
    return cleared.empty();
}

static_assert(inplace_vector_constexpr_special_members_work_after_destroy());

consteval auto inplace_vector_constexpr_ignores_nontrivial_selected_assignment() -> bool {
    ::pltxt2htm::container::InplaceVector<TriviallyCopyableWithNontrivialSelectedAssignment, 2> values;
    return values.empty();
}

static_assert(inplace_vector_constexpr_ignores_nontrivial_selected_assignment());

consteval auto zero_capacity_inplace_vector_works() -> bool {
    ::pltxt2htm::container::InplaceVector<int, 0> values;
    auto result = values.try_push_back(1);
    auto const erased = values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin(), values.end());
    auto const inserted = values.template insert<::pltxt2htm::Contracts::quick_enforce>(values.begin(), 0, 1);
    return values.empty() && values.size() == 0 && values.capacity() == 0 && values.data() == nullptr &&
           values.begin() == values.end() && erased == values.end() && inserted == values.end() && !result.has_value();
}

static_assert(zero_capacity_inplace_vector_works());

namespace pltxt2htm_test {

struct TrackedInplaceValue {
    static inline int alive{};
    int value{};

    explicit TrackedInplaceValue(int value_) noexcept
        : value{value_} {
        ++alive;
    }

    TrackedInplaceValue(TrackedInplaceValue const& other) noexcept
        : value{other.value} {
        ++alive;
    }

    TrackedInplaceValue(TrackedInplaceValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
        ++alive;
    }

    auto operator=(this TrackedInplaceValue& self, TrackedInplaceValue const& other) noexcept -> TrackedInplaceValue& {
        self.value = other.value;
        return self;
    }

    auto operator=(this TrackedInplaceValue& self, TrackedInplaceValue&& other) noexcept -> TrackedInplaceValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    ~TrackedInplaceValue() noexcept {
        --alive;
    }
};

struct ThrowingCopyValue {
    static inline int alive{};
    static inline int copies_before_throw{-1};
    int value{};

    explicit ThrowingCopyValue(int value_) noexcept
        : value{value_} {
        ++alive;
    }

    ThrowingCopyValue(ThrowingCopyValue const& other)
        : value{other.value} {
        if (copies_before_throw == 0) {
            throw 42;
        }
        if (copies_before_throw > 0) {
            --copies_before_throw;
        }
        ++alive;
    }

    ThrowingCopyValue(ThrowingCopyValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
        ++alive;
    }

    auto operator=(this ThrowingCopyValue& self, ThrowingCopyValue const& other) noexcept -> ThrowingCopyValue& {
        self.value = other.value;
        return self;
    }

    auto operator=(this ThrowingCopyValue& self, ThrowingCopyValue&& other) noexcept -> ThrowingCopyValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    ~ThrowingCopyValue() noexcept {
        --alive;
    }
};

} // namespace pltxt2htm_test

int main() {
    using TrackedVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::TrackedInplaceValue, 4>;

    {
        TrackedVector values;
        auto first = values.try_emplace_back(1);
        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(2);
        (void)values.template emplace<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, 3);
        pltxt2htm_test_assert_true(first.has_value());
        pltxt2htm_test_assert_true(first.value<::pltxt2htm::Contracts::quick_enforce>().value == 1);
        pltxt2htm_test_assert_true(values.size() == 3);
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedInplaceValue::alive == 3);

        auto const empty_erase_result =
            values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin() + 1, values.begin() + 1);
        pltxt2htm_test_assert_true(empty_erase_result == values.begin() + 1);
        pltxt2htm_test_assert_true(values.template index<::pltxt2htm::Contracts::quick_enforce>(0).value == 1 &&
                                   values.template index<::pltxt2htm::Contracts::quick_enforce>(1).value == 3 &&
                                   values.template index<::pltxt2htm::Contracts::quick_enforce>(2).value == 2);

        (void)values.template erase<::pltxt2htm::Contracts::quick_enforce>(values.begin());
        pltxt2htm_test_assert_true(values.size() == 2);
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedInplaceValue::alive == 2);
        values.clear();
        pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedInplaceValue::alive == 0);

        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(4);
        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(5);
        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(6);
        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(7);
        auto full_result = values.try_emplace_back(8);
        pltxt2htm_test_assert_false(full_result.has_value());
        pltxt2htm_test_assert_true(values.size() == values.capacity());
    }

    pltxt2htm_test_assert_true(::pltxt2htm_test::TrackedInplaceValue::alive == 0);

    {
        using ThrowingVector = ::pltxt2htm::container::InplaceVector<::pltxt2htm_test::ThrowingCopyValue, 4>;
        ThrowingVector values;
        values.template emplace_back<::pltxt2htm::Contracts::quick_enforce>(1);
        ::pltxt2htm_test::ThrowingCopyValue fill_value{9};
        ::pltxt2htm_test::ThrowingCopyValue::copies_before_throw = 1;
        try {
            values.template resize<::pltxt2htm::Contracts::quick_enforce>(4, fill_value);
            pltxt2htm_test_assert_true(false);
        } catch (int error) {
            pltxt2htm_test_assert_true(error == 42);
        }
        pltxt2htm_test_assert_true(values.size() == 1);
        pltxt2htm_test_assert_true(values.template front<::pltxt2htm::Contracts::quick_enforce>().value == 1);
        pltxt2htm_test_assert_true(::pltxt2htm_test::ThrowingCopyValue::alive == 2);
    }
    pltxt2htm_test_assert_true(::pltxt2htm_test::ThrowingCopyValue::alive == 0);

    return 0;
}
