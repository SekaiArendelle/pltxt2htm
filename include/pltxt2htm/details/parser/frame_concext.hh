/**
 * @file frame_concext.hh
 * @brief Parser frame contexts for parsing pl-text
 * @details Defines context storage used during parsing.
 * @note The filename "concext" is intentionally spelled this way (not "context")
 *       to avoid potential naming conflicts in the codebase.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "md_list.hh"
#include "md_table.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

class ParserFrameContextWithPltextInfo {
public:
    ::fast_io::u8string_view pltext;
};

class ParserFrameContextWithEqualSignTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string id;
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithExternalTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url<ndebug> url;
};

class ParserFrameContextWithPlSizeTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::std::size_t id;
};

class ParserFrameContextWithMdBlockQuotesInfo {
public:
    ::fast_io::u8string pltext;
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithMdLinkInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url<ndebug> link;
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithMdListInfo {
public:
    ::pltxt2htm::details::MdListAst<ndebug> md_list_ast;
    typename ::pltxt2htm::details::MdListAst<ndebug>::iterator iter;

    constexpr explicit ParserFrameContextWithMdListInfo(::pltxt2htm::details::MdListAst<ndebug>&& md_list_ast_) noexcept
        : md_list_ast(::std::move(md_list_ast_)),
          iter(md_list_ast.begin()) {
    }
};

/**
 * @brief Context for an individual table cell during parsing.
 *
 * Stores the cell text content and its alignment for md_th / md_td nodes.
 */
class ParserFrameContextWithMdCellInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::MdTableAlign align;
};

class ParserFrameContextWithMdLiCheckboxInfo {
public:
    ::fast_io::u8string_view pltext;
    bool checked;
};

enum class MdTableParsePhase : ::std::size_t {
    header = 0,
    body,
    finish,
};

/**
 * @brief Context for the top-level table parsing state machine.
 *
 * Holds the raw AST, parse phase (header / body / finish), and the
 * current row/cell index within the table.
 */
template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithMdTableInfo {
public:
    ::pltxt2htm::details::MdTableAstRaw<ndebug> raw_ast;
    MdTableParsePhase state{MdTableParsePhase::header};
    ::std::size_t row_index{};
    ::std::size_t cell_index{};

    constexpr explicit ParserFrameContextWithMdTableInfo(
        ::pltxt2htm::details::MdTableAstRaw<ndebug>&& raw_ast_) noexcept
        : raw_ast(::std::move(raw_ast_)) {
    }
};

template<::pltxt2htm::Contracts ndebug>
class ContextVariant {
public:
    union {
        ::pltxt2htm::details::ParserFrameContextWithPltextInfo pltext;
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo<ndebug> external_tag;
        ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo pl_size_tag;
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo md_block_quotes;
        ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo<ndebug> md_link;
        ::pltxt2htm::details::ParserFrameContextWithMdListInfo<ndebug> md_list;
        ::pltxt2htm::details::ParserFrameContextWithMdCellInfo md_cell;
        ::pltxt2htm::details::ParserFrameContextWithMdLiCheckboxInfo md_li_checkbox;
        ::pltxt2htm::details::ParserFrameContextWithMdTableInfo<ndebug> md_table;
    };

    ::pltxt2htm::NodeKind kind;

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPltextInfo&& pltext_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : pltext{::std::move(pltext_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithExternalTagInfo<ndebug>&& external_tag_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : external_tag{::std::move(external_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_block_quotes{::std::move(md_block_quotes_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdLinkInfo<ndebug>&& md_link_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_link{::std::move(md_link_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdListInfo<ndebug>&& md_list_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_list{::std::move(md_list_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdCellInfo&& md_cell_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_cell{::std::move(md_cell_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdLiCheckboxInfo&& md_li_checkbox_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_li_checkbox{::std::move(md_li_checkbox_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdTableInfo<ndebug>&& md_table_context,
                             ::pltxt2htm::NodeKind node_type) noexcept
        : md_table{::std::move(md_table_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ContextVariant<ndebug> const&) noexcept = delete;

    constexpr ContextVariant(::pltxt2htm::details::ContextVariant<ndebug>&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            ::std::construct_at(::std::addressof(this->external_tag), ::std::move(other.external_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            ::std::construct_at(::std::addressof(this->md_block_quotes), ::std::move(other.md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            ::std::construct_at(::std::addressof(this->md_link), ::std::move(other.md_link));
            return;
        }
        case ::pltxt2htm::NodeKind::md_ul:
        case ::pltxt2htm::NodeKind::md_ol: {
            ::std::construct_at(::std::addressof(this->md_list), ::std::move(other.md_list));
            return;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            ::std::construct_at(::std::addressof(this->md_li_checkbox), ::std::move(other.md_li_checkbox));
            return;
        }
        case ::pltxt2htm::NodeKind::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block: {
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            ::std::construct_at(::std::addressof(this->md_cell), ::std::move(other.md_cell));
            return;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            ::std::construct_at(::std::addressof(this->md_table), ::std::move(other.md_table));
            return;
        }
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    constexpr auto operator=(::pltxt2htm::details::ContextVariant<ndebug> const&) noexcept
        -> ::pltxt2htm::details::ContextVariant<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::ContextVariant<ndebug>&&) noexcept
        -> ::pltxt2htm::details::ContextVariant<ndebug>& = delete;

    constexpr ~ContextVariant() noexcept {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            ::std::destroy_at(::std::addressof(this->equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            ::std::destroy_at(::std::addressof(this->external_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            ::std::destroy_at(::std::addressof(this->pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            ::std::destroy_at(::std::addressof(this->md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            ::std::destroy_at(::std::addressof(this->md_link));
            return;
        }
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol: {
            ::std::destroy_at(::std::addressof(this->md_list));
            return;
        }
        case ::pltxt2htm::NodeKind::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block: {
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            ::std::destroy_at(::std::addressof(this->md_cell));
            return;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            ::std::destroy_at(::std::addressof(this->md_table));
            return;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            ::std::destroy_at(::std::addressof(this->md_li_checkbox));
            return;
        }
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
};

template<::pltxt2htm::Contracts ndebug>
class ParserFrameContext {
    ::pltxt2htm::details::ContextVariant<ndebug> context_data;

public:
    ::std::size_t current_index{};
    ::pltxt2htm::Ast<ndebug> subast{};

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_,
                                          ::pltxt2htm::NodeKind const nested_tag_type_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          subast{} {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_,
                                          ::pltxt2htm::NodeKind const nested_tag_type_,
                                          ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          subast(::std::move(subast_)) {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_,
                                          ::pltxt2htm::NodeKind const nested_tag_type_,
                                          ::fast_io::u8string&& id_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo{.pltext = pltext_, .id = ::std::move(id_)},
              nested_tag_type_}} {
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_,
                                          ::pltxt2htm::NodeKind const nested_tag_type_,
                                          ::pltxt2htm::Url<ndebug>&& url_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo{.pltext = pltext_, .url = ::std::move(url_)},
              nested_tag_type_}} {
        bool const is_external_tag_type{nested_tag_type_ == ::pltxt2htm::NodeKind::pl_external};
        pltxt2htm_assert(is_external_tag_type, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_,
                                          ::pltxt2htm::NodeKind const nested_tag_type_, ::std::size_t id_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo{.pltext = pltext_, .id = id_},
              nested_tag_type_}} {
        bool const is_pl_size_tag_type{nested_tag_type_ == ::pltxt2htm::NodeKind::pl_size};
        pltxt2htm_assert(is_pl_size_tag_type, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string&& pltext_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo{::std::move(pltext_)},
              ::pltxt2htm::NodeKind::md_block_quotes}} {
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::Url<ndebug>&& link_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo{.pltext = pltext_, .link = ::std::move(link_)},
              ::pltxt2htm::NodeKind::md_link}} {
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_, bool checked_,
                                          ::pltxt2htm::NodeKind node_type) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdLiCheckboxInfo{pltext_, checked_}, node_type}} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeKind::md_li_checkbox, u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::pltxt2htm::NodeKind node_type,
                                          ::pltxt2htm::details::MdListAst<ndebug>&& md_list_ast_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdListInfo<ndebug>{::std::move(md_list_ast_)}, node_type}} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeKind::md_ul || node_type == ::pltxt2htm::NodeKind::md_ol,
                         u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::MdTableAlign align_,
                                          ::pltxt2htm::NodeKind node_type) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdCellInfo{.pltext = pltext_, .align = align_}, node_type}} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeKind::md_th || node_type == ::pltxt2htm::NodeKind::md_td,
                         u8"mismatch node type");
    }

    constexpr explicit ParserFrameContext(::pltxt2htm::NodeKind node_type,
                                          ::pltxt2htm::details::MdTableAstRaw<ndebug>&& raw_ast_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdTableInfo<ndebug>{::std::move(raw_ast_)}, node_type}} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeKind::md_table, u8"mismatch node type");
    }

    constexpr ParserFrameContext(::pltxt2htm::details::ParserFrameContext<ndebug> const&) noexcept = delete;

    constexpr ParserFrameContext(::pltxt2htm::details::ParserFrameContext<ndebug>&& other) noexcept
        : context_data{::std::move(other.context_data)},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
    }

    constexpr auto operator=(::pltxt2htm::details::ParserFrameContext<ndebug> const&) noexcept
        -> ::pltxt2htm::details::ParserFrameContext<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::ParserFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::ParserFrameContext<ndebug>& = delete;

    constexpr ~ParserFrameContext() noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this auto&& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.context_data.kind;
    }

    [[nodiscard]]
    constexpr auto get_pltext(this auto&& self) noexcept -> ::fast_io::u8string_view {
        auto const& context_data_ref = self.context_data;
        switch (context_data_ref.kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block: {
            return context_data_ref.pltext.pltext;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            return context_data_ref.md_li_checkbox.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            return context_data_ref.equal_sign_tag.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            return context_data_ref.external_tag.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            return context_data_ref.pl_size_tag.pltext;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            auto const& pltext = context_data_ref.md_block_quotes.pltext;
            return ::fast_io::u8string_view{pltext.data(), pltext.size()};
        }
        case ::pltxt2htm::NodeKind::md_link: {
            return context_data_ref.md_link.pltext;
        }
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            return context_data_ref.md_cell.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        // suppress GCC -Wreturn-type warning
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    [[nodiscard]]
    constexpr auto get_equal_sign_tag_id(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id);
    }

    [[nodiscard]]
    constexpr auto get_external_tag_url(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_external_tag_type{context_data_ref.kind == ::pltxt2htm::NodeKind::pl_external};
        pltxt2htm_assert(is_external_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.external_tag.url);
    }

    [[nodiscard]]
    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        bool const is_pl_size_tag_type{context_data_ref.kind == ::pltxt2htm::NodeKind::pl_size};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return context_data_ref.pl_size_tag.id;
    }

    [[nodiscard]]
    constexpr auto get_md_link_url(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_md_link_type{context_data_ref.kind == ::pltxt2htm::NodeKind::md_link};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_link.link);
    }

    [[nodiscard]]
    constexpr auto get_md_list_ast(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_md_ul_or_ol_type{::pltxt2htm::details::is_md_list_ul_or_ol_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_ul_or_ol_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.md_list_ast);
    }

    [[nodiscard]]
    constexpr auto get_md_list_iter(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_md_ul_or_ol_type{::pltxt2htm::details::is_md_list_ul_or_ol_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_ul_or_ol_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.iter);
    }

    [[nodiscard]]
    constexpr auto get_md_table_raw_ast(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_table.raw_ast);
    }

    [[nodiscard]]
    constexpr auto get_md_table_state(this auto&& self) noexcept -> MdTableParsePhase {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        return context_data_ref.md_table.state;
    }

    [[nodiscard]]
    constexpr auto get_md_table_row_index(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        return context_data_ref.md_table.row_index;
    }

    [[nodiscard]]
    constexpr auto get_md_table_cell_index(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        return context_data_ref.md_table.cell_index;
    }

    constexpr auto set_md_table_state(this auto&& self, MdTableParsePhase s) noexcept -> void {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        context_data_ref.md_table.state = s;
    }

    constexpr auto set_md_table_row_index(this auto&& self, ::std::size_t r) noexcept -> void {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        context_data_ref.md_table.row_index = r;
    }

    constexpr auto set_md_table_cell_index(this auto&& self, ::std::size_t c) noexcept -> void {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_table, u8"context kind mismatch");
        context_data_ref.md_table.cell_index = c;
    }

    [[nodiscard]]
    constexpr auto get_md_cell_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::md_th ||
                             context_data_ref.kind == ::pltxt2htm::NodeKind::md_td,
                         u8"context kind mismatch");
        return context_data_ref.md_cell.align;
    }

    [[nodiscard]]
    constexpr auto get_checked(this auto&& self) noexcept -> bool {
        pltxt2htm_assert(self.context_data.kind == ::pltxt2htm::NodeKind::md_li_checkbox, u8"context kind mismatch");
        return self.context_data.md_li_checkbox.checked;
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
