/**
 * @file ast.hh
 * @brief Owned fenced-code AST.
 */

#pragma once

#include <cstddef>
#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io_dsal/vector.h>
#include "../../container/expected.hh"
#include "../../contracts.hh"
#include "../value_unit.hh"
#include "../vertical_align_value.hh"
#include "node.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
class CodeRenderedStyle {
    ::fast_io::u8string color;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};

public:
    constexpr CodeRenderedStyle() noexcept = default;

    constexpr CodeRenderedStyle(
        ::fast_io::u8string&& color_value,
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
    constexpr auto get_color(this CodeRenderedStyle const& self) noexcept -> ::fast_io::u8string_view {
        return ::fast_io::u8string_view{self.color.data(), self.color.size()};
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
    ::fast_io::u8string source;
    ::fast_io::vector<::pltxt2htm::CodeNode> nodes;
    ::fast_io::vector<::pltxt2htm::CodeRenderedStyle<ndebug>> rendered_styles;
    ::pltxt2htm::CodeLanguage language{::pltxt2htm::CodeLanguage::plain};

public:
    constexpr CodeAst() noexcept = default;

    constexpr explicit CodeAst(::pltxt2htm::CodeLanguage const language_value) noexcept
        : language(language_value) {
    }

    constexpr CodeAst(CodeAst const&) noexcept = default;
    constexpr CodeAst(CodeAst&&) noexcept = default;
    constexpr ~CodeAst() noexcept = default;
    constexpr auto operator=(CodeAst const&) noexcept -> CodeAst& = delete;
    constexpr auto operator=(this CodeAst& self, CodeAst&& other) noexcept -> CodeAst& = default;

    [[nodiscard]]
    constexpr auto operator==(this CodeAst const&, CodeAst const&) noexcept -> bool = default;

    constexpr void reserve(this CodeAst& self, ::std::size_t const source_size) noexcept {
        self.source.reserve(source_size);
    }

    template<::pltxt2htm::CodeLanguage node_language>
    constexpr void append(this CodeAst& self, ::fast_io::u8string& text,
                          ::pltxt2htm::CodeNodeKind<node_language> const node_kind) noexcept {
        pltxt2htm_assert(self.language == node_language, u8"code AST language mismatch");
        if (text.empty()) {
            return;
        }
        ::std::size_t const begin{self.source.size()};
        self.source.append(text);
        text.clear();
        ::std::size_t const end{self.source.size()};
        unsigned const kind{static_cast<unsigned>(node_kind)};
        if (kind == 0 && self.nodes.empty() == false && self.nodes.back().kind == kind &&
            self.nodes.back().metadata == 0 && self.nodes.back().end == begin) {
            self.nodes.back().end = end;
            return;
        }
        self.nodes.push_back(::pltxt2htm::CodeNode{begin, end, 0, kind});
    }

    constexpr auto add_rendered_style(
        this CodeAst& self, ::fast_io::u8string&& color,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const font_size,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>&& vertical_align) noexcept
        -> ::std::size_t {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered style added to language code AST");
        ::std::size_t const index{self.rendered_styles.size()};
        self.rendered_styles.push_back(
            ::pltxt2htm::CodeRenderedStyle<ndebug>{::std::move(color), font_size, ::std::move(vertical_align)});
        return index;
    }

    constexpr void append_rendered_style_begin(this CodeAst& self, ::std::size_t const style_index) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        pltxt2htm_assert(style_index < self.rendered_styles.size(), u8"rendered style index out of bounds");
        ::std::size_t const position{self.source.size()};
        self.nodes.push_back(::pltxt2htm::CodeNode{
            position, position, style_index, static_cast<unsigned>(::pltxt2htm::CodeRenderedNodeKind::style_begin)});
    }

    constexpr void append_rendered_style_end(this CodeAst& self) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        ::std::size_t const position{self.source.size()};
        self.nodes.push_back(::pltxt2htm::CodeNode{
            position, position, 0, static_cast<unsigned>(::pltxt2htm::CodeRenderedNodeKind::style_end)});
    }

    constexpr void append_rendered_text(this CodeAst& self, ::fast_io::u8string& text) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        self.template append<::pltxt2htm::CodeLanguage::rendered>(text, ::pltxt2htm::CodeRenderedNodeKind::text);
    }

    constexpr void append_rendered_entity_reference(this CodeAst& self, ::fast_io::u8string& entity) noexcept {
        pltxt2htm_assert(self.language == ::pltxt2htm::CodeLanguage::rendered,
                         u8"rendered node added to language code AST");
        self.template append<::pltxt2htm::CodeLanguage::rendered>(entity,
                                                                  ::pltxt2htm::CodeRenderedNodeKind::entity_reference);
    }

    [[nodiscard]]
    constexpr auto get_language(this CodeAst const& self) noexcept -> ::pltxt2htm::CodeLanguage {
        return self.language;
    }

    [[nodiscard]]
    constexpr auto get_nodes(this CodeAst const& self) noexcept -> ::fast_io::vector<::pltxt2htm::CodeNode> const& {
        return self.nodes;
    }

    [[nodiscard]]
    constexpr auto get_text(this CodeAst const& self, ::pltxt2htm::CodeNode const& node) noexcept
        -> ::fast_io::u8string_view {
        pltxt2htm_assert(node.begin <= node.end && node.end <= self.source.size(),
                         u8"code node source range out of bounds");
        return ::fast_io::u8string_view{self.source.data() + node.begin, node.end - node.begin};
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
        return self.rendered_styles[node.metadata];
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
