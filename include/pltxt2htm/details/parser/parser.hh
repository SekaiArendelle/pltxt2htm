#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "md_list.hh"
#include "md_table.hh"
#include "frame_concext.hh"
#include "try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

struct DevilStuffAfterLineBreakResult {
    ::std::size_t forward_index;
    bool new_frame_been_pushed_into_call_stack;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto devil_stuff_after_line_break(
    ::fast_io::u8string_view pltext, ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>>& call_stack,
    ::pltxt2htm::Ast<ndebug>& result) noexcept -> ::pltxt2htm::details::DevilStuffAfterLineBreakResult {
    ::std::size_t current_index{};
    while (true) {
        if (current_index >= pltext.size()) {
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index,
                                                                        .new_frame_been_pushed_into_call_stack = false};
        }

        if (auto opt_md_atx_heading = ::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_atx_heading.has_value()) {
            auto&& [start_index, sublength, forward_index, md_atx_heading_type] =
                opt_md_atx_heading.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            if (sublength != 0) {
                auto subtext =
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + start_index, sublength);
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(subtext, md_atx_heading_type));
                current_index += forward_index;
                return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{
                    .forward_index = current_index, .new_frame_been_pushed_into_call_stack = true};
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
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                }
            }
            current_index += forward_index;
            continue;
        }
        if (auto opt_len = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_len.has_value()) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdHr{}));
            current_index += opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            continue;
        }
        if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_code_fence.has_value()) {
            auto&& [node, forward_index] = opt_code_fence.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            result.push_back(::std::move(node));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = false};
        }
        if (auto opt_block_quote = ::pltxt2htm::details::try_parse_md_block_quotes<ndebug>(pltext);
            opt_block_quote.has_value()) {
            auto&& [forward_index, subpltext] =
                opt_block_quote.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::std::move(subpltext)));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_md_list_ast = ::pltxt2htm::details::optionally_to_md_list_ast<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_list_ast.has_value()) {
            auto&& [md_list_ast, forward_index, item_kind] =
                opt_md_list_ast.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(item_kind, ::std::move(md_list_ast)));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = true};
        }
        if (auto opt_md_table_raw = ::pltxt2htm::details::try_parse_md_table_raw<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_table_raw.has_value()) {
            auto&& [raw_ast, forward_index] =
                opt_md_table_raw.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::pltxt2htm::NodeKind::md_table,
                                                                             ::std::move(raw_ast)));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = true};
        }
        return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index,
                                                                    .new_frame_been_pushed_into_call_stack = false};
    }
}

/**
 * @brief Parse pl-text to nodes.
 * @tparam ndebug Contract checking mode; `::pltxt2htm::Contracts::ignore` disables checks.
 * @param call_stack: use `call_stack` + `goto entry` to avoid stack overflow.
 * @return Quantum-Physics text's ast.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>>& call_stack) noexcept
    -> ::pltxt2htm::Ast<ndebug> {
entry:
    if (::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type() == ::pltxt2htm::NodeKind::md_ul) {
        // ::pltxt2htm::details::MdListAst to ::pltxt2htm::Ast<ndebug>
        auto&& frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& frame_md_list_ast = frame.get_md_list_ast();
        auto&& frame_iter = frame.get_md_list_iter();
        if (frame_iter == frame_md_list_ast.end()) {
            ::pltxt2htm::details::ParserFrameContext<ndebug> previous_frame(::std::move(frame));
            call_stack.pop();
            if (call_stack.empty()) {
                return ::std::move(previous_frame.subast);
            }
            ::pltxt2htm::details::stack_top<ndebug>(call_stack)
                .subast.emplace_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdUl<ndebug>{::std::move(previous_frame.subast)}));
            // Given "before\n- item\nbetween\n+ another\n- last\nafter":
            //   devil_stuff_after_line_break first sees "- item\nbetween\n...",
            //   but optionally_to_md_list_ast stops at "between" (not a list marker),
            //   returning only "- item\n".  After that <ul> pops, the remaining text
            //   "between\n+ another\n- last\nafter" is processed by the parent loop.
            //   When the parent hits "\n" before "+ another",
            //   devil_stuff_after_line_break is called again and sees
            //   "+ another\n- last\nafter".  optionally_to_md_list_ast stops at
            //   "- last" (different marker at root level), returning only
            //   "+ another\n".  After that <ul> pops, the parent has unconsumed
            //   text "- last\nafter".  Without re-scanning here, "- last" would be
            //   treated as plain text "--last" instead of a new list.
            //
            // When the parent is another md_ul/md_ol (child nested inside a
            // parent list): no re-scan needed -- the parent iterates its own AST.
            auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            if (::pltxt2htm::details::is_md_list_ul_or_ol_type(parent_frame.get_nested_tag_type()) == false) {
                ::std::size_t& parent_index = parent_frame.current_index;
                auto parent_text = parent_frame.get_pltext();
                auto&& [fwd, restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(parent_text, parent_index), call_stack,
                    parent_frame.subast);
                parent_index += fwd;
                if (restart) {
                    goto entry;
                }
            }
            goto entry;
        }
        switch (frame_iter->get_type()) {
        case ::pltxt2htm::details::MdListNodeType::text: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(frame_iter->get_text_view(),
                                                                             ::pltxt2htm::NodeKind::md_li));
            {
                auto& new_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                new_frame.checkbox = frame_iter->is_checkbox();
                new_frame.checked = frame_iter->is_checked();
            }
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::pltxt2htm::NodeKind::md_ul,
                                                                             ::std::move(frame_iter->get_sublist())));
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::pltxt2htm::NodeKind::md_ol,
                                                                             ::std::move(frame_iter->get_sublist())));
            break;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }

        ++frame_iter;
        goto entry;
    }
    if (::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type() == ::pltxt2htm::NodeKind::md_ol) {
        // ::pltxt2htm::details::MdListAst to ::pltxt2htm::Ast<ndebug>
        auto&& frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& frame_md_list_ast = frame.get_md_list_ast();
        auto&& frame_iter = frame.get_md_list_iter();
        if (frame_iter == frame_md_list_ast.end()) {
            ::pltxt2htm::details::ParserFrameContext<ndebug> previous_frame(::std::move(frame));
            call_stack.pop();
            if (call_stack.empty()) {
                return ::std::move(previous_frame.subast);
            }
            ::pltxt2htm::details::stack_top<ndebug>(call_stack)
                .subast.emplace_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdOl<ndebug>{::std::move(previous_frame.subast)}));
            // Same logic as the MdUl case above -- re-scan only when the parent is a
            // plain-text frame, not another md_ul/md_ol.
            auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            if (::pltxt2htm::details::is_md_list_ul_or_ol_type(parent_frame.get_nested_tag_type()) == false) {
                ::std::size_t& parent_index = parent_frame.current_index;
                auto parent_text = parent_frame.get_pltext();
                auto&& [fwd, restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(parent_text, parent_index), call_stack,
                    parent_frame.subast);
                parent_index += fwd;
                if (restart) {
                    goto entry;
                }
            }
            goto entry;
        }
        switch (frame_iter->get_type()) {
        case ::pltxt2htm::details::MdListNodeType::text: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(frame_iter->get_text_view(),
                                                                             ::pltxt2htm::NodeKind::md_li));
            {
                auto& new_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                new_frame.checkbox = frame_iter->is_checkbox();
                new_frame.checked = frame_iter->is_checked();
            }
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::pltxt2htm::NodeKind::md_ul,
                                                                             ::std::move(frame_iter->get_sublist())));
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(::pltxt2htm::NodeKind::md_ol,
                                                                             ::std::move(frame_iter->get_sublist())));
            break;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
        ++frame_iter;
        goto entry;
    }
    if (::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type() == ::pltxt2htm::NodeKind::md_table) {
        auto&& frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& raw_ast = frame.get_md_table_raw_ast();
        auto state = frame.get_md_table_state();
        auto row_index = frame.get_md_table_row_index();
        auto cell_index = frame.get_md_table_cell_index();

        switch (state) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdTableParsePhase::header: {
            if (cell_index < raw_ast.header_cells_count()) {
                auto const& cell = raw_ast.header_cell_at(cell_index);
                call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                    ::fast_io::u8string_view{cell.text.data(), cell.text.size()}, cell.align,
                    ::pltxt2htm::NodeKind::md_th));
                frame.set_md_table_cell_index(cell_index + 1);
                goto entry;
            }
            frame.set_md_table_state(::pltxt2htm::details::MdTableParsePhase::body);
            frame.set_md_table_row_index(0);
            frame.set_md_table_cell_index(0);
            goto entry;
        }
        case ::pltxt2htm::details::MdTableParsePhase::body: {
            if (row_index < raw_ast.body_rows_count()) {
                if (cell_index < raw_ast.body_cells_count()) {
                    auto const& cell = raw_ast.body_cell_at(row_index, cell_index);
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::fast_io::u8string_view{cell.text.data(), cell.text.size()}, cell.align,
                        ::pltxt2htm::NodeKind::md_td));
                    frame.set_md_table_cell_index(cell_index + 1);
                    goto entry;
                }
                frame.set_md_table_row_index(row_index + 1);
                frame.set_md_table_cell_index(0);
                goto entry;
            }
            frame.set_md_table_state(::pltxt2htm::details::MdTableParsePhase::finish);
            goto entry;
        }
        case ::pltxt2htm::details::MdTableParsePhase::finish: {
            auto previous_frame = ::std::move(frame);
            call_stack.pop();

            ::pltxt2htm::Ast<ndebug> flat_ast = ::std::move(previous_frame.subast);
            auto&& prev_raw_ast = previous_frame.get_md_table_raw_ast();
            ::pltxt2htm::Ast<ndebug> table_ast{};
            ::std::size_t col{};

            // build <thead><tr>
            if (prev_raw_ast.header_cells_count() > 0) {
                ::pltxt2htm::Ast<ndebug> header_tr_ast{};
                for (; col < prev_raw_ast.header_cells_count() && col < flat_ast.size(); ++col) {
                    header_tr_ast.push_back(::std::move(::pltxt2htm::details::vector_index<ndebug>(flat_ast, col)));
                }
                ::pltxt2htm::Ast<ndebug> thead_ast{};
                thead_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTr<ndebug>{::std::move(header_tr_ast)}));
                table_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdThead<ndebug>{::std::move(thead_ast)}));
            }

            // build <tbody>
            if (col < flat_ast.size()) {
                ::pltxt2htm::Ast<ndebug> tbody_ast{};
                ::std::size_t body_row_index{};
                while (col < flat_ast.size() && body_row_index < prev_raw_ast.body_rows_count()) {
                    ::pltxt2htm::Ast<ndebug> tr_ast{};
                    for (::std::size_t c{}; c < prev_raw_ast.body_cells_count() && col < flat_ast.size(); ++c, ++col) {
                        tr_ast.push_back(::std::move(::pltxt2htm::details::vector_index<ndebug>(flat_ast, col)));
                    }
                    ++body_row_index;
                    tbody_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTr<ndebug>{::std::move(tr_ast)}));
                }
                table_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTbody<ndebug>{::std::move(tbody_ast)}));
            }

            if (call_stack.empty()) {
                return table_ast;
            }

            auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            parent_frame.subast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTable<ndebug>{::std::move(table_ast)}));
            goto entry;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }

        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    auto&& top_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
    auto&& current_index = top_frame.current_index;
    ::fast_io::u8string_view pltext{top_frame.get_pltext()};
    auto&& result = top_frame.subast;
    ::std::size_t const pltext_size{pltext.size()};

    if (top_frame.get_nested_tag_type() == ::pltxt2htm::NodeKind::md_block_quotes && current_index == 0) {
        // https://spec.commonmark.org/0.31.2/#example-228
        // to support parsing md-atx-heading e.t.c inside md-block-quotes
        auto&& [forward_index, require_restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), call_stack, result);
        current_index += forward_index;
        if (require_restart) {
            goto entry;
        }
    }

    while (current_index < pltext_size) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8'\n') {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));

            auto&& [forward_index, require_restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), call_stack, result);
            current_index += forward_index;
            if (require_restart) {
                current_index += 1;
                goto entry;
            }
            ++current_index;
            continue;
        }
        if (chr == u8' ') {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
            ++current_index;
            continue;
        }
        if (chr == u8'&') {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\'') {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuotationMark{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\"') {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuotationMark{}));
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
        if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}));
                break;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char<ndebug>(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                result.push_back(::std::move(escape_node.template value<ndebug == ::pltxt2htm::Contracts::ignore>()));
                ++current_index;
            }
            else {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}));
            }
            ++current_index;
            continue;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{project}">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            constexpr auto length_of_literal_string = ::std::size_t{9};
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroProject{}));
            current_index += length_of_literal_string;
            continue;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{visitor}">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            constexpr auto length_of_literal_string = ::std::size_t{9};
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroVisitor{}));
            current_index += length_of_literal_string;
            continue;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{author}">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            constexpr auto length_of_literal_string = ::std::size_t{8};
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroAuthor{}));
            current_index += length_of_literal_string;
            continue;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"{coauthors}">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            constexpr auto length_of_literal_string = ::std::size_t{11};
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlMacroCoauthors{}));
            current_index += length_of_literal_string;
            continue;
        }
        if (auto opt_triple_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"***">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_triple_emphasis_asterisk.has_value()) {
            // parsing markdown ***example***
            ::std::size_t const forward_index{
                opt_triple_emphasis_asterisk.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 3, forward_index),
                ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk));
            current_index += forward_index + 6;
            goto entry;
        }
        if (auto opt_double_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"**">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_double_emphasis_asterisk.has_value()) {
            // parsing markdown **example**
            ::std::size_t const forward_index{
                opt_double_emphasis_asterisk.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2, forward_index),
                ::pltxt2htm::NodeKind::md_double_emphasis_asterisk));
            current_index += forward_index + 4;
            goto entry;
        }
        if (auto opt_single_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"*">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_single_emphasis_asterisk.has_value()) {
            // parsing markdown *example*
            ::std::size_t const forward_index{
                opt_single_emphasis_asterisk.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1, forward_index),
                ::pltxt2htm::NodeKind::md_single_emphasis_asterisk));
            current_index += forward_index + 2;
            goto entry;
        }
        if (auto opt_triple_emphasis_underscore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"___">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_triple_emphasis_underscore.has_value()) {
            // parsing markdown ___example___
            ::std::size_t const forward_index{
                opt_triple_emphasis_underscore.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 3, forward_index),
                ::pltxt2htm::NodeKind::md_triple_emphasis_underscore));
            current_index += forward_index + 6;
            goto entry;
        }
        if (auto opt_double_emphasis_undersore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"__">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_double_emphasis_undersore.has_value()) {
            // parsing markdown __example__
            ::std::size_t const forward_index{
                opt_double_emphasis_undersore.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2, forward_index),
                ::pltxt2htm::NodeKind::md_double_emphasis_underscore));
            current_index += forward_index + 4;
            goto entry;
        }
        if (auto opt_single_emphasis_undersore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"_">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_single_emphasis_undersore.has_value()) {
            // parsing markdown _example_
            ::std::size_t const forward_index{
                opt_single_emphasis_undersore.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1, forward_index),
                ::pltxt2htm::NodeKind::md_single_emphasis_underscore));
            current_index += forward_index + 2;
            goto entry;
        }
        if (auto opt_md_del = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8"~~">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_del.has_value()) {
            // parsing markdown ~~example~~
            ::std::size_t const forward_index{opt_md_del.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2, forward_index),
                ::pltxt2htm::NodeKind::md_del));
            current_index += forward_index + 4;
            goto entry;
        }
        if (auto opt_code_span_3_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"```">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_code_span_3_backtick.has_value()) {
            // parsing markdown ```example```
            auto&& [forward_index, subast] =
                opt_code_span_3_backtick.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan3Backtick<ndebug>{::std::move(subast)}));
            continue;
        }
        if (auto opt_code_span_2_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"``">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_code_span_2_backtick.has_value()) {
            // parsing markdown ``example``
            auto&& [forward_index, subast] =
                opt_code_span_2_backtick.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan2Backtick<ndebug>{::std::move(subast)}));
            continue;
        }
        if (auto opt_code_span_1_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8"`">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_code_span_1_backtick.has_value()) {
            // parsing markdown `example`
            auto&& [forward_index, subast] =
                opt_code_span_1_backtick.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            result.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdCodeSpan1Backtick<ndebug>{::std::move(subast)}));
            continue;
        }
        if (auto opt_md_latex_block_dollar = ::pltxt2htm::details::try_parse_md_latex_block_dollar<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_latex_block_dollar.has_value()) {
            auto&& [forward_index, subast] =
                opt_md_latex_block_dollar.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexBlock<ndebug>{::std::move(subast)}));
            continue;
        }
        if (auto opt_md_latex_inline = ::pltxt2htm::details::try_parse_md_latex_inline<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_latex_inline.has_value()) {
            auto&& [forward_index, subast] =
                opt_md_latex_inline.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdLatexInline<ndebug>{::std::move(subast)}));
            continue;
        }
        if (auto opt_md_link = ::pltxt2htm::details::try_parse_md_link<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_link.has_value()) {
            auto&& [forward_index, url_text, url_link] =
                opt_md_link.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
            call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(url_text, ::std::move(url_link)));
            goto entry;
        }
        if (auto opt_md_image = ::pltxt2htm::details::try_parse_md_image<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_md_image.has_value()) {
            auto&& [forward_index, text, link] =
                opt_md_image.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += forward_index;
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
            switch (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1)) {
            case u8'a':
                [[fallthrough]];
            case u8'A': {
                // parsing pl <a>$1</a> tag (not html <a> tag)
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_a));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'b':
                [[fallthrough]];
            case u8'B': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // parsing pl&html <b> tag
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_b));
                    goto entry;
                }
                if (auto opt_br_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"r">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_br_tag_len.has_value()) {
                    current_index += opt_br_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBr{}));

                    auto&& [forward_index, require_restart] =
                        ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), call_stack,
                            result);
                    current_index += forward_index;
                    if (require_restart) {
                        current_index += 1;
                        goto entry;
                    }
                    ++current_index;
                    continue;
                }
                if (auto opt_blockquote_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"lockquote">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_blockquote_tag.has_value()) {
                    // parsing pl&html <b> tag
                    current_index += opt_blockquote_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_blockquote));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'c':
                [[fallthrough]];
            case u8'C': {
                // parsing: <color=$1>$2</color>
                if (auto opt_color_tag = ::pltxt2htm::details::try_parse_color_tag<ndebug>(
                        u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_color_tag.has_value()) {
                    auto&& [tag_len, color] = opt_color_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    current_index += tag_len + 3;
                    // parsing start tag <color> successed
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_color, ::std::move(color)));
                    goto entry;
                }
                if (auto opt_code_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ode">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_code_tag.has_value()) {
                    // parsing html <code> tag
                    current_index += opt_code_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_code));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_caption_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    // parsing html <caption> tag
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_caption));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_colgroup_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    // parsing html <colgroup> tag
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_colgroup));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_col_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    // parsing html <col> self-closing tag
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCol{}));
                    ++current_index;
                    continue;
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
                    // parsing <del>$1</del>
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_del));
                    goto entry;
                }
                // parsing: <discussion=$1>$2</discussion>
                if (auto opt_discussion_tag =
                        ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"iscussion">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            },
                            call_stack);
                    opt_discussion_tag.has_value()) {
                    auto&& [tag_len, id] =
                        opt_discussion_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_discussion, ::std::move(id)));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'e':
                [[fallthrough]];
            case u8'E': {
                if (auto opt_experiment_tag =
                        ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"xperiment">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            },
                            call_stack);
                    opt_experiment_tag.has_value()) {
                    // parsing: <experiment=$1>$2</experiment>
                    auto&& [tag_len, id] =
                        opt_experiment_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_experiment, ::std::move(id)));
                    goto entry;
                }
                if (auto opt_external_tag = ::pltxt2htm::details::try_parse_external_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2), call_stack);
                    opt_external_tag.has_value()) {
                    auto&& [tag_len, url] = opt_external_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_external, ::std::move(url)));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"m">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_em));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'h':
                [[fallthrough]];
            case u8'H': {
                if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"1">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h1_tag_len.has_value()) {
                    // parsing html <h1> tag
                    current_index += opt_h1_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h1));
                    goto entry;
                }
                if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"2">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h2_tag_len.has_value()) {
                    // parsing html <h2> tag
                    current_index += opt_h2_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h2));
                    goto entry;
                }
                if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"3">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h3_tag_len.has_value()) {
                    // parsing html <h3> tag
                    current_index += opt_h3_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h3));
                    goto entry;
                }
                if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"4">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h4_tag_len.has_value()) {
                    // parsing html <h4> tag
                    current_index += opt_h4_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h4));
                    goto entry;
                }
                if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"5">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h5_tag_len.has_value()) {
                    // parsing html <h5> tag
                    current_index += opt_h5_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h5));
                    goto entry;
                }
                if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"6">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h6_tag_len.has_value()) {
                    // parsing html <h6> tag
                    current_index += opt_h6_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_h6));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8"r">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlHr{}));
                    ++current_index;
                    continue;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'i':
                [[fallthrough]];
            case u8'I': {
                // parsing pl&html <i>$1</i> tag
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_i));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'l':
                [[fallthrough]];
            case u8'L': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_li_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_li));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'o':
                [[fallthrough]];
            case u8'O': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"l">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_ol));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'p':
                [[fallthrough]];
            case u8'P': {
                // parsing html <p></p> tag
                if (auto opt_p_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_p_tag_len.has_value()) {
                    current_index += opt_p_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_p));
                    goto entry;
                }
                if (auto opt_pre_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"re">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_pre_tag_len.has_value()) {
                    current_index += opt_pre_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_pre));
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
                if (auto opt_size_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8"ize">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept { return u8'0' <= u8chr && u8chr <= u8'9'; });
                    opt_size_tag.has_value()) {
                    auto&& [tag_len, id_] = opt_size_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    ::exception::optional<::std::size_t> id{
                        ::pltxt2htm::details::u8str2size_t(::fast_io::u8string_view{id_.data(), id_.size()})};
                    if (!id.has_value()) [[unlikely]] {
                        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }

                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_size, id.template value<ndebug == ::pltxt2htm::Contracts::ignore>()));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"trong">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // HTML <strong> tag
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_strong));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8't':
                [[fallthrough]];
            case u8'T': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"able">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_table));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_thead_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_thead));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tbody_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_tbody));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tfoot_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_tfoot));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tr_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_tr));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_th_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_th));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_td_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type());
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_td));
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
                if (auto opt_user_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8"ser">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept {
                            return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                        });
                    opt_user_tag.has_value()) {
                    auto&& [tag_len, id] = opt_user_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::pl_user, ::std::move(id)));
                    goto entry;
                }
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"l">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                    call_stack.push(::pltxt2htm::details::ParserFrameContext<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeKind::html_ul));
                    goto entry;
                }
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }

            case u8'!': {
                // parsing: <!--$1-->
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8"--">(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))) {
                    // Find the closing -->
                    ::std::size_t comment_end{current_index + 4}; // Position after <!--
                    ::pltxt2htm::Ast<ndebug> subast{};

                    for (; comment_end < pltext_size; ++comment_end) {
                        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"-->">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, comment_end))) {
                            break;
                        }
                        subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{
                            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, comment_end)}));
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
                auto&& frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                switch (frame.get_nested_tag_type()) /* -Werror=switch */ {
                case ::pltxt2htm::NodeKind::pl_color: {
                    // parsing </color> or </a>
                    ::exception::optional<::std::size_t> opt_tag_len{
                        ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"color">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))};
                    if (!opt_tag_len.has_value()) {
                        opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    }
                    if (opt_tag_len.has_value()) {
                        // parsing end tag </color> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlColor staged_node(::std::move(result),
                                                         ::std::move(frame.get_equal_sign_tag_id()));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlColor<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_a: {
                    // parsing </color> or </a>
                    ::exception::optional<::std::size_t> opt_tag_len{
                        ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"color">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))};
                    if (!opt_tag_len.has_value()) {
                        opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"a">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    }
                    if (opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlA staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlA<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_experiment: {
                    // parsing </experiment>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"experiment">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlExperiment staged_node(::std::move(result),
                                                              ::std::move(frame.get_equal_sign_tag_id()));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::PlExperiment<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_discussion: {
                    // parsing </discussion>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"discussion">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlDiscussion staged_node(::std::move(result),
                                                              ::std::move(frame.get_equal_sign_tag_id()));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::PlDiscussion<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_external: {
                    // parsing </external>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"external">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlExternal staged_node(::std::move(result),
                                                            ::std::move(frame.get_external_tag_url()));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlExternal<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_user: {
                    // parsing </user>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"user">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlUser staged_node(::std::move(result),
                                                        ::std::move(frame.get_equal_sign_tag_id()));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlUser<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_size: {
                    // parsing </size>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"size">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlSize staged_node(::std::move(result), frame.get_pl_size_tag_id());
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlSize<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_b: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"b">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </b> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlB staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlB<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::pl_i: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"i">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::PlI staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlI<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </p> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlP staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h1> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH1 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH1<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h2> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH2 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH2<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h3> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH3 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH3<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h4> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH4 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH4<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h5> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH5 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH5<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </h6> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlH6 staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlH6<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </del> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlDel staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlDel<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_note:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }
                case ::pltxt2htm::NodeKind::html_em: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"em">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </em> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlEm staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </strong> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlStrong staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlStrong<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_ul: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ul">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </ul> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlUl staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlUl<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_ol: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ol">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </ol> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlOl staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlOl<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_li: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"li">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </li> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlLi staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlLi<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </code> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlCode staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCode<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_pre: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"pre">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </pre> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlPre staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlPre<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_table: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"table">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </table> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTable staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTable<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_tr: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"tr">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </tr> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTr staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_td: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"td">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </td> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTd staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTd<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_th: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"th">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </th> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTh staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTh<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_thead: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"thead">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </thead> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlThead staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlThead<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_tbody: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"tbody">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </tbody> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTbody staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTbody<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_tfoot: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"tfoot">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </tfoot> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlTfoot staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTfoot<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_caption: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"caption">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </caption> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlCaption staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(
                            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCaption<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::html_colgroup: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"colgroup">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </colgroup> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlColgroup staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::HtmlColgroup<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
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
                        // parsing end tag </blockquote> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::HtmlBlockquote staged_node(::std::move(result));
                        call_stack.pop();
                        auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                        parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(staged_node)}));
                        parent_frame.current_index +=
                            staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::md_block_quotes:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_code_fence_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_code_fence_tilde:
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
                case ::pltxt2htm::NodeKind::u8char:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::invalid_u8char:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::text:
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
                case ::pltxt2htm::NodeKind::html_br:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::html_hr:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::html_col:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_li:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_backslash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_exclamation:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_double_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_hash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_dollar:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_percent:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_ampersand:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_single_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_left_paren:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_right_paren:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_asterisk:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_plus:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_comma:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_hyphen:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_dot:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_slash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_colon:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_semicolon:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_less_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_equals:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_greater_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_question:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_at:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_left_bracket:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_right_bracket:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_caret:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_underscore:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_left_brace:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_pipe:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_right_brace:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_escape_tilde:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_hr:
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
                case ::pltxt2htm::NodeKind::md_latex_block: {
                    // relate to 0041.fuzzing-crash3
                    // any tag contains `</` context would hit this branch
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }
                case ::pltxt2htm::NodeKind::md_image:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::pl_macro_project:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::pl_macro_visitor:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::pl_macro_author:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::pl_macro_coauthors:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_table:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_thead:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_tbody:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_tr:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_th:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_td:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_ul:
                    [[fallthrough]];
                case ::pltxt2htm::NodeKind::md_ol:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }
                }
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }

            default: {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                ++current_index;
                continue;
            }
            }

            ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
        }
        auto forward_index = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), result);
        current_index += forward_index;
        continue;
    }

    {
        ::pltxt2htm::details::ParserFrameContext<ndebug> frame(
            ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack)));
        ::std::size_t const staged_index = pltext_size;
        call_stack.pop();
        if (call_stack.empty()) {
            // Considering the following markdown:
            // ```md
            // <b>e</b>xample
            // ```
            // Text without any tag in the end will hit this branch.
            return ::std::move(frame.subast);
        }
        // Considering the following markdown:
        // ```md
        // <b>example
        // ```
        // Any tag without a closing tag will hit this branch.
        auto&& subast = frame.subast;
        auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& parent_ast = parent_frame.subast;
        auto&& parent_index = parent_frame.current_index;
        switch (frame.get_nested_tag_type()) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color: {
            auto&& id = frame.get_equal_sign_tag_id();
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
            auto&& id = frame.get_equal_sign_tag_id();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::PlExperiment<ndebug>{::std::move(subast), ::std::move(id)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            auto&& id = frame.get_equal_sign_tag_id();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::PlDiscussion<ndebug>{::std::move(subast), ::std::move(id)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            auto&& id = frame.get_equal_sign_tag_id();
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlUser<ndebug>{::std::move(subast), ::std::move(id)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            auto&& url = frame.get_external_tag_url();
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlExternal<ndebug>{::std::move(subast), ::std::move(url)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            auto&& id = frame.get_pl_size_tag_id();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlSize<ndebug>{::std::move(subast), id}));
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
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(subast)}));
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
        case ::pltxt2htm::NodeKind::html_em: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlUl<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlOl<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlLi<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            auto checkbox = frame.checkbox;
            auto checked = frame.checked;
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::MdLi<ndebug>{::std::move(subast), checkbox, checked}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            auto align = frame.get_md_cell_align();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTh<ndebug>{::std::move(subast), align}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            auto align = frame.get_md_cell_align();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTd<ndebug>{::std::move(subast), align}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCode<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlPre<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTable<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTd<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTh<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlThead<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTbody<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTfoot<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCaption<ndebug>{::std::move(subast)}));
            parent_index += staged_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlColgroup<ndebug>{::std::move(subast)}));
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
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH1<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH2<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH3<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH4<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH5<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            ::fast_io::u8string_view super_pltext{::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pltext()};
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdAtxH6<ndebug>{::std::move(subast)}));
            auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, parent_index), call_stack,
                parent_ast);
            parent_index += forward_index;
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
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>{::std::move(subast)}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>{::std::move(subast)}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>{::std::move(subast)}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>{::std::move(subast)}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            parent_ast.push_back(
                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>{::std::move(subast)}));
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
            auto&& link_url = frame.get_md_link_url();
            parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::MdLink<ndebug>{::std::move(subast), ::std::move(link_url)}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::text:
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
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
