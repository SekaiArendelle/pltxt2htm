/**
 * @file table_ast.hh
 * @brief Shared intermediate &lt;table&gt; AST produced by the Markdown and HTML scanners.
 *
 * @details The Markdown and HTML table block scanners both build a row-based
 *          intermediate AST: each row carries a section tag (thead/tbody/tfoot
 *          or none) and each cell carries raw text plus alignment and a header
 *          flag.  The shared table frame machinery (see frame_context.hh)
 *          re-parses each cell through the inline parser and groups the rows
 *          back into &lt;tr&gt;/&lt;thead&gt;/&lt;tbody&gt;/&lt;tfoot&gt; nodes.
 *          Analogous to ListAst.
 */

#pragma once

#include <cstddef>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include <fast_io/fast_io_dsal/vector.h>
#include "../../container/optional.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/node_kind.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Raw cell data (text + alignment + header flag) for the intermediate table AST.
 */
struct TableCellRaw {
    ::pltxt2htm::container::U8String text; ///< Cell raw content (to be inline-parsed later).
    ::pltxt2htm::TableAlign align; ///< Cell alignment.
    bool is_header; ///< True for &lt;th&gt; (or Markdown header) cells, false for &lt;td&gt; (or Markdown body) cells.
};

/**
 * @brief Section a row belongs to, or `none` for rows directly under &lt;table&gt;.
 */
enum class TableRowSection : unsigned {
    none = 0,
    thead,
    tbody,
    tfoot
};

/**
 * @brief Raw row data for the intermediate table AST.
 */
struct TableRowRaw {
    ::fast_io::vector<TableCellRaw> cells; ///< Row cells in authored order.
    TableRowSection section; ///< thead/tbody/tfoot or none.
};

/**
 * @brief Intermediate table AST representing raw cell content.
 *
 * Stores rows (each with a section tag) plus an optional caption and colgroup
 * column count.  Analogous to ListAst: raw text is re-parsed through the
 * inline parser when the table frame is processed.
 *
 * @tparam ndebug Contract checking mode
 */
template<::pltxt2htm::Contracts ndebug>
class TableAstRaw {
    ::fast_io::vector<TableRowRaw> rows{};
    ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String> caption_text{::pltxt2htm::container::nullopt};
    ::std::size_t col_count{}; // <==> optional<non_zero_usize> col_count;

public:
    /// @return Whether a caption was recorded.
    [[nodiscard]]
    constexpr auto has_caption(this TableAstRaw<ndebug> const& self) noexcept -> bool {
        return self.caption_text.has_value();
    }

    /// @return Raw caption content (valid only when has_caption()).
    [[nodiscard]]
    constexpr auto caption(this TableAstRaw<ndebug> const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        auto&& text = self.caption_text.template value<ndebug>();
        return ::pltxt2htm::container::U8StringView{text};
    }

    /// Record the table caption and that it was present.
    constexpr void set_caption(this TableAstRaw& self, ::pltxt2htm::container::U8String&& text) noexcept {
        self.caption_text = ::std::move(text);
    }

    /// @return Whether a colgroup was present.
    [[nodiscard]]
    constexpr auto has_colgroup(this auto const& self) noexcept -> bool {
        return self.col_count != 0;
    }

    /// @return Number of &lt;col&gt; elements inside the colgroup.
    [[nodiscard]]
    constexpr auto get_col_count(this auto const& self) noexcept -> ::std::size_t {
        return self.col_count;
    }

    /// Record one &lt;col&gt; element.
    constexpr void add_col(this TableAstRaw& self) noexcept {
        ++self.col_count;
    }

    /// @return Number of rows.
    [[nodiscard]]
    constexpr auto rows_count(this auto const& self) noexcept -> ::std::size_t {
        return self.rows.size();
    }

    /// @param row Row index.
    /// @return Cells in row @p row (const/non-const reference).
    [[nodiscard]]
    constexpr auto row_cells(this auto&& self, ::std::size_t row) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(self.rows, row).cells;
    }

    /// @param row Row index.
    /// @param col Column index.
    /// @return Cell at (@p row, @p col) (const/non-const reference).
    [[nodiscard]]
    constexpr auto cell_at(this auto&& self, ::std::size_t row, ::std::size_t col) noexcept -> decltype(auto) {
        return ::pltxt2htm::details::vector_index<ndebug>(
            ::pltxt2htm::details::vector_index<ndebug>(self.rows, row).cells, col);
    }

    /// @param row Row index.
    /// @return Section tag of row @p row.
    [[nodiscard]]
    constexpr auto row_section(this TableAstRaw<ndebug> const& self, ::std::size_t row) noexcept -> TableRowSection {
        return ::pltxt2htm::details::vector_index<ndebug>(self.rows, row).section;
    }

    /// Append a row (its section tag is supplied by the scanner).
    constexpr void add_row(this TableAstRaw& self, TableRowRaw&& row) noexcept {
        self.rows.push_back(::std::move(row));
    }

    /// Append a cell to the most recently added row.
    constexpr void add_cell_to_last_row(this TableAstRaw& self, TableCellRaw&& cell) noexcept {
        ::pltxt2htm::details::vector_index<ndebug>(self.rows, self.rows.size() - 1).cells.push_back(::std::move(cell));
    }
};

/**
 * @brief Phase of the shared table frame state machine.
 *
 * Unifies the per-scanner phase enums: caption (optional caption frame),
 * body (walk the rows and push a cell frame per cell) and finish (assemble
 * the &lt;tr&gt;/section nodes and pop).
 */
enum class TableParsePhase : unsigned {
    caption = 0,
    body,
    finish
};

/**
 * @brief Append a table section node (&lt;thead&gt;/&lt;tbody&gt;/&lt;tfoot&gt;) to a table AST.
 * @tparam ndebug Contract checking mode.
 * @param table_ast AST of the owning table node.
 * @param section The row section tag (thead/tbody/tfoot, or none for no-op).
 * @param section_ast The section's sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void push_table_section_node(::pltxt2htm::Ast<ndebug>& table_ast, TableRowSection const section,
                                       ::pltxt2htm::Ast<ndebug>&& section_ast) noexcept {
    switch (section) /* -Werror=switch */ {
    case TableRowSection::thead: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableThead<ndebug>{::std::move(section_ast)}));
        return;
    }
    case TableRowSection::tbody: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTbody<ndebug>{::std::move(section_ast)}));
        return;
    }
    case TableRowSection::tfoot: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::TableTfoot<ndebug>{::std::move(section_ast)}));
        return;
    }
    case TableRowSection::none: {
        return;
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected TableRowSection");
        }
#endif
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
