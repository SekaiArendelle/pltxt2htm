#pragma once

constexpr ::pltxt2htm::Group::Group(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::Group::Group(::pltxt2htm::Group&& other) noexcept = default;

constexpr auto ::pltxt2htm::Group::operator=(this Group& self,
                                                     ::pltxt2htm::Group&& other) noexcept
    -> ::pltxt2htm::Group& = default;

constexpr auto ::pltxt2htm::Group::operator==(this Group const&,
                                                      ::pltxt2htm::Group const&) noexcept -> bool = default;

constexpr ::pltxt2htm::CodeFence::CodeFence(
    ::pltxt2htm::Ast&& subast_,
    ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

constexpr ::pltxt2htm::CodeFence::CodeFence(::pltxt2htm::CodeFence&&) noexcept = default;
constexpr auto ::pltxt2htm::CodeFence::operator=(this CodeFence& self,
                                                         ::pltxt2htm::CodeFence&&) noexcept
    -> ::pltxt2htm::CodeFence& = default;
constexpr auto ::pltxt2htm::CodeFence::operator==(this CodeFence const&,
                                                          ::pltxt2htm::CodeFence const&) noexcept
    -> bool = default;
