#include "precompile.hh"

#include <pltxt2htm/container/expected.hh>

static_assert(::pltxt2htm::container::is_expected<::pltxt2htm::container::expected<int, int>>);
static_assert(::pltxt2htm::container::is_optional<::pltxt2htm::container::optional<int>>);
static_assert(::pltxt2htm::container::is_unexpected<::pltxt2htm::container::unexpected<int>>);

int main() {
    ::pltxt2htm::container::expected<int, int> value{42};
    pltxt2htm_test_assert_true(value.has_value());
    pltxt2htm_test_assert_true(value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);

    ::pltxt2htm::container::expected<int, int> error{::pltxt2htm::container::unexpected<int>{7}};
    pltxt2htm_test_assert_false(error.has_value());
    pltxt2htm_test_assert_true(error.error<::pltxt2htm::Contracts::quick_enforce>() == 7);

    ::pltxt2htm::container::optional<int> empty{::pltxt2htm::container::nullopt};
    pltxt2htm_test_assert_false(empty.has_value());

    return 0;
}
