#include <concepts>
#include <utility>

#include "precompile.hh"

#include <pltxt2htm/container/expected.hh>

using IntExpected = ::pltxt2htm::container::Expected<int, int>;

static_assert(::pltxt2htm::container::is_expected<IntExpected>);
static_assert(::pltxt2htm::container::is_optional<::pltxt2htm::container::Optional<int>>);
static_assert(::pltxt2htm::container::is_unexpected<::pltxt2htm::container::Unexpected<int>>);
static_assert(::std::same_as<decltype(::std::declval<IntExpected const&>().has_value()), bool>);
static_assert(::std::same_as<decltype(::std::declval<IntExpected const&&>().has_value()), bool>);

static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected const&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected&&>().value<::pltxt2htm::Contracts::quick_enforce>()), int&&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected const&&>().value<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&&>);

static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected&>().error<::pltxt2htm::Contracts::quick_enforce>()), int&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected const&>().error<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected&&>().error<::pltxt2htm::Contracts::quick_enforce>()), int&&>);
static_assert(
    ::std::same_as<decltype(::std::declval<IntExpected const&&>().error<::pltxt2htm::Contracts::quick_enforce>()),
                   int const&&>);

consteval bool ignored_contract_accessors_work() noexcept {
    IntExpected value{42};
    IntExpected error{::pltxt2htm::container::Unexpected<int>{7}};
    return value.value<::pltxt2htm::Contracts::ignore>() == 42 && error.error<::pltxt2htm::Contracts::ignore>() == 7;
}

static_assert(ignored_contract_accessors_work());

namespace pltxt2htm_test {

struct TrackedValue {
    int value{};

    constexpr TrackedValue() noexcept = default;

    constexpr explicit TrackedValue(int value_) noexcept
        : value{value_} {
    }

    constexpr TrackedValue(TrackedValue const&) noexcept = default;

    constexpr TrackedValue(TrackedValue&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    constexpr auto operator=(this TrackedValue& self, TrackedValue const&) noexcept -> TrackedValue& = default;

    constexpr auto operator=(this TrackedValue& self, TrackedValue&& other) noexcept -> TrackedValue& {
        self.value = other.value;
        other.value = -1;
        return self;
    }

    constexpr ~TrackedValue() noexcept {
    }
};

} // namespace pltxt2htm_test

int main() {
    ::pltxt2htm::container::Expected<int, int> value{42};
    pltxt2htm_test_assert_true(value.has_value());
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);

    ::pltxt2htm::container::Expected<int, int> error{::pltxt2htm::container::Unexpected<int>{7}};
    pltxt2htm_test_assert_false(error.has_value());
    pltxt2htm_test_assert_true(error.error<::pltxt2htm::Contracts::quick_enforce>() == 7);

    ::pltxt2htm::container::Optional<int> empty{::pltxt2htm::container::nullopt};
    pltxt2htm_test_assert_false(empty.has_value());

    using TrackedExpected =
        ::pltxt2htm::container::Expected<::pltxt2htm_test::TrackedValue, ::pltxt2htm_test::TrackedValue>;

    TrackedExpected state{::pltxt2htm_test::TrackedValue{42}};
    ::pltxt2htm::container::Unexpected<::pltxt2htm_test::TrackedValue> lvalue_error{::pltxt2htm_test::TrackedValue{7}};
    state = lvalue_error;
    pltxt2htm_test_assert_false(state.has_value());
    pltxt2htm_test_assert_true(lvalue_error.value.value == 7);
    pltxt2htm_test_assert_true(state.error<::pltxt2htm::Contracts::quick_enforce>().value == 7);

    state = ::pltxt2htm_test::TrackedValue{42};
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    TrackedExpected other{
        ::pltxt2htm::container::Unexpected<::pltxt2htm_test::TrackedValue>{::pltxt2htm_test::TrackedValue{9}}};
    state.swap(other);
    pltxt2htm_test_assert_false(state.has_value());
    pltxt2htm_test_assert_true(state.error<::pltxt2htm::Contracts::quick_enforce>().value == 9);
    pltxt2htm_test_assert_true(other.has_value());
    pltxt2htm_test_assert_true(other.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    state.swap(other);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);
    pltxt2htm_test_assert_false(other.has_value());
    pltxt2htm_test_assert_true(other.error<::pltxt2htm::Contracts::quick_enforce>().value == 9);

    state.swap(state);
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    return 0;
}
