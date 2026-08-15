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

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::CodeFence(::pltxt2htm::CodeFence<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::CodeFence<ndebug>::~CodeFence() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator=(this ::pltxt2htm::CodeFence<ndebug>& self,
                                                         ::pltxt2htm::CodeFence<ndebug>&&) noexcept
    -> ::pltxt2htm::CodeFence<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::CodeFence<ndebug>::operator==(this ::pltxt2htm::CodeFence<ndebug> const&,
                                                          ::pltxt2htm::CodeFence<ndebug> const&) noexcept
    -> bool = default;

} // namespace pltxt2htm
