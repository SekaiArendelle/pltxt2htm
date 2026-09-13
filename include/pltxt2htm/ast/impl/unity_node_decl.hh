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
#include "basic_node_decl.hh"
#include "../value_unit.hh"
#include "../node_kind.hh"
#include "../../contracts.hh"

namespace pltxt2htm {

/**
 * @brief Unity color tag node
 * @details Represents &lt;color=value&gt;...&lt;/color&gt; with a color string and sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
class UnityColor {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String color;

public:
    constexpr UnityColor(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::container::U8String&& color_) noexcept;
    constexpr UnityColor(::pltxt2htm::UnityColor<ndebug> const&) noexcept;
    constexpr UnityColor(::pltxt2htm::UnityColor<ndebug>&&) noexcept;
    constexpr ~UnityColor() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityColor<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityColor<ndebug>& = delete;
    constexpr auto operator=(this UnityColor<ndebug>& self, ::pltxt2htm::UnityColor<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityColor<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnityLink {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::Url url;

public:
    constexpr UnityLink(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr UnityLink(::pltxt2htm::UnityLink<ndebug> const&) noexcept;
    constexpr UnityLink(::pltxt2htm::UnityLink<ndebug>&&) noexcept;
    constexpr ~UnityLink() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityLink<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityLink<ndebug>& = delete;
    constexpr auto operator=(this UnityLink<ndebug>& self, ::pltxt2htm::UnityLink<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityLink<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnitySize {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::ValueWithUnit<double> font_size;

public:
    constexpr UnitySize(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::ValueWithUnit<double> font_size_) noexcept;
    constexpr UnitySize(::pltxt2htm::UnitySize<ndebug> const&) noexcept;
    constexpr UnitySize(::pltxt2htm::UnitySize<ndebug>&&) noexcept;
    constexpr ~UnitySize() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnitySize<ndebug> const&) noexcept
        -> ::pltxt2htm::UnitySize<ndebug>& = delete;
    constexpr auto operator=(this UnitySize<ndebug>& self, ::pltxt2htm::UnitySize<ndebug>&&) noexcept
        -> ::pltxt2htm::UnitySize<ndebug>&;

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
    constexpr auto get_font_size(this UnitySize<ndebug> const& self) noexcept -> ::pltxt2htm::ValueWithUnit<double> {
        return self.font_size;
    }
};

/**
 * @brief Unity vertical offset tag node
 * @details Represents &lt;voffset=value&gt;...&lt;/voffset&gt; (Unity TextMeshPro rich text)
 *          with a signed px/em offset (negative shifts text down; the unit defaults to px)
 *          and sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
class UnityVoffset {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value;

public:
    constexpr UnityVoffset(::pltxt2htm::Ast<ndebug>&& subast_,
                           ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value_) noexcept;
    constexpr UnityVoffset(::pltxt2htm::UnityVoffset<ndebug> const&) noexcept;
    constexpr UnityVoffset(::pltxt2htm::UnityVoffset<ndebug>&&) noexcept;
    constexpr ~UnityVoffset() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityVoffset<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityVoffset<ndebug>& = delete;
    constexpr auto operator=(this UnityVoffset<ndebug>& self, ::pltxt2htm::UnityVoffset<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityVoffset<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnityAlign {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::TextAlign align;

public:
    constexpr UnityAlign(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::TextAlign align_) noexcept;
    constexpr UnityAlign(::pltxt2htm::UnityAlign<ndebug> const&) noexcept;
    constexpr UnityAlign(::pltxt2htm::UnityAlign<ndebug>&&) noexcept;
    constexpr ~UnityAlign() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityAlign<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityAlign<ndebug>& = delete;
    constexpr auto operator=(this UnityAlign<ndebug>& self, ::pltxt2htm::UnityAlign<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityAlign<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnityMark {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::U8String background_color;

public:
    constexpr UnityMark(::pltxt2htm::Ast<ndebug>&& subast_,
                        ::pltxt2htm::container::U8String&& background_color_) noexcept;
    constexpr UnityMark(::pltxt2htm::UnityMark<ndebug> const&) noexcept;
    constexpr UnityMark(::pltxt2htm::UnityMark<ndebug>&&) noexcept;
    constexpr ~UnityMark() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityMark<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityMark<ndebug>& = delete;
    constexpr auto operator=(this UnityMark<ndebug>& self, ::pltxt2htm::UnityMark<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityMark<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnityMargin {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;

public:
    constexpr UnityMargin(::pltxt2htm::Ast<ndebug>&& subast_,
                          ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
                          ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept;
    constexpr UnityMargin(::pltxt2htm::UnityMargin<ndebug> const&) noexcept;
    constexpr UnityMargin(::pltxt2htm::UnityMargin<ndebug>&&) noexcept;
    constexpr ~UnityMargin() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityMargin<ndebug> const&) noexcept
        -> ::pltxt2htm::UnityMargin<ndebug>& = delete;
    constexpr auto operator=(this UnityMargin<ndebug>& self, ::pltxt2htm::UnityMargin<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityMargin<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this UnityMargin const&, UnityMargin const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_left(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.left;
    }

    [[nodiscard]]
    constexpr auto get_right(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.right;
    }
};

/**
 * @brief Unity italic text tag node
 * @details Represents &lt;i&gt;...&lt;/i&gt; with sub-AST content.
 */
template<::pltxt2htm::Contracts ndebug>
class UnityI {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr UnityI(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr UnityI(::pltxt2htm::UnityI<ndebug> const&) noexcept;
    constexpr UnityI(::pltxt2htm::UnityI<ndebug>&&) noexcept;
    constexpr ~UnityI() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityI<ndebug> const&) noexcept -> ::pltxt2htm::UnityI<ndebug>& = delete;
    constexpr auto operator=(this UnityI<ndebug>& self, ::pltxt2htm::UnityI<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityI<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class UnityB {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr UnityB(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr UnityB(::pltxt2htm::UnityB<ndebug> const&) noexcept;
    constexpr UnityB(::pltxt2htm::UnityB<ndebug>&&) noexcept;
    constexpr ~UnityB() noexcept = default;
    constexpr auto operator=(::pltxt2htm::UnityB<ndebug> const&) noexcept -> ::pltxt2htm::UnityB<ndebug>& = delete;
    constexpr auto operator=(this UnityB<ndebug>& self, ::pltxt2htm::UnityB<ndebug>&&) noexcept
        -> ::pltxt2htm::UnityB<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this UnityB const&, UnityB const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm
