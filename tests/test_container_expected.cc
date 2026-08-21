#include <pltxt2htm/container/expected.hh>

static_assert(::pltxt2htm::container::is_expected<::pltxt2htm::container::expected<int, int>>);
static_assert(::pltxt2htm::container::is_optional<::pltxt2htm::container::optional<int>>);
static_assert(::pltxt2htm::container::is_unexpected<::pltxt2htm::container::unexpected<int>>);

consteval bool ignored_contracts_skip_assertions() noexcept {
    ::pltxt2htm::details::assert_true<::pltxt2htm::Contracts::ignore>(false);
    ::pltxt2htm::details::assert_false<::pltxt2htm::Contracts::ignore>(true);
    return true;
}

static_assert(ignored_contracts_skip_assertions());

int main() {
    ::pltxt2htm::container::expected<int, int> value{42};
    ::pltxt2htm::details::assert_true<::pltxt2htm::Contracts::quick_enforce>(value.has_value());
    ::pltxt2htm::details::assert_true<::pltxt2htm::Contracts::quick_enforce>(
        value.value<::pltxt2htm::Contracts::quick_enforce>() == 42);

    ::pltxt2htm::container::expected<int, int> error{::pltxt2htm::container::unexpected<int>{7}};
    ::pltxt2htm::details::assert_false<::pltxt2htm::Contracts::quick_enforce>(error.has_value());
    ::pltxt2htm::details::assert_true<::pltxt2htm::Contracts::quick_enforce>(
        error.error<::pltxt2htm::Contracts::quick_enforce>() == 7);

    ::pltxt2htm::container::optional<int> empty{::pltxt2htm::container::nullopt};
    ::pltxt2htm::details::assert_false<::pltxt2htm::Contracts::quick_enforce>(empty.has_value());

    return 0;
}
