#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "try_parse.hh"

namespace pltxt2htm::details {

enum class MdTableAlign : ::std::uint_least32_t {
    left = 0,
    center,
    right,
};

struct MdTableRow {
    ::fast_io::vector<::fast_io::u8string> cells;
};

struct MdTableData {
    ::fast_io::vector<::pltxt2htm::details::MdTableRow> rows;
    ::fast_io::vector<::pltxt2htm::details::MdTableAlign> aligns;
};

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
        // parse cell content until | or end of line
        ::fast_io::u8string cell{};
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr == u8'|') {
                break;
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

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table_align(::fast_io::u8string_view cell_text) noexcept
    -> ::pltxt2htm::details::MdTableAlign {
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
            return ::pltxt2htm::details::MdTableAlign::left;
        }
    }
    if (left && right) {
        return ::pltxt2htm::details::MdTableAlign::center;
    }
    if (right) {
        return ::pltxt2htm::details::MdTableAlign::right;
    }
    if (left) {
        return ::pltxt2htm::details::MdTableAlign::left;
    }
    return ::pltxt2htm::details::MdTableAlign::left;
}

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
        // parse alignment segment
        if (::pltxt2htm::details::u8string_view_index<ndebug>(line, i) == u8':') {
            ++i;
        }
        ::std::size_t dash_count{};
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr == u8'-') {
                ++dash_count;
            }
            else if (chr == u8':') {
                ++i;
                break;
            }
            else {
                break;
            }
        }
        if (dash_count < 1) {
            return false;
        }
        has_content = true;
        // skip spaces until | or end
        for (; i < line.size(); ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(line, i);
            if (chr != u8' ' && chr != u8'\t') {
                break;
            }
        }
        if (i < line.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(line, i) == u8'|') {
            ++i;
        }
    }
    return has_content;
}

/**
 * @brief Basic text parser for table cell content.
 *
 * Handles special characters, escape sequences, and UTF-8 code points
 * without requiring an end-string template parameter.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_cell_text(::fast_io::u8string_view text) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::Ast<ndebug> ast{};
    ::std::size_t current_index{};
    ::std::size_t const text_size{text.size()};
    while (current_index < text_size) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, current_index)};
        if (chr == u8' ') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
            ++current_index;
            continue;
        }
        if (chr == u8'&') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\'') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuotationMark{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\"') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuotationMark{}));
            ++current_index;
            continue;
        }
        if (chr == u8'>') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\t') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\\') {
            if (current_index + 1 == text_size) {
                ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}));
                ++current_index;
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char<ndebug>(
                ::pltxt2htm::details::u8string_view_index<ndebug>(text, current_index + 1));
            if (escape_node.has_value()) {
                ast.push_back(::std::move(escape_node.template value<ndebug == ::pltxt2htm::Contracts::ignore>()));
                ++current_index;
            }
            else {
                ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}));
            }
            ++current_index;
            continue;
        }
        if (chr == u8'<') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            ++current_index;
            continue;
        }
        auto forward_index = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(text, current_index), ast);
        current_index += forward_index;
    }
    return ast;
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdTableResult {
    ::pltxt2htm::Ast<ndebug> ast;
    ::std::size_t forward_index;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_table(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdTableResult<ndebug>> {
    ::std::size_t current_index{};
    ::std::size_t const pltext_size{pltext.size()};

    auto parse_line = [&](::std::size_t offset) -> ::exception::optional<::std::pair<::fast_io::u8string, ::std::size_t>> {
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
        ::std::size_t next_offset = line_end < pltext_size ? line_end + 1 : line_end;
        return ::std::pair{::std::move(line), next_offset};
    };

    // parse first line
    auto first_line_opt = parse_line(current_index);
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
    auto second_line_opt = parse_line(current_index);
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
    ::fast_io::vector<::pltxt2htm::details::MdTableAlign> aligns{};
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
            aligns.push_back(::pltxt2htm::details::MdTableAlign::left);
        }
    }

    // parse data rows
    ::pltxt2htm::Ast<ndebug> table_ast{};

    // build <table>
    // build <thead><tr>
    ::pltxt2htm::Ast<ndebug> thead_ast{};
    ::pltxt2htm::Ast<ndebug> header_tr_ast{};
    for (::std::size_t col{}; col < header_row.cells.size(); ++col) {
        auto const& cell_text = header_row.cells[col];
        auto parsed = ::pltxt2htm::details::parse_cell_text<ndebug>(
            ::fast_io::u8string_view{cell_text.data(), cell_text.size()});
        header_tr_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
            ::pltxt2htm::HtmlTh<ndebug>{::std::move(parsed)}));
    }
    thead_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(header_tr_ast)}));
    table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlThead<ndebug>{::std::move(thead_ast)}));

    // build <tbody>
    ::pltxt2htm::Ast<ndebug> tbody_ast{};
    while (true) {
        auto line_opt = parse_line(current_index);
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

        ::pltxt2htm::Ast<ndebug> tr_ast{};
        for (::std::size_t col{}; col < row.cells.size() && col < num_cols; ++col) {
            auto const& cell_text = row.cells[col];
            auto parsed = ::pltxt2htm::details::parse_cell_text<ndebug>(
                ::fast_io::u8string_view{cell_text.data(), cell_text.size()});
            tr_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(
                ::pltxt2htm::HtmlTd<ndebug>{::std::move(parsed)}));
        }
        tbody_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTr<ndebug>{::std::move(tr_ast)}));
    }

    if (!tbody_ast.empty()) {
        table_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::HtmlTbody<ndebug>{::std::move(tbody_ast)}));
    }

    return ::pltxt2htm::details::TryParseMdTableResult<ndebug>{
        .ast = ::std::move(table_ast),
        .forward_index = current_index};
}

} // namespace pltxt2htm::details
