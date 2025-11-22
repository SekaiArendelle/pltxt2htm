#pragma once

#include <cstddef>

namespace pltxt2htm {

/**
 * @brief ast node type of Quantum-Physics (aka. Physics-Lab, pl) text
 * @details show details in astnode.hh, every AstNode owns a NodeType
 */
enum class NodeType : ::std::size_t {
    base = 0,
    // a utf-8 char
    u8char,
    // invalid utf-8 char
    invalid_u8char,
    // text
    text,

    // \n
    line_break,
    // space will be squashed by html render
    space,
    // &
    ampersand,
    // "
    double_quote,
    // '
    single_quote,
    // <
    less_than,
    // >
    greater_than,
    // \t
    tab,

    // Quantum-Physics's <color> tag
    pl_color,
    // Quantum-Physics's <a> tag
    pl_a,
    // Quantum-Physics's <experiment> tag
    pl_experiment,
    // Quantum-Physics's <discussion> tag
    pl_discussion,
    // Quantum-Physics's <user> tag
    pl_user,
    // Quantum-Physics's <size> tag
    pl_size,

    // bold
    pl_b, // be used by Quantum-Physics, html, markdown(**)
    // italic
    pl_i, // be used by Quantum-Physics, html, markdown(*)

    // paragraph
    html_p, // be used by html, markdown(\n\n)
    // HTML's <br> tag
    html_br,
    // html header tags
    html_h1,
    html_h2,
    html_h3,
    html_h4,
    html_h5,
    html_h6,
    // html <del> tag
    html_del,
    // html <hr> tag
    html_hr,
    // html notes
    html_note,
    // html <em> tag
    html_em,
    // html <strong> tag
    html_strong,
    // html <ul> tag
    html_ul,
    // html <ol> tag
    html_ol,
    // html <li> tag
    html_li,
    // html <code> tag
    html_code,
    // html <pre> tag
    html_pre,
    // html <blockquote> tag
    html_blockquote,

    // markdown headers
    md_atx_h1,
    md_atx_h2,
    md_atx_h3,
    md_atx_h4,
    md_atx_h5,
    md_atx_h6,

    /* \ */
    md_escape_backslash,
    /* ! */
    md_escape_exclamation,
    /* " */
    md_escape_double_quote,
    /* # */
    md_escape_hash,
    /* $ */
    md_escape_dollar,
    /* % */
    md_escape_percent,
    /* & */
    md_escape_ampersand,
    /* ' */
    md_escape_single_quote,
    /* ( */
    md_escape_left_paren,
    /* ) */
    md_escape_right_paren,
    /* * */
    md_escape_asterisk,
    /* + */
    md_escape_plus,
    /* , */
    md_escape_comma,
    /* - */
    md_escape_hyphen,
    /* . */
    md_escape_dot,
    /* / */
    md_escape_slash,
    /* : */
    md_escape_colon,
    /* ; */
    md_escape_semicolon,
    /* < */
    md_escape_less_than,
    /* = */
    md_escape_equals,
    /* > */
    md_escape_greater_than,
    /* ? */
    md_escape_question,
    /* @ */
    md_escape_at,
    /* [ */
    md_escape_left_bracket,
    /* ] */
    md_escape_right_bracket,
    /* ^ */
    md_escape_caret,
    /* _ */
    md_escape_underscore,
    /* ` */
    md_escape_backtick,
    /* { */
    md_escape_left_brace,
    /* | */
    md_escape_pipe,
    /* } */
    md_escape_right_brace,
    /* ~ */
    md_escape_tilde,
    // https://spec.commonmark.org/0.31.2/#thematic-break
    md_hr,
    // https://spec.commonmark.org/0.31.2/#code-fence
    md_code_fence_backtick,
    md_code_fence_tilde,
    // https://spec.commonmark.org/0.31.2/#code-spans
    md_code_span_1_backtick,
    md_code_span_2_backtick,
    md_code_span_3_backtick,
    // https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
    md_single_emphasis_asterisk,
    md_double_emphasis_asterisk,
    md_triple_emphasis_asterisk,
    md_single_emphasis_underscore,
    md_double_emphasis_underscore,
    md_triple_emphasis_underscore,
    // ~~example~~
    md_del,
    // https://spec.commonmark.org/0.31.2/#links
    md_link,
    // https://spec.commonmark.org/0.31.2/#block-quotes
    md_block_quotes,
    // Markdown list
    md_ul,
    md_ol,
    md_li,

    // latex
    // latex,
};

} // namespace pltxt2htm
