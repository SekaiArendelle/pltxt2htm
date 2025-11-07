#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../heap_guard.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/html_node.hh"
#include "../../astnode/markdown_node.hh"
#include "../../astnode/physics_lab_node.hh"
#include "frame_concext.hh"
#include "pltxt2htm/heap_guard.hh"
#include "try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

struct DevilStuffAfterLineBreakResult {
    ::std::size_t forward_index;
    bool new_frame_been_pushed_into_call_stack;
};

template<bool ndebug>
[[nodiscard]]
constexpr auto devil_stuff_after_line_break(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>& call_stack,
    ::pltxt2htm::Ast& result) noexcept -> ::pltxt2htm::details::DevilStuffAfterLineBreakResult {
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
                opt_md_atx_heading.template value<ndebug>();
            if (sublength != 0) {
                auto subtext =
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + start_index, sublength);
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(subtext, md_atx_heading_type));
                current_index += forward_index;
                return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{
                    .forward_index = current_index, .new_frame_been_pushed_into_call_stack = true};
            } else {
                switch (md_atx_heading_type) {
                case ::pltxt2htm::NodeType::md_atx_h1: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH1>(::pltxt2htm::Ast{}));
                    break;
                }
                case ::pltxt2htm::NodeType::md_atx_h2: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH2>(::pltxt2htm::Ast{}));
                    break;
                }
                case ::pltxt2htm::NodeType::md_atx_h3: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH3>(::pltxt2htm::Ast{}));
                    break;
                }
                case ::pltxt2htm::NodeType::md_atx_h4: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH4>(::pltxt2htm::Ast{}));
                    break;
                }
                case ::pltxt2htm::NodeType::md_atx_h5: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH5>(::pltxt2htm::Ast{}));
                    break;
                }
                case ::pltxt2htm::NodeType::md_atx_h6: {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH6>(::pltxt2htm::Ast{}));
                    break;
                }
                default:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug>();
                    }
                }
                current_index += forward_index;
                continue;
            }
        } else if (auto opt_len = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_len.has_value()) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdHr>{});
            current_index += opt_len.template value<ndebug>();
            continue;
        } else if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_code_fence.has_value()) {
            auto&& [node, forward_index] = opt_code_fence.template value<ndebug>();
            result.push_back(::std::move(node));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = false};
        } else if (auto opt_block_quote = ::pltxt2htm::details::try_parse_md_block_quotes<ndebug>(pltext);
                   opt_block_quote.has_value()) {
            auto&& [forward_index, subpltext] = opt_block_quote.template value<ndebug>();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::MdBlockQuotesContext>(::std::move(subpltext)));
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index + forward_index,
                                                                        .new_frame_been_pushed_into_call_stack = true};
        } else {
            return ::pltxt2htm::details::DevilStuffAfterLineBreakResult{.forward_index = current_index,
                                                                        .new_frame_been_pushed_into_call_stack = false};
        }
    }
}

template<bool ndebug>
constexpr auto get_pltext_from_parser_frame_context(
    ::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext> const& top_frame) noexcept
    -> ::fast_io::u8string_view {
    switch (top_frame->nested_tag_type) {
    case ::pltxt2htm::NodeType::base:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_b:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_i:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_p:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h1:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h2:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h3:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h4:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h5:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_h6:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_del:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_del:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_em:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_strong:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_ul:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_li:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_code:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_pre:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_blockquote:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h1:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h2:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h3:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h4:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h5:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_atx_h6:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_single_emphasis_underscore: {
        return static_cast<::pltxt2htm::details::BareTagContext const*>(top_frame.release_imul())->pltext;
    }
    case ::pltxt2htm::NodeType::pl_color:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_a:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_experiment:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_discussion:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::pl_user: {
        return static_cast<::pltxt2htm::details::EqualSignTagContext const*>(top_frame.release_imul())->pltext;
    }
    case ::pltxt2htm::NodeType::pl_size: {
        return static_cast<::pltxt2htm::details::PlSizeTagContext const*>(top_frame.release_imul())->pltext;
    }
    case ::pltxt2htm::NodeType::md_block_quotes: {
        return ::fast_io::mnp::os_c_str(
            static_cast<::pltxt2htm::details::MdBlockQuotesContext const*>(top_frame.release_imul())->pltext);
    }
    case ::pltxt2htm::NodeType::md_code_span_1_backtick:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_code_span_2_backtick:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_code_span_3_backtick:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_link:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_code_fence_backtick:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_code_fence_tilde:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_br:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_hr:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_hr:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::html_note:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::u8char:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::invalid_u8char:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::text:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::line_break:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::space:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::ampersand:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::double_quote:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::single_quote:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::less_than:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::greater_than:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::tab:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_backslash:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_exclamation:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_double_quote:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_hash:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_dollar:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_percent:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_ampersand:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_single_quote:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_left_paren:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_right_paren:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_asterisk:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_plus:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_comma:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_hyphen:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_dot:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_slash:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_colon:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_semicolon:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_less_than:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_equals:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_greater_than:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_question:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_at:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_left_bracket:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_right_bracket:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_caret:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_underscore:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_backtick:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_left_brace:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_pipe:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_right_brace:
        [[fallthrough]];
    case ::pltxt2htm::NodeType::md_escape_tilde:
        [[unlikely]] {
            ::exception::unreachable<ndebug>();
        }
    }
    // suppress GCC -Wreturn-type warning
    ::exception::unreachable<ndebug>();
}

/**
 * @brief Parse pl-text to nodes.
 * @tparam ndebug: Whether disables all debug checks.
 * @param call_stack: use `call_stack` + `goto entry` to avoid stack overflow.
 * @return Quantum-Physics text's ast.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(
    ::fast_io::stack<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>&
        call_stack) noexcept -> ::pltxt2htm::Ast {
entry:
    auto&& current_index = call_stack.top()->current_index;
    ::fast_io::u8string_view pltext{
        ::pltxt2htm::details::get_pltext_from_parser_frame_context<ndebug>(call_stack.top())};
    auto&& result = call_stack.top()->subast;
    ::std::size_t const pltext_size{pltext.size()};

    if (call_stack.top()->nested_tag_type == ::pltxt2htm::NodeType::md_block_quotes && current_index == 0) {
        // https://spec.commonmark.org/0.31.2/#example-228
        // to support parsing md-atx-heading e.t.c inside md-block-quotes
        auto&& [forward_index, require_restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), call_stack, result);
        current_index += forward_index;
        if (require_restart) {
            goto entry;
        }
    }

    for (; current_index < pltext_size; ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8'\n') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LineBreak>{});

            auto&& [forward_index, require_restart] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), call_stack, result);
            current_index += forward_index;
            if (require_restart) {
                current_index += 1;
                goto entry;
            }
            continue;
        } else if (chr == u8' ') {
            // TODO should we delete tail space?
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Space>{});
            continue;
        } else if (chr == u8'&') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        } else if (chr == u8'\'') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        } else if (chr == u8'\"') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        } else if (chr == u8'>') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        } else if (chr == u8'\t') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        } else if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                break;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                result.push_back(::std::move(escape_node.template value<ndebug>()));
                ++current_index;
            } else {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        } else if (auto opt_triple_emphasis_asterisk =
                       ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'*', u8'*', u8'*'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_triple_emphasis_asterisk.has_value()) {
            // parsing markdown ***example***
            current_index += 3;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_triple_emphasis_asterisk.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_triple_emphasis_asterisk));
            goto entry;
        } else if (auto opt_double_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'*', u8'*'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_double_emphasis_asterisk.has_value()) {
            // parsing markdown **example**
            current_index += 2;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_double_emphasis_asterisk.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_double_emphasis_asterisk));
            goto entry;
        } else if (auto opt_single_emphasis_asterisk = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'*'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_single_emphasis_asterisk.has_value()) {
            // parsing markdown *example*
            current_index += 1;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_single_emphasis_asterisk.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_single_emphasis_asterisk));
            goto entry;
        } else if (auto opt_triple_emphasis_underscore =
                       ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'_', u8'_', u8'_'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_triple_emphasis_underscore.has_value()) {
            // parsing markdown ___example___
            current_index += 3;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_triple_emphasis_underscore.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_triple_emphasis_underscore));
            goto entry;
        } else if (auto opt_double_emphasis_undersore =
                       ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'_', u8'_'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_double_emphasis_undersore.has_value()) {
            // parsing markdown __example__
            current_index += 2;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_double_emphasis_undersore.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_double_emphasis_underscore));
            goto entry;
        } else if (auto opt_single_emphasis_undersore = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'_'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_single_emphasis_undersore.has_value()) {
            // parsing markdown _example_
            current_index += 1;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    pltext, current_index, opt_single_emphasis_undersore.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_single_emphasis_underscore));
            goto entry;
        } else if (auto opt_md_del = ::pltxt2htm::details::try_parse_md_inlines<ndebug, u8'~', u8'~'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_md_del.has_value()) {
            // parsing markdown ~~example~~
            current_index += 2;
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index,
                                                                    opt_md_del.template value<ndebug>()),
                ::pltxt2htm::NodeType::md_del));
            goto entry;
        } else if (auto opt_code_span_3_backtick =
                       ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8'`', u8'`', u8'`'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_code_span_3_backtick.has_value()) {
            // parsing markdown ```example```
            auto&& [forward_index, subast] = opt_code_span_3_backtick.template value<ndebug>();
            current_index = current_index + forward_index - 1;
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan3Backtick>(::std::move(subast)));
            continue;
        } else if (auto opt_code_span_2_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8'`', u8'`'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_code_span_2_backtick.has_value()) {
            // parsing markdown ``example``
            auto&& [forward_index, subast] = opt_code_span_2_backtick.template value<ndebug>();
            current_index = current_index + forward_index - 1;
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan2Backtick>(::std::move(subast)));
            continue;
        } else if (auto opt_code_span_1_backtick = ::pltxt2htm::details::try_parse_md_code_span<ndebug, u8'`'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                   opt_code_span_1_backtick.has_value()) {
            // parsing markdown `example`
            auto&& [forward_index, subast] = opt_code_span_1_backtick.template value<ndebug>();
            current_index = current_index + forward_index - 1;
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan1Backtick>(::std::move(subast)));
            continue;
        } else if (chr == u8'<') {
            // if i is a valid value, i always less than pltext_size
            pltxt2htm_assert(current_index < pltext_size, u8"Index of parser out of bound");

            if (current_index + 1 == pltext_size) {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
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
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_a));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'b':
                [[fallthrough]];
            case u8'B': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // parsing pl&html <b> tag
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_b));
                    goto entry;
                } else if (auto opt_br_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'r'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_br_tag_len.has_value()) {
                    current_index += opt_br_tag_len.template value<ndebug>() + 2;
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Br>(::pltxt2htm::Br()));

                    auto&& [forward_index, require_restart] =
                        ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), call_stack,
                            result);
                    current_index += forward_index;
                    if (require_restart) {
                        current_index += 1;
                        goto entry;
                    }
                    continue;
                } else if (auto opt_blockquote_tag =
                               ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'l', u8'o', u8'c', u8'k', u8'q',
                                                                        u8'u', u8'o', u8't', u8'e'>(
                                   ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_blockquote_tag.has_value()) {
                    // parsing pl&html <b> tag
                    current_index += opt_blockquote_tag.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_blockquote));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'c':
                [[fallthrough]];
            case u8'C': {
                // parsing: <color=$1>$2</color>
                if (auto opt_color_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'o', u8'l', u8'o', u8'r'>(
                            u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'0' <= u8chr && u8chr <= u8'9') || (u8'a' <= u8chr && u8chr <= u8'z') ||
                                       (u8'A' <= u8chr && u8chr <= u8'Z') || u8chr == u8'#';
                            });
                    opt_color_tag.has_value()) {
                    auto&& [tag_len, color] = opt_color_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    // parsing start tag <color> successed
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_color, ::std::move(color)));
                    goto entry;
                } else if (auto opt_code_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'o', u8'd', u8'e'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_code_tag.has_value()) {
                    // parsing html <code> tag
                    current_index += opt_code_tag.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_code));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'd':
                [[fallthrough]];
            case u8'D': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'l'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // parsing <del>$1</del>
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_del));
                    goto entry;
                }
                // parsing: <discussion=$1>$2</discussion>
                if (auto opt_discussion_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'i', u8's', u8'c', u8'u', u8's', u8's',
                                                                       u8'i', u8'o', u8'n'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            });
                    opt_discussion_tag.has_value()) {
                    auto&& [tag_len, id] = opt_discussion_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_discussion, ::std::move(id)));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'e':
                [[fallthrough]];
            case u8'E': {
                // parsing: <experiment=$1>$2</experiment>
                if (auto opt_experiment_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'x', u8'p', u8'e', u8'r', u8'i', u8'm',
                                                                       u8'e', u8'n', u8't'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            });
                    opt_experiment_tag.has_value()) {
                    auto&& [tag_len, id] = opt_experiment_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_experiment, ::std::move(id)));
                    goto entry;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'm'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_em));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'h':
                [[fallthrough]];
            case u8'H': {
                if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'1'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h1_tag_len.has_value()) {
                    // parsing html <h1> tag
                    current_index += opt_h1_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h1));
                    goto entry;
                } else if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'2'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h2_tag_len.has_value()) {
                    // parsing html <h2> tag
                    current_index += opt_h2_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h2));
                    goto entry;
                } else if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'3'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h3_tag_len.has_value()) {
                    // parsing html <h3> tag
                    current_index += opt_h3_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h3));
                    goto entry;
                } else if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'4'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h4_tag_len.has_value()) {
                    // parsing html <h4> tag
                    current_index += opt_h4_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h4));
                    goto entry;
                } else if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'5'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h5_tag_len.has_value()) {
                    // parsing html <h5> tag
                    current_index += opt_h5_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h5));
                    goto entry;
                } else if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'6'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h6_tag_len.has_value()) {
                    // parsing html <h6> tag
                    current_index += opt_h6_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h6));
                    goto entry;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'r'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 2;
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Hr>());
                    continue;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'i':
                [[fallthrough]];
            case u8'I': {
                // parsing pl&html <i>$1</i> tag
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_i));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'l':
                [[fallthrough]];
            case u8'L': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'i'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_li));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'p':
                [[fallthrough]];
            case u8'P': {
                // parsing html <p></p> tag
                if (auto opt_p_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_p_tag_len.has_value()) {
                    current_index += opt_p_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_p));
                    goto entry;
                } else if (auto opt_pre_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'r', u8'e'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_pre_tag_len.has_value()) {
                    current_index += opt_pre_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_pre));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8's':
                [[fallthrough]];
            case u8'S': {
                // parsing pl <size=$1>$2</size> tag
                if (auto opt_size_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'i', u8'z', u8'e'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept { return u8'0' <= u8chr && u8chr <= u8'9'; });
                    opt_size_tag.has_value()) {
                    auto&& [tag_len, id_] = opt_size_tag.template value<ndebug>();
                    ::exception::optional<::std::size_t> id{
                        ::pltxt2htm::details::u8str2size_t(::fast_io::mnp::os_c_str(id_))};
                    if (!id.has_value()) [[unlikely]] {
                        ::exception::unreachable<ndebug>();
                    }

                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::PlSizeTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_size, id.template value<ndebug>()));
                    goto entry;
                } else if (auto opt_tag_len =
                               ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8't', u8'r', u8'o', u8'n', u8'g'>(
                                   ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    // HTML <strong> tag
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_strong));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'u':
                [[fallthrough]];
            case u8'U': {
                // parsing pl <user=$1>$2</user> tag
                if (auto opt_user_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8's', u8'e', u8'r'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept {
                            return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                        });
                    opt_user_tag.has_value()) {
                    auto&& [tag_len, id] = opt_user_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_user, ::std::move(id)));
                    goto entry;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'l'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_ul));
                    goto entry;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'!': {
                // parsing: <!--$1-->
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8'-', u8'-'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))) {
                    // Find the closing -->
                    ::std::size_t comment_end{current_index + 4}; // Position after <!--
                    ::pltxt2htm::Ast subast{};

                    for (; comment_end < pltext_size; ++comment_end) {
                        if (::pltxt2htm::details::is_prefix_match<ndebug, u8'-', u8'-', u8'>'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, comment_end))) {
                            break;
                        }
                        subast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>(
                            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, comment_end)));
                    }

                    current_index = comment_end + 2; // Point to '>'
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Note>(::std::move(subast)));
                    continue;
                } else {
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'/': {
                switch (call_stack.top()->nested_tag_type) {
                case ::pltxt2htm::NodeType::pl_color: {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'c', u8'o', u8'l', u8'o', u8'r'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </color> successed
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Color staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Color>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_a: {
                    // parsing </a>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'a'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::A staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::A>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_experiment: {
                    // parsing </experiment>
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'x', u8'p', u8'e', u8'r', u8'i',
                                                                     u8'm', u8'e', u8'n', u8't'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Experiment staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Experiment>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_discussion: {
                    // parsing </discussion>
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'd', u8'i', u8's', u8'c', u8'u', u8's',
                                                                     u8's', u8'i', u8'o', u8'n'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Discussion staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Discussion>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_user: {
                    // parsing </user>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'u', u8's', u8'e', u8'r'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::User staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::User>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_size: {
                    // parsing </size>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8's', u8'i', u8'z', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        auto frame =
                            static_cast<::pltxt2htm::details::PlSizeTagContext const*>(call_stack.top().release_imul());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Size staged_node(::std::move(result), frame->id);
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Size>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_b: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'b'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </b> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::B staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::B>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_i: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'i'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::I staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::I>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_p: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'p'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </p> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::P staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::P>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h1: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'1'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h1> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H1 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H1>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h2: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'2'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h2> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H2 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H2>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h3: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'3'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h3> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H3 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H3>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h4: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'4'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h4> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H4 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H4>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h5: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'5'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h5> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H5 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H5>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h6: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'6'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h6> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H6 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::H6>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_del: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'd', u8'e', u8'l'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </del> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Del staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Del>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_note: {
                    ::exception::unreachable<ndebug>();
                }
                case ::pltxt2htm::NodeType::html_em: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'm'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </em> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Em staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Em>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_strong: {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8's', u8't', u8'r', u8'o', u8'n', u8'g'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </strong> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Strong staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Strong>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_ul: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'u', u8'l'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </ul> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Ul staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Ul>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_li: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'l', u8'i'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </li> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Li staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Li>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_code: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'c', u8'o', u8'd', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </code> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Code staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Code>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_pre: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'p', u8'r', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </pre> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Pre staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Pre>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_blockquote: {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'b', u8'l', u8'o', u8'c', u8'k', u8'q',
                                                                     u8'u', u8'o', u8't', u8'e'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </blockquote> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Blockquote staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::HeapGuard<::pltxt2htm::Blockquote>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto entry;
                    } else {
                        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::base:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_block_quotes:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_code_fence_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_code_fence_tilde:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_del:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_link:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h1:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h2:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h3:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h4:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h5:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_atx_h6:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::u8char:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::invalid_u8char:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::text:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::line_break:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::space:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::ampersand:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::double_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::single_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::less_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::greater_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::tab:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::html_br:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::html_hr:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_backslash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_exclamation:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_double_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_hash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_dollar:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_percent:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_ampersand:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_single_quote:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_left_paren:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_right_paren:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_asterisk:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_plus:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_comma:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_hyphen:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_dot:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_slash:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_colon:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_semicolon:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_less_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_equals:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_greater_than:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_question:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_at:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_left_bracket:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_right_bracket:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_caret:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_underscore:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_left_brace:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_pipe:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_right_brace:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_escape_tilde:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_hr:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_code_span_1_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_code_span_2_backtick:
                    [[fallthrough]];
                case ::pltxt2htm::NodeType::md_code_span_3_backtick:
                    // relate to 0041.fuzzing-crash3
                    // any tag contains `</` context would hit this branch
                    result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
                ::exception::unreachable<ndebug>();
            }

            default: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
                continue;
            }
            }

            ::exception::unreachable<ndebug>();
        } else {
            auto forward_index = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), result);
            current_index += forward_index;
            continue;
        }
    }

    {
        ::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext> frame(::std::move(call_stack.top()));
        ::std::size_t const staged_index = pltext_size;
        call_stack.pop();
        if (call_stack.empty()) {
            // Considering the following markdown:
            // ```md
            // <b>e</b>xample
            // ```
            // Text without any tag in the end will hit this branch.
            return ::std::move(frame->subast);
        } else {
            // Considering the following markdown:
            // ```md
            // <b>example
            // ```
            // Any tag without a closing tag will hit this branch.
            auto&& subast = frame->subast;
            auto&& super_ast = call_stack.top()->subast;
            ::fast_io::u8string_view super_pltext{
                ::pltxt2htm::details::get_pltext_from_parser_frame_context<ndebug>(call_stack.top())};
            auto&& super_index = call_stack.top()->current_index;
            switch (frame->nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_color: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Color>(::std::move(subast), ::std::move(id)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_a: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::A>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                super_ast.push_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Experiment>(::std::move(subast), ::std::move(id)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                super_ast.push_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Discussion>(::std::move(subast), ::std::move(id)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::User>(::std::move(subast), ::std::move(id)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                auto&& id = static_cast<::pltxt2htm::details::PlSizeTagContext const*>(frame.release_imul())->id;
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Size>(::std::move(subast), id));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::B>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_i: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::I>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_p: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::P>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H1>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H2>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H3>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H4>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H5>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::H6>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_del: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Del>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_em: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Em>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_ul: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ul>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_li: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Li>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_code: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Code>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_pre: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Pre>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_blockquote: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Blockquote>(::std::move(subast)));
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h1: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH1>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h2: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH2>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h3: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH3>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h4: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH4>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h5: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH5>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_atx_h6: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH6>(::std::move(subast)));
                auto&& [forward_index, _] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(super_pltext, super_index), call_stack,
                    super_ast);
                super_index += forward_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_span_1_backtick: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan1Backtick>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_span_2_backtick: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan2Backtick>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_span_3_backtick: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeSpan3Backtick>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_single_emphasis_asterisk: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdSingleEmphasisAsterisk>(::std::move(subast)));
                super_index += 1;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_single_emphasis_underscore: {
                super_ast.push_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::MdSingleEmphasisUnderscore>(::std::move(subast)));
                super_index += 1;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_double_emphasis_asterisk: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdDoubleEmphasisAsterisk>(::std::move(subast)));
                super_index += 2;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_double_emphasis_underscore: {
                super_ast.push_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::MdDoubleEmphasisUnderscore>(::std::move(subast)));
                super_index += 2;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdTripleEmphasisAsterisk>(::std::move(subast)));
                super_index += 3;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_underscore: {
                super_ast.push_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::MdTripleEmphasisUnderscore>(::std::move(subast)));
                super_index += 3;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_del: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdDel>(::std::move(subast)));
                super_index += 2;
                super_index += staged_index;
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_block_quotes: {
                super_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdBlockQuotes>(::std::move(subast)));
                goto entry;
            }
            case ::pltxt2htm::NodeType::base:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::u8char:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::invalid_u8char:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::text:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::line_break:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::space:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::double_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::single_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::less_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::greater_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::tab:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_br:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_backslash:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_exclamation:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_double_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_hash:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_dollar:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_percent:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_single_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_left_paren:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_right_paren:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_plus:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_comma:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_hyphen:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_dot:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_slash:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_colon:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_semicolon:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_less_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_equals:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_greater_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_question:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_at:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_left_bracket:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_right_bracket:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_caret:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_left_brace:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_pipe:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_right_brace:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_escape_tilde:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_tilde:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_note:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
            ::exception::unreachable<ndebug>();
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
