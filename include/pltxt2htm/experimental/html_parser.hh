#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../ast/node_kind.hh"
#include "../ast/ast.hh"
#include "../contracts.hh"
#include "../details/utils.hh"
#include "../details/parser/frame_context.hh"
#include "../details/parser/try_parse.hh"
#include "../details/parser/html_list.hh"
#include "../details/parser/html_table.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::experimental {
namespace details {

/**
 * @brief Return type of find_next_block_after_line_break.
 */
struct FindNextBlockAfterLineBreakResult {
    ::std::size_t advance_count; ///< Bytes consumed from the input subview.
    bool new_frame_been_pushed_into_call_stack; ///< Whether a new frame was pushed.
};

/**
 * @brief Scan for a block-level frame at the current position and push it onto the call stack.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input subview starting at the candidate block position.
 * @param call_stack Active parser call stack.
 * @param result AST being built.
 * @return How many bytes were consumed and whether a new frame was created.
 * @note Handles the `<p>`, `<h1>`–`<h6>`, `<hr>`, `<blockquote>`, `<ul>`/`<ol>` and `<table>` blocks.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto find_next_block_after_line_break(
    ::fast_io::u8string_view pltext, ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>>& call_stack,
    ::pltxt2htm::Ast<ndebug>& result) noexcept
    -> ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult {
    ::std::size_t current_index{};
    while (true) {
        // Check for HTML <hr> self-closing tag at block position. Emit it as a leaf and keep
        // scanning so a following <h1>/<p> on the same line is recognized as a block (same as
        // after a line break).
        if (auto opt_hr_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"<hr">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_hr_tag_len.has_value()) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlHr{}));
            current_index += opt_hr_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            continue;
        }
        // Check for an HTML <pre><code>...</code></pre> code block at a block position.
        if (auto opt_pre_code_block = ::pltxt2htm::details::try_parse_html_pre_code_block<ndebug, false>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_pre_code_block.has_value()) {
            auto&& [node, advance_count] =
                opt_pre_code_block.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(::std::move(node));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = false};
        }
        if (auto opt_p_tag = ::pltxt2htm::details::try_parse_p_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_p_tag.has_value()) {
            ::std::size_t const consumed{opt_p_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len +
                                         1};
            auto const align = opt_p_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>().align;
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithAlignInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed), align},
                    ::pltxt2htm::NodeKind::html_p},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h1">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h1_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h1_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h1},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h2">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h2_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h2_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h2},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h3">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h3_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h3_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h3},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h4">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h4_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h4_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h4},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h5">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h5_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h5_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h5},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<h6">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_h6_tag_len.has_value()) {
            ::std::size_t consumed{opt_h6_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h6},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_blockquote_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<blockquote">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_blockquote_tag.has_value()) {
            ::std::size_t const consumed{opt_blockquote_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                         1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_blockquote},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for an HTML <div style="margin-left:...;margin-right:..."> at a block position.
        if (auto opt_div_tag = ::pltxt2htm::details::try_parse_html_div_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_div_tag.has_value()) {
            auto&& [tag_len, left, right] = opt_div_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            ::std::size_t const consumed{tag_len + 1};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithHtmlDivInfo{
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + consumed), left,
                        right},
                    ::pltxt2htm::NodeKind::html_div},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <ul>/<ol> list at a block position (block-level lists).
        if (auto opt_html_list_ast = ::pltxt2htm::details::optionally_to_html_list_ast<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_html_list_ast.has_value()) {
            auto&& [top_node, advance_count] =
                opt_html_list_ast.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            ::pltxt2htm::details::push_list_frame<ndebug>(call_stack, ::std::move(top_node));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for an HTML <table> block at a block position (block-level tables).
        if (auto opt_html_table_ast = ::pltxt2htm::details::optionally_to_html_table_ast<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_html_table_ast.has_value()) {
            auto&& [raw_ast, advance_count] =
                opt_html_table_ast.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                    ::pltxt2htm::details::ParserFrameContextWithTableInfo<ndebug>{::std::move(raw_ast)},
                    ::pltxt2htm::NodeKind::table},
                ::pltxt2htm::Ast<ndebug>{}));
            return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = true};
        }
        return ::pltxt2htm::experimental::details::FindNextBlockAfterLineBreakResult{
            .advance_count = current_index, .new_frame_been_pushed_into_call_stack = false};
    }
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt_html(::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>>& call_stack) noexcept
    -> ::pltxt2htm::Ast<ndebug> {
entry:
    while (true) {
        // List frames hold an intermediate ListAst; iterate it like the main parser does.
        if (auto const list_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            list_tag_type == ::pltxt2htm::NodeKind::list_ul || list_tag_type == ::pltxt2htm::NodeKind::list_ol) {
            auto&& list_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            auto&& frame_list_ast = list_frame.get_list_ast();
            auto&& frame_iter = list_frame.get_list_iter();
            if (frame_iter == frame_list_ast.end()) {
                ::pltxt2htm::details::ParserFrameContext<ndebug> previous_frame(::std::move(list_frame));
                call_stack.pop();
                if (call_stack.empty()) {
                    return ::std::move(previous_frame.subast);
                }
                if (list_tag_type == ::pltxt2htm::NodeKind::list_ul) {
                    ::pltxt2htm::details::stack_top<ndebug>(call_stack)
                        .subast.emplace_back(::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::ListUl<ndebug>{::std::move(previous_frame.subast)}));
                }
                else {
                    ::pltxt2htm::details::stack_top<ndebug>(call_stack)
                        .subast.emplace_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListOl<ndebug>{
                            ::std::move(previous_frame.subast), previous_frame.get_list_start()}));
                }
                goto entry;
            }
            switch (frame_iter->get_type()) {
            case ::pltxt2htm::details::ListNodeType::list_li: {
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                    ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                        ::pltxt2htm::details::ParserFrameContextWithPltextInfo{frame_iter->get_text_view()},
                        ::pltxt2htm::NodeKind::list_li},
                    ::pltxt2htm::Ast<ndebug>{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
                // HTML lists never produce checkbox items, but the branch must exist for
                // exhaustive switch checking.
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                    ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                        ::pltxt2htm::details::ParserFrameContextWithListLiCheckboxInfo{frame_iter->get_text_view(),
                                                                                       frame_iter->is_checked()},
                        ::pltxt2htm::NodeKind::list_li_checkbox},
                    ::pltxt2htm::Ast<ndebug>{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_ul: {
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                    ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                        ::pltxt2htm::details::ParserFrameContextWithListInfo<ndebug>{
                            ::std::move(frame_iter->get_sublist())},
                        ::pltxt2htm::NodeKind::list_ul},
                    ::pltxt2htm::Ast<ndebug>{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_ol: {
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                    ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                        ::pltxt2htm::details::ParserFrameContextWithListInfo<ndebug>{
                            ::std::move(frame_iter->get_sublist()), frame_iter->get_start()},
                        ::pltxt2htm::NodeKind::list_ol},
                    ::pltxt2htm::Ast<ndebug>{}));
                break;
            }
#if PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected ListNodeType in html parser");
                }
#endif
            }
            ++frame_iter;
            goto entry;
        }
        // HTML table frames hold an intermediate TableAstRaw; iterate them like the list frames.
        if (::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type() == ::pltxt2htm::NodeKind::table) {
            auto opt_table_ast = ::pltxt2htm::details::process_table_frame<ndebug>(call_stack);
            if (opt_table_ast.has_value()) {
                auto&& table_ast = opt_table_ast.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                return ::std::move(table_ast);
            }
            goto entry;
        }
        auto&& top_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& current_index = top_frame.current_index;
        ::fast_io::u8string_view const pltext{top_frame.get_pltext()};
        auto&& result = top_frame.subast;
        ::std::size_t const pltext_size{pltext.size()};

        // Check for block-level <p> tag at line start (start of input or after frame completion)
        auto&& [entry_advance, entry_new_frame] =
            ::pltxt2htm::experimental::details::find_next_block_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), call_stack, result);
        current_index += entry_advance;
        if (entry_new_frame) {
            goto entry;
        }

        while (current_index < pltext_size) {
            char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

            if (chr == u8'\n') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));
                ++current_index;
                // Check for block-level <p> tag after newline
                auto&& [nl_advance, nl_new_frame] =
                    ::pltxt2htm::experimental::details::find_next_block_after_line_break<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), call_stack, result);
                current_index += nl_advance;
                if (nl_new_frame) {
                    goto entry;
                }
                continue;
            }
            if (chr == u8' ') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
                ++current_index;
                continue;
            }
            if (chr == u8'&') {
                if (auto const opt_entity_len = ::pltxt2htm::details::try_parse_entity_reference<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                    opt_entity_len.has_value()) {
                    auto const entity_len = opt_entity_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::EntityReference{::fast_io::u8string{
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
            if (chr == u8'<') {
                pltxt2htm_assert(current_index < pltext_size, u8"Index of parser out of bound");

                if (current_index + 1 == pltext_size) {
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                switch (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1)) {
                case u8'a':
                    [[fallthrough]];
                case u8'A': {
                    if (auto a_tag = ::pltxt2htm::details::try_parse_html_a_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        a_tag.is_valid()) {
                        auto tag_len = a_tag.tag_len;
                        ::pltxt2htm::Url url =
                            ::std::move(a_tag.url).template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        auto const internal = a_tag.internal;
                        current_index += tag_len + 2;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlATagInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                                    ::std::move(url), internal}},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'b':
                    [[fallthrough]];
                case u8'B': {
                    if (auto opt_br_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"r">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_br_tag_len.has_value()) {
                        current_index += opt_br_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBr{}));
                        // Check for block-level <p> tag right after <br>. current_index currently sits
                        // on the '>' of <br>, so the candidate block starts at current_index + 1.
                        auto&& [br_advance, br_new_frame] =
                            ::pltxt2htm::experimental::details::find_next_block_after_line_break<ndebug>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1),
                                call_stack, result);
                        current_index += br_advance;
                        if (br_new_frame) {
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
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing start tag <code> successed
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_code},
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
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_del},
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
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"m">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
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
                    // <input> is only recognized inside block-level <ul>/<ol> items
                    // (parsed by html_list.hh); inline occurrences are literal text.
                    if (auto opt_img_tag = ::pltxt2htm::details::try_parse_img_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_img_tag.has_value()) {
                        auto&& [tag_len, src, alt] =
                            opt_img_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
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

                case u8'm':
                    [[fallthrough]];
                case u8'M': {
                    if (auto opt_mark_tag = ::pltxt2htm::details::try_parse_mark_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_mark_tag.has_value()) {
                        auto&& [tag_len, background_color] =
                            opt_mark_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 2;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlMarkInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
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
                    if (auto opt_span_tag = ::pltxt2htm::details::try_parse_span_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_span_tag.has_value()) {
                        auto&& [tag_len, color, font_size, vertical_align] =
                            opt_span_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 2;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlSpanInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                                    ::std::move(color), ::std::move(font_size), ::std::move(vertical_align)},
                                ::pltxt2htm::NodeKind::html_span},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"trong">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_strong},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"up">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_sup},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ub">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_sub},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::pl_s},
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
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'u':
                    [[fallthrough]];
                case u8'U': {
                    // <ul> is a block-level list; inline occurrences are plain literal text
                    // (except <u> underline below).
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::pl_u},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'!': {
                    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"--">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))) {
                        ::std::size_t comment_end{current_index + 4};
                        ::pltxt2htm::Ast<ndebug> subast{};

                        for (; comment_end < pltext_size; ++comment_end) {
                            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"-->">(
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, comment_end))) {
                                break;
                            }
                            subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{
                                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, comment_end)}));
                        }

                        current_index = comment_end + 2;
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
                    auto&& frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                    switch (frame.get_nested_tag_type()) {
                    case ::pltxt2htm::NodeKind::html_span: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"span">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSpan staged_node(
                                ::std::move(result), ::std::move(frame.get_html_span_color()),
                                frame.get_html_span_font_size(), frame.get_html_span_vertical_align());
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlSpan<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_a: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlA staged_node(::std::move(result), ::std::move(frame.get_html_a_url()),
                                                           frame.get_html_a_internal());
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlA<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_div: {
                        // parsing </div>
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"div">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDiv staged_node(::std::move(result), frame.get_html_div_left(),
                                                             frame.get_html_div_right());
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDiv<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_p: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"p">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            auto const align = frame.get_align();
                            ::pltxt2htm::HtmlP staged_node(::std::move(result), align);
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h1: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h1">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH1 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH1<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h2: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h2">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH2 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH2<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h3: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h3">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH3 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH3<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h4: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h4">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH4 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH4<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h5: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h5">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH5 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH5<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h6: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"h6">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH6 staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH6<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_del: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"del">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDel staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDel<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_code: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"code">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlCode staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlCode<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_u: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"u">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlU staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlU<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::pl_s: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"s">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::PlS staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlS<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sup: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"sup">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSup staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSup<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sub: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"sub">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSub staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSub<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlEm staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_strong: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"strong">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlStrong staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlStrong<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_mark: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"mark">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlMark staged_node(::std::move(result),
                                                              ::std::move(frame.get_html_mark_background_color()));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlMark<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_blockquote: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"blockquote">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlBlockquote staged_node(::std::move(result));
                            call_stack.pop();
                            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    default: {
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                        ++current_index;
                        continue;
                    }
                    }
                    pltxt2htm_unreachable(u8"Unreachable after end-tag inner switch");
                }

                default: {
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                }

                pltxt2htm_unreachable(u8"Unreachable after outer switch");
            }
            auto advance_count = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), result);
            current_index += advance_count;
            continue;
        }

        {
            ::pltxt2htm::details::ParserFrameContext<ndebug> frame(
                ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack)));
            call_stack.pop();
            if (call_stack.empty()) {
                return ::std::move(frame.subast);
            }
            auto&& subast = frame.subast;
            auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            auto&& parent_ast = parent_frame.subast;
            parent_frame.current_index += pltext_size;
            switch (frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeKind::html_span: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSpan<ndebug>{
                    ::std::move(subast), ::std::move(frame.get_html_span_color()), frame.get_html_span_font_size(),
                    frame.get_html_span_vertical_align()}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDiv<ndebug>{
                    ::std::move(subast), frame.get_html_div_left(), frame.get_html_div_right()}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlA<ndebug>{
                    ::std::move(subast), ::std::move(frame.get_html_a_url()), frame.get_html_a_internal()}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(subast), frame.get_align()}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH1<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH2<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH3<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH4<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH5<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH6<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDel<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCode<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlU<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_s: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlS<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSup<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlSub<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlStrong<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlMark<ndebug>{
                    ::std::move(subast), ::std::move(frame.get_html_mark_background_color())}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                auto const checked = frame.get_checked();
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListLiCheckbox<ndebug>{::std::move(subast), checked}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::ListLi<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Table<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTr<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto align = frame.get_cell_align();
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTd<ndebug>{::std::move(subast), align}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                auto align = frame.get_cell_align();
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTh<ndebug>{::std::move(subast), align}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableThead<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTbody<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTfoot<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableCaption<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableColgroup<ndebug>{::std::move(subast)}));
                goto entry;
            }
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected node kind in html parser unclosed-tag fallback");
                }
            }
            pltxt2htm_unreachable(u8"Unreachable after html unclosed-tag switch");
        }
    }
}

} // namespace details

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt_html(::fast_io::u8string_view html_text) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> call_stack{};

    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
        ::pltxt2htm::details::FrontendContextVariant<ndebug>{
            ::pltxt2htm::details::ParserFrameContextWithPltextInfo{html_text}, ::pltxt2htm::NodeKind::text},
        ::pltxt2htm::Ast<ndebug>{}));

    auto result = details::parse_pltxt_html<ndebug>(call_stack);

    pltxt2htm_assert(call_stack.empty(), u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm::experimental

#include "../details/pop_macro.hh"
