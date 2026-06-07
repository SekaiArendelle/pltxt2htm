#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/node_type.hh"
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
        return ::exception::nullopt_t{};
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
        return ::exception::nullopt_t{};
    }
    ++current_index; // skip the first |

    ::fast_io::vector<::fast_io::u8string> row{};
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
            ++current_index; // skip |
        }
    }

    if (row.empty()) {
        return ::exception::nullopt_t{};
    }

    // Require trailing |: the row text must end with | (possibly followed
    // by spaces/tabs and at most one \n).  A row like "|cell" (no trailing |)
    // is rejected.
    {
        ::std::size_t end_idx = current_index;
        if (end_idx > 0 && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, end_idx - 1) == u8'\n') {
            --end_idx;
        }
        while (end_idx > 0) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, end_idx - 1);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
            --end_idx;
        }
        if (end_idx == 0 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, end_idx - 1) != u8'|') {
            return ::exception::nullopt_t{};
        }
    }

    return ::pltxt2htm::details::TryParseMdTableRowResult{::std::move(row), current_index};
}

/**
 * @brief Check whether a single non-empty delimiter cell is syntactically valid.
 *
 * A valid cell matches `:? -+ :?`.
 *
 * @tparam ndebug Contract checking mode
 * @param cell The delimiter cell text (e.g. `":---:"`)
 * @return true if the cell is a valid delimiter cell
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto is_delimiter_cell_valid(::fast_io::u8string_view cell) noexcept -> bool {
    if (cell.empty()) {
        return false;
    }
    ::std::size_t i{};
    if (::pltxt2htm::details::u8string_view_index<ndebug>(cell, i) == u8':') {
        ++i;
    }
    bool has_dash{};
    for (; i < cell.size(); ++i) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(cell, i);
        if (chr == u8'-') {
            has_dash = true;
        }
        else {
            break;
        }
    }
    if (i >= cell.size()) {
        return has_dash;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(cell, i) == u8':') {
        return i + 1 == cell.size();
    }
    return false;
}

/**
 * @brief Parse alignment from a single delimiter cell.
 *
 * Recognises `:---` (left), `---:` (right), `:---:` (center), and
 * plain `---` (left, the default).
 *
 * @tparam ndebug Contract checking mode
 * @param cell_text The delimiter cell text (e.g. `":---:"`)
 * @return MdTableAlign::left, MdTableAlign::center, or MdTableAlign::right
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_align(::fast_io::u8string_view cell_text) noexcept -> ::pltxt2htm::MdTableAlign {
    bool left = false;
    bool right = false;
    ::std::size_t i{};
    if (i < cell_text.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(cell_text, i) == u8':') {
        left = true;
        ++i;
    }
    for (; i < cell_text.size(); ++i) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(cell_text, i);
        if (chr == u8':') {
            right = true;
            break;
        }
        if (chr != u8'-') {
            return ::pltxt2htm::MdTableAlign::left;
        }
    }
    if (left && right) {
        return ::pltxt2htm::MdTableAlign::center;
    }
    if (right) {
        return ::pltxt2htm::MdTableAlign::right;
    }
    if (left) {
        return ::pltxt2htm::MdTableAlign::left;
    }
    return ::pltxt2htm::MdTableAlign::left;
}

/**
 * @brief Raw table cell data (text + alignment) for intermediate table AST.
 */
struct MdTableCellRaw {
    ::fast_io::u8string text; ///< cell text content (to be inline-parsed later)
    ::pltxt2htm::MdTableAlign align; ///< cell alignment from delimiter row
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
    ::std::size_t num_cols_{}; ///< number of columns (= header cells count)
    ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw> cells_{}; ///< flat storage: header then body (row-major)

public:
    /// @return number of header cells (= column count for every row)
    [[nodiscard]]
    constexpr auto header_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols_;
    }

    /// @param col column index
    /// @return const/non-const reference to the header cell at @p col
    [[nodiscard]]
    constexpr auto header_cell_at(this auto&& self, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.cells_, col);
    }

    /// Append one header cell.  Increments internal column counter.
    constexpr void add_header_cell(this MdTableAstRaw& self, ::pltxt2htm::details::MdTableCellRaw&& cell) noexcept {
        self.cells_.push_back(::std::move(cell));
        ++self.num_cols_;
    }

    /// @return number of body rows (derived from flat vector size and column count)
    [[nodiscard]]
    constexpr auto body_rows_count(this auto&& self) noexcept -> ::std::size_t {
        pltxt2htm_assert(self.num_cols_ != 0, u8"num_cols_ should be > 0 when calculating body rows count");
        return (self.cells_.size() - self.num_cols_) / self.num_cols_;
    }

    /// @return cells per body row (= num_cols_, all rows have equal length)
    [[nodiscard]]
    constexpr auto body_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols_;
    }

    /// @param row body row index
    /// @param col column index within the row
    /// @return const/non-const reference to the body cell at (@p row, @p col)
    [[nodiscard]]
    constexpr auto body_cell_at(this auto&& self, ::std::size_t row, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.cells_, self.num_cols_ + row * self.num_cols_ + col);
    }

    /// Append one body row (all its cells) to the flat vector.
    constexpr void add_body_row(this MdTableAstRaw& self,
                                ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw>&& row_cells) noexcept {
        bool const is_cow_cell_size_correct{row_cells.size() == self.num_cols_};
        pltxt2htm_assert(is_cow_cell_size_correct, u8"row_cells size should match num_cols_ when adding a body row");
        self.cells_.append_range(::std::move(row_cells));
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
        return ::exception::nullopt_t{};
    }
    auto&& [header_row, header_forward] = header_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += header_forward;
    ::std::size_t const num_cols{header_row.size()};

    // parse delimiter line (second line) & extract alignment in one pass
    auto delim_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
        ::fast_io::u8string_view{pltext.data() + current_index, pltext.size() - current_index});
    if (delim_opt.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& [delim_row, delim_forward] = delim_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += delim_forward;
    ::fast_io::vector<::pltxt2htm::MdTableAlign> aligns{};
    bool has_delimiter_content{};
    for (auto const& cell : delim_row) {
        auto cell_view = ::fast_io::u8string_view{cell.data(), cell.size()};
        if (cell_view.empty() == false) {
            if (::pltxt2htm::details::is_delimiter_cell_valid<ndebug>(cell_view) == false) {
                return ::exception::nullopt_t{};
            }
            has_delimiter_content = true;
        }
        aligns.push_back(::pltxt2htm::details::try_parse_md_table_align<ndebug>(cell_view));
    }
    if (has_delimiter_content == false) {
        return ::exception::nullopt_t{};
    }

    // delimiter row must have the same column count as the header row
    if (aligns.size() != header_row.size()) {
        return ::exception::nullopt_t{};
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
        if (!row_opt.has_value()) {
            break;
        }
        auto&& [row, forward] = row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        current_index += forward;

        // each body row must have exactly num_cols cells
        if (row.size() != num_cols) {
            return ::exception::nullopt_t{};
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
