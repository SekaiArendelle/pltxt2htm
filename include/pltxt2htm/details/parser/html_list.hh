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
#include "../../container/string_view.hh"
#include "../../container/optional.hh"
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
    ListBaseNode<ndebug> top_node;
    ::std::size_t advance_count;
};

/**
 * @brief Try to parse an HTML &lt;ul&gt;...&lt;/ul&gt; / &lt;ol&gt;...&lt;/ol&gt; block
 *        into an intermediate ListAst.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text starting at the opening &lt;ul&gt;/&lt;ol&gt; tag.
 * @return Parsed list as the top-level ListUlNode/ListOlNode and the consumed byte count
 *         on success; nullopt if the text is not a well-formed HTML list.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto optionally_to_html_list_ast(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<ToHtmlListAstResult<ndebug>> {
    ::pltxt2htm::NodeKind item_kind{};
    ::std::size_t start{1};
    ::std::size_t current_index{};
    if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<ul">(pltext); opt_len.has_value()) {
        item_kind = ::pltxt2htm::NodeKind::list_ul;
        current_index = opt_len.template value<ndebug>() + 1;
    }
    else if (auto opt_ol_tag = ::pltxt2htm::details::try_parse_ol_tag<ndebug>(pltext); opt_ol_tag.has_value()) {
        item_kind = ::pltxt2htm::NodeKind::list_ol;
        auto&& [ol_len, ol_start] = opt_ol_tag.template value<ndebug>();
        current_index = ol_len;
        start = ol_start;
    }
    else {
        return ::pltxt2htm::container::nullopt;
    }

    ListAst<ndebug> ast{};
    ::std::size_t const pltext_size{pltext.size()};

    while (true) {
        // Skip whitespace between items.
        while (current_index < pltext_size) {
            auto const chr = pltext.template index<ndebug>(current_index);
            if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                break;
            }
            ++current_index;
        }
        if (current_index >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        // Closing tag </ul> / </ol> (must match the opener kind). A list must
        // contain at least one <li> item: `<ol></ol>`/`<ul></ul>` (or a variant
        // with only whitespace but no items) is not a valid list today, so fall
        // back to literal text like any other malformed list instead of building
        // an empty list container.
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</ul">(
                pltext.template subview<ndebug>(current_index));
            item_kind == ::pltxt2htm::NodeKind::list_ul && opt_len.has_value()) {
            if (ast.empty()) {
                return ::pltxt2htm::container::nullopt;
            }
            return ToHtmlListAstResult<ndebug>{.top_node = ListBaseNode<ndebug>{ListUlNode<ndebug>(::std::move(ast))},
                                               .advance_count = current_index + opt_len.template value<ndebug>() + 1};
        }
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</ol">(
                pltext.template subview<ndebug>(current_index));
            item_kind == ::pltxt2htm::NodeKind::list_ol && opt_len.has_value()) {
            if (ast.empty()) {
                return ::pltxt2htm::container::nullopt;
            }
            return ToHtmlListAstResult<ndebug>{
                .top_node = ListBaseNode<ndebug>{ListOlNode<ndebug>(::std::move(ast), start)},
                .advance_count = current_index + opt_len.template value<ndebug>() + 1};
        }
        // Only <li> elements are allowed inside <ul>/<ol>.
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<li">(
                pltext.template subview<ndebug>(current_index));
            opt_len.has_value()) {
            current_index += opt_len.template value<ndebug>() + 1;
        }
        else {
            return ::pltxt2htm::container::nullopt;
        }

        // Under the HTML block content model, leading whitespace/newlines inside an
        // item are just formatting for readability and are not rendered.
        while (current_index < pltext_size) {
            auto const chr = pltext.template index<ndebug>(current_index);
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
                pltext.template subview<ndebug>(current_index));
            opt_input.has_value()) {
            auto&& [input_len, checked] = opt_input.template value<ndebug>();
            current_index += input_len;
            checkbox = true;
            checkbox_checked = checked;
            // Skip whitespace between the checkbox input and the item text.
            while (current_index < pltext_size) {
                auto const chr = pltext.template index<ndebug>(current_index);
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
        ListAst<ndebug> pending_nested{};
        while (true) {
            if (current_index >= pltext_size) {
                return ::pltxt2htm::container::nullopt;
            }
            if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</li">(
                    pltext.template subview<ndebug>(current_index));
                opt_len.has_value()) {
                current_index += opt_len.template value<ndebug>() + 1;
                break;
            }
            ::pltxt2htm::NodeKind nested_list_kind{};
            if (auto const opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<ul">(
                    pltext.template subview<ndebug>(current_index));
                opt_len.has_value()) {
                nested_list_kind = ::pltxt2htm::NodeKind::list_ul;
            }
            else if (auto const opt_ol_tag =
                         ::pltxt2htm::details::try_parse_ol_tag<ndebug>(pltext.template subview<ndebug>(current_index));
                     opt_ol_tag.has_value()) {
                nested_list_kind = ::pltxt2htm::NodeKind::list_ol;
            }
            if (nested_list_kind == ::pltxt2htm::NodeKind::list_ul ||
                nested_list_kind == ::pltxt2htm::NodeKind::list_ol) {
                auto opt_nested = ::pltxt2htm::details::optionally_to_html_list_ast<ndebug>(
                    pltext.template subview<ndebug>(current_index));
                if (opt_nested.has_value() == false) {
                    return ::pltxt2htm::container::nullopt;
                }
                auto&& [nested_node, nested_advance] = opt_nested.template value<ndebug>();
                current_index += nested_advance;
                pending_nested.emplace_back(::std::move(nested_node));
                continue;
            }
            // Ordinary content character.
            text.push_back(pltext.template index<ndebug>(current_index));
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
            ast.emplace_back(ListLiCheckboxNode(::std::move(text), checkbox_checked));
        }
        else {
            ast.emplace_back(ListLiNode(::std::move(text)));
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
