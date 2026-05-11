#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr PlColor<ndebug>::PlColor(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& color) noexcept
    : subast(::std::move(subast)),
      color_(::std::move(color)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlColor<ndebug>::PlColor(PlColor<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlColor<ndebug>::~PlColor() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlColor<ndebug>::operator=(PlColor<ndebug>&&) noexcept -> PlColor<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlA<ndebug>::PlA(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlA<ndebug>::PlA(PlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlA<ndebug>::~PlA() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlA<ndebug>::operator=(PlA<ndebug>&&) noexcept -> PlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlExperiment<ndebug>::PlExperiment(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlExperiment<ndebug>::PlExperiment(PlExperiment<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlExperiment<ndebug>::~PlExperiment() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlExperiment<ndebug>::operator=(PlExperiment<ndebug>&&) noexcept -> PlExperiment<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlDiscussion<ndebug>::PlDiscussion(PlDiscussion<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlDiscussion<ndebug>::~PlDiscussion() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlDiscussion<ndebug>::operator=(PlDiscussion<ndebug>&&) noexcept -> PlDiscussion<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlUser<ndebug>::PlUser(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)),
      id_(::std::move(id)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlUser<ndebug>::PlUser(PlUser<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlUser<ndebug>::~PlUser() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlUser<ndebug>::operator=(PlUser<ndebug>&&) noexcept -> PlUser<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlExternal<ndebug>::PlExternal(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                         ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept
    : subast(::std::move(subast)),
      url_(::std::move(url)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlExternal<ndebug>::PlExternal(PlExternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlExternal<ndebug>::~PlExternal() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlExternal<ndebug>::operator=(PlExternal<ndebug>&&) noexcept -> PlExternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlSize<ndebug>::PlSize(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::std::size_t size) noexcept
    : subast(::std::move(subast)),
      size_(size) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlSize<ndebug>::PlSize(PlSize<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlSize<ndebug>::~PlSize() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlSize<ndebug>::operator=(PlSize<ndebug>&&) noexcept -> PlSize<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlI<ndebug>::PlI(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlI<ndebug>::PlI(PlI<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlI<ndebug>::~PlI() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlI<ndebug>::operator=(PlI<ndebug>&&) noexcept -> PlI<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr PlB<ndebug>::PlB(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr PlB<ndebug>::PlB(PlB<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr PlB<ndebug>::~PlB() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto PlB<ndebug>::operator=(PlB<ndebug>&&) noexcept -> PlB<ndebug>& = default;

} // namespace pltxt2htm::ast2
