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

namespace pltxt2htm::ast2 {

/**
 * @brief Markdown ATX heading level 1
 * @details Represents a heading started with '# '.
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH1 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH1() noexcept = delete;
    constexpr explicit MdAtxH1(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH1(::pltxt2htm::ast2::MdAtxH1<ndebug> const&) noexcept = delete;
    constexpr MdAtxH1(::pltxt2htm::ast2::MdAtxH1<ndebug>&&) noexcept;
    constexpr ~MdAtxH1() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH1<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH1<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH1<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH1<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ATX heading level 2
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH2 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH2() noexcept = delete;
    constexpr explicit MdAtxH2(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH2(::pltxt2htm::ast2::MdAtxH2<ndebug> const&) noexcept = delete;
    constexpr MdAtxH2(::pltxt2htm::ast2::MdAtxH2<ndebug>&&) noexcept;
    constexpr ~MdAtxH2() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH2<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH2<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH2<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH2<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ATX heading level 3
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH3 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH3() noexcept = delete;
    constexpr explicit MdAtxH3(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH3(::pltxt2htm::ast2::MdAtxH3<ndebug> const&) noexcept = delete;
    constexpr MdAtxH3(::pltxt2htm::ast2::MdAtxH3<ndebug>&&) noexcept;
    constexpr ~MdAtxH3() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH3<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH3<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH3<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH3<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ATX heading level 4
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH4 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH4() noexcept = delete;
    constexpr explicit MdAtxH4(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH4(::pltxt2htm::ast2::MdAtxH4<ndebug> const&) noexcept = delete;
    constexpr MdAtxH4(::pltxt2htm::ast2::MdAtxH4<ndebug>&&) noexcept;
    constexpr ~MdAtxH4() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH4<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH4<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH4<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH4<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ATX heading level 5
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH5 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH5() noexcept = delete;
    constexpr explicit MdAtxH5(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH5(::pltxt2htm::ast2::MdAtxH5<ndebug> const&) noexcept = delete;
    constexpr MdAtxH5(::pltxt2htm::ast2::MdAtxH5<ndebug>&&) noexcept;
    constexpr ~MdAtxH5() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH5<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH5<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH5<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH5<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ATX heading level 6
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH6 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdAtxH6() noexcept = delete;
    constexpr explicit MdAtxH6(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdAtxH6(::pltxt2htm::ast2::MdAtxH6<ndebug> const&) noexcept = delete;
    constexpr MdAtxH6(::pltxt2htm::ast2::MdAtxH6<ndebug>&&) noexcept;
    constexpr ~MdAtxH6() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH6<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdAtxH6<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdAtxH6<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdAtxH6<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

// Markdown escaped characters (each represents a backslash-escaped character)
/** @brief Escaped backslash '\\' */ class MdEscapeBackslash {};

/** @brief Escaped exclamation '!' */ class MdEscapeExclamation {};

/** @brief Escaped double quote '"' */ class MdEscapeDoubleQuote {};

/** @brief Escaped hash '#' */ class MdEscapeHash {};

/** @brief Escaped dollar '$' */ class MdEscapeDollar {};

/** @brief Escaped percent '%' */ class MdEscapePercent {};

/** @brief Escaped ampersand '&' */ class MdEscapeAmpersand {};

/** @brief Escaped single quote '\'' */ class MdEscapeSingleQuote {};

/** @brief Escaped left parenthesis '(' */ class MdEscapeLeftParen {};

/** @brief Escaped right parenthesis ')' */ class MdEscapeRightParen {};

/** @brief Escaped asterisk '*' */ class MdEscapeAsterisk {};

/** @brief Escaped plus '+' */ class MdEscapePlus {};

/** @brief Escaped comma ',' */ class MdEscapeComma {};

/** @brief Escaped hyphen '-' */ class MdEscapeHyphen {};

/** @brief Escaped dot '.' */ class MdEscapeDot {};

/** @brief Escaped slash '/' */ class MdEscapeSlash {};

/** @brief Escaped colon ':' */ class MdEscapeColon {};

/** @brief Escaped semicolon ';' */ class MdEscapeSemicolon {};

/** @brief Escaped less-than '<' */ class MdEscapeLessThan {};

/** @brief Escaped equals '=' */ class MdEscapeEquals {};

/** @brief Escaped greater-than '>' */ class MdEscapeGreaterThan {};

/** @brief Escaped question '?' */ class MdEscapeQuestion {};

/** @brief Escaped at '@' */ class MdEscapeAt {};

/** @brief Escaped left bracket '[' */ class MdEscapeLeftBracket {};

/** @brief Escaped right bracket ']' */ class MdEscapeRightBracket {};

/** @brief Escaped caret '^' */ class MdEscapeCaret {};

/** @brief Escaped underscore '_' */ class MdEscapeUnderscore {};

/** @brief Escaped backtick '`' */ class MdEscapeBacktick {};

/** @brief Escaped left brace '{' */ class MdEscapeLeftBrace {};

/** @brief Escaped pipe '|' */ class MdEscapePipe {};

/** @brief Escaped right brace '}' */ class MdEscapeRightBrace {};

/** @brief Escaped tilde '~' */ class MdEscapeTilde {};

/**
 * @brief Markdown thematic break (horizontal rule)
 * @details Represents '---', '***', or '___' lines.
 */
class MdHr {};

/**
 * @brief Markdown fenced code block using backticks
 * @details Contains code content and an optional language identifier.
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeFenceBacktick {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};
    ::exception::optional<::fast_io::u8string> lang_;

public:
    constexpr MdCodeFenceBacktick() noexcept = delete;
    /**
     * @brief Construct a backtick fenced code block.
     * @param subast The inner code content as an AST.
     * @param lang Optional language string.
     */
    constexpr explicit MdCodeFenceBacktick(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                           ::exception::optional<::fast_io::u8string>&& lang) noexcept;
    constexpr MdCodeFenceBacktick(::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug> const&) noexcept = delete;
    constexpr MdCodeFenceBacktick(::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>&&) noexcept;
    constexpr ~MdCodeFenceBacktick() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }

    [[nodiscard]]
    constexpr auto&& get_language(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.lang_);
    }
};

/**
 * @brief Markdown fenced code block using tildes
 * @details Similar to backtick fence but using '~~~' delimiters.
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeFenceTilde {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};
    ::exception::optional<::fast_io::u8string> lang_;

public:
    constexpr MdCodeFenceTilde() noexcept = delete;
    /**
     * @brief Construct a tilde fenced code block.
     * @param subast The code content AST.
     * @param lang Optional language string.
     */
    constexpr explicit MdCodeFenceTilde(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                        ::exception::optional<::fast_io::u8string>&& lang) noexcept;
    constexpr MdCodeFenceTilde(::pltxt2htm::ast2::MdCodeFenceTilde<ndebug> const&) noexcept = delete;
    constexpr MdCodeFenceTilde(::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>&&) noexcept;
    constexpr ~MdCodeFenceTilde() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeFenceTilde<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }

    [[nodiscard]]
    constexpr auto&& get_language(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.lang_);
    }
};

/**
 * @brief Markdown code span delimited by a single backtick
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan1Backtick {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdCodeSpan1Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan1Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan1Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown code span delimited by two backticks
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan2Backtick {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdCodeSpan2Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan2Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan2Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown code span delimited by three backticks
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan3Backtick {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdCodeSpan3Backtick() noexcept = delete;
    constexpr explicit MdCodeSpan3Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug> const&) noexcept = delete;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan3Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Single asterisk emphasis (e.g., *italic*)
 */
template<::pltxt2htm::Contracts ndebug>
class MdSingleEmphasisAsterisk {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdSingleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdSingleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Double asterisk emphasis (e.g., **bold**)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDoubleEmphasisAsterisk {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdDoubleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdDoubleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Triple asterisk emphasis (e.g., ***bold italic***)
 */
template<::pltxt2htm::Contracts ndebug>
class MdTripleEmphasisAsterisk {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdTripleEmphasisAsterisk() noexcept = delete;
    constexpr explicit MdTripleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug> const&) noexcept = delete;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Single underscore emphasis (e.g., _italic_)
 */
template<::pltxt2htm::Contracts ndebug>
class MdSingleEmphasisUnderscore {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdSingleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdSingleEmphasisUnderscore(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug> const&) noexcept =
        delete;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Double underscore emphasis (e.g., __bold__)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDoubleEmphasisUnderscore {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdDoubleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdDoubleEmphasisUnderscore(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept =
        delete;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Triple underscore emphasis (e.g., ___bold italic___)
 */
template<::pltxt2htm::Contracts ndebug>
class MdTripleEmphasisUnderscore {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdTripleEmphasisUnderscore() noexcept = delete;
    constexpr explicit MdTripleEmphasisUnderscore(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug> const&) noexcept =
        delete;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown strikethrough (e.g., ~~deleted~~)
 */
template<::pltxt2htm::Contracts ndebug>
class MdDel {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdDel() noexcept = delete;
    constexpr explicit MdDel(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdDel(::pltxt2htm::ast2::MdDel<ndebug> const&) noexcept = delete;
    constexpr MdDel(::pltxt2htm::ast2::MdDel<ndebug>&&) noexcept;
    constexpr ~MdDel() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdDel<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdDel<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdDel<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdDel<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown link (e.g., [text](url))
 */
template<::pltxt2htm::Contracts ndebug>
class MdLink {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};
    ::pltxt2htm::ast2::Url<ndebug> url_;

public:
    constexpr MdLink() noexcept = delete;
    /**
     * @brief Construct a link.
     * @param subast The link text/content AST.
     * @param url The target URL.
     */
    constexpr explicit MdLink(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept;
    constexpr MdLink(::pltxt2htm::ast2::MdLink<ndebug> const&) noexcept = delete;
    constexpr MdLink(::pltxt2htm::ast2::MdLink<ndebug>&&) noexcept;
    constexpr ~MdLink() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdLink<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdLink<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdLink<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdLink<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }

    [[nodiscard]]
    constexpr auto&& get_url(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_);
    }
};

/**
 * @brief Markdown image (e.g., ![alt](src))
 */
template<::pltxt2htm::Contracts ndebug>
class MdImage {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};
    ::pltxt2htm::ast2::Url<ndebug> url_;

public:
    constexpr MdImage() noexcept = delete;
    /**
     * @brief Construct an image.
     * @param subast The alt text/content AST.
     * @param url The image source URL.
     */
    constexpr explicit MdImage(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept;
    constexpr MdImage(::pltxt2htm::ast2::MdImage<ndebug> const&) noexcept = delete;
    constexpr MdImage(::pltxt2htm::ast2::MdImage<ndebug>&&) noexcept;
    constexpr ~MdImage() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdImage<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdImage<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdImage<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdImage<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }

    [[nodiscard]]
    constexpr auto&& get_url(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_);
    }
};

/**
 * @brief Markdown block quote (lines starting with '>')
 */
template<::pltxt2htm::Contracts ndebug>
class MdBlockQuotes {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdBlockQuotes() noexcept = delete;
    constexpr explicit MdBlockQuotes(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdBlockQuotes(::pltxt2htm::ast2::MdBlockQuotes<ndebug> const&) noexcept = delete;
    constexpr MdBlockQuotes(::pltxt2htm::ast2::MdBlockQuotes<ndebug>&&) noexcept;
    constexpr ~MdBlockQuotes() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdBlockQuotes<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdBlockQuotes<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdBlockQuotes<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdBlockQuotes<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown unordered list (items prefixed with '-', '+', or '*')
 */
template<::pltxt2htm::Contracts ndebug>
class MdUl {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdUl() noexcept = delete;
    constexpr explicit MdUl(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdUl(::pltxt2htm::ast2::MdUl<ndebug> const&) noexcept = delete;
    constexpr MdUl(::pltxt2htm::ast2::MdUl<ndebug>&&) noexcept;
    constexpr ~MdUl() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdUl<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdUl<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdUl<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdUl<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown ordered list (items prefixed with numbers followed by '.')
 */
template<::pltxt2htm::Contracts ndebug>
class MdOl {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdOl() noexcept = delete;
    constexpr explicit MdOl(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdOl(::pltxt2htm::ast2::MdOl<ndebug> const&) noexcept = delete;
    constexpr MdOl(::pltxt2htm::ast2::MdOl<ndebug>&&) noexcept;
    constexpr ~MdOl() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdOl<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdOl<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdOl<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdOl<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Markdown list item
 */
template<::pltxt2htm::Contracts ndebug>
class MdLi {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdLi() noexcept = delete;
    constexpr explicit MdLi(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdLi(::pltxt2htm::ast2::MdLi<ndebug> const&) noexcept = delete;
    constexpr MdLi(::pltxt2htm::ast2::MdLi<ndebug>&&) noexcept;
    constexpr ~MdLi() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdLi<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdLi<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdLi<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdLi<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Inline LaTeX math (delimited by $...$)
 */
template<::pltxt2htm::Contracts ndebug>
class MdLatexInline {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdLatexInline() noexcept = delete;
    constexpr explicit MdLatexInline(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdLatexInline(::pltxt2htm::ast2::MdLatexInline<ndebug> const&) noexcept = delete;
    constexpr MdLatexInline(::pltxt2htm::ast2::MdLatexInline<ndebug>&&) noexcept;
    constexpr ~MdLatexInline() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdLatexInline<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdLatexInline<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdLatexInline<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdLatexInline<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief Block LaTeX math (delimited by $$...$$)
 */
template<::pltxt2htm::Contracts ndebug>
class MdLatexBlock {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr MdLatexBlock() noexcept = delete;
    constexpr explicit MdLatexBlock(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr MdLatexBlock(::pltxt2htm::ast2::MdLatexBlock<ndebug> const&) noexcept = delete;
    constexpr MdLatexBlock(::pltxt2htm::ast2::MdLatexBlock<ndebug>&&) noexcept;
    constexpr ~MdLatexBlock() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::MdLatexBlock<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::MdLatexBlock<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::MdLatexBlock<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::MdLatexBlock<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

} // namespace pltxt2htm::ast2
