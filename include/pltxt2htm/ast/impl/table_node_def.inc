#pragma once

constexpr ::pltxt2htm::Table::Table(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::Table::Table(::pltxt2htm::Table&&) noexcept = default;

constexpr auto ::pltxt2htm::Table::operator=(this Table & self, ::pltxt2htm::Table&&) noexcept
    -> ::pltxt2htm::Table& = default;

constexpr auto ::pltxt2htm::Table::operator==(this Table const&, ::pltxt2htm::Table const&) noexcept -> bool = default;

constexpr ::pltxt2htm::TableTr::TableTr(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableTr::TableTr(::pltxt2htm::TableTr&&) noexcept = default;
constexpr auto ::pltxt2htm::TableTr::operator=(this TableTr & self, ::pltxt2htm::TableTr&&) noexcept
    -> ::pltxt2htm::TableTr& = default;
constexpr auto ::pltxt2htm::TableTr::operator==(this TableTr const&, ::pltxt2htm::TableTr const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableTd::TableTd(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align{align_} {
}

constexpr ::pltxt2htm::TableTd::TableTd(::pltxt2htm::TableTd&&) noexcept = default;
constexpr auto ::pltxt2htm::TableTd::operator=(this TableTd & self, ::pltxt2htm::TableTd&&) noexcept
    -> ::pltxt2htm::TableTd& = default;
constexpr auto ::pltxt2htm::TableTd::operator==(this TableTd const&, ::pltxt2htm::TableTd const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableTh::TableTh(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align{align_} {
}

constexpr ::pltxt2htm::TableTh::TableTh(::pltxt2htm::TableTh&&) noexcept = default;
constexpr auto ::pltxt2htm::TableTh::operator=(this TableTh & self, ::pltxt2htm::TableTh&&) noexcept
    -> ::pltxt2htm::TableTh& = default;
constexpr auto ::pltxt2htm::TableTh::operator==(this TableTh const&, ::pltxt2htm::TableTh const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableThead::TableThead(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableThead::TableThead(::pltxt2htm::TableThead&&) noexcept = default;
constexpr auto ::pltxt2htm::TableThead::operator=(this TableThead & self, ::pltxt2htm::TableThead&&) noexcept
    -> ::pltxt2htm::TableThead& = default;
constexpr auto ::pltxt2htm::TableThead::operator==(this TableThead const&, ::pltxt2htm::TableThead const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableTbody::TableTbody(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableTbody::TableTbody(::pltxt2htm::TableTbody&&) noexcept = default;
constexpr auto ::pltxt2htm::TableTbody::operator=(this TableTbody & self, ::pltxt2htm::TableTbody&&) noexcept
    -> ::pltxt2htm::TableTbody& = default;
constexpr auto ::pltxt2htm::TableTbody::operator==(this TableTbody const&, ::pltxt2htm::TableTbody const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableTfoot::TableTfoot(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableTfoot::TableTfoot(::pltxt2htm::TableTfoot&&) noexcept = default;
constexpr auto ::pltxt2htm::TableTfoot::operator=(this TableTfoot & self, ::pltxt2htm::TableTfoot&&) noexcept
    -> ::pltxt2htm::TableTfoot& = default;
constexpr auto ::pltxt2htm::TableTfoot::operator==(this TableTfoot const&, ::pltxt2htm::TableTfoot const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::TableCaption::TableCaption(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableCaption::TableCaption(::pltxt2htm::TableCaption&&) noexcept = default;
constexpr auto ::pltxt2htm::TableCaption::operator=(this TableCaption & self, ::pltxt2htm::TableCaption&&) noexcept
    -> ::pltxt2htm::TableCaption& = default;
constexpr auto ::pltxt2htm::TableCaption::operator==(this TableCaption const&,
                                                     ::pltxt2htm::TableCaption const&) noexcept -> bool = default;

constexpr ::pltxt2htm::TableColgroup::TableColgroup(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::TableColgroup::TableColgroup(::pltxt2htm::TableColgroup&&) noexcept = default;
constexpr auto ::pltxt2htm::TableColgroup::operator=(this TableColgroup & self, ::pltxt2htm::TableColgroup&&) noexcept
    -> ::pltxt2htm::TableColgroup& = default;
constexpr auto ::pltxt2htm::TableColgroup::operator==(this TableColgroup const&,
                                                      ::pltxt2htm::TableColgroup const&) noexcept -> bool = default;
