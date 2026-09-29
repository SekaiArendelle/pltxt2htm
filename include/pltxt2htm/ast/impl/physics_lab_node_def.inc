#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator=(this PlA<ndebug>& self, ::pltxt2htm::PlA<ndebug>&&) noexcept
    -> ::pltxt2htm::PlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator==(this PlA<ndebug> const&, ::pltxt2htm::PlA<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator=(this PlExperiment<ndebug>& self,
                                                            ::pltxt2htm::PlExperiment<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExperiment<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator==(this PlExperiment<ndebug> const&,
                                                             ::pltxt2htm::PlExperiment<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator=(this PlDiscussion<ndebug>& self,
                                                            ::pltxt2htm::PlDiscussion<ndebug>&&) noexcept
    -> ::pltxt2htm::PlDiscussion<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator==(this PlDiscussion<ndebug> const&,
                                                             ::pltxt2htm::PlDiscussion<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::Ast<ndebug>&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::PlExperiments<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::PlExperiments<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiments<ndebug>::operator=(this PlExperiments<ndebug>& self,
                                                             ::pltxt2htm::PlExperiments<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExperiments<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiments<ndebug>::operator==(this PlExperiments<ndebug> const&,
                                                              ::pltxt2htm::PlExperiments<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::Ast<ndebug>&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::PlDiscussions<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::PlDiscussions<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussions<ndebug>::operator=(this PlDiscussions<ndebug>& self,
                                                             ::pltxt2htm::PlDiscussions<ndebug>&&) noexcept
    -> ::pltxt2htm::PlDiscussions<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussions<ndebug>::operator==(this PlDiscussions<ndebug> const&,
                                                              ::pltxt2htm::PlDiscussions<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::Ast<ndebug>&& subast_,
                                              ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator=(this PlUser<ndebug>& self, ::pltxt2htm::PlUser<ndebug>&&) noexcept
    -> ::pltxt2htm::PlUser<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator==(this PlUser<ndebug> const&,
                                                       ::pltxt2htm::PlUser<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::PlTrigger<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::PlTrigger<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlTrigger<ndebug>::operator=(this PlTrigger<ndebug>& self,
                                                         ::pltxt2htm::PlTrigger<ndebug>&&) noexcept
    -> ::pltxt2htm::PlTrigger<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlTrigger<ndebug>::operator==(this PlTrigger<ndebug> const&,
                                                          ::pltxt2htm::PlTrigger<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::PlInternal<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::PlInternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlInternal<ndebug>::operator=(this PlInternal<ndebug>& self,
                                                          ::pltxt2htm::PlInternal<ndebug>&&) noexcept
    -> ::pltxt2htm::PlInternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlInternal<ndebug>::operator==(this PlInternal<ndebug> const&,
                                                           ::pltxt2htm::PlInternal<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator=(this PlExternal<ndebug>& self,
                                                          ::pltxt2htm::PlExternal<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator==(this PlExternal<ndebug> const&,
                                                           ::pltxt2htm::PlExternal<ndebug> const&) noexcept
    -> bool = default;
