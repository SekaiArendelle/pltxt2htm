/**
 * @file html_table.hh
 * @brief HTML &lt;table&gt; block scanner that produces the shared intermediate TableAstRaw.
 *
 * @details &lt;table&gt; is treated as a block-level construct: an opening
 *          &lt;table&gt; at a block position is scanned as a whole table.  The
 *          interior follows the HTML content model with an explicit open-tag
 *          stack: &lt;caption&gt; &rarr; &lt;colgroup&gt; &rarr; &lt;thead&gt; /
 *          &lt;tbody&gt; / &lt;tfoot&gt; / &lt;tr&gt; &rarr; &lt;th&gt; / &lt;td&gt;.
 *          Cell and caption content are captured as raw text (whitespace
 *          trimmed) and re-parsed through the inline parser by the frame
 *          machinery.  Any violation of the content model or an unclosed
 *          table makes the whole scan fail, so malformed tables fall back to
 *          literal text (the same strict behaviour as &lt;ul&gt;/&lt;ol&gt;).
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <pltxt2htm/container/expected.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/node_kind.hh"

#include "try_parse.hh"
#include "table_ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Result of the HTML-table block scan attempt.
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
struct ToHtmlTableAstResult {
    TableAstRaw<ndebug> raw_ast; ///< Parsed table AST.
    ::std::size_t advance_count; ///< Bytes consumed from the input up to the close tag.
};

/**
 * @brief Result of scanning raw content up to a closing tag.
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
struct TryCaptureUntilTagResult {
    ::fast_io::u8string text; ///< Raw content (not whitespace-trimmed).
    ::std::size_t advance_count; ///< Bytes consumed past the closing tag.
};

/**
 * @brief Capture everything up to the closing tag @p close_tag.
 *
 * The content is captured raw (no nested-tag interpretation); the caller
 * trims whitespace.  Recognising a nested closing tag or running out of
 * input makes the whole table scan fail.
 *
 * @tparam ndebug Contract checking mode
 * @tparam close_tag Closing tag prefix, e.g. `u8"</th"`.
 * @param pltext Input starting right after the opening cell/caption tag.
 * @return Captured text and consumed byte count past the close tag; nullopt on failure.
 */
template<::pltxt2htm::Contracts ndebug, U8LiteralString close_tag>
[[nodiscard]]
constexpr auto try_capture_until_tag(::fast_io::u8string_view pltext) noexcept
    -> ::pltxt2htm::container::optional<TryCaptureUntilTagResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    ::fast_io::u8string text{};
    while (current_index < pltext_size) {
        if (auto opt_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, close_tag>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_len.has_value()) {
            current_index += opt_len.template value<ndebug>() + 1;
            return TryCaptureUntilTagResult<ndebug>{.text = ::std::move(text), .advance_count = current_index};
        }
        text.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index));
        ++current_index;
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Trim leading/trailing whitespace (space, tab, newline) from cell/caption content.
 * @param text Raw captured content.
 */
constexpr auto trim_table_content(::fast_io::u8string& text) noexcept -> void {
    while (text.empty() == false) {
        auto const chr = text[text.size() - 1];
        if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
            break;
        }
        text.pop_back();
    }
}

/**
 * @brief Try to parse an HTML &lt;table&gt;...&lt;/table&gt; block into an intermediate TableAst.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text starting at the opening &lt;table&gt; tag.
 * @return Parsed table raw AST and the consumed byte count on success; nullopt if the
 *         text is not a well-formed HTML table block.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto optionally_to_html_table_ast(::fast_io::u8string_view pltext) noexcept
    -> ::pltxt2htm::container::optional<ToHtmlTableAstResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};

    if (auto opt_table_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<table">(pltext);
        opt_table_len.has_value()) {
        current_index = opt_table_len.template value<ndebug>() + 1;
    }
    else {
        return ::pltxt2htm::container::nullopt;
    }

    TableAstRaw<ndebug> raw_ast{};
    TableRowSection active_section{TableRowSection::none};
    bool row_inside_section{};
    bool inside_caption{};
    bool inside_colgroup{};
    bool inside_row{};
    bool inside_section{};

    while (true) {
        // Skip whitespace between table tags.
        while (current_index < pltext_size) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
            if (chr != u8' ' && chr != u8'\t' && chr != u8'\n') {
                break;
            }
            ++current_index;
        }
        if (current_index + 2 > pltext_size) {
            // No nested tag can start with fewer than two bytes left (`<` plus a letter);
            // probing subview(pltext, current_index + 2) would go out of bounds.
            return ::pltxt2htm::container::nullopt;
        }

        auto const tag_view = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index);

        if (inside_row) {
            // Directly under <tr>: </tr>, <th>, <td>.
            if (auto opt_tr_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</tr">(tag_view);
                opt_tr_len.has_value()) {
                current_index += opt_tr_len.template value<ndebug>() + 1;
                inside_row = false;
                if (row_inside_section) {
                    inside_section = true;
                }
                continue;
            }
            if (auto opt_th_tag = ::pltxt2htm::details::try_parse_th_tag<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                    ::pltxt2htm::NodeKind::table_tr);
                opt_th_tag.has_value()) {
                auto&& [tag_len, align] = opt_th_tag.template value<ndebug>();
                auto opt_cell = ::pltxt2htm::details::try_capture_until_tag<ndebug, u8"</th">(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + tag_len + 3));
                if (opt_cell.has_value() == false) {
                    return ::pltxt2htm::container::nullopt;
                }
                auto&& [cell_text, cell_advance] = opt_cell.template value<ndebug>();
                ::pltxt2htm::details::trim_table_content(cell_text);
                current_index += tag_len + 3 + cell_advance;
                raw_ast.add_cell_to_last_row(
                    TableCellRaw{.text = ::std::move(cell_text), .align = align, .is_header = true});
                continue;
            }
            if (auto opt_td_tag = ::pltxt2htm::details::try_parse_td_tag<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                    ::pltxt2htm::NodeKind::table_tr);
                opt_td_tag.has_value()) {
                auto&& [tag_len, align] = opt_td_tag.template value<ndebug>();
                auto opt_cell = ::pltxt2htm::details::try_capture_until_tag<ndebug, u8"</td">(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + tag_len + 3));
                if (opt_cell.has_value() == false) {
                    return ::pltxt2htm::container::nullopt;
                }
                auto&& [cell_text, cell_advance] = opt_cell.template value<ndebug>();
                ::pltxt2htm::details::trim_table_content(cell_text);
                current_index += tag_len + 3 + cell_advance;
                raw_ast.add_cell_to_last_row(
                    TableCellRaw{.text = ::std::move(cell_text), .align = align, .is_header = false});
                continue;
            }
            return ::pltxt2htm::container::nullopt;
        }

        if (inside_caption) {
            auto opt_caption = ::pltxt2htm::details::try_capture_until_tag<ndebug, u8"</caption">(tag_view);
            if (opt_caption.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            auto&& [caption_text, caption_advance] = opt_caption.template value<ndebug>();
            ::pltxt2htm::details::trim_table_content(caption_text);
            raw_ast.set_caption(::std::move(caption_text));
            current_index += caption_advance;
            inside_caption = false;
            continue;
        }

        if (inside_colgroup) {
            // Directly under <colgroup>: </colgroup>, <col>.
            if (auto opt_cg_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</colgroup">(tag_view);
                opt_cg_len.has_value()) {
                current_index += opt_cg_len.template value<ndebug>() + 1;
                inside_colgroup = false;
                continue;
            }
            if (auto opt_col_len = ::pltxt2htm::details::try_parse_col_tag<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                    ::pltxt2htm::NodeKind::table_colgroup);
                opt_col_len.has_value()) {
                current_index += opt_col_len.template value<ndebug>() + 2;
                raw_ast.add_col();
                continue;
            }
            return ::pltxt2htm::container::nullopt;
        }

        if (inside_section) {
            // Directly under <thead>/<tbody>/<tfoot>: the matching close tag or <tr>.
            auto const opt_sec_len =
                active_section == TableRowSection::thead
                    ? ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</thead">(tag_view)
                    : (active_section == TableRowSection::tbody
                           ? ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</tbody">(tag_view)
                           : ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</tfoot">(tag_view));
            if (opt_sec_len.has_value()) {
                current_index += opt_sec_len.template value<ndebug>() + 1;
                inside_section = false;
                active_section = TableRowSection::none;
                continue;
            }
            auto const section_node_kind =
                active_section == TableRowSection::thead
                    ? ::pltxt2htm::NodeKind::table_thead
                    : (active_section == TableRowSection::tbody ? ::pltxt2htm::NodeKind::table_tbody
                                                                : ::pltxt2htm::NodeKind::table_tfoot);
            if (auto opt_tr_len = ::pltxt2htm::details::try_parse_tr_tag<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2), section_node_kind);
                opt_tr_len.has_value()) {
                current_index += opt_tr_len.template value<ndebug>() + 3;
                raw_ast.add_row(TableRowRaw{.cells = {}, .section = active_section});
                inside_row = true;
                row_inside_section = true;
                continue;
            }
            return ::pltxt2htm::container::nullopt;
        }

        // Directly under <table>: </table>, <caption>, <colgroup>, sections, <tr>.
        if (auto opt_table_close = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"</table">(tag_view);
            opt_table_close.has_value()) {
            current_index += opt_table_close.template value<ndebug>() + 1;
            return ToHtmlTableAstResult<ndebug>{.raw_ast = ::std::move(raw_ast), .advance_count = current_index};
        }
        if (auto opt_caption_len = ::pltxt2htm::details::try_parse_caption_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_caption_len.has_value()) {
            current_index += opt_caption_len.template value<ndebug>() + 3;
            inside_caption = true;
            continue;
        }
        if (auto opt_colgroup_len = ::pltxt2htm::details::try_parse_colgroup_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_colgroup_len.has_value()) {
            current_index += opt_colgroup_len.template value<ndebug>() + 3;
            inside_colgroup = true;
            continue;
        }
        if (auto opt_thead_len = ::pltxt2htm::details::try_parse_thead_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_thead_len.has_value()) {
            current_index += opt_thead_len.template value<ndebug>() + 3;
            active_section = TableRowSection::thead;
            inside_section = true;
            continue;
        }
        if (auto opt_tbody_len = ::pltxt2htm::details::try_parse_tbody_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_tbody_len.has_value()) {
            current_index += opt_tbody_len.template value<ndebug>() + 3;
            active_section = TableRowSection::tbody;
            inside_section = true;
            continue;
        }
        if (auto opt_tfoot_len = ::pltxt2htm::details::try_parse_tfoot_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_tfoot_len.has_value()) {
            current_index += opt_tfoot_len.template value<ndebug>() + 3;
            active_section = TableRowSection::tfoot;
            inside_section = true;
            continue;
        }
        if (auto opt_tr_len = ::pltxt2htm::details::try_parse_tr_tag<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                ::pltxt2htm::NodeKind::table);
            opt_tr_len.has_value()) {
            current_index += opt_tr_len.template value<ndebug>() + 3;
            raw_ast.add_row(TableRowRaw{.cells = {}, .section = active_section});
            inside_row = true;
            row_inside_section = false;
            continue;
        }
        return ::pltxt2htm::container::nullopt;
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
