#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListUl<ndebug>::ListUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListUl<ndebug>::ListUl(::pltxt2htm::ListUl<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListUl<ndebug>::ListUl(::pltxt2htm::ListUl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListUl<ndebug>::~ListUl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListUl<ndebug>::operator=(this ::pltxt2htm::ListUl<ndebug>& self,
                                                      ::pltxt2htm::ListUl<ndebug>&&) noexcept
    -> ::pltxt2htm::ListUl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListUl<ndebug>::operator==(this ::pltxt2htm::ListUl<ndebug> const&,
                                                       ::pltxt2htm::ListUl<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListOl<ndebug>::ListOl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListOl<ndebug>::ListOl(::pltxt2htm::ListOl<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListOl<ndebug>::ListOl(::pltxt2htm::ListOl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListOl<ndebug>::~ListOl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListOl<ndebug>::operator=(this ::pltxt2htm::ListOl<ndebug>& self,
                                                      ::pltxt2htm::ListOl<ndebug>&&) noexcept
    -> ::pltxt2htm::ListOl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListOl<ndebug>::operator==(this ::pltxt2htm::ListOl<ndebug> const&,
                                                       ::pltxt2htm::ListOl<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLi<ndebug>::ListLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLi<ndebug>::ListLi(::pltxt2htm::ListLi<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLi<ndebug>::ListLi(::pltxt2htm::ListLi<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLi<ndebug>::~ListLi() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListLi<ndebug>::operator=(this ::pltxt2htm::ListLi<ndebug>& self,
                                                      ::pltxt2htm::ListLi<ndebug>&&) noexcept
    -> ::pltxt2htm::ListLi<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListLi<ndebug>::operator==(this ::pltxt2htm::ListLi<ndebug> const&,
                                                       ::pltxt2htm::ListLi<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLiCheckbox<ndebug>::ListLiCheckbox(::pltxt2htm::Ast<ndebug>&& subast_,
                                                              bool checked_) noexcept
    : subast(::std::move(subast_)),
      checked(checked_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLiCheckbox<ndebug>::ListLiCheckbox(::pltxt2htm::ListLiCheckbox<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLiCheckbox<ndebug>::ListLiCheckbox(::pltxt2htm::ListLiCheckbox<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ListLiCheckbox<ndebug>::~ListLiCheckbox() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListLiCheckbox<ndebug>::operator=(this ::pltxt2htm::ListLiCheckbox<ndebug>& self,
                                                              ::pltxt2htm::ListLiCheckbox<ndebug>&&) noexcept
    -> ::pltxt2htm::ListLiCheckbox<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ListLiCheckbox<ndebug>::operator==(this ::pltxt2htm::ListLiCheckbox<ndebug> const&,
                                                               ::pltxt2htm::ListLiCheckbox<ndebug> const&) noexcept
    -> bool = default;

} // namespace pltxt2htm
