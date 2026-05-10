#pragma once

namespace pltxt2htm::ast2 {

constexpr PlColor::PlColor(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast(::std::move(subast)) {}

constexpr PlColor::PlColor(::pltxt2htm::ast2::PlColor&& other) noexcept = default;

constexpr PlColor::~PlColor() noexcept = default;

}
