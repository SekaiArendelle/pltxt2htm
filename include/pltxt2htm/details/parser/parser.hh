/**
 * @file parser.hh
 * @brief Main pl-text parser - converts raw pl-text into an AST.
 * @details Provides `find_next_block_after_line_break` and `parse_pltxt`
 *          templates that drive the recursive-descent (goto-based) parsing
 *          of pl-text / Markdown content into ::pltxt2htm::Ast<ndebug>.
 */
#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include "../../container/string.hh"
#include "../call_stack.hh"
#include "../../container/string_view.hh"
#include "../../container/optional.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../inline_parser.hh"
#include "list_ast.hh"
#include "md_list.hh"
#include "html_list.hh"
#include "md_table.hh"
#include "html_table.hh"
#include "frame_context.hh"
#include "non_nestable_tag.hh"
#include "try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Return type of find_next_block_after_line_break.
 */
struct FindNextBlockAfterLineBreakResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
    bool new_frame_been_pushed_into_call_stack; ///< Whether a new frame was pushed.
};

/**
 * @brief Scan forward from the current position to find and push the next block-level frame.
 * @tparam ndebug Contract checking mode.
 * @param pltext Remaining input text to scan.
 * @param call_stack Active parser call stack.
 * @param result AST being built.
 * @return How many characters were consumed and whether a new frame was created.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto find_next_block_after_line_break(::pltxt2htm::container::U8StringView pltext,
                                                ::pltxt2htm::details::CallStack<ParserFrame<ndebug>>& call_stack,
                                                ::pltxt2htm::Ast<ndebug>& result) noexcept
    -> FindNextBlockAfterLineBreakResult {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    while (true) {
        if (current_index >= pltext_size) {
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = false};
        }

        if (auto opt_md_atx_heading =
                ::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_md_atx_heading.has_value()) {
            auto&& [start_index, sublength, advance_count, md_atx_heading_type] =
                opt_md_atx_heading.template value<ndebug>();
            if (sublength != 0) {
                auto const subtext = pltext.template subview<ndebug>(current_index + start_index, sublength);
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{subtext}, md_atx_heading_type},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count;
                return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                         .new_frame_been_pushed_into_call_stack = true};
            }

            switch (md_atx_heading_type) {
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH1<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH2<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH3<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH4<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH5<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH6<ndebug>{::pltxt2htm::Ast<ndebug>{}}));
                break;
            }
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected heading node kind");
                }
            }
            current_index += advance_count;
            continue;
        }
        if (auto opt_len = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                pltext.template subview<ndebug>(current_index));
            opt_len.has_value()) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdHr{}));
            current_index += opt_len.template value<ndebug>().template get<ndebug>();
            continue;
        }
        // Check for HTML <hr> self-closing tag at block position. Treating <hr> as a
        // block-level element lets the scan continue after it, so a following <h1>/<p>/
        // <align> on the same line is recognized as a block (same as after a line break).
        if (auto opt_hr_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"<hr">(
                pltext.template subview<ndebug>(current_index));
            opt_hr_tag_len.has_value()) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlHr{}));
            current_index += opt_hr_tag_len.template value<ndebug>().template get<ndebug>();
            continue;
        }
        if (auto opt_code_fence =
                ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_code_fence.has_value()) {
            auto&& [node, advance_count] = opt_code_fence.template value<ndebug>();
            result.push_back(::std::move(node));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = false};
        }
        // Check for an HTML <pre><code>...</code></pre> code block at a block position.
        if (auto opt_pre_code_block = ::pltxt2htm::details::try_parse_html_pre_code_block<ndebug>(
                pltext.template subview<ndebug>(current_index));
            opt_pre_code_block.has_value()) {
            auto&& [node, advance_count] = opt_pre_code_block.template value<ndebug>();
            result.push_back(::std::move(node));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = false};
        }
        if (auto opt_block_quote =
                ::pltxt2htm::details::try_parse_md_block_quotes<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_block_quote.has_value()) {
            auto&& [advance_count, subpltext] = opt_block_quote.template value<ndebug>();
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{ParserFrameContextWithMdBlockQuotesInfo{::std::move(subpltext)},
                                               ::pltxt2htm::NodeKind::md_block_quotes},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_md_list_ast =
                ::pltxt2htm::details::optionally_to_md_list_ast<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_md_list_ast.has_value()) {
            auto&& [top_node, advance_count] = opt_md_list_ast.template value<ndebug>();
            ::pltxt2htm::details::push_list_frame<ndebug>(call_stack, ::std::move(top_node));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <ul>/<ol> list at a block position (block-level lists).
        if (auto opt_html_list_ast = ::pltxt2htm::details::optionally_to_html_list_ast<ndebug>(
                pltext.template subview<ndebug>(current_index));
            opt_html_list_ast.has_value()) {
            auto&& [top_node, advance_count] = opt_html_list_ast.template value<ndebug>();
            ::pltxt2htm::details::push_list_frame<ndebug>(call_stack, ::std::move(top_node));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_md_table_raw =
                ::pltxt2htm::details::try_parse_md_table_raw<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_md_table_raw.has_value()) {
            auto&& [raw_ast, advance_count] = opt_md_table_raw.template value<ndebug>();
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{ParserFrameContextWithTableInfo<ndebug>{::std::move(raw_ast)},
                                               ::pltxt2htm::NodeKind::table},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for an HTML <table> block at a block position (block-level tables).
        if (auto opt_html_table_ast = ::pltxt2htm::details::optionally_to_html_table_ast<ndebug>(
                pltext.template subview<ndebug>(current_index));
            opt_html_table_ast.has_value()) {
            auto&& [raw_ast, advance_count] = opt_html_table_ast.template value<ndebug>();
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{ParserFrameContextWithTableInfo<ndebug>{::std::move(raw_ast)},
                                               ::pltxt2htm::NodeKind::table},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index + advance_count,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <p> tag at line start
        if (auto opt_p_tag =
                ::pltxt2htm::details::try_parse_p_tag<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_p_tag.has_value()) {
            auto const [tag_len, align] = opt_p_tag.template value<ndebug>();
            current_index += tag_len + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithAlignInfo{pltext.template subview<ndebug>(current_index), align},
                    ::pltxt2htm::NodeKind::html_p},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for Unity TMP <align=...> tag at line start
        if (auto opt_align_tag =
                ::pltxt2htm::details::try_parse_align_tag<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_align_tag.has_value()) {
            auto const [tag_len, align] = opt_align_tag.template value<ndebug>();
            current_index += tag_len + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithAlignInfo{pltext.template subview<ndebug>(current_index), align},
                    ::pltxt2htm::NodeKind::pl_align},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for Unity TMP <margin...> tag (attribute form or legacy single-value form) at line start
        if (auto opt_margin_tag =
                ::pltxt2htm::details::try_parse_margin_tag<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_margin_tag.has_value()) {
            auto const [tag_len, left, right] = opt_margin_tag.template value<ndebug>();
            current_index += tag_len + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPlMarginTagInfo{pltext.template subview<ndebug>(current_index), left, right},
                    ::pltxt2htm::NodeKind::pl_margin},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <div style="margin-left:...;margin-right:..."> tag at line start
        if (auto opt_div_tag =
                ::pltxt2htm::details::try_parse_html_div_tag<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_div_tag.has_value()) {
            auto const [tag_len, left, right] = opt_div_tag.template value<ndebug>();
            current_index += tag_len + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithHtmlDivInfo{pltext.template subview<ndebug>(current_index), left, right},
                    ::pltxt2htm::NodeKind::html_div},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h1> tag at line start
        if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h1">(
                pltext.template subview<ndebug>(current_index));
            opt_h1_tag_len.has_value()) {
            current_index += opt_h1_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h1},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h2> tag at line start
        if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h2">(
                pltext.template subview<ndebug>(current_index));
            opt_h2_tag_len.has_value()) {
            current_index += opt_h2_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h2},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h3> tag at line start
        if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h3">(
                pltext.template subview<ndebug>(current_index));
            opt_h3_tag_len.has_value()) {
            current_index += opt_h3_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h3},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h4> tag at line start
        if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h4">(
                pltext.template subview<ndebug>(current_index));
            opt_h4_tag_len.has_value()) {
            current_index += opt_h4_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h4},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h5> tag at line start
        if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h5">(
                pltext.template subview<ndebug>(current_index));
            opt_h5_tag_len.has_value()) {
            current_index += opt_h5_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h5},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <h6> tag at line start
        if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h6">(
                pltext.template subview<ndebug>(current_index));
            opt_h6_tag_len.has_value()) {
            current_index += opt_h6_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_h6},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <blockquote> tag at line start
        if (auto opt_blockquote_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<blockquote">(
                pltext.template subview<ndebug>(current_index));
            opt_blockquote_tag_len.has_value()) {
            current_index += opt_blockquote_tag_len.template value<ndebug>() + 1;
            call_stack.push_frame(ParserFrame<ndebug>(
                FrontendContextVariant<ndebug>{
                    ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                    ::pltxt2htm::NodeKind::html_blockquote},
                ::pltxt2htm::Ast<ndebug>{}));
            return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                     .new_frame_been_pushed_into_call_stack = true};
        }
        return FindNextBlockAfterLineBreakResult{.advance_count = current_index,
                                                 .new_frame_been_pushed_into_call_stack = false};
    }
}

/**
 * @brief Return type of parse_pltxt.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
struct ParsePlTxtResult {
    ::pltxt2htm::Ast<ndebug> subast; ///< Parsed AST for the bottom frame.
    ///< Bytes consumed from the bottom frame's pltext (the caller uses it to advance past html_p blocks).
    ::std::size_t consumed_bytes{};
};

/**
 * @brief Parse pl-text to nodes.
 * @tparam ndebug Contract checking mode; `::pltxt2htm::Contracts::ignore` disables checks.
 * @param call_stack: use `call_stack` + `goto entry` to avoid stack overflow.
 * @return The parsed ast and how many bytes of the bottom frame's pltext were consumed.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(::pltxt2htm::details::CallStack<ParserFrame<ndebug>>& call_stack) noexcept
    -> ParsePlTxtResult<ndebug> {
entry:
    while (true) {
        if (auto const nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
            nested_tag_type == ::pltxt2htm::NodeKind::list_ul || nested_tag_type == ::pltxt2htm::NodeKind::list_ol) {
            // ListAst to ::pltxt2htm::Ast<ndebug>
            auto&& frame = call_stack.template current_frame<ndebug>();
            auto&& frame_list_ast = frame.as_list_info().list_ast;
            auto&& frame_iter = frame.as_list_info().iter;
            if (frame_iter == frame_list_ast.end()) {
                ParserFrame<ndebug> previous_frame(::std::move(frame));
                call_stack.template discard_current_frame<ndebug>();
                if (call_stack.empty()) {
                    return ParsePlTxtResult<ndebug>{.subast = ::std::move(previous_frame.subast)};
                }
                if (nested_tag_type == ::pltxt2htm::NodeKind::list_ul) {
                    call_stack.template current_frame<ndebug>().subast.emplace_back(::pltxt2htm::PlTxtNode<ndebug>(
                        ::pltxt2htm::ListUl<ndebug>{::std::move(previous_frame.subast)}));
                }
                else {
                    call_stack.template current_frame<ndebug>().subast.emplace_back(
                        ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListOl<ndebug>{
                            ::std::move(previous_frame.subast), previous_frame.as_list_info().list_start}));
                }
                // Given "before\n- item\nbetween\n+ another\n- last\nafter":
                //   find_next_block_after_line_break first sees "- item\nbetween\n...",
                //   but optionally_to_md_list_ast stops at "between" (not a list marker),
                //   returning only "- item\n".  After that list pops, the remaining text
                //   "between\n+ another\n- last\nafter" is processed by the parent loop.
                //   When the parent hits "\n" before "+ another",
                //   find_next_block_after_line_break is called again and sees
                //   "+ another\n- last\nafter".  optionally_to_md_list_ast stops at
                //   "- last" (different marker at root level), returning only
                //   "+ another\n".  After that list pops, the parent has unconsumed
                //   text "- last\nafter".  Without re-scanning here, "- last" would be
                //   treated as plain text "--last" instead of a new list.
                //
                // When the parent is another list (child nested inside a parent list):
                // no re-scan needed -- the parent iterates its own AST.
                auto&& parent_frame = call_stack.template current_frame<ndebug>();
                if (parent_frame.get_nested_tag_type() != ::pltxt2htm::NodeKind::list_ul &&
                    parent_frame.get_nested_tag_type() != ::pltxt2htm::NodeKind::list_ol) {
                    ::std::size_t& parent_index = parent_frame.current_index;
                    auto const parent_text = parent_frame.get_pltext();
                    auto&& [fwd, restart] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                        parent_text.template subview<ndebug>(parent_index), call_stack, parent_frame.subast);
                    parent_index += fwd;
                    if (restart) {
                        goto entry;
                    }
                }
                goto entry;
            }
            switch (frame_iter->get_type()) {
            case ListNodeType::list_li: {
                auto const text = frame_iter->get_text_view();
                ++frame_iter;
                call_stack.push_frame(
                    ParserFrame<ndebug>(FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{text},
                                                                       ::pltxt2htm::NodeKind::list_li},
                                        ::pltxt2htm::Ast<ndebug>{}));
                ::pltxt2htm::details::inline_parse_pltxt<ndebug>(call_stack);
                goto entry;
            }
            case ListNodeType::list_li_checkbox: {
                auto const text = frame_iter->get_text_view();
                auto const checked = frame_iter->is_checked();
                ++frame_iter;
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithListLiCheckboxInfo{text, checked},
                                                   ::pltxt2htm::NodeKind::list_li_checkbox},
                    ::pltxt2htm::Ast<ndebug>{}));
                ::pltxt2htm::details::inline_parse_pltxt<ndebug>(call_stack);
                goto entry;
            }
            case ListNodeType::list_ul: {
                auto sublist = ::std::move(frame_iter->get_sublist());
                ++frame_iter;
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithListInfo<ndebug>{::std::move(sublist)},
                                                   ::pltxt2htm::NodeKind::list_ul},
                    ::pltxt2htm::Ast<ndebug>{}));
                goto entry;
            }
            case ListNodeType::list_ol: {
                auto sublist = ::std::move(frame_iter->get_sublist());
                auto const start = frame_iter->get_start();
                ++frame_iter;
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithListInfo<ndebug>{::std::move(sublist), start},
                                                   ::pltxt2htm::NodeKind::list_ol},
                    ::pltxt2htm::Ast<ndebug>{}));
                goto entry;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected ListNodeType");
                }
#endif
            }
            pltxt2htm_unreachable(u8"Unreachable after ListNodeType switch");
        }
        if (auto const nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
            nested_tag_type == ::pltxt2htm::NodeKind::table) {
            ::std::size_t const previous_stack_size{call_stack.frame_count()};
            auto opt_table_ast = ::pltxt2htm::details::process_table_frame<ndebug>(call_stack);
            if (opt_table_ast.has_value()) {
                auto&& table_ast = opt_table_ast.template value<ndebug>();
                return ParsePlTxtResult<ndebug>{.subast = ::std::move(table_ast)};
            }
            if (call_stack.frame_count() > previous_stack_size) {
                ::pltxt2htm::details::inline_parse_pltxt<ndebug>(call_stack);
            }
            goto entry;
        }

        auto&& top_frame = call_stack.template current_frame<ndebug>();
        auto&& current_index = top_frame.current_index;
        ::pltxt2htm::container::U8StringView const pltext{top_frame.get_pltext()};
        auto&& result = top_frame.subast;
        ::std::size_t const pltext_size{pltext.size()};
        auto const nested_tag_type = top_frame.get_nested_tag_type();

        if ((nested_tag_type == ::pltxt2htm::NodeKind::md_block_quotes ||
             nested_tag_type == ::pltxt2htm::NodeKind::pl_margin ||
             nested_tag_type == ::pltxt2htm::NodeKind::html_div ||
             nested_tag_type == ::pltxt2htm::NodeKind::html_blockquote) &&
            current_index == 0) {
            // https://spec.commonmark.org/0.31.2/#example-228
            // to support parsing md-atx-heading e.t.c inside md-block-quotes
            // (and nested <margin...>/<div...>/<blockquote> blocks right after the opening tag)
            auto&& [advance_count, require_restart] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                pltext.template subview<ndebug>(current_index), call_stack, result);
            current_index += advance_count;
            if (require_restart) {
                goto entry;
            }
        }

        while (current_index < pltext_size) {
            char8_t const chr{pltext.template index<ndebug>(current_index)};

            if (chr == u8'\n') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));

                auto&& [advance_count, require_restart] =
                    ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                        pltext.template subview<ndebug>(current_index + 1), call_stack, result);
                current_index += advance_count;
                if (require_restart) {
                    current_index += 1;
                    goto entry;
                }
                ++current_index;
                continue;
            }
            if (auto const opt_space_size =
                    ::pltxt2htm::details::try_parse_space<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_space_size.has_value()) {
                auto const space_size = opt_space_size.template value<ndebug>().template get<ndebug>();
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
                current_index += space_size;
                continue;
            }
            if (chr == u8'&') {
                if (auto const opt_entity_len = ::pltxt2htm::details::try_parse_entity_reference<ndebug>(
                        pltext.template subview<ndebug>(current_index));
                    opt_entity_len.has_value()) {
                    auto const entity_len = opt_entity_len.template value<ndebug>().template get<ndebug>();
                    result.push_back(
                        ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::EntityReference{::pltxt2htm::container::U8String{
                            pltext.data() + current_index + 1, pltext.data() + current_index + entity_len - 1}}));
                    current_index += entity_len;
                    continue;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
                ++current_index;
                continue;
            }
            if (chr == u8'\'') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
                ++current_index;
                continue;
            }
            if (chr == u8'\"') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
                ++current_index;
                continue;
            }
            if (chr == u8'>') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
                ++current_index;
                continue;
            }
            if (chr == u8'\t') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
                ++current_index;
                continue;
            }
            if (auto opt_escape =
                    ::pltxt2htm::details::try_parse_md_escape<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_escape.has_value()) {
                auto&& [node, advance_count] = opt_escape.template value<ndebug>();
                result.push_back(::std::move(node));
                current_index += advance_count;
                continue;
            }
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{project}">(
                    pltext.template subview<ndebug>(current_index))) {
                constexpr auto length_of_literal_string = ::std::size_t{9};
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroProject{}));
                current_index += length_of_literal_string;
                continue;
            }
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{visitor}">(
                    pltext.template subview<ndebug>(current_index))) {
                constexpr auto length_of_literal_string = ::std::size_t{9};
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroVisitor{}));
                current_index += length_of_literal_string;
                continue;
            }
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{author}">(
                    pltext.template subview<ndebug>(current_index))) {
                constexpr auto length_of_literal_string = ::std::size_t{8};
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroAuthor{}));
                current_index += length_of_literal_string;
                continue;
            }
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{coauthors}">(
                    pltext.template subview<ndebug>(current_index))) {
                constexpr auto length_of_literal_string = ::std::size_t{11};
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroCoauthors{}));
                current_index += length_of_literal_string;
                continue;
            }
            if (auto opt_triple_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"***">(
                    pltext.template subview<ndebug>(current_index));
                opt_triple_emphasis_asterisk.has_value()) {
                // parsing markdown ***example***
                ::std::size_t const advance_count{
                    opt_triple_emphasis_asterisk.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 3, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 6;
                goto entry;
            }
            if (auto opt_double_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"**">(
                    pltext.template subview<ndebug>(current_index));
                opt_double_emphasis_asterisk.has_value()) {
                // parsing markdown **example**
                ::std::size_t const advance_count{
                    opt_double_emphasis_asterisk.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 2, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_double_emphasis_asterisk},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 4;
                goto entry;
            }
            if (auto opt_single_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"*">(
                    pltext.template subview<ndebug>(current_index));
                opt_single_emphasis_asterisk.has_value()) {
                // parsing markdown *example*
                ::std::size_t const advance_count{
                    opt_single_emphasis_asterisk.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 1, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_single_emphasis_asterisk},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 2;
                goto entry;
            }
            if (auto opt_triple_emphasis_underscore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"___">(
                    pltext.template subview<ndebug>(current_index));
                opt_triple_emphasis_underscore.has_value()) {
                // parsing markdown ___example___
                ::std::size_t const advance_count{
                    opt_triple_emphasis_underscore.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 3, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_triple_emphasis_underscore},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 6;
                goto entry;
            }
            if (auto opt_double_emphasis_undersore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"__">(
                    pltext.template subview<ndebug>(current_index));
                opt_double_emphasis_undersore.has_value()) {
                // parsing markdown __example__
                ::std::size_t const advance_count{
                    opt_double_emphasis_undersore.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 2, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_double_emphasis_underscore},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 4;
                goto entry;
            }
            if (auto opt_single_emphasis_undersore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"_">(
                    pltext.template subview<ndebug>(current_index));
                opt_single_emphasis_undersore.has_value()) {
                // parsing markdown _example_
                ::std::size_t const advance_count{
                    opt_single_emphasis_undersore.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 1, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_single_emphasis_underscore},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 2;
                goto entry;
            }
            if (auto opt_md_del = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"~~">(
                    pltext.template subview<ndebug>(current_index));
                opt_md_del.has_value()) {
                // parsing markdown ~~example~~
                ::std::size_t const advance_count{opt_md_del.template value<ndebug>().template get<ndebug>()};
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(
                                                       current_index + 2, advance_count)},
                                                   ::pltxt2htm::NodeKind::md_del},
                    ::pltxt2htm::Ast<ndebug>{}));
                current_index += advance_count + 4;
                goto entry;
            }
            if (auto opt_code_span_3_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"```">(
                    pltext.template subview<ndebug>(current_index));
                opt_code_span_3_backtick.has_value()) {
                // parsing markdown ```example```
                auto&& [content_size, subast] = opt_code_span_3_backtick.template value<ndebug>();
                current_index += content_size + 6;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan3Backtick<ndebug>{::std::move(subast)}));
                continue;
            }
            if (auto opt_code_span_2_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"``">(
                    pltext.template subview<ndebug>(current_index));
                opt_code_span_2_backtick.has_value()) {
                // parsing markdown ``example``
                auto&& [content_size, subast] = opt_code_span_2_backtick.template value<ndebug>();
                current_index += content_size + 4;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan2Backtick<ndebug>{::std::move(subast)}));
                continue;
            }
            if (auto opt_code_span_1_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"`">(
                    pltext.template subview<ndebug>(current_index));
                opt_code_span_1_backtick.has_value()) {
                // parsing markdown `example`
                auto&& [content_size, subast] = opt_code_span_1_backtick.template value<ndebug>();
                current_index += content_size + 2;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan1Backtick<ndebug>{::std::move(subast)}));
                continue;
            }
            if (auto opt_md_latex_block_dollar = ::pltxt2htm::details::try_parse_md_latex_block_dollar<ndebug>(
                    pltext.template subview<ndebug>(current_index));
                opt_md_latex_block_dollar.has_value()) {
                auto&& [advance_count, subast] = opt_md_latex_block_dollar.template value<ndebug>();
                current_index += advance_count;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexBlock<ndebug>{::std::move(subast)}));
                continue;
            }
            if (auto opt_md_latex_inline = ::pltxt2htm::details::try_parse_md_latex_inline<ndebug>(
                    pltext.template subview<ndebug>(current_index));
                opt_md_latex_inline.has_value()) {
                auto&& [advance_count, subast] = opt_md_latex_inline.template value<ndebug>();
                current_index += advance_count;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexInline<ndebug>{::std::move(subast)}));
                continue;
            }
            if (auto opt_md_link =
                    ::pltxt2htm::details::try_parse_md_link<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_md_link.has_value()) {
                auto&& [advance_count, url_text, url_link] = opt_md_link.template value<ndebug>();
                current_index += advance_count;
                call_stack.push_frame(ParserFrame<ndebug>(
                    FrontendContextVariant<ndebug>{ParserFrameContextWithUrlInfo{url_text, ::std::move(url_link)},
                                                   ::pltxt2htm::NodeKind::md_link},
                    ::pltxt2htm::Ast<ndebug>{}));
                goto entry;
            }
            if (auto opt_url =
                    ::pltxt2htm::details::try_parse_auto_url<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_url.has_value()) {
                // Suppress auto-link when inside a URL-link container frame (pl_link,
                // pl_external, md_link, html_a): a bare URL there would otherwise nest
                // an <a> inside another <a>.  The URL then falls through to literal text.
                bool const in_url_link_frame{
                    call_stack.contains_frame_if([](ParserFrame<ndebug> const& frame) noexcept {
                        return ::pltxt2htm::details::is_url_link_tag_type(frame.get_nested_tag_type());
                    })};
                if (in_url_link_frame == false) {
                    auto&& [consumed_size, url_obj] = opt_url.template value<ndebug>();
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(url_obj)));
                    current_index += consumed_size;
                    continue;
                }
            }
            if (auto opt_md_image =
                    ::pltxt2htm::details::try_parse_md_image<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_md_image.has_value()) {
                auto&& [advance_count, text, link] = opt_md_image.template value<ndebug>();
                current_index += advance_count;
                result.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdImage<ndebug>{::std::move(text), ::std::move(link)}));
                continue;
            }
            if (chr == u8'<') {
                // if i is a valid value, i always less than pltext_size
                pltxt2htm_assert(current_index < pltext_size, u8"Index of parser out of bound");

                if (current_index + 1 == pltext_size) {
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                // a trie for tags
                switch (pltext.template index<ndebug>(current_index + 1)) {
                case u8'a':
                    [[fallthrough]];
                case u8'A': {
                    // parsing pl <a>$1</a> tag (not html <a> tag)
                    // Note: <align=...> (Unity TextMeshPro) is only parsed at line-start
                    // block context via find_next_block_after_line_break; inline occurrences
                    // render as literal text (same as inline <p>).
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::pl_a},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing html <a href="URL"> tag
                    if (auto a_tag = ::pltxt2htm::details::try_parse_html_a_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        a_tag.is_valid()) {
                        auto const tag_len = a_tag.tag_len;
                        ::pltxt2htm::Url url = ::std::move(a_tag.url.template value<ndebug>());
                        auto const internal = a_tag.internal;
                        current_index += tag_len + 2;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{ParserFrameContextWithHtmlATagInfo{
                                pltext.template subview<ndebug>(current_index), ::std::move(url),
                                internal}}, // ::pltxt2htm::NodeKind::html_a is automatically set in ctor
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    else if (a_tag.is_invalid_url()) {
                        // the <a href="..."> opening tag was recognized but its URL is invalid:
                        // consume the whole span as literal text (no auto-link / tag dispatch inside).
                        auto const tag_len = a_tag.tag_len;
                        auto const span = pltext.template subview<ndebug>(current_index, tag_len + 2);
                        auto&& [_, literal_ast, found_end_] =
                            ::pltxt2htm::details::simply_parse_pltext<ndebug, U8LiteralString<0>{}>(span);
                        result.append_range(::std::move(literal_ast));
                        current_index += tag_len + 2;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'b':
                    [[fallthrough]];
                case u8'B': {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing pl&html <b> tag
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::pl_b},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_br_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"r">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_br_tag_len.has_value()) {
                        current_index += opt_br_tag_len.template value<ndebug>().template get<ndebug>() + 1;
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBr{}));

                        auto&& [advance_count, require_restart] =
                            ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                                pltext.template subview<ndebug>(current_index + 1), call_stack, result);
                        current_index += advance_count;
                        if (require_restart) {
                            current_index += 1;
                            goto entry;
                        }
                        ++current_index;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'c':
                    [[fallthrough]];
                case u8'C': {
                    // parsing: <code>$1</code> inline element (no language/class support)
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ode">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing start tag <code> successed
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_code},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing: <color=$1>$2</color>
                    if (auto opt_color_tag = ::pltxt2htm::details::try_parse_color_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_color_tag.has_value()) {
                        auto&& [tag_len, color] = opt_color_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        // parsing start tag <color> successed
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{color}},
                                ::pltxt2htm::NodeKind::pl_color},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'd':
                    [[fallthrough]];
                case u8'D': {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"el">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing <del>$1</del>
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_del},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing: <discussions=$1>$2</discussions>
                    if (auto opt_discussions_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"iscussions", ::pltxt2htm::details::is_url_value_char>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_discussions_tag.has_value()) {
                        auto&& [tag_len, value] = opt_discussions_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{value}},
                                ::pltxt2htm::NodeKind::pl_discussions},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing: <discussion=$1>$2</discussion>
                    if (auto opt_discussion_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"iscussion", ::pltxt2htm::details::is_ascii_lowercase_alphanumeric>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_discussion_tag.has_value()) {
                        auto&& [tag_len, id] = opt_discussion_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{id}},
                                ::pltxt2htm::NodeKind::pl_discussion},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'e':
                    [[fallthrough]];
                case u8'E': {
                    // parsing: <experiments=$1>$2</experiments>
                    if (auto opt_experiments_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"xperiments", ::pltxt2htm::details::is_url_value_char>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_experiments_tag.has_value()) {
                        auto&& [tag_len, value] = opt_experiments_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{value}},
                                ::pltxt2htm::NodeKind::pl_experiments},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_experiment_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"xperiment", ::pltxt2htm::details::is_ascii_lowercase_alphanumeric>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_experiment_tag.has_value()) {
                        // parsing: <experiment=$1>$2</experiment>
                        auto&& [tag_len, id] = opt_experiment_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{id}},
                                ::pltxt2htm::NodeKind::pl_experiment},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto external_tag = ::pltxt2htm::details::try_parse_external_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        external_tag.is_valid()) {
                        auto const tag_len = external_tag.tag_len;
                        ::pltxt2htm::Url url = ::std::move(external_tag.url.template value<ndebug>());
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithUrlInfo{pltext.template subview<ndebug>(current_index),
                                                              ::std::move(url)},
                                ::pltxt2htm::NodeKind::pl_external},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    else if (external_tag.is_invalid_url()) {
                        // the <external=...> opening tag was recognized but its URL is invalid:
                        // consume the whole span as literal text (no auto-link / tag dispatch inside).
                        auto const tag_len = external_tag.tag_len;
                        auto const span = pltext.template subview<ndebug>(current_index, tag_len + 3);
                        auto&& [_, literal_ast, found_end_] =
                            ::pltxt2htm::details::simply_parse_pltext<ndebug, U8LiteralString<0>{}>(span);
                        result.append_range(::std::move(literal_ast));
                        current_index += tag_len + 3;
                        continue;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"m">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_em},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'i':
                    [[fallthrough]];
                case u8'I': {
                    // parsing: <internal=$1>$2</internal>
                    if (auto opt_internal_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"nternal", ::pltxt2htm::details::is_ascii_graphic>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_internal_tag.has_value()) {
                        auto&& [tag_len, value] = opt_internal_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{value}},
                                ::pltxt2htm::NodeKind::pl_internal},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing pl&html <i>$1</i> tag
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::pl_i},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing html <img src="..." alt="..."> self-closing tag
                    if (auto opt_img_tag = ::pltxt2htm::details::try_parse_img_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_img_tag.has_value()) {
                        auto&& [tag_len, src, alt] = opt_img_tag.template value<ndebug>();
                        current_index += tag_len + 1;
                        result.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlImg{::std::move(src), ::std::move(alt)}));
                        ++current_index;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'l':
                    [[fallthrough]];
                case u8'L': {
                    if (auto link_tag = ::pltxt2htm::details::try_parse_link_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        link_tag.is_valid()) {
                        // parsing: <link="url">$1</link>
                        auto const tag_len = link_tag.tag_len;
                        ::pltxt2htm::Url url = ::std::move(link_tag.url.template value<ndebug>());
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithUrlInfo{pltext.template subview<ndebug>(current_index),
                                                              ::std::move(url)},
                                ::pltxt2htm::NodeKind::pl_link},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    else if (link_tag.is_invalid_url()) {
                        // the <link="..."> opening tag was recognized but its URL is invalid:
                        // consume the whole span as literal text (no auto-link / tag dispatch inside).
                        auto const tag_len = link_tag.tag_len;
                        auto const span = pltext.template subview<ndebug>(current_index, tag_len + 3);
                        auto&& [_, literal_ast, found_end_] =
                            ::pltxt2htm::details::simply_parse_pltext<ndebug, U8LiteralString<0>{}>(span);
                        result.append_range(::std::move(literal_ast));
                        current_index += tag_len + 3;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'm':
                    [[fallthrough]];
                case u8'M': {
                    if (auto opt_mark_tag = ::pltxt2htm::details::try_parse_mark_equal_sign_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_mark_tag.has_value()) {
                        auto&& [tag_len, background_color] = opt_mark_tag.template value<ndebug>();
                        // parsing pl <mark=Xxx> tag (TMP rich text)
                        current_index += tag_len + 2;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPlMarkInfo{pltext.template subview<ndebug>(current_index),
                                                                 ::std::move(background_color)},
                                ::pltxt2htm::NodeKind::pl_mark},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_mark_tag = ::pltxt2htm::details::try_parse_mark_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_mark_tag.has_value()) {
                        auto&& [tag_len, background_color] = opt_mark_tag.template value<ndebug>();
                        // parsing html <mark> tag (bare or with style="background-color:...")
                        current_index += tag_len + 2;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithHtmlMarkInfo{pltext.template subview<ndebug>(current_index),
                                                                   ::std::move(background_color)},
                                ::pltxt2htm::NodeKind::html_mark},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8's':
                    [[fallthrough]];
                case u8'S': {
                    // parsing pl <size=$1>$2</size> tag
                    if (auto opt_size_tag = ::pltxt2htm::details::try_parse_size_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_size_tag.has_value()) {
                        auto const [tag_len, value] = opt_size_tag.template value<ndebug>();
                        if (value.value == 0) {
                            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                            ++current_index;
                            goto entry;
                        }

                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{ParserFrameContextWithPlSizeTagInfo{
                                                               pltext.template subview<ndebug>(current_index), value},
                                                           ::pltxt2htm::NodeKind::pl_size},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // parsing <span style="color:...;font-size:..."> as html_span
                    if (auto opt_span_tag = ::pltxt2htm::details::try_parse_span_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_span_tag.has_value()) {
                        auto&& [tag_len, color, font_size, vertical_align] = opt_span_tag.template value<ndebug>();
                        current_index += tag_len + 2;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithHtmlSpanInfo{pltext.template subview<ndebug>(current_index),
                                                                   ::std::move(color), ::std::move(font_size),
                                                                   ::std::move(vertical_align)},
                                ::pltxt2htm::NodeKind::html_span},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"trong">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // HTML <strong> tag
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_strong},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"up">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing <sup>$1</sup> (HTML and Unity TextMeshPro superscript)
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_sup},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ub">(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing <sub>$1</sub> (HTML and Unity TextMeshPro subscript)
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_sub},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing <s>$1</s> (HTML and Unity TextMeshPro strikethrough)
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_s},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8't':
                    [[fallthrough]];
                case u8'T': {
                    // parsing: <trigger=$1>$2</trigger>
                    if (auto opt_trigger_tag = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<
                            ndebug, u8"rigger", ::pltxt2htm::details::is_ascii_graphic>(
                            pltext.template subview<ndebug>(current_index + 2), call_stack);
                        opt_trigger_tag.has_value()) {
                        auto&& [tag_len, value] = opt_trigger_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{value}},
                                ::pltxt2htm::NodeKind::pl_trigger},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'u':
                    [[fallthrough]];
                case u8'U': {
                    // parsing pl <user=$1>$2</user> tag
                    if (auto opt_user_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<
                            ndebug, u8"ser", ::pltxt2htm::details::is_ascii_lowercase_alphanumeric>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_user_tag.has_value()) {
                        auto&& [tag_len, id] = opt_user_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithEqualSignTagInfo{pltext.template subview<ndebug>(current_index),
                                                                       ::pltxt2htm::container::U8String{id}},
                                ::pltxt2htm::NodeKind::pl_user},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    // <ul> is a block-level list; inline occurrences are plain literal text
                    // (except <u> underline below).
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing <u>$1</u> (HTML and Unity TextMeshPro underline)
                        current_index += opt_tag_len.template value<ndebug>() + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{
                                ParserFrameContextWithPltextInfo{pltext.template subview<ndebug>(current_index)},
                                ::pltxt2htm::NodeKind::html_u},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'v':
                    [[fallthrough]];
                case u8'V': {
                    // parsing pl <voffset=$1>$2</voffset> tag (Unity TextMeshPro rich text)
                    if (auto opt_voffset_tag = ::pltxt2htm::details::try_parse_voffset_tag<ndebug>(
                            pltext.template subview<ndebug>(current_index + 2));
                        opt_voffset_tag.has_value()) {
                        auto const [tag_len, value] = opt_voffset_tag.template value<ndebug>();
                        current_index += tag_len + 3;
                        call_stack.push_frame(ParserFrame<ndebug>(
                            FrontendContextVariant<ndebug>{ParserFrameContextWithPlVoffsetTagInfo{
                                                               pltext.template subview<ndebug>(current_index), value},
                                                           ::pltxt2htm::NodeKind::pl_voffset},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'!': {
                    // parsing: <!--$1-->
                    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"--">(
                            pltext.template subview<ndebug>(current_index + 2))) {
                        // Find the closing -->
                        ::std::size_t comment_end{current_index + 4}; // Position after <!--
                        ::pltxt2htm::Ast<ndebug> subast{};

                        for (; comment_end < pltext_size; ++comment_end) {
                            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"-->">(
                                    pltext.template subview<ndebug>(comment_end))) {
                                break;
                            }
                            subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::U8Char{pltext.template index<ndebug>(comment_end)}));
                        }

                        current_index = comment_end + 2; // Point to '>'
                        result.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlNote<ndebug>{::std::move(subast)}));
                        ++current_index;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'/': {
                    auto&& frame = call_stack.template current_frame<ndebug>();
                    switch (frame.get_nested_tag_type()) /* -Werror=switch */ {
                    case ::pltxt2htm::NodeKind::pl_color: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </color> or </a>
                        ::pltxt2htm::container::Optional<::std::size_t> opt_tag_len{
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"color">(
                                pltext.template subview<ndebug>(current_index + 2))};
                        if (opt_tag_len.has_value() == false) {
                            opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                                pltext.template subview<ndebug>(current_index + 2));
                        }
                        if (opt_tag_len.has_value()) {
                            // parsing end tag </color> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlColor staged_node(::std::move(result), ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_a: {
                        // parsing </color> or </a>
                        ::pltxt2htm::container::Optional<::std::size_t> opt_tag_len{
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"color">(
                                pltext.template subview<ndebug>(current_index + 2))};
                        if (opt_tag_len.has_value() == false) {
                            opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                                pltext.template subview<ndebug>(current_index + 2));
                        }
                        if (opt_tag_len.has_value()) {
                            // parsing end tag </a> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlA staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_span: {
                        auto&& active_frame_data = frame.as_html_span_info();
                        // parsing </span>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"span">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSpan staged_node(::std::move(result), ::std::move(active_frame_data.color),
                                                              active_frame_data.font_size,
                                                              active_frame_data.vertical_align);
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_a: {
                        auto&& active_frame_data = frame.as_html_a_tag_info();
                        // parsing </a>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlA staged_node(::std::move(result), ::std::move(active_frame_data.url),
                                                           active_frame_data.internal);
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_experiment: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </experiment>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"experiment">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlExperiment staged_node(::std::move(result),
                                                                  ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_experiments: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </experiments>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"experiments">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlExperiments staged_node(::std::move(result),
                                                                   ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_discussion: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </discussion>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"discussion">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlDiscussion staged_node(::std::move(result),
                                                                  ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_discussions: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </discussions>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"discussions">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlDiscussions staged_node(::std::move(result),
                                                                   ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_external: {
                        auto&& active_frame_data = frame.as_url_info();
                        // parsing </external>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"external">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlExternal staged_node(::std::move(result),
                                                                ::std::move(active_frame_data.url));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_link: {
                        auto&& active_frame_data = frame.as_url_info();
                        // parsing </link>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"link">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlLink staged_node(::std::move(result), ::std::move(active_frame_data.url));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_trigger: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </trigger>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"trigger">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlTrigger staged_node(::std::move(result), ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_internal: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </internal>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"internal">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlInternal staged_node(::std::move(result), ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_user: {
                        auto&& active_frame_data = frame.as_equal_sign_tag();
                        // parsing </user>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"user">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlUser staged_node(::std::move(result), ::std::move(active_frame_data.id));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_size: {
                        auto&& active_frame_data = frame.as_pl_size_tag();
                        // parsing </size>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"size">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlSize staged_node(::std::move(result), active_frame_data.value);
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_voffset: {
                        auto&& active_frame_data = frame.as_pl_voffset_tag();
                        // parsing </voffset>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"voffset">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlVoffset staged_node(::std::move(result), active_frame_data.value);
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_align: {
                        auto&& active_frame_data = frame.as_align_info();
                        // parsing </align>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"align">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlAlign staged_node(::std::move(result), active_frame_data.align);
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_mark: {
                        auto&& active_frame_data = frame.as_pl_mark_info();
                        // parsing </mark>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"mark">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlMark staged_node(::std::move(result),
                                                            ::std::move(active_frame_data.background_color));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMark<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_margin: {
                        auto&& active_frame_data = frame.as_pl_margin_tag();
                        // parsing </margin>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"margin">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlMargin staged_node(::std::move(result), active_frame_data.left,
                                                              active_frame_data.right);
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_div: {
                        auto&& active_frame_data = frame.as_html_div_info();
                        // parsing </div>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"div">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDiv staged_node(::std::move(result), active_frame_data.left,
                                                             active_frame_data.right);
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_b: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"b">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </b> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlB staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_i: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"i">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </a> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlI staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_p: {
                        auto&& active_frame_data = frame.as_align_info();
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"p">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </p> successed
                            ::std::size_t const staged_index{current_index};
                            auto const align = active_frame_data.align;
                            ::pltxt2htm::HtmlP staged_node(::std::move(result), align);
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h1: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h1">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h1> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH1 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h2: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h2">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h2> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH2 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h3: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h3">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h3> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH3 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h4: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h4">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h4> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH4 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h5: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h5">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h5> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH5 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h6: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h6">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </h6> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH6 staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_del: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"del">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </del> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDel staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_code: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"code">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </code> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlCode staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_note:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected html_note node during end-tag parsing");
                        }
                    case ::pltxt2htm::NodeKind::html_em: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"em">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </em> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlEm staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_strong: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"strong">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </strong> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlStrong staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_mark: {
                        auto&& active_frame_data = frame.as_html_mark_info();
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"mark">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </mark> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlMark staged_node(::std::move(result),
                                                              ::std::move(active_frame_data.background_color));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_u: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"u">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </u> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlU staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_s: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"s">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </s> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlS staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sup: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"sup">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </sup> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSup staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sub: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"sub">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </sub> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSub staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_blockquote: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"blockquote">(
                                pltext.template subview<ndebug>(current_index + 2));
                            opt_tag_len.has_value()) {
                            // parsing end tag </blockquote> successed
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlBlockquote staged_node(::std::move(result));
                            call_stack.template discard_current_frame<ndebug>();
                            if (call_stack.empty()) {
                                return ParsePlTxtResult<ndebug>{
                                    .subast = ::std::move(staged_node.get_subast()),
                                    .consumed_bytes = staged_index + opt_tag_len.template value<ndebug>() + 3};
                            }
                            auto& parent_frame = call_stack.template current_frame<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::md_block_quotes:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::code_fence:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_del:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_link:
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
                    case ::pltxt2htm::NodeKind::group:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_li:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_li_checkbox:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_latex_inline:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_latex_block:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table:
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
                    case ::pltxt2htm::NodeKind::table_tr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_th:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_td: {
                        // any tag contains `</` context would hit this branch
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
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
                    case ::pltxt2htm::NodeKind::entity_reference:
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
                    case ::pltxt2htm::NodeKind::md_hr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_br:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_hr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_col:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_img:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_image:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::url:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_project:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_visitor:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_author:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_coauthors:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_ul:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_ol:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_escape:
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                        [[fallthrough]];
                    default:
#endif
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected escape node kind in inner switch");
                        }
                    }
                    pltxt2htm_unreachable(u8"Unreachable after escape-node inner switch");
                }
                default: {
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                }
                pltxt2htm_unreachable(u8"Unreachable after outer switch");
            }
            auto const advance_count = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                pltext.template subview<ndebug>(current_index), result);
            current_index += advance_count;
            continue;
        }

        {
            ParserFrame<ndebug> frame{call_stack.template pop_frame<ndebug>()};
            ::std::size_t const staged_index = pltext_size;
            if (call_stack.empty()) {
                // Considering the following markdown:
                // ```md
                // <b>e</b>xample
                // ```
                // Text without any tag in the end will hit this branch.
                return ParsePlTxtResult<ndebug>{.subast = ::std::move(frame.subast), .consumed_bytes = staged_index};
            }
            // Considering the following markdown:
            // ```md
            // <b>example
            // ```
            // Any tag without a closing tag will hit this branch.
            auto&& subast = frame.subast;
            auto&& parent_frame = call_stack.template current_frame<ndebug>();
            auto&& parent_ast = parent_frame.subast;
            auto&& parent_index = parent_frame.current_index;
            switch (frame.get_nested_tag_type()) /* -Werror=switch */ {
            case ::pltxt2htm::NodeKind::pl_color: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& id = active_frame_data.id;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlColor<ndebug>{::std::move(subast), ::std::move(id)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlA<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& id = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlExperiment<ndebug>{::std::move(subast), ::std::move(id)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& value = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlExperiments<ndebug>{::std::move(subast), ::std::move(value)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& id = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlDiscussion<ndebug>{::std::move(subast), ::std::move(id)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& value = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlDiscussions<ndebug>{::std::move(subast), ::std::move(value)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& id = active_frame_data.id;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlUser<ndebug>{::std::move(subast), ::std::move(id)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& value = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlTrigger<ndebug>{::std::move(subast), ::std::move(value)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                auto&& active_frame_data = frame.as_equal_sign_tag();
                auto&& value = active_frame_data.id;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlInternal<ndebug>{::std::move(subast), ::std::move(value)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                auto&& active_frame_data = frame.as_url_info();
                auto&& url = active_frame_data.url;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlExternal<ndebug>{::std::move(subast), ::std::move(url)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                auto&& active_frame_data = frame.as_url_info();
                auto&& url = active_frame_data.url;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlLink<ndebug>{::std::move(subast), ::std::move(url)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                auto&& active_frame_data = frame.as_pl_size_tag();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlSize<ndebug>{::std::move(subast), active_frame_data.value}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                auto&& active_frame_data = frame.as_pl_voffset_tag();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlVoffset<ndebug>{::std::move(subast), active_frame_data.value}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                auto&& active_frame_data = frame.as_align_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlAlign<ndebug>{::std::move(subast), active_frame_data.align}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                auto&& active_frame_data = frame.as_pl_margin_tag();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMargin<ndebug>{
                    ::std::move(subast), active_frame_data.left, active_frame_data.right}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                auto&& active_frame_data = frame.as_html_div_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDiv<ndebug>{
                    ::std::move(subast), active_frame_data.left, active_frame_data.right}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                auto&& active_frame_data = frame.as_pl_mark_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::PlMark<ndebug>{::std::move(subast), ::std::move(active_frame_data.background_color)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto&& active_frame_data = frame.as_html_span_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::HtmlSpan<ndebug>{::std::move(subast), ::std::move(active_frame_data.color),
                                                  active_frame_data.font_size, active_frame_data.vertical_align}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                auto&& active_frame_data = frame.as_html_a_tag_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlA<ndebug>{
                    ::std::move(subast), ::std::move(active_frame_data.url), active_frame_data.internal}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_b: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlB<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlI<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto&& active_frame_data = frame.as_align_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::HtmlP<ndebug>{::std::move(subast), active_frame_data.align}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH1<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH2<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH3<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH4<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH5<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH6<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDel<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCode<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_u: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlU<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_s: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlS<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSup<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSub<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                auto&& active_frame_data = frame.as_html_mark_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlMark<ndebug>{
                    ::std::move(subast), ::std::move(active_frame_data.background_color)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ul: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListUl<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListOl<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListLi<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                auto&& active_frame_data = frame.as_list_li_checkbox();
                auto const checked = active_frame_data.checked;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListLiCheckbox<ndebug>{::std::move(subast), checked}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Table<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTr<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto&& active_frame_data = frame.as_cell();
                auto const align = active_frame_data.align;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTd<ndebug>{::std::move(subast), align}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                auto&& active_frame_data = frame.as_cell();
                auto const align = active_frame_data.align;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTh<ndebug>{::std::move(subast), align}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableThead<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTbody<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTfoot<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableCaption<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableColgroup<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(subast)}));
                parent_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH1<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH2<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH3<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH4<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH5<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                ::pltxt2htm::container::U8StringView const super_pltext{
                    call_stack.template current_frame<ndebug>().get_pltext()};
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH6<ndebug>{::std::move(subast)}));
                auto&& [advance_count, _] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
                    super_pltext.template subview<ndebug>(parent_index), call_stack, parent_ast);
                parent_index += advance_count;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan1Backtick<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan2Backtick<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan3Backtick<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexInline<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexBlock<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDel<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdBlockQuotes<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                auto&& active_frame_data = frame.as_url_info();
                auto&& link_url = active_frame_data.url;
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::MdLink<ndebug>{::std::move(subast), ::std::move(link_url)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::u8char:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::invalid_utf8:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::group:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::line_break:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::space:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::entity_reference:
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
            case ::pltxt2htm::NodeKind::html_br:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_col:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_img:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_escape:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::code_fence:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_note:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_image:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::url:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_project:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_visitor:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_author:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_coauthors:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected block node kind in inline context");
                }
            }
            pltxt2htm_unreachable(u8"Unreachable after block-node-in-inline switch");
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
