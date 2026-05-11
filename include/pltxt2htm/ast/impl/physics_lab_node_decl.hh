#pragma once

#include <utility>
#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "../../details/literal_string.hh"
#include "ast_decl.hh"
#include "basic_node_decl.hh"
#include "pltxt2htm/contracts.hh"

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
class PlColor {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::fast_io::u8string color_;

public:
    constexpr PlColor(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& color) noexcept;
    constexpr PlColor(::pltxt2htm::ast2::PlColor<ndebug> const&) noexcept = delete;
    constexpr PlColor(::pltxt2htm::ast2::PlColor<ndebug>&&) noexcept;
    constexpr ~PlColor() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlColor<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlColor<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlColor<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlColor<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlA {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    static constexpr ::pltxt2htm::details::U8LiteralString<7> color_{u8"#0000AA"};

public:
    constexpr PlA(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr PlA(::pltxt2htm::ast2::PlA<ndebug> const&) noexcept = delete;
    constexpr PlA(::pltxt2htm::ast2::PlA<ndebug>&&) noexcept;
    constexpr ~PlA() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlA<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlA<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlA<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlA<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    // TODO maybe this can be gnu::const
    [[__gnu__::__pure__]]
#endif
    static constexpr auto const& get_color_literal() noexcept {
        return color_;
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlExperiment {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::fast_io::u8string id_;

public:
    constexpr PlExperiment(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlExperiment(::pltxt2htm::ast2::PlExperiment<ndebug> const&) noexcept = delete;
    constexpr PlExperiment(::pltxt2htm::ast2::PlExperiment<ndebug>&&) noexcept;
    constexpr ~PlExperiment() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlExperiment<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlExperiment<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlExperiment<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlExperiment<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlDiscussion {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::fast_io::u8string id_;

public:
    constexpr PlDiscussion(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlDiscussion(::pltxt2htm::ast2::PlDiscussion<ndebug> const&) noexcept = delete;
    constexpr PlDiscussion(::pltxt2htm::ast2::PlDiscussion<ndebug>&&) noexcept;
    constexpr ~PlDiscussion() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlDiscussion<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlDiscussion<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlDiscussion<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlDiscussion<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlUser {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::fast_io::u8string id_;

public:
    constexpr PlUser(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlUser(::pltxt2htm::ast2::PlUser<ndebug> const&) noexcept = delete;
    constexpr PlUser(::pltxt2htm::ast2::PlUser<ndebug>&&) noexcept;
    constexpr ~PlUser() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlUser<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlUser<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlUser<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlUser<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlExternal {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::pltxt2htm::ast2::Url<ndebug> url_;

public:
    constexpr PlExternal(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept;
    constexpr PlExternal(::pltxt2htm::ast2::PlExternal<ndebug> const&) noexcept = delete;
    constexpr PlExternal(::pltxt2htm::ast2::PlExternal<ndebug>&&) noexcept;
    constexpr ~PlExternal() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlExternal<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlExternal<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlExternal<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlExternal<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_url(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlSize {
    ::pltxt2htm::ast2::Ast<ndebug> subast;
    ::std::size_t size_;

public:
    constexpr PlSize(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::std::size_t size) noexcept;
    constexpr PlSize(::pltxt2htm::ast2::PlSize<ndebug> const&) noexcept = delete;
    constexpr PlSize(::pltxt2htm::ast2::PlSize<ndebug>&&) noexcept;
    constexpr ~PlSize() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlSize<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlSize<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlSize<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlSize<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto&& get_size(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.size_);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlI {
    ::pltxt2htm::ast2::Ast<ndebug> subast;

public:
    constexpr PlI(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr PlI(::pltxt2htm::ast2::PlI<ndebug> const&) noexcept = delete;
    constexpr PlI(::pltxt2htm::ast2::PlI<ndebug>&&) noexcept;
    constexpr ~PlI() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlI<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlI<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlI<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlI<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

template<::pltxt2htm::Contracts ndebug>
class PlB {
    ::pltxt2htm::ast2::Ast<ndebug> subast;

public:
    constexpr PlB(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr PlB(::pltxt2htm::ast2::PlB<ndebug> const&) noexcept = delete;
    constexpr PlB(::pltxt2htm::ast2::PlB<ndebug>&&) noexcept;
    constexpr ~PlB() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlB<ndebug> const&) noexcept -> ::pltxt2htm::ast2::PlB<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlB<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlB<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

class PlMacroProject {};

class PlMacroVisitor {};

class PlMacroAuthor {};

class PlMacroCoauthors {};

} // namespace pltxt2htm::ast2
