/**
 * @file node_kind.hh
 * @brief AST node type definitions for pltxt2htm
 * @details Defines all possible node types in the Abstract Syntax Tree for
 *          Physics-Lab text parsing. Each node type represents a different
 *          syntactic element that can appear in the input text.
 */

#pragma once

namespace pltxt2htm {

/**
 * @brief AST node type enumeration for Physics-Lab (pl) text
 * @details This enum defines all possible node types that can appear in the
 *          Abstract Syntax Tree. Each node in the AST has exactly one of these
 *          types, which determines how it should be processed and rendered.
 * @note The values are ordered with basic types first, then Physics-Lab specific
 *       tags, HTML tags, Markdown syntax, and finally escape sequences.
 */
enum class NodeKind : unsigned {
    // Character and basic text nodes
    u8char = 0, ///< Valid UTF-8 character
    invalid_utf8, ///< Invalid UTF-8 input (replacement character)
    group, ///< Transparent container node (groups characters and inline elements)
    url, ///< Auto-detected URL link: https://example.com

    // Whitespace and special characters
    line_break, ///< Line break character (\n)
    // TODO: Distinguish U+0020 from U+00A0 after testing both behaviors in Unity TextMeshPro.
    // Both currently render as non-breaking spaces to match Physics-Lab.
    space, ///< Space character (will be rendered as &nbsp; in HTML)
    ampersand, ///< Ampersand character (&) - escaped to &amp;
    double_quote, ///< Double quote character (") - escaped to &quot;
    single_quote, ///< Single quote character (') - escaped to &apos;
    less_than, ///< Less-than character (<) - escaped to &lt;
    greater_than, ///< Greater-than character (>) - escaped to &gt;
    entity_reference, ///< HTML entity reference: &amp; name &amp;; e.g. &amp;quot;, &amp;amp;, &amp;#38;
    tab, ///< Tab character - rendered as multiple &nbsp; entities

    // Physics-Lab specific formatting tags
    pl_color, ///< Physics-Lab color tag: &lt;color=value&gt;...&lt;/color&gt;
    pl_a, ///< Physics-Lab anchor tag: &lt;a&gt;...&lt;/a&gt; (styled like a link)
    pl_experiment, ///< Physics-Lab experiment reference: &lt;experiment=id&gt;...&lt;/experiment&gt;
    pl_discussion, ///< Physics-Lab discussion reference: &lt;discussion=id&gt;...&lt;/discussion&gt;
    pl_experiments,
    ///< Physics-Lab experiment list-entry reference: &lt;experiments=params&gt;...&lt;/experiments&gt;
    pl_discussions,
    ///< Physics-Lab discussion list-entry reference: &lt;discussions=params&gt;...&lt;/discussions&gt;
    pl_user, ///< Physics-Lab user reference: &lt;user=id&gt;...&lt;/user&gt;
    pl_size, ///< Physics-Lab font size: &lt;size=value&gt;...&lt;/size&gt;
    pl_voffset, ///< Physics-Lab vertical offset (Unity TMP rich text): &lt;voffset=value&gt;...&lt;/voffset&gt;
    pl_align, ///< Text alignment (Unity TMP rich text): &lt;align=value&gt;...&lt;/align&gt;
    pl_mark, ///< Physics-Lab mark (TMP rich text): &lt;mark=Xxx&gt;...&lt;/mark&gt;
    pl_margin, ///< Physics-Lab margin (Unity TMP rich text): &lt;margin left=v right=v&gt;...&lt;/margin&gt;
    pl_external, ///< Physics-Lab external link: &lt;external=url&gt;...&lt;/external&gt;
    pl_link, ///< Physics-Lab link (Unity TextMeshPro rich text): &lt;link=&quot;url&quot;&gt;...&lt;/link&gt;
    pl_trigger, ///< Physics-Lab trigger tag: &lt;trigger=value&gt;...&lt;/trigger&gt; (legacy NetLogo-style interaction
                ///< tag)
    pl_internal, ///< Physics-Lab internal tag: &lt;internal=value&gt;...&lt;/internal&gt; (rendered verbatim)

    // Text formatting (shared across Physics-Lab, HTML, and Markdown)
    pl_b, ///< Bold text: &lt;b&gt;...&lt;/b&gt;, Markdown double emphasis, &lt;strong&gt; in HTML
    pl_i, ///< Italic text: &lt;i&gt;...&lt;/i&gt;, Markdown single emphasis, &lt;em&gt; in HTML
    html_u, ///< Underline text: &lt;u&gt;...&lt;/u&gt; (Unity TextMeshPro rich text), &lt;u&gt; in HTML
    html_s, ///< Strikethrough text: &lt;s&gt;...&lt;/s&gt; (Unity TextMeshPro rich text), &lt;s&gt; in HTML

    // Physics-Lab specific macros
    pl_macro_project, // {Project}
    pl_macro_visitor, // {Visitor}
    pl_macro_author, // {Author}
    pl_macro_coauthors, // {CoAuthors}

    // HTML structural elements
    html_p, ///< Paragraph: &lt;p&gt;...&lt;/p&gt; (HTML), created by double newline (Markdown)
    html_br, ///< Line break: &lt;br&gt; (HTML)

    // HTML heading elements
    html_h1, ///< Level 1 heading: &lt;h1&gt;...&lt;/h1&gt;
    html_h2, ///< Level 2 heading: &lt;h2&gt;...&lt;/h2&gt;
    html_h3, ///< Level 3 heading: &lt;h3&gt;...&lt;/h3&gt;
    html_h4, ///< Level 4 heading: &lt;h4&gt;...&lt;/h4&gt;
    html_h5, ///< Level 5 heading: &lt;h5&gt;...&lt;/h5&gt;
    html_h6, ///< Level 6 heading: &lt;h6&gt;...&lt;/h6&gt;

    // HTML formatting elements
    html_del, ///< Deleted/strikethrough text: &lt;del&gt;...&lt;/del&gt;
    html_sup, ///< Superscript text: &lt;sup&gt;...&lt;/sup&gt;
    html_sub, ///< Subscript text: &lt;sub&gt;...&lt;/sub&gt;
    html_hr, ///< Horizontal rule: &lt;hr&gt; (HTML), --- (Markdown)
    html_note, ///< HTML comment/note (not rendered in output)
    html_em, ///< Emphasis: &lt;em&gt;...&lt;/em&gt; (HTML equivalent of &lt;i&gt;)
    html_strong, ///< Strong emphasis: &lt;strong&gt;...&lt;/strong&gt; (HTML equivalent of &lt;b&gt;)
    html_mark, ///< Marked/highlighted text: &lt;mark&gt;...&lt;/mark&gt;
    html_span,
    ///< HTML span with style attributes: &lt;span style="color:...;font-size:...;vertical-align:..."&gt;
    html_div,
    ///< HTML div with style attributes: &lt;div style="margin-left:...;margin-right:..."&gt;
    html_a, ///< HTML anchor/link: &lt;a href="URL"&gt;...&lt;/a&gt;

    // List elements (shared by HTML <ul>/<ol>/<li> and Markdown lists)
    list_ul, ///< Unordered list: &lt;ul&gt;...&lt;/ul&gt; or Markdown `- item`
    list_ol, ///< Ordered list: &lt;ol&gt;...&lt;/ol&gt; or Markdown `1. item`
    list_li, ///< List item: &lt;li&gt;...&lt;/li&gt; or Markdown list item
    list_li_checkbox, ///< Checkbox list item (Markdown - [ ] / - [x])

    // HTML code and quote elements
    html_code, ///< Inline code: &lt;code&gt;...&lt;/code&gt;
    html_blockquote, ///< Block quote: &lt;blockquote&gt;...&lt;/blockquote&gt;

    // Table elements (shared by HTML <table> and Markdown pipe tables)
    table, ///< Table: &lt;table&gt;...&lt;/table&gt; or Markdown pipe table
    table_tr, ///< Table row: &lt;tr&gt;...&lt;/tr&gt;
    table_td, ///< Table cell: &lt;td&gt;...&lt;/td&gt;
    table_th, ///< Table header cell: &lt;th&gt;...&lt;/th&gt;
    table_thead, ///< Table head: &lt;thead&gt;...&lt;/thead&gt;
    table_tbody, ///< Table body: &lt;tbody&gt;...&lt;/tbody&gt;
    table_tfoot, ///< Table foot: &lt;tfoot&gt;...&lt;/tfoot&gt;
    table_caption, ///< Table caption: &lt;caption&gt;...&lt;/caption&gt;
    table_colgroup, ///< Table column group: &lt;colgroup&gt;...&lt;/colgroup&gt;
    table_col, ///< Table column: &lt;col&gt; (self-closing)
    html_img, ///< HTML image: &lt;img src=&quot;...&quot; alt=&quot;...&quot;&gt;

    // Markdown ATX-style headers (# ## ### #### ##### ######)
    md_atx_h1, ///< Markdown level 1 heading: # Heading
    md_atx_h2, ///< Markdown level 2 heading: ## Heading
    md_atx_h3, ///< Markdown level 3 heading: ### Heading
    md_atx_h4, ///< Markdown level 4 heading: #### Heading
    md_atx_h5, ///< Markdown level 5 heading: ##### Heading
    md_atx_h6, ///< Markdown level 6 heading: ###### Heading

    // Markdown escape sequence (backslash-prefixed ASCII punctuation)
    md_escape,
    // Markdown thematic elements
    md_hr, ///< Thematic break/horizontal rule: ---, ***, ___

    // Markdown code blocks (fenced code)
    code_fence, ///< Code fence: ``` or ~~~

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
    // Markdown images
    md_image, ///< Markdown image: ![text](url)

    // Markdown block quotes and lists
    md_block_quotes, ///< Block quote: > quote text

    // LaTeX support
    md_latex_inline, ///< Inline LaTeX math: $...$
    md_latex_block, ///< Block/display LaTeX math: $$...$$
};

namespace details {

[[nodiscard]]
constexpr auto is_equal_sign_tag_type(::pltxt2htm::NodeKind const node_type) noexcept -> bool {
    return node_type == ::pltxt2htm::NodeKind::pl_color || node_type == ::pltxt2htm::NodeKind::pl_experiment ||
           node_type == ::pltxt2htm::NodeKind::pl_discussion || node_type == ::pltxt2htm::NodeKind::pl_experiments ||
           node_type == ::pltxt2htm::NodeKind::pl_discussions || node_type == ::pltxt2htm::NodeKind::pl_user ||
           node_type == ::pltxt2htm::NodeKind::pl_trigger || node_type == ::pltxt2htm::NodeKind::pl_internal;
}

[[nodiscard]]
constexpr auto is_em_like(::pltxt2htm::NodeKind const node_type) noexcept -> bool {
    return node_type == ::pltxt2htm::NodeKind::html_em || node_type == ::pltxt2htm::NodeKind::pl_i ||
           node_type == ::pltxt2htm::NodeKind::md_single_emphasis_asterisk ||
           node_type == ::pltxt2htm::NodeKind::md_single_emphasis_underscore ||
           node_type == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk ||
           node_type == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore;
}

[[nodiscard]]
constexpr auto is_strong_like(::pltxt2htm::NodeKind const node_type) noexcept -> bool {
    return node_type == ::pltxt2htm::NodeKind::html_strong || node_type == ::pltxt2htm::NodeKind::pl_b ||
           node_type == ::pltxt2htm::NodeKind::md_double_emphasis_asterisk ||
           node_type == ::pltxt2htm::NodeKind::md_double_emphasis_underscore ||
           node_type == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk ||
           node_type == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore;
}

/**
 * @brief Whether the node kind is a list/table frame containing inline-only text.
 */
[[nodiscard]]
constexpr auto is_inline_content_frame_kind(::pltxt2htm::NodeKind const node_type) noexcept -> bool {
    return node_type == ::pltxt2htm::NodeKind::list_li || node_type == ::pltxt2htm::NodeKind::list_li_checkbox ||
           node_type == ::pltxt2htm::NodeKind::table_caption || node_type == ::pltxt2htm::NodeKind::table_th ||
           node_type == ::pltxt2htm::NodeKind::table_td;
}

/**
 * @brief Whether the node kind is a URL-link container tag.
 * @details Auto-detected bare URLs inside these tags are suppressed to avoid
 *          nested anchors (e.g. an `<a>` inside `<link="url">...</link>`).
 */
[[nodiscard]]
constexpr auto is_url_link_tag_type(::pltxt2htm::NodeKind const node_type) noexcept -> bool {
    return node_type == ::pltxt2htm::NodeKind::pl_link || node_type == ::pltxt2htm::NodeKind::pl_external ||
           node_type == ::pltxt2htm::NodeKind::md_link || node_type == ::pltxt2htm::NodeKind::html_a;
}

} // namespace details

/**
 * @brief Table cell alignment
 */
enum class TableAlign : unsigned {
    left = 0,
    center,
    right,
};

/**
 * @brief Paragraph text alignment
 */
enum class TextAlign : unsigned {
    left = 0,
    center,
    right,
    justify,
};

} // namespace pltxt2htm
