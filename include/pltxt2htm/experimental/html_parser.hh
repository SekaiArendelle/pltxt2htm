#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../ast/node_kind.hh"
#include "../ast/ast.hh"
#include "../contracts.hh"
#include "../details/call_stack.hh"
#include "../details/utils.hh"
#include "../details/parser/frame_context.hh"
#include "../details/parser/try_parse.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::experimental {
namespace details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_pltxt_html(
    ::pltxt2htm::details::CallStack<::pltxt2htm::details::ParserFrameContext<ndebug>>& call_stack) noexcept
    -> ::pltxt2htm::Ast<ndebug> {
entry:
    while (true) {
        auto&& top_frame = call_stack.template top<ndebug>();
        auto&& current_index = top_frame.current_index;
        ::fast_io::u8string_view const pltext{top_frame.get_pltext()};
        auto&& result = top_frame.subast;
        ::std::size_t const pltext_size{pltext.size()};

        while (current_index < pltext_size) {
            char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

            if (chr == u8'\n') {
                result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));
                ++current_index;
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
                    auto const entity_len = opt_entity_len.value();
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
                    if (auto opt_a_tag = ::pltxt2htm::details::try_parse_html_a_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_a_tag.has_value()) {
                        auto&& [tag_len, url, internal] =
                            opt_a_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 2;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlATagInfo<ndebug>{
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
                        ++current_index;
                        continue;
                    }
                    if (auto opt_blockquote_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"lockquote">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_blockquote_tag.has_value()) {
                        current_index +=
                            opt_blockquote_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_blockquote},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'c':
                    [[fallthrough]];
                case u8'C': {
                    if (auto opt_code_tag = ::pltxt2htm::details::try_parse_code_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_code_tag.has_value()) {
                        // parsing html <code> tag (bare or with class="language-...")
                        auto&& [tag_len, lang] =
                            opt_code_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 2;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlCodeInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                                    ::std::move(lang)},
                                ::pltxt2htm::NodeKind::html_code},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_caption_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_caption},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_colgroup_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_colgroup},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_col_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
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
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
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
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
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

                case u8'h':
                    [[fallthrough]];
                case u8'H': {
                    if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"1">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h1_tag_len.has_value()) {
                        current_index += opt_h1_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h1},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"2">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h2_tag_len.has_value()) {
                        current_index += opt_h2_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h2},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"3">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h3_tag_len.has_value()) {
                        current_index += opt_h3_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h3},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"4">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h4_tag_len.has_value()) {
                        current_index += opt_h4_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h4},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"5">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h5_tag_len.has_value()) {
                        current_index += opt_h5_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h5},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"6">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_h6_tag_len.has_value()) {
                        current_index += opt_h6_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_h6},
                            ::pltxt2htm::Ast<ndebug>{}));
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
                    if (auto opt_input_tag = ::pltxt2htm::details::try_parse_input_checkbox_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_input_tag.has_value()) {
                        auto&& [tag_len, checked] =
                            opt_input_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 1;
                        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlInput{checked}));
                        ++current_index;
                        continue;
                    }
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

                case u8'l':
                    [[fallthrough]];
                case u8'L': {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_li_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_li},
                            ::pltxt2htm::Ast<ndebug>{}));
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
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_ol},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
                    ++current_index;
                    continue;
                }

                case u8'p':
                    [[fallthrough]];
                case u8'P': {
                    if (auto opt_p_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_p_tag_len.has_value()) {
                        current_index += opt_p_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_p},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_pre_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"re">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_pre_tag_len.has_value()) {
                        current_index += opt_pre_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_pre},
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
                        auto&& [tag_len, color, font_size] =
                            opt_span_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 2;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithHtmlSpanInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                                    ::std::move(color), ::std::move(font_size)},
                                ::pltxt2htm::NodeKind::html_span},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"trong">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_strong},
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
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"able">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_table},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_thead_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_thead},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tbody_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_tbody},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tfoot_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_tfoot},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_tr_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_tr},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_th_tag = ::pltxt2htm::details::try_parse_th_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_th_tag.has_value()) {
                        auto&& [tag_len, align] = opt_th_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithCellInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), align},
                                ::pltxt2htm::NodeKind::html_th},
                            ::pltxt2htm::Ast<ndebug>{}));
                        goto entry;
                    }
                    if (auto opt_td_tag = ::pltxt2htm::details::try_parse_td_tag<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            call_stack.template top<ndebug>().get_nested_tag_type());
                        opt_td_tag.has_value()) {
                        auto&& [tag_len, align] = opt_td_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                        current_index += tag_len + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithCellInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), align},
                                ::pltxt2htm::NodeKind::html_td},
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
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"l">(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        current_index += opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 3;
                        call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
                            ::pltxt2htm::details::FrontendContextVariant<ndebug>{
                                ::pltxt2htm::details::ParserFrameContextWithPltextInfo{
                                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index)},
                                ::pltxt2htm::NodeKind::html_ul},
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
                    auto&& frame = call_stack.template top<ndebug>();
                    switch (frame.get_nested_tag_type()) {
                    case ::pltxt2htm::NodeKind::html_span: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"span">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlSpan staged_node(::std::move(result),
                                                              ::std::move(frame.get_html_span_color()),
                                                              ::std::move(frame.get_html_span_font_size()));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                    case ::pltxt2htm::NodeKind::html_p: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"p">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlP staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                    case ::pltxt2htm::NodeKind::html_ul: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"ul">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlUl staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlUl<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlOl staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlOl<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlLi staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlLi<ndebug>{::std::move(staged_node)}));
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
                            auto& code_frame = call_stack.template top<ndebug>();
                            ::pltxt2htm::HtmlCode staged_node(::std::move(result),
                                                              ::std::move(code_frame.get_html_code_language()));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                    case ::pltxt2htm::NodeKind::html_pre: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"pre">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlPre staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlPre<ndebug>{::std::move(staged_node)}));
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
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
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
                    case ::pltxt2htm::NodeKind::html_table: {
                        if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"table">(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                            opt_tag_len.has_value()) {
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlTable staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlTable<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlTr staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            auto align = frame.get_cell_align();
                            ::pltxt2htm::HtmlTd staged_node(::std::move(result), align);
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTd<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            auto align = frame.get_cell_align();
                            ::pltxt2htm::HtmlTh staged_node(::std::move(result), align);
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(
                                ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTh<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlThead staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlThead<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlTbody staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlTbody<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlTfoot staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlTfoot<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlCaption staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlCaption<ndebug>{::std::move(staged_node)}));
                            parent_frame.current_index +=
                                staged_index + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() +
                                3;
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
                            ::std::size_t const staged_index{current_index};
                            ::pltxt2htm::HtmlColgroup staged_node(::std::move(result));
                            call_stack.template pop<ndebug>();
                            auto& parent_frame = call_stack.template top<ndebug>();
                            parent_frame.subast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                                ::pltxt2htm::HtmlColgroup<ndebug>{::std::move(staged_node)}));
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
            ::pltxt2htm::details::ParserFrameContext<ndebug> frame(::std::move(call_stack.template top<ndebug>()));
            call_stack.template pop<ndebug>();
            if (call_stack.empty()) {
                return ::std::move(frame.subast);
            }
            auto&& subast = frame.subast;
            auto&& parent_frame = call_stack.template top<ndebug>();
            auto&& parent_ast = parent_frame.subast;
            switch (frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeKind::html_span: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::HtmlSpan<ndebug>{::std::move(subast), ::std::move(frame.get_html_span_color()),
                                                  ::std::move(frame.get_html_span_font_size())}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlA<ndebug>{
                    ::std::move(subast), ::std::move(frame.get_html_a_url()), frame.get_html_a_internal()}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlP<ndebug>{::std::move(subast)}));
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
            case ::pltxt2htm::NodeKind::html_em: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlEm<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlStrong<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ul: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlUl<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ol: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlOl<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_li: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlLi<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                    ::pltxt2htm::HtmlCode<ndebug>{::std::move(subast), ::std::move(frame.get_html_code_language())}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_pre: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlPre<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlBlockquote<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_table: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTable<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tr: {
                parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_td: {
                auto align = frame.get_cell_align();
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTd<ndebug>{::std::move(subast), align}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_th: {
                auto align = frame.get_cell_align();
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTh<ndebug>{::std::move(subast), align}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_thead: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlThead<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tbody: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTbody<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tfoot: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTfoot<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_caption: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlCaption<ndebug>{::std::move(subast)}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_colgroup: {
                parent_ast.push_back(
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlColgroup<ndebug>{::std::move(subast)}));
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
    ::pltxt2htm::details::CallStack<::pltxt2htm::details::ParserFrameContext<ndebug>> call_stack{};

    call_stack.template push<ndebug>(::pltxt2htm::details::ParserFrameContext<ndebug>(
        ::pltxt2htm::details::FrontendContextVariant<ndebug>{
            ::pltxt2htm::details::ParserFrameContextWithPltextInfo{html_text}, ::pltxt2htm::NodeKind::text},
        ::pltxt2htm::Ast<ndebug>{}));

    auto result = details::parse_pltxt_html<ndebug>(call_stack);

    bool const call_stack_is_empty{call_stack.empty()};
    pltxt2htm_assert(call_stack_is_empty, u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm::experimental

#include "../details/pop_macro.hh"
