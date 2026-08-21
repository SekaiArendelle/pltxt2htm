#include "precompile.hh"

#include <pltxt2htm/container/expected.hh>

static_assert(::pltxt2htm::container::is_expected<::pltxt2htm::container::expected<int, int>>);
static_assert(::pltxt2htm::container::is_optional<::pltxt2htm::container::optional<int>>);
static_assert(::pltxt2htm::container::is_unexpected<::pltxt2htm::container::unexpected<int>>);

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
    ::pltxt2htm::container::expected<int, int> value{42};
    pltxt2htm_test_assert_true(value.has_value());
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);

    ::pltxt2htm::container::expected<int, int> error{::pltxt2htm::container::unexpected<int>{7}};
    pltxt2htm_test_assert_false(error.has_value());
    pltxt2htm_test_assert_true(error.error<::pltxt2htm::Contracts::quick_enforce>() == 7);

    ::pltxt2htm::container::optional<int> empty{::pltxt2htm::container::nullopt};
    pltxt2htm_test_assert_false(empty.has_value());

    using TrackedExpected =
        ::pltxt2htm::container::expected<::pltxt2htm_test::TrackedValue, ::pltxt2htm_test::TrackedValue>;

    TrackedExpected state{::pltxt2htm_test::TrackedValue{42}};
    ::pltxt2htm::container::unexpected<::pltxt2htm_test::TrackedValue> lvalue_error{::pltxt2htm_test::TrackedValue{7}};
    state = lvalue_error;
    pltxt2htm_test_assert_false(state.has_value());
    pltxt2htm_test_assert_true(lvalue_error.val_.value == 7);
    pltxt2htm_test_assert_true(state.error<::pltxt2htm::Contracts::quick_enforce>().value == 7);

    state = ::pltxt2htm_test::TrackedValue{42};
    pltxt2htm_test_assert_true(state.has_value());
    pltxt2htm_test_assert_true(state.value<::pltxt2htm::Contracts::quick_enforce>().value == 42);

    TrackedExpected other{
        ::pltxt2htm::container::unexpected<::pltxt2htm_test::TrackedValue>{::pltxt2htm_test::TrackedValue{9}}};
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
