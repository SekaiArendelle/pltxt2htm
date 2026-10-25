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
class Table {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit Table(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr Table(::pltxt2htm::Table const&) noexcept = default;
    constexpr Table(::pltxt2htm::Table&&) noexcept;
    constexpr ~Table() noexcept = default;
    constexpr auto operator=(::pltxt2htm::Table const&) noexcept -> ::pltxt2htm::Table& = default;
    constexpr auto operator=(this Table& self, ::pltxt2htm::Table&&) noexcept
        -> ::pltxt2htm::Table&;

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
class TableTr {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableTr(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableTr(::pltxt2htm::TableTr const&) noexcept = default;
    constexpr TableTr(::pltxt2htm::TableTr&&) noexcept;
    constexpr ~TableTr() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableTr const&) noexcept -> ::pltxt2htm::TableTr& = default;
    constexpr auto operator=(this TableTr& self, ::pltxt2htm::TableTr&&) noexcept
        -> ::pltxt2htm::TableTr&;

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
class TableTd {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::TableAlign align;

public:
    constexpr explicit TableTd(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TableAlign align_) noexcept;
    constexpr TableTd(::pltxt2htm::TableTd const&) noexcept = default;
    constexpr TableTd(::pltxt2htm::TableTd&&) noexcept;
    constexpr ~TableTd() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableTd const&) noexcept -> ::pltxt2htm::TableTd& = default;
    constexpr auto operator=(this TableTd& self, ::pltxt2htm::TableTd&&) noexcept
        -> ::pltxt2htm::TableTd&;

    [[nodiscard]]
    constexpr auto operator==(this TableTd const&, TableTd const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this TableTd const& self) noexcept -> ::pltxt2htm::TableAlign {
        return self.align;
    }
};

/**
 * @brief Table header cell node (&lt;th&gt;...&lt;/th&gt; or Markdown header cell)
 */
class TableTh {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::TableAlign align;

public:
    constexpr explicit TableTh(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TableAlign align_) noexcept;
    constexpr TableTh(::pltxt2htm::TableTh const&) noexcept = default;
    constexpr TableTh(::pltxt2htm::TableTh&&) noexcept;
    constexpr ~TableTh() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableTh const&) noexcept -> ::pltxt2htm::TableTh& = default;
    constexpr auto operator=(this TableTh& self, ::pltxt2htm::TableTh&&) noexcept
        -> ::pltxt2htm::TableTh&;

    [[nodiscard]]
    constexpr auto operator==(this TableTh const&, TableTh const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this TableTh const& self) noexcept -> ::pltxt2htm::TableAlign {
        return self.align;
    }
};

/**
 * @brief Table header section node (&lt;thead&gt;...&lt;/thead&gt; or Markdown header)
 */
class TableThead {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableThead(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableThead(::pltxt2htm::TableThead const&) noexcept = default;
    constexpr TableThead(::pltxt2htm::TableThead&&) noexcept;
    constexpr ~TableThead() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableThead const&) noexcept
        -> ::pltxt2htm::TableThead& = default;
    constexpr auto operator=(this TableThead& self, ::pltxt2htm::TableThead&&) noexcept
        -> ::pltxt2htm::TableThead&;

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
class TableTbody {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableTbody(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableTbody(::pltxt2htm::TableTbody const&) noexcept = default;
    constexpr TableTbody(::pltxt2htm::TableTbody&&) noexcept;
    constexpr ~TableTbody() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableTbody const&) noexcept
        -> ::pltxt2htm::TableTbody& = default;
    constexpr auto operator=(this TableTbody& self, ::pltxt2htm::TableTbody&&) noexcept
        -> ::pltxt2htm::TableTbody&;

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
class TableTfoot {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableTfoot(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableTfoot(::pltxt2htm::TableTfoot const&) noexcept = default;
    constexpr TableTfoot(::pltxt2htm::TableTfoot&&) noexcept;
    constexpr ~TableTfoot() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableTfoot const&) noexcept
        -> ::pltxt2htm::TableTfoot& = default;
    constexpr auto operator=(this TableTfoot& self, ::pltxt2htm::TableTfoot&&) noexcept
        -> ::pltxt2htm::TableTfoot&;

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
class TableCaption {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableCaption(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableCaption(::pltxt2htm::TableCaption const&) noexcept = default;
    constexpr TableCaption(::pltxt2htm::TableCaption&&) noexcept;
    constexpr ~TableCaption() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableCaption const&) noexcept
        -> ::pltxt2htm::TableCaption& = default;
    constexpr auto operator=(this TableCaption& self, ::pltxt2htm::TableCaption&&) noexcept
        -> ::pltxt2htm::TableCaption&;

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
class TableColgroup {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit TableColgroup(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr TableColgroup(::pltxt2htm::TableColgroup const&) noexcept = default;
    constexpr TableColgroup(::pltxt2htm::TableColgroup&&) noexcept;
    constexpr ~TableColgroup() noexcept = default;
    constexpr auto operator=(::pltxt2htm::TableColgroup const&) noexcept
        -> ::pltxt2htm::TableColgroup& = default;
    constexpr auto operator=(this TableColgroup& self, ::pltxt2htm::TableColgroup&&) noexcept
        -> ::pltxt2htm::TableColgroup&;

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
    [[nodiscard]]
    constexpr auto operator==(this TableCol const&, TableCol const&) noexcept -> bool = default;
};

} // namespace pltxt2htm
