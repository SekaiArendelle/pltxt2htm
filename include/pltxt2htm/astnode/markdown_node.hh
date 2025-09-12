#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "basic.hh"

namespace pltxt2htm {

/**
 * @brief Represents markdown atx `#` header.
 */
class MdAtxH1 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH1() noexcept = delete;

    constexpr MdAtxH1(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h1, ::std::move(subast)} {
    }

    constexpr MdAtxH1(::pltxt2htm::MdAtxH1 const&) noexcept = default;

    constexpr MdAtxH1(::pltxt2htm::MdAtxH1&&) noexcept = default;

    constexpr ~MdAtxH1() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH1& operator=(::pltxt2htm::MdAtxH1 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH1& operator=(::pltxt2htm::MdAtxH1&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `##` header.
 */
class MdAtxH2 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH2() noexcept = delete;

    constexpr MdAtxH2(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h2, ::std::move(subast)} {
    }

    constexpr MdAtxH2(::pltxt2htm::MdAtxH2 const&) noexcept = default;

    constexpr MdAtxH2(::pltxt2htm::MdAtxH2&&) noexcept = default;

    constexpr ~MdAtxH2() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH2& operator=(::pltxt2htm::MdAtxH2 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH2& operator=(::pltxt2htm::MdAtxH2&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `###` header.
 */
class MdAtxH3 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH3() noexcept = delete;

    constexpr MdAtxH3(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h3, ::std::move(subast)} {
    }

    constexpr MdAtxH3(::pltxt2htm::MdAtxH3 const&) noexcept = default;

    constexpr MdAtxH3(::pltxt2htm::MdAtxH3&&) noexcept = default;

    constexpr ~MdAtxH3() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH3& operator=(::pltxt2htm::MdAtxH3 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH3& operator=(::pltxt2htm::MdAtxH3&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `####` header.
 */
class MdAtxH4 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH4() noexcept = delete;

    constexpr MdAtxH4(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h4, ::std::move(subast)} {
    }

    constexpr MdAtxH4(::pltxt2htm::MdAtxH4 const&) noexcept = default;

    constexpr MdAtxH4(::pltxt2htm::MdAtxH4&&) noexcept = default;

    constexpr ~MdAtxH4() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH4& operator=(::pltxt2htm::MdAtxH4 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH4& operator=(::pltxt2htm::MdAtxH4&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `#####` header.
 */
class MdAtxH5 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH5() noexcept = delete;

    constexpr MdAtxH5(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h5, ::std::move(subast)} {
    }

    constexpr MdAtxH5(::pltxt2htm::MdAtxH5 const&) noexcept = default;

    constexpr MdAtxH5(::pltxt2htm::MdAtxH5&&) noexcept = default;

    constexpr ~MdAtxH5() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH5& operator=(::pltxt2htm::MdAtxH5 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH5& operator=(::pltxt2htm::MdAtxH5&&) noexcept = default;
};

/**
 * @brief Represents markdown atx `######` header.
 */
class MdAtxH6 : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr MdAtxH6() noexcept = delete;

    constexpr MdAtxH6(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase{::pltxt2htm::NodeType::md_atx_h6, ::std::move(subast)} {
    }

    constexpr MdAtxH6(::pltxt2htm::MdAtxH6 const&) noexcept = default;

    constexpr MdAtxH6(::pltxt2htm::MdAtxH6&&) noexcept = default;

    constexpr ~MdAtxH6() noexcept = default;

    constexpr ::pltxt2htm::MdAtxH6& operator=(::pltxt2htm::MdAtxH6 const&) noexcept = delete;

    constexpr ::pltxt2htm::MdAtxH6& operator=(::pltxt2htm::MdAtxH6&&) noexcept = default;
};

/**
 * @brief Represents \.
 */
class EscapeBackslash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeBackslash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_backslash) {
    }

    constexpr EscapeBackslash(::pltxt2htm::EscapeBackslash const&) noexcept = default;

    constexpr EscapeBackslash(::pltxt2htm::EscapeBackslash&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeBackslash& operator=(::pltxt2htm::EscapeBackslash const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeBackslash& operator=(::pltxt2htm::EscapeBackslash&&) noexcept = default;
};

/**
 * @brief Represents !.
 */
class EscapeExclamation : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeExclamation() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_exclamation) {
    }

    constexpr EscapeExclamation(::pltxt2htm::EscapeExclamation const&) noexcept = default;

    constexpr EscapeExclamation(::pltxt2htm::EscapeExclamation&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeExclamation& operator=(::pltxt2htm::EscapeExclamation const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeExclamation& operator=(::pltxt2htm::EscapeExclamation&&) noexcept = default;
};

/**
 * @brief Represents ".
 */
class EscapeDoubleQuote : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeDoubleQuote() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_double_quote) {
    }

    constexpr EscapeDoubleQuote(::pltxt2htm::EscapeDoubleQuote const&) noexcept = default;

    constexpr EscapeDoubleQuote(::pltxt2htm::EscapeDoubleQuote&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDoubleQuote& operator=(::pltxt2htm::EscapeDoubleQuote const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDoubleQuote& operator=(::pltxt2htm::EscapeDoubleQuote&&) noexcept = default;
};

/**
 * @brief Represents #.
 */
class EscapeHash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeHash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_hash) {
    }

    constexpr EscapeHash(::pltxt2htm::EscapeHash const&) noexcept = default;

    constexpr EscapeHash(::pltxt2htm::EscapeHash&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeHash& operator=(::pltxt2htm::EscapeHash const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeHash& operator=(::pltxt2htm::EscapeHash&&) noexcept = default;
};

/**
 * @brief Represents $.
 */
class EscapeDollar : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeDollar() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_dollar) {
    }

    constexpr EscapeDollar(::pltxt2htm::EscapeDollar const&) noexcept = default;

    constexpr EscapeDollar(::pltxt2htm::EscapeDollar&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDollar& operator=(::pltxt2htm::EscapeDollar const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDollar& operator=(::pltxt2htm::EscapeDollar&&) noexcept = default;
};

/**
 * @brief Represents %.
 */
class EscapePercent : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapePercent() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_percent) {
    }

    constexpr EscapePercent(::pltxt2htm::EscapePercent const&) noexcept = default;

    constexpr EscapePercent(::pltxt2htm::EscapePercent&&) noexcept = default;

    constexpr ::pltxt2htm::EscapePercent& operator=(::pltxt2htm::EscapePercent const&) noexcept = default;

    constexpr ::pltxt2htm::EscapePercent& operator=(::pltxt2htm::EscapePercent&&) noexcept = default;
};

/**
 * @brief Represents &.
 */
class EscapeAmpersand : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeAmpersand() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_ampersand) {
    }

    constexpr EscapeAmpersand(::pltxt2htm::EscapeAmpersand const&) noexcept = default;

    constexpr EscapeAmpersand(::pltxt2htm::EscapeAmpersand&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAmpersand& operator=(::pltxt2htm::EscapeAmpersand const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAmpersand& operator=(::pltxt2htm::EscapeAmpersand&&) noexcept = default;
};

/**
 * @brief Represents '.
 */
class EscapeSingleQuote : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeSingleQuote() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_single_quote) {
    }

    constexpr EscapeSingleQuote(::pltxt2htm::EscapeSingleQuote const&) noexcept = default;

    constexpr EscapeSingleQuote(::pltxt2htm::EscapeSingleQuote&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSingleQuote& operator=(::pltxt2htm::EscapeSingleQuote const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSingleQuote& operator=(::pltxt2htm::EscapeSingleQuote&&) noexcept = default;
};

/**
 * @brief Represents (.
 */
class EscapeLeftParen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeLeftParen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_paren) {
    }

    constexpr EscapeLeftParen(::pltxt2htm::EscapeLeftParen const&) noexcept = default;

    constexpr EscapeLeftParen(::pltxt2htm::EscapeLeftParen&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftParen& operator=(::pltxt2htm::EscapeLeftParen const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftParen& operator=(::pltxt2htm::EscapeLeftParen&&) noexcept = default;
};

/**
 * @brief Represents ).
 */
class EscapeRightParen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeRightParen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_paren) {
    }

    constexpr EscapeRightParen(::pltxt2htm::EscapeRightParen const&) noexcept = default;

    constexpr EscapeRightParen(::pltxt2htm::EscapeRightParen&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightParen& operator=(::pltxt2htm::EscapeRightParen const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightParen& operator=(::pltxt2htm::EscapeRightParen&&) noexcept = default;
};

/**
 * @brief Represents *.
 */
class EscapeAsterisk : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeAsterisk() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_asterisk) {
    }

    constexpr EscapeAsterisk(::pltxt2htm::EscapeAsterisk const&) noexcept = default;

    constexpr EscapeAsterisk(::pltxt2htm::EscapeAsterisk&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAsterisk& operator=(::pltxt2htm::EscapeAsterisk const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAsterisk& operator=(::pltxt2htm::EscapeAsterisk&&) noexcept = default;
};

/**
 * @brief Represents +.
 */
class EscapePlus : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapePlus() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_plus) {
    }

    constexpr EscapePlus(::pltxt2htm::EscapePlus const&) noexcept = default;

    constexpr EscapePlus(::pltxt2htm::EscapePlus&&) noexcept = default;

    constexpr ::pltxt2htm::EscapePlus& operator=(::pltxt2htm::EscapePlus const&) noexcept = default;

    constexpr ::pltxt2htm::EscapePlus& operator=(::pltxt2htm::EscapePlus&&) noexcept = default;
};

/**
 * @brief Represents ,.
 */
class EscapeComma : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeComma() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_comma) {
    }

    constexpr EscapeComma(::pltxt2htm::EscapeComma const&) noexcept = default;

    constexpr EscapeComma(::pltxt2htm::EscapeComma&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeComma& operator=(::pltxt2htm::EscapeComma const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeComma& operator=(::pltxt2htm::EscapeComma&&) noexcept = default;
};

/**
 * @brief Represents -.
 */
class EscapeHyphen : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeHyphen() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_hyphen) {
    }

    constexpr EscapeHyphen(::pltxt2htm::EscapeHyphen const&) noexcept = default;

    constexpr EscapeHyphen(::pltxt2htm::EscapeHyphen&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeHyphen& operator=(::pltxt2htm::EscapeHyphen const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeHyphen& operator=(::pltxt2htm::EscapeHyphen&&) noexcept = default;
};

/**
 * @brief Represents ..
 */
class EscapeDot : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeDot() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_dot) {
    }

    constexpr EscapeDot(::pltxt2htm::EscapeDot const&) noexcept = default;

    constexpr EscapeDot(::pltxt2htm::EscapeDot&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDot& operator=(::pltxt2htm::EscapeDot const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeDot& operator=(::pltxt2htm::EscapeDot&&) noexcept = default;
};

/**
 * @brief Represents /.
 */
class EscapeSlash : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeSlash() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_slash) {
    }

    constexpr EscapeSlash(::pltxt2htm::EscapeSlash const&) noexcept = default;

    constexpr EscapeSlash(::pltxt2htm::EscapeSlash&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSlash& operator=(::pltxt2htm::EscapeSlash const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSlash& operator=(::pltxt2htm::EscapeSlash&&) noexcept = default;
};

/**
 * @brief Represents :.
 */
class EscapeColon : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeColon() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_colon) {
    }

    constexpr EscapeColon(::pltxt2htm::EscapeColon const&) noexcept = default;

    constexpr EscapeColon(::pltxt2htm::EscapeColon&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeColon& operator=(::pltxt2htm::EscapeColon const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeColon& operator=(::pltxt2htm::EscapeColon&&) noexcept = default;
};

/**
 * @brief Represents ;.
 */
class EscapeSemicolon : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeSemicolon() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_semicolon) {
    }

    constexpr EscapeSemicolon(::pltxt2htm::EscapeSemicolon const&) noexcept = default;

    constexpr EscapeSemicolon(::pltxt2htm::EscapeSemicolon&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSemicolon& operator=(::pltxt2htm::EscapeSemicolon const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeSemicolon& operator=(::pltxt2htm::EscapeSemicolon&&) noexcept = default;
};

/**
 * @brief Represents <.
 */
class EscapeLessThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeLessThan() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_less_than) {
    }

    constexpr EscapeLessThan(::pltxt2htm::EscapeLessThan const&) noexcept = default;

    constexpr EscapeLessThan(::pltxt2htm::EscapeLessThan&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLessThan& operator=(::pltxt2htm::EscapeLessThan const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLessThan& operator=(::pltxt2htm::EscapeLessThan&&) noexcept = default;
};

/**
 * @brief Represents =.
 */
class EscapeEquals : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeEquals() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_equals) {
    }

    constexpr EscapeEquals(::pltxt2htm::EscapeEquals const&) noexcept = default;

    constexpr EscapeEquals(::pltxt2htm::EscapeEquals&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeEquals& operator=(::pltxt2htm::EscapeEquals const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeEquals& operator=(::pltxt2htm::EscapeEquals&&) noexcept = default;
};

/**
 * @brief Represents >.
 */
class EscapeGreaterThan : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeGreaterThan() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_greater_than) {
    }

    constexpr EscapeGreaterThan(::pltxt2htm::EscapeGreaterThan const&) noexcept = default;

    constexpr EscapeGreaterThan(::pltxt2htm::EscapeGreaterThan&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeGreaterThan& operator=(::pltxt2htm::EscapeGreaterThan const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeGreaterThan& operator=(::pltxt2htm::EscapeGreaterThan&&) noexcept = default;
};

/**
 * @brief Represents ?.
 */
class EscapeQuestion : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeQuestion() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_question) {
    }

    constexpr EscapeQuestion(::pltxt2htm::EscapeQuestion const&) noexcept = default;

    constexpr EscapeQuestion(::pltxt2htm::EscapeQuestion&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeQuestion& operator=(::pltxt2htm::EscapeQuestion const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeQuestion& operator=(::pltxt2htm::EscapeQuestion&&) noexcept = default;
};

/**
 * @brief Represents @.
 */
class EscapeAt : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeAt() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_at) {
    }

    constexpr EscapeAt(::pltxt2htm::EscapeAt const&) noexcept = default;

    constexpr EscapeAt(::pltxt2htm::EscapeAt&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAt& operator=(::pltxt2htm::EscapeAt const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeAt& operator=(::pltxt2htm::EscapeAt&&) noexcept = default;
};

/**
 * @brief Represents [.
 */
class EscapeLeftBracket : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeLeftBracket() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_bracket) {
    }

    constexpr EscapeLeftBracket(::pltxt2htm::EscapeLeftBracket const&) noexcept = default;

    constexpr EscapeLeftBracket(::pltxt2htm::EscapeLeftBracket&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftBracket& operator=(::pltxt2htm::EscapeLeftBracket const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftBracket& operator=(::pltxt2htm::EscapeLeftBracket&&) noexcept = default;
};

/**
 * @brief Represents ].
 */
class EscapeRightBracket : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeRightBracket() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_bracket) {
    }

    constexpr EscapeRightBracket(::pltxt2htm::EscapeRightBracket const&) noexcept = default;

    constexpr EscapeRightBracket(::pltxt2htm::EscapeRightBracket&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightBracket& operator=(::pltxt2htm::EscapeRightBracket const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightBracket& operator=(::pltxt2htm::EscapeRightBracket&&) noexcept = default;
};

/**
 * @brief Represents ^.
 */
class EscapeCaret : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeCaret() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_caret) {
    }

    constexpr EscapeCaret(::pltxt2htm::EscapeCaret const&) noexcept = default;

    constexpr EscapeCaret(::pltxt2htm::EscapeCaret&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeCaret& operator=(::pltxt2htm::EscapeCaret const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeCaret& operator=(::pltxt2htm::EscapeCaret&&) noexcept = default;
};

/**
 * @brief Represents _.
 */
class EscapeUnderscore : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeUnderscore() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_underscore) {
    }

    constexpr EscapeUnderscore(::pltxt2htm::EscapeUnderscore const&) noexcept = default;

    constexpr EscapeUnderscore(::pltxt2htm::EscapeUnderscore&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeUnderscore& operator=(::pltxt2htm::EscapeUnderscore const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeUnderscore& operator=(::pltxt2htm::EscapeUnderscore&&) noexcept = default;
};

/**
 * @brief Represents `.
 */
class EscapeBacktick : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeBacktick() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_backtick) {
    }

    constexpr EscapeBacktick(::pltxt2htm::EscapeBacktick const&) noexcept = default;

    constexpr EscapeBacktick(::pltxt2htm::EscapeBacktick&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeBacktick& operator=(::pltxt2htm::EscapeBacktick const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeBacktick& operator=(::pltxt2htm::EscapeBacktick&&) noexcept = default;
};

/**
 * @brief Represents {.
 */
class EscapeLeftBrace : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeLeftBrace() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_left_brace) {
    }

    constexpr EscapeLeftBrace(::pltxt2htm::EscapeLeftBrace const&) noexcept = default;

    constexpr EscapeLeftBrace(::pltxt2htm::EscapeLeftBrace&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftBrace& operator=(::pltxt2htm::EscapeLeftBrace const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeLeftBrace& operator=(::pltxt2htm::EscapeLeftBrace&&) noexcept = default;
};

/**
 * @brief Represents |.
 */
class EscapePipe : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapePipe() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_pipe) {
    }

    constexpr EscapePipe(::pltxt2htm::EscapePipe const&) noexcept = default;

    constexpr EscapePipe(::pltxt2htm::EscapePipe&&) noexcept = default;

    constexpr ::pltxt2htm::EscapePipe& operator=(::pltxt2htm::EscapePipe const&) noexcept = default;

    constexpr ::pltxt2htm::EscapePipe& operator=(::pltxt2htm::EscapePipe&&) noexcept = default;
};

/**
 * @brief Represents }.
 */
class EscapeRightBrace : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeRightBrace() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_right_brace) {
    }

    constexpr EscapeRightBrace(::pltxt2htm::EscapeRightBrace const&) noexcept = default;

    constexpr EscapeRightBrace(::pltxt2htm::EscapeRightBrace&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightBrace& operator=(::pltxt2htm::EscapeRightBrace const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeRightBrace& operator=(::pltxt2htm::EscapeRightBrace&&) noexcept = default;
};

/**
 * @brief Represents ~.
 */
class EscapeTilde : public ::pltxt2htm::PlTxtNode {
public:
    constexpr EscapeTilde() noexcept
        : ::pltxt2htm::PlTxtNode(::pltxt2htm::NodeType::md_escape_tilde) {
    }

    constexpr EscapeTilde(::pltxt2htm::EscapeTilde const&) noexcept = default;

    constexpr EscapeTilde(::pltxt2htm::EscapeTilde&&) noexcept = default;

    constexpr ::pltxt2htm::EscapeTilde& operator=(::pltxt2htm::EscapeTilde const&) noexcept = default;

    constexpr ::pltxt2htm::EscapeTilde& operator=(::pltxt2htm::EscapeTilde&&) noexcept = default;
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

    constexpr MdHr(::pltxt2htm::MdHr const&) noexcept = default;

    constexpr MdHr(::pltxt2htm::MdHr&&) noexcept = default;

    constexpr ::pltxt2htm::MdHr& operator=(::pltxt2htm::MdHr const&) noexcept = default;

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

    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick const&) noexcept = default;

    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeFenceBacktick& operator=(::pltxt2htm::MdCodeFenceBacktick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeFenceBacktick& operator=(::pltxt2htm::MdCodeFenceBacktick&&) noexcept = default;

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

    constexpr MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde const&) noexcept = default;

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

    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick const&) noexcept = default;

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

    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick const&) noexcept = default;

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

    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept = default;

    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick&&) noexcept = default;

    constexpr ::pltxt2htm::MdCodeSpan3Backtick& operator=(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept = delete;

    constexpr ::pltxt2htm::MdCodeSpan3Backtick& operator=(::pltxt2htm::MdCodeSpan3Backtick&&) noexcept = default;
};

} // namespace pltxt2htm
