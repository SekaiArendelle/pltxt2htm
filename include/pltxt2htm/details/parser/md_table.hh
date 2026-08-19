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
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/node_kind.hh"
#include "table_ast.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Result of try_parse_md_table_row: parsed cell strings and consumed character count.
 */
struct TryParseMdTableRowResult {
    ::fast_io::vector<::fast_io::u8string> cells;
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
constexpr auto try_parse_md_table_row(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdTableRowResult> {
    if (pltext.empty()) {
        return ::exception::nullopt;
    }
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    // skip leading spaces
    for (; current_index < pltext_size; ++current_index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    // must start with |
    if (current_index >= pltext_size ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'|') {
        return ::exception::nullopt;
    }
    ++current_index; // skip the first |

    ::fast_io::vector<::fast_io::u8string> row{};
    bool has_trailing_pipe{};
    while (current_index < pltext_size) {
        // skip spaces before cell content
        for (; current_index < pltext_size; ++current_index) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        if (current_index >= pltext_size) {
            break;
        }
        // end of row at \n
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
            ++current_index;
            break;
        }
        has_trailing_pipe = false;
        // parse cell content until unescaped | or \n or end of view
        ::fast_io::u8string cell{};
        bool prev_was_backslash{};
        for (; current_index < pltext_size; ++current_index) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
            if (chr == u8'|') {
                if (prev_was_backslash) {
                    cell.pop_back(); // remove the escape backslash
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
        while (!cell.empty() && (cell.back() == u8' ' || cell.back() == u8'\t')) {
            cell.pop_back();
        }
        row.push_back(::std::move(cell));
        if (current_index < pltext_size &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'|') {
            has_trailing_pipe = true;
            ++current_index; // skip |
        }
    }

    if (row.empty()) {
        return ::exception::nullopt;
    }

    // A row like "|cell" (no trailing |) is rejected.
    if (has_trailing_pipe == false) {
        return ::exception::nullopt;
    }

    return ::pltxt2htm::details::TryParseMdTableRowResult{.cells = ::std::move(row), .advance_count = current_index};
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
constexpr auto try_parse_table_align(::fast_io::u8string_view cell) noexcept
    -> ::exception::optional<::pltxt2htm::TableAlign> {
    if (cell.empty()) {
        return ::exception::nullopt;
    }

    ::std::size_t const cell_size{cell.size()};
    ::std::size_t current_index{};
    bool left{};
    if (::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8':') {
        left = true;
        ++current_index;
    }

    bool has_dash{};
    while (current_index < cell_size &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8'-') {
        has_dash = true;
        ++current_index;
    }
    if (has_dash == false) {
        return ::exception::nullopt;
    }

    bool right{};
    if (current_index < cell_size && ::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8':') {
        right = true;
        ++current_index;
    }
    if (current_index != cell_size) {
        return ::exception::nullopt;
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
    ::pltxt2htm::details::TableAstRaw<ndebug> raw_ast; ///< parsed table AST
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
constexpr auto try_parse_md_table_raw(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdTableRawResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};

    // parse header row
    auto header_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(pltext);
    if (header_opt.has_value() == false) {
        return ::exception::nullopt;
    }
    auto&& [header_row, header_forward] = header_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += header_forward;
    ::std::size_t const num_cols{header_row.size()};

    // parse delimiter line (second line) & extract alignment in one pass
    auto delim_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
        ::fast_io::u8string_view{pltext.data() + current_index, pltext_size - current_index});
    if (delim_opt.has_value() == false) {
        return ::exception::nullopt;
    }
    auto&& [delim_row, delim_forward] = delim_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += delim_forward;
    ::fast_io::vector<::pltxt2htm::TableAlign> aligns{};
    bool has_delimiter_content{};
    for (auto const& cell : delim_row) {
        auto const cell_view = ::fast_io::u8string_view{cell.data(), cell.size()};
        if (cell_view.empty()) {
            aligns.push_back(::pltxt2htm::TableAlign::left);
            continue;
        }
        auto opt_align = ::pltxt2htm::details::try_parse_table_align<ndebug>(cell_view);
        if (opt_align.has_value() == false) {
            return ::exception::nullopt;
        }
        has_delimiter_content = true;
        aligns.push_back(opt_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
    }
    if (has_delimiter_content == false) {
        return ::exception::nullopt;
    }

    // delimiter row must have the same column count as the header row
    if (aligns.size() != num_cols) {
        return ::exception::nullopt;
    }

    // build raw header row (section=thead, cells marked as headers)
    ::pltxt2htm::details::TableAstRaw<ndebug> raw_ast{};
    ::pltxt2htm::details::TableRowRaw header_row_raw{.cells = {},
                                                     .section = ::pltxt2htm::details::TableRowSection::thead};
    for (::std::size_t col{}; col < num_cols; ++col) {
        auto const align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
        header_row_raw.cells.push_back(::pltxt2htm::details::TableCellRaw{
            .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(header_row, col)),
            .align = align_val,
            .is_header = true,
        });
    }
    raw_ast.add_row(::std::move(header_row_raw));

    // parse data rows
    while (true) {
        auto row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
            ::fast_io::u8string_view{pltext.data() + current_index, pltext_size - current_index});
        if (row_opt.has_value() == false) {
            break;
        }
        auto&& [row, forward] = row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        current_index += forward;

        // each body row must have exactly num_cols cells
        if (row.size() != num_cols) {
            return ::exception::nullopt;
        }
        ::pltxt2htm::details::TableRowRaw body_row_raw{.cells = {},
                                                       .section = ::pltxt2htm::details::TableRowSection::tbody};
        for (::std::size_t col{}; col < num_cols; ++col) {
            auto const align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
            body_row_raw.cells.push_back(::pltxt2htm::details::TableCellRaw{
                .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(row, col)),
                .align = align_val,
                .is_header = false,
            });
        }
        raw_ast.add_row(::std::move(body_row_raw));
    }

    return ::pltxt2htm::details::TryParseMdTableRawResult<ndebug>{.raw_ast = ::std::move(raw_ast),
                                                                  .advance_count = current_index};
}

} // namespace pltxt2htm::details

#include "../../details/pop_macro.hh"
