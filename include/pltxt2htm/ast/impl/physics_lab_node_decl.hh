#pragma once

#include <utility>
#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include "../../details/literal_string.hh"
#include "ast_decl.hh"
#include "basic_node_decl.hh"

namespace pltxt2htm::ast2 {

class PlColor {
    ::pltxt2htm::ast2::Ast subast;
    ::fast_io::u8string color_;

public:
    constexpr PlColor(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& color) noexcept;
    constexpr PlColor(PlColor const&) noexcept = delete;
    constexpr PlColor(PlColor&&) noexcept;
    constexpr ~PlColor() noexcept;
    constexpr auto operator=(PlColor const&) noexcept -> PlColor& = delete;
    constexpr auto operator=(PlColor&&) noexcept -> PlColor&;

    [[nodiscard]]
    constexpr auto&& get_color(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.color_);
    }
};

class PlA {
    ::pltxt2htm::ast2::Ast subast;
    static constexpr ::pltxt2htm::details::U8LiteralString<7> color_{u8"#0000AA"};

public:
    constexpr PlA(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr PlA(PlA const&) noexcept = delete;
    constexpr PlA(PlA&&) noexcept;
    constexpr ~PlA() noexcept;
    constexpr auto operator=(PlA const&) noexcept -> PlA& = delete;
    constexpr auto operator=(PlA&&) noexcept -> PlA&;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    // TODO maybe this can be gnu::const
    [[__gnu__::__pure__]]
#endif
    static constexpr auto const& get_color_literal() noexcept {
        return color_;
    }
};

class PlExperiment {
    ::pltxt2htm::ast2::Ast subast;
    ::fast_io::u8string id_;

public:
    constexpr PlExperiment(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlExperiment(PlExperiment const&) noexcept = delete;
    constexpr PlExperiment(PlExperiment&&) noexcept;
    constexpr ~PlExperiment() noexcept;
    constexpr auto operator=(PlExperiment const&) noexcept -> PlExperiment& = delete;
    constexpr auto operator=(PlExperiment&&) noexcept -> PlExperiment&;

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

class PlDiscussion {
    ::pltxt2htm::ast2::Ast subast;
    ::fast_io::u8string id_;

public:
    constexpr PlDiscussion(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlDiscussion(PlDiscussion const&) noexcept = delete;
    constexpr PlDiscussion(PlDiscussion&&) noexcept;
    constexpr ~PlDiscussion() noexcept;
    constexpr auto operator=(PlDiscussion const&) noexcept -> PlDiscussion& = delete;
    constexpr auto operator=(PlDiscussion&&) noexcept -> PlDiscussion&;

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

class PlUser {
    ::pltxt2htm::ast2::Ast subast;
    ::fast_io::u8string id_;

public:
    constexpr PlUser(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept;
    constexpr PlUser(PlUser const&) noexcept = delete;
    constexpr PlUser(PlUser&&) noexcept;
    constexpr ~PlUser() noexcept;
    constexpr auto operator=(PlUser const&) noexcept -> PlUser& = delete;
    constexpr auto operator=(PlUser&&) noexcept -> PlUser&;

    [[nodiscard]]
    constexpr auto&& get_id(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.id_);
    }
};

class PlExternal {
    ::pltxt2htm::ast2::Ast subast;
    ::pltxt2htm::ast2::Url url_;

public:
    constexpr PlExternal(::pltxt2htm::ast2::Ast&& subast, ::pltxt2htm::ast2::Url&& url) noexcept;
    constexpr PlExternal(PlExternal const&) noexcept = delete;
    constexpr PlExternal(PlExternal&&) noexcept;
    constexpr ~PlExternal() noexcept;
    constexpr auto operator=(PlExternal const&) noexcept -> PlExternal& = delete;
    constexpr auto operator=(PlExternal&&) noexcept -> PlExternal&;

    [[nodiscard]]
    constexpr auto&& get_url(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_);
    }
};

class PlSize {
    ::pltxt2htm::ast2::Ast subast;
    ::std::size_t size_;

public:
    constexpr PlSize(::pltxt2htm::ast2::Ast&& subast, ::std::size_t size) noexcept;
    constexpr PlSize(PlSize const&) noexcept = delete;
    constexpr PlSize(PlSize&&) noexcept;
    constexpr ~PlSize() noexcept;
    constexpr auto operator=(PlSize const&) noexcept -> PlSize& = delete;
    constexpr auto operator=(PlSize&&) noexcept -> PlSize&;

    [[nodiscard]]
    constexpr auto&& get_size(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.size_);
    }
};

class PlI {
    ::pltxt2htm::ast2::Ast subast;

public:
    constexpr PlI(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr PlI(PlI const&) noexcept = delete;
    constexpr PlI(PlI&&) noexcept;
    constexpr ~PlI() noexcept;
    constexpr auto operator=(PlI const&) noexcept -> PlI& = delete;
    constexpr auto operator=(PlI&&) noexcept -> PlI&;
};

class PlB {
    ::pltxt2htm::ast2::Ast subast;

public:
    constexpr PlB(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr PlB(PlB const&) noexcept = delete;
    constexpr PlB(PlB&&) noexcept;
    constexpr ~PlB() noexcept;
    constexpr auto operator=(PlB const&) noexcept -> PlB& = delete;
    constexpr auto operator=(PlB&&) noexcept -> PlB&;
};

class PlMacroProject {};

class PlMacroVisitor {};

class PlMacroAuthor {};

class PlMacroCoauthors {};

} // namespace pltxt2htm::ast2
