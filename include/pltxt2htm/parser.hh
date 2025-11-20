#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "astnode/node_type.hh"
#include "heap_guard.hh"
#include "details/utils.hh"
#include "details/parser/frame_concext.hh"
#include "details/parser/parser.hh"
#include "details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief Impl of parse pl-text to nodes.
 * @tparam ndebug: Whether or not to disable debugging checks (like NDEBUG macro).
 * @param pltext: The text readed from Quantum-Physics.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept -> ::pltxt2htm::Ast {
    // fast_io::deque contains bug about RAII, use fast_io::list instead
    ::fast_io::stack<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>
        call_stack{};
    ::pltxt2htm::Ast result{};

    ::std::size_t start_index{};

    while (true) {
        auto&& [forward_index, has_new_frame] = ::pltxt2htm::details::devil_stuff_after_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), call_stack, result);
        start_index += forward_index;
        if (has_new_frame) {
            ::pltxt2htm::NodeType type_of_subast{call_stack.top()->nested_tag_type};
            auto subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
            switch (type_of_subast) {
            case ::pltxt2htm::NodeType::md_atx_h1: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH1>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h2: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH2>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h3: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH3>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h4: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH4>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h5: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH5>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h6: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdAtxH6>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_block_quotes: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdBlockQuotes>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_ul: {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdUl>(::std::move(subast)));
                break;
            }
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
        } else {
            break;
        }
    }

    if (start_index < pltext.size()) {
        // other common cases
        call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), ::pltxt2htm::NodeType::base,
            ::std::move(result)));
        result = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
    }

    bool const call_stack_is_empty{call_stack.empty()};
    pltxt2htm_assert(call_stack_is_empty, u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
