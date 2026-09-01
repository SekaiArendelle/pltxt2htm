#include <concepts>
#include <cstddef>
#include <utility>

#include "precompile.hh"

#include <pltxt2htm/container/expected.hh>
#include <pltxt2htm/container/non_zero.hh>
#include <pltxt2htm/container/optional.hh>

using IntOptional = ::pltxt2htm::container::Optional<int>;

static_assert(::pltxt2htm::container::is_optional<IntOptional>);
static_assert(!::pltxt2htm::container::is_expected<IntOptional>);
static_assert(!::std::default_initializable<IntOptional>);
static_assert(::std::same_as<IntOptional::value_type, int>);
static_assert(::std::same_as<IntOptional::rebind<double>, ::pltxt2htm::container::Optional<double>>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&>().has_value()), bool>);
static_assert(::std::same_as<decltype(::std::declval<IntOptional const&&>().has_value()), bool>);

static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional const&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional&&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntOptional const&&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&&>);

consteval bool optional_constexpr_operations_work() noexcept {
    IntOptional value{42};
    IntOptional empty{::pltxt2htm::container::nullopt};
    if (!value.has_value() || empty.has_value()) {
        return false;
    }
    if (value.value<::pltxt2htm::Contracts::ignore>() != 42 || value != 42 ||
        empty != ::pltxt2htm::container::nullopt) {
        return false;
    }
    empty = value;
    if (!empty.has_value() || empty.value<::pltxt2htm::Contracts::ignore>() != 42) {
        return false;
    }
    value = ::pltxt2htm::container::nullopt;
    empty.swap(value);
    return value == 42 && empty == ::pltxt2htm::container::nullopt;
}

static_assert(optional_constexpr_operations_work());

using NonZeroSize = ::pltxt2htm::container::NonZeroSize;
using NonZeroSizeOptional = ::pltxt2htm::container::Optional<NonZeroSize>;

template<typename T>
consteval auto optional_non_zero_has_niche_representation() noexcept -> bool {
    using OptionalNonZero = ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZero<T>>;

    return sizeof(OptionalNonZero) == sizeof(T) && alignof(OptionalNonZero) == alignof(T);
}

static_assert(optional_non_zero_has_niche_representation<unsigned char>());
static_assert(optional_non_zero_has_niche_representation<unsigned short>());
static_assert(optional_non_zero_has_niche_representation<unsigned>());
static_assert(optional_non_zero_has_niche_representation<unsigned long>());
static_assert(optional_non_zero_has_niche_representation<unsigned long long>());
static_assert(optional_non_zero_has_niche_representation<::std::size_t>());

consteval bool optional_non_zero_constexpr_operations_work() noexcept {
    auto const seven = NonZeroSize::from<::pltxt2htm::Contracts::quick_enforce>(7);
    auto const eleven = NonZeroSize::from<::pltxt2htm::Contracts::quick_enforce>(11);
    NonZeroSizeOptional value{seven};
    NonZeroSizeOptional empty{::pltxt2htm::container::nullopt};
    if (!value.has_value() || empty.has_value()) {
        return false;
    }
    if (value.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    empty = value;
    if (!empty.has_value() ||
        empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    value = eleven;
    empty.swap(value);
    if (empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 11 ||
        value.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() != 7) {
        return false;
    }
    empty = ::pltxt2htm::container::nullopt;
    value.swap(empty);
    return !value.has_value() && empty.has_value() &&
           empty.value<::pltxt2htm::Contracts::ignore>().get<::pltxt2htm::Contracts::ignore>() == 7;
}

static_assert(optional_non_zero_constexpr_operations_work());

namespace pltxt2htm_test {

struct OptionalTrackedValue {
    int value{};

    constexpr OptionalTrackedValue() noexcept = default;

    constexpr explicit OptionalTrackedValue(int value_) noexcept
        : value{value_} {
    }

    constexpr OptionalTrackedValue(OptionalTrackedValue const&) noexcept = default;

    constexpr OptionalTrackedValue(OptionalTrackedValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    constexpr auto operator=(this OptionalTrackedValue& self, OptionalTrackedValue const&) noexcept
        -> OptionalTrackedValue& = default;

    constexpr auto operator=(this OptionalTrackedValue& self, OptionalTrackedValue&& other) noexcept
        -> OptionalTrackedValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    constexpr bool operator==(this OptionalTrackedValue const&, OptionalTrackedValue const&) noexcept = default;

    constexpr ~OptionalTrackedValue() noexcept {
    }
};

} // namespace pltxt2htm_test

int main() {
    IntOptional empty{::pltxt2htm::container::nullopt};
    pltxt2htm_test_assert_false(empty.has_value());
    pltxt2htm_test_assert_true(empty == ::pltxt2htm::container::nullopt);
    pltxt2htm_test_assert_true(::pltxt2htm::container::nullopt == empty);

    IntOptional value{42};
    pltxt2htm_test_assert_true(value.has_value());
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);
    pltxt2htm_test_assert_true(value == 42);

    int fallback{7};
    int const const_fallback{9};
    pltxt2htm_test_assert_true(::std::addressof(empty.value_or(fallback)) == ::std::addressof(fallback));
    pltxt2htm_test_assert_true(::std::addressof(value.value_or(fallback)) ==
                               ::std::addressof(value.value<::pltxt2htm::Contracts::quick_enforce>()));
    pltxt2htm_test_assert_true(static_cast<IntOptional const&>(empty).value_or(const_fallback) == const_fallback);
    pltxt2htm_test_assert_true(IntOptional{::pltxt2htm::container::nullopt}.value_or(int{11}) == 11);

    using TrackedOptional = ::pltxt2htm::container::Optional<::pltxt2htm_test::OptionalTrackedValue>;

    TrackedOptional state{::pltxt2htm_test::OptionalTrackedValue{42}};
    TrackedOptional other{::pltxt2htm::container::nullopt};
    state.swap(other);
    pltxt2htm_test_assert_false(state.has_value());
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    state.swap(other);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_false(other.has_value());

    state.swap(state);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    other = state;
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    other = ::pltxt2htm::container::nullopt;
    pltxt2htm_test_assert_false(other.has_value());
    other = ::pltxt2htm_test::OptionalTrackedValue{9};
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    TrackedOptional moved{::std::move(other)};
    pltxt2htm_test_assert_true(moved.has_value());
    pltxt2htm_test_assert_true(moved.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    TrackedOptional moved_assignment{::pltxt2htm_test::OptionalTrackedValue{13}};
    moved_assignment = ::std::move(moved);
    pltxt2htm_test_assert_true(moved_assignment.has_value());
    pltxt2htm_test_assert_true(moved_assignment.value<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    return 0;
}
