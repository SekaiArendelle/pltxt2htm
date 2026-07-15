/**
 * @file html_parser.hh
 * @brief HTML-only parser interface for pltxt2htm
 * @details Provides parsing functionality to convert an HTML subset of pl-text
 *          into an Abstract Syntax Tree (AST). Supports HTML tags only,
 *          no Markdown or Physics-Lab specific syntax.
 */
#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "ast/node_kind.hh"
#include "contracts.hh"
#include "details/utils.hh"
#include "details/html_parser/frame_concext.hh"
#include "details/html_parser/parser.hh"
#include "details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief Parse HTML-only text into an Abstract Syntax Tree (AST)
 * @details Converts text containing HTML tags into a structured AST.
 *          Supports a subset of HTML tags including &lt;p&gt;, &lt;h1&gt;-&lt;h6&gt;,
 *          &lt;table&gt;, &lt;ul&gt;, &lt;ol&gt;, &lt;li&gt;, &lt;a&gt;, &lt;span&gt;,
 *          &lt;em&gt;, &lt;strong&gt;, &lt;del&gt;, &lt;code&gt;, &lt;pre&gt;,
 *          &lt;blockquote&gt;, &lt;br&gt;, &lt;hr&gt;, &lt;img&gt;, &lt;input&gt;,
 *          &lt;!-- --&gt; comments, and table elements (&lt;table&gt;, &lt;tr&gt;,
 *          &lt;td&gt;, &lt;th&gt;, &lt;thead&gt;, &lt;tbody&gt;, &lt;tfoot&gt;,
 *          &lt;caption&gt;, &lt;colgroup&gt;, &lt;col&gt;).
 *
 *          Does NOT support Markdown syntax (headings, emphasis, lists, etc.)
 *          or Physics-Lab specific tags (&lt;color&gt;, &lt;experiment&gt;, etc.).
 * @tparam ndebug Contract-checking mode
 * @param[in] html_text The HTML text content to parse
 * @return An AST representing the parsed structure
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_html(::fast_io::u8string_view html_text) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::fast_io::stack<::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>> call_stack{};

    call_stack.push(::pltxt2htm::details::html_parser::ParserFrameContext<ndebug>(
        ::pltxt2htm::details::html_parser::FrontendContextVariant<ndebug>{
            ::pltxt2htm::details::html_parser::ParserFrameContextWithPltextInfo{
                html_text},
            ::pltxt2htm::NodeKind::text},
        ::pltxt2htm::Ast<ndebug>{}));

    auto result = ::pltxt2htm::details::html_parser::parse_pltxt_html<ndebug>(call_stack);

    bool const call_stack_is_empty{call_stack.empty()};
    pltxt2htm_assert(call_stack_is_empty, u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
