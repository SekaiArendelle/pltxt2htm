#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::Text(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::Text(::pltxt2htm::Text<ndebug> const&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::Text(::pltxt2htm::Text<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::~Text() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Text<ndebug>::operator=(this ::pltxt2htm::Text<ndebug>& self,
                                                    ::pltxt2htm::Text<ndebug>&& other) noexcept
    -> ::pltxt2htm::Text<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Text<ndebug>::operator==(this ::pltxt2htm::Text<ndebug> const&,
                                                     ::pltxt2htm::Text<ndebug> const&) noexcept -> bool = default;

} // namespace pltxt2htm
