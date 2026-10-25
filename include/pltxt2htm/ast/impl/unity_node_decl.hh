/**
 * @file unity_node_decl.hh
 * @brief Unity rich-text AST node declarations for pltxt2htm
 * @details Defines nodes for supported Unity rich-text formatting and link tags.
 */

#pragma once

#include <cstddef>
#include <utility>
#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "ast_decl.hh"
#include "url_node_decl.hh"
#include "../value_unit.hh"
#include "../node_kind.hh"
#include "../../contracts.hh"

namespace pltxt2htm {

/**
 * @brief Unity color tag node
 * @details Represents &lt;color=value&gt;...&lt;/color&gt; with a color string and sub-AST.
 */
class UnityColor {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::container::U8String color;

public:
    constexpr UnityColor(::pltxt2htm::Ast&& subast_, ::pltxt2htm::container::U8String&& color_) noexcept;
    constexpr UnityColor(::pltxt2htm::UnityColor const&) noexcept = default;
    constexpr UnityColor(::pltxt2htm::UnityColor&&) noexcept;
    constexpr ~UnityColor() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityColor const&) noexcept
        -> ::pltxt2htm::UnityColor& = default;
    constexpr auto operator=(this UnityColor& self, ::pltxt2htm::UnityColor&&) noexcept
        -> ::pltxt2htm::UnityColor&;

    [[nodiscard]]
    constexpr auto operator==(this UnityColor const&, UnityColor const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_color(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.color);
    }
};

/**
 * @brief Unity link tag node
 * @details Represents &lt;link=&quot;url&quot;&gt;...&lt;/link&gt; with a URL.
 */
class UnityLink {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::Url url;

public:
    constexpr UnityLink(::pltxt2htm::Ast&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr UnityLink(::pltxt2htm::UnityLink const&) noexcept = default;
    constexpr UnityLink(::pltxt2htm::UnityLink&&) noexcept;
    constexpr ~UnityLink() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityLink const&) noexcept
        -> ::pltxt2htm::UnityLink& = default;
    constexpr auto operator=(this UnityLink& self, ::pltxt2htm::UnityLink&&) noexcept
        -> ::pltxt2htm::UnityLink&;

    [[nodiscard]]
    constexpr auto operator==(this UnityLink const&, UnityLink const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }
};

/**
 * @brief Unity font size tag node
 * @details Represents &lt;size=value&gt;...&lt;/size&gt; or &lt;size=value%&gt;...&lt;/size&gt;
 *          (TMP_Text rich text) with a font size value and its unit.
 */
class UnitySize {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::ValueWithUnit<double> font_size;

public:
    constexpr UnitySize(::pltxt2htm::Ast&& subast_, ::pltxt2htm::ValueWithUnit<double> font_size_) noexcept;
    constexpr UnitySize(::pltxt2htm::UnitySize const&) noexcept = default;
    constexpr UnitySize(::pltxt2htm::UnitySize&&) noexcept;
    constexpr ~UnitySize() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnitySize const&) noexcept
        -> ::pltxt2htm::UnitySize& = default;
    constexpr auto operator=(this UnitySize& self, ::pltxt2htm::UnitySize&&) noexcept
        -> ::pltxt2htm::UnitySize&;

    [[nodiscard]]
    constexpr auto operator==(this UnitySize const&, UnitySize const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_size(this auto const& self) noexcept -> double {
        return self.font_size.value;
    }

    [[nodiscard]]
    constexpr auto get_unit(this auto const& self) noexcept -> ::pltxt2htm::Unit {
        return self.font_size.unit;
    }

    [[nodiscard]]
    constexpr auto get_font_size(this UnitySize const& self) noexcept -> ::pltxt2htm::ValueWithUnit<double> {
        return self.font_size;
    }
};

/**
 * @brief Unity vertical offset tag node
 * @details Represents &lt;voffset=value&gt;...&lt;/voffset&gt; (Unity TextMeshPro rich text)
 *          with a signed px/em offset (negative shifts text down; the unit defaults to px)
 *          and sub-AST.
 */
class UnityVoffset {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value;

public:
    constexpr UnityVoffset(::pltxt2htm::Ast&& subast_,
                           ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value_) noexcept;
    constexpr UnityVoffset(::pltxt2htm::UnityVoffset const&) noexcept = default;
    constexpr UnityVoffset(::pltxt2htm::UnityVoffset&&) noexcept;
    constexpr ~UnityVoffset() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityVoffset const&) noexcept
        -> ::pltxt2htm::UnityVoffset& = default;
    constexpr auto operator=(this UnityVoffset& self, ::pltxt2htm::UnityVoffset&&) noexcept
        -> ::pltxt2htm::UnityVoffset&;

    [[nodiscard]]
    constexpr auto operator==(this UnityVoffset const&, UnityVoffset const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_value(this auto const& self) noexcept -> ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> {
        return self.value;
    }
};

/**
 * @brief Unity alignment tag node
 * @details Represents &lt;align=value&gt;...&lt;/align&gt; (Unity TextMeshPro rich text)
 *          with a text-alignment keyword and sub-AST.
 */
class UnityAlign {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::TextAlign align;

public:
    constexpr UnityAlign(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TextAlign align_) noexcept;
    constexpr UnityAlign(::pltxt2htm::UnityAlign const&) noexcept = default;
    constexpr UnityAlign(::pltxt2htm::UnityAlign&&) noexcept;
    constexpr ~UnityAlign() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityAlign const&) noexcept
        -> ::pltxt2htm::UnityAlign& = default;
    constexpr auto operator=(this UnityAlign& self, ::pltxt2htm::UnityAlign&&) noexcept
        -> ::pltxt2htm::UnityAlign&;

    [[nodiscard]]
    constexpr auto operator==(this UnityAlign const&, UnityAlign const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto const& self) noexcept -> ::pltxt2htm::TextAlign {
        return self.align;
    }
};

/**
 * @brief Unity mark tag node
 * @details Represents &lt;mark=value&gt;...&lt;/mark&gt; (TMP rich text) with a background
 *          color string and sub-AST.
 */
class UnityMark {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::container::U8String background_color;

public:
    constexpr UnityMark(::pltxt2htm::Ast&& subast_,
                        ::pltxt2htm::container::U8String&& background_color_) noexcept;
    constexpr UnityMark(::pltxt2htm::UnityMark const&) noexcept = default;
    constexpr UnityMark(::pltxt2htm::UnityMark&&) noexcept;
    constexpr ~UnityMark() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityMark const&) noexcept
        -> ::pltxt2htm::UnityMark& = default;
    constexpr auto operator=(this UnityMark& self, ::pltxt2htm::UnityMark&&) noexcept
        -> ::pltxt2htm::UnityMark&;

    [[nodiscard]]
    constexpr auto operator==(this UnityMark const&, UnityMark const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_background_color(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.background_color);
    }
};

/**
 * @brief Unity margin tag node
 * @details Represents &lt;margin left=v right=v&gt; / &lt;margin-left=v&gt; /
 *          &lt;margin-right=v&gt; / &lt;margin=v&gt; ... &lt;/margin&gt; (Unity TextMeshPro rich
 *          text) with optional left and right unsigned px/em/% values (the unit
 *          defaults to px), and sub-AST.
 */
class UnityMargin {
    struct MarginStorage {
        ::std::size_t left_value{};
        ::std::size_t right_value{};
        ::pltxt2htm::Unit left_unit : 2 {::pltxt2htm::Unit::px};
        ::pltxt2htm::Unit right_unit : 2 {::pltxt2htm::Unit::px};
        bool has_left : 1 {};
        bool has_right : 1 {};

        constexpr MarginStorage(
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> const& left,
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> const& right) noexcept {
            if (left.has_value()) {
                auto const& value = left.value();
                left_value = value.value;
                left_unit = value.unit;
                has_left = true;
            }
            if (right.has_value()) {
                auto const& value = right.value();
                right_value = value.value;
                right_unit = value.unit;
                has_right = true;
            }
        }

        [[nodiscard]]
        constexpr auto operator==(this MarginStorage const& self, MarginStorage const& other) noexcept -> bool {
            bool const left_equal{
                self.has_left == other.has_left &&
                (self.has_left == false || (self.left_value == other.left_value && self.left_unit == other.left_unit))};
            return left_equal && self.has_right == other.has_right &&
                   (self.has_right == false ||
                    (self.right_value == other.right_value && self.right_unit == other.right_unit));
        }

        [[nodiscard]]
        constexpr auto get_left(this MarginStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
            if (self.has_left == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return ::pltxt2htm::ValueWithUnit<::std::size_t>{.value = self.left_value, .unit = self.left_unit};
        }

        [[nodiscard]]
        constexpr auto get_right(this MarginStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
            if (self.has_right == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return ::pltxt2htm::ValueWithUnit<::std::size_t>{.value = self.right_value, .unit = self.right_unit};
        }
    };

    ::pltxt2htm::Ast subast;
    MarginStorage margins;

public:
    constexpr UnityMargin(::pltxt2htm::Ast&& subast_,
                          ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
                          ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept;
    constexpr UnityMargin(::pltxt2htm::UnityMargin const&) noexcept = default;
    constexpr UnityMargin(::pltxt2htm::UnityMargin&&) noexcept;
    constexpr ~UnityMargin() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityMargin const&) noexcept
        -> ::pltxt2htm::UnityMargin& = default;
    constexpr auto operator=(this UnityMargin& self, ::pltxt2htm::UnityMargin&&) noexcept
        -> ::pltxt2htm::UnityMargin&;

    [[nodiscard]]
    constexpr auto operator==(this UnityMargin const&, UnityMargin const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_left(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.margins.get_left();
    }

    [[nodiscard]]
    constexpr auto get_right(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.margins.get_right();
    }
};

/**
 * @brief Unity italic text tag node
 * @details Represents &lt;i&gt;...&lt;/i&gt; with sub-AST content.
 */
class UnityI {
    ::pltxt2htm::Ast subast;

public:
    constexpr UnityI(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr UnityI(::pltxt2htm::UnityI const&) noexcept = default;
    constexpr UnityI(::pltxt2htm::UnityI&&) noexcept;
    constexpr ~UnityI() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityI const&) noexcept -> ::pltxt2htm::UnityI& = default;
    constexpr auto operator=(this UnityI& self, ::pltxt2htm::UnityI&&) noexcept
        -> ::pltxt2htm::UnityI&;

    [[nodiscard]]
    constexpr auto operator==(this UnityI const&, UnityI const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Unity bold text tag node
 * @details Represents &lt;b&gt;...&lt;/b&gt; with sub-AST content.
 */
class UnityB {
    ::pltxt2htm::Ast subast;

public:
    constexpr UnityB(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr UnityB(::pltxt2htm::UnityB const&) noexcept = default;
    constexpr UnityB(::pltxt2htm::UnityB&&) noexcept;
    constexpr ~UnityB() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityB const&) noexcept -> ::pltxt2htm::UnityB& = default;
    constexpr auto operator=(this UnityB& self, ::pltxt2htm::UnityB&&) noexcept
        -> ::pltxt2htm::UnityB&;

    [[nodiscard]]
    constexpr auto operator==(this UnityB const&, UnityB const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm
