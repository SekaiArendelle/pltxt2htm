/**
 * @file html_node_decl.hh
 * @brief HTML-like AST node declarations for pltxt2htm
 * @details Defines nodes corresponding to HTML tags and special characters
 *          (line breaks, headings, lists, formatting, etc.)
 */

#pragma once

#include <utility>
#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "ast_decl.hh"
#include "../node_kind.hh"

namespace pltxt2htm {

/**
 * @brief HTML &lt;br&gt; tag node
 * @details Represents a forced line break in HTML output.
 */
class HtmlBr {
public:
    constexpr auto operator==(this HtmlBr const&, HtmlBr const&) noexcept -> bool = default;
};

/**
 * @brief HTML &lt;hr&gt; tag node
 * @details Represents a horizontal rule.
 */
class HtmlHr {
public:
    constexpr auto operator==(this HtmlHr const&, HtmlHr const&) noexcept -> bool = default;
};

/**
 * @brief HTML &lt;col&gt; self-closing column node
 */
class HtmlCol {
public:
    constexpr auto operator==(this HtmlCol const&, HtmlCol const&) noexcept -> bool = default;
};

/**
 * @brief HTML &lt;h1&gt; heading node
 * @details Represents a level‑1 heading containing sub‑AST content.
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH1 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH1() noexcept = delete;
    constexpr explicit HtmlH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH1(::pltxt2htm::HtmlH1<ndebug> const&) noexcept;
    constexpr HtmlH1(::pltxt2htm::HtmlH1<ndebug>&&) noexcept;
    constexpr ~HtmlH1() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH1<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH1<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH1<ndebug>& self, ::pltxt2htm::HtmlH1<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH1<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH1 const&, HtmlH1 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h2&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH2 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH2() noexcept = delete;
    constexpr explicit HtmlH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH2(::pltxt2htm::HtmlH2<ndebug> const&) noexcept;
    constexpr HtmlH2(::pltxt2htm::HtmlH2<ndebug>&&) noexcept;
    constexpr ~HtmlH2() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH2<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH2<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH2<ndebug>& self, ::pltxt2htm::HtmlH2<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH2<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH2 const&, HtmlH2 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h3&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH3 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH3() noexcept = delete;
    constexpr explicit HtmlH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH3(::pltxt2htm::HtmlH3<ndebug> const&) noexcept;
    constexpr HtmlH3(::pltxt2htm::HtmlH3<ndebug>&&) noexcept;
    constexpr ~HtmlH3() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH3<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH3<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH3<ndebug>& self, ::pltxt2htm::HtmlH3<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH3<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH3 const&, HtmlH3 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h4&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH4 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH4() noexcept = delete;
    constexpr explicit HtmlH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH4(::pltxt2htm::HtmlH4<ndebug> const&) noexcept;
    constexpr HtmlH4(::pltxt2htm::HtmlH4<ndebug>&&) noexcept;
    constexpr ~HtmlH4() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH4<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH4<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH4<ndebug>& self, ::pltxt2htm::HtmlH4<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH4<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH4 const&, HtmlH4 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h5&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH5 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH5() noexcept = delete;
    constexpr explicit HtmlH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH5(::pltxt2htm::HtmlH5<ndebug> const&) noexcept;
    constexpr HtmlH5(::pltxt2htm::HtmlH5<ndebug>&&) noexcept;
    constexpr ~HtmlH5() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH5<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH5<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH5<ndebug>& self, ::pltxt2htm::HtmlH5<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH5<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH5 const&, HtmlH5 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;h6&gt; heading node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlH6 {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlH6() noexcept = delete;
    constexpr explicit HtmlH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH6(::pltxt2htm::HtmlH6<ndebug> const&) noexcept;
    constexpr HtmlH6(::pltxt2htm::HtmlH6<ndebug>&&) noexcept;
    constexpr ~HtmlH6() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH6<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH6<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlH6<ndebug>& self, ::pltxt2htm::HtmlH6<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlH6<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlH6 const&, HtmlH6 const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;p&gt; paragraph node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlP {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlP() noexcept = delete;
    constexpr explicit HtmlP(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlP(::pltxt2htm::HtmlP<ndebug> const&) noexcept;
    constexpr HtmlP(::pltxt2htm::HtmlP<ndebug>&&) noexcept;
    constexpr ~HtmlP() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlP<ndebug> const&) noexcept -> ::pltxt2htm::HtmlP<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlP<ndebug>& self, ::pltxt2htm::HtmlP<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlP<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlP const&, HtmlP const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;del&gt; deleted text node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlDel {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlDel() noexcept = delete;
    constexpr explicit HtmlDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlDel(::pltxt2htm::HtmlDel<ndebug> const&) noexcept;
    constexpr HtmlDel(::pltxt2htm::HtmlDel<ndebug>&&) noexcept;
    constexpr ~HtmlDel() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlDel<ndebug> const&) noexcept -> ::pltxt2htm::HtmlDel<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlDel<ndebug>& self, ::pltxt2htm::HtmlDel<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlDel<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlDel const&, HtmlDel const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;note&gt; custom note node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlNote {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlNote() noexcept = delete;
    constexpr explicit HtmlNote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlNote(::pltxt2htm::HtmlNote<ndebug> const&) noexcept;
    constexpr HtmlNote(::pltxt2htm::HtmlNote<ndebug>&&) noexcept;
    constexpr ~HtmlNote() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlNote<ndebug> const&) noexcept -> ::pltxt2htm::HtmlNote<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlNote<ndebug>& self, ::pltxt2htm::HtmlNote<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlNote<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlNote const&, HtmlNote const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;em&gt; emphasis node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlEm {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlEm() noexcept = delete;
    constexpr explicit HtmlEm(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlEm(::pltxt2htm::HtmlEm<ndebug> const&) noexcept;
    constexpr HtmlEm(::pltxt2htm::HtmlEm<ndebug>&&) noexcept;
    constexpr ~HtmlEm() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlEm<ndebug> const&) noexcept -> ::pltxt2htm::HtmlEm<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlEm<ndebug>& self, ::pltxt2htm::HtmlEm<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlEm<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlEm const&, HtmlEm const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;strong&gt; strong importance node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlStrong {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlStrong() noexcept = delete;
    constexpr explicit HtmlStrong(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong<ndebug> const&) noexcept;
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong<ndebug>&&) noexcept;
    constexpr ~HtmlStrong() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlStrong<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlStrong<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlStrong<ndebug>& self, ::pltxt2htm::HtmlStrong<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlStrong<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlStrong const&, HtmlStrong const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;ul&gt; unordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlUl {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlUl() noexcept = delete;
    constexpr explicit HtmlUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlUl(::pltxt2htm::HtmlUl<ndebug> const&) noexcept;
    constexpr HtmlUl(::pltxt2htm::HtmlUl<ndebug>&&) noexcept;
    constexpr ~HtmlUl() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlUl<ndebug> const&) noexcept -> ::pltxt2htm::HtmlUl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlUl<ndebug>& self, ::pltxt2htm::HtmlUl<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlUl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlUl const&, HtmlUl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;ol&gt; ordered list node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlOl {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlOl() noexcept = delete;
    constexpr explicit HtmlOl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlOl(::pltxt2htm::HtmlOl<ndebug> const&) noexcept;
    constexpr HtmlOl(::pltxt2htm::HtmlOl<ndebug>&&) noexcept;
    constexpr ~HtmlOl() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlOl<ndebug> const&) noexcept -> ::pltxt2htm::HtmlOl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlOl<ndebug>& self, ::pltxt2htm::HtmlOl<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlOl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlOl const&, HtmlOl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;li&gt; list item node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlLi {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlLi() noexcept = delete;
    constexpr explicit HtmlLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlLi(::pltxt2htm::HtmlLi<ndebug> const&) noexcept;
    constexpr HtmlLi(::pltxt2htm::HtmlLi<ndebug>&&) noexcept;
    constexpr ~HtmlLi() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlLi<ndebug> const&) noexcept -> ::pltxt2htm::HtmlLi<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlLi<ndebug>& self, ::pltxt2htm::HtmlLi<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlLi<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlLi const&, HtmlLi const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;code&gt; inline code node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlCode {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlCode() noexcept = delete;
    constexpr explicit HtmlCode(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlCode(::pltxt2htm::HtmlCode<ndebug> const&) noexcept;
    constexpr HtmlCode(::pltxt2htm::HtmlCode<ndebug>&&) noexcept;
    constexpr ~HtmlCode() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlCode<ndebug> const&) noexcept -> ::pltxt2htm::HtmlCode<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlCode<ndebug>& self, ::pltxt2htm::HtmlCode<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlCode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlCode const&, HtmlCode const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;pre&gt; preformatted text node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlPre {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlPre() noexcept = delete;
    constexpr explicit HtmlPre(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlPre(::pltxt2htm::HtmlPre<ndebug> const&) noexcept;
    constexpr HtmlPre(::pltxt2htm::HtmlPre<ndebug>&&) noexcept;
    constexpr ~HtmlPre() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlPre<ndebug> const&) noexcept -> ::pltxt2htm::HtmlPre<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlPre<ndebug>& self, ::pltxt2htm::HtmlPre<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlPre<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlPre const&, HtmlPre const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;span style="color:...;font-size:..."&gt; node
 * @details Represents an HTML span element with color and/or font-size style attributes.
 *          color stores the CSS color value (e.g. "red" or "#FF0000").
 *          font_size stores the numeric font-size value if present (e.g. 20 for 20px).
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlSpan {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::fast_io::u8string color;
    ::exception::optional<::std::size_t> font_size;

public:
    constexpr HtmlSpan(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_,
                       ::exception::optional<::std::size_t>&& font_size_) noexcept;
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept;
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan<ndebug>&&) noexcept;
    constexpr ~HtmlSpan() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept -> ::pltxt2htm::HtmlSpan<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlSpan<ndebug>& self, ::pltxt2htm::HtmlSpan<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlSpan<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlSpan const&, HtmlSpan const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_color(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.color);
    }

    [[nodiscard]]
    constexpr auto get_font_size(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.font_size);
    }
};

/**
 * @brief HTML &lt;blockquote&gt; block quotation node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlBlockquote {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlBlockquote() noexcept = delete;
    constexpr explicit HtmlBlockquote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept;
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept;
    constexpr ~HtmlBlockquote() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlBlockquote<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlBlockquote<ndebug>& self,
                             ::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept -> ::pltxt2htm::HtmlBlockquote<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlBlockquote const&, HtmlBlockquote const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;table&gt; table node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTable {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlTable() noexcept = delete;
    constexpr explicit HtmlTable(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlTable(::pltxt2htm::HtmlTable<ndebug> const&) noexcept;
    constexpr HtmlTable(::pltxt2htm::HtmlTable<ndebug>&&) noexcept;
    constexpr ~HtmlTable() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTable<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlTable<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTable<ndebug>& self, ::pltxt2htm::HtmlTable<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTable<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTable const&, HtmlTable const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;tr&gt; table row node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTr {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlTr() noexcept = delete;
    constexpr explicit HtmlTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlTr(::pltxt2htm::HtmlTr<ndebug> const&) noexcept;
    constexpr HtmlTr(::pltxt2htm::HtmlTr<ndebug>&&) noexcept;
    constexpr ~HtmlTr() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTr<ndebug> const&) noexcept -> ::pltxt2htm::HtmlTr<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTr<ndebug>& self, ::pltxt2htm::HtmlTr<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTr<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTr const&, HtmlTr const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;td&gt; table data cell node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTd {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::MdTableAlign align_;

public:
    constexpr HtmlTd() noexcept = delete;
    constexpr explicit HtmlTd(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::MdTableAlign align_) noexcept;
    constexpr HtmlTd(::pltxt2htm::HtmlTd<ndebug> const&) noexcept;
    constexpr HtmlTd(::pltxt2htm::HtmlTd<ndebug>&&) noexcept;
    constexpr ~HtmlTd() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTd<ndebug> const&) noexcept -> ::pltxt2htm::HtmlTd<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTd<ndebug>& self, ::pltxt2htm::HtmlTd<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTd<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTd const&, HtmlTd const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this auto&& self) noexcept -> ::pltxt2htm::MdTableAlign {
        return self.align_;
    }
};

/**
 * @brief HTML &lt;th&gt; table header cell node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTh {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlTh() noexcept = delete;
    constexpr explicit HtmlTh(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlTh(::pltxt2htm::HtmlTh<ndebug> const&) noexcept;
    constexpr HtmlTh(::pltxt2htm::HtmlTh<ndebug>&&) noexcept;
    constexpr ~HtmlTh() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTh<ndebug> const&) noexcept -> ::pltxt2htm::HtmlTh<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTh<ndebug>& self, ::pltxt2htm::HtmlTh<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTh<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTh const&, HtmlTh const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;thead&gt; table head section node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlThead {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlThead() noexcept = delete;
    constexpr explicit HtmlThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlThead(::pltxt2htm::HtmlThead<ndebug> const&) noexcept;
    constexpr HtmlThead(::pltxt2htm::HtmlThead<ndebug>&&) noexcept;
    constexpr ~HtmlThead() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlThead<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlThead<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlThead<ndebug>& self, ::pltxt2htm::HtmlThead<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlThead<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlThead const&, HtmlThead const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;tbody&gt; table body section node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTbody {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlTbody() noexcept = delete;
    constexpr explicit HtmlTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlTbody(::pltxt2htm::HtmlTbody<ndebug> const&) noexcept;
    constexpr HtmlTbody(::pltxt2htm::HtmlTbody<ndebug>&&) noexcept;
    constexpr ~HtmlTbody() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTbody<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlTbody<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTbody<ndebug>& self, ::pltxt2htm::HtmlTbody<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTbody<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTbody const&, HtmlTbody const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;tfoot&gt; table foot section node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlTfoot {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlTfoot() noexcept = delete;
    constexpr explicit HtmlTfoot(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlTfoot(::pltxt2htm::HtmlTfoot<ndebug> const&) noexcept;
    constexpr HtmlTfoot(::pltxt2htm::HtmlTfoot<ndebug>&&) noexcept;
    constexpr ~HtmlTfoot() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlTfoot<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlTfoot<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlTfoot<ndebug>& self, ::pltxt2htm::HtmlTfoot<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlTfoot<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlTfoot const&, HtmlTfoot const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;caption&gt; table caption node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlCaption {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlCaption() noexcept = delete;
    constexpr explicit HtmlCaption(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlCaption(::pltxt2htm::HtmlCaption<ndebug> const&) noexcept;
    constexpr HtmlCaption(::pltxt2htm::HtmlCaption<ndebug>&&) noexcept;
    constexpr ~HtmlCaption() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlCaption<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlCaption<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlCaption<ndebug>& self, ::pltxt2htm::HtmlCaption<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlCaption<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlCaption const&, HtmlCaption const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;colgroup&gt; table column group node
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlColgroup {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr HtmlColgroup() noexcept = delete;
    constexpr explicit HtmlColgroup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlColgroup(::pltxt2htm::HtmlColgroup<ndebug> const&) noexcept;
    constexpr HtmlColgroup(::pltxt2htm::HtmlColgroup<ndebug>&&) noexcept;
    constexpr ~HtmlColgroup() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlColgroup<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlColgroup<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::HtmlColgroup<ndebug>& self, ::pltxt2htm::HtmlColgroup<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlColgroup<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlColgroup const&, HtmlColgroup const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm
