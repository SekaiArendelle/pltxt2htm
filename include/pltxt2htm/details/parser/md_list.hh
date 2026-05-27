/**
 * @file md_list.hh
 * @brief Markdown list parser utilities for nested unordered/ordered list AST construction.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"

namespace pltxt2htm::details {

/**
 * @brief Marker describing parsed markdown list item style.
 */
enum class MdUlListItemKind : char8_t {
    hyphen = u8'-',
    plus = u8'+',
    asterisk = u8'*',
    ordered_item = u8',',
};

/**
 * @brief Summary of the previously parsed item, used for hierarchy validation.
 */
struct PreviousItemInfo {
    ::std::size_t space_hierarchy;
    bool call_stack_is_single;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
};

template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::MdUlListItemKind item_kind>
[[nodiscard]]
constexpr auto is_valid_md_ul_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept -> bool {
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) != static_cast<char8_t>(item_kind)) {
        return false;
    }

    if ( // parsing the first line
        !expect.has_value() ||
        // e.g.
        // - test
        // - test
        //   - text <== here
        space_hierarchy > expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy + 1 ||
        // e.g.
        // - test
        //   - test
        //   + test <== here
        (!expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
         space_hierarchy >= expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy) ||
        // e.g.
        // - test
        // - test
        // + test <== here, this line is invalid markdown list
        // Note that only the first hierarchy apply this rule
        // e.g.
        // - test
        //   - test
        //   + test <== here, this is allowed
        (expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
         expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().item_kind == item_kind)) {
        return true;
    }

    // e.g.
    // - test
    // - test
    //   - text
    // - text <== here
    // Despite this is a valid list, but we will return nullopt to leave it as the problem of
    // previous frame of call_stack
    // Or rest of those invalid situations
    return false;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto is_valid_md_ol_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept
    -> ::exception::optional<::std::size_t> {
    ::std::size_t pltext_size{pltext.size()};
    if (pltext_size < 4) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) < u8'0' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) > u8'9') {
        return ::exception::nullopt_t{};
    }
    {
        ::std::size_t i{space_hierarchy + 1};
        for (; i < pltext_size; ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
            if (chr < u8'0' || chr > u8'9') {
                break;
            }
        }
        if (i == pltext_size) {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i) != u8'.') {
            return ::exception::nullopt_t{};
        }
        if (++i >= pltext_size) {
            return ::exception::nullopt_t{};
        }
        if ( // parsing the first line
            !expect.has_value() ||
            // e.g.
            // - test
            // - test
            //   1. text <== here
            space_hierarchy > expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy + 1 ||
            // e.g.
            // - test
            //   1. test
            //   2. test <== here
            (!expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
             space_hierarchy >= expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy) ||
            // e.g.
            // - test
            // - test
            // 1. test <== here, this line is invalid markdown list
            (expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
             expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().item_kind ==
                 ::pltxt2htm::details::MdUlListItemKind::ordered_item)) {
            return i;
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Parsed representation for a single markdown-list item candidate.
 */
struct TryParseItemResult {
    ::std::size_t space_hierarchy;
    ::std::size_t forward_index;
    ::fast_io::u8string_view text;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_item(
    ::fast_io::u8string_view pltext,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect = ::exception::nullopt_t{}) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseItemResult> {
    ::std::size_t current_index{};
    // parsing spaces before - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    if (current_index == pltext.size()) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t const space_hierarchy{current_index};

    // parsing item kind
    ::pltxt2htm::details::MdUlListItemKind item_kind
#if __has_cpp_attribute(indeterminate)
        // https://en.cppreference.com/w/cpp/language/attributes/indeterminate.html
        [[indeterminate]]
#endif
        ;
    if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug, ::pltxt2htm::details::MdUlListItemKind::hyphen>(
            pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::hyphen;
        ++current_index;
    }
    else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug, ::pltxt2htm::details::MdUlListItemKind::plus>(
                 pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::plus;

        ++current_index;
    }
    else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug,
                                                                 ::pltxt2htm::details::MdUlListItemKind::asterisk>(
                 pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::asterisk;
        ++current_index;
    }
    else if (auto opt_size =
                 ::pltxt2htm::details::is_valid_md_ol_list_hierarchy<ndebug>(pltext, space_hierarchy, expect);
             opt_size.has_value()) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::ordered_item;
        current_index = opt_size.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
    else {
        return ::exception::nullopt_t{};
    }

    // - or + or * must be followed by space
    if (current_index == pltext.size()) {
        return ::exception::nullopt_t{};
    }
    if (char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        chr != u8' ' && chr != u8'\t') {
        return ::exception::nullopt_t{};
    }
    // parsing spaces after - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    // parsing text after - or + or *
    ::std::size_t const text_start = current_index;
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr == u8'\n') {
            break;
        }
    }
    ::fast_io::u8string_view text{pltext.data() + text_start, current_index - text_start};
    if (current_index < pltext.size()) {
        ++current_index;
    }
    return ::pltxt2htm::details::TryParseItemResult{
        .space_hierarchy = space_hierarchy,
        .forward_index = current_index,
        .text = ::std::move(text),
        .item_kind = item_kind,
    };
}

} // namespace pltxt2htm::details
