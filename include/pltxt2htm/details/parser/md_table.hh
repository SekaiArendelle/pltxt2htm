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
 * @brief Raw table row containing cell text strings.
 *
 * Produced by try_parse_md_table_row: splits a `|`-delimited line
 * into individual cell strings (with escaped pipe handling).
 */
struct MdTableRow {
    ::fast_io::vector<::fast_io::u8string> cells;
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
 * @return MdTableRow on success, nullopt if the line is not a valid table row
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_row(::fast_io::u8string_view line) noexcept
    -> ::exception::optional<::pltxt2htm::details::MdTableRow> {
    if (line.empty()) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t i{};
    // skip leading spaces
    for (; i < line.size(); ++i) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    // must start with |
    if (i >= line.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(line, i) != u8'|') {
        return ::exception::nullopt_t{};
    }
    ++i; // skip the first |

    ::pltxt2htm::details::MdTableRow row{};
    while (i < line.size()) {
        // skip spaces before cell content
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        // parse cell content until unescaped | or end of line
        ::fast_io::u8string cell{};
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr == u8'|') {
                // count consecutive backslashes before |
                ::std::size_t bs_count{};
                while (bs_count < i &&
                       ::pltxt2htm::details::u8string_view_index<ndebug>(line, i - 1 - bs_count) == u8'\\') {
                    ++bs_count;
                }
                if (bs_count % 2 == 1) {
                    // odd backslashes: backslash-pipe is escaped pipe, treat | as content
                    cell.pop_back(); // remove the escape backslash
                    chr = u8'|';
                }
                else {
                    break;
                }
            }
            cell.push_back(chr);
        }
        // trim trailing spaces from cell
        while (!cell.empty() && (cell.back() == u8' ' || cell.back() == u8'\t')) {
            cell.pop_back();
        }
        row.cells.push_back(::std::move(cell));
        if (i < line.size()) {
            ++i; // skip |
        }
    }

    if (row.cells.empty()) {
        return ::exception::nullopt_t{};
    }

    return row;
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
 * @brief Validate that a line looks like a pipe-table delimiter row.
 *
 * A delimiter row consists of `|`-separated segments containing only
 * dashes and optional colons, e.g. `|:---:|---:|`.
 *
 * @tparam ndebug Contract checking mode
 * @param line A candidate delimiter line
 * @return true if the line is a valid delimiter row
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_delimiter(::fast_io::u8string_view line) noexcept -> bool {
    if (line.empty()) {
        return false;
    }
    ::std::size_t i{};
    for (; i < line.size(); ++i) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    if (i >= line.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(line, i) != u8'|') {
        return false;
    }
    ++i;
    bool has_content = false;
    while (i < line.size()) {
        // skip spaces
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        if (i >= line.size()) {
            break;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(line, i) == u8'|') {
            ++i;
            continue;
        }
        // parse alignment segment: :? -+ :?
        if (::pltxt2htm::details::u8string_view_index<ndebug>(line, i) == u8':') {
            ++i;
        }
        ::std::size_t dash_count{};
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr == u8'-') {
                ++dash_count;
            }
            else {
                break;
            }
        }
        if (dash_count < 1) {
            return false;
        }
        if (i < line.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(line, i) == u8':') {
            ++i;
        }
        has_content = true;
        // skip trailing spaces
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        if (i < line.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(line, i) != u8'|') {
            return false;
        }
        if (i < line.size()) {
            ++i;
        }
    }
    return has_content;
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
    [[nodiscard]] constexpr auto header_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols_;
    }

    /// @param col column index
    /// @return const/non-const reference to the header cell at @p col
    [[nodiscard]] constexpr auto header_cell_at(this auto&& self, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.cells_, col);
    }

    /// Append one header cell.  Increments internal column counter.
    constexpr void add_header_cell(this MdTableAstRaw& self, ::pltxt2htm::details::MdTableCellRaw&& cell) noexcept {
        self.cells_.push_back(::std::move(cell));
        ++self.num_cols_;
    }

    /// @return number of body rows (derived from flat vector size and column count)
    [[nodiscard]] constexpr auto body_rows_count(this auto&& self) noexcept -> ::std::size_t {
        pltxt2htm_assert(self.num_cols_ != 0, u8"num_cols_ should be > 0 when calculating body rows count");
        return (self.cells_.size() - self.num_cols_) / self.num_cols_;
    }

    /// @return cells per body row (= num_cols_, all rows have equal length)
    [[nodiscard]] constexpr auto body_cells_count(this auto&& self) noexcept -> ::std::size_t {
        return self.num_cols_;
    }

    /// @param row body row index
    /// @param col column index within the row
    /// @return const/non-const reference to the body cell at (@p row, @p col)
    [[nodiscard]] constexpr auto body_cell_at(this auto&& self, ::std::size_t row, ::std::size_t col) noexcept
        -> decltype(auto) {
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
    ::std::size_t forward_index; ///< number of characters consumed from input
};

/**
 * @brief Result of try_parse_md_table_line: extracted line text and forward index.
 */
struct TryParseMdTableLineResult {
    ::fast_io::u8string line; ///< extracted line text (without trailing newline)
    ::std::size_t forward_index; ///< index past the line (including newline)
};

/**
 * @brief Extract one line (up to `\n`) from pl-text starting at @p offset.
 *
 * The returned line does NOT include the trailing newline.
 *
 * @tparam ndebug Contract checking mode
 * @param pltext The full pl-text input
 * @param offset Start index within @p pltext
 * @return TryParseMdTableLineResult on success, nullopt if @p offset is past the end
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_line(::fast_io::u8string_view pltext, ::std::size_t offset) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdTableLineResult> {
    ::std::size_t const pltext_size{pltext.size()};
    if (offset >= pltext_size) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t line_start{offset};
    ::std::size_t line_end{offset};
    for (; line_end < pltext_size; ++line_end) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, line_end);
        if (chr == u8'\n') {
            break;
        }
    }
    ::fast_io::u8string line{};
    for (::std::size_t i{line_start}; i < line_end; ++i) {
        line.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i));
    }
    ::std::size_t forward_index = line_end < pltext_size ? line_end + 1 : line_end;
    return ::pltxt2htm::details::TryParseMdTableLineResult{::std::move(line), forward_index};
}

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

    // parse first line
    auto first_line_opt = ::pltxt2htm::details::try_parse_md_table_line<ndebug>(pltext, current_index);
    if (!first_line_opt.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto&& [first_line, after_first] = first_line_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index = after_first;

    auto header_row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
        ::fast_io::u8string_view{first_line.data(), first_line.size()});
    if (!header_row_opt.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto header_row = header_row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    ::std::size_t num_cols{header_row.cells.size()};

    // parse delimiter line (second line)
    auto second_line_opt = ::pltxt2htm::details::try_parse_md_table_line<ndebug>(pltext, current_index);
    if (!second_line_opt.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto&& [second_line, after_second] = second_line_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index = after_second;

    if (!::pltxt2htm::details::try_parse_md_table_delimiter<ndebug>(
            ::fast_io::u8string_view{second_line.data(), second_line.size()})) {
        return ::exception::nullopt_t{};
    }

    // parse alignment from delimiter row
    auto delim_row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
        ::fast_io::u8string_view{second_line.data(), second_line.size()});
    ::fast_io::vector<::pltxt2htm::MdTableAlign> aligns{};
    if (delim_row_opt.has_value()) {
        auto delim_row = delim_row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        for (auto const& cell : delim_row.cells) {
            aligns.push_back(::pltxt2htm::details::try_parse_md_table_align<ndebug>(
                ::fast_io::u8string_view{cell.data(), cell.size()}));
        }
    }
    // if delimiter row parsing failed, default to left alignment
    if (aligns.empty()) {
        for (::std::size_t i{}; i < num_cols; ++i) {
            aligns.push_back(::pltxt2htm::MdTableAlign::left);
        }
    }

    // build raw header cells
    ::pltxt2htm::details::MdTableAstRaw<ndebug> raw_ast{};
    for (::std::size_t col{}; col < header_row.cells.size(); ++col) {
        auto const& cell_text = ::pltxt2htm::details::vector_index<ndebug>(header_row.cells, col);
        auto align_val = col < aligns.size() ? ::pltxt2htm::details::vector_index<ndebug>(aligns, col)
                                             : ::pltxt2htm::MdTableAlign::left;
        raw_ast.add_header_cell(::pltxt2htm::details::MdTableCellRaw{
            .text = ::fast_io::u8string{cell_text.data(), cell_text.data() + cell_text.size()},
            .align = align_val,
        });
    }

    // parse data rows
    while (true) {
        auto line_opt = ::pltxt2htm::details::try_parse_md_table_line<ndebug>(pltext, current_index);
        if (!line_opt.has_value()) {
            break;
        }
        auto&& [line_text, after_line] = line_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        auto row_opt = ::pltxt2htm::details::try_parse_md_table_row<ndebug>(
            ::fast_io::u8string_view{line_text.data(), line_text.size()});
        if (!row_opt.has_value()) {
            break;
        }
        current_index = after_line;
        auto row = row_opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>();

        // each body row must have exactly num_cols cells
        if (row.cells.size() != num_cols) {
            return ::exception::nullopt_t{};
        }
        ::fast_io::vector<::pltxt2htm::details::MdTableCellRaw> body_cells{};
        for (::std::size_t col{}; col < row.cells.size(); ++col) {
            auto const& cell_text = ::pltxt2htm::details::vector_index<ndebug>(row.cells, col);
            auto align_val = col < aligns.size() ? ::pltxt2htm::details::vector_index<ndebug>(aligns, col)
                                                 : ::pltxt2htm::MdTableAlign::left;
            body_cells.push_back(::pltxt2htm::details::MdTableCellRaw{
                .text = ::fast_io::u8string{cell_text.data(), cell_text.data() + cell_text.size()},
                .align = align_val,
            });
        }
        raw_ast.add_body_row(::std::move(body_cells));
    }

    return ::pltxt2htm::details::TryParseMdTableRawResult<ndebug>{.raw_ast = ::std::move(raw_ast),
                                                                  .forward_index = current_index};
}

} // namespace pltxt2htm::details

#include "../../details/pop_macro.hh"
