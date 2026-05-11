/**
 * @file html_node_decl.hh
 * @brief HTML-like AST node declarations for pltxt2htm
 * @details Defines nodes corresponding to HTML tags and special characters
 *          (line breaks, headings, lists, formatting, etc.)
 */

#pragma once

#include <utility>
#include "ast_decl.hh"

namespace pltxt2htm::ast2 {

/**
 * @brief Line break (newline) node
 * @details Represents a line break character in the source text.
 */
class LineBreak {};

/**
 * @brief HTML &lt;br&gt; tag node
 * @details Represents a forced line break in HTML output.
 */
class Br {};

/**
 * @brief Space character node
 * @details Represents a whitespace character (space, tab, etc.).
 */
class Space {};

/**
 * @brief Less-than sign node
 * @details Represents the '<' character, which may need escaping in HTML.
 */
class LessThan {};

/**
 * @brief Greater-than sign node
 * @details Represents the '>' character.
 */
class GreaterThan {};

/**
 * @brief Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {};

/**
 * @brief Ampersand node
 * @details Represents the '&' character, which may be part of an HTML entity.
 */
class Ampersand {};

/**
 * @brief Single quotation mark node
 * @details Represents the "'" character.
 */
class SingleQuotationMark {};

/**
 * @brief Double quotation mark node
 * @details Represents the '"' character.
 */
class DoubleQuotationMark {};

/**
 * @brief HTML &lt;hr&gt; tag node
 * @details Represents a horizontal rule.
 */
class Hr {};

/**
 * @brief HTML &lt;h1&gt; heading node
 * @details Represents a level‑1 heading containing sub‑AST content.
 */
template<::pltxt2htm::Contracts ndebug>
class H1 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H1() noexcept = delete;
    constexpr explicit H1(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H1(H1 const&) noexcept = delete;
    constexpr H1(H1&&) noexcept;
    constexpr ~H1() noexcept;
    constexpr auto operator=(H1 const&) noexcept -> H1& = delete;
    constexpr auto operator=(H1&&) noexcept -> H1&;

    /**
     * @brief Get the sub‑AST contained in this heading.
     * @return Reference to the sub‑AST (modifiable if this is an lvalue).
     */
    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;h2&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H2 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H2() noexcept = delete;
    constexpr explicit H2(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H2(H2 const&) noexcept = delete;
    constexpr H2(H2&&) noexcept;
    constexpr ~H2() noexcept;
    constexpr auto operator=(H2 const&) noexcept -> H2& = delete;
    constexpr auto operator=(H2&&) noexcept -> H2&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;h3&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H3 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H3() noexcept = delete;
    constexpr explicit H3(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H3(H3 const&) noexcept = delete;
    constexpr H3(H3&&) noexcept;
    constexpr ~H3() noexcept;
    constexpr auto operator=(H3 const&) noexcept -> H3& = delete;
    constexpr auto operator=(H3&&) noexcept -> H3&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;h4&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H4 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H4() noexcept = delete;
    constexpr explicit H4(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H4(H4 const&) noexcept = delete;
    constexpr H4(H4&&) noexcept;
    constexpr ~H4() noexcept;
    constexpr auto operator=(H4 const&) noexcept -> H4& = delete;
    constexpr auto operator=(H4&&) noexcept -> H4&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;h5&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H5 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H5() noexcept = delete;
    constexpr explicit H5(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H5(H5 const&) noexcept = delete;
    constexpr H5(H5&&) noexcept;
    constexpr ~H5() noexcept;
    constexpr auto operator=(H5 const&) noexcept -> H5& = delete;
    constexpr auto operator=(H5&&) noexcept -> H5&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;h6&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H6 {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr H6() noexcept = delete;
    constexpr explicit H6(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr H6(H6 const&) noexcept = delete;
    constexpr H6(H6&&) noexcept;
    constexpr ~H6() noexcept;
    constexpr auto operator=(H6 const&) noexcept -> H6& = delete;
    constexpr auto operator=(H6&&) noexcept -> H6&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;p&gt; paragraph node
 */
template<::pltxt2htm::Contracts ndebug>
class P {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr P() noexcept = delete;
    constexpr explicit P(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr P(P const&) noexcept = delete;
    constexpr P(P&&) noexcept;
    constexpr ~P() noexcept;
    constexpr auto operator=(P const&) noexcept -> P& = delete;
    constexpr auto operator=(P&&) noexcept -> P&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;del&gt; deleted text node
 */
template<::pltxt2htm::Contracts ndebug>
class Del {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Del() noexcept = delete;
    constexpr explicit Del(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Del(Del const&) noexcept = delete;
    constexpr Del(Del&&) noexcept;
    constexpr ~Del() noexcept;
    constexpr auto operator=(Del const&) noexcept -> Del& = delete;
    constexpr auto operator=(Del&&) noexcept -> Del&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;note&gt; custom note node
 */
template<::pltxt2htm::Contracts ndebug>
class Note {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Note() noexcept = delete;
    constexpr explicit Note(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Note(Note const&) noexcept = delete;
    constexpr Note(Note&&) noexcept;
    constexpr ~Note() noexcept;
    constexpr auto operator=(Note const&) noexcept -> Note& = delete;
    constexpr auto operator=(Note&&) noexcept -> Note&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;em&gt; emphasis node
 */
template<::pltxt2htm::Contracts ndebug>
class Em {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Em() noexcept = delete;
    constexpr explicit Em(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Em(Em const&) noexcept = delete;
    constexpr Em(Em&&) noexcept;
    constexpr ~Em() noexcept;
    constexpr auto operator=(Em const&) noexcept -> Em& = delete;
    constexpr auto operator=(Em&&) noexcept -> Em&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;strong&gt; strong importance node
 */
template<::pltxt2htm::Contracts ndebug>
class Strong {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Strong() noexcept = delete;
    constexpr explicit Strong(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Strong(Strong const&) noexcept = delete;
    constexpr Strong(Strong&&) noexcept;
    constexpr ~Strong() noexcept;
    constexpr auto operator=(Strong const&) noexcept -> Strong& = delete;
    constexpr auto operator=(Strong&&) noexcept -> Strong&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;ul&gt; unordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class Ul {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Ul() noexcept = delete;
    constexpr explicit Ul(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Ul(Ul const&) noexcept = delete;
    constexpr Ul(Ul&&) noexcept;
    constexpr ~Ul() noexcept;
    constexpr auto operator=(Ul const&) noexcept -> Ul& = delete;
    constexpr auto operator=(Ul&&) noexcept -> Ul&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;ol&gt; ordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class Ol {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Ol() noexcept = delete;
    constexpr explicit Ol(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Ol(Ol const&) noexcept = delete;
    constexpr Ol(Ol&&) noexcept;
    constexpr ~Ol() noexcept;
    constexpr auto operator=(Ol const&) noexcept -> Ol& = delete;
    constexpr auto operator=(Ol&&) noexcept -> Ol&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;li&gt; list item node
 */
template<::pltxt2htm::Contracts ndebug>
class Li {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Li() noexcept = delete;
    constexpr explicit Li(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Li(Li const&) noexcept = delete;
    constexpr Li(Li&&) noexcept;
    constexpr ~Li() noexcept;
    constexpr auto operator=(Li const&) noexcept -> Li& = delete;
    constexpr auto operator=(Li&&) noexcept -> Li&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;code&gt; inline code node
 */
template<::pltxt2htm::Contracts ndebug>
class Code {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Code() noexcept = delete;
    constexpr explicit Code(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Code(Code const&) noexcept = delete;
    constexpr Code(Code&&) noexcept;
    constexpr ~Code() noexcept;
    constexpr auto operator=(Code const&) noexcept -> Code& = delete;
    constexpr auto operator=(Code&&) noexcept -> Code&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;pre&gt; preformatted text node
 */
template<::pltxt2htm::Contracts ndebug>
class Pre {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Pre() noexcept = delete;
    constexpr explicit Pre(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Pre(Pre const&) noexcept = delete;
    constexpr Pre(Pre&&) noexcept;
    constexpr ~Pre() noexcept;
    constexpr auto operator=(Pre const&) noexcept -> Pre& = delete;
    constexpr auto operator=(Pre&&) noexcept -> Pre&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

/**
 * @brief HTML &lt;blockquote&gt; block quotation node
 */
template<::pltxt2htm::Contracts ndebug>
class Blockquote {
    ::pltxt2htm::ast2::Ast<ndebug> subast_{};

public:
    constexpr Blockquote() noexcept = delete;
    constexpr explicit Blockquote(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Blockquote(Blockquote const&) noexcept = delete;
    constexpr Blockquote(Blockquote&&) noexcept;
    constexpr ~Blockquote() noexcept;
    constexpr auto operator=(Blockquote const&) noexcept -> Blockquote& = delete;
    constexpr auto operator=(Blockquote&&) noexcept -> Blockquote&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast_);
    }
};

} // namespace pltxt2htm::ast2
