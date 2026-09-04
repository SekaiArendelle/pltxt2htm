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
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "ast_decl.hh"
#include "basic_node_decl.hh"
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
 * @brief HTML &lt;img&gt; image node (self-closing)
 * @details Represents &lt;img src=&quot;...&quot; alt=&quot;...&quot;&gt;.
 */
class HtmlImg {
    ::fast_io::u8string src;
    ::fast_io::u8string alt;

public:
    constexpr HtmlImg(::fast_io::u8string&& src_, ::fast_io::u8string&& alt_) noexcept
        : src(::std::move(src_)),
          alt(::std::move(alt_)) {
    }

    constexpr HtmlImg(::pltxt2htm::HtmlImg const&) noexcept = default;
    constexpr HtmlImg(::pltxt2htm::HtmlImg&&) noexcept = default;
    constexpr ~HtmlImg() noexcept = default;
    constexpr auto operator=(::pltxt2htm::HtmlImg const&) noexcept -> ::pltxt2htm::HtmlImg& = delete;
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
template<::pltxt2htm::Contracts ndebug>
class HtmlH1 {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH1(::pltxt2htm::HtmlH1<ndebug> const&) noexcept;
    constexpr HtmlH1(::pltxt2htm::HtmlH1<ndebug>&&) noexcept;
    constexpr ~HtmlH1() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH1<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH1<ndebug>& = delete;
    constexpr auto operator=(this HtmlH1<ndebug>& self, ::pltxt2htm::HtmlH1<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH2(::pltxt2htm::HtmlH2<ndebug> const&) noexcept;
    constexpr HtmlH2(::pltxt2htm::HtmlH2<ndebug>&&) noexcept;
    constexpr ~HtmlH2() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH2<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH2<ndebug>& = delete;
    constexpr auto operator=(this HtmlH2<ndebug>& self, ::pltxt2htm::HtmlH2<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH3(::pltxt2htm::HtmlH3<ndebug> const&) noexcept;
    constexpr HtmlH3(::pltxt2htm::HtmlH3<ndebug>&&) noexcept;
    constexpr ~HtmlH3() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH3<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH3<ndebug>& = delete;
    constexpr auto operator=(this HtmlH3<ndebug>& self, ::pltxt2htm::HtmlH3<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH4(::pltxt2htm::HtmlH4<ndebug> const&) noexcept;
    constexpr HtmlH4(::pltxt2htm::HtmlH4<ndebug>&&) noexcept;
    constexpr ~HtmlH4() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH4<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH4<ndebug>& = delete;
    constexpr auto operator=(this HtmlH4<ndebug>& self, ::pltxt2htm::HtmlH4<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH5(::pltxt2htm::HtmlH5<ndebug> const&) noexcept;
    constexpr HtmlH5(::pltxt2htm::HtmlH5<ndebug>&&) noexcept;
    constexpr ~HtmlH5() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH5<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH5<ndebug>& = delete;
    constexpr auto operator=(this HtmlH5<ndebug>& self, ::pltxt2htm::HtmlH5<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlH6(::pltxt2htm::HtmlH6<ndebug> const&) noexcept;
    constexpr HtmlH6(::pltxt2htm::HtmlH6<ndebug>&&) noexcept;
    constexpr ~HtmlH6() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlH6<ndebug> const&) noexcept -> ::pltxt2htm::HtmlH6<ndebug>& = delete;
    constexpr auto operator=(this HtmlH6<ndebug>& self, ::pltxt2htm::HtmlH6<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::TextAlign align;

public:
    constexpr HtmlP(::pltxt2htm::Ast<ndebug>&& subast_,
                    ::pltxt2htm::TextAlign align_ = ::pltxt2htm::TextAlign::left) noexcept;
    constexpr HtmlP(::pltxt2htm::HtmlP<ndebug> const&) noexcept;
    constexpr HtmlP(::pltxt2htm::HtmlP<ndebug>&&) noexcept;
    constexpr ~HtmlP() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlP<ndebug> const&) noexcept -> ::pltxt2htm::HtmlP<ndebug>& = delete;
    constexpr auto operator=(this HtmlP<ndebug>& self, ::pltxt2htm::HtmlP<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlP<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlDel {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlDel(::pltxt2htm::HtmlDel<ndebug> const&) noexcept;
    constexpr HtmlDel(::pltxt2htm::HtmlDel<ndebug>&&) noexcept;
    constexpr ~HtmlDel() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlDel<ndebug> const&) noexcept -> ::pltxt2htm::HtmlDel<ndebug>& = delete;
    constexpr auto operator=(this HtmlDel<ndebug>& self, ::pltxt2htm::HtmlDel<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlDel<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlU {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlU(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlU(::pltxt2htm::HtmlU<ndebug> const&) noexcept;
    constexpr HtmlU(::pltxt2htm::HtmlU<ndebug>&&) noexcept;
    constexpr ~HtmlU() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlU<ndebug> const&) noexcept -> ::pltxt2htm::HtmlU<ndebug>& = delete;
    constexpr auto operator=(this HtmlU<ndebug>& self, ::pltxt2htm::HtmlU<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlU<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlS {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlS(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlS(::pltxt2htm::HtmlS<ndebug> const&) noexcept;
    constexpr HtmlS(::pltxt2htm::HtmlS<ndebug>&&) noexcept;
    constexpr ~HtmlS() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlS<ndebug> const&) noexcept -> ::pltxt2htm::HtmlS<ndebug>& = delete;
    constexpr auto operator=(this HtmlS<ndebug>& self, ::pltxt2htm::HtmlS<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlS<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlSup {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr HtmlSup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlSup(::pltxt2htm::HtmlSup<ndebug> const&) noexcept;
    constexpr HtmlSup(::pltxt2htm::HtmlSup<ndebug>&&) noexcept;
    constexpr ~HtmlSup() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlSup<ndebug> const&) noexcept -> ::pltxt2htm::HtmlSup<ndebug>& = delete;
    constexpr auto operator=(this HtmlSup<ndebug>& self, ::pltxt2htm::HtmlSup<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlSup<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlSub {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr HtmlSub(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlSub(::pltxt2htm::HtmlSub<ndebug> const&) noexcept;
    constexpr HtmlSub(::pltxt2htm::HtmlSub<ndebug>&&) noexcept;
    constexpr ~HtmlSub() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlSub<ndebug> const&) noexcept -> ::pltxt2htm::HtmlSub<ndebug>& = delete;
    constexpr auto operator=(this HtmlSub<ndebug>& self, ::pltxt2htm::HtmlSub<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlSub<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlNote {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlNote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlNote(::pltxt2htm::HtmlNote<ndebug> const&) noexcept;
    constexpr HtmlNote(::pltxt2htm::HtmlNote<ndebug>&&) noexcept;
    constexpr ~HtmlNote() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlNote<ndebug> const&) noexcept -> ::pltxt2htm::HtmlNote<ndebug>& = delete;
    constexpr auto operator=(this HtmlNote<ndebug>& self, ::pltxt2htm::HtmlNote<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlEm(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlEm(::pltxt2htm::HtmlEm<ndebug> const&) noexcept;
    constexpr HtmlEm(::pltxt2htm::HtmlEm<ndebug>&&) noexcept;
    constexpr ~HtmlEm() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlEm<ndebug> const&) noexcept -> ::pltxt2htm::HtmlEm<ndebug>& = delete;
    constexpr auto operator=(this HtmlEm<ndebug>& self, ::pltxt2htm::HtmlEm<ndebug>&&) noexcept
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
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlStrong(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong<ndebug> const&) noexcept;
    constexpr HtmlStrong(::pltxt2htm::HtmlStrong<ndebug>&&) noexcept;
    constexpr ~HtmlStrong() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlStrong<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlStrong<ndebug>& = delete;
    constexpr auto operator=(this HtmlStrong<ndebug>& self, ::pltxt2htm::HtmlStrong<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlStrong<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlMark {
    ::pltxt2htm::Ast<ndebug> subast;
    ::fast_io::u8string background_color;

public:
    static constexpr auto default_background_color = ::pltxt2htm::container::U8StringView{u8"#FFFF00"};

    constexpr explicit HtmlMark(::pltxt2htm::Ast<ndebug>&& subast_,
                                ::fast_io::u8string&& background_color_ = ::fast_io::u8string{
                                    default_background_color}) noexcept;
    constexpr HtmlMark(HtmlMark<ndebug> const&) noexcept;
    constexpr HtmlMark(HtmlMark<ndebug>&&) noexcept;
    constexpr ~HtmlMark() noexcept;
    constexpr auto operator=(HtmlMark<ndebug> const&) noexcept -> HtmlMark<ndebug>& = delete;
    constexpr auto operator=(this HtmlMark<ndebug>& self, HtmlMark<ndebug>&&) noexcept -> HtmlMark<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlCode {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlCode(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlCode(::pltxt2htm::HtmlCode<ndebug> const&) noexcept;
    constexpr HtmlCode(::pltxt2htm::HtmlCode<ndebug>&&) noexcept;
    constexpr ~HtmlCode() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlCode<ndebug> const&) noexcept -> ::pltxt2htm::HtmlCode<ndebug>& = delete;
    constexpr auto operator=(this HtmlCode<ndebug>& self, ::pltxt2htm::HtmlCode<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlCode<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlSpan {
    /**
     * @brief Compact storage for the optional span style attributes.
     * @details Semantically equivalent to the clearer but larger representation:
     * @code
     * Optional<ValueWithUnit<double>> font_size;
     * Optional<VerticalAlignValue<ndebug>> vertical_align;
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
            ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> const& vertical_align) noexcept {
            if (font_size.has_value()) {
                auto const& value = font_size.template value<ndebug>();
                font_size_value = value.value;
                font_size_unit = value.unit;
                has_font_size = true;
            }
            if (vertical_align.has_value() == false) {
                return;
            }
            auto const& value = vertical_align.template value<ndebug>();
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
            -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> {
            switch (self.vertical_align_kind) /* -Werror=switch */ {
            case VerticalAlignStorageKind::none: {
                return ::pltxt2htm::container::nullopt;
            }
            case VerticalAlignStorageKind::keyword_baseline: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::baseline};
            }
            case VerticalAlignStorageKind::keyword_sub: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::sub};
            }
            case VerticalAlignStorageKind::keyword_super: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::super};
            }
            case VerticalAlignStorageKind::keyword_text_top: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::text_top};
            }
            case VerticalAlignStorageKind::keyword_text_bottom: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::text_bottom};
            }
            case VerticalAlignStorageKind::keyword_middle: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::middle};
            }
            case VerticalAlignStorageKind::keyword_top: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::top};
            }
            case VerticalAlignStorageKind::keyword_bottom: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::bottom};
            }
            case VerticalAlignStorageKind::length_px: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
                    .value = self.vertical_align_length, .unit = ::pltxt2htm::Unit::px}};
            }
            case VerticalAlignStorageKind::length_percent: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
                    .value = self.vertical_align_length, .unit = ::pltxt2htm::Unit::percent}};
            }
            case VerticalAlignStorageKind::length_em: {
                return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{
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

    ::pltxt2htm::Ast<ndebug> subast;
    ::fast_io::u8string color;
    StyleStorage style;

public:
    constexpr HtmlSpan(
        ::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_,
        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_,
        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align_) noexcept;
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept;
    constexpr HtmlSpan(::pltxt2htm::HtmlSpan<ndebug>&&) noexcept;
    constexpr ~HtmlSpan() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept -> ::pltxt2htm::HtmlSpan<ndebug>& = delete;
    constexpr auto operator=(this HtmlSpan<ndebug>& self, ::pltxt2htm::HtmlSpan<ndebug>&&) noexcept
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

    constexpr auto get_font_size(this HtmlSpan<ndebug> const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> {
        return self.style.get_font_size();
    }

    constexpr auto get_vertical_align(this HtmlSpan<ndebug> const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> {
        return self.style.get_vertical_align();
    }
};

/**
 * @brief HTML &lt;div style="margin-left:...;margin-right:..."&gt; node
 * @details Represents an HTML div element whose only allowed style attributes are
 *          margin-left and margin-right (each an optional px/em/% value). Any other
 *          attribute or CSS declaration is rejected by the parser to prevent XSS.
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlDiv {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;

public:
    constexpr HtmlDiv(::pltxt2htm::Ast<ndebug>&& subast_,
                      ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
                      ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept;
    constexpr HtmlDiv(::pltxt2htm::HtmlDiv<ndebug> const&) noexcept;
    constexpr HtmlDiv(::pltxt2htm::HtmlDiv<ndebug>&&) noexcept;
    constexpr ~HtmlDiv() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlDiv<ndebug> const&) noexcept -> ::pltxt2htm::HtmlDiv<ndebug>& = delete;
    constexpr auto operator=(this HtmlDiv<ndebug>& self, ::pltxt2htm::HtmlDiv<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlDiv<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlDiv const&, HtmlDiv const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_left(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.left;
    }

    [[nodiscard]]
    constexpr auto get_right(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> {
        return self.right;
    }
};

/**
 * @brief HTML &lt;a href="URL"&gt; anchor/link node
 * @details Represents an HTML anchor element with href attribute.
 */
template<::pltxt2htm::Contracts ndebug>
class HtmlA {
    ::pltxt2htm::Ast<ndebug> subast;
    ::pltxt2htm::Url url;
    bool internal;

public:
    constexpr explicit HtmlA(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_, bool internal_) noexcept;
    constexpr HtmlA(::pltxt2htm::HtmlA<ndebug> const&) noexcept;
    constexpr HtmlA(::pltxt2htm::HtmlA<ndebug>&&) noexcept;
    constexpr ~HtmlA() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlA<ndebug> const&) noexcept -> ::pltxt2htm::HtmlA<ndebug>& = delete;
    constexpr auto operator=(this HtmlA<ndebug>& self, ::pltxt2htm::HtmlA<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlA<ndebug>&;

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
template<::pltxt2htm::Contracts ndebug>
class HtmlBlockquote {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit HtmlBlockquote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept;
    constexpr HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept;
    constexpr ~HtmlBlockquote() noexcept;
    constexpr auto operator=(::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept
        -> ::pltxt2htm::HtmlBlockquote<ndebug>& = delete;
    constexpr auto operator=(this HtmlBlockquote<ndebug>& self, ::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept
        -> ::pltxt2htm::HtmlBlockquote<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this HtmlBlockquote const&, HtmlBlockquote const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
