#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
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
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) /* throws */ -> ::pltxt2htm::Ast {
    // fast_io::deque contains bug about RAII, use fast_io::list instead
    ::fast_io::stack<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>
        call_stack{};
    ::pltxt2htm::Ast result{};

    ::std::size_t start_index{};

    while (start_index < pltext.size()) {
        if (auto opt_md_atx_heading = ::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index));
            opt_md_atx_heading.has_value()) {
            // Consider the following text
            // ## test
            // ...
            // Here, the first line is a markdown atx heading, will hit this case
            auto&& [start_index_, sublength, forward_index, md_atx_heading_type] =
                opt_md_atx_heading.template value<ndebug>();
            ::pltxt2htm::Ast subast{};
            if (sublength) {
                auto subtext = ::pltxt2htm::details::u8string_view_subview<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), start_index_, sublength);
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(subtext, md_atx_heading_type));
                subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
            }
            result.push_back(
                ::pltxt2htm::details::switch_md_atx_header<ndebug>(md_atx_heading_type, ::std::move(subast)));
            // rectify the start index to the start of next text (aka. below common cases)
            start_index += forward_index;
            continue;
        } else if (auto opt_thematic_break = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index));
                   opt_thematic_break.has_value()) {
            // considering following markdown
            // ---
            // ...
            // above example will hit this branch
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::MdHr>{});
            start_index += opt_thematic_break.template value<ndebug>();
            continue;
        } else if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index));
                   opt_code_fence.has_value()) {
            auto&& [node, forward_index] = opt_code_fence.template value<ndebug>();
            result.push_back(::std::move(node));
            start_index += forward_index;
            break;
        } else {
            break;
        }
    }

    if (start_index < pltext.size()) {
        // other common cases
        call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::BareTagContext>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), ::pltxt2htm::NodeType::base));
        auto subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
        for (auto&& node : subast) {
            result.push_back(::std::move(node));
        }
    }

    pltxt2htm_assert(call_stack.empty(), u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
