#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Text<ndebug>::Text(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Text<ndebug>::Text(::pltxt2htm::ast2::Text<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Text<ndebug>::~Text() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Text<ndebug>::operator=(::pltxt2htm::ast2::Text<ndebug>&& other) noexcept
    -> ::pltxt2htm::ast2::Text<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Url<ndebug>::Url(::pltxt2htm::ast2::Ast<ndebug>&& attr) noexcept
    : url_ast_(::std::move(attr)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Url<ndebug>::Url(::pltxt2htm::ast2::Url<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Url<ndebug>::~Url() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Url<ndebug>::operator=(::pltxt2htm::ast2::Url<ndebug>&& other) noexcept -> ::pltxt2htm::ast2::Url<ndebug>& = default;

} // namespace pltxt2htm::ast2
