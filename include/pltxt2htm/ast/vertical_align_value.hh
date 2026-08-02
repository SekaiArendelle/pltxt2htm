/**
 * @file vertical_align_value.hh
 * @brief Vertical-align value type definitions for pltxt2htm
 * @details Defines the vertical-align value used by &lt;span style="vertical-align:..."&gt;
 *          so the keyword (e.g. super/sub) or numeric length and its unit never
 *          travel independently. Percent/px lengths reuse ::pltxt2htm::FontSizeValue.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string_view.h>
#include "font_size_value.hh"
#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief CSS keyword of a vertical-align value
 * @details The keywords accepted by &lt;span style="vertical-align:..."&gt;. Only
 *          lowercase spellings are accepted during parsing.
 */
enum class VerticalAlignKeyword : unsigned {
    baseline = 0,
    sub,
    super,
    text_top,
    text_bottom,
    middle,
    top,
    bottom,
};

/**
 * @brief Kind of a vertical-align value.
 */
enum class VerticalAlignKind : unsigned {
    keyword = 0, ///< One of the CSS keywords (see ::pltxt2htm::VerticalAlignKeyword).
    length, ///< A px/percent length (stored in `length`).
};

/**
 * @brief A vertical-align value.
 * @details A tagged union discriminated by `kind`: when `kind == keyword`, the
 *          active member is `keyword`; when `kind == length`, the active member
 *          is `length` (a ::pltxt2htm::FontSizeValue). Exactly one member is
 *          active at any time. The `kind` is derived from the constructor used
 *          and needs no explicit argument. Template parameter `ndebug` selects
 *          whether the accessor contract checks are enforced at runtime.
 */
template<::pltxt2htm::Contracts ndebug>
class VerticalAlignValue {
    ::pltxt2htm::VerticalAlignKind kind; ///< Discriminator of the value.

    union {
        ::pltxt2htm::VerticalAlignKeyword keyword; ///< Keyword value (kind == keyword).
        ::pltxt2htm::FontSizeValue length; ///< Length value+unit (kind == length).
    };

public:
    constexpr VerticalAlignValue(::pltxt2htm::VerticalAlignKeyword keyword_) noexcept
        : kind(::pltxt2htm::VerticalAlignKind::keyword),
          keyword(keyword_) {
    }

    constexpr VerticalAlignValue(::pltxt2htm::FontSizeValue length_) noexcept
        : kind(::pltxt2htm::VerticalAlignKind::length),
          length(length_) {
    }

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::VerticalAlignValue<ndebug> const& other) const noexcept -> bool {
        if (kind != other.kind) {
            return false;
        }
        if (kind == ::pltxt2htm::VerticalAlignKind::keyword) {
            return keyword == other.keyword;
        }
        return length == other.length;
    }

    [[nodiscard]]
    constexpr auto get_kind(this auto const& self) noexcept -> VerticalAlignKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto get_keyword(this auto const& self) noexcept -> VerticalAlignKeyword {
        bool const is_keyword{self.kind == ::pltxt2htm::VerticalAlignKind::keyword};
        pltxt2htm_assert(is_keyword, u8"vertical-align kind mismatch");
        return self.keyword;
    }

    [[nodiscard]]
    constexpr auto get_length(this auto&& self) noexcept -> decltype(auto) {
        bool const is_length{self.kind == ::pltxt2htm::VerticalAlignKind::length};
        pltxt2htm_assert(is_length, u8"vertical-align kind mismatch");
        return ::std::forward_like<decltype(self)>(self.length);
    }
};

/**
 * @brief Lowercase CSS spelling of a vertical-align keyword.
 * @param keyword The keyword to stringify.
 * @return The CSS keyword spelling; baseline for any invalid value.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr auto vertical_align_keyword_string(::pltxt2htm::VerticalAlignKeyword const keyword) noexcept
    -> ::fast_io::u8string_view {
    switch (keyword) /* -Werror=switch */ {
    case ::pltxt2htm::VerticalAlignKeyword::sub: {
        return ::fast_io::u8string_view{u8"sub"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::super: {
        return ::fast_io::u8string_view{u8"super"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::text_top: {
        return ::fast_io::u8string_view{u8"text-top"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::text_bottom: {
        return ::fast_io::u8string_view{u8"text-bottom"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::middle: {
        return ::fast_io::u8string_view{u8"middle"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::top: {
        return ::fast_io::u8string_view{u8"top"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::bottom: {
        return ::fast_io::u8string_view{u8"bottom"};
    }
    case ::pltxt2htm::VerticalAlignKeyword::baseline: {
        return ::fast_io::u8string_view{u8"baseline"};
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected vertical-align keyword");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable code after exhaustive switch on vertical-align keyword");
}

} // namespace pltxt2htm

#include "../details/pop_macro.hh"
