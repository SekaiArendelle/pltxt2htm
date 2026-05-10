#pragma once

#include "node_decl.hh"

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

} // namespace pltxt2htm::ast2
