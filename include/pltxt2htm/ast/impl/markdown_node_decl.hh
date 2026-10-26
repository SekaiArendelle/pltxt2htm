/**
 * @file markdown_node_decl.hh
 * @brief Markdown-specific AST node declarations for pltxt2htm
 * @details Defines nodes for Markdown syntax: ATX headings, escaped characters,
 *          thematic breaks, code fences/spans, emphasis, strikethrough, links,
 *          images, block quotes, lists, and LaTeX.
 */

#pragma once

#include <utility>
#include "../../container/string.hh"
#include "ast_decl.hh"
#include "url_node_decl.hh"
#include "../plain_text.hh"
#include "../node_kind.hh"

namespace pltxt2htm {

/**
 * @brief Markdown ATX heading level 1
 * @details Represents a heading started with '# '.
 */
class MdAtxH1 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH1(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1 const&) noexcept = default;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1&&) noexcept;
    constexpr ~MdAtxH1() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH1 const&) noexcept -> ::pltxt2htm::MdAtxH1& = default;
    constexpr auto operator=(this MdAtxH1& self, ::pltxt2htm::MdAtxH1&&) noexcept -> ::pltxt2htm::MdAtxH1&;

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
class MdAtxH2 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH2(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2 const&) noexcept = default;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2&&) noexcept;
    constexpr ~MdAtxH2() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH2 const&) noexcept -> ::pltxt2htm::MdAtxH2& = default;
    constexpr auto operator=(this MdAtxH2& self, ::pltxt2htm::MdAtxH2&&) noexcept -> ::pltxt2htm::MdAtxH2&;

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
class MdAtxH3 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH3(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3 const&) noexcept = default;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3&&) noexcept;
    constexpr ~MdAtxH3() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH3 const&) noexcept -> ::pltxt2htm::MdAtxH3& = default;
    constexpr auto operator=(this MdAtxH3& self, ::pltxt2htm::MdAtxH3&&) noexcept -> ::pltxt2htm::MdAtxH3&;

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
class MdAtxH4 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH4(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4 const&) noexcept = default;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4&&) noexcept;
    constexpr ~MdAtxH4() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH4 const&) noexcept -> ::pltxt2htm::MdAtxH4& = default;
    constexpr auto operator=(this MdAtxH4& self, ::pltxt2htm::MdAtxH4&&) noexcept -> ::pltxt2htm::MdAtxH4&;

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
class MdAtxH5 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH5(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5 const&) noexcept = default;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5&&) noexcept;
    constexpr ~MdAtxH5() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH5 const&) noexcept -> ::pltxt2htm::MdAtxH5& = default;
    constexpr auto operator=(this MdAtxH5& self, ::pltxt2htm::MdAtxH5&&) noexcept -> ::pltxt2htm::MdAtxH5&;

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
class MdAtxH6 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdAtxH6(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6 const&) noexcept = default;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6&&) noexcept;
    constexpr ~MdAtxH6() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdAtxH6 const&) noexcept -> ::pltxt2htm::MdAtxH6& = default;
    constexpr auto operator=(this MdAtxH6& self, ::pltxt2htm::MdAtxH6&&) noexcept -> ::pltxt2htm::MdAtxH6&;

    [[nodiscard]]
    constexpr auto operator==(this MdAtxH6 const&, MdAtxH6 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown backslash escape.
 * @details Stores the escaped ASCII punctuation character while preserving that
 *          it originated from Markdown backslash syntax.
 */
class MdEscape {
    char8_t character;

public:
    constexpr explicit MdEscape(char8_t character_) noexcept
        : character{character_} {
    }

    [[nodiscard]]
    constexpr auto operator==(this MdEscape const&, MdEscape const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto get_character(this MdEscape const& self) noexcept -> char8_t {
        return self.character;
    }
};

/**
 * @brief Markdown thematic break (horizontal rule)
 * @details Represents '---', '***', or '___' lines.
 */
class MdHr {
public:
    [[nodiscard]]
    constexpr auto operator==(this MdHr const&, MdHr const&) noexcept -> bool = default;
};

/**
 * @brief Markdown code span delimited by a single backtick
 */
class MdCodeSpan1Backtick {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdCodeSpan1Backtick(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick const&) noexcept = default;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick&&) noexcept;
    constexpr ~MdCodeSpan1Backtick() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan1Backtick const&) noexcept
        -> ::pltxt2htm::MdCodeSpan1Backtick& = default;
    constexpr auto operator=(this MdCodeSpan1Backtick& self, ::pltxt2htm::MdCodeSpan1Backtick&&) noexcept
        -> ::pltxt2htm::MdCodeSpan1Backtick&;

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
class MdCodeSpan2Backtick {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdCodeSpan2Backtick(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick const&) noexcept = default;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick&&) noexcept;
    constexpr ~MdCodeSpan2Backtick() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan2Backtick const&) noexcept
        -> ::pltxt2htm::MdCodeSpan2Backtick& = default;
    constexpr auto operator=(this MdCodeSpan2Backtick& self, ::pltxt2htm::MdCodeSpan2Backtick&&) noexcept
        -> ::pltxt2htm::MdCodeSpan2Backtick&;

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
class MdCodeSpan3Backtick {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdCodeSpan3Backtick(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept = default;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick&&) noexcept;
    constexpr ~MdCodeSpan3Backtick() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan3Backtick const&) noexcept
        -> ::pltxt2htm::MdCodeSpan3Backtick& = default;
    constexpr auto operator=(this MdCodeSpan3Backtick& self, ::pltxt2htm::MdCodeSpan3Backtick&&) noexcept
        -> ::pltxt2htm::MdCodeSpan3Backtick&;

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
class MdSingleEmphasisAsterisk {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdSingleEmphasisAsterisk(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk const&) noexcept = default;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept;
    constexpr ~MdSingleEmphasisAsterisk() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisAsterisk const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisAsterisk& = default;
    constexpr auto operator=(this MdSingleEmphasisAsterisk& self, ::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisAsterisk&;

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
class MdDoubleEmphasisAsterisk {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdDoubleEmphasisAsterisk(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk const&) noexcept = default;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept;
    constexpr ~MdDoubleEmphasisAsterisk() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisAsterisk const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisAsterisk& = default;
    constexpr auto operator=(this MdDoubleEmphasisAsterisk& self, ::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisAsterisk&;

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
class MdTripleEmphasisAsterisk {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdTripleEmphasisAsterisk(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk const&) noexcept = default;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept;
    constexpr ~MdTripleEmphasisAsterisk() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisAsterisk const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisAsterisk& = default;
    constexpr auto operator=(this MdTripleEmphasisAsterisk& self, ::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisAsterisk&;

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
class MdSingleEmphasisUnderscore {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdSingleEmphasisUnderscore(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore const&) noexcept = default;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept;
    constexpr ~MdSingleEmphasisUnderscore() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisUnderscore const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore& = default;
    constexpr auto operator=(this MdSingleEmphasisUnderscore& self, ::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore&;

    [[nodiscard]]
    constexpr auto operator==(this MdSingleEmphasisUnderscore const&, MdSingleEmphasisUnderscore const&) noexcept
        -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Double underscore emphasis (e.g., __bold__)
 */
class MdDoubleEmphasisUnderscore {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdDoubleEmphasisUnderscore(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore const&) noexcept = default;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept;
    constexpr ~MdDoubleEmphasisUnderscore() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisUnderscore const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore& = default;
    constexpr auto operator=(this MdDoubleEmphasisUnderscore& self, ::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore&;

    [[nodiscard]]
    constexpr auto operator==(this MdDoubleEmphasisUnderscore const&, MdDoubleEmphasisUnderscore const&) noexcept
        -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Triple underscore emphasis (e.g., ___bold italic___)
 */
class MdTripleEmphasisUnderscore {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdTripleEmphasisUnderscore(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore const&) noexcept = default;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept;
    constexpr ~MdTripleEmphasisUnderscore() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisUnderscore const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore& = default;
    constexpr auto operator=(this MdTripleEmphasisUnderscore& self, ::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore&;

    [[nodiscard]]
    constexpr auto operator==(this MdTripleEmphasisUnderscore const&, MdTripleEmphasisUnderscore const&) noexcept
        -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown strikethrough (e.g., ~~deleted~~)
 */
class MdDel {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdDel(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdDel(::pltxt2htm::MdDel const&) noexcept = default;
    constexpr MdDel(::pltxt2htm::MdDel&&) noexcept;
    constexpr ~MdDel() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdDel const&) noexcept -> ::pltxt2htm::MdDel& = default;
    constexpr auto operator=(this MdDel& self, ::pltxt2htm::MdDel&&) noexcept -> ::pltxt2htm::MdDel&;

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
class MdLink {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::Url url;

public:
    /**
     * @brief Construct a link.
     * @param subast The link text/content AST.
     * @param url The target URL.
     */
    constexpr explicit MdLink(::pltxt2htm::Ast&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr MdLink(::pltxt2htm::MdLink const&) noexcept = default;
    constexpr MdLink(::pltxt2htm::MdLink&&) noexcept;
    constexpr ~MdLink() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdLink const&) noexcept -> ::pltxt2htm::MdLink& = default;
    constexpr auto operator=(this MdLink& self, ::pltxt2htm::MdLink&&) noexcept -> ::pltxt2htm::MdLink&;

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
class MdImage {
    ::pltxt2htm::PlainText alt;
    ::pltxt2htm::Url url;

public:
    /**
     * @brief Construct an image.
     * @param alt The parsed alt text.
     * @param url The image source URL.
     */
    constexpr explicit MdImage(::pltxt2htm::PlainText&& alt_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr MdImage(::pltxt2htm::MdImage const&) noexcept;
    constexpr MdImage(::pltxt2htm::MdImage&&) noexcept;
    constexpr ~MdImage() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdImage const&) noexcept -> ::pltxt2htm::MdImage& = default;
    constexpr auto operator=(this MdImage& self, ::pltxt2htm::MdImage&&) noexcept -> ::pltxt2htm::MdImage&;

    [[nodiscard]]
    constexpr auto operator==(this MdImage const&, MdImage const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_alt(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.alt);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }
};

/**
 * @brief Markdown block quote (lines starting with '>')
 */
class MdBlockQuotes {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdBlockQuotes(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes const&) noexcept = default;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes&&) noexcept;
    constexpr ~MdBlockQuotes() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdBlockQuotes const&) noexcept -> ::pltxt2htm::MdBlockQuotes& = default;
    constexpr auto operator=(this MdBlockQuotes& self, ::pltxt2htm::MdBlockQuotes&&) noexcept
        -> ::pltxt2htm::MdBlockQuotes&;

    [[nodiscard]]
    constexpr auto operator==(this MdBlockQuotes const&, MdBlockQuotes const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Inline LaTeX math (delimited by $...$)
 */
class MdLatexInline {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdLatexInline(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline const&) noexcept = default;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline&&) noexcept;
    constexpr ~MdLatexInline() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdLatexInline const&) noexcept -> ::pltxt2htm::MdLatexInline& = default;
    constexpr auto operator=(this MdLatexInline& self, ::pltxt2htm::MdLatexInline&&) noexcept
        -> ::pltxt2htm::MdLatexInline&;

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
class MdLatexBlock {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit MdLatexBlock(::pltxt2htm::Ast&& subast_) noexcept;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock const&) noexcept = default;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock&&) noexcept;
    constexpr ~MdLatexBlock() noexcept = default;
    constexpr auto operator=(::pltxt2htm::MdLatexBlock const&) noexcept -> ::pltxt2htm::MdLatexBlock& = default;
    constexpr auto operator=(this MdLatexBlock& self, ::pltxt2htm::MdLatexBlock&&) noexcept
        -> ::pltxt2htm::MdLatexBlock&;

    [[nodiscard]]
    constexpr auto operator==(this MdLatexBlock const&, MdLatexBlock const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm
