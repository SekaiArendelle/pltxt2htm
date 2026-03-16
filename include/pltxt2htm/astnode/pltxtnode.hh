#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include "node_type.hh"
#include "basic.hh"
#include "physics_lab_node.hh"
#include "html_node.hh"
#include "markdown_node.hh"

namespace pltxt2htm {

// variant of all node types
class PlTxtNode {
    union {
        // Character and basic text nodes
        ::pltxt2htm::U8Char u8char;
        ::pltxt2htm::InvalidU8Char invalid_u8char;
        ::pltxt2htm::Text text;

        // Whitespace and special characters
        ::pltxt2htm::LineBreak line_break;
        ::pltxt2htm::Space space;
        ::pltxt2htm::Ampersand ampersand;
        ::pltxt2htm::DoubleQuotationMark double_quote;
        ::pltxt2htm::SingleQuotationMark single_quote;
        ::pltxt2htm::LessThan less_than;
        ::pltxt2htm::GreaterThan greater_than;
        ::pltxt2htm::Tab tab;

        // Physics-Lab specific formatting tags
        ::pltxt2htm::Color pl_color;
        ::pltxt2htm::A pl_a;
        ::pltxt2htm::Experiment pl_experiment;
        ::pltxt2htm::Discussion pl_discussion;
        ::pltxt2htm::User pl_user;
        ::pltxt2htm::Size pl_size;
        ::pltxt2htm::External pl_external;

        // Text formatting
        ::pltxt2htm::B pl_b;
        ::pltxt2htm::I pl_i;

        // Physics-Lab specific macros
        ::pltxt2htm::PlMacroProject pl_macro_project;
        ::pltxt2htm::PlMacroVisitor pl_macro_visitor;
        ::pltxt2htm::PlMacroAuthor pl_macro_author;
        ::pltxt2htm::PlMacroCoauthors pl_macro_coauthors;

        // HTML structural elements
        ::pltxt2htm::P html_p;
        ::pltxt2htm::Br html_br;

        // HTML heading elements
        ::pltxt2htm::H1 html_h1;
        ::pltxt2htm::H2 html_h2;
        ::pltxt2htm::H3 html_h3;
        ::pltxt2htm::H4 html_h4;
        ::pltxt2htm::H5 html_h5;
        ::pltxt2htm::H6 html_h6;

        // HTML formatting elements
        ::pltxt2htm::Del html_del;
        ::pltxt2htm::Hr html_hr;
        ::pltxt2htm::Note html_note;
        ::pltxt2htm::Em html_em;
        ::pltxt2htm::Strong html_strong;

        // HTML list elements
        ::pltxt2htm::Ul html_ul;
        ::pltxt2htm::Ol html_ol;
        ::pltxt2htm::Li html_li;

        // HTML code and quote elements
        ::pltxt2htm::Code html_code;
        ::pltxt2htm::Pre html_pre;
        ::pltxt2htm::Blockquote html_blockquote;

        // Markdown ATX-style headers
        ::pltxt2htm::MdAtxH1 md_atx_h1;
        ::pltxt2htm::MdAtxH2 md_atx_h2;
        ::pltxt2htm::MdAtxH3 md_atx_h3;
        ::pltxt2htm::MdAtxH4 md_atx_h4;
        ::pltxt2htm::MdAtxH5 md_atx_h5;
        ::pltxt2htm::MdAtxH6 md_atx_h6;

        // Markdown escape sequences
        ::pltxt2htm::EscapeBackslash md_escape_backslash;
        ::pltxt2htm::EscapeExclamation md_escape_exclamation;
        ::pltxt2htm::EscapeDoubleQuote md_escape_double_quote;
        ::pltxt2htm::EscapeHash md_escape_hash;
        ::pltxt2htm::EscapeDollar md_escape_dollar;
        ::pltxt2htm::EscapePercent md_escape_percent;
        ::pltxt2htm::EscapeAmpersand md_escape_ampersand;
        ::pltxt2htm::EscapeSingleQuote md_escape_single_quote;
        ::pltxt2htm::EscapeLeftParen md_escape_left_paren;
        ::pltxt2htm::EscapeRightParen md_escape_right_paren;
        ::pltxt2htm::EscapeAsterisk md_escape_asterisk;
        ::pltxt2htm::EscapePlus md_escape_plus;
        ::pltxt2htm::EscapeComma md_escape_comma;
        ::pltxt2htm::EscapeHyphen md_escape_hyphen;
        ::pltxt2htm::EscapeDot md_escape_dot;
        ::pltxt2htm::EscapeSlash md_escape_slash;
        ::pltxt2htm::EscapeColon md_escape_colon;
        ::pltxt2htm::EscapeSemicolon md_escape_semicolon;
        ::pltxt2htm::EscapeLessThan md_escape_less_than;
        ::pltxt2htm::EscapeEquals md_escape_equals;
        ::pltxt2htm::EscapeGreaterThan md_escape_greater_than;
        ::pltxt2htm::EscapeQuestion md_escape_question;
        ::pltxt2htm::EscapeAt md_escape_at;
        ::pltxt2htm::EscapeLeftBracket md_escape_left_bracket;
        ::pltxt2htm::EscapeRightBracket md_escape_right_bracket;
        ::pltxt2htm::EscapeCaret md_escape_caret;
        ::pltxt2htm::EscapeUnderscore md_escape_underscore;
        ::pltxt2htm::EscapeBacktick md_escape_backtick;
        ::pltxt2htm::EscapeLeftBrace md_escape_left_brace;
        ::pltxt2htm::EscapePipe md_escape_pipe;
        ::pltxt2htm::EscapeRightBrace md_escape_right_brace;
        ::pltxt2htm::EscapeTilde md_escape_tilde;

        // Markdown thematic elements
        ::pltxt2htm::MdHr md_hr;

        // Markdown code blocks (fenced code)
        ::pltxt2htm::MdCodeFenceBacktick md_code_fence_backtick;
        ::pltxt2htm::MdCodeFenceTilde md_code_fence_tilde;

        // Markdown inline code spans
        ::pltxt2htm::MdCodeSpan1Backtick md_code_span_1_backtick;
        ::pltxt2htm::MdCodeSpan2Backtick md_code_span_2_backtick;
        ::pltxt2htm::MdCodeSpan3Backtick md_code_span_3_backtick;

        // Markdown emphasis
        ::pltxt2htm::MdSingleEmphasisAsterisk md_single_emphasis_asterisk;
        ::pltxt2htm::MdDoubleEmphasisAsterisk md_double_emphasis_asterisk;
        ::pltxt2htm::MdTripleEmphasisAsterisk md_triple_emphasis_asterisk;
        ::pltxt2htm::MdSingleEmphasisUnderscore md_single_emphasis_underscore;
        ::pltxt2htm::MdDoubleEmphasisUnderscore md_double_emphasis_underscore;
        ::pltxt2htm::MdTripleEmphasisUnderscore md_triple_emphasis_underscore;

        // Markdown strikethrough
        ::pltxt2htm::MdDel md_del;

        // Markdown links and references
        ::pltxt2htm::MdLink md_link;

        // Markdown block quotes and lists
        ::pltxt2htm::MdBlockQuotes md_block_quotes;
        ::pltxt2htm::MdUl md_ul;
        ::pltxt2htm::MdOl md_ol;
        ::pltxt2htm::MdLi md_li;

        // LaTeX support
        ::pltxt2htm::MdLatexInline md_latex_inline;
        ::pltxt2htm::MdLatexBlock md_latex_block;
    };
    ::pltxt2htm::NodeType node_type;

public:
    constexpr auto get_node_type(this ::pltxt2htm::PlTxtNode const& self) noexcept -> ::pltxt2htm::NodeType {
        return self.node_type;
    }
};

}
