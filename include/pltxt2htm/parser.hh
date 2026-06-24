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
#include "details/parser/frame_concext.hh"
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
        auto subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
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
        case ::pltxt2htm::NodeKind::md_ul: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdUl<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdOl<ndebug>{::std::move(subast)}));
            continue;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTable<ndebug>{::std::move(subast)}));
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
        result = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
    }

    bool const call_stack_is_empty{call_stack.empty()};
    pltxt2htm_assert(call_stack_is_empty, u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
