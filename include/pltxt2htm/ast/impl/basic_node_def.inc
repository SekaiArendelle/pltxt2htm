#pragma once

namespace pltxt2htm::ast2 {

constexpr Text::Text(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast(::std::move(subast)) {
}

constexpr Text::Text(::pltxt2htm::ast2::Text&& other) noexcept = default;

constexpr Text::~Text() noexcept = default;

constexpr auto Text::operator=(::pltxt2htm::ast2::Text&& other) noexcept -> ::pltxt2htm::ast2::Text& = default;

} // namespace pltxt2htm::ast2
