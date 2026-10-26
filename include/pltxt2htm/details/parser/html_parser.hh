/**
 * @file html_parser.hh
 * @brief Second-stage parser for HTML input text.
 * @details Parses text whose block structure comes from HTML tags into the shared AST. It is
 *          the HTML counterpart of the Markdown/pl-text inline parser and is driven by the
 *          roundtrip checks that re-parse emitted HTML.
 */
#pragma once

#include <cstddef>
#include "../../container/string.hh"
#include "../call_stack.hh"
#include "../../container/string_view.hh"
#include "../../ast/node_kind.hh"
#include "../../ast/ast.hh"
#include "../contracts.hh"
#include "../utils.hh"
#include "frame_context.hh"
#include "try_parse.hh"
#include "html_list.hh"
#include "html_table.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Return type of html_find_next_block_after_line_break.
 */
struct HtmlFindNextBlockAfterLineBreakResult {
    ::std::size_t advance_count; ///< Bytes consumed from the input subview.
    bool new_frame_been_pushed_into_call_stack; ///< Whether a new frame was pushed.
};

/**
 * @brief Scan for a block-level frame at the current position and push it onto the call stack.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 * @param pltext Input subview starting at the candidate block position.
 * @param call_stack Active parser call stack.
 * @param result AST being built.
 * @return How many bytes were consumed and whether a new frame was created.
 * @note Handles the `<p>`, `<h1>`-`<h6>`, `<hr>`, `<blockquote>`, `<ul>`/`<ol>` and `<table>` blocks.
 */
[[nodiscard]]
constexpr auto html_find_next_block_after_line_break(
    ::pltxt2htm::container::U8StringView pltext,
    ::pltxt2htm::details::CallStack<::pltxt2htm::details::ParserFrame>& call_stack, ::pltxt2htm::Ast& result) noexcept
    -> ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult {
    ::std::size_t current_index{};
    while (true) {
        // Check for HTML <hr> self-closing tag at block position. Emit it as a leaf and keep
        // scanning so a following <h1>/<p> on the same line is recognized as a block (same as
        // after a line break).
        if (auto opt_hr_tag_len =
                ::pltxt2htm::details::try_parse_self_closing_tag<u8"<hr">(pltext.subview(current_index));
            opt_hr_tag_len.has_value()) {
            result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlHr>());
            current_index += opt_hr_tag_len.value().get();
            continue;
        }
        // Check for an HTML <pre><code>...</code></pre> code block at a block position.
        if (auto opt_pre_code_block =
                ::pltxt2htm::details::try_parse_html_pre_code_block<false>(pltext.subview(current_index));
            opt_pre_code_block.has_value()) {
            auto&& [node, advance_count] = opt_pre_code_block.value();
            result.push_back(::std::move(node));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = false};
        }
        if (auto opt_p_tag = ::pltxt2htm::details::try_parse_p_tag(pltext.subview(current_index));
            opt_p_tag.has_value()) {
            ::std::size_t const consumed{opt_p_tag.value().tag_len};
            auto const align = opt_p_tag.value().align;
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{::pltxt2htm::details::ParserFrameContextWithAlignInfo{
                                                                 pltext.subview(current_index + consumed), align},
                                                             ::pltxt2htm::NodeKind::html_p},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h1">(pltext.subview(current_index));
            opt_h1_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h1_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h1},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h2">(pltext.subview(current_index));
            opt_h2_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h2_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h2},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h3">(pltext.subview(current_index));
            opt_h3_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h3_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h3},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h4">(pltext.subview(current_index));
            opt_h4_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h4_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h4},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h5">(pltext.subview(current_index));
            opt_h5_tag_len.has_value()) {
            ::std::size_t const consumed{opt_h5_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h5},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"<h6">(pltext.subview(current_index));
            opt_h6_tag_len.has_value()) {
            ::std::size_t consumed{opt_h6_tag_len.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_h6},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_blockquote_tag =
                ::pltxt2htm::details::try_parse_bare_tag<u8"<blockquote">(pltext.subview(current_index));
            opt_blockquote_tag.has_value()) {
            ::std::size_t const consumed{opt_blockquote_tag.value()};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index + consumed)},
                    ::pltxt2htm::NodeKind::html_blockquote},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for an HTML <div style="margin-left:...;margin-right:..."> at a block position.
        if (auto opt_div_tag = ::pltxt2htm::details::try_parse_html_div_tag(pltext.subview(current_index));
            opt_div_tag.has_value()) {
            auto&& [tag_len, left, right] = opt_div_tag.value();
            ::std::size_t const consumed{tag_len};
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{::pltxt2htm::details::ParserFrameContextWithMarginsInfo{
                                                                 pltext.subview(current_index + consumed), left, right},
                                                             ::pltxt2htm::NodeKind::html_div},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + consumed, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for HTML <ul>/<ol> list at a block position (block-level lists).
        if (auto opt_html_list_ast = ::pltxt2htm::details::optionally_to_html_list_ast(pltext.subview(current_index));
            opt_html_list_ast.has_value()) {
            auto&& [top_node, advance_count] = opt_html_list_ast.value();
            ::pltxt2htm::details::push_list_frame(call_stack, ::std::move(top_node));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = true};
        }
        // Check for an HTML <table> block at a block position (block-level tables).
        if (auto opt_html_table_ast = ::pltxt2htm::details::optionally_to_html_table_ast(pltext.subview(current_index));
            opt_html_table_ast.has_value()) {
            auto&& [raw_ast, advance_count] = opt_html_table_ast.value();
            call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                ::pltxt2htm::details::FrontendContextVariant{
                    ::pltxt2htm::details::ParserFrameContextWithTableInfo{::std::move(raw_ast)},
                    ::pltxt2htm::NodeKind::table},
                ::pltxt2htm::Ast{}));
            return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
                .advance_count = current_index + advance_count, .new_frame_been_pushed_into_call_stack = true};
        }
        return ::pltxt2htm::details::HtmlFindNextBlockAfterLineBreakResult{
            .advance_count = current_index, .new_frame_been_pushed_into_call_stack = false};
    }
}

/**
 * @brief Parse the top frame's text with the shared parser stack.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 * @param[in,out] call_stack Active parser call stack whose top frame holds the text to parse.
 * @return The AST built from the top frame.
 */
[[nodiscard]]
constexpr auto parse_pltxt_html(::pltxt2htm::details::CallStack<::pltxt2htm::details::ParserFrame>& call_stack) noexcept
    -> ::pltxt2htm::Ast {
entry:
    while (true) {
        // List frames hold an intermediate ListAst; iterate it like the main parser does.
        if (auto const list_tag_type = call_stack.current_frame().get_nested_tag_type();
            list_tag_type == ::pltxt2htm::NodeKind::list_ul || list_tag_type == ::pltxt2htm::NodeKind::list_ol) {
            auto&& list_frame = call_stack.current_frame();
            auto&& frame_list_ast = list_frame.as_list_info().list_ast;
            auto&& frame_iter = list_frame.as_list_info().iter;
            if (frame_iter == frame_list_ast.end()) {
                ::pltxt2htm::details::ParserFrame previous_frame(::std::move(list_frame));
                call_stack.discard_current_frame();
                if (call_stack.empty()) {
                    return ::std::move(previous_frame.subast);
                }
                if (list_tag_type == ::pltxt2htm::NodeKind::list_ul) {
                    call_stack.current_frame().subast.emplace_back(
                        ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::ListUl>(
                            ::std::move(previous_frame.subast)));
                }
                else {
                    call_stack.current_frame().subast.emplace_back(
                        ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::ListOl>(
                            ::std::move(previous_frame.subast), previous_frame.as_list_info().list_start));
                }
                goto entry;
            }
            switch (frame_iter->get_type()) {
            case ::pltxt2htm::details::ListNodeType::list_li: {
                call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                    ::pltxt2htm::details::FrontendContextVariant{
                        ::pltxt2htm::details::ParserFrameContextWithPltextInfo{frame_iter->get_text_view()},
                        ::pltxt2htm::NodeKind::list_li},
                    ::pltxt2htm::Ast{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
                // HTML lists never produce checkbox items, but the branch must exist for
                // exhaustive switch checking.
                call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                    ::pltxt2htm::details::FrontendContextVariant{
                        ::pltxt2htm::details::ParserFrameContextWithListLiCheckboxInfo{frame_iter->get_text_view(),
                                                                                       frame_iter->is_checked()},
                        ::pltxt2htm::NodeKind::list_li_checkbox},
                    ::pltxt2htm::Ast{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_ul: {
                call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                    ::pltxt2htm::details::FrontendContextVariant{
                        ::pltxt2htm::details::ParserFrameContextWithListInfo{::std::move(frame_iter->get_sublist())},
                        ::pltxt2htm::NodeKind::list_ul},
                    ::pltxt2htm::Ast{}));
                break;
            }
            case ::pltxt2htm::details::ListNodeType::list_ol: {
                call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                    ::pltxt2htm::details::FrontendContextVariant{
                        ::pltxt2htm::details::ParserFrameContextWithListInfo{::std::move(frame_iter->get_sublist()),
                                                                             frame_iter->get_start()},
                        ::pltxt2htm::NodeKind::list_ol},
                    ::pltxt2htm::Ast{}));
                break;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
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
        if (call_stack.current_frame().get_nested_tag_type() == ::pltxt2htm::NodeKind::table) {
            auto opt_table_ast = ::pltxt2htm::details::process_table_frame(call_stack);
            if (opt_table_ast.has_value()) {
                auto&& table_ast = opt_table_ast.value();
                return ::std::move(table_ast);
            }
            goto entry;
        }
        auto&& top_frame = call_stack.current_frame();
        auto&& current_index = top_frame.current_index;
        ::pltxt2htm::container::U8StringView const pltext{top_frame.get_pltext()};
        auto&& result = top_frame.subast;
        ::std::size_t const pltext_size{pltext.size()};

        // Check for block-level <p> tag at line start (start of input or after frame completion)
        auto&& [entry_advance, entry_new_frame] = ::pltxt2htm::details::html_find_next_block_after_line_break(
            pltext.subview(current_index), call_stack, result);
        current_index += entry_advance;
        if (entry_new_frame) {
            goto entry;
        }

        while (current_index < pltext_size) {
            char8_t const chr{pltext.index(current_index)};

            if (chr == u8'\n') {
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>());
                ++current_index;
                // Check for block-level <p> tag after newline
                auto&& [nl_advance, nl_new_frame] = ::pltxt2htm::details::html_find_next_block_after_line_break(
                    pltext.subview(current_index), call_stack, result);
                current_index += nl_advance;
                if (nl_new_frame) {
                    goto entry;
                }
                continue;
            }
            if (auto const opt_space_size = ::pltxt2htm::details::try_parse_space(pltext.subview(current_index));
                opt_space_size.has_value()) {
                auto const space_size = opt_space_size.value().get();
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Space>());
                current_index += space_size;
                continue;
            }
            if (chr == u8'&') {
                if (auto const opt_entity_len =
                        ::pltxt2htm::details::try_append_character_reference(pltext.subview(current_index), result);
                    opt_entity_len.has_value()) {
                    current_index += opt_entity_len.value();
                    continue;
                }
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Ampersand>());
                ++current_index;
                continue;
            }
            if (chr == u8'\'') {
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::SingleQuote>());
                ++current_index;
                continue;
            }
            if (chr == u8'\"') {
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::DoubleQuote>());
                ++current_index;
                continue;
            }
            if (chr == u8'>') {
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::GreaterThan>());
                ++current_index;
                continue;
            }
            if (chr == u8'\t') {
                result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Tab>());
                ++current_index;
                continue;
            }
            if (chr == u8'<') {
                pltxt2htm_assert(current_index < pltext_size, u8"Index of parser out of bound");

                if (current_index + 1 == pltext_size) {
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                switch (pltext.index(current_index + 1)) {
                case u8'a':
                    [[fallthrough]];
                case u8'A': {
                    if (auto a_tag = ::pltxt2htm::details::try_parse_html_a_tag(pltext.subview(current_index + 2));
                        a_tag.is_valid()) {
                        auto tag_len = a_tag.tag_len;
                        ::pltxt2htm::Url url = ::std::move(a_tag.url).value();
                        auto const internal = a_tag.internal;
                        current_index += tag_len + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlATagInfo{pltext.subview(current_index),
                                                                                         ::std::move(url), internal}},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'b':
                    [[fallthrough]];
                case u8'B': {
                    if (auto opt_br_tag_len =
                            ::pltxt2htm::details::try_parse_self_closing_tag<u8"r">(pltext.subview(current_index + 2));
                        opt_br_tag_len.has_value()) {
                        // The helper counts the bytes after "<b", so the two leading bytes put
                        // current_index right after the closing '>'; the block candidate starts there.
                        current_index += opt_br_tag_len.value().get() + 2;
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlBr>());
                        auto&& [br_advance, br_new_frame] = ::pltxt2htm::details::html_find_next_block_after_line_break(
                            pltext.subview(current_index), call_stack, result);
                        current_index += br_advance;
                        if (br_new_frame) {
                            goto entry;
                        }
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'c':
                    [[fallthrough]];
                case u8'C': {
                    // parsing: <code>$1</code> inline element (no language/class support)
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"ode">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing start tag <code> successed
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_code},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'd':
                    [[fallthrough]];
                case u8'D': {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"el">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_del},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'e':
                    [[fallthrough]];
                case u8'E': {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"m">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_em},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'i':
                    [[fallthrough]];
                case u8'I': {
                    // <input> is only recognized inside block-level <ul>/<ol> items
                    // (parsed by html_list.hh); inline occurrences are literal text.
                    if (auto opt_img_tag = ::pltxt2htm::details::try_parse_img_tag(pltext.subview(current_index + 2));
                        opt_img_tag.has_value()) {
                        auto&& [tag_len, src, alt] = opt_img_tag.value();
                        current_index += tag_len + 2;
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlImg>(
                            ::std::move(src), ::std::move(alt)));
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'm':
                    [[fallthrough]];
                case u8'M': {
                    if (auto opt_mark_tag = ::pltxt2htm::details::try_parse_mark_tag(pltext.subview(current_index + 2));
                        opt_mark_tag.has_value()) {
                        auto&& [tag_len, background_color] = opt_mark_tag.value();
                        current_index += tag_len + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithBackgroundColorInfo{
                                    pltext.subview(current_index), ::std::move(background_color)},
                                ::pltxt2htm::NodeKind::html_mark},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8's':
                    [[fallthrough]];
                case u8'S': {
                    if (auto opt_span_tag = ::pltxt2htm::details::try_parse_span_tag(pltext.subview(current_index + 2));
                        opt_span_tag.has_value()) {
                        auto&& [tag_len, color, font_size, vertical_align] = opt_span_tag.value();
                        current_index += tag_len + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlSpanInfo{
                                    pltext.subview(current_index), ::std::move(color), ::std::move(font_size),
                                    ::std::move(vertical_align)},
                                ::pltxt2htm::NodeKind::html_span},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"trong">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_strong},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"up">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_sup},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<u8"ub">(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_sub},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_s},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8't':
                    [[fallthrough]];
                case u8'T': {
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'u':
                    [[fallthrough]];
                case u8'U': {
                    // <ul> is a block-level list; inline occurrences are plain literal text
                    // (except <u> underline below).
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag(pltext.subview(current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.value() + 2;
                        call_stack.push_frame(::pltxt2htm::details::ParserFrame(
                            ::pltxt2htm::details::FrontendContextVariant{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext.subview(current_index)},
                                ::pltxt2htm::NodeKind::html_u},
                            ::pltxt2htm::Ast{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'!': {
                    if (::pltxt2htm::details::is_prefix_match<u8"--">(pltext.subview(current_index + 2))) {
                        ::std::size_t comment_end{current_index + 4};
                        ::pltxt2htm::Ast subast{};

                        for (; comment_end < pltext_size; ++comment_end) {
                            if (::pltxt2htm::details::is_prefix_match<u8"-->">(pltext.subview(comment_end))) {
                                break;
                            }
                            subast.append_text(pltext.index(comment_end));
                        }

                        current_index = comment_end + 2;
                        result.push_back(
                            ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlNote>(::std::move(subast)));
                        ++current_index;
                        continue;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }

                case u8'/': {
                    auto&& frame = call_stack.current_frame();
                    switch (frame.get_nested_tag_type()) {
                    case ::pltxt2htm::NodeKind::html_span: {
                        auto&& active_frame_data = frame.as_html_span_info();
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"span">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSpan staged_node(::std::move(result), ::std::move(active_frame_data.color),
                                                              active_frame_data.font_size,
                                                              active_frame_data.vertical_align);
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSpan>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_a: {
                        auto&& active_frame_data = frame.as_html_a_tag_info();
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"a">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlA staged_node(::std::move(result), ::std::move(active_frame_data.url),
                                                           active_frame_data.internal);
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlA>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_div: {
                        auto&& active_frame_data = frame.as_margins_info();
                        // parsing </div>
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"div">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDiv staged_node(::std::move(result), active_frame_data.left,
                                                             active_frame_data.right);
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlDiv>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_p: {
                        auto&& active_frame_data = frame.as_align_info();
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"p">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            auto const align = active_frame_data.align;
                            ::pltxt2htm::HtmlP staged_node(::std::move(result), align);
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlP>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h1: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h1">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH1 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH1>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h2: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h2">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH2 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH2>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h3: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h3">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH3 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH3>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h4: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h4">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH4 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH4>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h5: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h5">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH5 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH5>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_h6: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"h6">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlH6 staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH6>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_del: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"del">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlDel staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlDel>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_code: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"code">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlCode staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlCode>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_u: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"u">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlU staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlU>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_s: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"s">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlS staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlS>(::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sup: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"sup">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSup staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSup>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_sub: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"sub">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSub staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSub>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_em: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"em">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlEm staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlEm>(
                                ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_strong: {
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"strong">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlStrong staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlStrong>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_mark: {
                        auto&& active_frame_data = frame.as_background_color_info();
                        if (auto opt_tag_len =
                                ::pltxt2htm::details::try_parse_bare_tag<u8"mark">(pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlMark staged_node(::std::move(result),
                                                              ::std::move(active_frame_data.background_color));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlMark>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::html_blockquote: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<u8"blockquote">(
                                pltext.subview(current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlBlockquote staged_node(::std::move(result));
                            call_stack.discard_current_frame();
                            auto& parent_frame = call_stack.current_frame();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlBlockquote>(
                                    ::std::move(staged_node)));
                            parent_frame.current_index += staged_index + opt_tag_len.value() + 2;
                            goto entry;
                        }
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::group:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_li:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_li_checkbox:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_ol:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::list_ul:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_caption:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_th:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_td: {
                        result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                        ++current_index;
                        continue;
                    }
                    case ::pltxt2htm::NodeKind::text:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::invalid_utf8:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::url:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::line_break:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::space:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::ampersand:
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
                    case ::pltxt2htm::NodeKind::unity_color:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_size:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_voffset:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_align:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_mark:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_margin:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_link:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_b:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::unity_i:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_a:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_experiment:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_discussion:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_experiments:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_discussions:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_user:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_external:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_trigger:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_internal:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_project:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_visitor:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_author:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::pl_macro_coauthors:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_br:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_hr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_tr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_thead:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_tbody:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_tfoot:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_colgroup:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::table_col:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_img:
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
                    case ::pltxt2htm::NodeKind::md_escape:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_hr:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::code_fence:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_del:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_link:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_image:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_block_quotes:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_latex_inline:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::md_latex_block:
                        [[fallthrough]];
                    case ::pltxt2htm::NodeKind::html_note:
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                        [[fallthrough]];
                    default:
#endif
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected node kind in html parser end-tag switch");
                        }
                    }
                    pltxt2htm_unreachable(u8"Unreachable after end-tag inner switch");
                }

                default: {
                    result.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LessThan>());
                    ++current_index;
                    continue;
                }
                }

                pltxt2htm_unreachable(u8"Unreachable after outer switch");
            }
            auto advance_count = ::pltxt2htm::details::parse_utf8_code_point(pltext.subview(current_index), result);
            current_index += advance_count;
            continue;
        }

        {
            ::pltxt2htm::details::ParserFrame frame{call_stack.pop_frame()};
            if (call_stack.empty()) {
                return ::std::move(frame.subast);
            }
            auto&& subast = frame.subast;
            auto&& parent_frame = call_stack.current_frame();
            auto&& parent_ast = parent_frame.subast;
            parent_frame.current_index += pltext_size;
            switch (frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeKind::html_span: {
                auto&& active_frame_data = frame.as_html_span_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSpan>(
                    ::std::move(subast), ::std::move(active_frame_data.color), active_frame_data.font_size,
                    active_frame_data.vertical_align));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                auto&& active_frame_data = frame.as_margins_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlDiv>(
                    ::std::move(subast), active_frame_data.left, active_frame_data.right));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                auto&& active_frame_data = frame.as_html_a_tag_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlA>(
                    ::std::move(subast), ::std::move(active_frame_data.url), active_frame_data.internal));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto&& active_frame_data = frame.as_align_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlP>(
                    ::std::move(subast), active_frame_data.align));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH1>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH2>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH3>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH4>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH5>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH6>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlDel>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlCode>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_u: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlU>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_s: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlS>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSup>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlSub>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlEm>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlStrong>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                auto&& active_frame_data = frame.as_background_color_info();
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlMark>(
                    ::std::move(subast), ::std::move(active_frame_data.background_color)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                auto&& active_frame_data = frame.as_list_li_checkbox();
                auto const checked = active_frame_data.checked;
                parent_ast.push_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::ListLiCheckbox>(
                    ::std::move(subast), checked));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::ListLi>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlBlockquote>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto&& active_frame_data = frame.as_cell();
                auto align = active_frame_data.align;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::TableTd>(::std::move(subast), align));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                auto&& active_frame_data = frame.as_cell();
                auto align = active_frame_data.align;
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::TableTh>(::std::move(subast), align));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::TableCaption>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::text:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::invalid_utf8:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::group:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::url:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::line_break:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::space:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::ampersand:
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
            case ::pltxt2htm::NodeKind::unity_color:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_size:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_voffset:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_align:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_mark:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_margin:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::unity_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_experiment:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_discussion:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_experiments:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_discussions:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_user:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_external:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_trigger:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_internal:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_project:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_visitor:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_author:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_macro_coauthors:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_br:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_note:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_thead:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tbody:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tfoot:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_colgroup:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_col:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_img:
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
            case ::pltxt2htm::NodeKind::md_escape:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::code_fence:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_image:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_latex_inline:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_latex_block:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table:
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                [[fallthrough]];
            default:
#endif
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected node kind in html parser unclosed-tag fallback");
                }
            }
            pltxt2htm_unreachable(u8"Unreachable after html unclosed-tag switch");
        }
    }
}

/**
 * @brief Parse HTML input text into an Abstract Syntax Tree.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 * @param[in] html_text Text whose block structure comes from HTML tags.
 * @return An AST representing the parsed structure of @p html_text.
 */
[[nodiscard]]
constexpr auto parse_pltxt_html(::pltxt2htm::container::U8StringView html_text) noexcept -> ::pltxt2htm::Ast {
    ::pltxt2htm::details::CallStack<::pltxt2htm::details::ParserFrame> call_stack{};

    call_stack.push_frame(::pltxt2htm::details::ParserFrame(
        ::pltxt2htm::details::FrontendContextVariant{::pltxt2htm::details::ParserFrameContextWithPltextInfo{html_text},
                                                     ::pltxt2htm::NodeKind::group},
        ::pltxt2htm::Ast{}));

    auto result = parse_pltxt_html(call_stack);

    pltxt2htm_assert(call_stack.empty(), u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
