/**
 * @file node_type.hh
 * @brief AST node type definitions for pltxt2htm
 * @details Defines all possible node types in the Abstract Syntax Tree for
 *          Physics-Lab text parsing. Each node type represents a different
 *          syntactic element that can appear in the input text.
 */

#pragma once

#include <cstddef>

namespace pltxt2htm {

/**
 * @brief AST node type enumeration for Physics-Lab (pl) text
 * @details This enum defines all possible node types that can appear in the
 *          Abstract Syntax Tree. Each node in the AST has exactly one of these
 *          types, which determines how it should be processed and rendered.
 * @note The values are ordered with basic types first, then Physics-Lab specific
 *       tags, HTML tags, Markdown syntax, and finally escape sequences.
 */
enum class NodeType : ::std::size_t {
    base = 0, ///< Base node type (should not appear in final AST)
    // Character and basic text nodes
    u8char, ///< Valid UTF-8 character
    invalid_u8char, ///< Invalid UTF-8 character (replacement character)
    text, ///< Text container node (groups characters and inline elements)

    // Whitespace and special characters
    line_break, ///< Line break character (\n)
    space, ///< Space character (will be rendered as &nbsp; in HTML)
    ampersand, ///< Ampersand character (&) - escaped to &amp;
    double_quote, ///< Double quote character (") - escaped to &quot;
    single_quote, ///< Single quote character (') - escaped to &apos;
    less_than, ///< Less-than character (<) - escaped to &lt;
    greater_than, ///< Greater-than character (>) - escaped to &gt;
    tab, ///< Tab character (\t) - rendered as multiple &nbsp; entities

    // Physics-Lab specific formatting tags
    pl_color, ///< Physics-Lab color tag: <color=value>...</color>
    pl_a, ///< Physics-Lab anchor tag: <a>...</a> (styled like a link)
    pl_experiment, ///< Physics-Lab experiment reference: <experiment=id>...</experiment>
    pl_discussion, ///< Physics-Lab discussion reference: <discussion=id>...</discussion>
    pl_user, ///< Physics-Lab user reference: <user=id>...</user>
    pl_size, ///< Physics-Lab font size: <size=value>...</size>

    // Text formatting (shared across Physics-Lab, HTML, and Markdown)
    pl_b, ///< Bold text: <b>...</b>, **text** (Markdown), <strong> (HTML)
    pl_i, ///< Italic text: <i>...</i>, *text* (Markdown), <em> (HTML)

    // Physics-Lab specific macros
    pl_macro_project, // {Project}
    pl_macro_visitor, // {Visitor}
    pl_macro_author, // {Author}
    pl_macro_coauthors, // {CoAuthors}

    // HTML structural elements
    html_p, ///< Paragraph: <p>...</p> (HTML), created by double newline (Markdown)
    html_br, ///< Line break: <br> (HTML)

    // HTML heading elements
    html_h1, ///< Level 1 heading: <h1>...</h1>
    html_h2, ///< Level 2 heading: <h2>...</h2>
    html_h3, ///< Level 3 heading: <h3>...</h3>
    html_h4, ///< Level 4 heading: <h4>...</h4>
    html_h5, ///< Level 5 heading: <h5>...</h5>
    html_h6, ///< Level 6 heading: <h6>...</h6>

    // HTML formatting elements
    html_del, ///< Deleted/strikethrough text: <del>...</del>
    html_hr, ///< Horizontal rule: <hr> (HTML), --- (Markdown)
    html_note, ///< HTML comment/note (not rendered in output)
    html_em, ///< Emphasis: <em>...</em> (HTML equivalent of <i>)
    html_strong, ///< Strong emphasis: <strong>...</strong> (HTML equivalent of <b>)

    // HTML list elements
    html_ul, ///< Unordered list: <ul>...</ul>
    html_ol, ///< Ordered list: <ol>...</ol>
    html_li, ///< List item: <li>...</li>

    // HTML code and quote elements
    html_code, ///< Inline code: <code>...</code>
    html_pre, ///< Preformatted text: <pre>...</pre>
    html_blockquote, ///< Block quote: <blockquote>...</blockquote>

    // Markdown ATX-style headers (# ## ### #### ##### ######)
    md_atx_h1, ///< Markdown level 1 heading: # Heading
    md_atx_h2, ///< Markdown level 2 heading: ## Heading
    md_atx_h3, ///< Markdown level 3 heading: ### Heading
    md_atx_h4, ///< Markdown level 4 heading: #### Heading
    md_atx_h5, ///< Markdown level 5 heading: ##### Heading
    md_atx_h6, ///< Markdown level 6 heading: ###### Heading

    // Markdown escape sequences (backslash-prefixed characters)
    md_escape_backslash,
    md_escape_exclamation, ///< Escaped exclamation: \\!
    md_escape_double_quote, ///< Escaped double quote: \\"
    md_escape_hash, ///< Escaped hash: \\#
    md_escape_dollar, ///< Escaped dollar: \\$
    md_escape_percent, ///< Escaped percent: \\%
    md_escape_ampersand, ///< Escaped ampersand: \\&
    md_escape_single_quote, ///< Escaped single quote: \\'
    md_escape_left_paren, ///< Escaped left parenthesis: \\(
    md_escape_right_paren, ///< Escaped right parenthesis: \\)
    md_escape_asterisk, ///< Escaped asterisk: \\*
    md_escape_plus, ///< Escaped plus: \\+
    md_escape_comma, ///< Escaped comma: \\,
    md_escape_hyphen, ///< Escaped hyphen: \\-
    md_escape_dot, ///< Escaped dot: \\.
    md_escape_slash, ///< Escaped slash: \\/
    md_escape_colon, ///< Escaped colon: \\:
    md_escape_semicolon, ///< Escaped semicolon: \\;
    md_escape_less_than, ///< Escaped less-than: \\<
    md_escape_equals, ///< Escaped equals: \\=
    md_escape_greater_than, ///< Escaped greater-than: \\>
    md_escape_question, ///< Escaped question mark: \\?
    md_escape_at, ///< Escaped at symbol: \\@
    md_escape_left_bracket, ///< Escaped left bracket: \\[
    md_escape_right_bracket, ///< Escaped right bracket: \\]
    md_escape_caret, ///< Escaped caret: \\^
    md_escape_underscore, ///< Escaped underscore: \\_
    md_escape_backtick, ///< Escaped backtick: \\`
    md_escape_left_brace, ///< Escaped left brace: \\{
    md_escape_pipe, ///< Escaped pipe: \\|
    md_escape_right_brace, ///< Escaped right brace: \\}
    md_escape_tilde, ///< Escaped tilde: \\~
    // Markdown thematic elements
    md_hr, ///< Thematic break/horizontal rule: ---, ***, ___

    // Markdown code blocks (fenced code)
    md_code_fence_backtick, ///< Code fence with backticks: ```
    md_code_fence_tilde, ///< Code fence with tildes: ~~~

    // Markdown inline code spans
    md_code_span_1_backtick, ///< Inline code with 1 backtick: `code`
    md_code_span_2_backtick, ///< Inline code with 2 backticks: ``code``
    md_code_span_3_backtick, ///< Inline code with 3 backticks: ```code```

    // Markdown emphasis (CommonMark spec section 6.4)
    md_single_emphasis_asterisk, ///< Single emphasis with asterisk: *text* (italic)
    md_double_emphasis_asterisk, ///< Double emphasis with asterisk: **text** (bold)
    md_triple_emphasis_asterisk, ///< Triple emphasis with asterisk: ***text*** (bold italic)
    md_single_emphasis_underscore, ///< Single emphasis with underscore: _text_ (italic)
    md_double_emphasis_underscore, ///< Double emphasis with underscore: __text__ (bold)
    md_triple_emphasis_underscore, ///< Triple emphasis with underscore: ___text___ (bold italic)

    // Markdown strikethrough
    md_del, ///< Strikethrough text: ~~text~~

    // Markdown links and references
    md_link, ///< Markdown link: [text](url)

    // Markdown block quotes and lists
    md_block_quotes, ///< Block quote: > quote text
    md_ul, ///< Unordered list marker
    md_ol, ///< Ordered list marker
    md_li, ///< List item

    // LaTeX support
    md_latex_inline, ///< Inline LaTeX math: $...$
    md_latex_block, ///< Block/display LaTeX math: $$...$$
};

} // namespace pltxt2htm
