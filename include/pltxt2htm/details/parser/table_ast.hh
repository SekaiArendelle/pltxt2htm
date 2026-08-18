/**
 * @file table_ast.hh
 * @brief Shared intermediate &lt;table&gt; AST produced by the Markdown and HTML scanners.
 *
 * @details The Markdown and HTML table block scanners both build a row-based
 *          intermediate AST: each row carries a section tag (thead/tbody/tfoot
 *          or none) and each cell carries raw text plus alignment and a header
 *          flag.  The shared table frame machinery (see frame_context.hh)
 *          re-parses each cell through the inline parser and groups the rows
 *          back into &lt;tr&gt;/&lt;thead&gt;/&lt;tbody&gt;/&lt;tfoot&gt; nodes,
 *          with the node kind selected by the owning table node kind
 *          (::pltxt2htm::NodeKind::md_table or ::pltxt2htm::NodeKind::html_table).
 *          Analogous to ListAst.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <exception/exception.hh>
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
    ::fast_io::u8string text; ///< Cell raw content (to be inline-parsed later).
    ::pltxt2htm::TableAlign align; ///< Cell alignment.
    bool is_header; ///< True for &lt;th&gt; (or Markdown header) cells, false for &lt;td&gt; (or Markdown body) cells.
};

/**
 * @brief Section a row belongs to, or `none` for rows directly under &lt;table&gt;.
 */
enum class TableRowSection : unsigned {
    none,
    thead,
    tbody,
    tfoot
};

/**
 * @brief Raw row data for the intermediate table AST.
 */
struct TableRowRaw {
    ::fast_io::vector<::pltxt2htm::details::TableCellRaw> cells; ///< Row cells in authored order.
    ::pltxt2htm::details::TableRowSection section; ///< thead/tbody/tfoot or none.
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
    ::fast_io::vector<::pltxt2htm::details::TableRowRaw> rows{};
    ::fast_io::u8string caption_text{};
    bool has_caption_{};
    ::std::size_t col_count_{};
    bool has_colgroup_{};

public:
    /// @return Whether a caption was recorded.
    [[nodiscard]]
    constexpr auto has_caption(this auto&& self) noexcept -> bool {
        return self.has_caption_;
    }

    /// @return Raw caption content (valid only when has_caption()).
    [[nodiscard]]
    constexpr auto caption(this auto&& self) noexcept -> ::fast_io::u8string_view {
        return ::fast_io::u8string_view{self.caption_text.data(), self.caption_text.size()};
    }

    /// Record the table caption and that it was present.
    constexpr void set_caption(this TableAstRaw& self, ::fast_io::u8string&& text) noexcept {
        self.caption_text = ::std::move(text);
        self.has_caption_ = true;
    }

    /// @return Whether a colgroup was present.
    [[nodiscard]]
    constexpr auto has_colgroup(this auto&& self) noexcept -> bool {
        return self.has_colgroup_;
    }

    /// @return Number of &lt;col&gt; elements inside the colgroup.
    [[nodiscard]]
    constexpr auto col_count(this auto&& self) noexcept -> ::std::size_t {
        return self.col_count_;
    }

    /// Record one &lt;col&gt; element.
    constexpr void add_col(this TableAstRaw& self) noexcept {
        self.has_colgroup_ = true;
        ++self.col_count_;
    }

    /// @return Number of rows.
    [[nodiscard]]
    constexpr auto rows_count(this auto&& self) noexcept -> ::std::size_t {
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
    constexpr auto row_section(this auto&& self, ::std::size_t row) noexcept -> ::pltxt2htm::details::TableRowSection {
        return ::pltxt2htm::details::vector_index<ndebug>(self.rows, row).section;
    }

    /// Append a row (its section tag is supplied by the scanner).
    constexpr void add_row(this TableAstRaw& self, ::pltxt2htm::details::TableRowRaw&& row) noexcept {
        self.rows.push_back(::std::move(row));
    }

    /// Append a cell to the most recently added row.
    constexpr void add_cell_to_last_row(this TableAstRaw& self, ::pltxt2htm::details::TableCellRaw&& cell) noexcept {
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
 * @brief Node kinds used to assemble a table AST for a given table flavor.
 */
struct TableNodeKinds {
    ::pltxt2htm::NodeKind table; ///< The table node kind itself.
    ::pltxt2htm::NodeKind th; ///< Header cell node kind.
    ::pltxt2htm::NodeKind td; ///< Data cell node kind.
    ::pltxt2htm::NodeKind tr; ///< Row node kind.
    ::pltxt2htm::NodeKind thead; ///< Header section node kind.
    ::pltxt2htm::NodeKind tbody; ///< Body section node kind.
    ::pltxt2htm::NodeKind tfoot; ///< Footer section node kind.
};

/**
 * @brief Map a table node kind to the flavors of the child node kinds.
 * @tparam ndebug Contract checking mode.
 * @param table_kind The owning table node kind (md_table or html_table).
 * @return The child node kinds for that flavor.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto table_node_kinds(::pltxt2htm::NodeKind const table_kind) noexcept
    -> ::pltxt2htm::details::TableNodeKinds {
    switch (table_kind) /* -Werror=switch */ {
    case ::pltxt2htm::NodeKind::md_table:
        return {.table = ::pltxt2htm::NodeKind::md_table,
                .th = ::pltxt2htm::NodeKind::md_th,
                .td = ::pltxt2htm::NodeKind::md_td,
                .tr = ::pltxt2htm::NodeKind::md_tr,
                .thead = ::pltxt2htm::NodeKind::md_thead,
                .tbody = ::pltxt2htm::NodeKind::md_tbody,
                .tfoot = ::pltxt2htm::NodeKind::md_tbody};
    case ::pltxt2htm::NodeKind::html_table:
        return {.table = ::pltxt2htm::NodeKind::html_table,
                .th = ::pltxt2htm::NodeKind::html_th,
                .td = ::pltxt2htm::NodeKind::html_td,
                .tr = ::pltxt2htm::NodeKind::html_tr,
                .thead = ::pltxt2htm::NodeKind::html_thead,
                .tbody = ::pltxt2htm::NodeKind::html_tbody,
                .tfoot = ::pltxt2htm::NodeKind::html_tfoot};
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected table node kind");
        }
    }
}

/**
 * @brief Append a &lt;tr&gt; node of the given flavor to an AST.
 * @tparam ndebug Contract checking mode.
 * @param parent_ast AST to append to.
 * @param tr_kind The row node kind (md_tr or html_tr).
 * @param tr_ast The row's sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void push_table_tr_node(::pltxt2htm::Ast<ndebug>& parent_ast, ::pltxt2htm::NodeKind const tr_kind,
                                  ::pltxt2htm::Ast<ndebug>&& tr_ast) noexcept {
    switch (tr_kind) /* -Werror=switch */ {
    case ::pltxt2htm::NodeKind::md_tr: {
        parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTr<ndebug>{::std::move(tr_ast)}));
        return;
    }
    case ::pltxt2htm::NodeKind::html_tr: {
        parent_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(tr_ast)}));
        return;
    }
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected table row node kind");
        }
    }
}

/**
 * @brief Append a table section node (&lt;thead&gt;/&lt;tbody&gt;/&lt;tfoot&gt;) of the given flavor to an AST.
 * @tparam ndebug Contract checking mode.
 * @param table_ast AST of the owning table node.
 * @param section_kind The section node kind (md_thead/md_tbody/html_thead/html_tbody/html_tfoot).
 * @param section_ast The section's sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void push_table_section_node(::pltxt2htm::Ast<ndebug>& table_ast, ::pltxt2htm::NodeKind const section_kind,
                                       ::pltxt2htm::Ast<ndebug>&& section_ast) noexcept {
    switch (section_kind) /* -Werror=switch */ {
    case ::pltxt2htm::NodeKind::text:
        return;
    case ::pltxt2htm::NodeKind::md_thead: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdThead<ndebug>{::std::move(section_ast)}));
        return;
    }
    case ::pltxt2htm::NodeKind::md_tbody: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdTbody<ndebug>{::std::move(section_ast)}));
        return;
    }
    case ::pltxt2htm::NodeKind::html_thead: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlThead<ndebug>{::std::move(section_ast)}));
        return;
    }
    case ::pltxt2htm::NodeKind::html_tbody: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTbody<ndebug>{::std::move(section_ast)}));
        return;
    }
    case ::pltxt2htm::NodeKind::html_tfoot: {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTfoot<ndebug>{::std::move(section_ast)}));
        return;
    }
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected section node kind");
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
