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
    constexpr MdAtxH1(MdAtxH1 const&) noexcept = delete;
    constexpr MdAtxH1(MdAtxH1&&) noexcept;
    constexpr ~MdAtxH1() noexcept;
    constexpr auto operator=(MdAtxH1 const&) noexcept -> MdAtxH1& = delete;
    constexpr auto operator=(MdAtxH1&&) noexcept -> MdAtxH1&;

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
    constexpr MdAtxH2(MdAtxH2 const&) noexcept = delete;
    constexpr MdAtxH2(MdAtxH2&&) noexcept;
    constexpr ~MdAtxH2() noexcept;
    constexpr auto operator=(MdAtxH2 const&) noexcept -> MdAtxH2& = delete;
    constexpr auto operator=(MdAtxH2&&) noexcept -> MdAtxH2&;

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
    constexpr MdAtxH3(MdAtxH3 const&) noexcept = delete;
    constexpr MdAtxH3(MdAtxH3&&) noexcept;
    constexpr ~MdAtxH3() noexcept;
    constexpr auto operator=(MdAtxH3 const&) noexcept -> MdAtxH3& = delete;
    constexpr auto operator=(MdAtxH3&&) noexcept -> MdAtxH3&;

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
    constexpr MdAtxH4(MdAtxH4 const&) noexcept = delete;
    constexpr MdAtxH4(MdAtxH4&&) noexcept;
    constexpr ~MdAtxH4() noexcept;
    constexpr auto operator=(MdAtxH4 const&) noexcept -> MdAtxH4& = delete;
    constexpr auto operator=(MdAtxH4&&) noexcept -> MdAtxH4&;

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
    constexpr MdAtxH5(MdAtxH5 const&) noexcept = delete;
    constexpr MdAtxH5(MdAtxH5&&) noexcept;
    constexpr ~MdAtxH5() noexcept;
    constexpr auto operator=(MdAtxH5 const&) noexcept -> MdAtxH5& = delete;
    constexpr auto operator=(MdAtxH5&&) noexcept -> MdAtxH5&;

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
    constexpr MdAtxH6(MdAtxH6 const&) noexcept = delete;
    constexpr MdAtxH6(MdAtxH6&&) noexcept;
    constexpr ~MdAtxH6() noexcept;
    constexpr auto operator=(MdAtxH6 const&) noexcept -> MdAtxH6& = delete;
    constexpr auto operator=(MdAtxH6&&) noexcept -> MdAtxH6&;

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
    constexpr MdCodeFenceBacktick(MdCodeFenceBacktick const&) noexcept = delete;
    constexpr MdCodeFenceBacktick(MdCodeFenceBacktick&&) noexcept;
    constexpr ~MdCodeFenceBacktick() noexcept;
    constexpr auto operator=(MdCodeFenceBacktick const&) noexcept -> MdCodeFenceBacktick& = delete;
    constexpr auto operator=(MdCodeFenceBacktick&&) noexcept -> MdCodeFenceBacktick&;

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
    constexpr MdCodeFenceTilde(MdCodeFenceTilde const&) noexcept = delete;
    constexpr MdCodeFenceTilde(MdCodeFenceTilde&&) noexcept;
    constexpr ~MdCodeFenceTilde() noexcept;
    constexpr auto operator=(MdCodeFenceTilde const&) noexcept -> MdCodeFenceTilde& = delete;
    constexpr auto operator=(MdCodeFenceTilde&&) noexcept -> MdCodeFenceTilde&;

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
    constexpr MdCodeSpan1Backtick(MdCodeSpan1Backtick const&) noexcept = delete;
    constexpr MdCodeSpan1Backtick(MdCodeSpan1Backtick&&) noexcept;
    constexpr ~MdCodeSpan1Backtick() noexcept;
    constexpr auto operator=(MdCodeSpan1Backtick const&) noexcept -> MdCodeSpan1Backtick& = delete;
    constexpr auto operator=(MdCodeSpan1Backtick&&) noexcept -> MdCodeSpan1Backtick&;

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
    constexpr MdCodeSpan2Backtick(MdCodeSpan2Backtick const&) noexcept = delete;
    constexpr MdCodeSpan2Backtick(MdCodeSpan2Backtick&&) noexcept;
    constexpr ~MdCodeSpan2Backtick() noexcept;
    constexpr auto operator=(MdCodeSpan2Backtick const&) noexcept -> MdCodeSpan2Backtick& = delete;
    constexpr auto operator=(MdCodeSpan2Backtick&&) noexcept -> MdCodeSpan2Backtick&;

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
    constexpr MdCodeSpan3Backtick(MdCodeSpan3Backtick const&) noexcept = delete;
    constexpr MdCodeSpan3Backtick(MdCodeSpan3Backtick&&) noexcept;
    constexpr ~MdCodeSpan3Backtick() noexcept;
    constexpr auto operator=(MdCodeSpan3Backtick const&) noexcept -> MdCodeSpan3Backtick& = delete;
    constexpr auto operator=(MdCodeSpan3Backtick&&) noexcept -> MdCodeSpan3Backtick&;

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
    constexpr MdSingleEmphasisAsterisk(MdSingleEmphasisAsterisk const&) noexcept = delete;
    constexpr MdSingleEmphasisAsterisk(MdSingleEmphasisAsterisk&&) noexcept;
    constexpr ~MdSingleEmphasisAsterisk() noexcept;
    constexpr auto operator=(MdSingleEmphasisAsterisk const&) noexcept -> MdSingleEmphasisAsterisk& = delete;
    constexpr auto operator=(MdSingleEmphasisAsterisk&&) noexcept -> MdSingleEmphasisAsterisk&;

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
    constexpr MdDoubleEmphasisAsterisk(MdDoubleEmphasisAsterisk const&) noexcept = delete;
    constexpr MdDoubleEmphasisAsterisk(MdDoubleEmphasisAsterisk&&) noexcept;
    constexpr ~MdDoubleEmphasisAsterisk() noexcept;
    constexpr auto operator=(MdDoubleEmphasisAsterisk const&) noexcept -> MdDoubleEmphasisAsterisk& = delete;
    constexpr auto operator=(MdDoubleEmphasisAsterisk&&) noexcept -> MdDoubleEmphasisAsterisk&;

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
    constexpr MdTripleEmphasisAsterisk(MdTripleEmphasisAsterisk const&) noexcept = delete;
    constexpr MdTripleEmphasisAsterisk(MdTripleEmphasisAsterisk&&) noexcept;
    constexpr ~MdTripleEmphasisAsterisk() noexcept;
    constexpr auto operator=(MdTripleEmphasisAsterisk const&) noexcept -> MdTripleEmphasisAsterisk& = delete;
    constexpr auto operator=(MdTripleEmphasisAsterisk&&) noexcept -> MdTripleEmphasisAsterisk&;

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
    constexpr MdSingleEmphasisUnderscore(MdSingleEmphasisUnderscore const&) noexcept = delete;
    constexpr MdSingleEmphasisUnderscore(MdSingleEmphasisUnderscore&&) noexcept;
    constexpr ~MdSingleEmphasisUnderscore() noexcept;
    constexpr auto operator=(MdSingleEmphasisUnderscore const&) noexcept -> MdSingleEmphasisUnderscore& = delete;
    constexpr auto operator=(MdSingleEmphasisUnderscore&&) noexcept -> MdSingleEmphasisUnderscore&;

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
    constexpr MdDoubleEmphasisUnderscore(MdDoubleEmphasisUnderscore const&) noexcept = delete;
    constexpr MdDoubleEmphasisUnderscore(MdDoubleEmphasisUnderscore&&) noexcept;
    constexpr ~MdDoubleEmphasisUnderscore() noexcept;
    constexpr auto operator=(MdDoubleEmphasisUnderscore const&) noexcept -> MdDoubleEmphasisUnderscore& = delete;
    constexpr auto operator=(MdDoubleEmphasisUnderscore&&) noexcept -> MdDoubleEmphasisUnderscore&;

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
    constexpr MdTripleEmphasisUnderscore(MdTripleEmphasisUnderscore const&) noexcept = delete;
    constexpr MdTripleEmphasisUnderscore(MdTripleEmphasisUnderscore&&) noexcept;
    constexpr ~MdTripleEmphasisUnderscore() noexcept;
    constexpr auto operator=(MdTripleEmphasisUnderscore const&) noexcept -> MdTripleEmphasisUnderscore& = delete;
    constexpr auto operator=(MdTripleEmphasisUnderscore&&) noexcept -> MdTripleEmphasisUnderscore&;

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
    constexpr MdDel(MdDel const&) noexcept = delete;
    constexpr MdDel(MdDel&&) noexcept;
    constexpr ~MdDel() noexcept;
    constexpr auto operator=(MdDel const&) noexcept -> MdDel& = delete;
    constexpr auto operator=(MdDel&&) noexcept -> MdDel&;

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
    constexpr MdLink(MdLink const&) noexcept = delete;
    constexpr MdLink(MdLink&&) noexcept;
    constexpr ~MdLink() noexcept;
    constexpr auto operator=(MdLink const&) noexcept -> MdLink& = delete;
    constexpr auto operator=(MdLink&&) noexcept -> MdLink&;

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
    constexpr MdImage(MdImage const&) noexcept = delete;
    constexpr MdImage(MdImage&&) noexcept;
    constexpr ~MdImage() noexcept;
    constexpr auto operator=(MdImage const&) noexcept -> MdImage& = delete;
    constexpr auto operator=(MdImage&&) noexcept -> MdImage&;

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
    constexpr MdBlockQuotes(MdBlockQuotes const&) noexcept = delete;
    constexpr MdBlockQuotes(MdBlockQuotes&&) noexcept;
    constexpr ~MdBlockQuotes() noexcept;
    constexpr auto operator=(MdBlockQuotes const&) noexcept -> MdBlockQuotes& = delete;
    constexpr auto operator=(MdBlockQuotes&&) noexcept -> MdBlockQuotes&;

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
    constexpr MdUl(MdUl const&) noexcept = delete;
    constexpr MdUl(MdUl&&) noexcept;
    constexpr ~MdUl() noexcept;
    constexpr auto operator=(MdUl const&) noexcept -> MdUl& = delete;
    constexpr auto operator=(MdUl&&) noexcept -> MdUl&;

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
    constexpr MdOl(MdOl const&) noexcept = delete;
    constexpr MdOl(MdOl&&) noexcept;
    constexpr ~MdOl() noexcept;
    constexpr auto operator=(MdOl const&) noexcept -> MdOl& = delete;
    constexpr auto operator=(MdOl&&) noexcept -> MdOl&;

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
    constexpr MdLi(MdLi const&) noexcept = delete;
    constexpr MdLi(MdLi&&) noexcept;
    constexpr ~MdLi() noexcept;
    constexpr auto operator=(MdLi const&) noexcept -> MdLi& = delete;
    constexpr auto operator=(MdLi&&) noexcept -> MdLi&;

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
    constexpr MdLatexInline(MdLatexInline const&) noexcept = delete;
    constexpr MdLatexInline(MdLatexInline&&) noexcept;
    constexpr ~MdLatexInline() noexcept;
    constexpr auto operator=(MdLatexInline const&) noexcept -> MdLatexInline& = delete;
    constexpr auto operator=(MdLatexInline&&) noexcept -> MdLatexInline&;

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
    constexpr MdLatexBlock(MdLatexBlock const&) noexcept = delete;
    constexpr MdLatexBlock(MdLatexBlock&&) noexcept;
    constexpr ~MdLatexBlock() noexcept;
    constexpr auto operator=(MdLatexBlock const&) noexcept -> MdLatexBlock& = delete;
    constexpr auto operator=(MdLatexBlock&&) noexcept -> MdLatexBlock&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

} // namespace pltxt2htm::ast2
