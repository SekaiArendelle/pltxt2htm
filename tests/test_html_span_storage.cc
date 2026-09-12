#include "precompile.hh"

#include <array>
#include <pltxt2htm/ast/ast.hh>

namespace {

constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
using FontSize = ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>>;
using VerticalAlign = ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>;

[[nodiscard]]
constexpr auto make_html_span(FontSize font_size, VerticalAlign vertical_align) noexcept
    -> ::pltxt2htm::HtmlSpan<ndebug> {
    return ::pltxt2htm::HtmlSpan<ndebug>{::pltxt2htm::Ast<ndebug>{}, ::pltxt2htm::container::U8String{},
                                         ::std::move(font_size), ::std::move(vertical_align)};
}

static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::HtmlSpan<::pltxt2htm::Contracts::ignore>) == 72);
static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::HtmlSpan<ndebug>) == 72);
static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::ignore>) == 80);
static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::PlTxtNode<ndebug>) == 80);

} // namespace

int main() {
    {
        auto const span = make_html_span(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        pltxt2htm_test_assert_false(span.get_font_size().has_value());
        pltxt2htm_test_assert_false(span.get_vertical_align().has_value());
    }

    {
        static constexpr auto font_sizes = ::std::array{
            ::pltxt2htm::ValueWithUnit<double>{12.5, ::pltxt2htm::Unit::px},
            ::pltxt2htm::ValueWithUnit<double>{80.0, ::pltxt2htm::Unit::percent},
            ::pltxt2htm::ValueWithUnit<double>{1.25, ::pltxt2htm::Unit::em},
        };
        for (auto const font_size : font_sizes) {
            auto const span = make_html_span(FontSize{font_size}, ::pltxt2htm::container::nullopt);
            auto const stored_font_size = span.get_font_size();
            pltxt2htm_test_assert_true(stored_font_size.has_value());
            pltxt2htm_test_assert_true(stored_font_size.template value<ndebug>() == font_size);
            pltxt2htm_test_assert_false(span.get_vertical_align().has_value());
        }
    }

    {
        static constexpr auto keywords = ::std::array{
            ::pltxt2htm::VerticalAlignKeyword::baseline,    ::pltxt2htm::VerticalAlignKeyword::sub,
            ::pltxt2htm::VerticalAlignKeyword::super,       ::pltxt2htm::VerticalAlignKeyword::text_top,
            ::pltxt2htm::VerticalAlignKeyword::text_bottom, ::pltxt2htm::VerticalAlignKeyword::middle,
            ::pltxt2htm::VerticalAlignKeyword::top,         ::pltxt2htm::VerticalAlignKeyword::bottom,
        };
        for (auto const keyword : keywords) {
            auto const expected = ::pltxt2htm::VerticalAlignValue<ndebug>{keyword};
            auto const span = make_html_span(::pltxt2htm::container::nullopt, VerticalAlign{expected});
            auto const stored_vertical_align = span.get_vertical_align();
            pltxt2htm_test_assert_true(stored_vertical_align.has_value());
            pltxt2htm_test_assert_true(stored_vertical_align.template value<ndebug>() == expected);
            pltxt2htm_test_assert_false(span.get_font_size().has_value());
        }
    }

    {
        static constexpr auto lengths = ::std::array{
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{-5, ::pltxt2htm::Unit::px},
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{20, ::pltxt2htm::Unit::percent},
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{10, ::pltxt2htm::Unit::em},
        };
        for (auto const length : lengths) {
            auto const expected = ::pltxt2htm::VerticalAlignValue<ndebug>{length};
            auto const span = make_html_span(::pltxt2htm::container::nullopt, VerticalAlign{expected});
            auto const stored_vertical_align = span.get_vertical_align();
            pltxt2htm_test_assert_true(stored_vertical_align.has_value());
            pltxt2htm_test_assert_true(stored_vertical_align.template value<ndebug>() == expected);
        }
    }

    {
        auto const font_size = FontSize{::pltxt2htm::ValueWithUnit<double>{12.5, ::pltxt2htm::Unit::px}};
        auto const vertical_align =
            VerticalAlign{::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::super}};
        auto const same_left = make_html_span(font_size, vertical_align);
        auto const same_right = make_html_span(font_size, vertical_align);
        auto const no_font_size = make_html_span(::pltxt2htm::container::nullopt, vertical_align);
        auto const zero_font_size =
            make_html_span(FontSize{::pltxt2htm::ValueWithUnit<double>{0.0, ::pltxt2htm::Unit::px}}, vertical_align);
        auto const different_keyword = make_html_span(
            font_size, VerticalAlign{::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::sub}});

        pltxt2htm_test_assert_true(same_left == same_right);
        pltxt2htm_test_assert_false(no_font_size == zero_font_size);
        pltxt2htm_test_assert_false(same_left == different_keyword);
    }
}
