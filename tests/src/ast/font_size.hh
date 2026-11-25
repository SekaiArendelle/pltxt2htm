#pragma once

#include <limits>

#include <pltxt2htm/ast/font_size.hh>

#include "doctest_config.hh"

TEST_SUITE("font_size") {
    TEST_CASE("domain-boundaries") {
        using ::pltxt2htm::FontSize;
        using ::pltxt2htm::Unit;

        CHECK(FontSize::min_value == 0);
        CHECK(FontSize::max_value == 1000000);

        CHECK(FontSize::is_representable(0));
        CHECK(FontSize::is_representable(-0.0));
        CHECK(FontSize::is_representable(FontSize::max_value));
        CHECK_FALSE(FontSize::is_representable(-1.0));
        CHECK_FALSE(FontSize::is_representable(FontSize::max_value + 1));
        CHECK_FALSE(FontSize::is_representable(::std::numeric_limits<double>::infinity()));
        CHECK_FALSE(FontSize::is_representable(::std::numeric_limits<double>::quiet_NaN()));

        CHECK(FontSize::try_make(12.5, Unit::px).has_value());
        CHECK(FontSize::try_make(FontSize::min_value, Unit::px).has_value());
        CHECK(FontSize::try_make(FontSize::max_value, Unit::px).has_value());
        CHECK_FALSE(FontSize::try_make(-1.0, Unit::px).has_value());
        CHECK_FALSE(FontSize::try_make(FontSize::max_value + 1, Unit::px).has_value());
        CHECK_FALSE(FontSize::try_make(::std::numeric_limits<double>::infinity(), Unit::px).has_value());
        CHECK_FALSE(FontSize::try_make(::std::numeric_limits<double>::quiet_NaN(), Unit::px).has_value());
    }

    TEST_CASE("accessors") {
        using ::pltxt2htm::FontSize;
        using ::pltxt2htm::Unit;

        auto const font_size = FontSize::try_make(12.5, Unit::em).value();
        CHECK(font_size.get_value() == 12.5);
        CHECK(font_size.get_unit() == Unit::em);

        auto const default_font_size = FontSize{};
        CHECK(default_font_size.get_value() == 0);
        CHECK(default_font_size.get_unit() == Unit::px);
    }

    TEST_CASE("constexpr-evaluation") {
        using ::pltxt2htm::FontSize;
        using ::pltxt2htm::Unit;

        constexpr auto font_size = FontSize::try_make(12.5, Unit::px);
        static_assert(font_size.has_value());
        static_assert(font_size.value().get_value() == 12.5);
        static_assert(font_size.value().get_unit() == Unit::px);
        static_assert(FontSize::try_make(FontSize::max_value + 1, Unit::px).has_value() == false);
    }
}