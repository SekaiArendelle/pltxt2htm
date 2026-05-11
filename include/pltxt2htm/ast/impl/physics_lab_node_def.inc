#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlColor<ndebug>::PlColor(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& color) noexcept
    : subast(::std::move(subast)),
      color_(::std::move(color)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlColor<ndebug>::PlColor(::pltxt2htm::ast2::PlColor<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlColor<ndebug>::~PlColor() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlColor<ndebug>::operator=(::pltxt2htm::ast2::PlColor<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlColor<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlA<ndebug>::PlA(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlA<ndebug>::PlA(::pltxt2htm::ast2::PlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlA<ndebug>::~PlA() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlA<ndebug>::operator=(::pltxt2htm::ast2::PlA<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::ast2::PlExperiment<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExperiment<ndebug>::~PlExperiment() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlExperiment<ndebug>::operator=(::pltxt2htm::ast2::PlExperiment<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlExperiment<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::ast2::PlDiscussion<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlDiscussion<ndebug>::~PlDiscussion() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlDiscussion<ndebug>::operator=(::pltxt2htm::ast2::PlDiscussion<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlDiscussion<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlUser<ndebug>::PlUser(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlUser<ndebug>::PlUser(::pltxt2htm::ast2::PlUser<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlUser<ndebug>::~PlUser() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlUser<ndebug>::operator=(::pltxt2htm::ast2::PlUser<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlUser<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExternal<ndebug>::PlExternal(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                         ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept
    : subast(::std::move(subast)),
      url_(::std::move(url)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExternal<ndebug>::PlExternal(::pltxt2htm::ast2::PlExternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlExternal<ndebug>::~PlExternal() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlExternal<ndebug>::operator=(::pltxt2htm::ast2::PlExternal<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlExternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlSize<ndebug>::PlSize(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::std::size_t size) noexcept
    : subast(::std::move(subast)),
      size_(size) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlSize<ndebug>::PlSize(::pltxt2htm::ast2::PlSize<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlSize<ndebug>::~PlSize() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlSize<ndebug>::operator=(::pltxt2htm::ast2::PlSize<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlSize<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlI<ndebug>::PlI(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlI<ndebug>::PlI(::pltxt2htm::ast2::PlI<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlI<ndebug>::~PlI() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlI<ndebug>::operator=(::pltxt2htm::ast2::PlI<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlI<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlB<ndebug>::PlB(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlB<ndebug>::PlB(::pltxt2htm::ast2::PlB<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::PlB<ndebug>::~PlB() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::PlB<ndebug>::operator=(::pltxt2htm::ast2::PlB<ndebug>&&) noexcept -> ::pltxt2htm::ast2::PlB<ndebug>& = default;

} // namespace pltxt2htm::ast2
