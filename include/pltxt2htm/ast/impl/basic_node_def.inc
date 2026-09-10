#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Group<ndebug> const&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Group<ndebug>::Group(::pltxt2htm::Group<ndebug>&& other) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Group<ndebug>::operator=(this Group<ndebug>& self,
                                                     ::pltxt2htm::Group<ndebug>&& other) noexcept
    -> ::pltxt2htm::Group<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Group<ndebug>::operator==(this Group<ndebug> const&,
                                                      ::pltxt2htm::Group<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeAst<ndebug>&& ast_value) noexcept
    : ast(::std::move(ast_value)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator=(this CodeFence<ndebug>& self,
                                                         ::pltxt2htm::CodeFence<ndebug>&&) noexcept
    -> ::pltxt2htm::CodeFence<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator==(this CodeFence<ndebug> const&,
                                                          ::pltxt2htm::CodeFence<ndebug> const&) noexcept
    -> bool = default;
