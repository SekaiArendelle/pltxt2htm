#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::Text(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::Text(::pltxt2htm::Text<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Text<ndebug>::~Text() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Text<ndebug>::operator=(this ::pltxt2htm::Text<ndebug>& self,
                                                    ::pltxt2htm::Text<ndebug>&& other) noexcept
    -> ::pltxt2htm::Text<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Text<ndebug>::operator==(this ::pltxt2htm::Text<ndebug> const&, ::pltxt2htm::Text<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Url<ndebug>::Url(::pltxt2htm::Ast<ndebug>&& attr) noexcept
    : url_ast(::std::move(attr)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Url<ndebug>::Url(::pltxt2htm::Url<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Url<ndebug>::~Url() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Url<ndebug>::operator=(this ::pltxt2htm::Url<ndebug>& self,
                                                   ::pltxt2htm::Url<ndebug>&& other) noexcept
    -> ::pltxt2htm::Url<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Url<ndebug>::operator==(this ::pltxt2htm::Url<ndebug> const&, ::pltxt2htm::Url<ndebug> const&) noexcept -> bool = default;

} // namespace pltxt2htm
