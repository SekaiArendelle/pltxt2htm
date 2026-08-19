/**
 * @file table_node_decl.hh
 * @brief Table AST node declarations (shared by HTML and Markdown tables)
 * @details Defines the table container, row, cell and section nodes: Table,
 *          TableTr, TableTd, TableTh, TableThead, TableTbody, TableTfoot,
 *          TableCaption, TableColgroup and TableCol.  Both the HTML
 *          &lt;table&gt; tags and the Markdown pipe-table scanner produce
 *          these nodes.
 */

#pragma once

#include <cstddef>
#include <utility>
#include "ast_decl.hh"
#include "../node_kind.hh"

namespace pltxt2htm {

/**
 * @brief Table node (&lt;table&gt;...&lt;/table&gt; or Markdown pipe-table)
 */
template<::pltxt2htm::Contracts ndebug>
class Table {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit Table(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr Table(::pltxt2htm::Table<ndebug> const&) noexcept;
    constexpr Table(::pltxt2htm::Table<ndebug>&&) noexcept;
    constexpr ~Table() noexcept;
    constexpr auto operator=(::pltxt2htm::Table<ndebug> const&) noexcept -> ::pltxt2htm::Table<ndebug>& = delete;
    constexpr auto operator=(this Table<ndebug>& self, ::pltxt2htm::Table<ndebug>&&) noexcept
        -> ::pltxt2htm::Table<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this Table const&, Table const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table row node (&lt;tr&gt;...&lt;/tr&gt; or Markdown pipe-table row)
 */
template<::pltxt2htm::Contracts ndebug>
class TableTr {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableTr(::pltxt2htm::TableTr<ndebug> const&) noexcept;
    constexpr TableTr(::pltxt2htm::TableTr<ndebug>&&) noexcept;
    constexpr ~TableTr() noexcept;
    constexpr auto operator=(::pltxt2htm::TableTr<ndebug> const&) noexcept -> ::pltxt2htm::TableTr<ndebug>& = delete;
    constexpr auto operator=(this TableTr<ndebug>& self, ::pltxt2htm::TableTr<ndebug>&&) noexcept
        -> ::pltxt2htm::TableTr<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableTr const&, TableTr const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table data cell node (&lt;td&gt;...&lt;/td&gt; or Markdown cell)
 */
template<::pltxt2htm::Contracts ndebug>
class TableTd {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::TableAlign align;

public:
    constexpr explicit TableTd(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::TableAlign align_) noexcept;
    constexpr TableTd(::pltxt2htm::TableTd<ndebug> const&) noexcept;
    constexpr TableTd(::pltxt2htm::TableTd<ndebug>&&) noexcept;
    constexpr ~TableTd() noexcept;
    constexpr auto operator=(::pltxt2htm::TableTd<ndebug> const&) noexcept -> ::pltxt2htm::TableTd<ndebug>& = delete;
    constexpr auto operator=(this TableTd<ndebug>& self, ::pltxt2htm::TableTd<ndebug>&&) noexcept
        -> ::pltxt2htm::TableTd<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableTd const&, TableTd const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto&& self) noexcept -> ::pltxt2htm::TableAlign {
        return self.align;
    }
};

/**
 * @brief Table header cell node (&lt;th&gt;...&lt;/th&gt; or Markdown header cell)
 */
template<::pltxt2htm::Contracts ndebug>
class TableTh {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::TableAlign align;

public:
    constexpr explicit TableTh(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::TableAlign align_) noexcept;
    constexpr TableTh(::pltxt2htm::TableTh<ndebug> const&) noexcept;
    constexpr TableTh(::pltxt2htm::TableTh<ndebug>&&) noexcept;
    constexpr ~TableTh() noexcept;
    constexpr auto operator=(::pltxt2htm::TableTh<ndebug> const&) noexcept -> ::pltxt2htm::TableTh<ndebug>& = delete;
    constexpr auto operator=(this TableTh<ndebug>& self, ::pltxt2htm::TableTh<ndebug>&&) noexcept
        -> ::pltxt2htm::TableTh<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableTh const&, TableTh const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto&& self) noexcept -> ::pltxt2htm::TableAlign {
        return self.align;
    }
};

/**
 * @brief Table header section node (&lt;thead&gt;...&lt;/thead&gt; or Markdown header)
 */
template<::pltxt2htm::Contracts ndebug>
class TableThead {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableThead(::pltxt2htm::TableThead<ndebug> const&) noexcept;
    constexpr TableThead(::pltxt2htm::TableThead<ndebug>&&) noexcept;
    constexpr ~TableThead() noexcept;
    constexpr auto operator=(::pltxt2htm::TableThead<ndebug> const&) noexcept
        -> ::pltxt2htm::TableThead<ndebug>& = delete;
    constexpr auto operator=(this TableThead<ndebug>& self, ::pltxt2htm::TableThead<ndebug>&&) noexcept
        -> ::pltxt2htm::TableThead<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableThead const&, TableThead const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table body section node (&lt;tbody&gt;...&lt;/tbody&gt; or Markdown body)
 */
template<::pltxt2htm::Contracts ndebug>
class TableTbody {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableTbody(::pltxt2htm::TableTbody<ndebug> const&) noexcept;
    constexpr TableTbody(::pltxt2htm::TableTbody<ndebug>&&) noexcept;
    constexpr ~TableTbody() noexcept;
    constexpr auto operator=(::pltxt2htm::TableTbody<ndebug> const&) noexcept
        -> ::pltxt2htm::TableTbody<ndebug>& = delete;
    constexpr auto operator=(this TableTbody<ndebug>& self, ::pltxt2htm::TableTbody<ndebug>&&) noexcept
        -> ::pltxt2htm::TableTbody<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableTbody const&, TableTbody const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table foot section node (&lt;tfoot&gt;...&lt;/tfoot&gt;)
 */
template<::pltxt2htm::Contracts ndebug>
class TableTfoot {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableTfoot(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableTfoot(::pltxt2htm::TableTfoot<ndebug> const&) noexcept;
    constexpr TableTfoot(::pltxt2htm::TableTfoot<ndebug>&&) noexcept;
    constexpr ~TableTfoot() noexcept;
    constexpr auto operator=(::pltxt2htm::TableTfoot<ndebug> const&) noexcept
        -> ::pltxt2htm::TableTfoot<ndebug>& = delete;
    constexpr auto operator=(this TableTfoot<ndebug>& self, ::pltxt2htm::TableTfoot<ndebug>&&) noexcept
        -> ::pltxt2htm::TableTfoot<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableTfoot const&, TableTfoot const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table caption node (&lt;caption&gt;...&lt;/caption&gt;)
 */
template<::pltxt2htm::Contracts ndebug>
class TableCaption {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableCaption(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableCaption(::pltxt2htm::TableCaption<ndebug> const&) noexcept;
    constexpr TableCaption(::pltxt2htm::TableCaption<ndebug>&&) noexcept;
    constexpr ~TableCaption() noexcept;
    constexpr auto operator=(::pltxt2htm::TableCaption<ndebug> const&) noexcept
        -> ::pltxt2htm::TableCaption<ndebug>& = delete;
    constexpr auto operator=(this TableCaption<ndebug>& self, ::pltxt2htm::TableCaption<ndebug>&&) noexcept
        -> ::pltxt2htm::TableCaption<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableCaption const&, TableCaption const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table column group node (&lt;colgroup&gt;...&lt;/colgroup&gt;)
 */
template<::pltxt2htm::Contracts ndebug>
class TableColgroup {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit TableColgroup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr TableColgroup(::pltxt2htm::TableColgroup<ndebug> const&) noexcept;
    constexpr TableColgroup(::pltxt2htm::TableColgroup<ndebug>&&) noexcept;
    constexpr ~TableColgroup() noexcept;
    constexpr auto operator=(::pltxt2htm::TableColgroup<ndebug> const&) noexcept
        -> ::pltxt2htm::TableColgroup<ndebug>& = delete;
    constexpr auto operator=(this TableColgroup<ndebug>& self, ::pltxt2htm::TableColgroup<ndebug>&&) noexcept
        -> ::pltxt2htm::TableColgroup<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this TableColgroup const&, TableColgroup const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Table column node (&lt;col&gt; self-closing)
 */
class TableCol {
public:
    constexpr auto operator==(this TableCol const&, TableCol const&) noexcept -> bool = default;
};

} // namespace pltxt2htm