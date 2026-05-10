#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <exception/exception.hh>
#include <memory>

#include "../astnode/node_type.hh"

#include "impl/basic_node_decl.hh"
#include "impl/html_node_decl.hh"
#include "impl/markdown_node_decl.hh"
#include "impl/physics_lab_node_decl.hh"

namespace pltxt2htm::ast2 {

namespace details {

class AstVariant {
public:
    union {
        // basic node
        ::pltxt2htm::ast2::U8Char u8char_node;
        ::pltxt2htm::ast2::InvalidU8Char invalid_u8char_node;
        ::pltxt2htm::ast2::Text text_node;

        // html node
        ::pltxt2htm::ast2::LineBreak linebreak_node;
        ::pltxt2htm::ast2::Br br_node;
        ::pltxt2htm::ast2::Space space_node;
        ::pltxt2htm::ast2::LessThan lessthan_node;
        ::pltxt2htm::ast2::GreaterThan greaterthan_node;
        ::pltxt2htm::ast2::Tab tab_node;
        ::pltxt2htm::ast2::Ampersand ampersand_node;
        ::pltxt2htm::ast2::SingleQuotationMark singlequotationmark_node;
        ::pltxt2htm::ast2::DoubleQuotationMark doublequotationmark_node;
        ::pltxt2htm::ast2::Hr hr_node;
        ::pltxt2htm::ast2::H1 h1_node;
        ::pltxt2htm::ast2::H2 h2_node;
        ::pltxt2htm::ast2::H3 h3_node;
        ::pltxt2htm::ast2::H4 h4_node;
        ::pltxt2htm::ast2::H5 h5_node;
        ::pltxt2htm::ast2::H6 h6_node;
        ::pltxt2htm::ast2::P p_node;
        ::pltxt2htm::ast2::Del del_node;
        ::pltxt2htm::ast2::Note note_node;
        ::pltxt2htm::ast2::Em em_node;
        ::pltxt2htm::ast2::Strong strong_node;
        ::pltxt2htm::ast2::Ul ul_node;
        ::pltxt2htm::ast2::Ol ol_node;
        ::pltxt2htm::ast2::Li li_node;
        ::pltxt2htm::ast2::Code code_node;
        ::pltxt2htm::ast2::Pre pre_node;
        ::pltxt2htm::ast2::Blockquote blockquote_node;

        // markdown node
        ::pltxt2htm::ast2::MdAtxH1 md_atx_h1_node;
        ::pltxt2htm::ast2::MdAtxH2 md_atx_h2_node;
        ::pltxt2htm::ast2::MdAtxH3 md_atx_h3_node;
        ::pltxt2htm::ast2::MdAtxH4 md_atx_h4_node;
        ::pltxt2htm::ast2::MdAtxH5 md_atx_h5_node;
        ::pltxt2htm::ast2::MdAtxH6 md_atx_h6_node;
        ::pltxt2htm::ast2::MdEscapeBackslash md_escape_backslash_node;
        ::pltxt2htm::ast2::MdEscapeExclamation md_escape_exclamation_node;
        ::pltxt2htm::ast2::MdEscapeDoubleQuote md_escape_double_quote_node;
        ::pltxt2htm::ast2::MdEscapeHash md_escape_hash_node;
        ::pltxt2htm::ast2::MdEscapeDollar md_escape_dollar_node;
        ::pltxt2htm::ast2::MdEscapePercent md_escape_percent_node;
        ::pltxt2htm::ast2::MdEscapeAmpersand md_escape_ampersand_node;
        ::pltxt2htm::ast2::MdEscapeSingleQuote md_escape_single_quote_node;
        ::pltxt2htm::ast2::MdEscapeLeftParen md_escape_left_paren_node;
        ::pltxt2htm::ast2::MdEscapeRightParen md_escape_right_paren_node;
        ::pltxt2htm::ast2::MdEscapeAsterisk md_escape_asterisk_node;
        ::pltxt2htm::ast2::MdEscapePlus md_escape_plus_node;
        ::pltxt2htm::ast2::MdEscapeComma md_escape_comma_node;
        ::pltxt2htm::ast2::MdEscapeHyphen md_escape_hyphen_node;
        ::pltxt2htm::ast2::MdEscapeDot md_escape_dot_node;
        ::pltxt2htm::ast2::MdEscapeSlash md_escape_slash_node;
        ::pltxt2htm::ast2::MdEscapeColon md_escape_colon_node;
        ::pltxt2htm::ast2::MdEscapeSemicolon md_escape_semicolon_node;
        ::pltxt2htm::ast2::MdEscapeLessThan md_escape_less_than_node;
        ::pltxt2htm::ast2::MdEscapeEquals md_escape_equals_node;
        ::pltxt2htm::ast2::MdEscapeGreaterThan md_escape_greater_than_node;
        ::pltxt2htm::ast2::MdEscapeQuestion md_escape_question_node;
        ::pltxt2htm::ast2::MdEscapeAt md_escape_at_node;
        ::pltxt2htm::ast2::MdEscapeLeftBracket md_escape_left_bracket_node;
        ::pltxt2htm::ast2::MdEscapeRightBracket md_escape_right_bracket_node;
        ::pltxt2htm::ast2::MdEscapeLeftBrace md_escape_left_brace_node;
        ::pltxt2htm::ast2::MdEscapePipe md_escape_pipe_node;
        ::pltxt2htm::ast2::MdEscapeRightBrace md_escape_right_brace_node;
        ::pltxt2htm::ast2::MdEscapeTilde md_escape_tilde_node;
        ::pltxt2htm::ast2::MdHr md_hr_node;
        ::pltxt2htm::ast2::MdCodeFenceBacktick md_code_fence_backtick_node;
        ::pltxt2htm::ast2::MdCodeFenceTilde md_code_fence_tilde_node;
        ::pltxt2htm::ast2::MdCodeSpan1Backtick md_code_span_1_backtick_node;
        ::pltxt2htm::ast2::MdCodeSpan2Backtick md_code_span_2_backtick_node;
        ::pltxt2htm::ast2::MdCodeSpan3Backtick md_code_span_3_backtick_node;
        ::pltxt2htm::ast2::MdSingleEmphasisAsterisk md_single_emphasis_asterisk_node;
        ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk md_double_emphasis_asterisk_node;
        ::pltxt2htm::ast2::MdTripleEmphasisAsterisk md_triple_emphasis_asterisk_node;
        ::pltxt2htm::ast2::MdSingleEmphasisUnderscore md_single_emphasis_underscore_node;
        ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore md_double_emphasis_underscore_node;
        ::pltxt2htm::ast2::MdTripleEmphasisUnderscore md_triple_emphasis_underscore_node;
        ::pltxt2htm::ast2::MdDel md_del_node;
        ::pltxt2htm::ast2::MdLink md_link_node;
        ::pltxt2htm::ast2::MdImage md_image_node;
        ::pltxt2htm::ast2::MdBlockQuotes md_block_quotes_node;
        ::pltxt2htm::ast2::MdUl md_ul_node;
        ::pltxt2htm::ast2::MdOl md_ol_node;
        ::pltxt2htm::ast2::MdLi md_li_node;
        ::pltxt2htm::ast2::MdLatexInline md_latex_inline_node;
        ::pltxt2htm::ast2::MdLatexBlock md_latex_block_node;

        // physics lab node
        ::pltxt2htm::ast2::PlColor pl_color_node;
        ::pltxt2htm::ast2::PlA pl_a_node;
        ::pltxt2htm::ast2::PlExperiment pl_experiment_node;
        ::pltxt2htm::ast2::PlDiscussion pl_discussion_node;
        ::pltxt2htm::ast2::PlUser pl_user_node;
        ::pltxt2htm::ast2::PlExternal pl_external_node;
        ::pltxt2htm::ast2::PlSize pl_size_node;
        ::pltxt2htm::ast2::PlI pl_i_node;
        ::pltxt2htm::ast2::PlB pl_b_node;
        ::pltxt2htm::ast2::PlMacroProject pl_macro_project_node;
        ::pltxt2htm::ast2::PlMacroVisitor pl_macro_visitor_node;
        ::pltxt2htm::ast2::PlMacroAuthor pl_macro_author_node;
        ::pltxt2htm::ast2::PlMacroCoauthors pl_macro_coauthors_node;
    };

    ::pltxt2htm::NodeType node_kind;

    constexpr AstVariant(::pltxt2htm::ast2::U8Char&& node)
        : u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::u8char} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::InvalidU8Char&& node)
        : invalid_u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::invalid_u8char} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Text&& node)
        : text_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::text} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlColor&& node)
        : pl_color_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_color} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlA&& node)
        : pl_a_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_a} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlExperiment&& node)
        : pl_experiment_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_experiment} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlDiscussion&& node)
        : pl_discussion_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_discussion} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlUser&& node)
        : pl_user_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_user} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlExternal&& node)
        : pl_external_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_external} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlSize&& node)
        : pl_size_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_size} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlI&& node)
        : pl_i_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_i} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlB&& node)
        : pl_b_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_b} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroProject&& node)
        : pl_macro_project_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_project} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroVisitor&& node)
        : pl_macro_visitor_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_visitor} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroAuthor&& node)
        : pl_macro_author_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_author} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroCoauthors&& node)
        : pl_macro_coauthors_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_coauthors} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::LineBreak&& node)
        : linebreak_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::line_break} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Br&& node)
        : br_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_br} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Space&& node)
        : space_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::space} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::LessThan&& node)
        : lessthan_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::less_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::GreaterThan&& node)
        : greaterthan_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::greater_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Tab&& node)
        : tab_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::tab} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ampersand&& node)
        : ampersand_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::ampersand} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::SingleQuotationMark&& node)
        : singlequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::single_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::DoubleQuotationMark&& node)
        : doublequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::double_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Hr&& node)
        : hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_hr} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H1&& node)
        : h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h1} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H2&& node)
        : h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h2} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H3&& node)
        : h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h3} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H4&& node)
        : h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h4} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H5&& node)
        : h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h5} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H6&& node)
        : h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h6} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::P&& node)
        : p_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_p} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Del&& node)
        : del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_del} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Note&& node)
        : note_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_note} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Em&& node)
        : em_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_em} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Strong&& node)
        : strong_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_strong} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ul&& node)
        : ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_ul} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ol&& node)
        : ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_ol} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Li&& node)
        : li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_li} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Code&& node)
        : code_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_code} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Pre&& node)
        : pre_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_pre} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Blockquote&& node)
        : blockquote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_blockquote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant const&) noexcept = delete;

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH1&& node)
        : md_atx_h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h1} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH2&& node)
        : md_atx_h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h2} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH3&& node)
        : md_atx_h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h3} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH4&& node)
        : md_atx_h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h4} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH5&& node)
        : md_atx_h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h5} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdAtxH6&& node)
        : md_atx_h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_atx_h6} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeBackslash&& node)
        : md_escape_backslash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_backslash} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeExclamation&& node)
        : md_escape_exclamation_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_exclamation} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeDoubleQuote&& node)
        : md_escape_double_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_double_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeHash&& node)
        : md_escape_hash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_hash} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeDollar&& node)
        : md_escape_dollar_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_dollar} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapePercent&& node)
        : md_escape_percent_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_percent} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeAmpersand&& node)
        : md_escape_ampersand_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_ampersand} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeSingleQuote&& node)
        : md_escape_single_quote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_single_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeLeftParen&& node)
        : md_escape_left_paren_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_left_paren} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeRightParen&& node)
        : md_escape_right_paren_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_right_paren} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeAsterisk&& node)
        : md_escape_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_asterisk} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapePlus&& node)
        : md_escape_plus_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_plus} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeComma&& node)
        : md_escape_comma_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_comma} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeHyphen&& node)
        : md_escape_hyphen_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_hyphen} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeDot&& node)
        : md_escape_dot_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_dot} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeSlash&& node)
        : md_escape_slash_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_slash} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeColon&& node)
        : md_escape_colon_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_colon} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeSemicolon&& node)
        : md_escape_semicolon_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_semicolon} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeLessThan&& node)
        : md_escape_less_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_less_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeEquals&& node)
        : md_escape_equals_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_equals} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeGreaterThan&& node)
        : md_escape_greater_than_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_greater_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeQuestion&& node)
        : md_escape_question_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_question} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeAt&& node)
        : md_escape_at_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_at} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeLeftBracket&& node)
        : md_escape_left_bracket_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_left_bracket} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeRightBracket&& node)
        : md_escape_right_bracket_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_right_bracket} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeLeftBrace&& node)
        : md_escape_left_brace_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_left_brace} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapePipe&& node)
        : md_escape_pipe_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_pipe} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeRightBrace&& node)
        : md_escape_right_brace_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_right_brace} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdEscapeTilde&& node)
        : md_escape_tilde_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_escape_tilde} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdHr&& node)
        : md_hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_hr} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdCodeFenceBacktick&& node)
        : md_code_fence_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_code_fence_backtick} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdCodeFenceTilde&& node)
        : md_code_fence_tilde_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_code_fence_tilde} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdCodeSpan1Backtick&& node)
        : md_code_span_1_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_code_span_1_backtick} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdCodeSpan2Backtick&& node)
        : md_code_span_2_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_code_span_2_backtick} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdCodeSpan3Backtick&& node)
        : md_code_span_3_backtick_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_code_span_3_backtick} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdSingleEmphasisAsterisk&& node)
        : md_single_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_single_emphasis_asterisk} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdDoubleEmphasisAsterisk&& node)
        : md_double_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_double_emphasis_asterisk} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdTripleEmphasisAsterisk&& node)
        : md_triple_emphasis_asterisk_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_triple_emphasis_asterisk} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdSingleEmphasisUnderscore&& node)
        : md_single_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_single_emphasis_underscore} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdDoubleEmphasisUnderscore&& node)
        : md_double_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_double_emphasis_underscore} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdTripleEmphasisUnderscore&& node)
        : md_triple_emphasis_underscore_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_triple_emphasis_underscore} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdDel&& node)
        : md_del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_del} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdLink&& node)
        : md_link_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_link} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdImage&& node)
        : md_image_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_image} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdBlockQuotes&& node)
        : md_block_quotes_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_block_quotes} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdUl&& node)
        : md_ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_ul} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdOl&& node)
        : md_ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_ol} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdLi&& node)
        : md_li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_li} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdLatexInline&& node)
        : md_latex_inline_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_latex_inline} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::MdLatexBlock&& node)
        : md_latex_block_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::md_latex_block} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant&& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) {
        case ::pltxt2htm::NodeType::u8char:
            new (::std::addressof(u8char_node))::pltxt2htm::ast2::U8Char(::std::move(other.u8char_node));
            break;
        case ::pltxt2htm::NodeType::invalid_u8char:
            new (::std::addressof(invalid_u8char_node))::pltxt2htm::ast2::InvalidU8Char(
                ::std::move(other.invalid_u8char_node));
            break;
        case ::pltxt2htm::NodeType::text:
            new (::std::addressof(text_node))::pltxt2htm::ast2::Text(::std::move(other.text_node));
            break;
        case ::pltxt2htm::NodeType::pl_color:
            new (::std::addressof(pl_color_node))::pltxt2htm::ast2::PlColor(::std::move(other.pl_color_node));
            break;
        case ::pltxt2htm::NodeType::pl_a:
            new (::std::addressof(pl_a_node))::pltxt2htm::ast2::PlA(::std::move(other.pl_a_node));
            break;
        case ::pltxt2htm::NodeType::pl_experiment:
            new (::std::addressof(pl_experiment_node))::pltxt2htm::ast2::PlExperiment(
                ::std::move(other.pl_experiment_node));
            break;
        case ::pltxt2htm::NodeType::pl_discussion:
            new (::std::addressof(pl_discussion_node))::pltxt2htm::ast2::PlDiscussion(
                ::std::move(other.pl_discussion_node));
            break;
        case ::pltxt2htm::NodeType::pl_user:
            new (::std::addressof(pl_user_node))::pltxt2htm::ast2::PlUser(::std::move(other.pl_user_node));
            break;
        case ::pltxt2htm::NodeType::pl_external:
            new (::std::addressof(pl_external_node))::pltxt2htm::ast2::PlExternal(::std::move(other.pl_external_node));
            break;
        case ::pltxt2htm::NodeType::pl_size:
            new (::std::addressof(pl_size_node))::pltxt2htm::ast2::PlSize(::std::move(other.pl_size_node));
            break;
        case ::pltxt2htm::NodeType::pl_i:
            new (::std::addressof(pl_i_node))::pltxt2htm::ast2::PlI(::std::move(other.pl_i_node));
            break;
        case ::pltxt2htm::NodeType::pl_b:
            new (::std::addressof(pl_b_node))::pltxt2htm::ast2::PlB(::std::move(other.pl_b_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_project:
            new (::std::addressof(pl_macro_project_node))::pltxt2htm::ast2::PlMacroProject(
                ::std::move(other.pl_macro_project_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            new (::std::addressof(pl_macro_visitor_node))::pltxt2htm::ast2::PlMacroVisitor(
                ::std::move(other.pl_macro_visitor_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_author:
            new (::std::addressof(pl_macro_author_node))::pltxt2htm::ast2::PlMacroAuthor(
                ::std::move(other.pl_macro_author_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            new (::std::addressof(pl_macro_coauthors_node))::pltxt2htm::ast2::PlMacroCoauthors(
                ::std::move(other.pl_macro_coauthors_node));
            break;

        case ::pltxt2htm::NodeType::line_break:
            new (::std::addressof(linebreak_node))::pltxt2htm::ast2::LineBreak(::std::move(other.linebreak_node));
            break;

        case ::pltxt2htm::NodeType::html_br:
            new (::std::addressof(br_node))::pltxt2htm::ast2::Br(::std::move(other.br_node));
            break;

        case ::pltxt2htm::NodeType::space:
            new (::std::addressof(space_node))::pltxt2htm::ast2::Space(::std::move(other.space_node));
            break;

        case ::pltxt2htm::NodeType::less_than:
            new (::std::addressof(lessthan_node))::pltxt2htm::ast2::LessThan(::std::move(other.lessthan_node));
            break;

        case ::pltxt2htm::NodeType::greater_than:
            new (::std::addressof(greaterthan_node))::pltxt2htm::ast2::GreaterThan(::std::move(other.greaterthan_node));
            break;

        case ::pltxt2htm::NodeType::tab:
            new (::std::addressof(tab_node))::pltxt2htm::ast2::Tab(::std::move(other.tab_node));
            break;

        case ::pltxt2htm::NodeType::ampersand:
            new (::std::addressof(ampersand_node))::pltxt2htm::ast2::Ampersand(::std::move(other.ampersand_node));
            break;

        case ::pltxt2htm::NodeType::single_quote:
            new (::std::addressof(singlequotationmark_node))::pltxt2htm::ast2::SingleQuotationMark(
                ::std::move(other.singlequotationmark_node));
            break;

        case ::pltxt2htm::NodeType::double_quote:
            new (::std::addressof(doublequotationmark_node))::pltxt2htm::ast2::DoubleQuotationMark(
                ::std::move(other.doublequotationmark_node));
            break;

        case ::pltxt2htm::NodeType::html_hr:
            new (::std::addressof(hr_node))::pltxt2htm::ast2::Hr(::std::move(other.hr_node));
            break;

        case ::pltxt2htm::NodeType::html_h1:
            new (::std::addressof(h1_node))::pltxt2htm::ast2::H1(::std::move(other.h1_node));
            break;

        case ::pltxt2htm::NodeType::html_h2:
            new (::std::addressof(h2_node))::pltxt2htm::ast2::H2(::std::move(other.h2_node));
            break;

        case ::pltxt2htm::NodeType::html_h3:
            new (::std::addressof(h3_node))::pltxt2htm::ast2::H3(::std::move(other.h3_node));
            break;

        case ::pltxt2htm::NodeType::html_h4:
            new (::std::addressof(h4_node))::pltxt2htm::ast2::H4(::std::move(other.h4_node));
            break;

        case ::pltxt2htm::NodeType::html_h5:
            new (::std::addressof(h5_node))::pltxt2htm::ast2::H5(::std::move(other.h5_node));
            break;

        case ::pltxt2htm::NodeType::html_h6:
            new (::std::addressof(h6_node))::pltxt2htm::ast2::H6(::std::move(other.h6_node));
            break;

        case ::pltxt2htm::NodeType::html_p:
            new (::std::addressof(p_node))::pltxt2htm::ast2::P(::std::move(other.p_node));
            break;

        case ::pltxt2htm::NodeType::html_del:
            new (::std::addressof(del_node))::pltxt2htm::ast2::Del(::std::move(other.del_node));
            break;

        case ::pltxt2htm::NodeType::html_note:
            new (::std::addressof(note_node))::pltxt2htm::ast2::Note(::std::move(other.note_node));
            break;

        case ::pltxt2htm::NodeType::html_em:
            new (::std::addressof(em_node))::pltxt2htm::ast2::Em(::std::move(other.em_node));
            break;

        case ::pltxt2htm::NodeType::html_strong:
            new (::std::addressof(strong_node))::pltxt2htm::ast2::Strong(::std::move(other.strong_node));
            break;

        case ::pltxt2htm::NodeType::html_ul:
            new (::std::addressof(ul_node))::pltxt2htm::ast2::Ul(::std::move(other.ul_node));
            break;

        case ::pltxt2htm::NodeType::html_ol:
            new (::std::addressof(ol_node))::pltxt2htm::ast2::Ol(::std::move(other.ol_node));
            break;

        case ::pltxt2htm::NodeType::html_li:
            new (::std::addressof(li_node))::pltxt2htm::ast2::Li(::std::move(other.li_node));
            break;

        case ::pltxt2htm::NodeType::html_code:
            new (::std::addressof(code_node))::pltxt2htm::ast2::Code(::std::move(other.code_node));
            break;

        case ::pltxt2htm::NodeType::html_pre:
            new (::std::addressof(pre_node))::pltxt2htm::ast2::Pre(::std::move(other.pre_node));
            break;

        case ::pltxt2htm::NodeType::html_blockquote:
            new (::std::addressof(blockquote_node))::pltxt2htm::ast2::Blockquote(::std::move(other.blockquote_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h1:
            new (::std::addressof(md_atx_h1_node))::pltxt2htm::ast2::MdAtxH1(::std::move(other.md_atx_h1_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h2:
            new (::std::addressof(md_atx_h2_node))::pltxt2htm::ast2::MdAtxH2(::std::move(other.md_atx_h2_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h3:
            new (::std::addressof(md_atx_h3_node))::pltxt2htm::ast2::MdAtxH3(::std::move(other.md_atx_h3_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h4:
            new (::std::addressof(md_atx_h4_node))::pltxt2htm::ast2::MdAtxH4(::std::move(other.md_atx_h4_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h5:
            new (::std::addressof(md_atx_h5_node))::pltxt2htm::ast2::MdAtxH5(::std::move(other.md_atx_h5_node));
            break;
        case ::pltxt2htm::NodeType::md_atx_h6:
            new (::std::addressof(md_atx_h6_node))::pltxt2htm::ast2::MdAtxH6(::std::move(other.md_atx_h6_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_backslash:
            new (::std::addressof(md_escape_backslash_node))::pltxt2htm::ast2::MdEscapeBackslash(
                ::std::move(other.md_escape_backslash_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            new (::std::addressof(md_escape_exclamation_node))::pltxt2htm::ast2::MdEscapeExclamation(
                ::std::move(other.md_escape_exclamation_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            new (::std::addressof(md_escape_double_quote_node))::pltxt2htm::ast2::MdEscapeDoubleQuote(
                ::std::move(other.md_escape_double_quote_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_hash:
            new (::std::addressof(md_escape_hash_node))::pltxt2htm::ast2::MdEscapeHash(
                ::std::move(other.md_escape_hash_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_dollar:
            new (::std::addressof(md_escape_dollar_node))::pltxt2htm::ast2::MdEscapeDollar(
                ::std::move(other.md_escape_dollar_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_percent:
            new (::std::addressof(md_escape_percent_node))::pltxt2htm::ast2::MdEscapePercent(
                ::std::move(other.md_escape_percent_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            new (::std::addressof(md_escape_ampersand_node))::pltxt2htm::ast2::MdEscapeAmpersand(
                ::std::move(other.md_escape_ampersand_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            new (::std::addressof(md_escape_single_quote_node))::pltxt2htm::ast2::MdEscapeSingleQuote(
                ::std::move(other.md_escape_single_quote_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_left_paren:
            new (::std::addressof(md_escape_left_paren_node))::pltxt2htm::ast2::MdEscapeLeftParen(
                ::std::move(other.md_escape_left_paren_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_right_paren:
            new (::std::addressof(md_escape_right_paren_node))::pltxt2htm::ast2::MdEscapeRightParen(
                ::std::move(other.md_escape_right_paren_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_asterisk:
            new (::std::addressof(md_escape_asterisk_node))::pltxt2htm::ast2::MdEscapeAsterisk(
                ::std::move(other.md_escape_asterisk_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_plus:
            new (::std::addressof(md_escape_plus_node))::pltxt2htm::ast2::MdEscapePlus(
                ::std::move(other.md_escape_plus_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_comma:
            new (::std::addressof(md_escape_comma_node))::pltxt2htm::ast2::MdEscapeComma(
                ::std::move(other.md_escape_comma_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_hyphen:
            new (::std::addressof(md_escape_hyphen_node))::pltxt2htm::ast2::MdEscapeHyphen(
                ::std::move(other.md_escape_hyphen_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_dot:
            new (::std::addressof(md_escape_dot_node))::pltxt2htm::ast2::MdEscapeDot(
                ::std::move(other.md_escape_dot_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_slash:
            new (::std::addressof(md_escape_slash_node))::pltxt2htm::ast2::MdEscapeSlash(
                ::std::move(other.md_escape_slash_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_colon:
            new (::std::addressof(md_escape_colon_node))::pltxt2htm::ast2::MdEscapeColon(
                ::std::move(other.md_escape_colon_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_semicolon:
            new (::std::addressof(md_escape_semicolon_node))::pltxt2htm::ast2::MdEscapeSemicolon(
                ::std::move(other.md_escape_semicolon_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_less_than:
            new (::std::addressof(md_escape_less_than_node))::pltxt2htm::ast2::MdEscapeLessThan(
                ::std::move(other.md_escape_less_than_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_equals:
            new (::std::addressof(md_escape_equals_node))::pltxt2htm::ast2::MdEscapeEquals(
                ::std::move(other.md_escape_equals_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            new (::std::addressof(md_escape_greater_than_node))::pltxt2htm::ast2::MdEscapeGreaterThan(
                ::std::move(other.md_escape_greater_than_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_question:
            new (::std::addressof(md_escape_question_node))::pltxt2htm::ast2::MdEscapeQuestion(
                ::std::move(other.md_escape_question_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_at:
            new (::std::addressof(md_escape_at_node))::pltxt2htm::ast2::MdEscapeAt(
                ::std::move(other.md_escape_at_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_left_bracket:
            new (::std::addressof(md_escape_left_bracket_node))::pltxt2htm::ast2::MdEscapeLeftBracket(
                ::std::move(other.md_escape_left_bracket_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_right_bracket:
            new (::std::addressof(md_escape_right_bracket_node))::pltxt2htm::ast2::MdEscapeRightBracket(
                ::std::move(other.md_escape_right_bracket_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_left_brace:
            new (::std::addressof(md_escape_left_brace_node))::pltxt2htm::ast2::MdEscapeLeftBrace(
                ::std::move(other.md_escape_left_brace_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_pipe:
            new (::std::addressof(md_escape_pipe_node))::pltxt2htm::ast2::MdEscapePipe(
                ::std::move(other.md_escape_pipe_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_right_brace:
            new (::std::addressof(md_escape_right_brace_node))::pltxt2htm::ast2::MdEscapeRightBrace(
                ::std::move(other.md_escape_right_brace_node));
            break;
        case ::pltxt2htm::NodeType::md_escape_tilde:
            new (::std::addressof(md_escape_tilde_node))::pltxt2htm::ast2::MdEscapeTilde(
                ::std::move(other.md_escape_tilde_node));
            break;
        case ::pltxt2htm::NodeType::md_hr:
            new (::std::addressof(md_hr_node))::pltxt2htm::ast2::MdHr(::std::move(other.md_hr_node));
            break;
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            new (::std::addressof(md_code_fence_backtick_node))::pltxt2htm::ast2::MdCodeFenceBacktick(
                ::std::move(other.md_code_fence_backtick_node));
            break;
        case ::pltxt2htm::NodeType::md_code_fence_tilde:
            new (::std::addressof(md_code_fence_tilde_node))::pltxt2htm::ast2::MdCodeFenceTilde(
                ::std::move(other.md_code_fence_tilde_node));
            break;
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
            new (::std::addressof(md_code_span_1_backtick_node))::pltxt2htm::ast2::MdCodeSpan1Backtick(
                ::std::move(other.md_code_span_1_backtick_node));
            break;
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
            new (::std::addressof(md_code_span_2_backtick_node))::pltxt2htm::ast2::MdCodeSpan2Backtick(
                ::std::move(other.md_code_span_2_backtick_node));
            break;
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
            new (::std::addressof(md_code_span_3_backtick_node))::pltxt2htm::ast2::MdCodeSpan3Backtick(
                ::std::move(other.md_code_span_3_backtick_node));
            break;
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            new (::std::addressof(md_single_emphasis_asterisk_node))::pltxt2htm::ast2::MdSingleEmphasisAsterisk(
                ::std::move(other.md_single_emphasis_asterisk_node));
            break;
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            new (::std::addressof(md_double_emphasis_asterisk_node))::pltxt2htm::ast2::MdDoubleEmphasisAsterisk(
                ::std::move(other.md_double_emphasis_asterisk_node));
            break;
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            new (::std::addressof(md_triple_emphasis_asterisk_node))::pltxt2htm::ast2::MdTripleEmphasisAsterisk(
                ::std::move(other.md_triple_emphasis_asterisk_node));
            break;
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            new (::std::addressof(md_single_emphasis_underscore_node))::pltxt2htm::ast2::MdSingleEmphasisUnderscore(
                ::std::move(other.md_single_emphasis_underscore_node));
            break;
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            new (::std::addressof(md_double_emphasis_underscore_node))::pltxt2htm::ast2::MdDoubleEmphasisUnderscore(
                ::std::move(other.md_double_emphasis_underscore_node));
            break;
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            new (::std::addressof(md_triple_emphasis_underscore_node))::pltxt2htm::ast2::MdTripleEmphasisUnderscore(
                ::std::move(other.md_triple_emphasis_underscore_node));
            break;
        case ::pltxt2htm::NodeType::md_del:
            new (::std::addressof(md_del_node))::pltxt2htm::ast2::MdDel(::std::move(other.md_del_node));
            break;
        case ::pltxt2htm::NodeType::md_link:
            new (::std::addressof(md_link_node))::pltxt2htm::ast2::MdLink(::std::move(other.md_link_node));
            break;
        case ::pltxt2htm::NodeType::md_image:
            new (::std::addressof(md_image_node))::pltxt2htm::ast2::MdImage(::std::move(other.md_image_node));
            break;
        case ::pltxt2htm::NodeType::md_block_quotes:
            new (::std::addressof(md_block_quotes_node))::pltxt2htm::ast2::MdBlockQuotes(
                ::std::move(other.md_block_quotes_node));
            break;
        case ::pltxt2htm::NodeType::md_ul:
            new (::std::addressof(md_ul_node))::pltxt2htm::ast2::MdUl(::std::move(other.md_ul_node));
            break;
        case ::pltxt2htm::NodeType::md_ol:
            new (::std::addressof(md_ol_node))::pltxt2htm::ast2::MdOl(::std::move(other.md_ol_node));
            break;
        case ::pltxt2htm::NodeType::md_li:
            new (::std::addressof(md_li_node))::pltxt2htm::ast2::MdLi(::std::move(other.md_li_node));
            break;
        case ::pltxt2htm::NodeType::md_latex_inline:
            new (::std::addressof(md_latex_inline_node))::pltxt2htm::ast2::MdLatexInline(
                ::std::move(other.md_latex_inline_node));
            break;
        case ::pltxt2htm::NodeType::md_latex_block:
            new (::std::addressof(md_latex_block_node))::pltxt2htm::ast2::MdLatexBlock(
                ::std::move(other.md_latex_block_node));
            break;
        case ::pltxt2htm::NodeType::base:
            [[unlikely]] {
                ::exception::unreachable();
            }
        }
    }

    constexpr ~AstVariant() noexcept {
        switch (node_kind) {
        case ::pltxt2htm::NodeType::u8char:
            u8char_node.~U8Char();
            break;
        case ::pltxt2htm::NodeType::invalid_u8char:
            invalid_u8char_node.~InvalidU8Char();
            break;
        case ::pltxt2htm::NodeType::text:
            text_node.~Text();
            break;
        case ::pltxt2htm::NodeType::pl_color:
            pl_color_node.~PlColor();
            break;
        case ::pltxt2htm::NodeType::pl_a:
            pl_a_node.~PlA();
            break;
        case ::pltxt2htm::NodeType::pl_experiment:
            pl_experiment_node.~PlExperiment();
            break;
        case ::pltxt2htm::NodeType::pl_discussion:
            pl_discussion_node.~PlDiscussion();
            break;
        case ::pltxt2htm::NodeType::pl_user:
            pl_user_node.~PlUser();
            break;
        case ::pltxt2htm::NodeType::pl_external:
            pl_external_node.~PlExternal();
            break;
        case ::pltxt2htm::NodeType::pl_size:
            pl_size_node.~PlSize();
            break;
        case ::pltxt2htm::NodeType::pl_i:
            pl_i_node.~PlI();
            break;
        case ::pltxt2htm::NodeType::pl_b:
            pl_b_node.~PlB();
            break;
        case ::pltxt2htm::NodeType::pl_macro_project:
            pl_macro_project_node.~PlMacroProject();
            break;
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            pl_macro_visitor_node.~PlMacroVisitor();
            break;
        case ::pltxt2htm::NodeType::pl_macro_author:
            pl_macro_author_node.~PlMacroAuthor();
            break;
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            pl_macro_coauthors_node.~PlMacroCoauthors();
            break;

        case ::pltxt2htm::NodeType::line_break:
            linebreak_node.~LineBreak();
            break;

        case ::pltxt2htm::NodeType::html_br:
            br_node.~Br();
            break;

        case ::pltxt2htm::NodeType::space:
            space_node.~Space();
            break;

        case ::pltxt2htm::NodeType::less_than:
            lessthan_node.~LessThan();
            break;

        case ::pltxt2htm::NodeType::greater_than:
            greaterthan_node.~GreaterThan();
            break;

        case ::pltxt2htm::NodeType::tab:
            tab_node.~Tab();
            break;

        case ::pltxt2htm::NodeType::ampersand:
            ampersand_node.~Ampersand();
            break;

        case ::pltxt2htm::NodeType::single_quote:
            singlequotationmark_node.~SingleQuotationMark();
            break;

        case ::pltxt2htm::NodeType::double_quote:
            doublequotationmark_node.~DoubleQuotationMark();
            break;

        case ::pltxt2htm::NodeType::html_hr:
            hr_node.~Hr();
            break;

        case ::pltxt2htm::NodeType::html_h1:
            h1_node.~H1();
            break;

        case ::pltxt2htm::NodeType::html_h2:
            h2_node.~H2();
            break;

        case ::pltxt2htm::NodeType::html_h3:
            h3_node.~H3();
            break;

        case ::pltxt2htm::NodeType::html_h4:
            h4_node.~H4();
            break;

        case ::pltxt2htm::NodeType::html_h5:
            h5_node.~H5();
            break;

        case ::pltxt2htm::NodeType::html_h6:
            h6_node.~H6();
            break;

        case ::pltxt2htm::NodeType::html_p:
            p_node.~P();
            break;

        case ::pltxt2htm::NodeType::html_del:
            del_node.~Del();
            break;

        case ::pltxt2htm::NodeType::html_note:
            note_node.~Note();
            break;

        case ::pltxt2htm::NodeType::html_em:
            em_node.~Em();
            break;

        case ::pltxt2htm::NodeType::html_strong:
            strong_node.~Strong();
            break;

        case ::pltxt2htm::NodeType::html_ul:
            ul_node.~Ul();
            break;

        case ::pltxt2htm::NodeType::html_ol:
            ol_node.~Ol();
            break;

        case ::pltxt2htm::NodeType::html_li:
            li_node.~Li();
            break;

        case ::pltxt2htm::NodeType::html_code:
            code_node.~Code();
            break;

        case ::pltxt2htm::NodeType::html_pre:
            pre_node.~Pre();
            break;

        case ::pltxt2htm::NodeType::html_blockquote:
            blockquote_node.~Blockquote();
            break;
        case ::pltxt2htm::NodeType::md_atx_h1:
            md_atx_h1_node.~MdAtxH1();
            break;
        case ::pltxt2htm::NodeType::md_atx_h2:
            md_atx_h2_node.~MdAtxH2();
            break;
        case ::pltxt2htm::NodeType::md_atx_h3:
            md_atx_h3_node.~MdAtxH3();
            break;
        case ::pltxt2htm::NodeType::md_atx_h4:
            md_atx_h4_node.~MdAtxH4();
            break;
        case ::pltxt2htm::NodeType::md_atx_h5:
            md_atx_h5_node.~MdAtxH5();
            break;
        case ::pltxt2htm::NodeType::md_atx_h6:
            md_atx_h6_node.~MdAtxH6();
            break;
        case ::pltxt2htm::NodeType::md_escape_backslash:
            md_escape_backslash_node.~MdEscapeBackslash();
            break;
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            md_escape_exclamation_node.~MdEscapeExclamation();
            break;
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            md_escape_double_quote_node.~MdEscapeDoubleQuote();
            break;
        case ::pltxt2htm::NodeType::md_escape_hash:
            md_escape_hash_node.~MdEscapeHash();
            break;
        case ::pltxt2htm::NodeType::md_escape_dollar:
            md_escape_dollar_node.~MdEscapeDollar();
            break;
        case ::pltxt2htm::NodeType::md_escape_percent:
            md_escape_percent_node.~MdEscapePercent();
            break;
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            md_escape_ampersand_node.~MdEscapeAmpersand();
            break;
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            md_escape_single_quote_node.~MdEscapeSingleQuote();
            break;
        case ::pltxt2htm::NodeType::md_escape_left_paren:
            md_escape_left_paren_node.~MdEscapeLeftParen();
            break;
        case ::pltxt2htm::NodeType::md_escape_right_paren:
            md_escape_right_paren_node.~MdEscapeRightParen();
            break;
        case ::pltxt2htm::NodeType::md_escape_asterisk:
            md_escape_asterisk_node.~MdEscapeAsterisk();
            break;
        case ::pltxt2htm::NodeType::md_escape_plus:
            md_escape_plus_node.~MdEscapePlus();
            break;
        case ::pltxt2htm::NodeType::md_escape_comma:
            md_escape_comma_node.~MdEscapeComma();
            break;
        case ::pltxt2htm::NodeType::md_escape_hyphen:
            md_escape_hyphen_node.~MdEscapeHyphen();
            break;
        case ::pltxt2htm::NodeType::md_escape_dot:
            md_escape_dot_node.~MdEscapeDot();
            break;
        case ::pltxt2htm::NodeType::md_escape_slash:
            md_escape_slash_node.~MdEscapeSlash();
            break;
        case ::pltxt2htm::NodeType::md_escape_colon:
            md_escape_colon_node.~MdEscapeColon();
            break;
        case ::pltxt2htm::NodeType::md_escape_semicolon:
            md_escape_semicolon_node.~MdEscapeSemicolon();
            break;
        case ::pltxt2htm::NodeType::md_escape_less_than:
            md_escape_less_than_node.~MdEscapeLessThan();
            break;
        case ::pltxt2htm::NodeType::md_escape_equals:
            md_escape_equals_node.~MdEscapeEquals();
            break;
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            md_escape_greater_than_node.~MdEscapeGreaterThan();
            break;
        case ::pltxt2htm::NodeType::md_escape_question:
            md_escape_question_node.~MdEscapeQuestion();
            break;
        case ::pltxt2htm::NodeType::md_escape_at:
            md_escape_at_node.~MdEscapeAt();
            break;
        case ::pltxt2htm::NodeType::md_escape_left_bracket:
            md_escape_left_bracket_node.~MdEscapeLeftBracket();
            break;
        case ::pltxt2htm::NodeType::md_escape_right_bracket:
            md_escape_right_bracket_node.~MdEscapeRightBracket();
            break;
        case ::pltxt2htm::NodeType::md_escape_left_brace:
            md_escape_left_brace_node.~MdEscapeLeftBrace();
            break;
        case ::pltxt2htm::NodeType::md_escape_pipe:
            md_escape_pipe_node.~MdEscapePipe();
            break;
        case ::pltxt2htm::NodeType::md_escape_right_brace:
            md_escape_right_brace_node.~MdEscapeRightBrace();
            break;
        case ::pltxt2htm::NodeType::md_escape_tilde:
            md_escape_tilde_node.~MdEscapeTilde();
            break;
        case ::pltxt2htm::NodeType::md_hr:
            md_hr_node.~MdHr();
            break;
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            md_code_fence_backtick_node.~MdCodeFenceBacktick();
            break;
        case ::pltxt2htm::NodeType::md_code_fence_tilde:
            md_code_fence_tilde_node.~MdCodeFenceTilde();
            break;
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
            md_code_span_1_backtick_node.~MdCodeSpan1Backtick();
            break;
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
            md_code_span_2_backtick_node.~MdCodeSpan2Backtick();
            break;
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
            md_code_span_3_backtick_node.~MdCodeSpan3Backtick();
            break;
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            md_single_emphasis_asterisk_node.~MdSingleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            md_double_emphasis_asterisk_node.~MdDoubleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            md_triple_emphasis_asterisk_node.~MdTripleEmphasisAsterisk();
            break;
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            md_single_emphasis_underscore_node.~MdSingleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            md_double_emphasis_underscore_node.~MdDoubleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            md_triple_emphasis_underscore_node.~MdTripleEmphasisUnderscore();
            break;
        case ::pltxt2htm::NodeType::md_del:
            md_del_node.~MdDel();
            break;
        case ::pltxt2htm::NodeType::md_link:
            md_link_node.~MdLink();
            break;
        case ::pltxt2htm::NodeType::md_image:
            md_image_node.~MdImage();
            break;
        case ::pltxt2htm::NodeType::md_block_quotes:
            md_block_quotes_node.~MdBlockQuotes();
            break;
        case ::pltxt2htm::NodeType::md_ul:
            md_ul_node.~MdUl();
            break;
        case ::pltxt2htm::NodeType::md_ol:
            md_ol_node.~MdOl();
            break;
        case ::pltxt2htm::NodeType::md_li:
            md_li_node.~MdLi();
            break;
        case ::pltxt2htm::NodeType::md_latex_inline:
            md_latex_inline_node.~MdLatexInline();
            break;
        case ::pltxt2htm::NodeType::md_latex_block:
            md_latex_block_node.~MdLatexBlock();
            break;
        case ::pltxt2htm::NodeType::base:
            [[unlikely]] {
                ::exception::unreachable();
            }
        }
    }
};

} // namespace details

class PlTxtNode {
    ::pltxt2htm::ast2::details::AstVariant node;
};

} // namespace pltxt2htm::ast2

#include "impl/basic_node_def.inc"
#include "impl/html_node_def.inc"
#include "impl/markdown_node_def.inc"
#include "impl/physics_lab_node_def.inc"
