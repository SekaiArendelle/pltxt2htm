#pragma once

#include "ast_decl.hh"

namespace pltxt2htm::ast2 {

class PlColor {
    ::pltxt2htm::ast2::Ast subast;

public:
    constexpr PlColor(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr PlColor(::pltxt2htm::ast2::PlColor const&) noexcept = delete;
    constexpr PlColor(::pltxt2htm::ast2::PlColor&&) noexcept;
    constexpr ~PlColor() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::PlColor const&) noexcept -> ::pltxt2htm::ast2::PlColor& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::PlColor&&) noexcept -> ::pltxt2htm::ast2::PlColor&;
};

} // namespace pltxt2htm::ast2
