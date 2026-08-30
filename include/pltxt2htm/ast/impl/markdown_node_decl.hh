/**
 * @file markdown_node_decl.hh
 * @brief Markdown-specific AST node declarations for pltxt2htm
 * @details Defines nodes for Markdown syntax: ATX headings, escaped characters,
 *          thematic breaks, code fences/spans, emphasis, strikethrough, links,
 *          images, block quotes, lists, and LaTeX.
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include "ast_decl.hh"
#include "basic_node_decl.hh"
#include "../node_kind.hh"

namespace pltxt2htm {

/**
 * @brief Markdown ATX heading level 1
 * @details Represents a heading started with '# '.
 */
template<::pltxt2htm::Contracts ndebug>
class MdAtxH1 {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1<ndebug> const&) noexcept;
    constexpr MdAtxH1(::pltxt2htm::MdAtxH1<ndebug>&&) noexcept;
    constexpr ~MdAtxH1() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH1<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH1<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH1<ndebug>& self, ::pltxt2htm::MdAtxH1<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2<ndebug> const&) noexcept;
    constexpr MdAtxH2(::pltxt2htm::MdAtxH2<ndebug>&&) noexcept;
    constexpr ~MdAtxH2() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH2<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH2<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH2<ndebug>& self, ::pltxt2htm::MdAtxH2<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3<ndebug> const&) noexcept;
    constexpr MdAtxH3(::pltxt2htm::MdAtxH3<ndebug>&&) noexcept;
    constexpr ~MdAtxH3() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH3<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH3<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH3<ndebug>& self, ::pltxt2htm::MdAtxH3<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4<ndebug> const&) noexcept;
    constexpr MdAtxH4(::pltxt2htm::MdAtxH4<ndebug>&&) noexcept;
    constexpr ~MdAtxH4() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH4<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH4<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH4<ndebug>& self, ::pltxt2htm::MdAtxH4<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5<ndebug> const&) noexcept;
    constexpr MdAtxH5(::pltxt2htm::MdAtxH5<ndebug>&&) noexcept;
    constexpr ~MdAtxH5() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH5<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH5<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH5<ndebug>& self, ::pltxt2htm::MdAtxH5<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdAtxH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6<ndebug> const&) noexcept;
    constexpr MdAtxH6(::pltxt2htm::MdAtxH6<ndebug>&&) noexcept;
    constexpr ~MdAtxH6() noexcept;
    constexpr auto operator=(::pltxt2htm::MdAtxH6<ndebug> const&) noexcept -> ::pltxt2htm::MdAtxH6<ndebug>& = delete;
    constexpr auto operator=(this MdAtxH6<ndebug>& self, ::pltxt2htm::MdAtxH6<ndebug>&&) noexcept
        -> ::pltxt2htm::MdAtxH6<ndebug>&;

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
    constexpr auto operator==(this MdHr const&, MdHr const&) noexcept -> bool = default;
};

/**
 * @brief Markdown code span delimited by a single backtick
 */
template<::pltxt2htm::Contracts ndebug>
class MdCodeSpan1Backtick {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdCodeSpan1Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick<ndebug> const&) noexcept;
    constexpr MdCodeSpan1Backtick(::pltxt2htm::MdCodeSpan1Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan1Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan1Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan1Backtick<ndebug>& = delete;
    constexpr auto operator=(this MdCodeSpan1Backtick<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdCodeSpan2Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick<ndebug> const&) noexcept;
    constexpr MdCodeSpan2Backtick(::pltxt2htm::MdCodeSpan2Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan2Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan2Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan2Backtick<ndebug>& = delete;
    constexpr auto operator=(this MdCodeSpan2Backtick<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdCodeSpan3Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick<ndebug> const&) noexcept;
    constexpr MdCodeSpan3Backtick(::pltxt2htm::MdCodeSpan3Backtick<ndebug>&&) noexcept;
    constexpr ~MdCodeSpan3Backtick() noexcept;
    constexpr auto operator=(::pltxt2htm::MdCodeSpan3Backtick<ndebug> const&) noexcept
        -> ::pltxt2htm::MdCodeSpan3Backtick<ndebug>& = delete;
    constexpr auto operator=(this MdCodeSpan3Backtick<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdSingleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> const&) noexcept;
    constexpr MdSingleEmphasisAsterisk(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this MdSingleEmphasisAsterisk<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdDoubleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept;
    constexpr MdDoubleEmphasisAsterisk(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this MdDoubleEmphasisAsterisk<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdTripleEmphasisAsterisk(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> const&) noexcept;
    constexpr MdTripleEmphasisAsterisk(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisAsterisk() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisAsterisk<ndebug> const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>& = delete;
    constexpr auto operator=(this MdTripleEmphasisAsterisk<ndebug>& self,
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdSingleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> const&) noexcept;
    constexpr MdSingleEmphasisUnderscore(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdSingleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this MdSingleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class MdDoubleEmphasisUnderscore {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdDoubleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept;
    constexpr MdDoubleEmphasisUnderscore(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdDoubleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this MdDoubleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class MdTripleEmphasisUnderscore {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdTripleEmphasisUnderscore(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> const&) noexcept;
    constexpr MdTripleEmphasisUnderscore(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept;
    constexpr ~MdTripleEmphasisUnderscore() noexcept;
    constexpr auto operator=(::pltxt2htm::MdTripleEmphasisUnderscore<ndebug> const&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>& = delete;
    constexpr auto operator=(this MdTripleEmphasisUnderscore<ndebug>& self,
                             ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept
        -> ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class MdDel {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdDel(::pltxt2htm::MdDel<ndebug> const&) noexcept;
    constexpr MdDel(::pltxt2htm::MdDel<ndebug>&&) noexcept;
    constexpr ~MdDel() noexcept;
    constexpr auto operator=(::pltxt2htm::MdDel<ndebug> const&) noexcept -> ::pltxt2htm::MdDel<ndebug>& = delete;
    constexpr auto operator=(this MdDel<ndebug>& self, ::pltxt2htm::MdDel<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::Url url;

public:
    /**
     * @brief Construct a link.
     * @param subast The link text/content AST.
     * @param url The target URL.
     */
    constexpr explicit MdLink(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr MdLink(::pltxt2htm::MdLink<ndebug> const&) noexcept;
    constexpr MdLink(::pltxt2htm::MdLink<ndebug>&&) noexcept;
    constexpr ~MdLink() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLink<ndebug> const&) noexcept -> ::pltxt2htm::MdLink<ndebug>& = delete;
    constexpr auto operator=(this MdLink<ndebug>& self, ::pltxt2htm::MdLink<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::Url url;

public:
    /**
     * @brief Construct an image.
     * @param subast The alt text/content AST.
     * @param url The image source URL.
     */
    constexpr explicit MdImage(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_) noexcept;
    constexpr MdImage(::pltxt2htm::MdImage<ndebug> const&) noexcept;
    constexpr MdImage(::pltxt2htm::MdImage<ndebug>&&) noexcept;
    constexpr ~MdImage() noexcept;
    constexpr auto operator=(::pltxt2htm::MdImage<ndebug> const&) noexcept -> ::pltxt2htm::MdImage<ndebug>& = delete;
    constexpr auto operator=(this MdImage<ndebug>& self, ::pltxt2htm::MdImage<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdBlockQuotes(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes<ndebug> const&) noexcept;
    constexpr MdBlockQuotes(::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept;
    constexpr ~MdBlockQuotes() noexcept;
    constexpr auto operator=(::pltxt2htm::MdBlockQuotes<ndebug> const&) noexcept
        -> ::pltxt2htm::MdBlockQuotes<ndebug>& = delete;
    constexpr auto operator=(this MdBlockQuotes<ndebug>& self, ::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept
        -> ::pltxt2htm::MdBlockQuotes<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class MdLatexInline {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdLatexInline(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline<ndebug> const&) noexcept;
    constexpr MdLatexInline(::pltxt2htm::MdLatexInline<ndebug>&&) noexcept;
    constexpr ~MdLatexInline() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLatexInline<ndebug> const&) noexcept
        -> ::pltxt2htm::MdLatexInline<ndebug>& = delete;
    constexpr auto operator=(this MdLatexInline<ndebug>& self, ::pltxt2htm::MdLatexInline<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLatexInline<ndebug>&;

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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit MdLatexBlock(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock<ndebug> const&) noexcept;
    constexpr MdLatexBlock(::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept;
    constexpr ~MdLatexBlock() noexcept;
    constexpr auto operator=(::pltxt2htm::MdLatexBlock<ndebug> const&) noexcept
        -> ::pltxt2htm::MdLatexBlock<ndebug>& = delete;
    constexpr auto operator=(this MdLatexBlock<ndebug>& self, ::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept
        -> ::pltxt2htm::MdLatexBlock<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this MdLatexBlock const&, MdLatexBlock const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm
