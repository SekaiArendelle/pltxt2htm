/**
 * @file html_list.hh
 * @brief HTML &lt;ul&gt;/&lt;ol&gt; list scanner that produces the shared intermediate ListAst.
 *
 * @details &lt;ul&gt;/&lt;ol&gt; are treated as block-level constructs: an opening
 *          &lt;ul&gt;/&lt;ol&gt; at a block position is scanned as a whole list.  The
 *          interior follows the HTML content model: whitespace between items is ignored
 *          and only &lt;li&gt; elements are allowed.  Each &lt;li&gt;'s raw content is
 *          captured in a ListLiNode; nested &lt;ul&gt;/&lt;ol&gt; inside an item are
 *          parsed recursively into sibling sublist nodes (the same sibling shape the
 *          Markdown list scanner produces).  Any other interior content makes the whole
 *          scan fail, so malformed lists fall back to literal text.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "list_ast.hh"
#include "try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Result of the HTML-list AST conversion attempt.
 */
template<::pltxt2htm::Contracts ndebug>
struct ToHtmlListAstResult {
    ::pltxt2htm::details::ListAst<ndebug> ast;
    ::std::size_t advance_count;
    ::pltxt2htm::NodeKind item_kind;
    ::std::size_t start; ///< `<ol start="N">` value (defaults to 1).
};

/**
 * @brief Try to parse an HTML &lt;ul&gt;...&lt;/ul&gt; / &lt;ol&gt;...&lt;/ol&gt; block
 *        into an intermediate ListAst.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text starting at the opening &lt;ul&gt;/&lt;ol&gt; tag.
 * @return Parsed list AST (ListLiNode leaves plus nested ListUlNode/ListOlNode siblings),
 *         the consumed byte count, and the list kind on success; nullopt if the text is
 *         not a well-formed HTML list.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto optionally_to_html_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::ToHtmlListAstResult<ndebug>> {
    ::pltxt2htm::NodeKind item_kind{};
    ::std::size_t start{1};
    ::std::size_t current_index{};
    if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<ul">(pltext); opt_len.has_value()) {
        item_kind = ::pltxt2htm::NodeKind::list_ul;
        current_index = opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
    }
    else if (auto opt_ol_tag = ::pltxt2htm::details::try_parse_ol_tag<ndebug>(pltext); opt_ol_tag.has_value()) {
        item_kind = ::pltxt2htm::NodeKind::list_ol;
        auto&& [ol_len, ol_start] = opt_ol_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        current_index = ol_len;
        start = ol_start;
    }
    else {
        return ::exception::nullopt;
    }

    ::pltxt2htm::details::ListAst<ndebug> ast{};
    ::std::size_t const pltext_size{pltext.size()};

    while (true) {
        // Skip whitespace between items.
        while (current_index < pltext_size) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
            if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                break;
            }
            ++current_index;
        }
        if (current_index >= pltext_size) {
            return ::exception::nullopt;
        }
        // Closing tag </ul> / </ol> (must match the opener kind).
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</ul">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            item_kind == ::pltxt2htm::NodeKind::list_ul && opt_len.has_value()) {
            return ::pltxt2htm::details::ToHtmlListAstResult<ndebug>{
                .ast = ::std::move(ast),
                .advance_count = current_index + opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1,
                .item_kind = item_kind,
                .start = start};
        }
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</ol">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            item_kind == ::pltxt2htm::NodeKind::list_ol && opt_len.has_value()) {
            return ::pltxt2htm::details::ToHtmlListAstResult<ndebug>{
                .ast = ::std::move(ast),
                .advance_count = current_index + opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1,
                .item_kind = item_kind,
                .start = start};
        }
        // Only <li> elements are allowed inside <ul>/<ol>.
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<li">(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_len.has_value()) {
            current_index += opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
        }
        else {
            return ::exception::nullopt;
        }

        // Under the HTML block content model, leading whitespace/newlines inside an
        // item are just formatting for readability and are not rendered.
        while (current_index < pltext_size) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
            if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                break;
            }
            ++current_index;
        }

        // A checkbox <input type="checkbox" disabled [checked]> may start the item
        // (the HTML equivalent of Markdown `- [ ]` / `- [x]`).  It is parsed here so
        // that inline occurrences of <input> stay literal text.
        bool checkbox{};
        bool checkbox_checked{};
        if (auto opt_input = ::pltxt2htm::details::try_parse_input_checkbox_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_input.has_value()) {
            auto&& [input_len, checked] = opt_input.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            current_index += input_len;
            checkbox = true;
            checkbox_checked = checked;
            // Skip whitespace between the checkbox input and the item text.
            while (current_index < pltext_size) {
                auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
                if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                    break;
                }
                ++current_index;
            }
        }

        // Collect the raw <li> content up to the matching </li>.  Nested <ul>/<ol> are
        // parsed recursively into sibling sublist nodes (appended after the item), so the
        // item text itself never contains list markup and no inline recognition is needed.
        ::fast_io::u8string text{};
        ::pltxt2htm::details::ListAst<ndebug> pending_nested{};
        while (true) {
            if (current_index >= pltext_size) {
                return ::exception::nullopt;
            }
            if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</li">(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                opt_len.has_value()) {
                current_index += opt_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
                break;
            }
            ::pltxt2htm::NodeKind nested_list_kind{};
            if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<ul">(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                opt_len.has_value()) {
                nested_list_kind = ::pltxt2htm::NodeKind::list_ul;
            }
            else if (auto opt_ol_tag = ::pltxt2htm::details::try_parse_ol_tag<ndebug>(
                         ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                     opt_ol_tag.has_value()) {
                nested_list_kind = ::pltxt2htm::NodeKind::list_ol;
            }
            if (nested_list_kind == ::pltxt2htm::NodeKind::list_ul ||
                nested_list_kind == ::pltxt2htm::NodeKind::list_ol) {
                auto opt_nested = ::pltxt2htm::details::optionally_to_html_list_ast<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                if (opt_nested.has_value() == false) {
                    return ::exception::nullopt;
                }
                auto&& [nested_ast, nested_advance, nested_kind, nested_start] =
                    opt_nested.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                current_index += nested_advance;
                if (nested_kind == ::pltxt2htm::NodeKind::list_ul) {
                    pending_nested.emplace_back(::pltxt2htm::details::ListUlNode<ndebug>(::std::move(nested_ast)));
                }
                else {
                    pending_nested.emplace_back(
                        ::pltxt2htm::details::ListOlNode<ndebug>(::std::move(nested_ast), nested_start));
                }
                continue;
            }
            // Ordinary content character.
            text.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index));
            ++current_index;
        }
        // Trailing whitespace/newlines are formatting as well (see the leading-skip above).
        while (text.empty() == false) {
            auto const chr = text[text.size() - 1];
            if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                break;
            }
            text.pop_back();
        }
        if (checkbox) {
            ast.emplace_back(::pltxt2htm::details::ListLiCheckboxNode(::std::move(text), checkbox_checked));
        }
        else {
            ast.emplace_back(::pltxt2htm::details::ListLiNode(::std::move(text)));
        }
        // Append the nested lists as siblings after the item (the Markdown sibling shape),
        // so the shared list frame handling and backends apply uniformly.
        for (auto& node : pending_nested) {
            ast.emplace_back(::std::move(node));
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
