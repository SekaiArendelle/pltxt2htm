#pragma once

#include <cstddef>

#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/inline_parser.hh>
#include <pltxt2htm/parser.hh>

#include "doctest_config.hh"

namespace {

constexpr auto inline_test_contracts = ::pltxt2htm::Contracts::quick_enforce;
using InlineTestAst = ::pltxt2htm::Ast<inline_test_contracts>;
using InlineTestKind = ::pltxt2htm::NodeKind;

// An input plus the node kind its frame must collapse into, either when the closing tag
// is reached or when the input ends with the frame still open.
struct InlineFrameCase {
    ::pltxt2htm::container::U8StringView pltext;
    InlineTestKind kind;
};

[[nodiscard]] auto parse_inline(::pltxt2htm::container::U8StringView pltext) -> InlineTestAst {
    return ::pltxt2htm::inline_parse_pltxt<inline_test_contracts>(pltext);
}

[[nodiscard]] auto parse_full(::pltxt2htm::container::U8StringView pltext) -> InlineTestAst {
    return ::pltxt2htm::parse_pltxt<inline_test_contracts>(pltext);
}

[[nodiscard]] auto contains_kind(InlineTestAst const& ast, InlineTestKind kind) -> bool {
    for (::std::size_t index{}; index < ast.size(); ++index) {
        if (ast.index(index).get_node_kind() == kind) {
            return true;
        }
    }
    return false;
}

void check_single_inline_frame(::pltxt2htm::container::U8StringView pltext, InlineTestKind expected_kind) {
    auto const ast = parse_inline(pltext);
    CAPTURE(pltext);
    CHECK(ast.size() == 1);
    if (ast.size() == 1) {
        CHECK(ast.index(0).get_node_kind() == expected_kind);
    }
}

// Block-level frames are created by the full parser, which hands their content to the
// shared inline parser, so the inline parser is what closes them.
void check_single_full_frame(::pltxt2htm::container::U8StringView pltext, InlineTestKind expected_kind) {
    auto const ast = parse_full(pltext);
    CAPTURE(pltext);
    CHECK(ast.size() == 1);
    if (ast.size() == 1) {
        CHECK(ast.index(0).get_node_kind() == expected_kind);
    }
}

// Inputs that must not open a tag frame have to survive as text-level nodes: both dropping
// the input and reinterpreting part of it as markup are failures.
void check_stays_literal(::pltxt2htm::container::U8StringView pltext) {
    auto const ast = parse_inline(pltext);
    CAPTURE(pltext);
    CHECK(ast.size() >= 1);
    for (::std::size_t index{}; index < ast.size(); ++index) {
        auto const kind = ast.index(index).get_node_kind();
        CAPTURE(index);
        switch (kind) {
        case InlineTestKind::text:
        case InlineTestKind::invalid_utf8:
        case InlineTestKind::line_break:
        case InlineTestKind::space:
        case InlineTestKind::tab:
        case InlineTestKind::ampersand:
        case InlineTestKind::single_quote:
        case InlineTestKind::double_quote:
        case InlineTestKind::less_than:
        case InlineTestKind::greater_than: {
            break;
        }
        // The list stays narrow on purpose: an auto-link or any tag frame here means the
        // input was reinterpreted as markup instead of staying literal.
        default: {
            CHECK_MESSAGE(false, "input produced a node that is not plain text");
            break;
        }
        }
    }
}

// Inline tags the inline parser opens itself, each closed by its own closing tag.
constexpr InlineFrameCase closed_inline_frame_cases[]{
    {u8"<a>x</a>", InlineTestKind::pl_a},
    {u8"<a href=\"https://example.com\">x</a>", InlineTestKind::html_a},
    {u8"<b>x</b>", InlineTestKind::unity_b},
    {u8"<code>x</code>", InlineTestKind::html_code},
    {u8"<color=red>x</color>", InlineTestKind::unity_color},
    {u8"<del>x</del>", InlineTestKind::html_del},
    {u8"<discussion=642cf37a494746375aae306a>x</discussion>", InlineTestKind::pl_discussion},
    {u8"<discussions=abc>x</discussions>", InlineTestKind::pl_discussions},
    {u8"<em>x</em>", InlineTestKind::html_em},
    {u8"<experiment=42>x</experiment>", InlineTestKind::pl_experiment},
    {u8"<experiments=abc>x</experiments>", InlineTestKind::pl_experiments},
    {u8"<external=https://example.com>x</external>", InlineTestKind::pl_external},
    {u8"<i>x</i>", InlineTestKind::unity_i},
    {u8"<internal=456>x</internal>", InlineTestKind::pl_internal},
    {u8"<link=\"https://example.com\">x</link>", InlineTestKind::unity_link},
    {u8"<mark>x</mark>", InlineTestKind::html_mark},
    {u8"<mark=#FF0000>x</mark>", InlineTestKind::unity_mark},
    {u8"<s>x</s>", InlineTestKind::html_s},
    {u8"<size=20>x</size>", InlineTestKind::unity_size},
    {u8"<span style=\"color:red\">x</span>", InlineTestKind::html_span},
    {u8"<strong>x</strong>", InlineTestKind::html_strong},
    {u8"<sub>x</sub>", InlineTestKind::html_sub},
    {u8"<sup>x</sup>", InlineTestKind::html_sup},
    {u8"<trigger=abc>x</trigger>", InlineTestKind::pl_trigger},
    {u8"<u>x</u>", InlineTestKind::html_u},
    {u8"<user=123>x</user>", InlineTestKind::pl_user},
    {u8"<voffset=5>x</voffset>", InlineTestKind::unity_voffset},
};

// The same frames left open: the trailing input must still close them.
constexpr InlineFrameCase unclosed_inline_frame_cases[]{
    {u8"<a>x", InlineTestKind::pl_a},
    {u8"<a href=\"https://example.com\">x", InlineTestKind::html_a},
    {u8"<b>x", InlineTestKind::unity_b},
    {u8"<code>x", InlineTestKind::html_code},
    {u8"<color=red>x", InlineTestKind::unity_color},
    {u8"<del>x", InlineTestKind::html_del},
    {u8"<discussion=642cf37a494746375aae306a>x", InlineTestKind::pl_discussion},
    {u8"<discussions=abc>x", InlineTestKind::pl_discussions},
    {u8"<em>x", InlineTestKind::html_em},
    {u8"<experiment=42>x", InlineTestKind::pl_experiment},
    {u8"<experiments=abc>x", InlineTestKind::pl_experiments},
    {u8"<external=https://example.com>x", InlineTestKind::pl_external},
    {u8"<i>x", InlineTestKind::unity_i},
    {u8"<internal=456>x", InlineTestKind::pl_internal},
    {u8"<link=\"https://example.com\">x", InlineTestKind::unity_link},
    {u8"<mark>x", InlineTestKind::html_mark},
    {u8"<mark=#FF0000>x", InlineTestKind::unity_mark},
    {u8"<s>x", InlineTestKind::html_s},
    {u8"<size=20>x", InlineTestKind::unity_size},
    {u8"<span style=\"color:red\">x", InlineTestKind::html_span},
    {u8"<sub>x", InlineTestKind::html_sub},
    {u8"<sup>x", InlineTestKind::html_sup},
    {u8"<trigger=abc>x", InlineTestKind::pl_trigger},
    {u8"<u>x", InlineTestKind::html_u},
    {u8"<user=123>x", InlineTestKind::pl_user},
    {u8"<voffset=5>x", InlineTestKind::unity_voffset},
};

// Inline constructs without tags: the inline parser is the only parser that produces them.
constexpr InlineFrameCase markdown_inline_frame_cases[]{
    {u8"*x*", InlineTestKind::md_single_emphasis_asterisk},
    {u8"**x**", InlineTestKind::md_double_emphasis_asterisk},
    {u8"***x***", InlineTestKind::md_triple_emphasis_asterisk},
    {u8"_x_", InlineTestKind::md_single_emphasis_underscore},
    {u8"__x__", InlineTestKind::md_double_emphasis_underscore},
    {u8"___x___", InlineTestKind::md_triple_emphasis_underscore},
    {u8"~~x~~", InlineTestKind::md_del},
    {u8"`x`", InlineTestKind::md_code_span_1_backtick},
    {u8"``x``", InlineTestKind::md_code_span_2_backtick},
    {u8"```x```", InlineTestKind::md_code_span_3_backtick},
    {u8"$x$", InlineTestKind::md_latex_inline},
    {u8"$$x$$", InlineTestKind::md_latex_block},
    {u8"[t](https://example.com)", InlineTestKind::md_link},
    {u8"![t](https://example.com/a.png)", InlineTestKind::md_image},
    {u8"https://example.com", InlineTestKind::url},
    {u8"{project}", InlineTestKind::pl_macro_project},
    {u8"{visitor}", InlineTestKind::pl_macro_visitor},
    {u8"{author}", InlineTestKind::pl_macro_author},
    {u8"{coauthors}", InlineTestKind::pl_macro_coauthors},
};

// Tags that carry no content of their own.
constexpr InlineFrameCase standalone_inline_frame_cases[]{
    {u8"<br>", InlineTestKind::html_br},
    {u8"<br/>", InlineTestKind::html_br},
    {u8"<img src=\"https://example.com/a.png\" alt=\"a\">", InlineTestKind::html_img},
    {u8"<!-- note -->", InlineTestKind::html_note},
};

// A closing tag that names no open frame must survive as literal text, and the frame it
// failed to close is still closed at the end of the input.
constexpr InlineFrameCase mismatched_closing_tag_cases[]{
    {u8"<a>x</q>", InlineTestKind::pl_a},
    {u8"<a href=\"https://example.com\">x</q>", InlineTestKind::html_a},
    {u8"<b>x</q>", InlineTestKind::unity_b},
    {u8"<code>x</q>", InlineTestKind::html_code},
    {u8"<color=red>x</q>", InlineTestKind::unity_color},
    {u8"<del>x</q>", InlineTestKind::html_del},
    {u8"<discussion=642cf37a494746375aae306a>x</q>", InlineTestKind::pl_discussion},
    {u8"<discussions=abc>x</q>", InlineTestKind::pl_discussions},
    {u8"<em>x</q>", InlineTestKind::html_em},
    {u8"<experiment=42>x</q>", InlineTestKind::pl_experiment},
    {u8"<experiments=abc>x</q>", InlineTestKind::pl_experiments},
    {u8"<external=https://example.com>x</q>", InlineTestKind::pl_external},
    {u8"<i>x</q>", InlineTestKind::unity_i},
    {u8"<internal=456>x</q>", InlineTestKind::pl_internal},
    {u8"<link=\"https://example.com\">x</q>", InlineTestKind::unity_link},
    {u8"<mark>x</q>", InlineTestKind::html_mark},
    {u8"<mark=#FF0000>x</q>", InlineTestKind::unity_mark},
    {u8"<s>x</q>", InlineTestKind::html_s},
    {u8"<size=20>x</q>", InlineTestKind::unity_size},
    {u8"<span style=\"color:red\">x</q>", InlineTestKind::html_span},
    // A failed </strong> leaves the frame open, so it is attached at end of input like the
    // unclosed <strong> case. That attach path remaps html_strong to unity_b, while an
    // explicit </strong> yields html_strong; the two kinds differ only in the AST and no
    // backend renders them differently, so this row pins a known anomaly, not an invariant.
    {u8"<strong>x</q>", InlineTestKind::unity_b},
    {u8"<sub>x</q>", InlineTestKind::html_sub},
    {u8"<sup>x</q>", InlineTestKind::html_sup},
    {u8"<trigger=abc>x</q>", InlineTestKind::pl_trigger},
    {u8"<u>x</q>", InlineTestKind::html_u},
    {u8"<user=123>x</q>", InlineTestKind::pl_user},
    {u8"<voffset=5>x</q>", InlineTestKind::unity_voffset},
};

// `<` that starts no known tag must survive as literal text instead of being dropped.
constexpr ::pltxt2htm::container::U8StringView literal_fallback_cases[]{
    u8"<", // nothing follows the '<'
    u8"a<", // the '<' ends the input
    u8"<9", // no tag name starts with a digit
    u8"<z", // no tag name starts with 'z'
    u8"</z", // closing tag for a frame that was never opened
    u8"<!x", // '!' that does not start a comment
    u8"<b", // unterminated tag
    u8"<a ", // unterminated html <a>
    u8"<ez", // 'e' form whose name matches none of the 'e' tags
    u8"<size=0>x</size>", // a zero font size is not a size tag
    u8"<user=", // tag without a value
    u8"<v", // unknown-for-this-input tag letter
    u8"<mark=", // mark without a value
    u8"<size=", // size without a value
    u8"<sup", // unterminated <sup>
    u8"<div", // block-level tag in inline context
};

// A recognized opening tag whose URL is rejected consumes the whole span as literal text.
constexpr ::pltxt2htm::container::U8StringView invalid_url_span_cases[]{
    u8"<a href=\"javascript:alert(1)\">x</a>",
    u8"<external=javascript:alert(1)>x</external>",
    u8"<link=\"javascript:alert(1)\">x</link>",
};

} // namespace

TEST_SUITE("inline_parser") {
    TEST_CASE("closed-inline-tag-yields-one-frame") {
        for (auto const& test_case : closed_inline_frame_cases) {
            check_single_inline_frame(test_case.pltext, test_case.kind);
        }
    }

    TEST_CASE("unclosed-inline-tag-is-closed-at-end-of-input") {
        for (auto const& test_case : unclosed_inline_frame_cases) {
            check_single_inline_frame(test_case.pltext, test_case.kind);
        }
    }

    TEST_CASE("markdown-inline-construct-yields-one-frame") {
        for (auto const& test_case : markdown_inline_frame_cases) {
            check_single_inline_frame(test_case.pltext, test_case.kind);
        }
    }

    TEST_CASE("standalone-inline-tag-yields-one-frame") {
        for (auto const& test_case : standalone_inline_frame_cases) {
            check_single_inline_frame(test_case.pltext, test_case.kind);
        }
    }

    TEST_CASE("mismatched-closing-tag-stays-literal") {
        for (auto const& test_case : mismatched_closing_tag_cases) {
            check_single_inline_frame(test_case.pltext, test_case.kind);
        }
    }

    TEST_CASE("tag-closed-by-its-legacy-alternative-closing-tag") {
        // <color> and <a> historically accept either closing tag, so a frame is closed by
        // whichever of the two appears.
        check_single_inline_frame(u8"<color=red>x</a>", InlineTestKind::unity_color);
        check_single_inline_frame(u8"<a>x</color>", InlineTestKind::pl_a);
    }

    TEST_CASE("nested-inline-frame-closes-into-its-parent") {
        auto const html = ::pltxt2htm_test::pltxt2common_htmld(u8"<b><i>x</i></b>");
        auto const& answer = u8"<strong><em>x</em></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-strong-tag-renders-like-a-closed-one") {
        // At end of input the html_strong frame is attached as a unity_b node: the two kinds
        // differ only in the AST and every backend renders them alike, so only the rendered
        // output is pinned here. This assertion survives normalizing that attach path.
        auto const html = ::pltxt2htm_test::pltxt2common_htmld(u8"<strong>x");
        auto const& answer = u8"<strong>x</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("literal-fallback-keeps-its-bytes") {
        // The stray angle bracket and the text around it must reach the output escaped,
        // rather than being dropped or turned into a tag.
        CHECK(::pltxt2htm_test::pltxt2common_htmld(u8"<") == u8"&lt;");
        CHECK(::pltxt2htm_test::pltxt2common_htmld(u8"<z") == u8"&lt;z");
        CHECK(::pltxt2htm_test::pltxt2common_htmld(u8"</z") == u8"&lt;/z");
        CHECK(::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"javascript:alert(1)\">x</a>") ==
              u8"&lt;a&nbsp;href=&quot;javascript:alert(1)&quot;&gt;x&lt;/a&gt;");
    }

    TEST_CASE("angle-bracket-without-a-known-tag-stays-literal") {
        for (auto const pltext : literal_fallback_cases) {
            check_stays_literal(pltext);
        }
    }

    TEST_CASE("invalid-url-span-stays-literal") {
        for (auto const pltext : invalid_url_span_cases) {
            check_stays_literal(pltext);
        }
    }

    TEST_CASE("block-level-frame-is-closed-by-the-inline-parser") {
        check_single_full_frame(u8"<p>x</p>", InlineTestKind::html_p);
        check_single_full_frame(u8"<h1>x</h1>", InlineTestKind::html_h1);
        check_single_full_frame(u8"<h2>x</h2>", InlineTestKind::html_h2);
        check_single_full_frame(u8"<h3>x</h3>", InlineTestKind::html_h3);
        check_single_full_frame(u8"<h4>x</h4>", InlineTestKind::html_h4);
        check_single_full_frame(u8"<h5>x</h5>", InlineTestKind::html_h5);
        check_single_full_frame(u8"<h6>x</h6>", InlineTestKind::html_h6);
        check_single_full_frame(u8"<blockquote>x</blockquote>", InlineTestKind::html_blockquote);
        check_single_full_frame(u8"<div style=\"margin-left:2em\">x</div>", InlineTestKind::html_div);
        check_single_full_frame(u8"<align=center>x</align>", InlineTestKind::unity_align);
        check_single_full_frame(u8"<margin left=2em>x</margin>", InlineTestKind::unity_margin);
    }

    TEST_CASE("code-span-content-must-be-non-empty-and-single-line") {
        // An empty code span is rejected one layer down, by try_parse_md_code_span, so these
        // inputs never reach the inline parser's own content test; what is pinned here is the
        // contract that an empty or multi-line code span is never a code-span node.
        auto const empty_span = parse_inline(u8"``");
        CHECK(contains_kind(empty_span, InlineTestKind::md_code_span_1_backtick) == false);
        CHECK(contains_kind(empty_span, InlineTestKind::md_code_span_2_backtick) == false);
        CHECK(contains_kind(empty_span, InlineTestKind::md_code_span_3_backtick) == false);

        // Paired delimiters with nothing between them are not a code span either.
        auto const paired_empty_span = parse_inline(u8"````");
        CHECK(contains_kind(paired_empty_span, InlineTestKind::md_code_span_2_backtick) == false);

        auto const multi_line_span = parse_inline(u8"`a\nb`");
        CHECK(contains_kind(multi_line_span, InlineTestKind::md_code_span_1_backtick) == false);
    }

    TEST_CASE("auto-link-is-suppressed-inside-a-url-link-frame") {
        // A bare URL inside a URL-link frame must stay text: nesting <a> inside <a> is the
        // regression being pinned. The suppression guard leaves the root node kind alone, so
        // the frame's subast is what has to be inspected for a url node.
        auto link_ast = parse_inline(u8"<link=\"https://example.com\">https://example.com</link>");
        REQUIRE(link_ast.size() == 1);
        CHECK(link_ast.index(0).get_node_kind() == InlineTestKind::unity_link);
        CHECK(contains_kind(link_ast.index(0).as_unity_link().get_subast(), InlineTestKind::url) == false);

        auto anchor_ast = parse_inline(u8"<a href=\"https://example.com\">https://example.com</a>");
        REQUIRE(anchor_ast.size() == 1);
        CHECK(anchor_ast.index(0).get_node_kind() == InlineTestKind::html_a);
        CHECK(contains_kind(anchor_ast.index(0).as_html_a().get_subast(), InlineTestKind::url) == false);
    }
}
