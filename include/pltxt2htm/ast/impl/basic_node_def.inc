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
constexpr ::pltxt2htm::Code<ndebug>::Code(::pltxt2htm::Ast<ndebug>&& subast_,
                                          ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::Code(::pltxt2htm::Code<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::Code(::pltxt2htm::Code<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::~Code() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Code<ndebug>::operator=(this ::pltxt2htm::Code<ndebug>& self,
                                                    ::pltxt2htm::Code<ndebug>&&) noexcept
    -> ::pltxt2htm::Code<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Code<ndebug>::operator==(this ::pltxt2htm::Code<ndebug> const&,
                                                     ::pltxt2htm::Code<ndebug> const&) noexcept -> bool = default;

} // namespace pltxt2htm
