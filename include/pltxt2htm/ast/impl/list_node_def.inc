#pragma once

constexpr ::pltxt2htm::ListUl::ListUl(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::ListUl::ListUl(::pltxt2htm::ListUl&&) noexcept = default;

constexpr auto ::pltxt2htm::ListUl::operator=(this ListUl& self, ::pltxt2htm::ListUl&&) noexcept
    -> ::pltxt2htm::ListUl& = default;

constexpr auto ::pltxt2htm::ListUl::operator==(this ListUl const&,
                                                       ::pltxt2htm::ListUl const&) noexcept -> bool = default;
constexpr ::pltxt2htm::ListOl::ListOl(::pltxt2htm::Ast&& subast_, ::std::size_t start_) noexcept
    : subast(::std::move(subast_)),
      start(start_) {
}

constexpr ::pltxt2htm::ListOl::ListOl(::pltxt2htm::ListOl&&) noexcept = default;
constexpr auto ::pltxt2htm::ListOl::operator=(this ListOl& self, ::pltxt2htm::ListOl&&) noexcept
    -> ::pltxt2htm::ListOl& = default;

constexpr auto ::pltxt2htm::ListOl::operator==(this ListOl const&,
                                                       ::pltxt2htm::ListOl const&) noexcept -> bool = default;

constexpr ::pltxt2htm::ListLi::ListLi(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::ListLi::ListLi(::pltxt2htm::ListLi&&) noexcept = default;
constexpr auto ::pltxt2htm::ListLi::operator=(this ListLi& self, ::pltxt2htm::ListLi&&) noexcept
    -> ::pltxt2htm::ListLi& = default;

constexpr auto ::pltxt2htm::ListLi::operator==(this ListLi const&,
                                                       ::pltxt2htm::ListLi const&) noexcept -> bool = default;

constexpr ::pltxt2htm::ListLiCheckbox::ListLiCheckbox(::pltxt2htm::Ast&& subast_,
                                                              bool checked_) noexcept
    : subast(::std::move(subast_)),
      checked(checked_) {
}

constexpr ::pltxt2htm::ListLiCheckbox::ListLiCheckbox(::pltxt2htm::ListLiCheckbox&&) noexcept = default;
constexpr auto ::pltxt2htm::ListLiCheckbox::operator=(this ListLiCheckbox& self,
                                                              ::pltxt2htm::ListLiCheckbox&&) noexcept
    -> ::pltxt2htm::ListLiCheckbox& = default;
constexpr auto ::pltxt2htm::ListLiCheckbox::operator==(this ListLiCheckbox const&,
                                                               ::pltxt2htm::ListLiCheckbox const&) noexcept
    -> bool = default;
