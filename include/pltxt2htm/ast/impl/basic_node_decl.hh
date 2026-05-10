#pragma once

#include <utility>
#include "ast_decl.hh"

namespace pltxt2htm::ast2 {

class U8Char {
    char8_t chr;
};

class InvalidU8Char {};

class Text {
    ::pltxt2htm::ast2::Ast subast;

public:
    constexpr Text(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr Text(::pltxt2htm::ast2::Text const&) noexcept = delete;
    constexpr Text(::pltxt2htm::ast2::Text&&) noexcept;
    constexpr ~Text() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Text const&) noexcept -> ::pltxt2htm::ast2::Text& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Text&&) noexcept -> ::pltxt2htm::ast2::Text&;
};

class Url {
    ::pltxt2htm::ast2::Ast url_ast_{};

public:
    constexpr Url() noexcept;
    constexpr explicit Url(::pltxt2htm::ast2::Ast&& attr) noexcept;
    constexpr Url(Url const&) noexcept = delete;
    constexpr Url(Url&&) noexcept;
    constexpr ~Url() noexcept;
    constexpr auto operator=(Url const&) noexcept -> Url& = delete;
    constexpr auto operator=(Url&&) noexcept -> Url&;

    [[nodiscard]]
    constexpr auto&& get_url_ast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_ast_);
    }
};

} // namespace pltxt2htm::ast2
