/**
 * @file frame_concext.hh
 * @brief Parser frame contexts for parsing pl-text
 * @details Defines various context classes used during the parsing process
 *          to maintain state and handle different types of parsing scenarios
 */

#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "md_list.hh"
#include "../../astnode/basic.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Basic frame context for parser state management
 * @details This is the fundamental context class that tracks the current
 *          parsing state including node type, current index, and sub-AST
 */
class BasicFrameContext {
public:
    ::pltxt2htm::NodeType const nested_tag_type; ///< Type of node being parsed in this context
    ::std::size_t current_index{}; ///< Current parsing position index
    ::pltxt2htm::Ast subast{}; ///< Sub-AST being built in this context

protected:
    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : nested_tag_type{nested_tag_type_} {
    }

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Ast&& subast_) noexcept
        : nested_tag_type{nested_tag_type_},
          subast(::std::move(subast_)) {
    }

    constexpr BasicFrameContext(BasicFrameContext const&) noexcept = delete;

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext&&) noexcept = default;

public:
    constexpr ~BasicFrameContext() noexcept = default;
};

/**
 * @brief Context for parsing bare tags (tags without attributes)
 * @details Used when parsing simple tags that don't have attributes like <b>, <i>, etc.
 */
class BareTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext; ///< The text being parsed in this context

    constexpr explicit BareTagContext(::fast_io::u8string_view pltext_,
                                      ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(nested_tag_type_),
          pltext(pltext_) {
    }

    constexpr explicit BareTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                      ::pltxt2htm::Ast&& subast_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(nested_tag_type_, ::std::move(subast_)),
          pltext(pltext_) {
    }

    constexpr ~BareTagContext() noexcept = default;
};

/**
 * @brief Context for parsing tags with equal sign attributes
 * @details Used when parsing tags that have attributes in the format key=value
 *          such as <color=red>, <size=12>, <experiment=123>, etc.
 */
class EqualSignTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext; ///< The text being parsed in this context
    ::fast_io::u8string id; ///< The value part of the attribute (e.g., "red" in color=red)

    constexpr explicit EqualSignTagContext(::fast_io::u8string_view pltext_,
                                           ::pltxt2htm::NodeType const nested_tag_type_,
                                           ::fast_io::u8string&& id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{nested_tag_type_},
          pltext(pltext_),
          id(::std::move(id_)) {
    }

    constexpr ~EqualSignTagContext() noexcept = default;
};

/**
 * @brief Context for parsing size tags with numeric values
 * @details Specialized context for size tags that expect numeric values
 *          like <size=12> where the value should be converted to std::size_t
 */
class PlSizeTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext; ///< The text being parsed in this context
    ::std::size_t id; ///< Numeric size value (e.g., 12 in size=12)

    constexpr explicit PlSizeTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                        ::std::size_t id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{nested_tag_type_},
          pltext(pltext_),
          id(::std::move(id_)) {
    }

    constexpr ~PlSizeTagContext() noexcept = default;
};

/**
 * @brief Context for parsing Markdown block quotes
 * @details Specialized context for handling Markdown block quote syntax (>)
 *          that starts with > character
 */
class MdBlockQuotesContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string pltext; ///< The text content of the block quote

    constexpr explicit MdBlockQuotesContext(::fast_io::u8string&& pltext_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_block_quotes},
          pltext(::std::move(pltext_)) {
    }

    constexpr ~MdBlockQuotesContext() noexcept = default;
};

/**
 * @brief Context for parsing Markdown links
 * @details Specialized context for handling Markdown link syntax [text](url)
 *          where both the link text and URL need to be parsed
 */
class MdLinkContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext; ///< The link text (inside [])
    ::fast_io::u8string link; ///< The URL (inside ())

    constexpr explicit MdLinkContext(::fast_io::u8string_view pltext_, ::fast_io::u8string&& link_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_link},
          pltext(::std::move(pltext_)),
          link(::std::move(link_)) {
    }

    constexpr ~MdLinkContext() noexcept = default;
};

/**
 * @brief Context for parsing Markdown lists (ordered and unordered)
 * @tparam ndebug Debug mode flag - controls assertion behavior
 * @details Specialized context for handling Markdown list syntax including
 *          both ordered lists (1. 2. 3.) and unordered lists (- * +)
 */
template<bool ndebug>
class MdListContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::pltxt2htm::details::MdListAst md_list_ast; ///< The parsed list structure
    ::pltxt2htm::details::MdListAst::iterator iter; ///< Iterator for processing list items

    constexpr explicit MdListContext(::pltxt2htm::NodeType node_type,
                                     ::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{node_type},
          md_list_ast(::std::move(md_list_ast_)),
          iter(this->md_list_ast.begin()) {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeType::md_ul || node_type == ::pltxt2htm::NodeType::md_ol,
                         u8"mismatch node type");
    }

    constexpr MdListContext(::pltxt2htm::details::MdListContext<ndebug>&& other) noexcept = default;

    constexpr ~MdListContext() noexcept = default;
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
