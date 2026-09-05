/**
 * @file frame_context.hh
 * @brief Parser frame contexts for parsing pl-text
 * @details Defines context storage used during parsing.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include "../../container/optional.hh"
#include "../call_stack.hh"
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/string_view.hh"
#include "list_ast.hh"
#include "md_table.hh"
#include "html_table.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
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
    ::pltxt2htm::container::U8StringView pltext;
};

/**
 * @brief Context for frames with an equals-sign attribute (color=, experiment=, etc.).
 */
class ParserFrameContextWithEqualSignTagInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::fast_io::u8string id;
};

/**
 * @brief Context for html_span frames during parsing.
 */
template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithHtmlSpanInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::fast_io::u8string color;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size;
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align;
};

/**
 * @brief Context for html_mark frames during parsing; stores the optional background-color.
 */
class ParserFrameContextWithHtmlMarkInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::fast_io::u8string background_color;
};

/**
 * @brief Context for URL frames during parsing; stores the raw text and parsed URL.
 */
class ParserFrameContextWithUrlInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::Url url;
};

/**
 * @brief Context for HTML <a> tag frames during parsing.
 */
class ParserFrameContextWithHtmlATagInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::Url url;
    bool internal;
};

/**
 * @brief Context for <size=N> / <size=N%> / <size=Nem> frames during parsing.
 */
class ParserFrameContextWithPlSizeTagInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::ValueWithUnit<double> value;
};

/**
 * @brief Context for <voffset=N> frames during parsing.
 */
class ParserFrameContextWithPlVoffsetTagInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value;
};

/**
 * @brief Context for <margin-left=N> / <margin-right=N> / <margin=N> /
 *        <margin left=N right=M> frames during parsing.
 */
class ParserFrameContextWithPlMarginTagInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;
};

/**
 * @brief Context for &lt;div style="margin-left:...;margin-right:..."&gt; frames during parsing.
 */
class ParserFrameContextWithHtmlDivInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;
};

/**
 * @brief Context for pl_mark frames during parsing; stores the background color.
 */
class ParserFrameContextWithPlMarkInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::fast_io::u8string background_color;
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
class ParserFrameContextWithListInfo {
public:
    ListAst<ndebug> list_ast;
    typename ListAst<ndebug>::iterator iter;
    ::std::size_t list_start{1}; ///< `<ol start="N">` value (defaults to 1).

    constexpr ParserFrameContextWithListInfo(ListAst<ndebug>&& list_ast_, ::std::size_t list_start_ = 1) noexcept
        : list_ast(::std::move(list_ast_)),
          iter(list_ast.begin()),
          list_start(list_start_) {
    }
};

/**
 * @brief Context for an individual table cell during parsing.
 *
 * Stores the cell text content and its alignment for table_th / table_td nodes.
 */
class ParserFrameContextWithCellInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::TableAlign align;
};

/**
 * @brief Context for text-alignment frames during parsing.
 * @details Shared by the HTML &lt;p&gt; paragraph and the Unity TMP &lt;align=...&gt; block
 *          frame types. Stores the text content and its text alignment.
 */
class ParserFrameContextWithAlignInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    ::pltxt2htm::TextAlign align;
};

/**
 * @brief Context for checkbox list-item frames during parsing.
 */
class ParserFrameContextWithListLiCheckboxInfo {
public:
    ::pltxt2htm::container::U8StringView pltext;
    bool checked;
};

/**
 * @brief Context for the shared table parsing state machine.
 *
 * Holds the raw table AST, the parse phase (caption / body / finish), and the
 * current row/cell index within the table.  Used by both the Markdown
 * pipe-table and the HTML &lt;table&gt; frames.
 */
template<::pltxt2htm::Contracts ndebug>
class ParserFrameContextWithTableInfo {
public:
    TableAstRaw<ndebug> raw_ast;
    TableParsePhase state{TableParsePhase::caption};
    ::std::size_t row_index{};
    ::std::size_t cell_index{};

    constexpr explicit ParserFrameContextWithTableInfo(TableAstRaw<ndebug>&& raw_ast_) noexcept
        : raw_ast(::std::move(raw_ast_)) {
    }
};

/**
 * @brief Tagged-union variant of all parser frame context types.
 * @details Dispatched on `kind` (::pltxt2htm::NodeKind). Used inside
 *          ParserFrame.
 */
template<::pltxt2htm::Contracts ndebug>
class FrontendContextVariant {
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
    enum class ContextBranch : unsigned {
        equal_sign_tag,
        html_a_tag,
        url_info,
        pl_size_tag,
        pl_voffset_tag,
        pl_margin_tag,
        align_info,
        html_span_info,
        html_div_info,
        md_block_quotes,
        list_info,
        list_li_checkbox,
        cell,
        table,
        pltext,
        html_mark_info,
        pl_mark_info,
    };
#endif

    union {
        ParserFrameContextWithPltextInfo pltext;
        ParserFrameContextWithEqualSignTagInfo equal_sign_tag;
        ParserFrameContextWithHtmlSpanInfo<ndebug> html_span_info;
        ParserFrameContextWithHtmlDivInfo html_div_info;
        ParserFrameContextWithHtmlMarkInfo html_mark_info;
        ParserFrameContextWithPlMarkInfo pl_mark_info;
        ParserFrameContextWithUrlInfo url_info;
        ParserFrameContextWithHtmlATagInfo html_a_tag_info;
        ParserFrameContextWithPlSizeTagInfo pl_size_tag;
        ParserFrameContextWithPlVoffsetTagInfo pl_voffset_tag;
        ParserFrameContextWithPlMarginTagInfo pl_margin_tag;
        ParserFrameContextWithMdBlockQuotesInfo md_block_quotes;
        ParserFrameContextWithListInfo<ndebug> list_info;
        ParserFrameContextWithCellInfo cell;
        ParserFrameContextWithAlignInfo align_info;
        ParserFrameContextWithListLiCheckboxInfo list_li_checkbox;
        ParserFrameContextWithTableInfo<ndebug> table;
    };

#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
    ContextBranch context_branch;
#endif
    ::pltxt2htm::NodeKind kind;

public:
    constexpr FrontendContextVariant(ParserFrameContextWithPltextInfo pltext_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pltext{pltext_context},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pltext},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::equal_sign_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithHtmlSpanInfo<ndebug>&& html_span_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_span_info{::std::move(html_span_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_span_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithHtmlDivInfo&& html_div_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_div_info{::std::move(html_div_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_div_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithHtmlMarkInfo&& html_mark_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : html_mark_info{::std::move(html_mark_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_mark_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithPlMarkInfo&& pl_mark_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pl_mark_info{::std::move(pl_mark_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pl_mark_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithHtmlATagInfo&& html_a_tag_context) noexcept
        : html_a_tag_info{::std::move(html_a_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::html_a_tag},
#endif
          kind{::pltxt2htm::NodeKind::html_a} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithUrlInfo&& url_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : url_info{::std::move(url_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::url_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pl_size_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithPlVoffsetTagInfo&& pl_voffset_tag_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pl_voffset_tag{::std::move(pl_voffset_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pl_voffset_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithPlMarginTagInfo&& pl_margin_tag_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : pl_margin_tag{::std::move(pl_margin_tag_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::pl_margin_tag},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : md_block_quotes{::std::move(md_block_quotes_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::md_block_quotes},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithListInfo<ndebug>&& list_info_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : list_info{::std::move(list_info_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::list_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithCellInfo&& cell_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : cell{::std::move(cell_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::cell},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithAlignInfo&& align_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : align_info{::std::move(align_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::align_info},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithListLiCheckboxInfo&& list_li_checkbox_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : list_li_checkbox{::std::move(list_li_checkbox_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::list_li_checkbox},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(ParserFrameContextWithTableInfo<ndebug>&& table_context,
                                     ::pltxt2htm::NodeKind node_kind_) noexcept
        : table{::std::move(table_context)},
#ifdef PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT
          context_branch{ContextBranch::table},
#endif
          kind{node_kind_} {
    }

    constexpr FrontendContextVariant(FrontendContextVariant<ndebug> const&) noexcept = delete;

    constexpr FrontendContextVariant(FrontendContextVariant<ndebug>&& other) noexcept
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
        case ::pltxt2htm::NodeKind::pl_experiments:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussions:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_trigger:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_internal:
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
        case ::pltxt2htm::NodeKind::pl_voffset: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_voffset_tag);
            ::std::construct_at(::std::addressof(this->pl_voffset_tag), ::std::move(other.pl_voffset_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_margin: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_margin_tag);
            ::std::construct_at(::std::addressof(this->pl_margin_tag), ::std::move(other.pl_margin_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_align: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::align_info);
            ::std::construct_at(::std::addressof(this->align_info), ::std::move(other.align_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_span_info);
            ::std::construct_at(::std::addressof(this->html_span_info), ::std::move(other.html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_div_info);
            ::std::construct_at(::std::addressof(this->html_div_info), ::std::move(other.html_div_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_mark_info);
            ::std::construct_at(::std::addressof(this->html_mark_info), ::std::move(other.html_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_mark: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_mark_info);
            ::std::construct_at(::std::addressof(this->pl_mark_info), ::std::move(other.pl_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_block_quotes);
            ::std::construct_at(::std::addressof(this->md_block_quotes), ::std::move(other.md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ol: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::list_info);
            ::std::construct_at(::std::addressof(this->list_info), ::std::move(other.list_info));
            return;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::list_li_checkbox);
            ::std::construct_at(::std::addressof(this->list_li_checkbox), ::std::move(other.list_li_checkbox));
            return;
        }
        case ::pltxt2htm::NodeKind::group:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
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
        case ::pltxt2htm::NodeKind::html_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_colgroup:
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
        case ::pltxt2htm::NodeKind::list_li: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pltext);
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::align_info);
            ::std::construct_at(::std::addressof(this->align_info), ::std::move(other.align_info));
            return;
        }
        case ::pltxt2htm::NodeKind::table_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_td: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::cell);
            ::std::construct_at(::std::addressof(this->cell), ::std::move(other.cell));
            return;
        }
        case ::pltxt2htm::NodeKind::table: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::table);
            ::std::construct_at(::std::addressof(this->table), ::std::move(other.table));
            return;
        }
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::code_fence:
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
        case ::pltxt2htm::NodeKind::invalid_utf8:
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
        case ::pltxt2htm::NodeKind::md_escape:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in FrontendContextVariant move");
            }
        }
    }

    constexpr auto operator=(FrontendContextVariant<ndebug> const&) noexcept
        -> FrontendContextVariant<ndebug>& = delete;
    constexpr auto operator=(FrontendContextVariant<ndebug>&&) noexcept -> FrontendContextVariant<ndebug>& = delete;

    [[nodiscard]]
    constexpr auto get_kind(this FrontendContextVariant<ndebug> const& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto as_pltext(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::pltext);
        return ::std::forward_like<decltype(self)>(self.pltext);
    }

    [[nodiscard]]
    constexpr auto as_equal_sign_tag(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(::pltxt2htm::details::is_equal_sign_tag_type(self.kind), u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::equal_sign_tag);
        return ::std::forward_like<decltype(self)>(self.equal_sign_tag);
    }

    [[nodiscard]]
    constexpr auto as_html_span_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_span, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::html_span_info);
        return ::std::forward_like<decltype(self)>(self.html_span_info);
    }

    [[nodiscard]]
    constexpr auto as_html_div_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_div, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::html_div_info);
        return ::std::forward_like<decltype(self)>(self.html_div_info);
    }

    [[nodiscard]]
    constexpr auto as_html_mark_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_mark, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::html_mark_info);
        return ::std::forward_like<decltype(self)>(self.html_mark_info);
    }

    [[nodiscard]]
    constexpr auto as_pl_mark_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::pl_mark, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::pl_mark_info);
        return ::std::forward_like<decltype(self)>(self.pl_mark_info);
    }

    [[nodiscard]]
    constexpr auto as_url_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::pl_external ||
                             self.kind == ::pltxt2htm::NodeKind::pl_link || self.kind == ::pltxt2htm::NodeKind::md_link,
                         u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::url_info);
        return ::std::forward_like<decltype(self)>(self.url_info);
    }

    [[nodiscard]]
    constexpr auto as_html_a_tag_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_a, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::html_a_tag);
        return ::std::forward_like<decltype(self)>(self.html_a_tag_info);
    }

    [[nodiscard]]
    constexpr auto as_pl_size_tag(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::pl_size, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::pl_size_tag);
        return ::std::forward_like<decltype(self)>(self.pl_size_tag);
    }

    [[nodiscard]]
    constexpr auto as_pl_voffset_tag(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::pl_voffset, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::pl_voffset_tag);
        return ::std::forward_like<decltype(self)>(self.pl_voffset_tag);
    }

    [[nodiscard]]
    constexpr auto as_pl_margin_tag(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::pl_margin, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::pl_margin_tag);
        return ::std::forward_like<decltype(self)>(self.pl_margin_tag);
    }

    [[nodiscard]]
    constexpr auto as_md_block_quotes(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::md_block_quotes, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::md_block_quotes);
        return ::std::forward_like<decltype(self)>(self.md_block_quotes);
    }

    [[nodiscard]]
    constexpr auto as_list_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::list_ul || self.kind == ::pltxt2htm::NodeKind::list_ol,
                         u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::list_info);
        return ::std::forward_like<decltype(self)>(self.list_info);
    }

    [[nodiscard]]
    constexpr auto as_cell(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::table_th || self.kind == ::pltxt2htm::NodeKind::table_td,
                         u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::cell);
        return ::std::forward_like<decltype(self)>(self.cell);
    }

    [[nodiscard]]
    constexpr auto as_align_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_p || self.kind == ::pltxt2htm::NodeKind::pl_align,
                         u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::align_info);
        return ::std::forward_like<decltype(self)>(self.align_info);
    }

    [[nodiscard]]
    constexpr auto as_list_li_checkbox(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::list_li_checkbox, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::list_li_checkbox);
        return ::std::forward_like<decltype(self)>(self.list_li_checkbox);
    }

    [[nodiscard]]
    constexpr auto as_table(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::table, u8"context kind mismatch");
        pltxt2htm_assert_context_branch(self, FrontendContextVariant<ndebug>::ContextBranch::table);
        return ::std::forward_like<decltype(self)>(self.table);
    }

    constexpr ~FrontendContextVariant() noexcept {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiments:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussions:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_trigger:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_internal:
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
        case ::pltxt2htm::NodeKind::pl_voffset: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_voffset_tag);
            ::std::destroy_at(::std::addressof(this->pl_voffset_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_margin: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_margin_tag);
            ::std::destroy_at(::std::addressof(this->pl_margin_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_align: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::align_info);
            ::std::destroy_at(::std::addressof(this->align_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_span_info);
            ::std::destroy_at(::std::addressof(this->html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_div_info);
            ::std::destroy_at(::std::addressof(this->html_div_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::html_mark_info);
            ::std::destroy_at(::std::addressof(this->html_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_mark: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pl_mark_info);
            ::std::destroy_at(::std::addressof(this->pl_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::md_block_quotes);
            ::std::destroy_at(::std::addressof(this->md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeKind::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ol: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::list_info);
            ::std::destroy_at(::std::addressof(this->list_info));
            return;
        }
        case ::pltxt2htm::NodeKind::group:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
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
        case ::pltxt2htm::NodeKind::html_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_colgroup:
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
        case ::pltxt2htm::NodeKind::list_li: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::pltext);
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::align_info);
            ::std::destroy_at(::std::addressof(this->align_info));
            return;
        }
        case ::pltxt2htm::NodeKind::table_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_td: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::cell);
            ::std::destroy_at(::std::addressof(this->cell));
            return;
        }
        case ::pltxt2htm::NodeKind::table: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::table);
            ::std::destroy_at(::std::addressof(this->table));
            return;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            pltxt2htm_assert_context_branch(*this, ContextBranch::list_li_checkbox);
            ::std::destroy_at(::std::addressof(this->list_li_checkbox));
            return;
        }
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::code_fence:
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
        case ::pltxt2htm::NodeKind::invalid_utf8:
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
        case ::pltxt2htm::NodeKind::md_escape:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in FrontendContextVariant destructor");
            }
        }
    }
};

/**
 * @brief Main parser activation record, one frame per nesting level on the call stack.
 * @details Holds the context data (tag type + payload), the current parse index
 *          within the raw text, and the sub-AST being built for this frame.
 *          Frames are manually managed on a call-stack to avoid stack overflow.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
class ParserFrame {
    FrontendContextVariant<ndebug> context_data;

public:
    ::std::size_t current_index{}; ///< Current parse position in the raw text.
    ::pltxt2htm::Ast<ndebug> subast; ///< Sub-AST being built for this frame.

    constexpr explicit ParserFrame(FrontendContextVariant<ndebug>&& ctx, ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
        : context_data(::std::move(ctx)),
          subast(::std::move(subast_)) {
    }

    constexpr ParserFrame(ParserFrame<ndebug> const&) noexcept = delete;

    constexpr ParserFrame(ParserFrame<ndebug>&& other) noexcept
        : context_data{::std::move(other.context_data)},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
    }

    constexpr auto operator=(ParserFrame<ndebug> const&) noexcept -> ParserFrame<ndebug>& = delete;
    constexpr auto operator=(ParserFrame<ndebug>&&) noexcept -> ParserFrame<ndebug>& = delete;

    constexpr ~ParserFrame() noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this ParserFrame<ndebug> const& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.context_data.get_kind();
    }

    [[nodiscard]]
    constexpr auto get_pltext(this ParserFrame<ndebug> const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        auto const& context_data_ref = self.context_data;
        switch (context_data_ref.get_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_utf8:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::group:
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
        case ::pltxt2htm::NodeKind::html_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_s:
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
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
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
        case ::pltxt2htm::NodeKind::code_fence:
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
        case ::pltxt2htm::NodeKind::list_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block: {
            auto&& active_context_data = context_data_ref.as_pltext();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::list_li_checkbox: {
            auto&& active_context_data = context_data_ref.as_list_li_checkbox();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiments:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussions:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_trigger:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_internal:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            auto&& active_context_data = context_data_ref.as_equal_sign_tag();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            auto&& active_context_data = context_data_ref.as_url_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_link: {
            auto&& active_context_data = context_data_ref.as_url_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            auto&& active_context_data = context_data_ref.as_pl_size_tag();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_voffset: {
            auto&& active_context_data = context_data_ref.as_pl_voffset_tag();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_margin: {
            auto&& active_context_data = context_data_ref.as_pl_margin_tag();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_align: {
            auto&& active_context_data = context_data_ref.as_align_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            auto&& active_context_data = context_data_ref.as_html_span_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            auto&& active_context_data = context_data_ref.as_html_div_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            auto&& active_context_data = context_data_ref.as_html_mark_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_mark: {
            auto&& active_context_data = context_data_ref.as_pl_mark_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::html_a: {
            auto&& active_context_data = context_data_ref.as_html_a_tag_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            auto&& active_context_data = context_data_ref.as_md_block_quotes();
            auto const& pltext = active_context_data.pltext;
            return ::pltxt2htm::container::U8StringView{pltext};
        }
        case ::pltxt2htm::NodeKind::md_link: {
            auto&& active_context_data = context_data_ref.as_url_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            auto&& active_context_data = context_data_ref.as_align_info();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::table_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_td: {
            auto&& active_context_data = context_data_ref.as_cell();
            return active_context_data.pltext;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in context-kind switch");
            }
        }
        // suppress GCC -Wreturn-type warning
        pltxt2htm_unreachable(u8"Unreachable after context-kind switch");
    }

    [[nodiscard]]
    constexpr auto as_pltext(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_pltext();
    }

    [[nodiscard]]
    constexpr auto as_equal_sign_tag(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_equal_sign_tag();
    }

    [[nodiscard]]
    constexpr auto as_html_span_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_html_span_info();
    }

    [[nodiscard]]
    constexpr auto as_html_div_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_html_div_info();
    }

    [[nodiscard]]
    constexpr auto as_html_mark_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_html_mark_info();
    }

    [[nodiscard]]
    constexpr auto as_pl_mark_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_pl_mark_info();
    }

    [[nodiscard]]
    constexpr auto as_url_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_url_info();
    }

    [[nodiscard]]
    constexpr auto as_html_a_tag_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_html_a_tag_info();
    }

    [[nodiscard]]
    constexpr auto as_pl_size_tag(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_pl_size_tag();
    }

    [[nodiscard]]
    constexpr auto as_pl_voffset_tag(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_pl_voffset_tag();
    }

    [[nodiscard]]
    constexpr auto as_pl_margin_tag(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_pl_margin_tag();
    }

    [[nodiscard]]
    constexpr auto as_md_block_quotes(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_md_block_quotes();
    }

    [[nodiscard]]
    constexpr auto as_list_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_list_info();
    }

    [[nodiscard]]
    constexpr auto as_cell(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_cell();
    }

    [[nodiscard]]
    constexpr auto as_align_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_align_info();
    }

    [[nodiscard]]
    constexpr auto as_list_li_checkbox(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_list_li_checkbox();
    }

    [[nodiscard]]
    constexpr auto as_table(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_table();
    }
};

/**
 * @brief Push a list frame for a freshly parsed top-level ListUlNode/ListOlNode.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void push_list_frame(::pltxt2htm::details::CallStack<ParserFrame<ndebug>>& call_stack,
                               ListBaseNode<ndebug>&& top_node) noexcept {
    switch (top_node.get_type()) {
    case ListNodeType::list_ul: {
        call_stack.push_frame(ParserFrame<ndebug>(
            FrontendContextVariant<ndebug>{ParserFrameContextWithListInfo<ndebug>{::std::move(top_node).get_sublist()},
                                           ::pltxt2htm::NodeKind::list_ul},
            ::pltxt2htm::Ast<ndebug>{}));
        break;
    }
    case ListNodeType::list_ol: {
        call_stack.push_frame(ParserFrame<ndebug>(
            FrontendContextVariant<ndebug>{
                ParserFrameContextWithListInfo<ndebug>{::std::move(top_node).get_sublist(), top_node.get_start()},
                ::pltxt2htm::NodeKind::list_ol},
            ::pltxt2htm::Ast<ndebug>{}));
        break;
    }
    case ListNodeType::list_li:
        [[fallthrough]];
    case ListNodeType::list_li_checkbox:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected list_li/list_li_checkbox in push_list_frame()");
        }
    }
}

/**
 * @brief Process the top table frame on the call stack.
 *
 * Implements the shared table state machine (caption / body / finish):
 * - caption: push a caption frame when the table has one;
 * - body: push one cell frame (th/td) per cell in row-major order;
 * - finish: group cells into &lt;tr&gt; rows and consecutive same-section rows
 *   into &lt;thead&gt;/&lt;tbody&gt;/&lt;tfoot&gt; nodes, wrap the table node
 *   and pop the frame.
 *
 * @tparam ndebug Contract checking mode.
 * @param call_stack Frame call stack whose top frame is a table frame.
 * @return The finished table AST when the top-level table has been fully
 *         consumed; nullopt otherwise (the caller should re-enter its parse
 *         loop).
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto process_table_frame(::pltxt2htm::details::CallStack<ParserFrame<ndebug>>& call_stack) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::Ast<ndebug>> {
    auto&& frame = call_stack.template current_frame<ndebug>();
    auto&& raw_ast = frame.as_table().raw_ast;
    auto const state = frame.as_table().state;
    auto const row_index = frame.as_table().row_index;
    auto const cell_index = frame.as_table().cell_index;

    switch (state) /* -Werror=switch */ {
    case TableParsePhase::caption: {
        auto&& active_frame_data = frame.as_table();
        if (raw_ast.has_caption()) {
            call_stack.push_frame(
                ParserFrame<ndebug>(FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{raw_ast.caption()},
                                                                   ::pltxt2htm::NodeKind::table_caption},
                                    ::pltxt2htm::Ast<ndebug>{}));
        }
        active_frame_data.state = TableParsePhase::body;
        return ::pltxt2htm::container::nullopt;
    }
    case TableParsePhase::body: {
        auto&& active_frame_data = frame.as_table();
        if (row_index < raw_ast.rows_count()) {
            auto const row_cells = raw_ast.row_cells(row_index);
            if (cell_index < row_cells.size()) {
                auto const& cell = raw_ast.cell_at(row_index, cell_index);
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{
                        ParserFrameContextWithCellInfo{::pltxt2htm::container::U8StringView{cell.text}, cell.align},
                        cell.is_header ? ::pltxt2htm::NodeKind::table_th : ::pltxt2htm::NodeKind::table_td},
                    ::pltxt2htm::Ast<ndebug>{}));
                active_frame_data.cell_index = cell_index + 1;
                return ::pltxt2htm::container::nullopt;
            }
            active_frame_data.row_index = row_index + 1;
            active_frame_data.cell_index = 0;
            return ::pltxt2htm::container::nullopt;
        }
        active_frame_data.state = TableParsePhase::finish;
        return ::pltxt2htm::container::nullopt;
    }
    case TableParsePhase::finish: {
        auto previous_frame = ::std::move(frame);
        call_stack.template discard_current_frame<ndebug>();

        ::pltxt2htm::Ast<ndebug> flat_ast = ::std::move(previous_frame.subast);
        auto&& prev_raw_ast = previous_frame.as_table().raw_ast;
        ::pltxt2htm::Ast<ndebug> table_ast{};
        ::std::size_t cell_cursor{};

        // caption node (the caption frame is pushed first, so it is flat_ast[0]).
        if (prev_raw_ast.has_caption()) {
            table_ast.push_back(::std::move(flat_ast.template index<ndebug>(cell_cursor)));
            ++cell_cursor;
        }

        // colgroup node built directly from the collected col count.
        if (prev_raw_ast.has_colgroup()) {
            ::pltxt2htm::Ast<ndebug> colgroup_ast{};
            ::std::size_t const column_count{prev_raw_ast.get_col_count()};
            for (::std::size_t c{}; c < column_count; ++c) {
                colgroup_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableCol{}));
            }
            table_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableColgroup<ndebug>{::std::move(colgroup_ast)}));
        }

        // Group cells into <tr> rows, then consecutive rows of the same section into
        // <thead>/<tbody>/<tfoot>. Direct rows (section == none) are emitted as bare
        // <tr> under <table>.
        TableRowSection active_section{TableRowSection::none};
        ::pltxt2htm::Ast<ndebug> active_section_ast{};
        ::std::size_t const rows_count{prev_raw_ast.rows_count()};
        ::std::size_t const flat_ast_size{flat_ast.size()};
        for (::std::size_t r{}; r < rows_count; ++r) {
            ::pltxt2htm::Ast<ndebug> tr_ast{};
            auto const row_cells = prev_raw_ast.row_cells(r);
            ::std::size_t const row_cells_size{row_cells.size()};
            for (::std::size_t c{}; c < row_cells_size && cell_cursor < flat_ast_size; ++c, ++cell_cursor) {
                tr_ast.push_back(::std::move(flat_ast.template index<ndebug>(cell_cursor)));
            }
            auto const section = prev_raw_ast.row_section(r);
            if (section == TableRowSection::none) {
                ::pltxt2htm::details::push_table_section_node<ndebug>(table_ast, active_section,
                                                                      ::std::move(active_section_ast));
                active_section = TableRowSection::none;
                table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTr<ndebug>{::std::move(tr_ast)}));
            }
            else if (section == active_section) {
                active_section_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTr<ndebug>{::std::move(tr_ast)}));
            }
            else {
                ::pltxt2htm::details::push_table_section_node<ndebug>(table_ast, active_section,
                                                                      ::std::move(active_section_ast));
                active_section = section;
                active_section_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTr<ndebug>{::std::move(tr_ast)}));
            }
        }
        ::pltxt2htm::details::push_table_section_node<ndebug>(table_ast, active_section,
                                                              ::std::move(active_section_ast));

        if (call_stack.empty()) {
            return table_ast;
        }

        auto&& parent_frame = call_stack.template current_frame<ndebug>();
        parent_frame.subast.push_back(
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Table<ndebug>{::std::move(table_ast)}));
        return ::pltxt2htm::container::nullopt;
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected TableParsePhase");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable after TableParsePhase switch");
}

} // namespace pltxt2htm::details

#pragma pop_macro("pltxt2htm_assert_context_branch")
#include "../pop_macro.hh"
