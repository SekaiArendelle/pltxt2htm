/**
 * @file frame_concext.hh
 * @brief HTML-only parser frame contexts.
 * @details Simplified context storage for the HTML-only parser.
 *          Only supports contexts needed for HTML tag parsing.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details::html_parser {

class ParserFrameContextWithPltextInfo {
public:
    ::fast_io::u8string_view pltext;
};

class ParserFrameContextWithHtmlSpanInfo {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string color;
    ::exception::optional<::std::size_t> font_size;
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithHtmlATagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url<ndebug> url;
    bool internal;
};

class ParserFrameContextWithMdCellInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::MdTableAlign align;
};

template<::pltxt2htm::Contracts ndebug>
class FrontendContextVariant {
public:
    union {
        ::pltxt2htm::details::html_parser::ParserFrameContextWithPltextInfo pltext;
        ::pltxt2htm::details::html_parser::ParserFrameContextWithHtmlSpanInfo html_span_info;
        ::pltxt2htm::details::html_parser::ParserFrameContextWithHtmlATagInfo<ndebug> html_a_tag_info;
        ::pltxt2htm::details::html_parser::ParserFrameContextWithMdCellInfo md_cell;
    };

    ::pltxt2htm::NodeKind kind;

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::html_parser::ParserFrameContextWithPltextInfo&& pltext_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : pltext{::std::move(pltext_context)},
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::html_parser::ParserFrameContextWithHtmlSpanInfo&& html_span_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_span_info{::std::move(html_span_context)},
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::html_parser::ParserFrameContextWithHtmlATagInfo<ndebug>&& html_a_tag_context) noexcept
        : html_a_tag_info{::std::move(html_a_tag_context)},
          kind{::pltxt2htm::NodeKind::html_a} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::html_parser::ParserFrameContextWithMdCellInfo&& md_cell_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_cell{::std::move(md_cell_context)},
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug> const&) noexcept =
        delete;

    constexpr FrontendContextVariant(::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) {
        case ::pltxt2htm::NodeKind::html_a: {
            ::std::construct_at(::std::addressof(this->html_a_tag_info), ::std::move(other.html_a_tag_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            ::std::construct_at(::std::addressof(this->html_span_info), ::std::move(other.html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            ::std::construct_at(::std::addressof(this->md_cell), ::std::move(other.md_cell));
            return;
        }
        default: {
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        }
    }

    constexpr auto operator=(::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug> const&) noexcept
        -> ::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>&&) noexcept
        -> ::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>& = delete;

    constexpr ~FrontendContextVariant() noexcept {
        switch (this->kind) {
        case ::pltxt2htm::NodeKind::html_a: {
            ::std::destroy_at(::std::addressof(this->html_a_tag_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            ::std::destroy_at(::std::addressof(this->html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            ::std::destroy_at(::std::addressof(this->md_cell));
            return;
        }
        default: {
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        }
    }
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContext {
    ::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug> context_data;

public:
    ::std::size_t current_index{};
    ::pltxt2htm::Ast<ndebug> subast;

    constexpr explicit ParserFrameContext(
        ::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>&& ctx,
        ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
        : context_data(::std::move(ctx)),
          subast(::std::move(subast_)) {
    }

    constexpr ParserFrameContext(::pltxt2htm::details::html_parser::ParserFrameContext<ndebug> const&) noexcept = delete;

    constexpr ParserFrameContext(::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>&& other) noexcept
        : context_data{::std::move(other.context_data)},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
    }

    constexpr auto operator=(::pltxt2htm::details::html_parser::ParserFrameContext<ndebug> const&) noexcept
        -> ::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>& = delete;

    constexpr ~ParserFrameContext() noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this auto&& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.context_data.kind;
    }

    [[nodiscard]]
    constexpr auto get_pltext(this auto&& self) noexcept -> ::fast_io::u8string_view {
        auto const& context_data_ref = self.context_data;
        switch (context_data_ref.kind) {
        case ::pltxt2htm::NodeKind::text:
        case ::pltxt2htm::NodeKind::html_p:
        case ::pltxt2htm::NodeKind::html_h1:
        case ::pltxt2htm::NodeKind::html_h2:
        case ::pltxt2htm::NodeKind::html_h3:
        case ::pltxt2htm::NodeKind::html_h4:
        case ::pltxt2htm::NodeKind::html_h5:
        case ::pltxt2htm::NodeKind::html_h6:
        case ::pltxt2htm::NodeKind::html_del:
        case ::pltxt2htm::NodeKind::html_em:
        case ::pltxt2htm::NodeKind::html_strong:
        case ::pltxt2htm::NodeKind::html_ul:
        case ::pltxt2htm::NodeKind::html_ol:
        case ::pltxt2htm::NodeKind::html_li:
        case ::pltxt2htm::NodeKind::html_code:
        case ::pltxt2htm::NodeKind::html_pre:
        case ::pltxt2htm::NodeKind::html_blockquote:
        case ::pltxt2htm::NodeKind::html_table:
        case ::pltxt2htm::NodeKind::html_tr:
        case ::pltxt2htm::NodeKind::html_th:
        case ::pltxt2htm::NodeKind::html_thead:
        case ::pltxt2htm::NodeKind::html_tbody:
        case ::pltxt2htm::NodeKind::html_tfoot:
        case ::pltxt2htm::NodeKind::html_caption:
        case ::pltxt2htm::NodeKind::html_colgroup: {
            return context_data_ref.pltext.pltext;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            return context_data_ref.html_a_tag_info.pltext;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            return context_data_ref.html_span_info.pltext;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            return context_data_ref.md_cell.pltext;
        }
        default:
            pltxt2htm_unreachable(u8"Unexpected node kind in context-kind switch");
        }
    }

    [[nodiscard]]
    constexpr auto get_html_span_color(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_html_span_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_span};
        pltxt2htm_assert(is_html_span_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.html_span_info.color);
    }

    [[nodiscard]]
    constexpr auto get_html_span_font_size(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_html_span_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_span};
        pltxt2htm_assert(is_html_span_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.html_span_info.font_size);
    }

    [[nodiscard]]
    constexpr auto get_html_a_url(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_html_a_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_a};
        pltxt2htm_assert(is_html_a_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.html_a_tag_info.url);
    }

    [[nodiscard]]
    constexpr auto get_html_a_internal(this auto&& self) noexcept -> bool {
        auto&& context_data_ref = self.context_data;
        bool const is_html_a_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_a};
        pltxt2htm_assert(is_html_a_type, u8"context kind mismatch");
        return context_data_ref.html_a_tag_info.internal;
    }

    [[nodiscard]]
    constexpr auto get_md_cell_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        auto&& context_data_ref = self.context_data;
        bool const is_html_td_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_td};
        pltxt2htm_assert(is_html_td_type, u8"context kind mismatch");
        return context_data_ref.md_cell.align;
    }
};

} // namespace pltxt2htm::details::html_parser

#include "../pop_macro.hh"
