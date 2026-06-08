/**
 * @file markdown_node_decl.hh
 * @brief Markdown-specific AST node declarations for pltxt2htm
 * @details Defines nodes for Markdown syntax: ATX headings, escaped characters,
 *          thematic breaks, code fences/spans, emphasis, strikethrough, links,
 *          images, block quotes, lists, and LaTeX.
 */

#pragma once

#include <utility>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string.h>
#include "ast_decl.hh"
#include "basic_node_decl.hh"
#include "../node_type.hh"

namespace pltxt2htm {

/**
 * @brief Markdown ATX heading level 1
 * @details Represents a heading started with '# '.
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH1 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH1() noexcept = delete;
    constexpr explicit MdAtxH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1<ndebug> const&) noexcept = delete;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1<ndebug>&&) noexcept;
    constexpr ~MdAtxH1() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH1<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH1<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH1<ndebug>& self, ::pltxt2htm::MdAtxH1<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH1<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH1 const&, MdAtxH1 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ATX heading level 2
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH2 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH2() noexcept = delete;
    constexpr explicit MdAtxH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2<ndebug> const&) noexcept = delete;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2<ndebug>&&) noexcept;
    constexpr ~MdAtxH2() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH2<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH2<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH2<ndebug>& self, ::pltxt2htm::MdAtxH2<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH2<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH2 const&, MdAtxH2 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ATX heading level 3
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH3 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH3() noexcept = delete;
    constexpr explicit MdAtxH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3<ndebug> const&) noexcept = delete;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3<ndebug>&&) noexcept;
    constexpr ~MdAtxH3() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH3<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH3<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH3<ndebug>& self, ::pltxt2htm::MdAtxH3<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH3<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH3 const&, MdAtxH3 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ATX heading level 4
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH4 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH4() noexcept = delete;
    constexpr explicit MdAtxH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4<ndebug> const&) noexcept = delete;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4<ndebug>&&) noexcept;
    constexpr ~MdAtxH4() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH4<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH4<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH4<ndebug>& self, ::pltxt2htm::MdAtxH4<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH4<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH4 const&, MdAtxH4 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ATX heading level 5
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH5 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH5() noexcept = delete;
    constexpr explicit MdAtxH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5<ndebug> const&) noexcept = delete;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5<ndebug>&&) noexcept;
    constexpr ~MdAtxH5() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH5<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH5<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH5<ndebug>& self, ::pltxt2htm::MdAtxH5<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH5<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH5 const&, MdAtxH5 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ATX heading level 6
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH6 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdAtxH6() noexcept = delete;
    constexpr explicit MdAtxH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6<ndebug> const&) noexcept = delete;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6<ndebug>&&) noexcept;
    constexpr ~MdAtxH6() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH6<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH6<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdAtxH6<ndebug>& self, ::pltxt2htm::MdAtxH6<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH6<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH6 const&, MdAtxH6 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

// Markdown escaped characters (each represents a backslash-escaped character)
/** @brief Escaped backslash '\\' */ class MdEscapeBackslash {
public:
    constexpr auto operator==(this MdEscapeBackslash const&, MdEscapeBackslash const&) noexcept -> bool = default;
};

/** @brief Escaped exclamation '!' */ class MdEscapeExclamation {
public:
    constexpr auto operator==(this MdEscapeExclamation const&, MdEscapeExclamation const&) noexcept -> bool = default;
};

/** @brief Escaped double quote '"' */ class MdEscapeDoubleQuote {
public:
    constexpr auto operator==(this MdEscapeDoubleQuote const&, MdEscapeDoubleQuote const&) noexcept -> bool = default;
};

/** @brief Escaped hash '#' */ class MdEscapeHash {
public:
    constexpr auto operator==(this MdEscapeHash const&, MdEscapeHash const&) noexcept -> bool = default;
};

/** @brief Escaped dollar '$' */ class MdEscapeDollar {
public:
    constexpr auto operator==(this MdEscapeDollar const&, MdEscapeDollar const&) noexcept -> bool = default;
};

/** @brief Escaped percent '%' */ class MdEscapePercent {
public:
    constexpr auto operator==(this MdEscapePercent const&, MdEscapePercent const&) noexcept -> bool = default;
};

/** @brief Escaped ampersand '&' */ class MdEscapeAmpersand {
public:
    constexpr auto operator==(this MdEscapeAmpersand const&, MdEscapeAmpersand const&) noexcept -> bool = default;
};

/** @brief Escaped single quote '\'' */ class MdEscapeSingleQuote {
public:
    constexpr auto operator==(this MdEscapeSingleQuote const&, MdEscapeSingleQuote const&) noexcept -> bool = default;
};

/** @brief Escaped left parenthesis '(' */ class MdEscapeLeftParen {
public:
    constexpr auto operator==(this MdEscapeLeftParen const&, MdEscapeLeftParen const&) noexcept -> bool = default;
};

/** @brief Escaped right parenthesis ')' */ class MdEscapeRightParen {
public:
    constexpr auto operator==(this MdEscapeRightParen const&, MdEscapeRightParen const&) noexcept -> bool = default;
};

/** @brief Escaped asterisk '*' */ class MdEscapeAsterisk {
public:
    constexpr auto operator==(this MdEscapeAsterisk const&, MdEscapeAsterisk const&) noexcept -> bool = default;
};

/** @brief Escaped plus '+' */ class MdEscapePlus {
public:
    constexpr auto operator==(this MdEscapePlus const&, MdEscapePlus const&) noexcept -> bool = default;
};

/** @brief Escaped comma ',' */ class MdEscapeComma {
public:
    constexpr auto operator==(this MdEscapeComma const&, MdEscapeComma const&) noexcept -> bool = default;
};

/** @brief Escaped hyphen '-' */ class MdEscapeHyphen {
public:
    constexpr auto operator==(this MdEscapeHyphen const&, MdEscapeHyphen const&) noexcept -> bool = default;
};

/** @brief Escaped dot '.' */ class MdEscapeDot {
public:
    constexpr auto operator==(this MdEscapeDot const&, MdEscapeDot const&) noexcept -> bool = default;
};

/** @brief Escaped slash '/' */ class MdEscapeSlash {
public:
    constexpr auto operator==(this MdEscapeSlash const&, MdEscapeSlash const&) noexcept -> bool = default;
};

/** @brief Escaped colon ':' */ class MdEscapeColon {
public:
    constexpr auto operator==(this MdEscapeColon const&, MdEscapeColon const&) noexcept -> bool = default;
};

/** @brief Escaped semicolon ';' */ class MdEscapeSemicolon {
public:
    constexpr auto operator==(this MdEscapeSemicolon const&, MdEscapeSemicolon const&) noexcept -> bool = default;
};

/** @brief Escaped less-than '<' */ class MdEscapeLessThan {
public:
    constexpr auto operator==(this MdEscapeLessThan const&, MdEscapeLessThan const&) noexcept -> bool = default;
};

/** @brief Escaped equals '=' */ class MdEscapeEquals {
public:
    constexpr auto operator==(this MdEscapeEquals const&, MdEscapeEquals const&) noexcept -> bool = default;
};

/** @brief Escaped greater-than '>' */ class MdEscapeGreaterThan {
public:
    constexpr auto operator==(this MdEscapeGreaterThan const&, MdEscapeGreaterThan const&) noexcept -> bool = default;
};

/** @brief Escaped question '?' */ class MdEscapeQuestion {
public:
    constexpr auto operator==(this MdEscapeQuestion const&, MdEscapeQuestion const&) noexcept -> bool = default;
};

/** @brief Escaped at '@' */ class MdEscapeAt {
public:
    constexpr auto operator==(this MdEscapeAt const&, MdEscapeAt const&) noexcept -> bool = default;
};

/** @brief Escaped left bracket '[' */ class MdEscapeLeftBracket {
public:
    constexpr auto operator==(this MdEscapeLeftBracket const&, MdEscapeLeftBracket const&) noexcept -> bool = default;
};

/** @brief Escaped right bracket ']' */ class MdEscapeRightBracket {
public:
    constexpr auto operator==(this MdEscapeRightBracket const&, MdEscapeRightBracket const&) noexcept -> bool = default;
};

/** @brief Escaped caret '^' */ class MdEscapeCaret {
public:
    constexpr auto operator==(this MdEscapeCaret const&, MdEscapeCaret const&) noexcept -> bool = default;
};

/** @brief Escaped underscore '_' */ class MdEscapeUnderscore {
public:
    constexpr auto operator==(this MdEscapeUnderscore const&, MdEscapeUnderscore const&) noexcept -> bool = default;
};

/** @brief Escaped backtick '`' */ class MdEscapeBacktick {
public:
    constexpr auto operator==(this MdEscapeBacktick const&, MdEscapeBacktick const&) noexcept -> bool = default;
};

/** @brief Escaped left brace '{' */ class MdEscapeLeftBrace {
public:
    constexpr auto operator==(this MdEscapeLeftBrace const&, MdEscapeLeftBrace const&) noexcept -> bool = default;
};

/** @brief Escaped pipe '|' */ class MdEscapePipe {
public:
    constexpr auto operator==(this MdEscapePipe const&, MdEscapePipe const&) noexcept -> bool = default;
};

/** @brief Escaped right brace '}' */ class MdEscapeRightBrace {
public:
    constexpr auto operator==(this MdEscapeRightBrace const&, MdEscapeRightBrace const&) noexcept -> bool = default;
};

/** @brief Escaped tilde '~' */ class MdEscapeTilde {
public:
    constexpr auto operator==(this MdEscapeTilde const&, MdEscapeTilde const&) noexcept -> bool = default;
};

/**
 * @brief Markdown thematic break (horizontal rule)
 * @details Represents '---', '***', or '___' lines.
 */
class MdHr {
public:
    constexpr auto operator==(this MdHr const&, MdHr const&) noexcept -> bool = default;
};

/**
 * @brief Markdown fenced code block using backticks
 * @details Contains code content and an optional language identifier.
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeFenceBacktick {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::exception::optional<::fast_io::u8string> lang;

public:
    constexpr MdCodeFenceBacktick() noexcept = delete;
    /**
     * @brief Construct a backtick fenced code block.
     * @param subast The inner code content as an AST.
     * @param lang Optional language string.
     */
    constexpr explicit MdCodeFenceBacktick(::pltxt2htm::Ast<ndebug>&& subast_,
                                           ::exception::optional<::fast_io::u8string>&& lang_) noexcept;
    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick<ndebug> const&) noexcept = delete;
    constexpr MdCodeFenceBacktick(::pltxt2htm::MdCodeFenceBacktick<ndebug>&&) noexcept;
    constexpr ~MdCodeFenceBacktick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeFenceBacktick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeFenceBacktick<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdCodeFenceBacktick<ndebug>& self,
                             ::pltxt2htm::MdCodeFenceBacktick<ndebug>&&) noexcept
        -> ::pltxt2htm::MdCodeFenceBacktick<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdCodeFenceBacktick const& self, MdCodeFenceBacktick const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_language(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

/**
 * @brief Markdown fenced code block using tildes
 * @details Similar to backtick fence but using '~~~' delimiters.
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeFenceTilde {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::exception::optional<::fast_io::u8string> lang;

public:
    constexpr MdCodeFenceTilde() noexcept = delete;
    /**
     * @brief Construct a tilde fenced code block.
     * @param subast The code content AST.
     * @param lang Optional language string.
     */
    constexpr explicit MdCodeFenceTilde(::pltxt2htm::Ast<ndebug>&& subast_,
                                        ::exception::optional<::fast_io::u8string>&& lang_) noexcept;
    constexpr MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde<ndebug> const&) noexcept = delete;
    constexpr MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde<ndebug>&&) noexcept;
    constexpr ~MdCodeFenceTilde() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeFenceTilde<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeFenceTilde<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdCodeFenceTilde<ndebug>& self,
                             ::pltxt2htm::MdCodeFenceTilde<ndebug>&&) noexcept
        -> ::pltxt2htm::MdCodeFenceTilde<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdCodeFenceTilde const& self, MdCodeFenceTilde const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_language(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

/**
 * @brief Markdown code span delimited by a single backtick
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan1Backtick {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdCodeSpan1Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan1Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan1Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan1Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan1Backtick<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdCodeSpan1Backtick<ndebug>& self,
                             ::pltxt2htm::MdCodeSpan1Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::MdCodeSpan1Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdCodeSpan1Backtick const&, MdCodeSpan1Backtick const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown code span delimited by two backticks
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan2Backtick {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdCodeSpan2Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan2Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan2Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan2Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan2Backtick<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdCodeSpan2Backtick<ndebug>& self,
                             ::pltxt2htm::MdCodeSpan2Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::MdCodeSpan2Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdCodeSpan2Backtick const&, MdCodeSpan2Backtick const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown code span delimited by three backticks
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan3Backtick {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdCodeSpan3Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan3Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan3Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan3Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan3Backtick<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdCodeSpan3Backtick<ndebug>& self,
                             ::pltxt2htm::MdCodeSpan3Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::MdCodeSpan3Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdCodeSpan3Backtick const&, MdCodeSpan3Backtick const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Single asterisk emphasis (e.g., *italic*)
 */
template<::pltxt2htm::Contracts ndebug>
class MdSingleEmphasisAsterisk {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdSingleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdSingleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>& self,
                             ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdSingleEmphasisAsterisk const&, MdSingleEmphasisAsterisk const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Double asterisk emphasis (e.g., **bold**)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDoubleEmphasisAsterisk {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdDoubleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdDoubleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>& self,
                             ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdDoubleEmphasisAsterisk const&, MdDoubleEmphasisAsterisk const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Triple asterisk emphasis (e.g., ***bold italic***)
 */
template<::pltxt2htm::Contracts ndebug>
class MdTripleEmphasisAsterisk {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdTripleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdTripleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>& self,
                             ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTripleEmphasisAsterisk const&, MdTripleEmphasisAsterisk const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Single underscore emphasis (e.g., _italic_)
 */
template<::pltxt2htm::Contracts ndebug>
class MdSingleEmphasisUnderscore {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdSingleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdSingleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> const&) noexcept = delete;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdSingleEmphasisUnderscore const&, MdSingleEmphasisUnderscore const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Double underscore emphasis (e.g., __bold__)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDoubleEmphasisUnderscore {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdDoubleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdDoubleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept = delete;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdDoubleEmphasisUnderscore const&, MdDoubleEmphasisUnderscore const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Triple underscore emphasis (e.g., ___bold italic___)
 */
template<::pltxt2htm::Contracts ndebug>
class MdTripleEmphasisUnderscore {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdTripleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdTripleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> const&) noexcept = delete;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTripleEmphasisUnderscore const&, MdTripleEmphasisUnderscore const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown strikethrough (e.g., ~~deleted~~)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDel {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdDel() noexcept = delete;
    constexpr explicit MdDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDel(::pltxt2htm::MdDel<ndebug> const&) noexcept = delete;
    constexpr MdDel(::pltxt2htm::MdDel<ndebug>&&) noexcept;
    constexpr ~MdDel() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDel<ndebug> const&) noexcept -> ::pltxt2htm::MdDel<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdDel<ndebug>& self, ::pltxt2htm::MdDel<ndebug>&&) noexcept
        -> ::pltxt2htm::MdDel<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdDel const&, MdDel const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown link (e.g., [text](url))
 */
template<::pltxt2htm::Contracts ndebug>
class MdLink {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::Url<ndebug> url;

public:
    constexpr MdLink() noexcept = delete;
    /**
     * @brief Construct a link.
     * @param subast The link text/content AST.
     * @param url The target URL.
     */
    constexpr explicit MdLink(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url<ndebug>&& url_) noexcept;
    constexpr MdLink(::pltxt2htm::MdLink<ndebug> const&) noexcept = delete;
    constexpr MdLink(::pltxt2htm::MdLink<ndebug>&&) noexcept;
    constexpr ~MdLink() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLink<ndebug> const&) noexcept -> ::pltxt2htm::MdLink<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdLink<ndebug>& self, ::pltxt2htm::MdLink<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLink<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLink const&, MdLink const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }
};

/**
 * @brief Markdown image (e.g., ![alt](src))
 */
template<::pltxt2htm::Contracts ndebug>
class MdImage {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::Url<ndebug> url;

public:
    constexpr MdImage() noexcept = delete;
    /**
     * @brief Construct an image.
     * @param subast The alt text/content AST.
     * @param url The image source URL.
     */
    constexpr explicit MdImage(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url<ndebug>&& url_) noexcept;
    constexpr MdImage(::pltxt2htm::MdImage<ndebug> const&) noexcept = delete;
    constexpr MdImage(::pltxt2htm::MdImage<ndebug>&&) noexcept;
    constexpr ~MdImage() noexcept;
    constexpr auto operator=(::pltxt2htm::MdImage<ndebug> const&) noexcept -> ::pltxt2htm::MdImage<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdImage<ndebug>& self, ::pltxt2htm::MdImage<ndebug>&&) noexcept
        -> ::pltxt2htm::MdImage<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdImage const&, MdImage const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }
};

/**
 * @brief Markdown block quote (lines starting with '>')
 */
template<::pltxt2htm::Contracts ndebug>
class MdBlockQuotes {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdBlockQuotes() noexcept = delete;
    constexpr explicit MdBlockQuotes(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes<ndebug> const&) noexcept = delete;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept;
    constexpr ~MdBlockQuotes() noexcept;
    constexpr auto operator=(::pltxt2htm::MdBlockQuotes<ndebug> const&) noexcept
        -> ::pltxt2htm::MdBlockQuotes<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdBlockQuotes<ndebug>& self,
                             ::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept -> ::pltxt2htm::MdBlockQuotes<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdBlockQuotes const&, MdBlockQuotes const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown unordered list (items prefixed with '-', '+', or '*')
 */
template<::pltxt2htm::Contracts ndebug>
class MdUl {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdUl() noexcept = delete;
    constexpr explicit MdUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdUl(::pltxt2htm::MdUl<ndebug> const&) noexcept = delete;
    constexpr MdUl(::pltxt2htm::MdUl<ndebug>&&) noexcept;
    constexpr ~MdUl() noexcept;
    constexpr auto operator=(::pltxt2htm::MdUl<ndebug> const&) noexcept -> ::pltxt2htm::MdUl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdUl<ndebug>& self, ::pltxt2htm::MdUl<ndebug>&&) noexcept
        -> ::pltxt2htm::MdUl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdUl const&, MdUl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown ordered list (items prefixed with numbers followed by '.')
 */
template<::pltxt2htm::Contracts ndebug>
class MdOl {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdOl() noexcept = delete;
    constexpr explicit MdOl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdOl(::pltxt2htm::MdOl<ndebug> const&) noexcept = delete;
    constexpr MdOl(::pltxt2htm::MdOl<ndebug>&&) noexcept;
    constexpr ~MdOl() noexcept;
    constexpr auto operator=(::pltxt2htm::MdOl<ndebug> const&) noexcept -> ::pltxt2htm::MdOl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdOl<ndebug>& self, ::pltxt2htm::MdOl<ndebug>&&) noexcept
        -> ::pltxt2htm::MdOl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdOl const&, MdOl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown list item
 */
template<::pltxt2htm::Contracts ndebug>
class MdLi {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdLi() noexcept = delete;
    constexpr explicit MdLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdLi(::pltxt2htm::MdLi<ndebug> const&) noexcept = delete;
    constexpr MdLi(::pltxt2htm::MdLi<ndebug>&&) noexcept;
    constexpr ~MdLi() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLi<ndebug> const&) noexcept -> ::pltxt2htm::MdLi<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdLi<ndebug>& self, ::pltxt2htm::MdLi<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLi<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLi const&, MdLi const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown checkbox list item (- [ ] / - [x])
 */
template<::pltxt2htm::Contracts ndebug>
class MdLiCheckbox {
    ::pltxt2htm::Ast<ndebug> subast{};
    bool checked_{};

public:
    constexpr MdLiCheckbox() noexcept = delete;
    constexpr explicit MdLiCheckbox(::pltxt2htm::Ast<ndebug>&& subast_, bool checked) noexcept;
    constexpr MdLiCheckbox(::pltxt2htm::MdLiCheckbox<ndebug> const&) noexcept = delete;
    constexpr MdLiCheckbox(::pltxt2htm::MdLiCheckbox<ndebug>&&) noexcept;
    constexpr ~MdLiCheckbox() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLiCheckbox<ndebug> const&) noexcept
        -> ::pltxt2htm::MdLiCheckbox<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdLiCheckbox<ndebug>& self, ::pltxt2htm::MdLiCheckbox<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLiCheckbox<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLiCheckbox const&, MdLiCheckbox const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto&& self) noexcept -> bool {
        return self.checked_;
    }
};

/**
 * @brief Inline LaTeX math (delimited by $...$)
 */
template<::pltxt2htm::Contracts ndebug>
class MdLatexInline {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdLatexInline() noexcept = delete;
    constexpr explicit MdLatexInline(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline<ndebug> const&) noexcept = delete;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline<ndebug>&&) noexcept;
    constexpr ~MdLatexInline() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLatexInline<ndebug> const&) noexcept
        -> ::pltxt2htm::MdLatexInline<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdLatexInline<ndebug>& self,
                             ::pltxt2htm::MdLatexInline<ndebug>&&) noexcept -> ::pltxt2htm::MdLatexInline<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLatexInline const&, MdLatexInline const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Block LaTeX math (delimited by $$...$$)
 */
template<::pltxt2htm::Contracts ndebug>
class MdLatexBlock {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdLatexBlock() noexcept = delete;
    constexpr explicit MdLatexBlock(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock<ndebug> const&) noexcept = delete;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept;
    constexpr ~MdLatexBlock() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLatexBlock<ndebug> const&) noexcept
        -> ::pltxt2htm::MdLatexBlock<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdLatexBlock<ndebug>& self, ::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLatexBlock<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLatexBlock const&, MdLatexBlock const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown table (pipe-table syntax)
 */
template<::pltxt2htm::Contracts ndebug>
class MdTable {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdTable() noexcept = delete;
    constexpr explicit MdTable(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTable(::pltxt2htm::MdTable<ndebug> const&) noexcept = delete;
    constexpr MdTable(::pltxt2htm::MdTable<ndebug>&&) noexcept;
    constexpr ~MdTable() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTable<ndebug> const&) noexcept -> ::pltxt2htm::MdTable<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTable<ndebug>& self, ::pltxt2htm::MdTable<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTable<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTable const&, MdTable const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown table header section
 */
template<::pltxt2htm::Contracts ndebug>
class MdThead {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdThead() noexcept = delete;
    constexpr explicit MdThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdThead(::pltxt2htm::MdThead<ndebug> const&) noexcept = delete;
    constexpr MdThead(::pltxt2htm::MdThead<ndebug>&&) noexcept;
    constexpr ~MdThead() noexcept;
    constexpr auto operator=(::pltxt2htm::MdThead<ndebug> const&) noexcept -> ::pltxt2htm::MdThead<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdThead<ndebug>& self, ::pltxt2htm::MdThead<ndebug>&&) noexcept
        -> ::pltxt2htm::MdThead<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdThead const&, MdThead const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown table body section
 */
template<::pltxt2htm::Contracts ndebug>
class MdTbody {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdTbody() noexcept = delete;
    constexpr explicit MdTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTbody(::pltxt2htm::MdTbody<ndebug> const&) noexcept = delete;
    constexpr MdTbody(::pltxt2htm::MdTbody<ndebug>&&) noexcept;
    constexpr ~MdTbody() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTbody<ndebug> const&) noexcept -> ::pltxt2htm::MdTbody<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTbody<ndebug>& self, ::pltxt2htm::MdTbody<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTbody<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTbody const&, MdTbody const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown table row
 */
template<::pltxt2htm::Contracts ndebug>
class MdTr {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr MdTr() noexcept = delete;
    constexpr explicit MdTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTr(::pltxt2htm::MdTr<ndebug> const&) noexcept = delete;
    constexpr MdTr(::pltxt2htm::MdTr<ndebug>&&) noexcept;
    constexpr ~MdTr() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTr<ndebug> const&) noexcept -> ::pltxt2htm::MdTr<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTr<ndebug>& self, ::pltxt2htm::MdTr<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTr<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTr const&, MdTr const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown table header cell
 */
template<::pltxt2htm::Contracts ndebug>
class MdTh {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::MdTableAlign align{};

public:
    constexpr MdTh() noexcept = delete;
    constexpr explicit MdTh(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::MdTableAlign align_ = {}) noexcept;
    constexpr MdTh(::pltxt2htm::MdTh<ndebug> const&) noexcept = delete;
    constexpr MdTh(::pltxt2htm::MdTh<ndebug>&&) noexcept;
    constexpr ~MdTh() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTh<ndebug> const&) noexcept -> ::pltxt2htm::MdTh<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTh<ndebug>& self, ::pltxt2htm::MdTh<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTh<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTh const&, MdTh const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        return self.align;
    }
};

/**
 * @brief Markdown table data cell
 */
template<::pltxt2htm::Contracts ndebug>
class MdTd {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::MdTableAlign align{};

public:
    constexpr MdTd() noexcept = delete;
    constexpr explicit MdTd(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::MdTableAlign align_ = {}) noexcept;
    constexpr MdTd(::pltxt2htm::MdTd<ndebug> const&) noexcept = delete;
    constexpr MdTd(::pltxt2htm::MdTd<ndebug>&&) noexcept;
    constexpr ~MdTd() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTd<ndebug> const&) noexcept -> ::pltxt2htm::MdTd<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::MdTd<ndebug>& self, ::pltxt2htm::MdTd<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTd<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdTd const&, MdTd const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        return self.align;
    }
};

} // namespace pltxt2htm
