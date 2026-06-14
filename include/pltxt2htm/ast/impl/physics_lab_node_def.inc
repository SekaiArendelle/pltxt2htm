#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::fast_io::u8string&& color_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::PlColor<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::PlColor<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::~PlColor() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlColor<ndebug>::operator=(this ::pltxt2htm::PlColor<ndebug>& self,
                                                       ::pltxt2htm::PlColor<ndebug>&&) noexcept
    -> ::pltxt2htm::PlColor<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlColor<ndebug>::operator==(this ::pltxt2htm::PlColor<ndebug> const&,
                                                        ::pltxt2htm::PlColor<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::~PlA() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator=(this ::pltxt2htm::PlA<ndebug>& self,
                                                   ::pltxt2htm::PlA<ndebug>&&) noexcept
    -> ::pltxt2htm::PlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator==(this ::pltxt2htm::PlA<ndebug> const&,
                                                    ::pltxt2htm::PlA<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::fast_io::u8string&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::~PlExperiment() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator=(this ::pltxt2htm::PlExperiment<ndebug>& self,
                                                            ::pltxt2htm::PlExperiment<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExperiment<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator==(this ::pltxt2htm::PlExperiment<ndebug> const&,
                                                             ::pltxt2htm::PlExperiment<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::fast_io::u8string&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::~PlDiscussion() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator=(this ::pltxt2htm::PlDiscussion<ndebug>& self,
                                                            ::pltxt2htm::PlDiscussion<ndebug>&&) noexcept
    -> ::pltxt2htm::PlDiscussion<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator==(this ::pltxt2htm::PlDiscussion<ndebug> const&,
                                                             ::pltxt2htm::PlDiscussion<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::~PlUser() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator=(this ::pltxt2htm::PlUser<ndebug>& self,
                                                      ::pltxt2htm::PlUser<ndebug>&&) noexcept
    -> ::pltxt2htm::PlUser<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator==(this ::pltxt2htm::PlUser<ndebug> const&,
                                                       ::pltxt2htm::PlUser<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::Url<ndebug>&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::~PlExternal() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator=(this ::pltxt2htm::PlExternal<ndebug>& self,
                                                          ::pltxt2htm::PlExternal<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator==(this ::pltxt2htm::PlExternal<ndebug> const&,
                                                           ::pltxt2htm::PlExternal<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::Ast<ndebug>&& subast_, ::std::size_t size_) noexcept
    : subast(::std::move(subast_)),
      size(size_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::PlSize<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::PlSize<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::~PlSize() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlSize<ndebug>::operator=(this ::pltxt2htm::PlSize<ndebug>& self,
                                                      ::pltxt2htm::PlSize<ndebug>&&) noexcept
    -> ::pltxt2htm::PlSize<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlSize<ndebug>::operator==(this ::pltxt2htm::PlSize<ndebug> const&,
                                                       ::pltxt2htm::PlSize<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::PlI<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::PlI<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::~PlI() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlI<ndebug>::operator=(this ::pltxt2htm::PlI<ndebug>& self,
                                                   ::pltxt2htm::PlI<ndebug>&&) noexcept
    -> ::pltxt2htm::PlI<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlI<ndebug>::operator==(this ::pltxt2htm::PlI<ndebug> const&,
                                                    ::pltxt2htm::PlI<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::PlB<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::PlB<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::~PlB() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlB<ndebug>::operator=(this ::pltxt2htm::PlB<ndebug>& self,
                                                   ::pltxt2htm::PlB<ndebug>&&) noexcept
    -> ::pltxt2htm::PlB<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlB<ndebug>::operator==(this ::pltxt2htm::PlB<ndebug> const&,
                                                    ::pltxt2htm::PlB<ndebug> const&) noexcept -> bool = default;

} // namespace pltxt2htm
