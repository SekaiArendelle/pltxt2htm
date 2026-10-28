#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/container/array.hh>

TEST_SUITE("html_span_storage") {
    using FontSize = ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>>;
    using VerticalAlign = ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue>;

    [[nodiscard]]
    constexpr auto make_html_span(FontSize font_size, VerticalAlign vertical_align) noexcept -> ::pltxt2htm::HtmlSpan {
        return ::pltxt2htm::HtmlSpan{::pltxt2htm::Ast{}, ::pltxt2htm::container::U8String{}, ::std::move(font_size),
                                     ::std::move(vertical_align)};
    }

    static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::HtmlSpan) == 72);
    static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::PlTxtNode) == 80);

    TEST_CASE("absent font size and vertical alignment") {
        auto const span = make_html_span(::pltxt2htm::container::nullopt, ::pltxt2htm::container::nullopt);
        CHECK_FALSE(span.get_font_size().has_value());
        CHECK_FALSE(span.get_vertical_align().has_value());
    }

    TEST_CASE("font sizes") {
        static constexpr auto font_sizes = ::pltxt2htm::container::Array{
            ::pltxt2htm::ValueWithUnit<double>{12.5, ::pltxt2htm::Unit::px},
            ::pltxt2htm::ValueWithUnit<double>{80.0, ::pltxt2htm::Unit::percent},
            ::pltxt2htm::ValueWithUnit<double>{1.25, ::pltxt2htm::Unit::em},
        };
        for (auto const font_size : font_sizes) {
            auto const span = make_html_span(FontSize{font_size}, ::pltxt2htm::container::nullopt);
            auto const stored_font_size = span.get_font_size();
            REQUIRE(stored_font_size.has_value());
            CHECK(stored_font_size.value() == font_size);
            CHECK_FALSE(span.get_vertical_align().has_value());
        }
    }

    TEST_CASE("vertical alignment keywords") {
        static constexpr auto keywords = ::pltxt2htm::container::Array{
            ::pltxt2htm::VerticalAlignKeyword::baseline,    ::pltxt2htm::VerticalAlignKeyword::sub,
            ::pltxt2htm::VerticalAlignKeyword::super,       ::pltxt2htm::VerticalAlignKeyword::text_top,
            ::pltxt2htm::VerticalAlignKeyword::text_bottom, ::pltxt2htm::VerticalAlignKeyword::middle,
            ::pltxt2htm::VerticalAlignKeyword::top,         ::pltxt2htm::VerticalAlignKeyword::bottom,
        };
        for (auto const keyword : keywords) {
            auto const expected = ::pltxt2htm::VerticalAlignValue{keyword};
            auto const span = make_html_span(::pltxt2htm::container::nullopt, VerticalAlign{expected});
            auto const stored_vertical_align = span.get_vertical_align();
            REQUIRE(stored_vertical_align.has_value());
            CHECK(stored_vertical_align.value() == expected);
            CHECK_FALSE(span.get_font_size().has_value());
        }
    }

    TEST_CASE("vertical alignment lengths") {
        static constexpr auto lengths = ::pltxt2htm::container::Array{
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{-5, ::pltxt2htm::Unit::px},
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{20, ::pltxt2htm::Unit::percent},
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{10, ::pltxt2htm::Unit::em},
        };
        for (auto const length : lengths) {
            auto const expected = ::pltxt2htm::VerticalAlignValue{length};
            auto const span = make_html_span(::pltxt2htm::container::nullopt, VerticalAlign{expected});
            auto const stored_vertical_align = span.get_vertical_align();
            REQUIRE(stored_vertical_align.has_value());
            CHECK(stored_vertical_align.value() == expected);
        }
    }

    TEST_CASE("span equality") {
        auto const font_size = FontSize{::pltxt2htm::ValueWithUnit<double>{12.5, ::pltxt2htm::Unit::px}};
        auto const vertical_align =
            VerticalAlign{::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::super}};
        auto const same_left = make_html_span(font_size, vertical_align);
        auto const same_right = make_html_span(font_size, vertical_align);
        auto const no_font_size = make_html_span(::pltxt2htm::container::nullopt, vertical_align);
        auto const zero_font_size =
            make_html_span(FontSize{::pltxt2htm::ValueWithUnit<double>{0.0, ::pltxt2htm::Unit::px}}, vertical_align);
        auto const different_keyword = make_html_span(
            font_size, VerticalAlign{::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::sub}});

        CHECK(same_left == same_right);
        CHECK_FALSE(no_font_size == zero_font_size);
        CHECK_FALSE(same_left == different_keyword);
    }
}
