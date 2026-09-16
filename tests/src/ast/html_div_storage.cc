#include "precompile.hh"

#include <array>
#include <pltxt2htm/ast/ast.hh>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
using Margin = ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>>;

[[nodiscard]]
constexpr auto make_html_div(Margin left, Margin right) noexcept -> ::pltxt2htm::HtmlDiv<ndebug> {
    return ::pltxt2htm::HtmlDiv<ndebug>{::pltxt2htm::Ast<ndebug>{}, ::std::move(left), ::std::move(right)};
}

static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::HtmlDiv<::pltxt2htm::Contracts::ignore>) == 48);
static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::HtmlDiv<ndebug>) == 48);

} // namespace

int main() {
    {
        auto const div = make_html_div(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        pltxt2htm_test_assert_false(div.get_left().has_value());
        pltxt2htm_test_assert_false(div.get_right().has_value());
    }

    static constexpr auto margins = ::std::array{
        ::pltxt2htm::ValueWithUnit<::std::size_t>{0, ::pltxt2htm::Unit::px},
        ::pltxt2htm::ValueWithUnit<::std::size_t>{20, ::pltxt2htm::Unit::percent},
        ::pltxt2htm::ValueWithUnit<::std::size_t>{10, ::pltxt2htm::Unit::em},
    };
    for (auto const margin : margins) {
        auto const div = make_html_div(Margin{margin}, Margin{margin});
        pltxt2htm_test_assert_true(div.get_left().template value<ndebug>() == margin);
        pltxt2htm_test_assert_true(div.get_right().template value<ndebug>() == margin);
    }

    {
        auto const margin = Margin{margins[1]};
        auto const same_left = make_html_div(margin, ::pltxt2htm::container::nullopt);
        auto const same_right = make_html_div(margin, ::pltxt2htm::container::nullopt);
        auto const absent = make_html_div(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        auto const zero = make_html_div(Margin{::pltxt2htm::ValueWithUnit<::std::size_t>{0, ::pltxt2htm::Unit::px}},
                                        ::pltxt2htm::container::nullopt);
        auto const different_unit =
            make_html_div(Margin{::pltxt2htm::ValueWithUnit<::std::size_t>{20, ::pltxt2htm::Unit::em}},
                          ::pltxt2htm::container::nullopt);

        pltxt2htm_test_assert_true(same_left == same_right);
        pltxt2htm_test_assert_false(absent == zero);
        pltxt2htm_test_assert_false(same_left == different_unit);
    }
}
