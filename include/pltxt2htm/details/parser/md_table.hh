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
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
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
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
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

    ::std::size_t current_index{};
    bool left{};
    if (::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8':') {
        left = true;
        ++current_index;
    }

    bool has_dash{};
    while (current_index < cell.size() &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8'-') {
        has_dash = true;
        ++current_index;
    }
    if (has_dash == false) {
        return ::exception::nullopt;
    }

    bool right{};
    if (current_index < cell.size() &&
        ::pltxt2htm::details::u8string_view_index<ndebug>(cell, current_index) == u8':') {
        right = true;
        ++current_index;
    }
    if (current_index != cell.size()) {
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
 * @brief Raw table cell data (text + alignment) for intermediate table AST.
 */
struct MdTableCellRaw {
    ::fast_io::u8string text; ///< cell text content (to be inline-parsed later)
    ::pltxt2htm::TableAlign align; ///< cell alignment from delimiter row
};

/**
 * @brief Intermediate table AST representing raw cell content.
 *
 * Stores all cells in a single flat vector:
 *   `[header_cell_0, …, header_cell_N, row0_cell_0, …, row0_cell_N, row1_cell_0, …]`
 *
 * This is analogous to MdListAst: raw text will be re-parsed through the
 * inline parser when the table frame is processed.
 *
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
class MdTableAstRaw {
    ::std::size_t num_cols{}; ///< number of columns (= header cells count)
    ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw> cells{}; ///< flat storage: header then body (row-major)

public:
    /// @return number of header cells (= column count for every row)
    [[nodiscard]]
    constexpr auto header_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols;
    }

    /// @param col column index
    /// @return const/non-const reference to the header cell at @p col
    [[nodiscard]]
    constexpr auto header_cell_at(this auto&& self, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.cells, col);
    }

    /// Append one header cell.  Increments internal column counter.
    constexpr void add_header_cell(this MdTableAstRaw& self, ::pltxt2htm::details::MdTableCellRaw&& cell) noexcept {
        self.cells.push_back(::std::move(cell));
        ++self.num_cols;
    }

    /// @return number of body rows (derived from flat vector size and column count)
    [[nodiscard]]
    constexpr auto body_rows_count(this auto&& self) noexcept -> ::std::size_t {
        pltxt2htm_assert(self.num_cols != 0, u8"num_cols should be > 0 when calculating body rows count");
        return (self.cells.size() - self.num_cols) / self.num_cols;
    }

    /// @return cells per body row (= num_cols, all rows have equal length)
    [[nodiscard]]
    constexpr auto body_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols;
    }

    /// @param row body row index
    /// @param col column index within the row
    /// @return const/non-const reference to the body cell at (@p row, @p col)
    [[nodiscard]]
    constexpr auto body_cell_at(this auto&& self, ::std::size_t row, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.cells, self.num_cols + row * self.num_cols + col);
    }

    /// Append one body row (all its cells) to the flat vector.
    constexpr void add_body_row(this MdTableAstRaw& self,
                                ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw>&& row_cells) noexcept {
        bool const is_cow_cell_size_correct{row_cells.size() == self.num_cols};
        pltxt2htm_assert(is_cow_cell_size_correct, u8"row_cells size should match num_cols when adding a body row");
        self.cells.append_range(::std::move(row_cells));
    }
};

/**
 * @brief Result of try_parse_md_table_raw: raw AST and consumed character count.
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseMdTableRawResult {
    ::pltxt2htm::details::MdTableAstRaw<ndebug> raw_ast; ///< parsed table AST
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
        ::fast_io::u8string_view{pltext.data() + current_index, pltext.size() - current_index});
    if (delim_opt.has_value() == false) {
        return ::exception::nullopt;
    }
    auto&& [delim_row, delim_forward] = delim_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += delim_forward;
    ::fast_io::vector<::pltxt2htm::TableAlign> aligns{};
    bool has_delimiter_content{};
    for (auto const& cell : delim_row) {
        auto cell_view = ::fast_io::u8string_view{cell.data(), cell.size()};
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
    if (aligns.size() != header_row.size()) {
        return ::exception::nullopt;
    }

    // build raw header cells
    ::pltxt2htm::details::MdTableAstRaw<ndebug> raw_ast{};
    for (::std::size_t col{}; col < header_row.size(); ++col) {
        auto align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
        raw_ast.add_header_cell(::pltxt2htm::details::MdTableCellRaw{
            .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(header_row, col)),
            .align = align_val,
        });
    }

    // parse data rows
    while (true) {
        auto row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
            ::fast_io::u8string_view{pltext.data() + current_index, pltext.size() - current_index});
        if (row_opt.has_value() == false) {
            break;
        }
        auto&& [row, forward] = row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        current_index += forward;

        // each body row must have exactly num_cols cells
        if (row.size() != num_cols) {
            return ::exception::nullopt;
        }
        ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw> body_cells{};
        for (::std::size_t col{}; col < row.size(); ++col) {
            auto align_val = ::pltxt2htm::details::vector_index<ndebug>(aligns, col);
            body_cells.push_back(::pltxt2htm::details::MdTableCellRaw{
                .text = ::std::move(::pltxt2htm::details::vector_index<ndebug>(row, col)),
                .align = align_val,
            });
        }
        raw_ast.add_body_row(::std::move(body_cells));
    }

    return ::pltxt2htm::details::TryParseMdTableRawResult<ndebug>{.raw_ast = ::std::move(raw_ast),
                                                                  .advance_count = current_index};
}

} // namespace pltxt2htm::details

#include "../../details/pop_macro.hh"
