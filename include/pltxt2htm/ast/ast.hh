/**
 * @file ast.hh
 * @brief AST tagged union node definition for pltxt2htm
 * @details Defines the main AST node class template ::pltxt2htm::PlTxtNode,
 *          which is a tagged union (discriminated by ::pltxt2htm::NodeKind) over
 *          all concrete node types (basic, HTML, Markdown, Physics-Lab).
 */

#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <exception/exception.hh>
#include <memory>

#include "../contracts.hh"
#include "node_type.hh"

#include "impl/basic_node_decl.hh"
#include "impl/html_node_decl.hh"
#include "impl/markdown_node_decl.hh"
#include "impl/physics_lab_node_decl.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
class PlTxtNode {
    union {
        // basic node
        ::pltxt2htm::U8Char u8char_node;
        ::pltxt2htm::InvalidU8Char invalid_u8char_node;
        ::pltxt2htm::Text<ndebug> text_node;

        // html node
        ::pltxt2htm::LineBreak line_break_node;
        ::pltxt2htm::HtmlBr br_node;
        ::pltxt2htm::Space space_node;
        ::pltxt2htm::LessThan less_than_node;
        ::pltxt2htm::GreaterThan greater_than_node;
        ::pltxt2htm::Tab tab_node;
        ::pltxt2htm::Ampersand ampersand_node;
        ::pltxt2htm::SingleQuote single_quote_node;
        ::pltxt2htm::DoubleQuote double_quote_node;
        ::pltxt2htm::HtmlHr hr_node;
        ::pltxt2htm::HtmlH1<ndebug> h1_node;
        ::pltxt2htm::HtmlH2<ndebug> h2_node;
        ::pltxt2htm::HtmlH3<ndebug> h3_node;
        ::pltxt2htm::HtmlH4<ndebug> h4_node;
        ::pltxt2htm::HtmlH5<ndebug> h5_node;
        ::pltxt2htm::HtmlH6<ndebug> h6_node;
        ::pltxt2htm::HtmlP<ndebug> p_node;
        ::pltxt2htm::HtmlDel<ndebug> del_node;
        ::pltxt2htm::HtmlNote<ndebug> note_node;
        ::pltxt2htm::HtmlEm<ndebug> em_node;
        ::pltxt2htm::HtmlStrong<ndebug> strong_node;
        ::pltxt2htm::HtmlUl<ndebug> ul_node;
        ::pltxt2htm::HtmlOl<ndebug> ol_node;
        ::pltxt2htm::HtmlLi<ndebug> li_node;
        ::pltxt2htm::HtmlCode<ndebug> code_node;
        ::pltxt2htm::HtmlPre<ndebug> pre_node;
        ::pltxt2htm::HtmlBlockquote<ndebug> blockquote_node;

        // html table node
        ::pltxt2htm::HtmlCol col_node;
        ::pltxt2htm::HtmlTable<ndebug> table_node;
        ::pltxt2htm::HtmlCaption<ndebug> caption_node;
        ::pltxt2htm::HtmlColgroup<ndebug> colgroup_node;
        ::pltxt2htm::HtmlThead<ndebug> thead_node;
        ::pltxt2htm::HtmlTbody<ndebug> tbody_node;
        ::pltxt2htm::HtmlTfoot<ndebug> tfoot_node;
        ::pltxt2htm::HtmlTr<ndebug> tr_node;
        ::pltxt2htm::HtmlTh<ndebug> th_node;
        ::pltxt2htm::HtmlTd<ndebug> td_node;

        // markdown node
        ::pltxt2htm::MdAtxH1<ndebug> md_atx_h1_node;
        ::pltxt2htm::MdAtxH2<ndebug> md_atx_h2_node;
        ::pltxt2htm::MdAtxH3<ndebug> md_atx_h3_node;
        ::pltxt2htm::MdAtxH4<ndebug> md_atx_h4_node;
        ::pltxt2htm::MdAtxH5<ndebug> md_atx_h5_node;
        ::pltxt2htm::MdAtxH6<ndebug> md_atx_h6_node;
        ::pltxt2htm::MdEscapeBackslash md_escape_backslash_node;
        ::pltxt2htm::MdEscapeExclamation md_escape_exclamation_node;
        ::pltxt2htm::MdEscapeDoubleQuote md_escape_double_quote_node;
        ::pltxt2htm::MdEscapeHash md_escape_hash_node;
        ::pltxt2htm::MdEscapeDollar md_escape_dollar_node;
        ::pltxt2htm::MdEscapePercent md_escape_percent_node;
        ::pltxt2htm::MdEscapeAmpersand md_escape_ampersand_node;
        ::pltxt2htm::MdEscapeSingleQuote md_escape_single_quote_node;
        ::pltxt2htm::MdEscapeLeftParen md_escape_left_paren_node;
        ::pltxt2htm::MdEscapeRightParen md_escape_right_paren_node;
        ::pltxt2htm::MdEscapeAsterisk md_escape_asterisk_node;
        ::pltxt2htm::MdEscapePlus md_escape_plus_node;
        ::pltxt2htm::MdEscapeComma md_escape_comma_node;
        ::pltxt2htm::MdEscapeHyphen md_escape_hyphen_node;
        ::pltxt2htm::MdEscapeDot md_escape_dot_node;
        ::pltxt2htm::MdEscapeSlash md_escape_slash_node;
        ::pltxt2htm::MdEscapeColon md_escape_colon_node;
        ::pltxt2htm::MdEscapeSemicolon md_escape_semicolon_node;
        ::pltxt2htm::MdEscapeLessThan md_escape_less_than_node;
        ::pltxt2htm::MdEscapeEquals md_escape_equals_node;
        ::pltxt2htm::MdEscapeGreaterThan md_escape_greater_than_node;
        ::pltxt2htm::MdEscapeQuestion md_escape_question_node;
        ::pltxt2htm::MdEscapeAt md_escape_at_node;
        ::pltxt2htm::MdEscapeLeftBracket md_escape_left_bracket_node;
        ::pltxt2htm::MdEscapeRightBracket md_escape_right_bracket_node;
        ::pltxt2htm::MdEscapeCaret md_escape_caret_node;
        ::pltxt2htm::MdEscapeUnderscore md_escape_underscore_node;
        ::pltxt2htm::MdEscapeBacktick md_escape_backtick_node;
        ::pltxt2htm::MdEscapeLeftBrace md_escape_left_brace_node;
        ::pltxt2htm::MdEscapePipe md_escape_pipe_node;
        ::pltxt2htm::MdEscapeRightBrace md_escape_right_brace_node;
        ::pltxt2htm::MdEscapeTilde md_escape_tilde_node;
        ::pltxt2htm::MdHr md_hr_node;
        ::pltxt2htm::MdCodeFenceBacktick<ndebug> md_code_fence_backtick_node;
        ::pltxt2htm::MdCodeFenceTilde<ndebug> md_code_fence_tilde_node;
        ::pltxt2htm::MdCodeSpan1Backtick<ndebug> md_code_span_1_backtick_node;
        ::pltxt2htm::MdCodeSpan2Backtick<ndebug> md_code_span_2_backtick_node;
        ::pltxt2htm::MdCodeSpan3Backtick<ndebug> md_code_span_3_backtick_node;
        ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> md_single_emphasis_asterisk_node;
        ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> md_double_emphasis_asterisk_node;
        ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> md_triple_emphasis_asterisk_node;
        ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> md_single_emphasis_underscore_node;
        ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> md_double_emphasis_underscore_node;
        ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> md_triple_emphasis_underscore_node;
        ::pltxt2htm::MdDel<ndebug> md_del_node;
        ::pltxt2htm::MdLink<ndebug> md_link_node;
        ::pltxt2htm::Url<ndebug> url_node;
        ::pltxt2htm::MdImage<ndebug> md_image_node;
        ::pltxt2htm::MdBlockQuotes<ndebug> md_block_quotes_node;
        ::pltxt2htm::MdUl<ndebug> md_ul_node;
        ::pltxt2htm::MdOl<ndebug> md_ol_node;
        ::pltxt2htm::MdLi<ndebug> md_li_node;
        ::pltxt2htm::MdLiCheckbox<ndebug> md_li_checkbox_node;
        ::pltxt2htm::MdTable<ndebug> md_table_node;
        ::pltxt2htm::MdThead<ndebug> md_thead_node;
        ::pltxt2htm::MdTbody<ndebug> md_tbody_node;
        ::pltxt2htm::MdTr<ndebug> md_tr_node;
        ::pltxt2htm::MdTh<ndebug> md_th_node;
        ::pltxt2htm::MdTd<ndebug> md_td_node;
        ::pltxt2htm::MdLatexInline<ndebug> md_latex_inline_node;
        ::pltxt2htm::MdLatexBlock<ndebug> md_latex_block_node;

        // physics lab node
        ::pltxt2htm::PlColor<ndebug> pl_color_node;
        ::pltxt2htm::PlA<ndebug> pl_a_node;
        ::pltxt2htm::PlExperiment<ndebug> pl_experiment_node;
        ::pltxt2htm::PlDiscussion<ndebug> pl_discussion_node;
        ::pltxt2htm::PlUser<ndebug> pl_user_node;
        ::pltxt2htm::PlExternal<ndebug> pl_external_node;
        ::pltxt2htm::PlSize<ndebug> pl_size_node;
        ::pltxt2htm::PlI<ndebug> pl_i_node;
        ::pltxt2htm::PlB<ndebug> pl_b_node;
        ::pltxt2htm::PlMacroProject pl_macro_project_node;
        ::pltxt2htm::PlMacroVisitor pl_macro_visitor_node;
        ::pltxt2htm::PlMacroAuthor pl_macro_author_node;
        ::pltxt2htm::PlMacroCoauthors pl_macro_coauthors_node;
    };

    ::pltxt2htm::NodeKind node_kind;

public:
    constexpr PlTxtNode(::pltxt2htm::U8Char&& node) noexcept
        : u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::u8char} {
    }

    constexpr PlTxtNode(::pltxt2htm::InvalidU8Char&& node) noexcept
        : invalid_u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::invalid_u8char} {
    }

    constexpr PlTxtNode(::pltxt2htm::Text<ndebug>&& node) noexcept
        : text_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::text} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlColor<ndebug>&& node) noexcept
        : pl_color_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_color} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlA<ndebug>&& node) noexcept
        : pl_a_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_a} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlExperiment<ndebug>&& node) noexcept
        : pl_experiment_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_experiment} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlDiscussion<ndebug>&& node) noexcept
        : pl_discussion_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_discussion} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlUser<ndebug>&& node) noexcept
        : pl_user_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_user} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlExternal<ndebug>&& node) noexcept
        : pl_external_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_external} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlSize<ndebug>&& node) noexcept
        : pl_size_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_size} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlI<ndebug>&& node) noexcept
        : pl_i_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_i} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlB<ndebug>&& node) noexcept
        : pl_b_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_b} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlMacroProject&& node) noexcept
        : pl_macro_project_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_macro_project} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlMacroVisitor&& node) noexcept
        : pl_macro_visitor_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_macro_visitor} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlMacroAuthor&& node) noexcept
        : pl_macro_author_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_macro_author} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlMacroCoauthors&& node) noexcept
        : pl_macro_coauthors_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::pl_macro_coauthors} {
    }

    constexpr PlTxtNode(::pltxt2htm::LineBreak&& node) noexcept
        : line_break_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::line_break} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlBr&& node) noexcept
        : br_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_br} {
    }

    constexpr PlTxtNode(::pltxt2htm::Space&& node) noexcept
        : space_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::space} {
    }

    constexpr PlTxtNode(::pltxt2htm::LessThan&& node) noexcept
        : less_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::less_than} {
    }

    constexpr PlTxtNode(::pltxt2htm::GreaterThan&& node) noexcept
        : greater_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::greater_than} {
    }

    constexpr PlTxtNode(::pltxt2htm::Tab&& node) noexcept
        : tab_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::tab} {
    }

    constexpr PlTxtNode(::pltxt2htm::Ampersand&& node) noexcept
        : ampersand_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::ampersand} {
    }

    constexpr PlTxtNode(::pltxt2htm::SingleQuote&& node) noexcept
        : single_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::single_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::DoubleQuote&& node) noexcept
        : double_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::double_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlHr&& node) noexcept
        : hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_hr} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH1<ndebug>&& node) noexcept
        : h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h1} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH2<ndebug>&& node) noexcept
        : h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h2} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH3<ndebug>&& node) noexcept
        : h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h3} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH4<ndebug>&& node) noexcept
        : h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h4} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH5<ndebug>&& node) noexcept
        : h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h5} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlH6<ndebug>&& node) noexcept
        : h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h6} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlP<ndebug>&& node) noexcept
        : p_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_p} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlDel<ndebug>&& node) noexcept
        : del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_del} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlNote<ndebug>&& node) noexcept
        : note_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_note} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlEm<ndebug>&& node) noexcept
        : em_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_em} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlStrong<ndebug>&& node) noexcept
        : strong_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_strong} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlUl<ndebug>&& node) noexcept
        : ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_ul} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlOl<ndebug>&& node) noexcept
        : ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_ol} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlLi<ndebug>&& node) noexcept
        : li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_li} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlCode<ndebug>&& node) noexcept
        : code_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_code} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlPre<ndebug>&& node) noexcept
        : pre_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_pre} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlBlockquote<ndebug>&& node) noexcept
        : blockquote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_blockquote} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlCol&& node) noexcept
        : col_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_col} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTable<ndebug>&& node) noexcept
        : table_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_table} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlCaption<ndebug>&& node) noexcept
        : caption_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_caption} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlColgroup<ndebug>&& node) noexcept
        : colgroup_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_colgroup} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlThead<ndebug>&& node) noexcept
        : thead_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_thead} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTbody<ndebug>&& node) noexcept
        : tbody_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_tbody} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTfoot<ndebug>&& node) noexcept
        : tfoot_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_tfoot} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTr<ndebug>&& node) noexcept
        : tr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_tr} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTh<ndebug>&& node) noexcept
        : th_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_th} {
    }

    constexpr PlTxtNode(::pltxt2htm::HtmlTd<ndebug>&& node) noexcept
        : td_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_td} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH1<ndebug>&& node) noexcept
        : md_atx_h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h1} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH2<ndebug>&& node) noexcept
        : md_atx_h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h2} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH3<ndebug>&& node) noexcept
        : md_atx_h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h3} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH4<ndebug>&& node) noexcept
        : md_atx_h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h4} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH5<ndebug>&& node) noexcept
        : md_atx_h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h5} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdAtxH6<ndebug>&& node) noexcept
        : md_atx_h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_atx_h6} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeBackslash&& node) noexcept
        : md_escape_backslash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_backslash} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeExclamation&& node) noexcept
        : md_escape_exclamation_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_exclamation} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeDoubleQuote&& node) noexcept
        : md_escape_double_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_double_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeHash&& node) noexcept
        : md_escape_hash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_hash} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeDollar&& node) noexcept
        : md_escape_dollar_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_dollar} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapePercent&& node) noexcept
        : md_escape_percent_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_percent} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeAmpersand&& node) noexcept
        : md_escape_ampersand_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_ampersand} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeSingleQuote&& node) noexcept
        : md_escape_single_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_single_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeLeftParen&& node) noexcept
        : md_escape_left_paren_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_left_paren} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeRightParen&& node) noexcept
        : md_escape_right_paren_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_right_paren} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeAsterisk&& node) noexcept
        : md_escape_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_asterisk} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapePlus&& node) noexcept
        : md_escape_plus_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_plus} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeComma&& node) noexcept
        : md_escape_comma_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_comma} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeHyphen&& node) noexcept
        : md_escape_hyphen_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_hyphen} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeDot&& node) noexcept
        : md_escape_dot_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_dot} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeSlash&& node) noexcept
        : md_escape_slash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_slash} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeColon&& node) noexcept
        : md_escape_colon_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_colon} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeSemicolon&& node) noexcept
        : md_escape_semicolon_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_semicolon} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeLessThan&& node) noexcept
        : md_escape_less_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_less_than} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeEquals&& node) noexcept
        : md_escape_equals_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_equals} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeGreaterThan&& node) noexcept
        : md_escape_greater_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_greater_than} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeQuestion&& node) noexcept
        : md_escape_question_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_question} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeAt&& node) noexcept
        : md_escape_at_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_at} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeLeftBracket&& node) noexcept
        : md_escape_left_bracket_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_left_bracket} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeRightBracket&& node) noexcept
        : md_escape_right_bracket_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_right_bracket} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeCaret&& node) noexcept
        : md_escape_caret_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_caret} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeUnderscore&& node) noexcept
        : md_escape_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_underscore} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeBacktick&& node) noexcept
        : md_escape_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_backtick} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeLeftBrace&& node) noexcept
        : md_escape_left_brace_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_left_brace} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapePipe&& node) noexcept
        : md_escape_pipe_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_pipe} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeRightBrace&& node) noexcept
        : md_escape_right_brace_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_right_brace} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdEscapeTilde&& node) noexcept
        : md_escape_tilde_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_escape_tilde} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdHr&& node) noexcept
        : md_hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_hr} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdCodeFenceBacktick<ndebug>&& node) noexcept
        : md_code_fence_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_code_fence_backtick} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdCodeFenceTilde<ndebug>&& node) noexcept
        : md_code_fence_tilde_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_code_fence_tilde} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdCodeSpan1Backtick<ndebug>&& node) noexcept
        : md_code_span_1_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_code_span_1_backtick} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdCodeSpan2Backtick<ndebug>&& node) noexcept
        : md_code_span_2_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_code_span_2_backtick} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdCodeSpan3Backtick<ndebug>&& node) noexcept
        : md_code_span_3_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_code_span_3_backtick} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&& node) noexcept
        : md_single_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_single_emphasis_asterisk} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&& node) noexcept
        : md_double_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_double_emphasis_asterisk} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&& node) noexcept
        : md_triple_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_triple_emphasis_asterisk} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&& node) noexcept
        : md_single_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_single_emphasis_underscore} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&& node) noexcept
        : md_double_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_double_emphasis_underscore} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&& node) noexcept
        : md_triple_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_triple_emphasis_underscore} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdDel<ndebug>&& node) noexcept
        : md_del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_del} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLink<ndebug>&& node) noexcept
        : md_link_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_link} {
    }

    constexpr PlTxtNode(::pltxt2htm::Url<ndebug>&& node) noexcept
        : url_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::url} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdImage<ndebug>&& node) noexcept
        : md_image_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_image} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdBlockQuotes<ndebug>&& node) noexcept
        : md_block_quotes_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_block_quotes} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdUl<ndebug>&& node) noexcept
        : md_ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_ul} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdOl<ndebug>&& node) noexcept
        : md_ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_ol} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLi<ndebug>&& node) noexcept
        : md_li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_li} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLiCheckbox<ndebug>&& node) noexcept
        : md_li_checkbox_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_li_checkbox} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTable<ndebug>&& node) noexcept
        : md_table_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_table} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdThead<ndebug>&& node) noexcept
        : md_thead_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_thead} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTbody<ndebug>&& node) noexcept
        : md_tbody_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_tbody} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTr<ndebug>&& node) noexcept
        : md_tr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_tr} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTh<ndebug>&& node) noexcept
        : md_th_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_th} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdTd<ndebug>&& node) noexcept
        : md_td_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_td} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLatexInline<ndebug>&& node) noexcept
        : md_latex_inline_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_latex_inline} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLatexBlock<ndebug>&& node) noexcept
        : md_latex_block_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_latex_block} {
    }

    /**
     * @note: Move construction is sufficient for this codebase. Copy construction is provided for completeness only.
     */
#if 1
    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug> const& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            new (::std::addressof(u8char_node))::pltxt2htm::U8Char(other.u8char_node);
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            new (::std::addressof(invalid_u8char_node))::pltxt2htm::InvalidU8Char(other.invalid_u8char_node);
            break;
        }
        case ::pltxt2htm::NodeKind::text: {
            new (::std::addressof(text_node))::pltxt2htm::Text(other.text_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            new (::std::addressof(pl_color_node))::pltxt2htm::PlColor(other.pl_color_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            new (::std::addressof(pl_a_node))::pltxt2htm::PlA<ndebug>(other.pl_a_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            new (::std::addressof(pl_experiment_node))::pltxt2htm::PlExperiment(other.pl_experiment_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            new (::std::addressof(pl_discussion_node))::pltxt2htm::PlDiscussion(other.pl_discussion_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            new (::std::addressof(pl_user_node))::pltxt2htm::PlUser(other.pl_user_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            new (::std::addressof(pl_external_node))::pltxt2htm::PlExternal(other.pl_external_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            new (::std::addressof(pl_size_node))::pltxt2htm::PlSize(other.pl_size_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            new (::std::addressof(pl_i_node))::pltxt2htm::PlI(other.pl_i_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            new (::std::addressof(pl_b_node))::pltxt2htm::PlB(other.pl_b_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            new (::std::addressof(pl_macro_project_node))::pltxt2htm::PlMacroProject(other.pl_macro_project_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            new (::std::addressof(pl_macro_visitor_node))::pltxt2htm::PlMacroVisitor(other.pl_macro_visitor_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            new (::std::addressof(pl_macro_author_node))::pltxt2htm::PlMacroAuthor(other.pl_macro_author_node);
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            new (::std::addressof(pl_macro_coauthors_node))::pltxt2htm::PlMacroCoauthors(other.pl_macro_coauthors_node);
            break;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            new (::std::addressof(line_break_node))::pltxt2htm::LineBreak(other.line_break_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            new (::std::addressof(br_node))::pltxt2htm::HtmlBr(other.br_node);
            break;
        }
        case ::pltxt2htm::NodeKind::space: {
            new (::std::addressof(space_node))::pltxt2htm::Space(other.space_node);
            break;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            new (::std::addressof(less_than_node))::pltxt2htm::LessThan(other.less_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            new (::std::addressof(greater_than_node))::pltxt2htm::GreaterThan(other.greater_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::tab: {
            new (::std::addressof(tab_node))::pltxt2htm::Tab(other.tab_node);
            break;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            new (::std::addressof(ampersand_node))::pltxt2htm::Ampersand(other.ampersand_node);
            break;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            new (::std::addressof(single_quote_node))::pltxt2htm::SingleQuote(other.single_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            new (::std::addressof(double_quote_node))::pltxt2htm::DoubleQuote(other.double_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            new (::std::addressof(hr_node))::pltxt2htm::HtmlHr(other.hr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            new (::std::addressof(h1_node))::pltxt2htm::HtmlH1(other.h1_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            new (::std::addressof(h2_node))::pltxt2htm::HtmlH2(other.h2_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            new (::std::addressof(h3_node))::pltxt2htm::HtmlH3(other.h3_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            new (::std::addressof(h4_node))::pltxt2htm::HtmlH4(other.h4_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            new (::std::addressof(h5_node))::pltxt2htm::HtmlH5(other.h5_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            new (::std::addressof(h6_node))::pltxt2htm::HtmlH6(other.h6_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            new (::std::addressof(p_node))::pltxt2htm::HtmlP(other.p_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            new (::std::addressof(del_node))::pltxt2htm::HtmlDel(other.del_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            new (::std::addressof(note_node))::pltxt2htm::HtmlNote(other.note_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            new (::std::addressof(em_node))::pltxt2htm::HtmlEm(other.em_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            new (::std::addressof(strong_node))::pltxt2htm::HtmlStrong(other.strong_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            new (::std::addressof(ul_node))::pltxt2htm::HtmlUl(other.ul_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            new (::std::addressof(ol_node))::pltxt2htm::HtmlOl(other.ol_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            new (::std::addressof(li_node))::pltxt2htm::HtmlLi(other.li_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            new (::std::addressof(code_node))::pltxt2htm::HtmlCode(other.code_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            new (::std::addressof(pre_node))::pltxt2htm::HtmlPre(other.pre_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            new (::std::addressof(blockquote_node))::pltxt2htm::HtmlBlockquote(other.blockquote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_col: {
            new (::std::addressof(col_node))::pltxt2htm::HtmlCol(other.col_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            new (::std::addressof(table_node))::pltxt2htm::HtmlTable(other.table_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            new (::std::addressof(caption_node))::pltxt2htm::HtmlCaption(other.caption_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            new (::std::addressof(colgroup_node))::pltxt2htm::HtmlColgroup(other.colgroup_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            new (::std::addressof(thead_node))::pltxt2htm::HtmlThead(other.thead_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            new (::std::addressof(tbody_node))::pltxt2htm::HtmlTbody(other.tbody_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            new (::std::addressof(tfoot_node))::pltxt2htm::HtmlTfoot(other.tfoot_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            new (::std::addressof(tr_node))::pltxt2htm::HtmlTr(other.tr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            new (::std::addressof(th_node))::pltxt2htm::HtmlTh(other.th_node);
            break;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            new (::std::addressof(td_node))::pltxt2htm::HtmlTd(other.td_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            new (::std::addressof(md_atx_h1_node))::pltxt2htm::MdAtxH1(other.md_atx_h1_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            new (::std::addressof(md_atx_h2_node))::pltxt2htm::MdAtxH2(other.md_atx_h2_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            new (::std::addressof(md_atx_h3_node))::pltxt2htm::MdAtxH3(other.md_atx_h3_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            new (::std::addressof(md_atx_h4_node))::pltxt2htm::MdAtxH4(other.md_atx_h4_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            new (::std::addressof(md_atx_h5_node))::pltxt2htm::MdAtxH5(other.md_atx_h5_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            new (::std::addressof(md_atx_h6_node))::pltxt2htm::MdAtxH6(other.md_atx_h6_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            new (::std::addressof(md_escape_backslash_node))::pltxt2htm::MdEscapeBackslash(
                other.md_escape_backslash_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            new (::std::addressof(md_escape_exclamation_node))::pltxt2htm::MdEscapeExclamation(
                other.md_escape_exclamation_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote: {
            new (::std::addressof(md_escape_double_quote_node))::pltxt2htm::MdEscapeDoubleQuote(
                other.md_escape_double_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            new (::std::addressof(md_escape_hash_node))::pltxt2htm::MdEscapeHash(other.md_escape_hash_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            new (::std::addressof(md_escape_dollar_node))::pltxt2htm::MdEscapeDollar(other.md_escape_dollar_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            new (::std::addressof(md_escape_percent_node))::pltxt2htm::MdEscapePercent(other.md_escape_percent_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand: {
            new (::std::addressof(md_escape_ampersand_node))::pltxt2htm::MdEscapeAmpersand(
                other.md_escape_ampersand_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote: {
            new (::std::addressof(md_escape_single_quote_node))::pltxt2htm::MdEscapeSingleQuote(
                other.md_escape_single_quote_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            new (::std::addressof(md_escape_left_paren_node))::pltxt2htm::MdEscapeLeftParen(
                other.md_escape_left_paren_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            new (::std::addressof(md_escape_right_paren_node))::pltxt2htm::MdEscapeRightParen(
                other.md_escape_right_paren_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            new (::std::addressof(md_escape_asterisk_node))::pltxt2htm::MdEscapeAsterisk(other.md_escape_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            new (::std::addressof(md_escape_plus_node))::pltxt2htm::MdEscapePlus(other.md_escape_plus_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            new (::std::addressof(md_escape_comma_node))::pltxt2htm::MdEscapeComma(other.md_escape_comma_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            new (::std::addressof(md_escape_hyphen_node))::pltxt2htm::MdEscapeHyphen(other.md_escape_hyphen_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            new (::std::addressof(md_escape_dot_node))::pltxt2htm::MdEscapeDot(other.md_escape_dot_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            new (::std::addressof(md_escape_slash_node))::pltxt2htm::MdEscapeSlash(other.md_escape_slash_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            new (::std::addressof(md_escape_colon_node))::pltxt2htm::MdEscapeColon(other.md_escape_colon_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            new (::std::addressof(md_escape_semicolon_node))::pltxt2htm::MdEscapeSemicolon(
                other.md_escape_semicolon_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than: {
            new (::std::addressof(md_escape_less_than_node))::pltxt2htm::MdEscapeLessThan(
                other.md_escape_less_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            new (::std::addressof(md_escape_equals_node))::pltxt2htm::MdEscapeEquals(other.md_escape_equals_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than: {
            new (::std::addressof(md_escape_greater_than_node))::pltxt2htm::MdEscapeGreaterThan(
                other.md_escape_greater_than_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            new (::std::addressof(md_escape_question_node))::pltxt2htm::MdEscapeQuestion(other.md_escape_question_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            new (::std::addressof(md_escape_at_node))::pltxt2htm::MdEscapeAt(other.md_escape_at_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            new (::std::addressof(md_escape_left_bracket_node))::pltxt2htm::MdEscapeLeftBracket(
                other.md_escape_left_bracket_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            new (::std::addressof(md_escape_right_bracket_node))::pltxt2htm::MdEscapeRightBracket(
                other.md_escape_right_bracket_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            new (::std::addressof(md_escape_caret_node))::pltxt2htm::MdEscapeCaret(other.md_escape_caret_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            new (::std::addressof(md_escape_underscore_node))::pltxt2htm::MdEscapeUnderscore(
                other.md_escape_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            new (::std::addressof(md_escape_backtick_node))::pltxt2htm::MdEscapeBacktick(other.md_escape_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            new (::std::addressof(md_escape_left_brace_node))::pltxt2htm::MdEscapeLeftBrace(
                other.md_escape_left_brace_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            new (::std::addressof(md_escape_pipe_node))::pltxt2htm::MdEscapePipe(other.md_escape_pipe_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            new (::std::addressof(md_escape_right_brace_node))::pltxt2htm::MdEscapeRightBrace(
                other.md_escape_right_brace_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            new (::std::addressof(md_escape_tilde_node))::pltxt2htm::MdEscapeTilde(other.md_escape_tilde_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            new (::std::addressof(md_hr_node))::pltxt2htm::MdHr(other.md_hr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            new (::std::addressof(md_code_fence_backtick_node))::pltxt2htm::MdCodeFenceBacktick(
                other.md_code_fence_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            new (::std::addressof(md_code_fence_tilde_node))::pltxt2htm::MdCodeFenceTilde(
                other.md_code_fence_tilde_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            new (::std::addressof(md_code_span_1_backtick_node))::pltxt2htm::MdCodeSpan1Backtick(
                other.md_code_span_1_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            new (::std::addressof(md_code_span_2_backtick_node))::pltxt2htm::MdCodeSpan2Backtick(
                other.md_code_span_2_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            new (::std::addressof(md_code_span_3_backtick_node))::pltxt2htm::MdCodeSpan3Backtick(
                other.md_code_span_3_backtick_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            new (::std::addressof(md_single_emphasis_asterisk_node))::pltxt2htm::MdSingleEmphasisAsterisk(
                other.md_single_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            new (::std::addressof(md_double_emphasis_asterisk_node))::pltxt2htm::MdDoubleEmphasisAsterisk(
                other.md_double_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            new (::std::addressof(md_triple_emphasis_asterisk_node))::pltxt2htm::MdTripleEmphasisAsterisk(
                other.md_triple_emphasis_asterisk_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            new (::std::addressof(md_single_emphasis_underscore_node))::pltxt2htm::MdSingleEmphasisUnderscore(
                other.md_single_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            new (::std::addressof(md_double_emphasis_underscore_node))::pltxt2htm::MdDoubleEmphasisUnderscore(
                other.md_double_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            new (::std::addressof(md_triple_emphasis_underscore_node))::pltxt2htm::MdTripleEmphasisUnderscore(
                other.md_triple_emphasis_underscore_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            new (::std::addressof(md_del_node))::pltxt2htm::MdDel(other.md_del_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            new (::std::addressof(md_link_node))::pltxt2htm::MdLink(other.md_link_node);
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            new (::std::addressof(url_node))::pltxt2htm::Url(other.url_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            new (::std::addressof(md_image_node))::pltxt2htm::MdImage(other.md_image_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            new (::std::addressof(md_block_quotes_node))::pltxt2htm::MdBlockQuotes(other.md_block_quotes_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            new (::std::addressof(md_ul_node))::pltxt2htm::MdUl(other.md_ul_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            new (::std::addressof(md_ol_node))::pltxt2htm::MdOl(other.md_ol_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            new (::std::addressof(md_li_node))::pltxt2htm::MdLi(other.md_li_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            new (::std::addressof(md_li_checkbox_node))::pltxt2htm::MdLiCheckbox(other.md_li_checkbox_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            new (::std::addressof(md_table_node))::pltxt2htm::MdTable(other.md_table_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            new (::std::addressof(md_thead_node))::pltxt2htm::MdThead(other.md_thead_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            new (::std::addressof(md_tbody_node))::pltxt2htm::MdTbody(other.md_tbody_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            new (::std::addressof(md_tr_node))::pltxt2htm::MdTr(other.md_tr_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            new (::std::addressof(md_th_node))::pltxt2htm::MdTh(other.md_th_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            new (::std::addressof(md_td_node))::pltxt2htm::MdTd(other.md_td_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            new (::std::addressof(md_latex_inline_node))::pltxt2htm::MdLatexInline(other.md_latex_inline_node);
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            new (::std::addressof(md_latex_block_node))::pltxt2htm::MdLatexBlock(other.md_latex_block_node);
            break;
        }
    // All enum values are covered above; default is unreachable.
    // #if 0 documents intent and keeps the formatter output clean.
    #if 0
        default:
            [[unlikely]] {
                ::exception::unreachable();
            }
    #endif
        }
    }
#else
    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug> const&) noexcept = delete;
#endif

    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug>&& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            new (::std::addressof(u8char_node))::pltxt2htm::U8Char(::std::move(other.u8char_node));
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            new (::std::addressof(invalid_u8char_node))::pltxt2htm::InvalidU8Char(
                ::std::move(other.invalid_u8char_node));
            break;
        }
        case ::pltxt2htm::NodeKind::text: {
            new (::std::addressof(text_node))::pltxt2htm::Text(::std::move(other.text_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            new (::std::addressof(pl_color_node))::pltxt2htm::PlColor(::std::move(other.pl_color_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            new (::std::addressof(pl_a_node))::pltxt2htm::PlA<ndebug>(::std::move(other.pl_a_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            new (::std::addressof(pl_experiment_node))::pltxt2htm::PlExperiment(::std::move(other.pl_experiment_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            new (::std::addressof(pl_discussion_node))::pltxt2htm::PlDiscussion(::std::move(other.pl_discussion_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            new (::std::addressof(pl_user_node))::pltxt2htm::PlUser(::std::move(other.pl_user_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            new (::std::addressof(pl_external_node))::pltxt2htm::PlExternal(::std::move(other.pl_external_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            new (::std::addressof(pl_size_node))::pltxt2htm::PlSize(::std::move(other.pl_size_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            new (::std::addressof(pl_i_node))::pltxt2htm::PlI(::std::move(other.pl_i_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            new (::std::addressof(pl_b_node))::pltxt2htm::PlB(::std::move(other.pl_b_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            new (::std::addressof(pl_macro_project_node))::pltxt2htm::PlMacroProject(
                ::std::move(other.pl_macro_project_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            new (::std::addressof(pl_macro_visitor_node))::pltxt2htm::PlMacroVisitor(
                ::std::move(other.pl_macro_visitor_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            new (::std::addressof(pl_macro_author_node))::pltxt2htm::PlMacroAuthor(
                ::std::move(other.pl_macro_author_node));
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            new (::std::addressof(pl_macro_coauthors_node))::pltxt2htm::PlMacroCoauthors(
                ::std::move(other.pl_macro_coauthors_node));
            break;
        }

        case ::pltxt2htm::NodeKind::line_break: {
            new (::std::addressof(line_break_node))::pltxt2htm::LineBreak(::std::move(other.line_break_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_br: {
            new (::std::addressof(br_node))::pltxt2htm::HtmlBr(::std::move(other.br_node));
            break;
        }

        case ::pltxt2htm::NodeKind::space: {
            new (::std::addressof(space_node))::pltxt2htm::Space(::std::move(other.space_node));
            break;
        }

        case ::pltxt2htm::NodeKind::less_than: {
            new (::std::addressof(less_than_node))::pltxt2htm::LessThan(::std::move(other.less_than_node));
            break;
        }

        case ::pltxt2htm::NodeKind::greater_than: {
            new (::std::addressof(greater_than_node))::pltxt2htm::GreaterThan(::std::move(other.greater_than_node));
            break;
        }

        case ::pltxt2htm::NodeKind::tab: {
            new (::std::addressof(tab_node))::pltxt2htm::Tab(::std::move(other.tab_node));
            break;
        }

        case ::pltxt2htm::NodeKind::ampersand: {
            new (::std::addressof(ampersand_node))::pltxt2htm::Ampersand(::std::move(other.ampersand_node));
            break;
        }

        case ::pltxt2htm::NodeKind::single_quote: {
            new (::std::addressof(single_quote_node))::pltxt2htm::SingleQuote(::std::move(other.single_quote_node));
            break;
        }

        case ::pltxt2htm::NodeKind::double_quote: {
            new (::std::addressof(double_quote_node))::pltxt2htm::DoubleQuote(::std::move(other.double_quote_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_hr: {
            new (::std::addressof(hr_node))::pltxt2htm::HtmlHr(::std::move(other.hr_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h1: {
            new (::std::addressof(h1_node))::pltxt2htm::HtmlH1(::std::move(other.h1_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h2: {
            new (::std::addressof(h2_node))::pltxt2htm::HtmlH2(::std::move(other.h2_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h3: {
            new (::std::addressof(h3_node))::pltxt2htm::HtmlH3(::std::move(other.h3_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h4: {
            new (::std::addressof(h4_node))::pltxt2htm::HtmlH4(::std::move(other.h4_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h5: {
            new (::std::addressof(h5_node))::pltxt2htm::HtmlH5(::std::move(other.h5_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h6: {
            new (::std::addressof(h6_node))::pltxt2htm::HtmlH6(::std::move(other.h6_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_p: {
            new (::std::addressof(p_node))::pltxt2htm::HtmlP(::std::move(other.p_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_del: {
            new (::std::addressof(del_node))::pltxt2htm::HtmlDel(::std::move(other.del_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_note: {
            new (::std::addressof(note_node))::pltxt2htm::HtmlNote(::std::move(other.note_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_em: {
            new (::std::addressof(em_node))::pltxt2htm::HtmlEm(::std::move(other.em_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_strong: {
            new (::std::addressof(strong_node))::pltxt2htm::HtmlStrong(::std::move(other.strong_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_ul: {
            new (::std::addressof(ul_node))::pltxt2htm::HtmlUl(::std::move(other.ul_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_ol: {
            new (::std::addressof(ol_node))::pltxt2htm::HtmlOl(::std::move(other.ol_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_li: {
            new (::std::addressof(li_node))::pltxt2htm::HtmlLi(::std::move(other.li_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_code: {
            new (::std::addressof(code_node))::pltxt2htm::HtmlCode(::std::move(other.code_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_pre: {
            new (::std::addressof(pre_node))::pltxt2htm::HtmlPre(::std::move(other.pre_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_blockquote: {
            new (::std::addressof(blockquote_node))::pltxt2htm::HtmlBlockquote(::std::move(other.blockquote_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_col: {
            new (::std::addressof(col_node))::pltxt2htm::HtmlCol(::std::move(other.col_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_table: {
            new (::std::addressof(table_node))::pltxt2htm::HtmlTable(::std::move(other.table_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_caption: {
            new (::std::addressof(caption_node))::pltxt2htm::HtmlCaption(::std::move(other.caption_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_colgroup: {
            new (::std::addressof(colgroup_node))::pltxt2htm::HtmlColgroup(::std::move(other.colgroup_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_thead: {
            new (::std::addressof(thead_node))::pltxt2htm::HtmlThead(::std::move(other.thead_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_tbody: {
            new (::std::addressof(tbody_node))::pltxt2htm::HtmlTbody(::std::move(other.tbody_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_tfoot: {
            new (::std::addressof(tfoot_node))::pltxt2htm::HtmlTfoot(::std::move(other.tfoot_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_tr: {
            new (::std::addressof(tr_node))::pltxt2htm::HtmlTr(::std::move(other.tr_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_th: {
            new (::std::addressof(th_node))::pltxt2htm::HtmlTh(::std::move(other.th_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_td: {
            new (::std::addressof(td_node))::pltxt2htm::HtmlTd(::std::move(other.td_node));
            break;
        }

        case ::pltxt2htm::NodeKind::md_atx_h1: {
            new (::std::addressof(md_atx_h1_node))::pltxt2htm::MdAtxH1(::std::move(other.md_atx_h1_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            new (::std::addressof(md_atx_h2_node))::pltxt2htm::MdAtxH2(::std::move(other.md_atx_h2_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            new (::std::addressof(md_atx_h3_node))::pltxt2htm::MdAtxH3(::std::move(other.md_atx_h3_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            new (::std::addressof(md_atx_h4_node))::pltxt2htm::MdAtxH4(::std::move(other.md_atx_h4_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            new (::std::addressof(md_atx_h5_node))::pltxt2htm::MdAtxH5(::std::move(other.md_atx_h5_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            new (::std::addressof(md_atx_h6_node))::pltxt2htm::MdAtxH6(::std::move(other.md_atx_h6_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            new (::std::addressof(md_escape_backslash_node))::pltxt2htm::MdEscapeBackslash(
                ::std::move(other.md_escape_backslash_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            new (::std::addressof(md_escape_exclamation_node))::pltxt2htm::MdEscapeExclamation(
                ::std::move(other.md_escape_exclamation_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote: {
            new (::std::addressof(md_escape_double_quote_node))::pltxt2htm::MdEscapeDoubleQuote(
                ::std::move(other.md_escape_double_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            new (::std::addressof(md_escape_hash_node))::pltxt2htm::MdEscapeHash(
                ::std::move(other.md_escape_hash_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            new (::std::addressof(md_escape_dollar_node))::pltxt2htm::MdEscapeDollar(
                ::std::move(other.md_escape_dollar_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            new (::std::addressof(md_escape_percent_node))::pltxt2htm::MdEscapePercent(
                ::std::move(other.md_escape_percent_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand: {
            new (::std::addressof(md_escape_ampersand_node))::pltxt2htm::MdEscapeAmpersand(
                ::std::move(other.md_escape_ampersand_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote: {
            new (::std::addressof(md_escape_single_quote_node))::pltxt2htm::MdEscapeSingleQuote(
                ::std::move(other.md_escape_single_quote_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            new (::std::addressof(md_escape_left_paren_node))::pltxt2htm::MdEscapeLeftParen(
                ::std::move(other.md_escape_left_paren_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            new (::std::addressof(md_escape_right_paren_node))::pltxt2htm::MdEscapeRightParen(
                ::std::move(other.md_escape_right_paren_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            new (::std::addressof(md_escape_asterisk_node))::pltxt2htm::MdEscapeAsterisk(
                ::std::move(other.md_escape_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            new (::std::addressof(md_escape_plus_node))::pltxt2htm::MdEscapePlus(
                ::std::move(other.md_escape_plus_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            new (::std::addressof(md_escape_comma_node))::pltxt2htm::MdEscapeComma(
                ::std::move(other.md_escape_comma_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            new (::std::addressof(md_escape_hyphen_node))::pltxt2htm::MdEscapeHyphen(
                ::std::move(other.md_escape_hyphen_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            new (::std::addressof(md_escape_dot_node))::pltxt2htm::MdEscapeDot(::std::move(other.md_escape_dot_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            new (::std::addressof(md_escape_slash_node))::pltxt2htm::MdEscapeSlash(
                ::std::move(other.md_escape_slash_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            new (::std::addressof(md_escape_colon_node))::pltxt2htm::MdEscapeColon(
                ::std::move(other.md_escape_colon_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            new (::std::addressof(md_escape_semicolon_node))::pltxt2htm::MdEscapeSemicolon(
                ::std::move(other.md_escape_semicolon_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than: {
            new (::std::addressof(md_escape_less_than_node))::pltxt2htm::MdEscapeLessThan(
                ::std::move(other.md_escape_less_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            new (::std::addressof(md_escape_equals_node))::pltxt2htm::MdEscapeEquals(
                ::std::move(other.md_escape_equals_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than: {
            new (::std::addressof(md_escape_greater_than_node))::pltxt2htm::MdEscapeGreaterThan(
                ::std::move(other.md_escape_greater_than_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            new (::std::addressof(md_escape_question_node))::pltxt2htm::MdEscapeQuestion(
                ::std::move(other.md_escape_question_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            new (::std::addressof(md_escape_at_node))::pltxt2htm::MdEscapeAt(::std::move(other.md_escape_at_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            new (::std::addressof(md_escape_left_bracket_node))::pltxt2htm::MdEscapeLeftBracket(
                ::std::move(other.md_escape_left_bracket_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            new (::std::addressof(md_escape_right_bracket_node))::pltxt2htm::MdEscapeRightBracket(
                ::std::move(other.md_escape_right_bracket_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            new (::std::addressof(md_escape_caret_node))::pltxt2htm::MdEscapeCaret(
                ::std::move(other.md_escape_caret_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            new (::std::addressof(md_escape_underscore_node))::pltxt2htm::MdEscapeUnderscore(
                ::std::move(other.md_escape_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            new (::std::addressof(md_escape_backtick_node))::pltxt2htm::MdEscapeBacktick(
                ::std::move(other.md_escape_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            new (::std::addressof(md_escape_left_brace_node))::pltxt2htm::MdEscapeLeftBrace(
                ::std::move(other.md_escape_left_brace_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            new (::std::addressof(md_escape_pipe_node))::pltxt2htm::MdEscapePipe(
                ::std::move(other.md_escape_pipe_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            new (::std::addressof(md_escape_right_brace_node))::pltxt2htm::MdEscapeRightBrace(
                ::std::move(other.md_escape_right_brace_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            new (::std::addressof(md_escape_tilde_node))::pltxt2htm::MdEscapeTilde(
                ::std::move(other.md_escape_tilde_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            new (::std::addressof(md_hr_node))::pltxt2htm::MdHr(::std::move(other.md_hr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            new (::std::addressof(md_code_fence_backtick_node))::pltxt2htm::MdCodeFenceBacktick(
                ::std::move(other.md_code_fence_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            new (::std::addressof(md_code_fence_tilde_node))::pltxt2htm::MdCodeFenceTilde(
                ::std::move(other.md_code_fence_tilde_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            new (::std::addressof(md_code_span_1_backtick_node))::pltxt2htm::MdCodeSpan1Backtick(
                ::std::move(other.md_code_span_1_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            new (::std::addressof(md_code_span_2_backtick_node))::pltxt2htm::MdCodeSpan2Backtick(
                ::std::move(other.md_code_span_2_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            new (::std::addressof(md_code_span_3_backtick_node))::pltxt2htm::MdCodeSpan3Backtick(
                ::std::move(other.md_code_span_3_backtick_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            new (::std::addressof(md_single_emphasis_asterisk_node))::pltxt2htm::MdSingleEmphasisAsterisk(
                ::std::move(other.md_single_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            new (::std::addressof(md_double_emphasis_asterisk_node))::pltxt2htm::MdDoubleEmphasisAsterisk(
                ::std::move(other.md_double_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            new (::std::addressof(md_triple_emphasis_asterisk_node))::pltxt2htm::MdTripleEmphasisAsterisk(
                ::std::move(other.md_triple_emphasis_asterisk_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            new (::std::addressof(md_single_emphasis_underscore_node))::pltxt2htm::MdSingleEmphasisUnderscore(
                ::std::move(other.md_single_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            new (::std::addressof(md_double_emphasis_underscore_node))::pltxt2htm::MdDoubleEmphasisUnderscore(
                ::std::move(other.md_double_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            new (::std::addressof(md_triple_emphasis_underscore_node))::pltxt2htm::MdTripleEmphasisUnderscore(
                ::std::move(other.md_triple_emphasis_underscore_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            new (::std::addressof(md_del_node))::pltxt2htm::MdDel(::std::move(other.md_del_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            new (::std::addressof(md_link_node))::pltxt2htm::MdLink(::std::move(other.md_link_node));
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            new (::std::addressof(url_node))::pltxt2htm::Url(::std::move(other.url_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            new (::std::addressof(md_image_node))::pltxt2htm::MdImage(::std::move(other.md_image_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            new (::std::addressof(md_block_quotes_node))::pltxt2htm::MdBlockQuotes(
                ::std::move(other.md_block_quotes_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            new (::std::addressof(md_ul_node))::pltxt2htm::MdUl(::std::move(other.md_ul_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            new (::std::addressof(md_ol_node))::pltxt2htm::MdOl(::std::move(other.md_ol_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            new (::std::addressof(md_li_node))::pltxt2htm::MdLi(::std::move(other.md_li_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            new (::std::addressof(md_li_checkbox_node))::pltxt2htm::MdLiCheckbox(
                ::std::move(other.md_li_checkbox_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            new (::std::addressof(md_table_node))::pltxt2htm::MdTable(::std::move(other.md_table_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            new (::std::addressof(md_thead_node))::pltxt2htm::MdThead(::std::move(other.md_thead_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            new (::std::addressof(md_tbody_node))::pltxt2htm::MdTbody(::std::move(other.md_tbody_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            new (::std::addressof(md_tr_node))::pltxt2htm::MdTr(::std::move(other.md_tr_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            new (::std::addressof(md_th_node))::pltxt2htm::MdTh(::std::move(other.md_th_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            new (::std::addressof(md_td_node))::pltxt2htm::MdTd(::std::move(other.md_td_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            new (::std::addressof(md_latex_inline_node))::pltxt2htm::MdLatexInline(
                ::std::move(other.md_latex_inline_node));
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            new (::std::addressof(md_latex_block_node))::pltxt2htm::MdLatexBlock(
                ::std::move(other.md_latex_block_node));
            break;
        }
// All enum values are covered above; default is unreachable.
// #if 0 documents intent and keeps the formatter output clean.
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable();
            }
#endif
        }
    }

    constexpr ~PlTxtNode() noexcept {
        switch (node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            u8char_node.~U8Char();
            break;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            invalid_u8char_node.~InvalidU8Char();
            break;
        }
        case ::pltxt2htm::NodeKind::text: {
            text_node.~Text();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            pl_color_node.~PlColor();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            pl_a_node.~PlA();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            pl_experiment_node.~PlExperiment();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            pl_discussion_node.~PlDiscussion();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            pl_user_node.~PlUser();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            pl_external_node.~PlExternal();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            pl_size_node.~PlSize();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            pl_i_node.~PlI();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            pl_b_node.~PlB();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            pl_macro_project_node.~PlMacroProject();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            pl_macro_visitor_node.~PlMacroVisitor();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            pl_macro_author_node.~PlMacroAuthor();
            break;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            pl_macro_coauthors_node.~PlMacroCoauthors();
            break;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            line_break_node.~LineBreak();
            break;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            br_node.~HtmlBr();
            break;
        }
        case ::pltxt2htm::NodeKind::space: {
            space_node.~Space();
            break;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            less_than_node.~LessThan();
            break;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            greater_than_node.~GreaterThan();
            break;
        }
        case ::pltxt2htm::NodeKind::tab: {
            tab_node.~Tab();
            break;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            ampersand_node.~Ampersand();
            break;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            single_quote_node.~SingleQuote();
            break;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            double_quote_node.~DoubleQuote();
            break;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            hr_node.~HtmlHr();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            h1_node.~HtmlH1();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            h2_node.~HtmlH2();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            h3_node.~HtmlH3();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            h4_node.~HtmlH4();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            h5_node.~HtmlH5();
            break;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            h6_node.~HtmlH6();
            break;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            p_node.~HtmlP();
            break;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            del_node.~HtmlDel();
            break;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            note_node.~HtmlNote();
            break;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            em_node.~HtmlEm();
            break;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            strong_node.~HtmlStrong();
            break;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            ul_node.~HtmlUl();
            break;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            ol_node.~HtmlOl();
            break;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            li_node.~HtmlLi();
            break;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            code_node.~HtmlCode();
            break;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            pre_node.~HtmlPre();
            break;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            blockquote_node.~HtmlBlockquote();
            break;
        }
        case ::pltxt2htm::NodeKind::html_col: {
            col_node.~HtmlCol();
            break;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            table_node.~HtmlTable();
            break;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            caption_node.~HtmlCaption();
            break;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            colgroup_node.~HtmlColgroup();
            break;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            thead_node.~HtmlThead();
            break;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            tbody_node.~HtmlTbody();
            break;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            tfoot_node.~HtmlTfoot();
            break;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            tr_node.~HtmlTr();
            break;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            th_node.~HtmlTh();
            break;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            td_node.~HtmlTd();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            md_atx_h1_node.~MdAtxH1();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            md_atx_h2_node.~MdAtxH2();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            md_atx_h3_node.~MdAtxH3();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            md_atx_h4_node.~MdAtxH4();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            md_atx_h5_node.~MdAtxH5();
            break;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            md_atx_h6_node.~MdAtxH6();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            md_escape_backslash_node.~MdEscapeBackslash();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            md_escape_exclamation_node.~MdEscapeExclamation();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote: {
            md_escape_double_quote_node.~MdEscapeDoubleQuote();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            md_escape_hash_node.~MdEscapeHash();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            md_escape_dollar_node.~MdEscapeDollar();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            md_escape_percent_node.~MdEscapePercent();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand: {
            md_escape_ampersand_node.~MdEscapeAmpersand();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote: {
            md_escape_single_quote_node.~MdEscapeSingleQuote();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            md_escape_left_paren_node.~MdEscapeLeftParen();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            md_escape_right_paren_node.~MdEscapeRightParen();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            md_escape_asterisk_node.~MdEscapeAsterisk();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            md_escape_plus_node.~MdEscapePlus();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            md_escape_comma_node.~MdEscapeComma();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            md_escape_hyphen_node.~MdEscapeHyphen();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            md_escape_dot_node.~MdEscapeDot();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            md_escape_slash_node.~MdEscapeSlash();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            md_escape_colon_node.~MdEscapeColon();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            md_escape_semicolon_node.~MdEscapeSemicolon();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than: {
            md_escape_less_than_node.~MdEscapeLessThan();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            md_escape_equals_node.~MdEscapeEquals();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than: {
            md_escape_greater_than_node.~MdEscapeGreaterThan();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            md_escape_question_node.~MdEscapeQuestion();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            md_escape_at_node.~MdEscapeAt();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            md_escape_left_bracket_node.~MdEscapeLeftBracket();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            md_escape_right_bracket_node.~MdEscapeRightBracket();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            md_escape_caret_node.~MdEscapeCaret();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            md_escape_underscore_node.~MdEscapeUnderscore();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            md_escape_backtick_node.~MdEscapeBacktick();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            md_escape_left_brace_node.~MdEscapeLeftBrace();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            md_escape_pipe_node.~MdEscapePipe();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            md_escape_right_brace_node.~MdEscapeRightBrace();
            break;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            md_escape_tilde_node.~MdEscapeTilde();
            break;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            md_hr_node.~MdHr();
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            md_code_fence_backtick_node.~MdCodeFenceBacktick();
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            md_code_fence_tilde_node.~MdCodeFenceTilde();
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            md_code_span_1_backtick_node.~MdCodeSpan1Backtick();
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            md_code_span_2_backtick_node.~MdCodeSpan2Backtick();
            break;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            md_code_span_3_backtick_node.~MdCodeSpan3Backtick();
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            md_single_emphasis_asterisk_node.~MdSingleEmphasisAsterisk();
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            md_double_emphasis_asterisk_node.~MdDoubleEmphasisAsterisk();
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            md_triple_emphasis_asterisk_node.~MdTripleEmphasisAsterisk();
            break;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            md_single_emphasis_underscore_node.~MdSingleEmphasisUnderscore();
            break;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            md_double_emphasis_underscore_node.~MdDoubleEmphasisUnderscore();
            break;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            md_triple_emphasis_underscore_node.~MdTripleEmphasisUnderscore();
            break;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            md_del_node.~MdDel();
            break;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            md_link_node.~MdLink();
            break;
        }
        case ::pltxt2htm::NodeKind::url: {
            url_node.~Url();
            break;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            md_image_node.~MdImage();
            break;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            md_block_quotes_node.~MdBlockQuotes();
            break;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            md_ul_node.~MdUl();
            break;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            md_ol_node.~MdOl();
            break;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            md_li_node.~MdLi();
            break;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            md_li_checkbox_node.~MdLiCheckbox();
            break;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            md_table_node.~MdTable();
            break;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            md_thead_node.~MdThead();
            break;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            md_tbody_node.~MdTbody();
            break;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            md_tr_node.~MdTr();
            break;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            md_th_node.~MdTh();
            break;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            md_td_node.~MdTd();
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            md_latex_inline_node.~MdLatexInline();
            break;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            md_latex_block_node.~MdLatexBlock();
            break;
        }
// All enum values are covered above; default is unreachable.
// #if 0 documents intent and keeps the formatter output clean.
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable();
            }
#endif
        }
    }

    constexpr auto operator=(::pltxt2htm::PlTxtNode<ndebug> const&) noexcept = delete;

    constexpr auto operator=(this ::pltxt2htm::PlTxtNode<ndebug>& self, ::pltxt2htm::PlTxtNode<ndebug>&& other) noexcept
        -> PlTxtNode& {
        pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"self assignment is not allowed");
        self.~PlTxtNode();
        ::std::construct_at(::std::addressof(self), ::std::move(other));
        return self;
    }

    [[nodiscard]]
    constexpr auto operator==(this ::pltxt2htm::PlTxtNode<ndebug> const& self,
                              ::pltxt2htm::PlTxtNode<ndebug> const& other) noexcept -> bool {
        if (self.node_kind != other.node_kind) {
            return false;
        }
        switch (self.node_kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            return self.u8char_node == other.u8char_node;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            return self.invalid_u8char_node == other.invalid_u8char_node;
        }
        case ::pltxt2htm::NodeKind::text: {
            return self.text_node == other.text_node;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            return self.pl_color_node == other.pl_color_node;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            return self.pl_a_node == other.pl_a_node;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            return self.pl_experiment_node == other.pl_experiment_node;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            return self.pl_discussion_node == other.pl_discussion_node;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            return self.pl_user_node == other.pl_user_node;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            return self.pl_external_node == other.pl_external_node;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            return self.pl_size_node == other.pl_size_node;
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            return self.pl_i_node == other.pl_i_node;
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            return self.pl_b_node == other.pl_b_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            return self.pl_macro_project_node == other.pl_macro_project_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            return self.pl_macro_visitor_node == other.pl_macro_visitor_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            return self.pl_macro_author_node == other.pl_macro_author_node;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            return self.pl_macro_coauthors_node == other.pl_macro_coauthors_node;
        }
        case ::pltxt2htm::NodeKind::line_break: {
            return self.line_break_node == other.line_break_node;
        }
        case ::pltxt2htm::NodeKind::html_br: {
            return self.br_node == other.br_node;
        }
        case ::pltxt2htm::NodeKind::space: {
            return self.space_node == other.space_node;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            return self.less_than_node == other.less_than_node;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            return self.greater_than_node == other.greater_than_node;
        }
        case ::pltxt2htm::NodeKind::tab: {
            return self.tab_node == other.tab_node;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            return self.ampersand_node == other.ampersand_node;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            return self.single_quote_node == other.single_quote_node;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            return self.double_quote_node == other.double_quote_node;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            return self.hr_node == other.hr_node;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            return self.h1_node == other.h1_node;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            return self.h2_node == other.h2_node;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            return self.h3_node == other.h3_node;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            return self.h4_node == other.h4_node;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            return self.h5_node == other.h5_node;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            return self.h6_node == other.h6_node;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            return self.p_node == other.p_node;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            return self.del_node == other.del_node;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            return self.note_node == other.note_node;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            return self.em_node == other.em_node;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            return self.strong_node == other.strong_node;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            return self.ul_node == other.ul_node;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            return self.ol_node == other.ol_node;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            return self.li_node == other.li_node;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            return self.code_node == other.code_node;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            return self.pre_node == other.pre_node;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            return self.blockquote_node == other.blockquote_node;
        }
        case ::pltxt2htm::NodeKind::html_col: {
            return self.col_node == other.col_node;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            return self.table_node == other.table_node;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            return self.caption_node == other.caption_node;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            return self.colgroup_node == other.colgroup_node;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            return self.thead_node == other.thead_node;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            return self.tbody_node == other.tbody_node;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            return self.tfoot_node == other.tfoot_node;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            return self.tr_node == other.tr_node;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            return self.th_node == other.th_node;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            return self.td_node == other.td_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            return self.md_atx_h1_node == other.md_atx_h1_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            return self.md_atx_h2_node == other.md_atx_h2_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            return self.md_atx_h3_node == other.md_atx_h3_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            return self.md_atx_h4_node == other.md_atx_h4_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            return self.md_atx_h5_node == other.md_atx_h5_node;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            return self.md_atx_h6_node == other.md_atx_h6_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            return self.md_escape_backslash_node == other.md_escape_backslash_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            return self.md_escape_exclamation_node == other.md_escape_exclamation_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote: {
            return self.md_escape_double_quote_node == other.md_escape_double_quote_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            return self.md_escape_hash_node == other.md_escape_hash_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            return self.md_escape_dollar_node == other.md_escape_dollar_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            return self.md_escape_percent_node == other.md_escape_percent_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand: {
            return self.md_escape_ampersand_node == other.md_escape_ampersand_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote: {
            return self.md_escape_single_quote_node == other.md_escape_single_quote_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            return self.md_escape_left_paren_node == other.md_escape_left_paren_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            return self.md_escape_right_paren_node == other.md_escape_right_paren_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            return self.md_escape_asterisk_node == other.md_escape_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            return self.md_escape_plus_node == other.md_escape_plus_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            return self.md_escape_comma_node == other.md_escape_comma_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            return self.md_escape_hyphen_node == other.md_escape_hyphen_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            return self.md_escape_dot_node == other.md_escape_dot_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            return self.md_escape_slash_node == other.md_escape_slash_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            return self.md_escape_colon_node == other.md_escape_colon_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            return self.md_escape_semicolon_node == other.md_escape_semicolon_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than: {
            return self.md_escape_less_than_node == other.md_escape_less_than_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            return self.md_escape_equals_node == other.md_escape_equals_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than: {
            return self.md_escape_greater_than_node == other.md_escape_greater_than_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            return self.md_escape_question_node == other.md_escape_question_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            return self.md_escape_at_node == other.md_escape_at_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            return self.md_escape_left_bracket_node == other.md_escape_left_bracket_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            return self.md_escape_right_bracket_node == other.md_escape_right_bracket_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            return self.md_escape_caret_node == other.md_escape_caret_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            return self.md_escape_underscore_node == other.md_escape_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            return self.md_escape_backtick_node == other.md_escape_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            return self.md_escape_left_brace_node == other.md_escape_left_brace_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            return self.md_escape_pipe_node == other.md_escape_pipe_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            return self.md_escape_right_brace_node == other.md_escape_right_brace_node;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            return self.md_escape_tilde_node == other.md_escape_tilde_node;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            return self.md_hr_node == other.md_hr_node;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            return self.md_code_fence_backtick_node == other.md_code_fence_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            return self.md_code_fence_tilde_node == other.md_code_fence_tilde_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            return self.md_code_span_1_backtick_node == other.md_code_span_1_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            return self.md_code_span_2_backtick_node == other.md_code_span_2_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            return self.md_code_span_3_backtick_node == other.md_code_span_3_backtick_node;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            return self.md_single_emphasis_asterisk_node == other.md_single_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            return self.md_double_emphasis_asterisk_node == other.md_double_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            return self.md_triple_emphasis_asterisk_node == other.md_triple_emphasis_asterisk_node;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            return self.md_single_emphasis_underscore_node == other.md_single_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            return self.md_double_emphasis_underscore_node == other.md_double_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            return self.md_triple_emphasis_underscore_node == other.md_triple_emphasis_underscore_node;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            return self.md_del_node == other.md_del_node;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            return self.md_link_node == other.md_link_node;
        }
        case ::pltxt2htm::NodeKind::url: {
            return self.url_node == other.url_node;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            return self.md_image_node == other.md_image_node;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            return self.md_block_quotes_node == other.md_block_quotes_node;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            return self.md_ul_node == other.md_ul_node;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            return self.md_ol_node == other.md_ol_node;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            return self.md_li_node == other.md_li_node;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            return self.md_li_checkbox_node == other.md_li_checkbox_node;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            return self.md_table_node == other.md_table_node;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            return self.md_thead_node == other.md_thead_node;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            return self.md_tbody_node == other.md_tbody_node;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            return self.md_tr_node == other.md_tr_node;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            return self.md_th_node == other.md_th_node;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            return self.md_td_node == other.md_td_node;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            return self.md_latex_inline_node == other.md_latex_inline_node;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            return self.md_latex_block_node == other.md_latex_block_node;
        }
// All enum values are covered above; default is unreachable.
// #if 0 documents intent and keeps the formatter output clean.
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
        // reduce MSVC return-path warning
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    [[nodiscard]]
    constexpr auto get_node_kind(this auto&& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.node_kind;
    }

    [[nodiscard]]
    constexpr auto get_u8char(this auto&& self) noexcept -> char8_t {
        bool const is_u8char_type{self.node_kind == ::pltxt2htm::NodeKind::u8char};
        pltxt2htm_assert(is_u8char_type, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.u8char_node.chr);
    }

    [[nodiscard]]
    constexpr auto get_equal_sign_tag_id(this auto&& self) noexcept -> decltype(auto) {
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(self.node_kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"node kind mismatch");
        switch (self.node_kind) {
        case ::pltxt2htm::NodeKind::pl_color: {
            return ::std::forward_like<decltype(self)>(self.pl_color_node).get_color();
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            return ::std::forward_like<decltype(self)>(self.pl_experiment_node).get_id();
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            return ::std::forward_like<decltype(self)>(self.pl_discussion_node).get_id();
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            return ::std::forward_like<decltype(self)>(self.pl_user_node).get_id();
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    [[nodiscard]]
    constexpr auto get_external_tag_url(this auto&& self) noexcept -> decltype(auto) {
        bool const is_external_tag_type{self.node_kind == ::pltxt2htm::NodeKind::pl_external};
        pltxt2htm_assert(is_external_tag_type, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.pl_external_node).get_url();
    }

    [[nodiscard]]
    constexpr auto get_url_node(this auto&& self) noexcept -> decltype(auto) {
        bool const is_url_type{self.node_kind == ::pltxt2htm::NodeKind::url};
        pltxt2htm_assert(is_url_type, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.url_node);
    }

    [[nodiscard]]
    constexpr auto get_md_image_url(this auto&& self) noexcept -> decltype(auto) {
        bool const is_md_image_type{self.node_kind == ::pltxt2htm::NodeKind::md_image};
        pltxt2htm_assert(is_md_image_type, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.md_image_node).get_url();
    }

    [[nodiscard]]
    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        bool const is_pl_size_tag_type{self.node_kind == ::pltxt2htm::NodeKind::pl_size};
        pltxt2htm_assert(is_pl_size_tag_type, u8"node kind mismatch");
        return self.pl_size_node.get_size();
    }

    [[nodiscard]]
    constexpr auto get_md_link_url(this auto&& self) noexcept -> decltype(auto) {
        bool const is_md_link_type{self.node_kind == ::pltxt2htm::NodeKind::md_link};
        pltxt2htm_assert(is_md_link_type, u8"node kind mismatch");
        return ::std::forward_like<decltype(self)>(self.md_link_node).get_url();
    }

    [[nodiscard]]
    constexpr auto get_md_code_fence_language(this auto&& self) noexcept -> decltype(auto) {
        switch (self.node_kind) {
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            return ::std::forward_like<decltype(self)>(self.md_code_fence_backtick_node).get_language();
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            return ::std::forward_like<decltype(self)>(self.md_code_fence_tilde_node).get_language();
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    [[nodiscard]]
    constexpr auto get_md_table_cell_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        switch (self.node_kind) {
        case ::pltxt2htm::NodeKind::md_th: {
            return self.md_th_node.get_align();
        }
        case ::pltxt2htm::NodeKind::md_td: {
            return self.md_td_node.get_align();
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        switch (self.node_kind) {
        case ::pltxt2htm::NodeKind::text: {
            return ::std::forward_like<decltype(self)>(self.text_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            return ::std::forward_like<decltype(self)>(self.h1_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            return ::std::forward_like<decltype(self)>(self.h2_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            return ::std::forward_like<decltype(self)>(self.h3_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            return ::std::forward_like<decltype(self)>(self.h4_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            return ::std::forward_like<decltype(self)>(self.h5_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            return ::std::forward_like<decltype(self)>(self.h6_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_p: {
            return ::std::forward_like<decltype(self)>(self.p_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_del: {
            return ::std::forward_like<decltype(self)>(self.del_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_note: {
            return ::std::forward_like<decltype(self)>(self.note_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_em: {
            return ::std::forward_like<decltype(self)>(self.em_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            return ::std::forward_like<decltype(self)>(self.strong_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            return ::std::forward_like<decltype(self)>(self.ul_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            return ::std::forward_like<decltype(self)>(self.ol_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_li: {
            return ::std::forward_like<decltype(self)>(self.li_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_code: {
            return ::std::forward_like<decltype(self)>(self.code_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            return ::std::forward_like<decltype(self)>(self.pre_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            return ::std::forward_like<decltype(self)>(self.blockquote_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_table: {
            return ::std::forward_like<decltype(self)>(self.table_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            return ::std::forward_like<decltype(self)>(self.caption_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            return ::std::forward_like<decltype(self)>(self.colgroup_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            return ::std::forward_like<decltype(self)>(self.thead_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            return ::std::forward_like<decltype(self)>(self.tbody_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            return ::std::forward_like<decltype(self)>(self.tfoot_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            return ::std::forward_like<decltype(self)>(self.tr_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_th: {
            return ::std::forward_like<decltype(self)>(self.th_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::html_td: {
            return ::std::forward_like<decltype(self)>(self.td_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h1_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h2_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h3_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h4_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h5_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            return ::std::forward_like<decltype(self)>(self.md_atx_h6_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            return ::std::forward_like<decltype(self)>(self.md_code_fence_backtick_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            return ::std::forward_like<decltype(self)>(self.md_code_fence_tilde_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            return ::std::forward_like<decltype(self)>(self.md_code_span_1_backtick_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            return ::std::forward_like<decltype(self)>(self.md_code_span_2_backtick_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            return ::std::forward_like<decltype(self)>(self.md_code_span_3_backtick_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            return ::std::forward_like<decltype(self)>(self.md_single_emphasis_asterisk_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            return ::std::forward_like<decltype(self)>(self.md_double_emphasis_asterisk_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            return ::std::forward_like<decltype(self)>(self.md_triple_emphasis_asterisk_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            return ::std::forward_like<decltype(self)>(self.md_single_emphasis_underscore_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            return ::std::forward_like<decltype(self)>(self.md_double_emphasis_underscore_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            return ::std::forward_like<decltype(self)>(self.md_triple_emphasis_underscore_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_del: {
            return ::std::forward_like<decltype(self)>(self.md_del_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_link: {
            return ::std::forward_like<decltype(self)>(self.md_link_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::url: {
            return ::std::forward_like<decltype(self)>(self.url_node).get_url_ast();
        }
        case ::pltxt2htm::NodeKind::md_image: {
            return ::std::forward_like<decltype(self)>(self.md_image_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            return ::std::forward_like<decltype(self)>(self.md_block_quotes_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            return ::std::forward_like<decltype(self)>(self.md_ul_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            return ::std::forward_like<decltype(self)>(self.md_ol_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_li: {
            return ::std::forward_like<decltype(self)>(self.md_li_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            return ::std::forward_like<decltype(self)>(self.md_li_checkbox_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_table: {
            return ::std::forward_like<decltype(self)>(self.md_table_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            return ::std::forward_like<decltype(self)>(self.md_thead_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            return ::std::forward_like<decltype(self)>(self.md_tbody_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            return ::std::forward_like<decltype(self)>(self.md_tr_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_th: {
            return ::std::forward_like<decltype(self)>(self.md_th_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_td: {
            return ::std::forward_like<decltype(self)>(self.md_td_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            return ::std::forward_like<decltype(self)>(self.md_latex_inline_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            return ::std::forward_like<decltype(self)>(self.md_latex_block_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            return ::std::forward_like<decltype(self)>(self.pl_color_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            return ::std::forward_like<decltype(self)>(self.pl_a_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            return ::std::forward_like<decltype(self)>(self.pl_experiment_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            return ::std::forward_like<decltype(self)>(self.pl_discussion_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            return ::std::forward_like<decltype(self)>(self.pl_user_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            return ::std::forward_like<decltype(self)>(self.pl_external_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            return ::std::forward_like<decltype(self)>(self.pl_size_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            return ::std::forward_like<decltype(self)>(self.pl_i_node).get_subast();
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            return ::std::forward_like<decltype(self)>(self.pl_b_node).get_subast();
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto&& self) noexcept -> bool {
        bool const is_checkbox_type{self.node_kind == ::pltxt2htm::NodeKind::md_li_checkbox};
        pltxt2htm_assert(is_checkbox_type, u8"node kind mismatch");
        return self.md_li_checkbox_node.is_checked();
    };
};

} // namespace pltxt2htm

#include "../details/pop_macro.hh"
#include "impl/basic_node_def.inc"
#include "impl/html_node_def.inc"
#include "impl/markdown_node_def.inc"
#include "impl/physics_lab_node_def.inc"
