/**
 * @file html_node_decl.hh
 * @brief HTML-like AST node declarations for pltxt2htm
 * @details Defines nodes corresponding to HTML tags and special characters
 *          (line breaks, headings, lists, formatting, etc.)
 */

#pragma once

#include <utility>
#include <cstddef>
#include <cstdint>
#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../plain_text.hh"
#include "ast_decl.hh"
#include "url_node_decl.hh"
#include "../value_unit.hh"
#include "../vertical_align_value.hh"
#include "../node_kind.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief HTML &lt;br&gt; tag node
 * @details Represents a forced line break in HTML output.
 */
class HtmlBr {
public:
    [[nodiscard]]
    constexpr auto operator==(this HtmlBr const&, HtmlBr const&) noexcept -> bool = default;
};

/**
 * @brief HTML &lt;hr&gt; tag node
 * @details Represents a horizontal rule.
 */
class HtmlHr {
public:
    [[nodiscard]]
    constexpr auto operator==(this HtmlHr const&, HtmlHr const&) noexcept -> bool = default;
};

/**
 * @brief HTML &lt;img&gt; image node (self-closing)
 * @details Represents &lt;img src=&quot;...&quot; alt=&quot;...&quot;&gt;.
 */
class HtmlImg {
    ::pltxt2htm::container::U8String src;
    ::pltxt2htm::PlainText alt;

public:
    constexpr HtmlImg(::pltxt2htm::container::U8String&& src_, ::pltxt2htm::PlainText&& alt_) noexcept
        : src(::std::move(src_)),
          alt(::std::move(alt_)) {
    }

    constexpr HtmlImg(::pltxt2htm::HtmlImg const&) noexcept = default;
    constexpr HtmlImg(::pltxt2htm::HtmlImg&&) noexcept = default;
    constexpr ~HtmlImg() noexcept = default;
    constexpr auto operator=(this HtmlImg& self, ::pltxt2htm::HtmlImg const&) noexcept
        -> ::pltxt2htm::HtmlImg& = default;
    constexpr auto operator=(this HtmlImg& self, ::pltxt2htm::HtmlImg&&) noexcept -> ::pltxt2htm::HtmlImg& = default;

    [[nodiscard]]
    constexpr auto operator==(this HtmlImg const&, HtmlImg const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto get_src(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.src);
    }

    [[nodiscard]]
    constexpr auto get_alt(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.alt);
    }
};

/**
 * @brief HTML &lt;h1&gt; heading node
 * @details Represents a level-1 heading containing sub-AST content.
 */
class HtmlH1 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH1(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH1(::pltxt2htm::HtmlH1 const&) noexcept = delete;
#else
    constexpr HtmlH1(::pltxt2htm::HtmlH1 const&) noexcept = default;
#endif
    constexpr HtmlH1(::pltxt2htm::HtmlH1&&) noexcept;
    constexpr ~HtmlH1() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH1 const&) noexcept -> ::pltxt2htm::HtmlH1& = delete;
#else
    constexpr auto operator=(this HtmlH1& self, ::pltxt2htm::HtmlH1 const&) noexcept -> ::pltxt2htm::HtmlH1& = default;
#endif
    constexpr auto operator=(this HtmlH1& self, ::pltxt2htm::HtmlH1&&) noexcept -> ::pltxt2htm::HtmlH1&;

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
class HtmlH2 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH2(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH2(::pltxt2htm::HtmlH2 const&) noexcept = delete;
#else
    constexpr HtmlH2(::pltxt2htm::HtmlH2 const&) noexcept = default;
#endif
    constexpr HtmlH2(::pltxt2htm::HtmlH2&&) noexcept;
    constexpr ~HtmlH2() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH2 const&) noexcept -> ::pltxt2htm::HtmlH2& = delete;
#else
    constexpr auto operator=(this HtmlH2& self, ::pltxt2htm::HtmlH2 const&) noexcept -> ::pltxt2htm::HtmlH2& = default;
#endif
    constexpr auto operator=(this HtmlH2& self, ::pltxt2htm::HtmlH2&&) noexcept -> ::pltxt2htm::HtmlH2&;

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
class HtmlH3 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH3(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH3(::pltxt2htm::HtmlH3 const&) noexcept = delete;
#else
    constexpr HtmlH3(::pltxt2htm::HtmlH3 const&) noexcept = default;
#endif
    constexpr HtmlH3(::pltxt2htm::HtmlH3&&) noexcept;
    constexpr ~HtmlH3() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH3 const&) noexcept -> ::pltxt2htm::HtmlH3& = delete;
#else
    constexpr auto operator=(this HtmlH3& self, ::pltxt2htm::HtmlH3 const&) noexcept -> ::pltxt2htm::HtmlH3& = default;
#endif
    constexpr auto operator=(this HtmlH3& self, ::pltxt2htm::HtmlH3&&) noexcept -> ::pltxt2htm::HtmlH3&;

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
class HtmlH4 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH4(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH4(::pltxt2htm::HtmlH4 const&) noexcept = delete;
#else
    constexpr HtmlH4(::pltxt2htm::HtmlH4 const&) noexcept = default;
#endif
    constexpr HtmlH4(::pltxt2htm::HtmlH4&&) noexcept;
    constexpr ~HtmlH4() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH4 const&) noexcept -> ::pltxt2htm::HtmlH4& = delete;
#else
    constexpr auto operator=(this HtmlH4& self, ::pltxt2htm::HtmlH4 const&) noexcept -> ::pltxt2htm::HtmlH4& = default;
#endif
    constexpr auto operator=(this HtmlH4& self, ::pltxt2htm::HtmlH4&&) noexcept -> ::pltxt2htm::HtmlH4&;

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
class HtmlH5 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH5(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH5(::pltxt2htm::HtmlH5 const&) noexcept = delete;
#else
    constexpr HtmlH5(::pltxt2htm::HtmlH5 const&) noexcept = default;
#endif
    constexpr HtmlH5(::pltxt2htm::HtmlH5&&) noexcept;
    constexpr ~HtmlH5() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH5 const&) noexcept -> ::pltxt2htm::HtmlH5& = delete;
#else
    constexpr auto operator=(this HtmlH5& self, ::pltxt2htm::HtmlH5 const&) noexcept -> ::pltxt2htm::HtmlH5& = default;
#endif
    constexpr auto operator=(this HtmlH5& self, ::pltxt2htm::HtmlH5&&) noexcept -> ::pltxt2htm::HtmlH5&;

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
class HtmlH6 {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlH6(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlH6(::pltxt2htm::HtmlH6 const&) noexcept = delete;
#else
    constexpr HtmlH6(::pltxt2htm::HtmlH6 const&) noexcept = default;
#endif
    constexpr HtmlH6(::pltxt2htm::HtmlH6&&) noexcept;
    constexpr ~HtmlH6() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlH6 const&) noexcept -> ::pltxt2htm::HtmlH6& = delete;
#else
    constexpr auto operator=(this HtmlH6& self, ::pltxt2htm::HtmlH6 const&) noexcept -> ::pltxt2htm::HtmlH6& = default;
#endif
    constexpr auto operator=(this HtmlH6& self, ::pltxt2htm::HtmlH6&&) noexcept -> ::pltxt2htm::HtmlH6&;

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
class HtmlP {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::TextAlign align;

public:
    constexpr HtmlP(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TextAlign align_ = ::pltxt2htm::TextAlign::left) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlP(::pltxt2htm::HtmlP const&) noexcept = delete;
#else
    constexpr HtmlP(::pltxt2htm::HtmlP const&) noexcept = default;
#endif
    constexpr HtmlP(::pltxt2htm::HtmlP&&) noexcept;
    constexpr ~HtmlP() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlP const&) noexcept -> ::pltxt2htm::HtmlP& = delete;
#else
    constexpr auto operator=(this HtmlP& self, ::pltxt2htm::HtmlP const&) noexcept -> ::pltxt2htm::HtmlP& = default;
#endif
    constexpr auto operator=(this HtmlP& self, ::pltxt2htm::HtmlP&&) noexcept -> ::pltxt2htm::HtmlP&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlP const&, HtmlP const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_align(this HtmlP const& self) noexcept -> ::pltxt2htm::TextAlign {
        return self.align;
    }
};

/**
 * @brief HTML &lt;del&gt; deleted text node
 */
class HtmlDel {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlDel(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlDel(::pltxt2htm::HtmlDel const&) noexcept = delete;
#else
    constexpr HtmlDel(::pltxt2htm::HtmlDel const&) noexcept = default;
#endif
    constexpr HtmlDel(::pltxt2htm::HtmlDel&&) noexcept;
    constexpr ~HtmlDel() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlDel const&) noexcept -> ::pltxt2htm::HtmlDel& = delete;
#else
    constexpr auto operator=(this HtmlDel& self, ::pltxt2htm::HtmlDel const&) noexcept -> ::pltxt2htm::HtmlDel& = default;
#endif
    constexpr auto operator=(this HtmlDel& self, ::pltxt2htm::HtmlDel&&) noexcept -> ::pltxt2htm::HtmlDel&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlDel const&, HtmlDel const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;u&gt; underline text node
 */
class HtmlU {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlU(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlU(::pltxt2htm::HtmlU const&) noexcept = delete;
#else
    constexpr HtmlU(::pltxt2htm::HtmlU const&) noexcept = default;
#endif
    constexpr HtmlU(::pltxt2htm::HtmlU&&) noexcept;
    constexpr ~HtmlU() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlU const&) noexcept -> ::pltxt2htm::HtmlU& = delete;
#else
    constexpr auto operator=(this HtmlU& self, ::pltxt2htm::HtmlU const&) noexcept -> ::pltxt2htm::HtmlU& = default;
#endif
    constexpr auto operator=(this HtmlU& self, ::pltxt2htm::HtmlU&&) noexcept -> ::pltxt2htm::HtmlU&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlU const&, HtmlU const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;s&gt; strikethrough text node
 */
class HtmlS {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlS(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlS(::pltxt2htm::HtmlS const&) noexcept = delete;
#else
    constexpr HtmlS(::pltxt2htm::HtmlS const&) noexcept = default;
#endif
    constexpr HtmlS(::pltxt2htm::HtmlS&&) noexcept;
    constexpr ~HtmlS() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlS const&) noexcept -> ::pltxt2htm::HtmlS& = delete;
#else
    constexpr auto operator=(this HtmlS& self, ::pltxt2htm::HtmlS const&) noexcept -> ::pltxt2htm::HtmlS& = default;
#endif
    constexpr auto operator=(this HtmlS& self, ::pltxt2htm::HtmlS&&) noexcept -> ::pltxt2htm::HtmlS&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlS const&, HtmlS const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;sup&gt; superscript text node
 * @details Represents &lt;sup&gt;...&lt;/sup&gt; with sub-AST content.
 */
class HtmlSup {
    ::pltxt2htm::Ast subast;

public:
    constexpr HtmlSup(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlSup(::pltxt2htm::HtmlSup const&) noexcept = delete;
#else
    constexpr HtmlSup(::pltxt2htm::HtmlSup const&) noexcept = default;
#endif
    constexpr HtmlSup(::pltxt2htm::HtmlSup&&) noexcept;
    constexpr ~HtmlSup() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlSup const&) noexcept -> ::pltxt2htm::HtmlSup& = delete;
#else
    constexpr auto operator=(this HtmlSup& self, ::pltxt2htm::HtmlSup const&) noexcept -> ::pltxt2htm::HtmlSup& = default;
#endif
    constexpr auto operator=(this HtmlSup& self, ::pltxt2htm::HtmlSup&&) noexcept -> ::pltxt2htm::HtmlSup&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlSup const&, HtmlSup const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;sub&gt; subscript text node
 * @details Represents &lt;sub&gt;...&lt;/sub&gt; with sub-AST content.
 */
class HtmlSub {
    ::pltxt2htm::Ast subast;

public:
    constexpr HtmlSub(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlSub(::pltxt2htm::HtmlSub const&) noexcept = delete;
#else
    constexpr HtmlSub(::pltxt2htm::HtmlSub const&) noexcept = default;
#endif
    constexpr HtmlSub(::pltxt2htm::HtmlSub&&) noexcept;
    constexpr ~HtmlSub() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlSub const&) noexcept -> ::pltxt2htm::HtmlSub& = delete;
#else
    constexpr auto operator=(this HtmlSub& self, ::pltxt2htm::HtmlSub const&) noexcept -> ::pltxt2htm::HtmlSub& = default;
#endif
    constexpr auto operator=(this HtmlSub& self, ::pltxt2htm::HtmlSub&&) noexcept -> ::pltxt2htm::HtmlSub&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlSub const&, HtmlSub const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;note&gt; custom note node
 */
class HtmlNote {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlNote(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlNote(::pltxt2htm::HtmlNote const&) noexcept = delete;
#else
    constexpr HtmlNote(::pltxt2htm::HtmlNote const&) noexcept = default;
#endif
    constexpr HtmlNote(::pltxt2htm::HtmlNote&&) noexcept;
    constexpr ~HtmlNote() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlNote const&) noexcept -> ::pltxt2htm::HtmlNote& = delete;
#else
    constexpr auto operator=(this HtmlNote& self, ::pltxt2htm::HtmlNote const&) noexcept -> ::pltxt2htm::HtmlNote& = default;
#endif
    constexpr auto operator=(this HtmlNote& self, ::pltxt2htm::HtmlNote&&) noexcept -> ::pltxt2htm::HtmlNote&;

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
class HtmlEm {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlEm(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlEm(::pltxt2htm::HtmlEm const&) noexcept = delete;
#else
    constexpr HtmlEm(::pltxt2htm::HtmlEm const&) noexcept = default;
#endif
    constexpr HtmlEm(::pltxt2htm::HtmlEm&&) noexcept;
    constexpr ~HtmlEm() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlEm const&) noexcept -> ::pltxt2htm::HtmlEm& = delete;
#else
    constexpr auto operator=(this HtmlEm& self, ::pltxt2htm::HtmlEm const&) noexcept -> ::pltxt2htm::HtmlEm& = default;
#endif
    constexpr auto operator=(this HtmlEm& self, ::pltxt2htm::HtmlEm&&) noexcept -> ::pltxt2htm::HtmlEm&;

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
class HtmlStrong {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlStrong(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong const&) noexcept = delete;
#else
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong const&) noexcept = default;
#endif
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong&&) noexcept;
    constexpr ~HtmlStrong() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlStrong const&) noexcept -> ::pltxt2htm::HtmlStrong& = delete;
#else
    constexpr auto operator=(this HtmlStrong& self, ::pltxt2htm::HtmlStrong const&) noexcept -> ::pltxt2htm::HtmlStrong& = default;
#endif
    constexpr auto operator=(this HtmlStrong& self, ::pltxt2htm::HtmlStrong&&) noexcept -> ::pltxt2htm::HtmlStrong&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlStrong const&, HtmlStrong const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;mark&gt; marked/highlighted text node
 * @details Represents &lt;mark&gt;...&lt;/mark&gt;. `background_color` stores the CSS
 *          background-color value. Defaults to the standard highlight color "#FFFF00";
 *          an explicitly provided style value (e.g. "red" or "#FF0000") overrides it.
 */
class HtmlMark {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::container::U8String background_color;

public:
    static constexpr auto default_background_color = ::pltxt2htm::container::U8StringView{u8"#FFFF00"};

    constexpr explicit HtmlMark(::pltxt2htm::Ast&& subast_,
                                ::pltxt2htm::container::U8String&& background_color_ = ::pltxt2htm::container::U8String{
                                    default_background_color}) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlMark(HtmlMark const&) noexcept = delete;
#else
    constexpr HtmlMark(HtmlMark const&) noexcept = default;
#endif
    constexpr HtmlMark(HtmlMark&&) noexcept;
    constexpr ~HtmlMark() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(HtmlMark const&) noexcept -> HtmlMark& = delete;
#else
    constexpr auto operator=(this HtmlMark& self, HtmlMark const&) noexcept -> HtmlMark& = default;
#endif
    constexpr auto operator=(this HtmlMark& self, HtmlMark&&) noexcept -> HtmlMark&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlMark const&, HtmlMark const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_background_color(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.background_color);
    }
};

/**
 * @brief HTML &lt;code&gt; inline code node
 */
class HtmlCode {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlCode(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlCode(::pltxt2htm::HtmlCode const&) noexcept = delete;
#else
    constexpr HtmlCode(::pltxt2htm::HtmlCode const&) noexcept = default;
#endif
    constexpr HtmlCode(::pltxt2htm::HtmlCode&&) noexcept;
    constexpr ~HtmlCode() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlCode const&) noexcept -> ::pltxt2htm::HtmlCode& = delete;
#else
    constexpr auto operator=(this HtmlCode& self, ::pltxt2htm::HtmlCode const&) noexcept -> ::pltxt2htm::HtmlCode& = default;
#endif
    constexpr auto operator=(this HtmlCode& self, ::pltxt2htm::HtmlCode&&) noexcept -> ::pltxt2htm::HtmlCode&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlCode const&, HtmlCode const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief HTML &lt;span style="color:...;font-size:...;vertical-align:..."&gt; node
 * @details Represents an HTML span element with color/font-size/vertical-align style attributes.
 *          color stores the CSS color value (e.g. "red" or "#FF0000").
 *          font_size stores the font-size value+unit if present (e.g. {20.5, px} for 20.5px).
 *          vertical_align stores the vertical-align value+unit if present (e.g. {super} or {5, px}).
 */
class HtmlSpan {
    /**
     * @brief Compact storage for the optional span style attributes.
     * @details Semantically equivalent to the clearer but larger representation:
     * @code
     * Optional<ValueWithUnit<double>> font_size;
     * Optional<VerticalAlignValue> vertical_align;
     * @endcode
     * Flattening their state lets the tags share alignment padding and avoids enlarging every PlTxtNode.
     */
    struct StyleStorage {
        // Intentionally use the least unsigned type with at least 8 value bits to keep this storage tag compact.
        enum class VerticalAlignStorageKind : ::std::uint_least8_t {
            none,
            keyword_baseline,
            keyword_sub,
            keyword_super,
            keyword_text_top,
            keyword_text_bottom,
            keyword_middle,
            keyword_top,
            keyword_bottom,
            length_px,
            length_percent,
            length_em,
        };

        double font_size_value{};
        ::std::ptrdiff_t vertical_align_length{};
        ::pltxt2htm::Unit font_size_unit{::pltxt2htm::Unit::px};
        VerticalAlignStorageKind vertical_align_kind{VerticalAlignStorageKind::none};
        bool has_font_size{};

        [[nodiscard]]
        static constexpr auto encode_vertical_align_unit(::pltxt2htm::Unit const unit) noexcept
            -> VerticalAlignStorageKind {
            switch (unit) /* -Werror=switch */ {
            case ::pltxt2htm::Unit::px: {
                return VerticalAlignStorageKind::length_px;
            }
            case ::pltxt2htm::Unit::percent: {
                return VerticalAlignStorageKind::length_percent;
            }
            case ::pltxt2htm::Unit::em: {
                return VerticalAlignStorageKind::length_em;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected vertical-align unit");
                }
#endif
            }
            pltxt2htm_unreachable(u8"Unreachable after vertical-align unit switch");
        }

        [[nodiscard]]
        static constexpr auto encode_vertical_align_keyword(::pltxt2htm::VerticalAlignKeyword const keyword) noexcept
            -> VerticalAlignStorageKind {
            switch (keyword) /* -Werror=switch */ {
            case ::pltxt2htm::VerticalAlignKeyword::baseline: {
                return VerticalAlignStorageKind::keyword_baseline;
            }
            case ::pltxt2htm::VerticalAlignKeyword::sub: {
                return VerticalAlignStorageKind::keyword_sub;
            }
            case ::pltxt2htm::VerticalAlignKeyword::super: {
                return VerticalAlignStorageKind::keyword_super;
            }
            case ::pltxt2htm::VerticalAlignKeyword::text_top: {
                return VerticalAlignStorageKind::keyword_text_top;
            }
            case ::pltxt2htm::VerticalAlignKeyword::text_bottom: {
                return VerticalAlignStorageKind::keyword_text_bottom;
            }
            case ::pltxt2htm::VerticalAlignKeyword::middle: {
                return VerticalAlignStorageKind::keyword_middle;
            }
            case ::pltxt2htm::VerticalAlignKeyword::top: {
                return VerticalAlignStorageKind::keyword_top;
            }
            case ::pltxt2htm::VerticalAlignKeyword::bottom: {
                return VerticalAlignStorageKind::keyword_bottom;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected vertical-align keyword");
                }
#endif
            }
            pltxt2htm_unreachable(u8"Unreachable after vertical-align keyword switch");
        }

        constexpr StyleStorage(
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> const& font_size,
            ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue> const& vertical_align) noexcept {
            if (font_size.has_value()) {
                auto const& value = font_size.value();
                font_size_value = value.value;
                font_size_unit = value.unit;
                has_font_size = true;
            }
            if (vertical_align.has_value() == false) {
                return;
            }
            auto const& value = vertical_align.value();
            if (value.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
                vertical_align_kind = encode_vertical_align_keyword(value.get_keyword());
                return;
            }
            auto const length = value.get_length();
            vertical_align_length = length.value;
            vertical_align_kind = encode_vertical_align_unit(length.unit);
        }

        [[nodiscard]]
        constexpr auto operator==(this StyleStorage const& self, StyleStorage const& other) noexcept -> bool {
            if (self.has_font_size != other.has_font_size || self.vertical_align_kind != other.vertical_align_kind) {
                return false;
            }
            if (self.has_font_size &&
                (self.font_size_value != other.font_size_value || self.font_size_unit != other.font_size_unit)) {
                return false;
            }
            bool const stores_vertical_align_length{self.vertical_align_kind == VerticalAlignStorageKind::length_px ||
                                                    self.vertical_align_kind ==
                                                        VerticalAlignStorageKind::length_percent ||
                                                    self.vertical_align_kind == VerticalAlignStorageKind::length_em};
            return stores_vertical_align_length == false || self.vertical_align_length == other.vertical_align_length;
        }

        [[nodiscard]]
        constexpr auto get_font_size(this StyleStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> {
            if (self.has_font_size == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return ::pltxt2htm::ValueWithUnit<double>{.value = self.font_size_value, .unit = self.font_size_unit};
        }

        [[nodiscard]]
        constexpr auto get_vertical_align(this StyleStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue> {
            switch (self.vertical_align_kind) /* -Werror=switch */ {
            case VerticalAlignStorageKind::none: {
                return ::pltxt2htm::container::nullopt;
            }
            case VerticalAlignStorageKind::keyword_baseline: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::baseline};
            }
            case VerticalAlignStorageKind::keyword_sub: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::sub};
            }
            case VerticalAlignStorageKind::keyword_super: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::super};
            }
            case VerticalAlignStorageKind::keyword_text_top: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::text_top};
            }
            case VerticalAlignStorageKind::keyword_text_bottom: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::text_bottom};
            }
            case VerticalAlignStorageKind::keyword_middle: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::middle};
            }
            case VerticalAlignStorageKind::keyword_top: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::top};
            }
            case VerticalAlignStorageKind::keyword_bottom: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::VerticalAlignKeyword::bottom};
            }
            case VerticalAlignStorageKind::length_px: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
                    .value = self.vertical_align_length, .unit = ::pltxt2htm::Unit::px}};
            }
            case VerticalAlignStorageKind::length_percent: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
                    .value = self.vertical_align_length, .unit = ::pltxt2htm::Unit::percent}};
            }
            case VerticalAlignStorageKind::length_em: {
                return ::pltxt2htm::VerticalAlignValue{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
                    .value = self.vertical_align_length, .unit = ::pltxt2htm::Unit::em}};
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected stored vertical-align kind");
                }
#endif
            }
            pltxt2htm_unreachable(u8"Unreachable after stored vertical-align kind switch");
        }
    };

    ::pltxt2htm::Ast subast;
    ::pltxt2htm::container::U8String color;
    StyleStorage style;

public:
    constexpr HtmlSpan(::pltxt2htm::Ast&& subast_, ::pltxt2htm::container::U8String&& color_,
                       ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_,
                       ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue> vertical_align_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan const&) noexcept = delete;
#else
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan const&) noexcept = default;
#endif
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan&&) noexcept;
    constexpr ~HtmlSpan() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlSpan const&) noexcept -> ::pltxt2htm::HtmlSpan& = delete;
#else
    constexpr auto operator=(this HtmlSpan& self, ::pltxt2htm::HtmlSpan const&) noexcept -> ::pltxt2htm::HtmlSpan& = default;
#endif
    constexpr auto operator=(this HtmlSpan& self, ::pltxt2htm::HtmlSpan&&) noexcept -> ::pltxt2htm::HtmlSpan&;

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
    constexpr auto get_font_size(this HtmlSpan const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> {
        return self.style.get_font_size();
    }

    [[nodiscard]]
    constexpr auto get_vertical_align(this HtmlSpan const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue> {
        return self.style.get_vertical_align();
    }
};

/**
 * @brief HTML &lt;div style="margin-left:...;margin-right:..."&gt; node
 * @details Represents an HTML div element whose only allowed style attributes are
 *          margin-left and margin-right (each an optional px/em/% value). Any other
 *          attribute or CSS declaration is rejected by the parser to prevent XSS.
 */
class HtmlDiv {
    struct MarginStorage {
        ::std::size_t left_value{};
        ::std::size_t right_value{};
        ::pltxt2htm::Unit left_unit : 2 {::pltxt2htm::Unit::px};
        ::pltxt2htm::Unit right_unit : 2 {::pltxt2htm::Unit::px};
        bool has_left : 1 {};
        bool has_right : 1 {};

        constexpr MarginStorage(
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> const& left,
            ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> const& right) noexcept {
            if (left.has_value()) {
                auto const& value = left.value();
                left_value = value.value;
                left_unit = value.unit;
                has_left = true;
            }
            if (right.has_value()) {
                auto const& value = right.value();
                right_value = value.value;
                right_unit = value.unit;
                has_right = true;
            }
        }

        [[nodiscard]]
        constexpr auto operator==(this MarginStorage const& self, MarginStorage const& other) noexcept -> bool {
            bool const left_equal{
                self.has_left == other.has_left &&
                (self.has_left == false || (self.left_value == other.left_value && self.left_unit == other.left_unit))};
            return left_equal && self.has_right == other.has_right &&
                   (self.has_right == false ||
                    (self.right_value == other.right_value && self.right_unit == other.right_unit));
        }

        [[nodiscard]]
        constexpr auto get_left(this MarginStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
            if (self.has_left == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return ::pltxt2htm::ValueWithUnit<::std::size_t>{.value = self.left_value, .unit = self.left_unit};
        }

        [[nodiscard]]
        constexpr auto get_right(this MarginStorage const& self) noexcept
            -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
            if (self.has_right == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return ::pltxt2htm::ValueWithUnit<::std::size_t>{.value = self.right_value, .unit = self.right_unit};
        }
    };

    ::pltxt2htm::Ast subast;
    MarginStorage margins;

public:
    constexpr HtmlDiv(::pltxt2htm::Ast&& subast_,
                      ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
                      ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlDiv(::pltxt2htm::HtmlDiv const&) noexcept = delete;
#else
    constexpr HtmlDiv(::pltxt2htm::HtmlDiv const&) noexcept = default;
#endif
    constexpr HtmlDiv(::pltxt2htm::HtmlDiv&&) noexcept;
    constexpr ~HtmlDiv() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlDiv const&) noexcept -> ::pltxt2htm::HtmlDiv& = delete;
#else
    constexpr auto operator=(this HtmlDiv& self, ::pltxt2htm::HtmlDiv const&) noexcept -> ::pltxt2htm::HtmlDiv& = default;
#endif
    constexpr auto operator=(this HtmlDiv& self, ::pltxt2htm::HtmlDiv&&) noexcept -> ::pltxt2htm::HtmlDiv&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlDiv const&, HtmlDiv const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_left(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.margins.get_left();
    }

    [[nodiscard]]
    constexpr auto get_right(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.margins.get_right();
    }
};

/**
 * @brief HTML &lt;a href="URL"&gt; anchor/link node
 * @details Represents an HTML anchor element with href attribute.
 */
class HtmlA {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::Url url;
    bool internal;

public:
    constexpr explicit HtmlA(::pltxt2htm::Ast&& subast_, ::pltxt2htm::Url&& url_, bool internal_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlA(::pltxt2htm::HtmlA const&) noexcept = delete;
#else
    constexpr HtmlA(::pltxt2htm::HtmlA const&) noexcept = default;
#endif
    constexpr HtmlA(::pltxt2htm::HtmlA&&) noexcept;
    constexpr ~HtmlA() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlA const&) noexcept -> ::pltxt2htm::HtmlA& = delete;
#else
    constexpr auto operator=(this HtmlA& self, ::pltxt2htm::HtmlA const&) noexcept -> ::pltxt2htm::HtmlA& = default;
#endif
    constexpr auto operator=(this HtmlA& self, ::pltxt2htm::HtmlA&&) noexcept -> ::pltxt2htm::HtmlA&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlA const&, HtmlA const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_url(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url);
    }

    [[nodiscard]]
    constexpr auto get_internal(this HtmlA const& self) noexcept -> bool {
        return self.internal;
    }
};

/**
 * @brief HTML &lt;blockquote&gt; block quotation node
 */
class HtmlBlockquote {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit HtmlBlockquote(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote const&) noexcept = delete;
#else
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote const&) noexcept = default;
#endif
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote&&) noexcept;
    constexpr ~HtmlBlockquote() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::HtmlBlockquote const&) noexcept -> ::pltxt2htm::HtmlBlockquote& = delete;
#else
    constexpr auto operator=(this HtmlBlockquote& self, ::pltxt2htm::HtmlBlockquote const&) noexcept -> ::pltxt2htm::HtmlBlockquote& = default;
#endif
    constexpr auto operator=(this HtmlBlockquote& self, ::pltxt2htm::HtmlBlockquote&&) noexcept
        -> ::pltxt2htm::HtmlBlockquote&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlBlockquote const&, HtmlBlockquote const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
