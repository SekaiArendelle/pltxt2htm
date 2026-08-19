#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::Table(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::Table(::pltxt2htm::Table<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::Table(::pltxt2htm::Table<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::~Table() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Table<ndebug>::operator=(this Table<ndebug>& self, ::pltxt2htm::Table<ndebug>&&) noexcept
    -> ::pltxt2htm::Table<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Table<ndebug>::operator==(this Table<ndebug> const&,
                                                      ::pltxt2htm::Table<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTr<ndebug>::TableTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTr<ndebug>::TableTr(::pltxt2htm::TableTr<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTr<ndebug>::TableTr(::pltxt2htm::TableTr<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTr<ndebug>::~TableTr() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTr<ndebug>::operator=(this TableTr<ndebug>& self,
                                                       ::pltxt2htm::TableTr<ndebug>&&) noexcept
    -> ::pltxt2htm::TableTr<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTr<ndebug>::operator==(this TableTr<ndebug> const&,
                                                        ::pltxt2htm::TableTr<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTd<ndebug>::TableTd(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::pltxt2htm::TableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align{align_} {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTd<ndebug>::TableTd(::pltxt2htm::TableTd<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTd<ndebug>::TableTd(::pltxt2htm::TableTd<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTd<ndebug>::~TableTd() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTd<ndebug>::operator=(this TableTd<ndebug>& self,
                                                       ::pltxt2htm::TableTd<ndebug>&&) noexcept
    -> ::pltxt2htm::TableTd<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTd<ndebug>::operator==(this TableTd<ndebug> const&,
                                                        ::pltxt2htm::TableTd<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTh<ndebug>::TableTh(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::pltxt2htm::TableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align{align_} {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTh<ndebug>::TableTh(::pltxt2htm::TableTh<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTh<ndebug>::TableTh(::pltxt2htm::TableTh<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTh<ndebug>::~TableTh() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTh<ndebug>::operator=(this TableTh<ndebug>& self,
                                                       ::pltxt2htm::TableTh<ndebug>&&) noexcept
    -> ::pltxt2htm::TableTh<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTh<ndebug>::operator==(this TableTh<ndebug> const&,
                                                        ::pltxt2htm::TableTh<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableThead<ndebug>::TableThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableThead<ndebug>::TableThead(::pltxt2htm::TableThead<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableThead<ndebug>::TableThead(::pltxt2htm::TableThead<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableThead<ndebug>::~TableThead() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableThead<ndebug>::operator=(this TableThead<ndebug>& self,
                                                          ::pltxt2htm::TableThead<ndebug>&&) noexcept
    -> ::pltxt2htm::TableThead<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableThead<ndebug>::operator==(this TableThead<ndebug> const&,
                                                           ::pltxt2htm::TableThead<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTbody<ndebug>::TableTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTbody<ndebug>::TableTbody(::pltxt2htm::TableTbody<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTbody<ndebug>::TableTbody(::pltxt2htm::TableTbody<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTbody<ndebug>::~TableTbody() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTbody<ndebug>::operator=(this TableTbody<ndebug>& self,
                                                          ::pltxt2htm::TableTbody<ndebug>&&) noexcept
    -> ::pltxt2htm::TableTbody<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTbody<ndebug>::operator==(this TableTbody<ndebug> const&,
                                                           ::pltxt2htm::TableTbody<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTfoot<ndebug>::TableTfoot(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTfoot<ndebug>::TableTfoot(::pltxt2htm::TableTfoot<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTfoot<ndebug>::TableTfoot(::pltxt2htm::TableTfoot<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableTfoot<ndebug>::~TableTfoot() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTfoot<ndebug>::operator=(this TableTfoot<ndebug>& self,
                                                          ::pltxt2htm::TableTfoot<ndebug>&&) noexcept
    -> ::pltxt2htm::TableTfoot<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableTfoot<ndebug>::operator==(this TableTfoot<ndebug> const&,
                                                           ::pltxt2htm::TableTfoot<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableCaption<ndebug>::TableCaption(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableCaption<ndebug>::TableCaption(::pltxt2htm::TableCaption<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableCaption<ndebug>::TableCaption(::pltxt2htm::TableCaption<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableCaption<ndebug>::~TableCaption() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableCaption<ndebug>::operator=(this TableCaption<ndebug>& self,
                                                            ::pltxt2htm::TableCaption<ndebug>&&) noexcept
    -> ::pltxt2htm::TableCaption<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableCaption<ndebug>::operator==(this TableCaption<ndebug> const&,
                                                             ::pltxt2htm::TableCaption<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableColgroup<ndebug>::TableColgroup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableColgroup<ndebug>::TableColgroup(::pltxt2htm::TableColgroup<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableColgroup<ndebug>::TableColgroup(::pltxt2htm::TableColgroup<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::TableColgroup<ndebug>::~TableColgroup() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableColgroup<ndebug>::operator=(this TableColgroup<ndebug>& self,
                                                             ::pltxt2htm::TableColgroup<ndebug>&&) noexcept
    -> ::pltxt2htm::TableColgroup<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::TableColgroup<ndebug>::operator==(this TableColgroup<ndebug> const&,
                                                              ::pltxt2htm::TableColgroup<ndebug> const&) noexcept
    -> bool = default;
