/**
 * @file parser.hh
 * @brief Main parser interface for pltxt2htm
 * @details Provides the main parsing functionality to convert pl-text
 *          (Physics-Lab text) into an Abstract Syntax Tree (AST)
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "ast/node_kind.hh"
#include "contracts.hh"
#include "details/utils.hh"
#include "details/parser/frame_context.hh"
#include "details/parser/parser.hh"
#include "details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief Parse pl-text into an Abstract Syntax Tree (AST)
 * @details This is the main parsing function that converts Physics-Lab text
 *          format into a structured AST that can be processed by other components.
 *          It handles nested structures, various tag types, and Markdown syntax.
 *
 *          The parsing process involves:
 *          - Tokenization of the input text
 *          - Recognition of Physics-Lab specific tags (&lt;color&gt;, &lt;experiment&gt;, etc.)
 *          - Markdown syntax parsing (headers, lists, emphasis, etc.)
 *          - HTML tag recognition and processing
 *          - Building a hierarchical AST structure
 * @tparam ndebug Contract-checking mode for parsing, using `::pltxt2htm::Contracts`
 *                 values such as `::pltxt2htm::Contracts::quick_enforce` or
 *                 `::pltxt2htm::Contracts::ignore`
 * @param[in] pltext The Physics-Lab text content to parse
 * @return An AST representing the parsed structure of the input text
 * @retval pltxt2htm::Ast Abstract Syntax Tree containing the parsed structure
 * @note This function uses a stack-based approach to handle nested tag structures
 * @warning The parsing process is recursive and handles complex nested structures
 * @see pltxt2htm::details::parse_pltxt for the internal implementation
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept -> ::pltxt2htm::Ast<ndebug> {
    // This stack is used to track nested tag contexts during parsing
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> call_stack{};
    ::pltxt2htm::Ast<ndebug> result{};

    ::std::size_t start_index{};

    while (true) {
        auto&& [advance_count, has_new_frame] = ::pltxt2htm::details::find_next_block_after_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), call_stack, result);
        start_index += advance_count;
        if (has_new_frame == false) {
            break;
        }
        ::pltxt2htm::NodeKind const type_of_subast{call_stack.top().get_nested_tag_type()};
        auto const opt_html_p_align = [&] constexpr noexcept -> ::exception::optional<::pltxt2htm::TextAlign> {
            if (type_of_subast == ::pltxt2htm::NodeKind::html_p) {
                return call_stack.top().get_align();
            }
            return ::exception::nullopt;
        }();
        auto const opt_pl_align = [&] constexpr noexcept -> ::exception::optional<::pltxt2htm::TextAlign> {
            if (type_of_subast == ::pltxt2htm::NodeKind::pl_align) {
                return call_stack.top().get_align();
            }
            return ::exception::nullopt;
        }();
        auto const opt_pl_margin = [&] constexpr noexcept
            -> ::exception::optional<::pltxt2htm::details::ParserFrameContextWithPlMarginTagInfo> {
            if (type_of_subast == ::pltxt2htm::NodeKind::pl_margin) {
                return ::pltxt2htm::details::ParserFrameContextWithPlMarginTagInfo{
                    call_stack.top().get_pltext(), call_stack.top().get_pl_margin_tag_left(),
                    call_stack.top().get_pl_margin_tag_right()};
            }
            return ::exception::nullopt;
        }();
        auto const opt_list_start = [&] constexpr noexcept -> ::exception::optional<::std::size_t> {
            if (type_of_subast == ::pltxt2htm::NodeKind::list_ol) {
                return call_stack.top().get_list_start();
            }
            return ::exception::nullopt;
        }();
        auto const opt_html_div =
            [&] constexpr noexcept -> ::exception::optional<::pltxt2htm::details::ParserFrameContextWithHtmlDivInfo> {
            if (type_of_subast == ::pltxt2htm::NodeKind::html_div) {
                return ::pltxt2htm::details::ParserFrameContextWithHtmlDivInfo{call_stack.top().get_pltext(),
                                                                               call_stack.top().get_html_div_left(),
                                                                               call_stack.top().get_html_div_right()};
            }
            return ::exception::nullopt;
        }();
        auto&& [subast, consumed_bytes] = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
        switch (type_of_subast) {
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH1<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH2<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH3<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH4<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH5<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH6<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdBlockQuotes<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::list_ul: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListUl<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::list_ol: {
            auto const list_start = opt_list_start.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListOl<ndebug>{::std::move(subast), list_start}));
            continue;
        }
        case ::pltxt2htm::NodeKind::table: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Table<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            // details::parse_pltxt reported how many bytes of the html_p frame's pltext it
            // consumed (up to the matching </p>, or all of it if unclosed). Advance start_index
            // past it so the remaining text handler doesn't re-process the consumed content.
            start_index += consumed_bytes;
            auto const html_p_align = opt_html_p_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(subast), html_p_align}));
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_align: {
            // Same as html_p: advance start_index past the consumed pl_align content, preserving
            // the Textalign read from the frame top before the recursive parse popped it.
            start_index += consumed_bytes;
            auto const pl_align_value = opt_pl_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlAlign<ndebug>{::std::move(subast), pl_align_value}));
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_margin: {
            // Same as pl_align: advance start_index past the consumed pl_margin content, preserving
            // the left/right margins read from the frame top before the recursive parse popped it.
            start_index += consumed_bytes;
            auto const pl_margin_info = opt_pl_margin.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::PlMargin<ndebug>{::std::move(subast), pl_margin_info.left, pl_margin_info.right}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_div: {
            // Same as pl_margin: advance start_index past the consumed html_div content, preserving
            // the left/right margins read from the frame top before the recursive parse popped it.
            start_index += consumed_bytes;
            auto const html_div_info = opt_html_div.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::HtmlDiv<ndebug>{::std::move(subast), html_div_info.left, html_div_info.right}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            // Same as html_p: advance start_index past the consumed html_h1 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH1<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            // Same as html_p: advance start_index past the consumed html_h2 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH2<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            // Same as html_p: advance start_index past the consumed html_h3 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH3<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            // Same as html_p: advance start_index past the consumed html_h4 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH4<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            // Same as html_p: advance start_index past the consumed html_h5 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH5<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            // Same as html_p: advance start_index past the consumed html_h6 content.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH6<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            // Same as html_p: advance start_index past the consumed html_blockquote content so the
            // remaining text handler doesn't re-process it.
            start_index += consumed_bytes;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(subast)}));
            continue;
        }
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind");
            }
        }
    }

    if (start_index < pltext.size()) {
        // other common cases
        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index)},
                ::pltxt2htm::NodeKind::text},
            ::std::move(result)));
        result = ::std::move(::pltxt2htm::details::parse_pltxt<ndebug>(call_stack).subast);
    }

    pltxt2htm_assert(call_stack.empty(), u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
