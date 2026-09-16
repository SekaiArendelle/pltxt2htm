#include "precompile.hh"

#include <array>
#include <pltxt2htm/ast/ast.hh>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
using Margin = ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>>;

[[nodiscard]]
constexpr auto make_unity_margin(Margin left, Margin right) noexcept -> ::pltxt2htm::UnityMargin<ndebug> {
    return ::pltxt2htm::UnityMargin<ndebug>{::pltxt2htm::Ast<ndebug>{}, ::std::move(left), ::std::move(right)};
}

static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::UnityMargin<::pltxt2htm::Contracts::ignore>) == 48);
static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::UnityMargin<ndebug>) == 48);

} // namespace

int main() {
    {
        auto const margin_node = make_unity_margin(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        pltxt2htm_test_assert_false(margin_node.get_left().has_value());
        pltxt2htm_test_assert_false(margin_node.get_right().has_value());
    }

    static constexpr auto margins = ::std::array{
        ::pltxt2htm::ValueWithUnit<::std::size_t>{0, ::pltxt2htm::Unit::px},
        ::pltxt2htm::ValueWithUnit<::std::size_t>{20, ::pltxt2htm::Unit::percent},
        ::pltxt2htm::ValueWithUnit<::std::size_t>{10, ::pltxt2htm::Unit::em},
    };
    for (auto const margin : margins) {
        auto const margin_node = make_unity_margin(Margin{margin}, Margin{margin});
        pltxt2htm_test_assert_true(margin_node.get_left().template value<ndebug>() == margin);
        pltxt2htm_test_assert_true(margin_node.get_right().template value<ndebug>() == margin);
    }

    {
        auto const margin = Margin{margins[1]};
        auto const same_left = make_unity_margin(margin, ::pltxt2htm::container::nullopt);
        auto const same_right = make_unity_margin(margin, ::pltxt2htm::container::nullopt);
        auto const absent = make_unity_margin(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        auto const zero = make_unity_margin(Margin{::pltxt2htm::ValueWithUnit<::std::size_t>{0, ::pltxt2htm::Unit::px}},
                                            ::pltxt2htm::container::nullopt);
        auto const different_unit =
            make_unity_margin(Margin{::pltxt2htm::ValueWithUnit<::std::size_t>{20, ::pltxt2htm::Unit::em}},
                              ::pltxt2htm::container::nullopt);

        pltxt2htm_test_assert_true(same_left == same_right);
        pltxt2htm_test_assert_false(absent == zero);
        pltxt2htm_test_assert_false(same_left == different_unit);
    }
}
