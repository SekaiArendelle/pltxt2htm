/**
 * @file markdown_node.hh
 * @brief Markdown-specific AST node definitions for pltxt2htm
 * @details Defines AST nodes for Markdown syntax elements including headers,
 *          emphasis, lists, code blocks, and other Markdown constructs
 * @note These nodes represent standard Markdown elements that can appear
 *       in Physics-Lab text format, following the CommonMark specification
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Represents markdown atx `#` header.
 * @details This node represents a level 1 Markdown heading in the AST
 */
class MdAtxH1 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH1() noexcept = delete;

    constexpr MdAtxH1(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h1, ::std::move(subast)} {
    }

    constexpr MdAtxH1(::pltxt2htm::MdAtxH1 const&) noexcept = delete;

    constexpr MdAtxH1(::pltxt2htm::MdAtxH1&&) noexcept = default;

    constexpr ~MdAtxH1() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH1& operator=(::pltxt2htm::MdAtxH1 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH1& operator=(::pltxt2htm::MdAtxH1&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `##` header.
 * @details This node represents a level 2 Markdown heading in the AST
 */
class MdAtxH2 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH2() noexcept = delete;

    constexpr MdAtxH2(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h2, ::std::move(subast)} {
    }

    constexpr MdAtxH2(::pltxt2htm::MdAtxH2 const&) noexcept = delete;

    constexpr MdAtxH2(::pltxt2htm::MdAtxH2&&) noexcept = default;

    constexpr ~MdAtxH2() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH2& operator=(::pltxt2htm::MdAtxH2 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH2& operator=(::pltxt2htm::MdAtxH2&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `###` header.
 * @details This node represents a level 3 Markdown heading in the AST
 */
class MdAtxH3 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH3() noexcept = delete;

    constexpr MdAtxH3(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h3, ::std::move(subast)} {
    }

    constexpr MdAtxH3(::pltxt2htm::MdAtxH3 const&) noexcept = delete;

    constexpr MdAtxH3(::pltxt2htm::MdAtxH3&&) noexcept = default;

    constexpr ~MdAtxH3() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH3& operator=(::pltxt2htm::MdAtxH3 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH3& operator=(::pltxt2htm::MdAtxH3&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `####` header.
 * @details This node represents a level 4 Markdown heading in the AST
 */
class MdAtxH4 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH4() noexcept = delete;

    constexpr MdAtxH4(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h4, ::std::move(subast)} {
    }

    constexpr MdAtxH4(::pltxt2htm::MdAtxH4 const&) noexcept = delete;

    constexpr MdAtxH4(::pltxt2htm::MdAtxH4&&) noexcept = default;

    constexpr ~MdAtxH4() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH4& operator=(::pltxt2htm::MdAtxH4 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH4& operator=(::pltxt2htm::MdAtxH4&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `#####` header.
 * @details This node represents a level 5 Markdown heading in the AST
 */
class MdAtxH5 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH5() noexcept = delete;

    constexpr MdAtxH5(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h5, ::std::move(subast)} {
    }

    constexpr MdAtxH5(::pltxt2htm::MdAtxH5 const&) noexcept = delete;

    constexpr MdAtxH5(::pltxt2htm::MdAtxH5&&) noexcept = default;

    constexpr ~MdAtxH5() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH5& operator=(::pltxt2htm::MdAtxH5 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH5& operator=(::pltxt2htm::MdAtxH5&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `######` header.
 * @details This node represents a level 6 Markdown heading in the AST
 */
class MdAtxH6 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH6() noexcept = delete;

    constexpr MdAtxH6(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h6, ::std::move(subast)} {
    }

    constexpr MdAtxH6(::pltxt2htm::MdAtxH6 const&) noexcept = delete;

    constexpr MdAtxH6(::pltxt2htm::MdAtxH6&&) noexcept = default;

    constexpr ~MdAtxH6() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH6& operator=(::pltxt2htm::MdAtxH6 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH6& operator=(::pltxt2htm::MdAtxH6&&) noexcept = default;
};

/**
 * @brief Represents \.
 */
class MdEscapeBackslash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeBackslash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_backslash) {
    }

    constexpr MdEscapeBackslash(::pltxt2htm::MdEscapeBackslash const&) noexcept = delete;

    constexpr MdEscapeBackslash(::pltxt2htm::MdEscapeBackslash&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeBackslash& operator=(::pltxt2htm::MdEscapeBackslash const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeBackslash& operator=(::pltxt2htm::MdEscapeBackslash&&) noexcept = default;
};

/**
 * @brief Represents !.
 */
class MdEscapeExclamation : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeExclamation() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_exclamation) {
    }

    constexpr MdEscapeExclamation(::pltxt2htm::MdEscapeExclamation const&) noexcept = delete;

    constexpr MdEscapeExclamation(::pltxt2htm::MdEscapeExclamation&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeExclamation& operator=(::pltxt2htm::MdEscapeExclamation const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeExclamation& operator=(::pltxt2htm::MdEscapeExclamation&&) noexcept = default;
};

/**
 * @brief Represents ".
 */
class MdEscapeDoubleQuote : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeDoubleQuote() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_double_quote) {
    }

    constexpr MdEscapeDoubleQuote(::pltxt2htm::MdEscapeDoubleQuote const&) noexcept = delete;

    constexpr MdEscapeDoubleQuote(::pltxt2htm::MdEscapeDoubleQuote&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeDoubleQuote& operator=(::pltxt2htm::MdEscapeDoubleQuote const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeDoubleQuote& operator=(::pltxt2htm::MdEscapeDoubleQuote&&) noexcept = default;
};

/**
 * @brief Represents #.
 */
class MdEscapeHash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeHash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_hash) {
    }

    constexpr MdEscapeHash(::pltxt2htm::MdEscapeHash const&) noexcept = delete;

    constexpr MdEscapeHash(::pltxt2htm::MdEscapeHash&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeHash& operator=(::pltxt2htm::MdEscapeHash const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeHash& operator=(::pltxt2htm::MdEscapeHash&&) noexcept = default;
};

/**
 * @brief Represents $.
 */
class MdEscapeDollar : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeDollar() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_dollar) {
    }

    constexpr MdEscapeDollar(::pltxt2htm::MdEscapeDollar const&) noexcept = delete;

    constexpr MdEscapeDollar(::pltxt2htm::MdEscapeDollar&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeDollar& operator=(::pltxt2htm::MdEscapeDollar const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeDollar& operator=(::pltxt2htm::MdEscapeDollar&&) noexcept = default;
};

/**
 * @brief Represents %.
 */
class MdEscapePercent : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapePercent() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_percent) {
    }

    constexpr MdEscapePercent(::pltxt2htm::MdEscapePercent const&) noexcept = delete;

    constexpr MdEscapePercent(::pltxt2htm::MdEscapePercent&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapePercent& operator=(::pltxt2htm::MdEscapePercent const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapePercent& operator=(::pltxt2htm::MdEscapePercent&&) noexcept = default;
};

/**
 * @brief Represents &.
 */
class MdEscapeAmpersand : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeAmpersand() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_ampersand) {
    }

    constexpr MdEscapeAmpersand(::pltxt2htm::MdEscapeAmpersand const&) noexcept = delete;

    constexpr MdEscapeAmpersand(::pltxt2htm::MdEscapeAmpersand&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeAmpersand& operator=(::pltxt2htm::MdEscapeAmpersand const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeAmpersand& operator=(::pltxt2htm::MdEscapeAmpersand&&) noexcept = default;
};

/**
 * @brief Represents '.
 */
class MdEscapeSingleQuote : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeSingleQuote() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_single_quote) {
    }

    constexpr MdEscapeSingleQuote(::pltxt2htm::MdEscapeSingleQuote const&) noexcept = delete;

    constexpr MdEscapeSingleQuote(::pltxt2htm::MdEscapeSingleQuote&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeSingleQuote& operator=(::pltxt2htm::MdEscapeSingleQuote const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeSingleQuote& operator=(::pltxt2htm::MdEscapeSingleQuote&&) noexcept = default;
};

/**
 * @brief Represents (.
 */
class MdEscapeLeftParen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeLeftParen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_paren) {
    }

    constexpr MdEscapeLeftParen(::pltxt2htm::MdEscapeLeftParen const&) noexcept = delete;

    constexpr MdEscapeLeftParen(::pltxt2htm::MdEscapeLeftParen&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeLeftParen& operator=(::pltxt2htm::MdEscapeLeftParen const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeLeftParen& operator=(::pltxt2htm::MdEscapeLeftParen&&) noexcept = default;
};

/**
 * @brief Represents ).
 */
class MdEscapeRightParen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeRightParen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_paren) {
    }

    constexpr MdEscapeRightParen(::pltxt2htm::MdEscapeRightParen const&) noexcept = delete;

    constexpr MdEscapeRightParen(::pltxt2htm::MdEscapeRightParen&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeRightParen& operator=(::pltxt2htm::MdEscapeRightParen const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeRightParen& operator=(::pltxt2htm::MdEscapeRightParen&&) noexcept = default;
};

/**
 * @brief Represents *.
 */
class MdEscapeAsterisk : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeAsterisk() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_asterisk) {
    }

    constexpr MdEscapeAsterisk(::pltxt2htm::MdEscapeAsterisk const&) noexcept = delete;

    constexpr MdEscapeAsterisk(::pltxt2htm::MdEscapeAsterisk&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeAsterisk& operator=(::pltxt2htm::MdEscapeAsterisk const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeAsterisk& operator=(::pltxt2htm::MdEscapeAsterisk&&) noexcept = default;
};

/**
 * @brief Represents +.
 */
class MdEscapePlus : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapePlus() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_plus) {
    }

    constexpr MdEscapePlus(::pltxt2htm::MdEscapePlus const&) noexcept = delete;

    constexpr MdEscapePlus(::pltxt2htm::MdEscapePlus&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapePlus& operator=(::pltxt2htm::MdEscapePlus const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapePlus& operator=(::pltxt2htm::MdEscapePlus&&) noexcept = default;
};

/**
 * @brief Represents ,.
 */
class MdEscapeComma : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeComma() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_comma) {
    }

    constexpr MdEscapeComma(::pltxt2htm::MdEscapeComma const&) noexcept = delete;

    constexpr MdEscapeComma(::pltxt2htm::MdEscapeComma&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeComma& operator=(::pltxt2htm::MdEscapeComma const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeComma& operator=(::pltxt2htm::MdEscapeComma&&) noexcept = default;
};

/**
 * @brief Represents -.
 */
class MdEscapeHyphen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeHyphen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_hyphen) {
    }

    constexpr MdEscapeHyphen(::pltxt2htm::MdEscapeHyphen const&) noexcept = delete;

    constexpr MdEscapeHyphen(::pltxt2htm::MdEscapeHyphen&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeHyphen& operator=(::pltxt2htm::MdEscapeHyphen const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeHyphen& operator=(::pltxt2htm::MdEscapeHyphen&&) noexcept = default;
};

/**
 * @brief Represents ..
 */
class MdEscapeDot : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeDot() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_dot) {
    }

    constexpr MdEscapeDot(::pltxt2htm::MdEscapeDot const&) noexcept = delete;

    constexpr MdEscapeDot(::pltxt2htm::MdEscapeDot&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeDot& operator=(::pltxt2htm::MdEscapeDot const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeDot& operator=(::pltxt2htm::MdEscapeDot&&) noexcept = default;
};

/**
 * @brief Represents /.
 */
class MdEscapeSlash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeSlash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_slash) {
    }

    constexpr MdEscapeSlash(::pltxt2htm::MdEscapeSlash const&) noexcept = delete;

    constexpr MdEscapeSlash(::pltxt2htm::MdEscapeSlash&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeSlash& operator=(::pltxt2htm::MdEscapeSlash const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeSlash& operator=(::pltxt2htm::MdEscapeSlash&&) noexcept = default;
};

/**
 * @brief Represents :.
 */
class MdEscapeColon : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeColon() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_colon) {
    }

    constexpr MdEscapeColon(::pltxt2htm::MdEscapeColon const&) noexcept = delete;

    constexpr MdEscapeColon(::pltxt2htm::MdEscapeColon&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeColon& operator=(::pltxt2htm::MdEscapeColon const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeColon& operator=(::pltxt2htm::MdEscapeColon&&) noexcept = default;
};

/**
 * @brief Represents ;.
 */
class MdEscapeSemicolon : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeSemicolon() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_semicolon) {
    }

    constexpr MdEscapeSemicolon(::pltxt2htm::MdEscapeSemicolon const&) noexcept = delete;

    constexpr MdEscapeSemicolon(::pltxt2htm::MdEscapeSemicolon&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeSemicolon& operator=(::pltxt2htm::MdEscapeSemicolon const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeSemicolon& operator=(::pltxt2htm::MdEscapeSemicolon&&) noexcept = default;
};

/**
 * @brief Represents <.
 */
class MdEscapeLessThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeLessThan() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_less_than) {
    }

    constexpr MdEscapeLessThan(::pltxt2htm::MdEscapeLessThan const&) noexcept = delete;

    constexpr MdEscapeLessThan(::pltxt2htm::MdEscapeLessThan&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeLessThan& operator=(::pltxt2htm::MdEscapeLessThan const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeLessThan& operator=(::pltxt2htm::MdEscapeLessThan&&) noexcept = default;
};

/**
 * @brief Represents =.
 */
class MdEscapeEquals : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeEquals() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_equals) {
    }

    constexpr MdEscapeEquals(::pltxt2htm::MdEscapeEquals const&) noexcept = delete;

    constexpr MdEscapeEquals(::pltxt2htm::MdEscapeEquals&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeEquals& operator=(::pltxt2htm::MdEscapeEquals const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeEquals& operator=(::pltxt2htm::MdEscapeEquals&&) noexcept = default;
};

/**
 * @brief Represents >.
 */
class MdEscapeGreaterThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeGreaterThan() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_greater_than) {
    }

    constexpr MdEscapeGreaterThan(::pltxt2htm::MdEscapeGreaterThan const&) noexcept = delete;

    constexpr MdEscapeGreaterThan(::pltxt2htm::MdEscapeGreaterThan&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeGreaterThan& operator=(::pltxt2htm::MdEscapeGreaterThan const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeGreaterThan& operator=(::pltxt2htm::MdEscapeGreaterThan&&) noexcept = default;
};

/**
 * @brief Represents ?.
 */
class MdEscapeQuestion : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeQuestion() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_question) {
    }

    constexpr MdEscapeQuestion(::pltxt2htm::MdEscapeQuestion const&) noexcept = delete;

    constexpr MdEscapeQuestion(::pltxt2htm::MdEscapeQuestion&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeQuestion& operator=(::pltxt2htm::MdEscapeQuestion const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeQuestion& operator=(::pltxt2htm::MdEscapeQuestion&&) noexcept = default;
};

/**
 * @brief Represents @.
 */
class MdEscapeAt : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeAt() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_at) {
    }

    constexpr MdEscapeAt(::pltxt2htm::MdEscapeAt const&) noexcept = delete;

    constexpr MdEscapeAt(::pltxt2htm::MdEscapeAt&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeAt& operator=(::pltxt2htm::MdEscapeAt const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeAt& operator=(::pltxt2htm::MdEscapeAt&&) noexcept = default;
};

/**
 * @brief Represents [.
 */
class MdEscapeLeftBracket : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeLeftBracket() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_bracket) {
    }

    constexpr MdEscapeLeftBracket(::pltxt2htm::MdEscapeLeftBracket const&) noexcept = delete;

    constexpr MdEscapeLeftBracket(::pltxt2htm::MdEscapeLeftBracket&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeLeftBracket& operator=(::pltxt2htm::MdEscapeLeftBracket const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeLeftBracket& operator=(::pltxt2htm::MdEscapeLeftBracket&&) noexcept = default;
};

/**
 * @brief Represents ].
 */
class MdEscapeRightBracket : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeRightBracket() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_bracket) {
    }

    constexpr MdEscapeRightBracket(::pltxt2htm::MdEscapeRightBracket const&) noexcept = delete;

    constexpr MdEscapeRightBracket(::pltxt2htm::MdEscapeRightBracket&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeRightBracket& operator=(::pltxt2htm::MdEscapeRightBracket const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeRightBracket& operator=(::pltxt2htm::MdEscapeRightBracket&&) noexcept = default;
};

/**
 * @brief Represents ^.
 */
class MdEscapeCaret : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeCaret() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_caret) {
    }

    constexpr MdEscapeCaret(::pltxt2htm::MdEscapeCaret const&) noexcept = delete;

    constexpr MdEscapeCaret(::pltxt2htm::MdEscapeCaret&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeCaret& operator=(::pltxt2htm::MdEscapeCaret const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeCaret& operator=(::pltxt2htm::MdEscapeCaret&&) noexcept = default;
};

/**
 * @brief Represents _.
 */
class MdEscapeUnderscore : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeUnderscore() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_underscore) {
    }

    constexpr MdEscapeUnderscore(::pltxt2htm::MdEscapeUnderscore const&) noexcept = delete;

    constexpr MdEscapeUnderscore(::pltxt2htm::MdEscapeUnderscore&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeUnderscore& operator=(::pltxt2htm::MdEscapeUnderscore const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeUnderscore& operator=(::pltxt2htm::MdEscapeUnderscore&&) noexcept = default;
};

/**
 * @brief Represents a backtick character.
 */
class MdEscapeBacktick : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeBacktick() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_backtick) {
    }

    constexpr MdEscapeBacktick(::pltxt2htm::MdEscapeBacktick const&) noexcept = delete;

    constexpr MdEscapeBacktick(::pltxt2htm::MdEscapeBacktick&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeBacktick& operator=(::pltxt2htm::MdEscapeBacktick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeBacktick& operator=(::pltxt2htm::MdEscapeBacktick&&) noexcept = default;
};

/**
 * @brief Represents {.
 */
class MdEscapeLeftBrace : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeLeftBrace() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_brace) {
    }

    constexpr MdEscapeLeftBrace(::pltxt2htm::MdEscapeLeftBrace const&) noexcept = delete;

    constexpr MdEscapeLeftBrace(::pltxt2htm::MdEscapeLeftBrace&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeLeftBrace& operator=(::pltxt2htm::MdEscapeLeftBrace const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeLeftBrace& operator=(::pltxt2htm::MdEscapeLeftBrace&&) noexcept = default;
};

/**
 * @brief Represents |.
 */
class MdEscapePipe : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapePipe() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_pipe) {
    }

    constexpr MdEscapePipe(::pltxt2htm::MdEscapePipe const&) noexcept = delete;

    constexpr MdEscapePipe(::pltxt2htm::MdEscapePipe&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapePipe& operator=(::pltxt2htm::MdEscapePipe const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapePipe& operator=(::pltxt2htm::MdEscapePipe&&) noexcept = default;
};

/**
 * @brief Represents }.
 */
class MdEscapeRightBrace : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeRightBrace() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_brace) {
    }

    constexpr MdEscapeRightBrace(::pltxt2htm::MdEscapeRightBrace const&) noexcept = delete;

    constexpr MdEscapeRightBrace(::pltxt2htm::MdEscapeRightBrace&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeRightBrace& operator=(::pltxt2htm::MdEscapeRightBrace const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeRightBrace& operator=(::pltxt2htm::MdEscapeRightBrace&&) noexcept = default;
};

/**
 * @brief Represents ~.
 */
class MdEscapeTilde : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdEscapeTilde() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_tilde) {
    }

    constexpr MdEscapeTilde(::pltxt2htm::MdEscapeTilde const&) noexcept = delete;

    constexpr MdEscapeTilde(::pltxt2htm::MdEscapeTilde&&) noexcept = default;

    constexpr ::pltxt2htm::MdEscapeTilde& operator=(::pltxt2htm::MdEscapeTilde const&) noexcept = delete;

    constexpr ::pltxt2htm::MdEscapeTilde& operator=(::pltxt2htm::MdEscapeTilde&&) noexcept = default;
};

/**
 * @brief Represents markdown horizontal rule (`---`, `***`, `___`).
 * @details https://spec.commonmark.org/0.31.2/#thematic-breaks
 */
class MdHr : public ::pltxt2htm::PlTxtNode {
public:
    constexpr MdHr() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_hr) {
    }

    constexpr MdHr(::pltxt2htm::MdHr const&) noexcept = delete;

    constexpr MdHr(::pltxt2htm::MdHr&&) noexcept = default;

    constexpr ::pltxt2htm::MdHr& operator=(::pltxt2htm::MdHr const&) noexcept = delete;

    constexpr ::pltxt2htm::MdHr& operator=(::pltxt2htm::MdHr&&) noexcept = default;
};

/**
 * @brief Represents markdown code fence (```).
 * @details https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
class MdCodeFenceBacktick : public ::pltxt2htm::details::PairedTagBase {
    ::exception::optional<::fast_io::u8string> lang;

public:
    constexpr MdCodeFenceBacktick(::pltxt2htm::Ast&& ast, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_code_fence_backtick, ::std::move(ast)},
          lang(::std::move(lang_)) {
    }

    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick const&) noexcept = delete;

    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeFenceBacktick& operator=(::pltxt2htm::MdCodeFenceBacktick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeFenceBacktick& operator=(::pltxt2htm::MdCodeFenceBacktick&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_language(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

/**
 * @brief Represents markdown code fence (~~~).
 * @details https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
class MdCodeFenceTilde : public ::pltxt2htm::details::PairedTagBase {
    ::exception::optional<::fast_io::u8string> lang;

public:
    constexpr MdCodeFenceTilde(::pltxt2htm::Ast&& ast, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_code_fence_tilde, ::std::move(ast)},
          lang(::std::move(lang_)) {
    }

    constexpr MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde const&) noexcept = delete;

    constexpr MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeFenceTilde& operator=(::pltxt2htm::MdCodeFenceTilde const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeFenceTilde& operator=(::pltxt2htm::MdCodeFenceTilde&&) noexcept = default;

    constexpr auto&& get_language(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

/**
 * @brief Represents markdown code span (`example`).
 * @details https://spec.commonmark.org/0.31.2/#code-spans
 */
class MdCodeSpan1Backtick : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdCodeSpan1Backtick() noexcept = delete;

    constexpr MdCodeSpan1Backtick(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_code_span_1_backtick, ::std::move(ast)} {
    }

    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick const&) noexcept = delete;

    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeSpan1Backtick& operator=(::pltxt2htm::MdCodeSpan1Backtick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeSpan1Backtick& operator=(::pltxt2htm::MdCodeSpan1Backtick&&) noexcept = default;
};

/**
 * @brief Represents markdown code span (``example``).
 * @details https://spec.commonmark.org/0.31.2/#code-spans
 */
class MdCodeSpan2Backtick : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdCodeSpan2Backtick() noexcept = delete;

    constexpr MdCodeSpan2Backtick(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_code_span_2_backtick, ::std::move(ast)} {
    }

    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick const&) noexcept = delete;

    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeSpan2Backtick& operator=(::pltxt2htm::MdCodeSpan2Backtick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeSpan2Backtick& operator=(::pltxt2htm::MdCodeSpan2Backtick&&) noexcept = default;
};

/**
 * @brief Represents markdown code span (``example``).
 * @details https://spec.commonmark.org/0.31.2/#code-spans
 */
class MdCodeSpan3Backtick : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdCodeSpan3Backtick() noexcept = delete;

    constexpr MdCodeSpan3Backtick(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_code_span_3_backtick, ::std::move(ast)} {
    }

    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept = delete;

    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeSpan3Backtick& operator=(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeSpan3Backtick& operator=(::pltxt2htm::MdCodeSpan3Backtick&&) noexcept = default;
};

/**
 * @brief Represents markdown code span (*example*).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdSingleEmphasisAsterisk : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdSingleEmphasisAsterisk() noexcept = delete;

    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_single_emphasis_asterisk, ::std::move(ast)} {
    }

    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk const&) noexcept = delete;

    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept = default;

    constexpr ::pltxt2htm::MdSingleEmphasisAsterisk& operator=(::pltxt2htm::MdSingleEmphasisAsterisk const&) noexcept =
        delete;

    constexpr ::pltxt2htm::MdSingleEmphasisAsterisk& operator=(::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept =
        default;
};

/**
 * @brief Represents markdown code span (**example**).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdDoubleEmphasisAsterisk : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdDoubleEmphasisAsterisk() noexcept = delete;

    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_double_emphasis_asterisk, ::std::move(ast)} {
    }

    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk const&) noexcept = delete;

    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept = default;

    constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk& operator=(::pltxt2htm::MdDoubleEmphasisAsterisk const&) noexcept =
        delete;

    constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk& operator=(::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept =
        default;
};

/**
 * @brief Represents markdown code span (***example***).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdTripleEmphasisAsterisk : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdTripleEmphasisAsterisk() noexcept = delete;

    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_triple_emphasis_asterisk, ::std::move(ast)} {
    }

    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk const&) noexcept = delete;

    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept = default;

    constexpr ::pltxt2htm::MdTripleEmphasisAsterisk& operator=(::pltxt2htm::MdTripleEmphasisAsterisk const&) noexcept =
        delete;

    constexpr ::pltxt2htm::MdTripleEmphasisAsterisk& operator=(::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept =
        default;
};

/**
 * @brief Represents markdown code span (_example_).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdSingleEmphasisUnderscore : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdSingleEmphasisUnderscore() noexcept = delete;

    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_single_emphasis_underscore, ::std::move(ast)} {
    }

    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore const&) noexcept = delete;

    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept = default;

    constexpr ::pltxt2htm::MdSingleEmphasisUnderscore& operator=(
        ::pltxt2htm::MdSingleEmphasisUnderscore const&) noexcept = delete;

    constexpr ::pltxt2htm::MdSingleEmphasisUnderscore& operator=(::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept =
        default;
};

/**
 * @brief Represents markdown code span (__example__).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdDoubleEmphasisUnderscore : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdDoubleEmphasisUnderscore() noexcept = delete;

    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_double_emphasis_underscore, ::std::move(ast)} {
    }

    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore const&) noexcept = delete;

    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept = default;

    constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore& operator=(
        ::pltxt2htm::MdDoubleEmphasisUnderscore const&) noexcept = delete;

    constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore& operator=(::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept =
        default;
};

/**
 * @brief Represents markdown code span (___example___).
 * @details https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 */
class MdTripleEmphasisUnderscore : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdTripleEmphasisUnderscore() noexcept = delete;

    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_triple_emphasis_underscore, ::std::move(ast)} {
    }

    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore const&) noexcept = delete;

    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept = default;

    constexpr ::pltxt2htm::MdTripleEmphasisUnderscore& operator=(
        ::pltxt2htm::MdTripleEmphasisUnderscore const&) noexcept = delete;

    constexpr ::pltxt2htm::MdTripleEmphasisUnderscore& operator=(::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept =
        default;
};

class MdDel : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdDel() noexcept = delete;

    constexpr MdDel(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_del, ::std::move(ast)} {
    }

    constexpr MdDel(::pltxt2htm::MdDel const&) noexcept = delete;

    constexpr MdDel(::pltxt2htm::MdDel&&) noexcept = default;

    constexpr ::pltxt2htm::MdDel& operator=(::pltxt2htm::MdDel const&) noexcept = delete;

    constexpr ::pltxt2htm::MdDel& operator=(::pltxt2htm::MdDel&&) noexcept = default;
};

class MdLink : public ::pltxt2htm::details::PairedTagBase {
public:
    // Can not be stored in string_view, otherwise tests/0048.pltext_maybe_destructed.cc will fail
    ::pltxt2htm::Url url_;

    constexpr MdLink(::pltxt2htm::Ast&& subast, ::pltxt2htm::Url&& url) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_link, ::std::move(subast)},
          url_(::std::move(url)) {
    }

    constexpr MdLink(::pltxt2htm::MdLink const&) noexcept = delete;

    constexpr MdLink(::pltxt2htm::MdLink&&) noexcept = default;

    constexpr ::pltxt2htm::MdLink& operator=(::pltxt2htm::MdLink const&) noexcept = delete;

    constexpr ::pltxt2htm::MdLink& operator=(::pltxt2htm::MdLink&&) noexcept = default;
};

/**
 * @brief Represents a Markdown image node (`![alt](url)`).
 * @details This node stores both the image destination URL and the inline AST
 *          used as the image alternative text (`alt` part in Markdown syntax).
 * @note The `subast` argument should only contain
 *       NodeType::(u8char + invalid_u8char + md_escape_* + Space + Ampersand
 *       + SingleQuotationMark + DoubleQuotationMark + LessThan + GreaterThan
 *       + Tab), matching the parser constraints for image alt text.
 */
class MdImage : public ::pltxt2htm::details::PairedTagBase {
public:
    /// Image destination URL in Markdown syntax.
    ::pltxt2htm::Url url_;

    /**
     * @brief Constructs a Markdown image node.
     * @param subast Inline AST used as image alternative text.
     * @param url Image destination URL.
     */
    constexpr MdImage(::pltxt2htm::Ast&& subast, ::pltxt2htm::Url&& url) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_image, ::std::move(subast)},
          url_(::std::move(url)) {
    }

    constexpr MdImage(::pltxt2htm::MdImage const&) noexcept = delete;

    constexpr MdImage(::pltxt2htm::MdImage&&) noexcept = default;

    constexpr ::pltxt2htm::MdImage& operator=(::pltxt2htm::MdImage const&) noexcept = delete;

    constexpr ::pltxt2htm::MdImage& operator=(::pltxt2htm::MdImage&&) noexcept = default;
};

class MdBlockQuotes : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdBlockQuotes() noexcept = delete;

    constexpr MdBlockQuotes(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_block_quotes, ::std::move(ast)} {
    }

    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes const&) noexcept = delete;

    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes&&) noexcept = default;

    constexpr ::pltxt2htm::MdBlockQuotes& operator=(::pltxt2htm::MdBlockQuotes const&) noexcept = delete;

    constexpr ::pltxt2htm::MdBlockQuotes& operator=(::pltxt2htm::MdBlockQuotes&&) noexcept = default;
};

class MdUl : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdUl() noexcept = delete;

    constexpr MdUl(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_ul, ::std::move(ast)} {
    }

    constexpr MdUl(::pltxt2htm::MdUl const&) noexcept = delete;

    constexpr MdUl(::pltxt2htm::MdUl&&) noexcept = default;

    constexpr ::pltxt2htm::MdUl& operator=(::pltxt2htm::MdUl const&) noexcept = delete;

    constexpr ::pltxt2htm::MdUl& operator=(::pltxt2htm::MdUl&&) noexcept = default;
};

class MdOl : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdOl() noexcept = delete;

    constexpr MdOl(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_ol, ::std::move(ast)} {
    }

    constexpr MdOl(::pltxt2htm::MdOl const&) noexcept = delete;

    constexpr MdOl(::pltxt2htm::MdOl&&) noexcept = default;

    constexpr ::pltxt2htm::MdOl& operator=(::pltxt2htm::MdOl const&) noexcept = delete;

    constexpr ::pltxt2htm::MdOl& operator=(::pltxt2htm::MdOl&&) noexcept = default;
};

class MdLi : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdLi() noexcept = delete;

    constexpr MdLi(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_li, ::std::move(ast)} {
    }

    constexpr MdLi(::pltxt2htm::MdLi const&) noexcept = delete;

    constexpr MdLi(::pltxt2htm::MdLi&&) noexcept = default;

    constexpr ::pltxt2htm::MdLi& operator=(::pltxt2htm::MdLi const&) noexcept = delete;

    constexpr ::pltxt2htm::MdLi& operator=(::pltxt2htm::MdLi&&) noexcept = default;
};

/**
 * @brief Represents inline LaTeX math ($...$).
 */
class MdLatexInline : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdLatexInline() noexcept = delete;

    constexpr MdLatexInline(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_latex_inline, ::std::move(ast)} {
    }

    constexpr MdLatexInline(::pltxt2htm::MdLatexInline const&) noexcept = delete;

    constexpr MdLatexInline(::pltxt2htm::MdLatexInline&&) noexcept = default;

    constexpr ::pltxt2htm::MdLatexInline& operator=(::pltxt2htm::MdLatexInline const&) noexcept = delete;

    constexpr ::pltxt2htm::MdLatexInline& operator=(::pltxt2htm::MdLatexInline&&) noexcept = default;
};

/**
 * @brief Represents block/display LaTeX math ($$...$$).
 */
class MdLatexBlock : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdLatexBlock() noexcept = delete;

    constexpr MdLatexBlock(::pltxt2htm::Ast&& ast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_latex_block, ::std::move(ast)} {
    }

    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock const&) noexcept = delete;

    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock&&) noexcept = default;

    constexpr ::pltxt2htm::MdLatexBlock& operator=(::pltxt2htm::MdLatexBlock const&) noexcept = delete;

    constexpr ::pltxt2htm::MdLatexBlock& operator=(::pltxt2htm::MdLatexBlock&&) noexcept = default;
};

} // namespace pltxt2htm
