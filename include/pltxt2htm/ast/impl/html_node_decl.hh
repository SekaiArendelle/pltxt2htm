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
 * @brief ::pltxt2htm::ast2::Space character node
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
 * @brief ::pltxt2htm::ast2::Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {};

/**
 * @brief ::pltxt2htm::ast2::Ampersand node
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
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H1() noexcept = delete;
    constexpr explicit H1(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H1(::pltxt2htm::ast2::H1<ndebug> const&) noexcept = delete;
    constexpr H1(::pltxt2htm::ast2::H1<ndebug>&&) noexcept;
    constexpr ~H1() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H1<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H1<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H1<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H1<ndebug>&;

    /**
     * @brief Get the sub‑AST contained in this heading.
     * @return Reference to the sub‑AST (modifiable if this is an lvalue).
     */
    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h2&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H2 {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H2() noexcept = delete;
    constexpr explicit H2(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H2(::pltxt2htm::ast2::H2<ndebug> const&) noexcept = delete;
    constexpr H2(::pltxt2htm::ast2::H2<ndebug>&&) noexcept;
    constexpr ~H2() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H2<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H2<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H2<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H2<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h3&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H3 {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H3() noexcept = delete;
    constexpr explicit H3(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H3(::pltxt2htm::ast2::H3<ndebug> const&) noexcept = delete;
    constexpr H3(::pltxt2htm::ast2::H3<ndebug>&&) noexcept;
    constexpr ~H3() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H3<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H3<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H3<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H3<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h4&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H4 {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H4() noexcept = delete;
    constexpr explicit H4(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H4(::pltxt2htm::ast2::H4<ndebug> const&) noexcept = delete;
    constexpr H4(::pltxt2htm::ast2::H4<ndebug>&&) noexcept;
    constexpr ~H4() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H4<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H4<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H4<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H4<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h5&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H5 {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H5() noexcept = delete;
    constexpr explicit H5(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H5(::pltxt2htm::ast2::H5<ndebug> const&) noexcept = delete;
    constexpr H5(::pltxt2htm::ast2::H5<ndebug>&&) noexcept;
    constexpr ~H5() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H5<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H5<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H5<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H5<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h6&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class H6 {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr H6() noexcept = delete;
    constexpr explicit H6(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr H6(::pltxt2htm::ast2::H6<ndebug> const&) noexcept = delete;
    constexpr H6(::pltxt2htm::ast2::H6<ndebug>&&) noexcept;
    constexpr ~H6() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::H6<ndebug> const&) noexcept -> ::pltxt2htm::ast2::H6<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::H6<ndebug>&&) noexcept -> ::pltxt2htm::ast2::H6<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;p&gt; paragraph node
 */
template<::pltxt2htm::Contracts ndebug>
class P {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr P() noexcept = delete;
    constexpr explicit P(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr P(::pltxt2htm::ast2::P<ndebug> const&) noexcept = delete;
    constexpr P(::pltxt2htm::ast2::P<ndebug>&&) noexcept;
    constexpr ~P() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::P<ndebug> const&) noexcept -> ::pltxt2htm::ast2::P<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::P<ndebug>&&) noexcept -> ::pltxt2htm::ast2::P<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;del&gt; deleted text node
 */
template<::pltxt2htm::Contracts ndebug>
class Del {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Del() noexcept = delete;
    constexpr explicit Del(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Del(::pltxt2htm::ast2::Del<ndebug> const&) noexcept = delete;
    constexpr Del(::pltxt2htm::ast2::Del<ndebug>&&) noexcept;
    constexpr ~Del() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Del<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Del<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Del<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Del<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;note&gt; custom note node
 */
template<::pltxt2htm::Contracts ndebug>
class Note {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Note() noexcept = delete;
    constexpr explicit Note(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Note(::pltxt2htm::ast2::Note<ndebug> const&) noexcept = delete;
    constexpr Note(::pltxt2htm::ast2::Note<ndebug>&&) noexcept;
    constexpr ~Note() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Note<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Note<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Note<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Note<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;em&gt; emphasis node
 */
template<::pltxt2htm::Contracts ndebug>
class Em {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Em() noexcept = delete;
    constexpr explicit Em(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Em(::pltxt2htm::ast2::Em<ndebug> const&) noexcept = delete;
    constexpr Em(::pltxt2htm::ast2::Em<ndebug>&&) noexcept;
    constexpr ~Em() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Em<ndebug> const&) noexcept -> ::pltxt2htm::ast2::Em<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Em<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Em<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;strong&gt; strong importance node
 */
template<::pltxt2htm::Contracts ndebug>
class Strong {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Strong() noexcept = delete;
    constexpr explicit Strong(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Strong(::pltxt2htm::ast2::Strong<ndebug> const&) noexcept = delete;
    constexpr Strong(::pltxt2htm::ast2::Strong<ndebug>&&) noexcept;
    constexpr ~Strong() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Strong<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Strong<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Strong<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Strong<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;ul&gt; unordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class Ul {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Ul() noexcept = delete;
    constexpr explicit Ul(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Ul(::pltxt2htm::ast2::Ul<ndebug> const&) noexcept = delete;
    constexpr Ul(::pltxt2htm::ast2::Ul<ndebug>&&) noexcept;
    constexpr ~Ul() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Ul<ndebug> const&) noexcept -> ::pltxt2htm::ast2::Ul<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Ul<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Ul<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;ol&gt; ordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class Ol {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Ol() noexcept = delete;
    constexpr explicit Ol(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Ol(::pltxt2htm::ast2::Ol<ndebug> const&) noexcept = delete;
    constexpr Ol(::pltxt2htm::ast2::Ol<ndebug>&&) noexcept;
    constexpr ~Ol() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Ol<ndebug> const&) noexcept -> ::pltxt2htm::ast2::Ol<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Ol<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Ol<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;li&gt; list item node
 */
template<::pltxt2htm::Contracts ndebug>
class Li {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Li() noexcept = delete;
    constexpr explicit Li(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Li(::pltxt2htm::ast2::Li<ndebug> const&) noexcept = delete;
    constexpr Li(::pltxt2htm::ast2::Li<ndebug>&&) noexcept;
    constexpr ~Li() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Li<ndebug> const&) noexcept -> ::pltxt2htm::ast2::Li<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Li<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Li<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;code&gt; inline code node
 */
template<::pltxt2htm::Contracts ndebug>
class Code {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Code() noexcept = delete;
    constexpr explicit Code(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Code(::pltxt2htm::ast2::Code<ndebug> const&) noexcept = delete;
    constexpr Code(::pltxt2htm::ast2::Code<ndebug>&&) noexcept;
    constexpr ~Code() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Code<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Code<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Code<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Code<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;pre&gt; preformatted text node
 */
template<::pltxt2htm::Contracts ndebug>
class Pre {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Pre() noexcept = delete;
    constexpr explicit Pre(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Pre(::pltxt2htm::ast2::Pre<ndebug> const&) noexcept = delete;
    constexpr Pre(::pltxt2htm::ast2::Pre<ndebug>&&) noexcept;
    constexpr ~Pre() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Pre<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Pre<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Pre<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Pre<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;blockquote&gt; block quotation node
 */
template<::pltxt2htm::Contracts ndebug>
class Blockquote {
    ::pltxt2htm::ast2::Ast<ndebug> subast{};

public:
    constexpr Blockquote() noexcept = delete;
    constexpr explicit Blockquote(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept;
    constexpr Blockquote(::pltxt2htm::ast2::Blockquote<ndebug> const&) noexcept = delete;
    constexpr Blockquote(::pltxt2htm::ast2::Blockquote<ndebug>&&) noexcept;
    constexpr ~Blockquote() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Blockquote<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Blockquote<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Blockquote<ndebug>&&) noexcept
        -> ::pltxt2htm::ast2::Blockquote<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm::ast2
