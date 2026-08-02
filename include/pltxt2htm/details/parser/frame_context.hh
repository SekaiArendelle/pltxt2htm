/**
 * @file frame_context.hh
 * @brief Parser frame contexts for parsing pl-text
 * @details Defines context storage used during parsing.
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
#include "../../ast/font_size_value.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

#pragma push_macro("pltxt2htm_assert_context_branch")
#undef pltxt2htm_assert_context_branch
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
    #define pltxt2htm_assert_context_branch(self, expected) \
        do { \
            if ((self).context_branch != (expected)) [[unlikely]] { \
                constexpr auto source_location = ::std::source_location::current(); \
                ::pltxt2htm::details::panic<::pltxt2htm::details::U8LiteralString{u8"context branch mismatch"}, \
                                            ::pltxt2htm::details::U8LiteralString{u8"" __FILE__}, \
                                            source_location.line(), source_location.column(), \
                                            ::pltxt2htm::details::U8LiteralString{u8"context branch mismatch"}>(); \
            } \
        } while (0)
#else
    #define pltxt2htm_assert_context_branch(self, expected)
#endif

/**
 * @brief Context for frames that only store raw pl-text (e.g., emphasis, headings).
 */
class ParserFrameContextWithPltextInfo {
public:
    ::fast_io::u8string_view pltext;
};

/**
 * @brief Context for frames with an equals-sign attribute (color=, experiment=, etc.).
 */
class ParserFrameContextWithEqualSignTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string id;
};

/**
 * @brief Context for html_span frames during parsing.
 */
class ParserFrameContextWithHtmlSpanInfo {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string color;
    ::exception::optional<::pltxt2htm::FontSizeValue> font_size;
};

/**
 * @brief Context for HTML <code> tag frames during parsing; stores class attribute.
 */
class ParserFrameContextWithHtmlCodeInfo {
public:
    ::fast_io::u8string_view pltext;
    ::exception::optional<::fast_io::u8string> language;
};

/**
 * @brief Context for URL frames during parsing; stores the raw text and parsed URL.
 */
class ParserFrameContextWithUrlInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url url;
};

/**
 * @brief Context for HTML <a> tag frames during parsing.
 */
class ParserFrameContextWithHtmlATagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url url;
    bool internal;
};

/**
 * @brief Context for <size=N> frames during parsing.
 */
class ParserFrameContextWithPlSizeTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::FontSizeValue value;
};

/**
 * @brief Context for block-quote frames; stores the owned sub-text.
 */
class ParserFrameContextWithMdBlockQuotesInfo {
public:
    ::fast_io::u8string pltext;
};

/**
 * @brief Context for ordered/unordered list frames; owns the intermediate list AST + iterator.
 */
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
 * Stores the cell text content and its alignment for md_th / md_td / html_th / html_td nodes.
 */
class ParserFrameContextWithCellInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::TableAlign align;
};

/**
 * @brief Context for checkbox list-item frames during parsing.
 */
class ParserFrameContextWithMdLiCheckboxInfo {
public:
    ::fast_io::u8string_view pltext;
    bool checked;
};

/**
 * @brief State-machine phase for table-frame parsing.
 */
enum class MdTableParsePhase : unsigned {
    header = 0, ///< Currently parsing header cells.
    body, ///< Currently parsing body cells.
    finish, ///< All cells consumed; finalise the table AST.
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

/**
 * @brief Tagged-union variant of all parser frame context types.
 * @details Dispatched on `kind` (::pltxt2htm::NodeKind). Used inside
 *          ::pltxt2htm::details::ParserFrameContext.
 */
template<::pltxt2htm::Contracts ndebug>
class FrontendContextVariant {
public:
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
    enum class ContextBranch : unsigned {
        equal_sign_tag,
        html_a_tag,
        url_info,
        pl_size_tag,
        html_span_info,
        html_code_info,
        md_block_quotes,
        md_list,
        md_li_checkbox,
        cell,
        md_table,
        pltext,
    };
#endif

    union {
        ::pltxt2htm::details::ParserFrameContextWithPltextInfo pltext;
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::ParserFrameContextWithHtmlSpanInfo html_span_info;
        ::pltxt2htm::details::ParserFrameContextWithHtmlCodeInfo html_code_info;
        ::pltxt2htm::details::ParserFrameContextWithUrlInfo url_info;
        ::pltxt2htm::details::ParserFrameContextWithHtmlATagInfo html_a_tag_info;
        ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo pl_size_tag;
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo md_block_quotes;
        ::pltxt2htm::details::ParserFrameContextWithMdListInfo<ndebug> md_list;
        ::pltxt2htm::details::ParserFrameContextWithCellInfo cell;
        ::pltxt2htm::details::ParserFrameContextWithMdLiCheckboxInfo md_li_checkbox;
        ::pltxt2htm::details::ParserFrameContextWithMdTableInfo<ndebug> md_table;
    };

#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
    ContextBranch context_branch;
#endif
    ::pltxt2htm::NodeKind kind;

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithPltextInfo pltext_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pltext{pltext_context},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pltext},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::equal_sign_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithHtmlSpanInfo&& html_span_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_span_info{::std::move(html_span_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_span_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithHtmlCodeInfo&& html_code_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_code_info{::std::move(html_code_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_code_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithHtmlATagInfo&& html_a_tag_context) noexcept
        : html_a_tag_info{::std::move(html_a_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_a_tag},
#endif
          kind{::pltxt2htm::NodeKind::html_a} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithUrlInfo&& url_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : url_info{::std::move(url_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::url_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pl_size_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_block_quotes{::std::move(md_block_quotes_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::md_block_quotes},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithMdListInfo<ndebug>&& md_list_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_list{::std::move(md_list_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::md_list},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithCellInfo&& cell_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : cell{::std::move(cell_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::cell},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithMdLiCheckboxInfo&& md_li_checkbox_context,
        ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_li_checkbox{::std::move(md_li_checkbox_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::md_li_checkbox},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::ParserFrameContextWithMdTableInfo<ndebug>&& md_table_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_table{::std::move(md_table_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::md_table},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(::pltxt2htm::details::FrontendContextVariant<ndebug> const&) noexcept = delete;

    constexpr FrontendContextVariant(::pltxt2htm::details::FrontendContextVariant<ndebug>&& other) noexcept
        :
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{other.context_branch},
#endif
          kind{other.kind} {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::equal_sign_tag);
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_a_tag);
            ::std::construct_at(::std::addressof(this->html_a_tag_info), ::std::move(other.html_a_tag_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_external:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_link: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::url_info);
            ::std::construct_at(::std::addressof(this->url_info), ::std::move(other.url_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_size_tag);
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_span_info);
            ::std::construct_at(::std::addressof(this->html_span_info), ::std::move(other.html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_code_info);
            ::std::construct_at(::std::addressof(this->html_code_info), ::std::move(other.html_code_info));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_block_quotes);
            ::std::construct_at(::std::addressof(this->md_block_quotes), ::std::move(other.md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_list);
            ::std::construct_at(::std::addressof(this->md_list), ::std::move(other.md_list));
            return;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_li_checkbox);
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
        case ::pltxt2htm::NodeKind::pl_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
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
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
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
        case ::pltxt2htm::NodeKind::md_li: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pltext);
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::cell);
            ::std::construct_at(::std::addressof(this->cell), ::std::move(other.cell));
            return;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_table);
            ::std::construct_at(::std::addressof(this->md_table), ::std::move(other.md_table));
            return;
        }
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_input:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
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
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
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
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::entity_reference:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in FrontendContextVariant move");
            }
        }
    }

    constexpr auto operator=(::pltxt2htm::details::FrontendContextVariant<ndebug> const&) noexcept
        -> ::pltxt2htm::details::FrontendContextVariant<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::FrontendContextVariant<ndebug>&&) noexcept
        -> ::pltxt2htm::details::FrontendContextVariant<ndebug>& = delete;

    constexpr ~FrontendContextVariant() noexcept {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::equal_sign_tag);
            ::std::destroy_at(::std::addressof(this->equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_a_tag);
            ::std::destroy_at(::std::addressof(this->html_a_tag_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_external:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_link: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::url_info);
            ::std::destroy_at(::std::addressof(this->url_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_size_tag);
            ::std::destroy_at(::std::addressof(this->pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_span_info);
            ::std::destroy_at(::std::addressof(this->html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_code_info);
            ::std::destroy_at(::std::addressof(this->html_code_info));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_block_quotes);
            ::std::destroy_at(::std::addressof(this->md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_list);
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
        case ::pltxt2htm::NodeKind::pl_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
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
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
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
        case ::pltxt2htm::NodeKind::md_li: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pltext);
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::cell);
            ::std::destroy_at(::std::addressof(this->cell));
            return;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_table);
            ::std::destroy_at(::std::addressof(this->md_table));
            return;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_li_checkbox);
            ::std::destroy_at(::std::addressof(this->md_li_checkbox));
            return;
        }
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_input:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
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
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
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
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::entity_reference:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in FrontendContextVariant destructor");
            }
        }
    }
};

/**
 * @brief Main parser frame context – one frame per nesting level on the call stack.
 * @details Holds the context data (tag type + payload), the current parse index
 *          within the raw text, and the sub-AST being built for this frame.
 *          Frames are manually managed on a call-stack to avoid stack overflow.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
class ParserFrameContext {
    ::pltxt2htm::details::FrontendContextVariant<ndebug> context_data;

public:
    ::std::size_t current_index{}; ///< Current parse position in the raw text.
    ::pltxt2htm::Ast<ndebug> subast; ///< Sub-AST being built for this frame.

    constexpr explicit ParserFrameContext(::pltxt2htm::details::FrontendContextVariant<ndebug>&& ctx,
                                          ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
        : context_data(::std::move(ctx)),
          subast(::std::move(subast_)) {
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
        case ::pltxt2htm::NodeKind::pl_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
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
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
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
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_input:
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
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::pltext);
            return context_data_ref.pltext.pltext;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::md_li_checkbox);
            return context_data_ref.md_li_checkbox.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::equal_sign_tag);
            return context_data_ref.equal_sign_tag.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::url_info);
            return context_data_ref.url_info.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_link: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::url_info);
            return context_data_ref.url_info.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::pl_size_tag);
            return context_data_ref.pl_size_tag.pltext;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::html_span_info);
            return context_data_ref.html_span_info.pltext;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::html_code_info);
            return context_data_ref.html_code_info.pltext;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::html_a_tag);
            return context_data_ref.html_a_tag_info.pltext;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::md_block_quotes);
            auto const& pltext = context_data_ref.md_block_quotes.pltext;
            return ::fast_io::u8string_view{pltext.data(), pltext.size()};
        }
        case ::pltxt2htm::NodeKind::md_link: {
            pltxt2htm_assert_context_branch(
                context_data_ref, ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::url_info);
            return context_data_ref.url_info.pltext;
        }
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td: {
            pltxt2htm_assert_context_branch(context_data_ref,
                                            ::pltxt2htm::details::FrontendContextVariant<ndebug>::ContextBranch::cell);
            return context_data_ref.cell.pltext;
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
        case ::pltxt2htm::NodeKind::entity_reference:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in context-kind switch");
            }
        }
        // suppress GCC -Wreturn-type warning
        pltxt2htm_unreachable(u8"Unreachable after context-kind switch");
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
        return ::std::forward_like<decltype(self)>(context_data_ref.url_info.url);
    }

    [[nodiscard]]
    constexpr auto get_link_tag_url(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_link_tag_type{context_data_ref.kind == ::pltxt2htm::NodeKind::pl_link};
        pltxt2htm_assert(is_link_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.url_info.url);
    }

    [[nodiscard]]
    constexpr auto get_pl_size_tag_value(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_pl_size_tag_type{context_data_ref.kind == ::pltxt2htm::NodeKind::pl_size};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.pl_size_tag.value);
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
    constexpr auto get_html_code_language(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_html_code_type{context_data_ref.kind == ::pltxt2htm::NodeKind::html_code};
        pltxt2htm_assert(is_html_code_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.html_code_info.language);
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
    constexpr auto get_md_link_url(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_md_link_type{context_data_ref.kind == ::pltxt2htm::NodeKind::md_link};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.url_info.url);
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
    constexpr auto get_cell_align(this auto&& self) noexcept -> ::pltxt2htm::TableAlign {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::html_td ||
                             context_data_ref.kind == ::pltxt2htm::NodeKind::html_th ||
                             context_data_ref.kind == ::pltxt2htm::NodeKind::md_th ||
                             context_data_ref.kind == ::pltxt2htm::NodeKind::md_td,
                         u8"context kind mismatch");
        return context_data_ref.cell.align;
    }

    [[nodiscard]]
    constexpr auto get_checked(this auto&& self) noexcept -> bool {
        pltxt2htm_assert(self.context_data.kind == ::pltxt2htm::NodeKind::md_li_checkbox, u8"context kind mismatch");
        return self.context_data.md_li_checkbox.checked;
    }
};

} // namespace pltxt2htm::details

#pragma pop_macro("pltxt2htm_assert_context_branch")
#include "../pop_macro.hh"
