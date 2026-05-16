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
        ::pltxt2htm::LineBreak linebreak_node;
        ::pltxt2htm::Br br_node;
        ::pltxt2htm::Space space_node;
        ::pltxt2htm::LessThan lessthan_node;
        ::pltxt2htm::GreaterThan greaterthan_node;
        ::pltxt2htm::Tab tab_node;
        ::pltxt2htm::Ampersand ampersand_node;
        ::pltxt2htm::SingleQuotationMark singlequotationmark_node;
        ::pltxt2htm::DoubleQuotationMark doublequotationmark_node;
        ::pltxt2htm::Hr hr_node;
        ::pltxt2htm::H1<ndebug> h1_node;
        ::pltxt2htm::H2<ndebug> h2_node;
        ::pltxt2htm::H3<ndebug> h3_node;
        ::pltxt2htm::H4<ndebug> h4_node;
        ::pltxt2htm::H5<ndebug> h5_node;
        ::pltxt2htm::H6<ndebug> h6_node;
        ::pltxt2htm::P<ndebug> p_node;
        ::pltxt2htm::Del<ndebug> del_node;
        ::pltxt2htm::Note<ndebug> note_node;
        ::pltxt2htm::Em<ndebug> em_node;
        ::pltxt2htm::Strong<ndebug> strong_node;
        ::pltxt2htm::Ul<ndebug> ul_node;
        ::pltxt2htm::Ol<ndebug> ol_node;
        ::pltxt2htm::Li<ndebug> li_node;
        ::pltxt2htm::Code<ndebug> code_node;
        ::pltxt2htm::Pre<ndebug> pre_node;
        ::pltxt2htm::Blockquote<ndebug> blockquote_node;

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
        ::pltxt2htm::MdImage<ndebug> md_image_node;
        ::pltxt2htm::MdBlockQuotes<ndebug> md_block_quotes_node;
        ::pltxt2htm::MdUl<ndebug> md_ul_node;
        ::pltxt2htm::MdOl<ndebug> md_ol_node;
        ::pltxt2htm::MdLi<ndebug> md_li_node;
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
        : linebreak_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::line_break} {
    }

    constexpr PlTxtNode(::pltxt2htm::Br&& node) noexcept
        : br_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_br} {
    }

    constexpr PlTxtNode(::pltxt2htm::Space&& node) noexcept
        : space_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::space} {
    }

    constexpr PlTxtNode(::pltxt2htm::LessThan&& node) noexcept
        : lessthan_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::less_than} {
    }

    constexpr PlTxtNode(::pltxt2htm::GreaterThan&& node) noexcept
        : greaterthan_node(::std::move(node)),
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

    constexpr PlTxtNode(::pltxt2htm::SingleQuotationMark&& node) noexcept
        : singlequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::single_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::DoubleQuotationMark&& node) noexcept
        : doublequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::double_quote} {
    }

    constexpr PlTxtNode(::pltxt2htm::Hr&& node) noexcept
        : hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_hr} {
    }

    constexpr PlTxtNode(::pltxt2htm::H1<ndebug>&& node) noexcept
        : h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h1} {
    }

    constexpr PlTxtNode(::pltxt2htm::H2<ndebug>&& node) noexcept
        : h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h2} {
    }

    constexpr PlTxtNode(::pltxt2htm::H3<ndebug>&& node) noexcept
        : h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h3} {
    }

    constexpr PlTxtNode(::pltxt2htm::H4<ndebug>&& node) noexcept
        : h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h4} {
    }

    constexpr PlTxtNode(::pltxt2htm::H5<ndebug>&& node) noexcept
        : h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h5} {
    }

    constexpr PlTxtNode(::pltxt2htm::H6<ndebug>&& node) noexcept
        : h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_h6} {
    }

    constexpr PlTxtNode(::pltxt2htm::P<ndebug>&& node) noexcept
        : p_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_p} {
    }

    constexpr PlTxtNode(::pltxt2htm::Del<ndebug>&& node) noexcept
        : del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_del} {
    }

    constexpr PlTxtNode(::pltxt2htm::Note<ndebug>&& node) noexcept
        : note_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_note} {
    }

    constexpr PlTxtNode(::pltxt2htm::Em<ndebug>&& node) noexcept
        : em_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_em} {
    }

    constexpr PlTxtNode(::pltxt2htm::Strong<ndebug>&& node) noexcept
        : strong_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_strong} {
    }

    constexpr PlTxtNode(::pltxt2htm::Ul<ndebug>&& node) noexcept
        : ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_ul} {
    }

    constexpr PlTxtNode(::pltxt2htm::Ol<ndebug>&& node) noexcept
        : ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_ol} {
    }

    constexpr PlTxtNode(::pltxt2htm::Li<ndebug>&& node) noexcept
        : li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_li} {
    }

    constexpr PlTxtNode(::pltxt2htm::Code<ndebug>&& node) noexcept
        : code_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_code} {
    }

    constexpr PlTxtNode(::pltxt2htm::Pre<ndebug>&& node) noexcept
        : pre_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_pre} {
    }

    constexpr PlTxtNode(::pltxt2htm::Blockquote<ndebug>&& node) noexcept
        : blockquote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::html_blockquote} {
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

    constexpr PlTxtNode(::pltxt2htm::MdLatexInline<ndebug>&& node) noexcept
        : md_latex_inline_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_latex_inline} {
    }

    constexpr PlTxtNode(::pltxt2htm::MdLatexBlock<ndebug>&& node) noexcept
        : md_latex_block_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeKind::md_latex_block} {
    }

    constexpr PlTxtNode(::pltxt2htm::PlTxtNode<ndebug> const&) noexcept = delete;

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
            new (::std::addressof(linebreak_node))::pltxt2htm::LineBreak(::std::move(other.linebreak_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_br: {
            new (::std::addressof(br_node))::pltxt2htm::Br(::std::move(other.br_node));
            break;
        }

        case ::pltxt2htm::NodeKind::space: {
            new (::std::addressof(space_node))::pltxt2htm::Space(::std::move(other.space_node));
            break;
        }

        case ::pltxt2htm::NodeKind::less_than: {
            new (::std::addressof(lessthan_node))::pltxt2htm::LessThan(::std::move(other.lessthan_node));
            break;
        }

        case ::pltxt2htm::NodeKind::greater_than: {
            new (::std::addressof(greaterthan_node))::pltxt2htm::GreaterThan(::std::move(other.greaterthan_node));
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
            new (::std::addressof(singlequotationmark_node))::pltxt2htm::SingleQuotationMark(
                ::std::move(other.singlequotationmark_node));
            break;
        }

        case ::pltxt2htm::NodeKind::double_quote: {
            new (::std::addressof(doublequotationmark_node))::pltxt2htm::DoubleQuotationMark(
                ::std::move(other.doublequotationmark_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_hr: {
            new (::std::addressof(hr_node))::pltxt2htm::Hr(::std::move(other.hr_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h1: {
            new (::std::addressof(h1_node))::pltxt2htm::H1(::std::move(other.h1_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h2: {
            new (::std::addressof(h2_node))::pltxt2htm::H2(::std::move(other.h2_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h3: {
            new (::std::addressof(h3_node))::pltxt2htm::H3(::std::move(other.h3_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h4: {
            new (::std::addressof(h4_node))::pltxt2htm::H4(::std::move(other.h4_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h5: {
            new (::std::addressof(h5_node))::pltxt2htm::H5(::std::move(other.h5_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_h6: {
            new (::std::addressof(h6_node))::pltxt2htm::H6(::std::move(other.h6_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_p: {
            new (::std::addressof(p_node))::pltxt2htm::P(::std::move(other.p_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_del: {
            new (::std::addressof(del_node))::pltxt2htm::Del(::std::move(other.del_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_note: {
            new (::std::addressof(note_node))::pltxt2htm::Note(::std::move(other.note_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_em: {
            new (::std::addressof(em_node))::pltxt2htm::Em(::std::move(other.em_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_strong: {
            new (::std::addressof(strong_node))::pltxt2htm::Strong(::std::move(other.strong_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_ul: {
            new (::std::addressof(ul_node))::pltxt2htm::Ul(::std::move(other.ul_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_ol: {
            new (::std::addressof(ol_node))::pltxt2htm::Ol(::std::move(other.ol_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_li: {
            new (::std::addressof(li_node))::pltxt2htm::Li(::std::move(other.li_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_code: {
            new (::std::addressof(code_node))::pltxt2htm::Code(::std::move(other.code_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_pre: {
            new (::std::addressof(pre_node))::pltxt2htm::Pre(::std::move(other.pre_node));
            break;
        }

        case ::pltxt2htm::NodeKind::html_blockquote: {
            new (::std::addressof(blockquote_node))::pltxt2htm::Blockquote(::std::move(other.blockquote_node));
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
        case ::pltxt2htm::NodeKind::u8char:
            u8char_node.~U8Char();
            break;
        case ::pltxt2htm::NodeKind::invalid_u8char:
            invalid_u8char_node.~InvalidU8Char();
            break;
        case ::pltxt2htm::NodeKind::text:
            text_node.~Text();
            break;
        case ::pltxt2htm::NodeKind::pl_color:
            pl_color_node.~PlColor();
            break;
        case ::pltxt2htm::NodeKind::pl_a:
            pl_a_node.~PlA();
            break;
        case ::pltxt2htm::NodeKind::pl_experiment:
            pl_experiment_node.~PlExperiment();
            break;
        case ::pltxt2htm::NodeKind::pl_discussion:
            pl_discussion_node.~PlDiscussion();
            break;
        case ::pltxt2htm::NodeKind::pl_user:
            pl_user_node.~PlUser();
            break;
        case ::pltxt2htm::NodeKind::pl_external:
            pl_external_node.~PlExternal();
            break;
        case ::pltxt2htm::NodeKind::pl_size:
            pl_size_node.~PlSize();
            break;
        case ::pltxt2htm::NodeKind::pl_i:
            pl_i_node.~PlI();
            break;
        case ::pltxt2htm::NodeKind::pl_b:
            pl_b_node.~PlB();
            break;
        case ::pltxt2htm::NodeKind::pl_macro_project:
            pl_macro_project_node.~PlMacroProject();
            break;
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            pl_macro_visitor_node.~PlMacroVisitor();
            break;
        case ::pltxt2htm::NodeKind::pl_macro_author:
            pl_macro_author_node.~PlMacroAuthor();
            break;
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            pl_macro_coauthors_node.~PlMacroCoauthors();
            break;

        case ::pltxt2htm::NodeKind::line_break:
            linebreak_node.~LineBreak();
            break;

        case ::pltxt2htm::NodeKind::html_br:
            br_node.~Br();
            break;

        case ::pltxt2htm::NodeKind::space:
            space_node.~Space();
            break;

        case ::pltxt2htm::NodeKind::less_than:
            lessthan_node.~LessThan();
            break;

        case ::pltxt2htm::NodeKind::greater_than:
            greaterthan_node.~GreaterThan();
            break;

        case ::pltxt2htm::NodeKind::tab:
            tab_node.~Tab();
            break;

        case ::pltxt2htm::NodeKind::ampersand:
            ampersand_node.~Ampersand();
            break;

        case ::pltxt2htm::NodeKind::single_quote:
            singlequotationmark_node.~SingleQuotationMark();
            break;

        case ::pltxt2htm::NodeKind::double_quote:
            doublequotationmark_node.~DoubleQuotationMark();
            break;

        case ::pltxt2htm::NodeKind::html_hr:
            hr_node.~Hr();
            break;

        case ::pltxt2htm::NodeKind::html_h1:
            h1_node.~H1();
            break;

        case ::pltxt2htm::NodeKind::html_h2:
            h2_node.~H2();
            break;

        case ::pltxt2htm::NodeKind::html_h3:
            h3_node.~H3();
            break;

        case ::pltxt2htm::NodeKind::html_h4:
            h4_node.~H4();
            break;

        case ::pltxt2htm::NodeKind::html_h5:
            h5_node.~H5();
            break;

        case ::pltxt2htm::NodeKind::html_h6:
            h6_node.~H6();
            break;

        case ::pltxt2htm::NodeKind::html_p:
            p_node.~P();
            break;

        case ::pltxt2htm::NodeKind::html_del:
            del_node.~Del();
            break;

        case ::pltxt2htm::NodeKind::html_note:
            note_node.~Note();
            break;

        case ::pltxt2htm::NodeKind::html_em:
            em_node.~Em();
            break;

        case ::pltxt2htm::NodeKind::html_strong:
            strong_node.~Strong();
            break;

        case ::pltxt2htm::NodeKind::html_ul:
            ul_node.~Ul();
            break;

        case ::pltxt2htm::NodeKind::html_ol:
            ol_node.~Ol();
            break;

        case ::pltxt2htm::NodeKind::html_li:
            li_node.~Li();
            break;

        case ::pltxt2htm::NodeKind::html_code:
            code_node.~Code();
            break;

        case ::pltxt2htm::NodeKind::html_pre:
            pre_node.~Pre();
            break;

        case ::pltxt2htm::NodeKind::html_blockquote:
            blockquote_node.~Blockquote();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h1:
            md_atx_h1_node.~MdAtxH1();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h2:
            md_atx_h2_node.~MdAtxH2();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h3:
            md_atx_h3_node.~MdAtxH3();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h4:
            md_atx_h4_node.~MdAtxH4();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h5:
            md_atx_h5_node.~MdAtxH5();
            break;
        case ::pltxt2htm::NodeKind::md_atx_h6:
            md_atx_h6_node.~MdAtxH6();
            break;
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            md_escape_backslash_node.~MdEscapeBackslash();
            break;
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            md_escape_exclamation_node.~MdEscapeExclamation();
            break;
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            md_escape_double_quote_node.~MdEscapeDoubleQuote();
            break;
        case ::pltxt2htm::NodeKind::md_escape_hash:
            md_escape_hash_node.~MdEscapeHash();
            break;
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            md_escape_dollar_node.~MdEscapeDollar();
            break;
        case ::pltxt2htm::NodeKind::md_escape_percent:
            md_escape_percent_node.~MdEscapePercent();
            break;
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            md_escape_ampersand_node.~MdEscapeAmpersand();
            break;
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            md_escape_single_quote_node.~MdEscapeSingleQuote();
            break;
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            md_escape_left_paren_node.~MdEscapeLeftParen();
            break;
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            md_escape_right_paren_node.~MdEscapeRightParen();
            break;
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            md_escape_asterisk_node.~MdEscapeAsterisk();
            break;
        case ::pltxt2htm::NodeKind::md_escape_plus:
            md_escape_plus_node.~MdEscapePlus();
            break;
        case ::pltxt2htm::NodeKind::md_escape_comma:
            md_escape_comma_node.~MdEscapeComma();
            break;
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            md_escape_hyphen_node.~MdEscapeHyphen();
            break;
        case ::pltxt2htm::NodeKind::md_escape_dot:
            md_escape_dot_node.~MdEscapeDot();
            break;
        case ::pltxt2htm::NodeKind::md_escape_slash:
            md_escape_slash_node.~MdEscapeSlash();
            break;
        case ::pltxt2htm::NodeKind::md_escape_colon:
            md_escape_colon_node.~MdEscapeColon();
            break;
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            md_escape_semicolon_node.~MdEscapeSemicolon();
            break;
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            md_escape_less_than_node.~MdEscapeLessThan();
            break;
        case ::pltxt2htm::NodeKind::md_escape_equals:
            md_escape_equals_node.~MdEscapeEquals();
            break;
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            md_escape_greater_than_node.~MdEscapeGreaterThan();
            break;
        case ::pltxt2htm::NodeKind::md_escape_question:
            md_escape_question_node.~MdEscapeQuestion();
            break;
        case ::pltxt2htm::NodeKind::md_escape_at:
            md_escape_at_node.~MdEscapeAt();
            break;
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            md_escape_left_bracket_node.~MdEscapeLeftBracket();
            break;
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            md_escape_right_bracket_node.~MdEscapeRightBracket();
            break;
        case ::pltxt2htm::NodeKind::md_escape_caret:
            md_escape_caret_node.~MdEscapeCaret();
            break;
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            md_escape_underscore_node.~MdEscapeUnderscore();
            break;
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            md_escape_backtick_node.~MdEscapeBacktick();
            break;
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            md_escape_left_brace_node.~MdEscapeLeftBrace();
            break;
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            md_escape_pipe_node.~MdEscapePipe();
            break;
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            md_escape_right_brace_node.~MdEscapeRightBrace();
            break;
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            md_escape_tilde_node.~MdEscapeTilde();
            break;
        case ::pltxt2htm::NodeKind::md_hr:
            md_hr_node.~MdHr();
            break;
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            md_code_fence_backtick_node.~MdCodeFenceBacktick();
            break;
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            md_code_fence_tilde_node.~MdCodeFenceTilde();
            break;
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            md_code_span_1_backtick_node.~MdCodeSpan1Backtick();
            break;
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            md_code_span_2_backtick_node.~MdCodeSpan2Backtick();
            break;
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            md_code_span_3_backtick_node.~MdCodeSpan3Backtick();
            break;
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            md_single_emphasis_asterisk_node.~MdSingleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            md_double_emphasis_asterisk_node.~MdDoubleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            md_triple_emphasis_asterisk_node.~MdTripleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            md_single_emphasis_underscore_node.~MdSingleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            md_double_emphasis_underscore_node.~MdDoubleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            md_triple_emphasis_underscore_node.~MdTripleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeKind::md_del:
            md_del_node.~MdDel();
            break;
        case ::pltxt2htm::NodeKind::md_link:
            md_link_node.~MdLink();
            break;
        case ::pltxt2htm::NodeKind::md_image:
            md_image_node.~MdImage();
            break;
        case ::pltxt2htm::NodeKind::md_block_quotes:
            md_block_quotes_node.~MdBlockQuotes();
            break;
        case ::pltxt2htm::NodeKind::md_ul:
            md_ul_node.~MdUl();
            break;
        case ::pltxt2htm::NodeKind::md_ol:
            md_ol_node.~MdOl();
            break;
        case ::pltxt2htm::NodeKind::md_li:
            md_li_node.~MdLi();
            break;
        case ::pltxt2htm::NodeKind::md_latex_inline:
            md_latex_inline_node.~MdLatexInline();
            break;
        case ::pltxt2htm::NodeKind::md_latex_block:
            md_latex_block_node.~MdLatexBlock();
            break;
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
        case ::pltxt2htm::NodeKind::pl_color:
            return ::std::forward_like<decltype(self)>(self.pl_color_node).get_color();
        case ::pltxt2htm::NodeKind::pl_experiment:
            return ::std::forward_like<decltype(self)>(self.pl_experiment_node).get_id();
        case ::pltxt2htm::NodeKind::pl_discussion:
            return ::std::forward_like<decltype(self)>(self.pl_discussion_node).get_id();
        case ::pltxt2htm::NodeKind::pl_user:
            return ::std::forward_like<decltype(self)>(self.pl_user_node).get_id();
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
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            return ::std::forward_like<decltype(self)>(self.md_code_fence_backtick_node).get_language();
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            return ::std::forward_like<decltype(self)>(self.md_code_fence_tilde_node).get_language();
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        switch (self.node_kind) {
        case ::pltxt2htm::NodeKind::text:
            return ::std::forward_like<decltype(self)>(self.text_node).get_subast();

        case ::pltxt2htm::NodeKind::html_h1:
            return ::std::forward_like<decltype(self)>(self.h1_node).get_subast();
        case ::pltxt2htm::NodeKind::html_h2:
            return ::std::forward_like<decltype(self)>(self.h2_node).get_subast();
        case ::pltxt2htm::NodeKind::html_h3:
            return ::std::forward_like<decltype(self)>(self.h3_node).get_subast();
        case ::pltxt2htm::NodeKind::html_h4:
            return ::std::forward_like<decltype(self)>(self.h4_node).get_subast();
        case ::pltxt2htm::NodeKind::html_h5:
            return ::std::forward_like<decltype(self)>(self.h5_node).get_subast();
        case ::pltxt2htm::NodeKind::html_h6:
            return ::std::forward_like<decltype(self)>(self.h6_node).get_subast();
        case ::pltxt2htm::NodeKind::html_p:
            return ::std::forward_like<decltype(self)>(self.p_node).get_subast();
        case ::pltxt2htm::NodeKind::html_del:
            return ::std::forward_like<decltype(self)>(self.del_node).get_subast();
        case ::pltxt2htm::NodeKind::html_note:
            return ::std::forward_like<decltype(self)>(self.note_node).get_subast();
        case ::pltxt2htm::NodeKind::html_em:
            return ::std::forward_like<decltype(self)>(self.em_node).get_subast();
        case ::pltxt2htm::NodeKind::html_strong:
            return ::std::forward_like<decltype(self)>(self.strong_node).get_subast();
        case ::pltxt2htm::NodeKind::html_ul:
            return ::std::forward_like<decltype(self)>(self.ul_node).get_subast();
        case ::pltxt2htm::NodeKind::html_ol:
            return ::std::forward_like<decltype(self)>(self.ol_node).get_subast();
        case ::pltxt2htm::NodeKind::html_li:
            return ::std::forward_like<decltype(self)>(self.li_node).get_subast();
        case ::pltxt2htm::NodeKind::html_code:
            return ::std::forward_like<decltype(self)>(self.code_node).get_subast();
        case ::pltxt2htm::NodeKind::html_pre:
            return ::std::forward_like<decltype(self)>(self.pre_node).get_subast();
        case ::pltxt2htm::NodeKind::html_blockquote:
            return ::std::forward_like<decltype(self)>(self.blockquote_node).get_subast();

        case ::pltxt2htm::NodeKind::md_atx_h1:
            return ::std::forward_like<decltype(self)>(self.md_atx_h1_node).get_subast();
        case ::pltxt2htm::NodeKind::md_atx_h2:
            return ::std::forward_like<decltype(self)>(self.md_atx_h2_node).get_subast();
        case ::pltxt2htm::NodeKind::md_atx_h3:
            return ::std::forward_like<decltype(self)>(self.md_atx_h3_node).get_subast();
        case ::pltxt2htm::NodeKind::md_atx_h4:
            return ::std::forward_like<decltype(self)>(self.md_atx_h4_node).get_subast();
        case ::pltxt2htm::NodeKind::md_atx_h5:
            return ::std::forward_like<decltype(self)>(self.md_atx_h5_node).get_subast();
        case ::pltxt2htm::NodeKind::md_atx_h6:
            return ::std::forward_like<decltype(self)>(self.md_atx_h6_node).get_subast();
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            return ::std::forward_like<decltype(self)>(self.md_code_fence_backtick_node).get_subast();
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            return ::std::forward_like<decltype(self)>(self.md_code_fence_tilde_node).get_subast();
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            return ::std::forward_like<decltype(self)>(self.md_code_span_1_backtick_node).get_subast();
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            return ::std::forward_like<decltype(self)>(self.md_code_span_2_backtick_node).get_subast();
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            return ::std::forward_like<decltype(self)>(self.md_code_span_3_backtick_node).get_subast();
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            return ::std::forward_like<decltype(self)>(self.md_single_emphasis_asterisk_node).get_subast();
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            return ::std::forward_like<decltype(self)>(self.md_double_emphasis_asterisk_node).get_subast();
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            return ::std::forward_like<decltype(self)>(self.md_triple_emphasis_asterisk_node).get_subast();
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            return ::std::forward_like<decltype(self)>(self.md_single_emphasis_underscore_node).get_subast();
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            return ::std::forward_like<decltype(self)>(self.md_double_emphasis_underscore_node).get_subast();
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            return ::std::forward_like<decltype(self)>(self.md_triple_emphasis_underscore_node).get_subast();
        case ::pltxt2htm::NodeKind::md_del:
            return ::std::forward_like<decltype(self)>(self.md_del_node).get_subast();
        case ::pltxt2htm::NodeKind::md_link:
            return ::std::forward_like<decltype(self)>(self.md_link_node).get_subast();
        case ::pltxt2htm::NodeKind::md_image:
            return ::std::forward_like<decltype(self)>(self.md_image_node).get_subast();
        case ::pltxt2htm::NodeKind::md_block_quotes:
            return ::std::forward_like<decltype(self)>(self.md_block_quotes_node).get_subast();
        case ::pltxt2htm::NodeKind::md_ul:
            return ::std::forward_like<decltype(self)>(self.md_ul_node).get_subast();
        case ::pltxt2htm::NodeKind::md_ol:
            return ::std::forward_like<decltype(self)>(self.md_ol_node).get_subast();
        case ::pltxt2htm::NodeKind::md_li:
            return ::std::forward_like<decltype(self)>(self.md_li_node).get_subast();
        case ::pltxt2htm::NodeKind::md_latex_inline:
            return ::std::forward_like<decltype(self)>(self.md_latex_inline_node).get_subast();
        case ::pltxt2htm::NodeKind::md_latex_block:
            return ::std::forward_like<decltype(self)>(self.md_latex_block_node).get_subast();

        case ::pltxt2htm::NodeKind::pl_color:
            return ::std::forward_like<decltype(self)>(self.pl_color_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_a:
            return ::std::forward_like<decltype(self)>(self.pl_a_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_experiment:
            return ::std::forward_like<decltype(self)>(self.pl_experiment_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_discussion:
            return ::std::forward_like<decltype(self)>(self.pl_discussion_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_user:
            return ::std::forward_like<decltype(self)>(self.pl_user_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_external:
            return ::std::forward_like<decltype(self)>(self.pl_external_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_size:
            return ::std::forward_like<decltype(self)>(self.pl_size_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_i:
            return ::std::forward_like<decltype(self)>(self.pl_i_node).get_subast();
        case ::pltxt2htm::NodeKind::pl_b:
            return ::std::forward_like<decltype(self)>(self.pl_b_node).get_subast();

        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
};

} // namespace pltxt2htm

#include "../details/pop_macro.hh"
#include "impl/basic_node_def.inc"
#include "impl/html_node_def.inc"
#include "impl/markdown_node_def.inc"
#include "impl/physics_lab_node_def.inc"
