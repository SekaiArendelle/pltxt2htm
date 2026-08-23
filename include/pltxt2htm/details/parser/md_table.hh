/**
 * @file md_table.hh
 * @brief Markdown pipe-table parser for constructing raw table ASTs.
 * @details Provides functions to parse pipe-table syntax (header row, delimiter
 *          row, body rows) into an intermediate raw table AST that is later
 *          re-parsed through the inline parser.
 */
#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/vector.h>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../container/expected.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/node_kind.hh"
#include "table_ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Result of try_parse_md_table_row: parsed cell strings and consumed character count.
 */
struct TryParseMdTableRowResult {
    ::fast_io::vector<::pltxt2htm::container::U8String> cells;
    ::std::size_t advance_count;
};

/**
 * @brief Parse a single pipe-table row into cell strings.
 *
 * Splits the line on unescaped `|` characters.  A backslash immediately
 * before a `|` (`\|`) escapes the pipe so it becomes part of the cell
 * content rather than a column boundary.  Leading/trailing whitespace
 * is trimmed from each cell.
 *
 * @tparam ndebug Contract checking mode
 * @param line A single table-row line (e.g. `"| a | b | c |"`)
 * @return a vector of cell strings on success, nullopt if the line is not a valid table row
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_row(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdTableRowResult> {
    if (pltext.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    // skip leading spaces
    for (; current_index < pltext_size; ++current_index) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    // must start with |
    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8'|') {
        return ::pltxt2htm::container::nullopt;
    }
    ++current_index; // skip the first |

    ::fast_io::vector<::pltxt2htm::container::U8String> row{};
    bool has_trailing_pipe{};
    while (current_index < pltext_size) {
        // skip spaces before cell content
        for (; current_index < pltext_size; ++current_index) {
            auto const chr = pltext.template index<ndebug>(current_index);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        if (current_index >= pltext_size) {
            break;
        }
        // end of row at \n
        if (pltext.template index<ndebug>(current_index) == u8'\n') {
            ++current_index;
            break;
        }
        has_trailing_pipe = false;
        // parse cell content until unescaped | or \n or end of view
        ::pltxt2htm::container::U8String cell{};
        bool prev_was_backslash{};
        for (; current_index < pltext_size; ++current_index) {
            auto chr = pltext.template index<ndebug>(current_index);
            if (chr == u8'|') {
                if (prev_was_backslash) {
                    cell.template pop_back<ndebug>(); // remove the escape backslash
                    prev_was_backslash = false;
                    chr = u8'|';
                }
                else {
                    break;
                }
            }
            if (chr == u8'\n') {
                break;
            }
            cell.push_back(chr);
            prev_was_backslash = (chr == u8'\\') ? !prev_was_backslash : false;
        }
        // trim trailing spaces from cell
        while (!cell.empty() && (cell.template back<ndebug>() == u8' ' || cell.template back<ndebug>() == u8'\t')) {
            cell.template pop_back<ndebug>();
        }
        row.push_back(::std::move(cell));
        if (current_index < pltext_size && pltext.template index<ndebug>(current_index) == u8'|') {
            has_trailing_pipe = true;
            ++current_index; // skip |
        }
    }

    if (row.empty()) {
        return ::pltxt2htm::container::nullopt;
    }

    // A row like "|cell" (no trailing |) is rejected.
    if (has_trailing_pipe == false) {
        return ::pltxt2htm::container::nullopt;
    }

    return TryParseMdTableRowResult{.cells = ::std::move(row), .advance_count = current_index};
}

/**
 * @brief Parse a single non-empty delimiter cell and determine its alignment.
 *
 * A valid cell matches `:? -+ :?`.
 *
 * @tparam ndebug Contract checking mode
 * @param cell The delimiter cell text (e.g. `":---:"`)
 * @return The parsed alignment, or nullopt if the cell is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_table_align(::pltxt2htm::container::U8StringView cell) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::TableAlign> {
    if (cell.empty()) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t const cell_size{cell.size()};
    ::std::size_t current_index{};
    bool left{};
    if (cell.template index<ndebug>(current_index) == u8':') {
        left = true;
        ++current_index;
    }

    bool has_dash{};
    while (current_index < cell_size && cell.template index<ndebug>(current_index) == u8'-') {
        has_dash = true;
        ++current_index;
    }
    if (has_dash == false) {
        return ::pltxt2htm::container::nullopt;
    }

    bool right{};
    if (current_index < cell_size && cell.template index<ndebug>(current_index) == u8':') {
        right = true;
        ++current_index;
    }
    if (current_index != cell_size) {
        return ::pltxt2htm::container::nullopt;
    }

    if (left && right) {
        return ::pltxt2htm::TableAlign::center;
    }
    if (right) {
        return ::pltxt2htm::TableAlign::right;
    }
    return ::pltxt2htm::TableAlign::left;
}

/**
 * @brief Result of try_parse_md_table_raw: raw AST and consumed character count.
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseMdTableRawResult {
    TableAstRaw<ndebug> raw_ast; ///< parsed table AST
    ::std::size_t advance_count; ///< number of characters consumed from input
};

/**
 * @brief Parse an entire pipe table block (header + delimiter + body rows).
 *
 * The input must start at the first line of a table.  The header row
 * determines the column count; every body row is required to have
 * exactly that many cells (strict enforcement).  Returns nullopt if
 * the input is not a valid table.
 *
 * @tparam ndebug Contract checking mode
 * @param pltext The pl-text input starting at the table
 * @return TryParseMdTableRawResult on success, nullopt if parsing fails
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_raw(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdTableRawResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};

    // parse header row
    auto header_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(pltext);
    if (header_opt.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto&& [header_row, header_forward] = header_opt.template value<ndebug>();
    current_index += header_forward;
    ::std::size_t const num_cols{header_row.size()};

    // parse delimiter line (second line) & extract alignment in one pass
    auto delim_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
        ::pltxt2htm::container::U8StringView{pltext.data() + current_index, pltext_size - current_index});
    if (delim_opt.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto&& [delim_row, delim_forward] = delim_opt.template value<ndebug>();
    current_index += delim_forward;
    ::fast_io::vector<::pltxt2htm::TableAlign> aligns{};
    bool has_delimiter_content{};
    for (auto const& cell : delim_row) {
        auto const cell_view = ::pltxt2htm::container::U8StringView{cell};
        if (cell_view.empty()) {
            aligns.push_back(::pltxt2htm::TableAlign::left);
            continue;
        }
        auto opt_align = ::pltxt2htm::details::try_parse_table_align<ndebug>(cell_view);
        if (opt_align.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        has_delimiter_content = true;
        aligns.push_back(opt_align.template value<ndebug>());
    }
    if (has_delimiter_content == false) {
        return ::pltxt2htm::container::nullopt;
    }

    // delimiter row must have the same column count as the header row
    if (aligns.size() != num_cols) {
        return ::pltxt2htm::container::nullopt;
    }

    // build raw header row (section=thead, cells marked as headers)
    TableAstRaw<ndebug> raw_ast{};
    TableRowRaw header_row_raw{.cells = {}, .section = TableRowSection::thead};
    for (::std::size_t col{}; col < num_cols; ++col) {
        auto const align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
        header_row_raw.cells.push_back(TableCellRaw{
            .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(header_row, col)),
            .align = align_val,
            .is_header = true,
        });
    }
    raw_ast.add_row(::std::move(header_row_raw));

    // parse data rows
    while (true) {
        auto row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
            ::pltxt2htm::container::U8StringView{pltext.data() + current_index, pltext_size - current_index});
        if (row_opt.has_value() == false) {
            break;
        }
        auto&& [row, forward] = row_opt.template value<ndebug>();
        current_index += forward;

        // each body row must have exactly num_cols cells
        if (row.size() != num_cols) {
            return ::pltxt2htm::container::nullopt;
        }
        TableRowRaw body_row_raw{.cells = {}, .section = TableRowSection::tbody};
        for (::std::size_t col{}; col < num_cols; ++col) {
            auto const align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
            body_row_raw.cells.push_back(TableCellRaw{
                .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(row, col)),
                .align = align_val,
                .is_header = false,
            });
        }
        raw_ast.add_row(::std::move(body_row_raw));
    }

    return TryParseMdTableRawResult<ndebug>{.raw_ast = ::std::move(raw_ast), .advance_count = current_index};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
