/**
 * @file ast.hh
 * @brief Owned fenced-code intermediate representations.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include "../../container/optional.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../container/vector.hh"
#include "../../contracts.hh"
#include "../value_unit.hh"
#include "../vertical_align_value.hh"
#include "node.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
class RenderedCodeStyle {
    ::pltxt2htm::container::U8String color;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};

public:
    constexpr RenderedCodeStyle() noexcept = default;

    constexpr RenderedCodeStyle(
        ::pltxt2htm::container::U8String&& color_value,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_value,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>&& vertical_align_value) noexcept
        : color(::std::move(color_value)),
          font_size(font_size_value),
          vertical_align(::std::move(vertical_align_value)) {
    }

    constexpr RenderedCodeStyle(RenderedCodeStyle const&) noexcept = default;
    constexpr RenderedCodeStyle(RenderedCodeStyle&&) noexcept = default;
    constexpr ~RenderedCodeStyle() noexcept = default;
    constexpr auto operator=(RenderedCodeStyle const&) noexcept -> RenderedCodeStyle& = delete;
    constexpr auto operator=(this RenderedCodeStyle& self, RenderedCodeStyle&& other) noexcept
        -> RenderedCodeStyle& = default;

    [[nodiscard]]
    constexpr auto operator==(this RenderedCodeStyle const&, RenderedCodeStyle const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto get_color(this RenderedCodeStyle const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        return ::pltxt2htm::container::U8StringView{self.color.data(), self.color.size()};
    }

    [[nodiscard]]
    constexpr auto get_font_size(this RenderedCodeStyle const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const& {
        return self.font_size;
    }

    [[nodiscard]]
    constexpr auto get_vertical_align(this RenderedCodeStyle const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> const& {
        return self.vertical_align;
    }
};

/**
 * @brief Language-neutral syntax-highlighting IR.
 */
template<::pltxt2htm::Contracts ndebug>
class HighlightedCodeAst {
    ::pltxt2htm::container::U8String source;
    ::pltxt2htm::container::Vector<::pltxt2htm::CodeNode> nodes;

public:
    constexpr HighlightedCodeAst() noexcept = default;
    constexpr HighlightedCodeAst(HighlightedCodeAst const&) noexcept = default;
    constexpr HighlightedCodeAst(HighlightedCodeAst&&) noexcept = default;
    constexpr ~HighlightedCodeAst() noexcept = default;

    constexpr auto operator=(this HighlightedCodeAst& self, HighlightedCodeAst const& other) noexcept -> HighlightedCodeAst& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self = HighlightedCodeAst{other};
        return self;
    }

    constexpr auto operator=(this HighlightedCodeAst& self, HighlightedCodeAst&& other) noexcept -> HighlightedCodeAst& = default;

    [[nodiscard]]
    constexpr auto operator==(this HighlightedCodeAst const&, HighlightedCodeAst const&) noexcept -> bool = default;

    constexpr void reserve(this HighlightedCodeAst& self, ::std::size_t const source_size) noexcept {
        self.source.template reserve<ndebug>(source_size);
    }

    constexpr void append(this HighlightedCodeAst& self, ::pltxt2htm::container::U8StringView const text,
                          ::pltxt2htm::CodeHighlightKind const kind) noexcept {
        if (text.is_empty()) {
            return;
        }
        ::std::size_t const begin{self.source.size()};
        self.source.template append<ndebug>(text);
        ::std::size_t const end{self.source.size()};
        if (self.nodes.is_empty() == false) {
            auto& last = self.nodes.template index<ndebug>(self.nodes.size() - 1);
            if (last.kind == kind && last.end == begin) {
                last.end = end;
                return;
            }
        }
        self.nodes.template push_back<ndebug>(::pltxt2htm::CodeNode{begin, end, kind});
    }

    constexpr void append(this HighlightedCodeAst& self, ::pltxt2htm::container::U8String& text,
                          ::pltxt2htm::CodeHighlightKind const kind) noexcept {
        self.append(::pltxt2htm::container::U8StringView{text}, kind);
        text.clear();
    }

    [[nodiscard]]
    constexpr auto get_nodes(this HighlightedCodeAst const& self) noexcept
        -> ::pltxt2htm::container::Vector<::pltxt2htm::CodeNode> const& {
        return self.nodes;
    }

    [[nodiscard]]
    constexpr auto get_text(this HighlightedCodeAst const& self, ::pltxt2htm::CodeNode const& node) noexcept
        -> ::pltxt2htm::container::U8StringView {
        pltxt2htm_assert(node.begin <= node.end && node.end <= self.source.size(),
                         u8"code node source range out of bounds");
        return ::pltxt2htm::container::U8StringView{self.source.data() + node.begin, node.end - node.begin};
    }
};

/**
 * @brief IR for code that already carries explicit inline presentation.
 */
template<::pltxt2htm::Contracts ndebug>
class RenderedCodeAst {
    ::pltxt2htm::container::U8String source;
    ::pltxt2htm::container::Vector<::pltxt2htm::RenderedCodeNode> nodes;
    ::pltxt2htm::container::Vector<::pltxt2htm::RenderedCodeStyle<ndebug>> styles;

    constexpr void append(this RenderedCodeAst& self, ::pltxt2htm::container::U8StringView const text,
                          ::pltxt2htm::RenderedCodeNodeKind const kind) noexcept {
        if (text.is_empty()) {
            return;
        }
        ::std::size_t const begin{self.source.size()};
        self.source.template append<ndebug>(text);
        ::std::size_t const end{self.source.size()};
        if (kind == ::pltxt2htm::RenderedCodeNodeKind::text && self.nodes.is_empty() == false) {
            auto& last = self.nodes.template index<ndebug>(self.nodes.size() - 1);
            if (last.kind == kind && last.metadata == 0 && last.end == begin) {
                last.end = end;
                return;
            }
        }
        self.nodes.template push_back<ndebug>(::pltxt2htm::RenderedCodeNode{begin, end, 0, kind});
    }

public:
    constexpr RenderedCodeAst() noexcept = default;
    constexpr RenderedCodeAst(RenderedCodeAst const&) noexcept = default;
    constexpr RenderedCodeAst(RenderedCodeAst&&) noexcept = default;
    constexpr ~RenderedCodeAst() noexcept = default;

    constexpr auto operator=(this RenderedCodeAst& self, RenderedCodeAst const& other) noexcept -> RenderedCodeAst& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self = RenderedCodeAst{other};
        return self;
    }

    constexpr auto operator=(this RenderedCodeAst& self, RenderedCodeAst&& other) noexcept
        -> RenderedCodeAst& = default;

    [[nodiscard]]
    constexpr auto operator==(this RenderedCodeAst const&, RenderedCodeAst const&) noexcept -> bool = default;

    constexpr void reserve(this RenderedCodeAst& self, ::std::size_t const source_size) noexcept {
        self.source.template reserve<ndebug>(source_size);
    }

    constexpr auto add_style(
        this RenderedCodeAst& self, ::pltxt2htm::container::U8String&& color,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const font_size,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>&& vertical_align) noexcept
        -> ::std::size_t {
        ::std::size_t const index{self.styles.size()};
        self.styles.template push_back<ndebug>(
            ::pltxt2htm::RenderedCodeStyle<ndebug>{::std::move(color), font_size, ::std::move(vertical_align)});
        return index;
    }

    constexpr void append_style_begin(this RenderedCodeAst& self, ::std::size_t const style_index) noexcept {
        pltxt2htm_assert(style_index < self.styles.size(), u8"rendered style index out of bounds");
        ::std::size_t const position{self.source.size()};
        self.nodes.template push_back<ndebug>(::pltxt2htm::RenderedCodeNode{
            position, position, style_index, ::pltxt2htm::RenderedCodeNodeKind::style_begin});
    }

    constexpr void append_style_end(this RenderedCodeAst& self) noexcept {
        ::std::size_t const position{self.source.size()};
        self.nodes.template push_back<ndebug>(::pltxt2htm::RenderedCodeNode{
            position, position, 0, ::pltxt2htm::RenderedCodeNodeKind::style_end});
    }

    constexpr void append_text(this RenderedCodeAst& self,
                               ::pltxt2htm::container::U8StringView const text) noexcept {
        self.append(text, ::pltxt2htm::RenderedCodeNodeKind::text);
    }

    constexpr void append_text(this RenderedCodeAst& self, ::pltxt2htm::container::U8String& text) noexcept {
        self.append_text(::pltxt2htm::container::U8StringView{text});
        text.clear();
    }

    constexpr void append_entity_reference(this RenderedCodeAst& self,
                                           ::pltxt2htm::container::U8StringView const entity) noexcept {
        self.append(entity, ::pltxt2htm::RenderedCodeNodeKind::entity_reference);
    }

    constexpr void append_entity_reference(this RenderedCodeAst& self,
                                           ::pltxt2htm::container::U8String& entity) noexcept {
        self.append_entity_reference(::pltxt2htm::container::U8StringView{entity});
        entity.clear();
    }

    [[nodiscard]]
    constexpr auto get_nodes(this RenderedCodeAst const& self) noexcept
        -> ::pltxt2htm::container::Vector<::pltxt2htm::RenderedCodeNode> const& {
        return self.nodes;
    }

    [[nodiscard]]
    constexpr auto get_text(this RenderedCodeAst const& self, ::pltxt2htm::RenderedCodeNode const& node) noexcept
        -> ::pltxt2htm::container::U8StringView {
        pltxt2htm_assert(node.begin <= node.end && node.end <= self.source.size(),
                         u8"rendered code node source range out of bounds");
        return ::pltxt2htm::container::U8StringView{self.source.data() + node.begin, node.end - node.begin};
    }

    [[nodiscard]]
    constexpr auto get_style(this RenderedCodeAst const& self, ::pltxt2htm::RenderedCodeNode const& node) noexcept
        -> ::pltxt2htm::RenderedCodeStyle<ndebug> const& {
        pltxt2htm_assert(node.metadata < self.styles.size(), u8"rendered style index out of bounds");
        return self.styles.template index<ndebug>(node.metadata);
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
