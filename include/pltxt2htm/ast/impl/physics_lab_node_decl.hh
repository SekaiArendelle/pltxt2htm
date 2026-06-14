/**
 * @file physics_lab_node_decl.hh
 * @brief Physics-Lab specific AST node declarations for pltxt2htm
 * @details Defines nodes for Physics-Lab specific tags: color, anchor,
 *          experiment, discussion, user, external link, font size,
 *          bold/italic formatting, and template macros.
 */

#pragma once

#include <utility>
#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "../../details/literal_string.hh"
#include "ast_decl.hh"
#include "basic_node_decl.hh"
#include "../../contracts.hh"

namespace pltxt2htm {

/**
 * @brief Physics-Lab color tag node
 * @details Represents &lt;color=value&gt;...&lt;/color&gt; with a color string and sub-AST.
 */
template<::pltxt2htm::Contracts ndebug>
class PlColor {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::fast_io::u8string color;

public:
    constexpr PlColor(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_) noexcept;
    constexpr PlColor(::pltxt2htm::PlColor<ndebug> const&) noexcept;
    constexpr PlColor(::pltxt2htm::PlColor<ndebug>&&) noexcept;
    constexpr ~PlColor() noexcept;
    constexpr auto operator=(::pltxt2htm::PlColor<ndebug> const&) noexcept -> ::pltxt2htm::PlColor<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlColor<ndebug>& self, ::pltxt2htm::PlColor<ndebug>&&) noexcept
        -> ::pltxt2htm::PlColor<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlColor const&, PlColor const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_color(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.color);
    }
};

/**
 * @brief Physics-Lab anchor tag node
 * @details Represents &lt;a&gt;...&lt;/a&gt; with a fixed blue color used for styled links.
 */
template<::pltxt2htm::Contracts ndebug>
class PlA {
    ::pltxt2htm::Ast<ndebug> subast{};
    static constexpr ::pltxt2htm::details::U8LiteralString<7> color_{u8"#0000AA"};

public:
    constexpr PlA(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr PlA(::pltxt2htm::PlA<ndebug> const&) noexcept;
    constexpr PlA(::pltxt2htm::PlA<ndebug>&&) noexcept;
    constexpr ~PlA() noexcept;
    constexpr auto operator=(::pltxt2htm::PlA<ndebug> const&) noexcept -> ::pltxt2htm::PlA<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlA<ndebug>& self, ::pltxt2htm::PlA<ndebug>&&) noexcept
        -> ::pltxt2htm::PlA<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlA const&, PlA const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    static constexpr auto const& get_color_literal() noexcept {
        return color_;
    }
};

/**
 * @brief Physics-Lab experiment reference tag node
 * @details Represents &lt;experiment=id&gt;...&lt;/experiment&gt; with an experiment ID.
 */
template<::pltxt2htm::Contracts ndebug>
class PlExperiment {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::fast_io::u8string id;

public:
    constexpr PlExperiment(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& id_) noexcept;
    constexpr PlExperiment(::pltxt2htm::PlExperiment<ndebug> const&) noexcept;
    constexpr PlExperiment(::pltxt2htm::PlExperiment<ndebug>&&) noexcept;
    constexpr ~PlExperiment() noexcept;
    constexpr auto operator=(::pltxt2htm::PlExperiment<ndebug> const&) noexcept
        -> ::pltxt2htm::PlExperiment<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlExperiment<ndebug>& self, ::pltxt2htm::PlExperiment<ndebug>&&) noexcept
        -> ::pltxt2htm::PlExperiment<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlExperiment const&, PlExperiment const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab discussion reference tag node
 * @details Represents &lt;discussion=id&gt;...&lt;/discussion&gt; with a discussion ID.
 */
template<::pltxt2htm::Contracts ndebug>
class PlDiscussion {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::fast_io::u8string id;

public:
    constexpr PlDiscussion(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& id_) noexcept;
    constexpr PlDiscussion(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept;
    constexpr PlDiscussion(::pltxt2htm::PlDiscussion<ndebug>&&) noexcept;
    constexpr ~PlDiscussion() noexcept;
    constexpr auto operator=(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept
        -> ::pltxt2htm::PlDiscussion<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlDiscussion<ndebug>& self, ::pltxt2htm::PlDiscussion<ndebug>&&) noexcept
        -> ::pltxt2htm::PlDiscussion<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlDiscussion const&, PlDiscussion const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab user reference tag node
 * @details Represents &lt;user=id&gt;...&lt;/user&gt; with a user ID.
 */
template<::pltxt2htm::Contracts ndebug>
class PlUser {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::fast_io::u8string id;

public:
    constexpr PlUser(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& id_) noexcept;
    constexpr PlUser(::pltxt2htm::PlUser<ndebug> const&) noexcept;
    constexpr PlUser(::pltxt2htm::PlUser<ndebug>&&) noexcept;
    constexpr ~PlUser() noexcept;
    constexpr auto operator=(::pltxt2htm::PlUser<ndebug> const&) noexcept -> ::pltxt2htm::PlUser<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlUser<ndebug>& self, ::pltxt2htm::PlUser<ndebug>&&) noexcept
        -> ::pltxt2htm::PlUser<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlUser const&, PlUser const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_id(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.id);
    }
};

/**
 * @brief Physics-Lab external link tag node
 * @details Represents &lt;external=url&gt;...&lt;/external&gt; with a URL.
 */
template<::pltxt2htm::Contracts ndebug>
class PlExternal {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::pltxt2htm::Url<ndebug> url;

public:
    constexpr PlExternal(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url<ndebug>&& url_) noexcept;
    constexpr PlExternal(::pltxt2htm::PlExternal<ndebug> const&) noexcept;
    constexpr PlExternal(::pltxt2htm::PlExternal<ndebug>&&) noexcept;
    constexpr ~PlExternal() noexcept;
    constexpr auto operator=(::pltxt2htm::PlExternal<ndebug> const&) noexcept
        -> ::pltxt2htm::PlExternal<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlExternal<ndebug>& self, ::pltxt2htm::PlExternal<ndebug>&&) noexcept
        -> ::pltxt2htm::PlExternal<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlExternal const&, PlExternal const&) noexcept -> bool;

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
 * @brief Physics-Lab font size tag node
 * @details Represents &lt;size=value&gt;...&lt;/size&gt; with a font size value.
 */
template<::pltxt2htm::Contracts ndebug>
class PlSize {
    ::pltxt2htm::Ast<ndebug> subast{};
    ::std::size_t size;

public:
    constexpr PlSize(::pltxt2htm::Ast<ndebug>&& subast_, ::std::size_t size_) noexcept;
    constexpr PlSize(::pltxt2htm::PlSize<ndebug> const&) noexcept;
    constexpr PlSize(::pltxt2htm::PlSize<ndebug>&&) noexcept;
    constexpr ~PlSize() noexcept;
    constexpr auto operator=(::pltxt2htm::PlSize<ndebug> const&) noexcept -> ::pltxt2htm::PlSize<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlSize<ndebug>& self, ::pltxt2htm::PlSize<ndebug>&&) noexcept
        -> ::pltxt2htm::PlSize<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlSize const&, PlSize const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_size(this auto&& self) noexcept -> ::std::size_t {
        return self.size;
    }
};

/**
 * @brief Physics-Lab italic text tag node
 * @details Represents &lt;i&gt;...&lt;/i&gt; with sub-AST content.
 */
template<::pltxt2htm::Contracts ndebug>
class PlI {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr PlI(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr PlI(::pltxt2htm::PlI<ndebug> const&) noexcept;
    constexpr PlI(::pltxt2htm::PlI<ndebug>&&) noexcept;
    constexpr ~PlI() noexcept;
    constexpr auto operator=(::pltxt2htm::PlI<ndebug> const&) noexcept -> ::pltxt2htm::PlI<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlI<ndebug>& self, ::pltxt2htm::PlI<ndebug>&&) noexcept
        -> ::pltxt2htm::PlI<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlI const&, PlI const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Physics-Lab bold text tag node
 * @details Represents &lt;b&gt;...&lt;/b&gt; with sub-AST content.
 */
template<::pltxt2htm::Contracts ndebug>
class PlB {
    ::pltxt2htm::Ast<ndebug> subast{};

public:
    constexpr PlB(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr PlB(::pltxt2htm::PlB<ndebug> const&) noexcept;
    constexpr PlB(::pltxt2htm::PlB<ndebug>&&) noexcept;
    constexpr ~PlB() noexcept;
    constexpr auto operator=(::pltxt2htm::PlB<ndebug> const&) noexcept -> ::pltxt2htm::PlB<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::PlB<ndebug>& self, ::pltxt2htm::PlB<ndebug>&&) noexcept
        -> ::pltxt2htm::PlB<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this PlB const&, PlB const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Physics-Lab {Project} macro node
 * @details Represents the {Project} template placeholder.
 */
class PlMacroProject {
public:
    constexpr auto operator==(this PlMacroProject const&, PlMacroProject const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {Visitor} macro node
 * @details Represents the {Visitor} template placeholder.
 */
class PlMacroVisitor {
public:
    constexpr auto operator==(this PlMacroVisitor const&, PlMacroVisitor const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {Author} macro node
 * @details Represents the {Author} template placeholder.
 */
class PlMacroAuthor {
public:
    constexpr auto operator==(this PlMacroAuthor const&, PlMacroAuthor const&) noexcept -> bool = default;
};

/**
 * @brief Physics-Lab {CoAuthors} macro node
 * @details Represents the {CoAuthors} template placeholder.
 */
class PlMacroCoauthors {
public:
    constexpr auto operator==(this PlMacroCoauthors const&, PlMacroCoauthors const&) noexcept -> bool = default;
};

} // namespace pltxt2htm
