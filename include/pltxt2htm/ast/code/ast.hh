/**
 * @file ast.hh
 * @brief Owned fenced-code AST.
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
class CodeRenderedStyle {
    ::pltxt2htm::container::U8String color;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};

public:
    constexpr CodeRenderedStyle() noexcept = default;

    constexpr CodeRenderedStyle(
        ::pltxt2htm::container::U8String&& color_value,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_value,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>&& vertical_align_value) noexcept
        : color(::std::move(color_value)),
          font_size(font_size_value),
          vertical_align(::std::move(vertical_align_value)) {
    }

    constexpr CodeRenderedStyle(CodeRenderedStyle const&) noexcept = default;
    constexpr CodeRenderedStyle(CodeRenderedStyle&&) noexcept = default;
    constexpr ~CodeRenderedStyle() noexcept = default;
    constexpr auto operator=(CodeRenderedStyle const&) noexcept -> CodeRenderedStyle& = delete;
    constexpr auto operator=(this CodeRenderedStyle& self, CodeRenderedStyle&& other) noexcept
        -> CodeRenderedStyle& = default;

    [[nodiscard]]
    constexpr auto operator==(this CodeRenderedStyle const&, CodeRenderedStyle const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto get_color(this CodeRenderedStyle const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        return ::pltxt2htm::container::U8StringView{self.color.data(), self.color.size()};
    }

    [[nodiscard]]
    constexpr auto get_font_size(this CodeRenderedStyle const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const& {
        return self.font_size;
    }

    [[nodiscard]]
    constexpr auto get_vertical_align(this CodeRenderedStyle const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> const& {
        return self.vertical_align;
    }
};

template<::pltxt2htm::Contracts ndebug>
class CodeAst {
    ::pltxt2htm::container::U8String source;
    ::pltxt2htm::container::Vector<::pltxt2htm::CodeNode> nodes;
    ::pltxt2htm::container::Vector<::pltxt2htm::CodeRenderedStyle<ndebug>> rendered_styles;
    ::pltxt2htm::CodeLanguage language{::pltxt2htm::CodeLanguage::plain};

public:
    constexpr CodeAst() noexcept = default;

    constexpr explicit CodeAst(::pltxt2htm::CodeLanguage const language_value) noexcept
        : language(language_value) {
    }

    constexpr CodeAst(CodeAst const&) noexcept = default;
    constexpr CodeAst(CodeAst&&) noexcept = default;
    constexpr ~CodeAst() noexcept = default;

    constexpr auto operator=(this CodeAst& self, CodeAst const& other) noexcept -> CodeAst& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self = CodeAst{other};
        return self;
    }

    constexpr auto operator=(this CodeAst& self, CodeAst&& other) noexcept -> CodeAst& = default;

    [[nodiscard]]
    constexpr auto operator==(this CodeAst const&, CodeAst const&) noexcept -> bool = default;

    constexpr void reserve(this CodeAst& self, ::std::size_t const source_size) noexcept {
        self.source.template reserve<ndebug>(source_size);
    }

    template<::pltxt2htm::CodeLanguage node_language>
    constexpr void append(this CodeAst& self, ::pltxt2htm::container::U8StringView const text,
                          ::pltxt2htm::CodeNodeKind<node_language> const node_kind) noexcept {
        pltxt2htm_assert(self.language == node_language, u8"code AST language mismatch");
        if (text.size() == 0) {
            return;
        }
        ::std::size_t const begin{self.source.size()};
        self.source.template append<ndebug>(text.data(), text.size());
        ::std::size_t const end{self.source.size()};
        unsigned const kind{static_cast<unsigned>(node_kind)};
        if (kind == 0 && self.nodes.is_empty() == false) {
            auto& last = self.nodes.template index<ndebug>(self.nodes.size() - 1);
            if (last.kind == kind && last.metadata == 0 && last.end == begin) {
                last.end = end;
                return;
            }
        }
        self.nodes.template push_back<ndebug>(::pltxt2htm::CodeNode{begin, end, 0, kind});
    }

    template<::pltxt2htm::CodeLanguage node_language>
    constexpr void append(this CodeAst& self, ::pltxt2htm::container::U8String& text,
                          ::pltxt2htm::CodeNodeKind<node_language> const node_kind) noexcept {
        self.template append<node_language>(::pltxt2htm::container::U8StringView{text}, node_kind);
        text.clear();
    }

    constexpr auto add_rendered_style(
        this CodeAst& self, ::pltxt2htm::container::U8String&& color,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const font_size,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>&& vertical_align) noexcept
        -> ::std::size_t {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered style added to language code AST");
        ::std::size_t const index{self.rendered_styles.size()};
        self.rendered_styles.template push_back<ndebug>(
            ::pltxt2htm::CodeRenderedStyle<ndebug>{::std::move(color), font_size, ::std::move(vertical_align)});
        return index;
    }

    constexpr void append_rendered_style_begin(this CodeAst& self, ::std::size_t const style_index) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        pltxt2htm_assert(style_index < self.rendered_styles.size(), u8"rendered style index out of bounds");
        ::std::size_t const position{self.source.size()};
        self.nodes.template push_back<ndebug>(::pltxt2htm::CodeNode{
            position, position, style_index, static_cast<unsigned>(::pltxt2htm::CodeRenderedNodeKind::style_begin)});
    }

    constexpr void append_rendered_style_end(this CodeAst& self) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        ::std::size_t const position{self.source.size()};
        self.nodes.template push_back<ndebug>(::pltxt2htm::CodeNode{
            position, position, 0, static_cast<unsigned>(::pltxt2htm::CodeRenderedNodeKind::style_end)});
    }

    constexpr void append_rendered_text(this CodeAst& self,
                                        ::pltxt2htm::container::U8StringView const text) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        self.template append<::pltxt2htm::CodeLanguage::rendered>(text, ::pltxt2htm::CodeRenderedNodeKind::text);
    }

    constexpr void append_rendered_text(this CodeAst& self, ::pltxt2htm::container::U8String& text) noexcept {
        self.append_rendered_text(::pltxt2htm::container::U8StringView{text});
        text.clear();
    }

    constexpr void append_rendered_entity_reference(
        this CodeAst& self, ::pltxt2htm::container::U8StringView const entity) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        self.template append<::pltxt2htm::CodeLanguage::rendered>(entity,
                                                                  ::pltxt2htm::CodeRenderedNodeKind::entity_reference);
    }

    constexpr void append_rendered_entity_reference(this CodeAst& self,
                                                     ::pltxt2htm::container::U8String& entity) noexcept {
        self.append_rendered_entity_reference(::pltxt2htm::container::U8StringView{entity});
        entity.clear();
    }

    [[nodiscard]]
    constexpr auto get_language(this CodeAst const& self) noexcept -> ::pltxt2htm::CodeLanguage {
        return self.language;
    }

    [[nodiscard]]
    constexpr auto get_nodes(this CodeAst const& self) noexcept
        -> ::pltxt2htm::container::Vector<::pltxt2htm::CodeNode> const& {
        return self.nodes;
    }

    [[nodiscard]]
    constexpr auto get_text(this CodeAst const& self, ::pltxt2htm::CodeNode const& node) noexcept
        -> ::pltxt2htm::container::U8StringView {
        pltxt2htm_assert(node.begin <= node.end && node.end <= self.source.size(),
                         u8"code node source range out of bounds");
        return ::pltxt2htm::container::U8StringView{self.source.data() + node.begin, node.end - node.begin};
    }

    template<::pltxt2htm::CodeLanguage node_language>
    [[nodiscard]]
    constexpr auto get_node_kind(this CodeAst const& self, ::pltxt2htm::CodeNode const& node) noexcept
        -> ::pltxt2htm::CodeNodeKind<node_language> {
        pltxt2htm_assert(self.language == node_language, u8"code AST language mismatch");
        return static_cast<::pltxt2htm::CodeNodeKind<node_language>>(node.kind);
    }

    [[nodiscard]]
    constexpr auto get_rendered_style(this CodeAst const& self, ::pltxt2htm::CodeNode const& node) noexcept
        -> ::pltxt2htm::CodeRenderedStyle<ndebug> const& {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered style requested from language code AST");
        pltxt2htm_assert(node.metadata < self.rendered_styles.size(), u8"rendered style index out of bounds");
        return self.rendered_styles.template index<ndebug>(node.metadata);
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
