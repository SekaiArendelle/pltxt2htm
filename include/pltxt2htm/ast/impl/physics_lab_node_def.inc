#pragma once

constexpr ::pltxt2htm::PlA::PlA(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::PlA::PlA(::pltxt2htm::PlA&&) noexcept = default;

constexpr auto ::pltxt2htm::PlA::operator=(this PlA& self, ::pltxt2htm::PlA&&) noexcept
    -> ::pltxt2htm::PlA& = default;
constexpr auto ::pltxt2htm::PlA::operator==(this PlA const&, ::pltxt2htm::PlA const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlExperiment::PlExperiment(::pltxt2htm::Ast&& subast_,
                                                          ::pltxt2htm::PlObjectId&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

constexpr ::pltxt2htm::PlExperiment::PlExperiment(::pltxt2htm::PlExperiment&&) noexcept = default;
constexpr auto ::pltxt2htm::PlExperiment::operator=(this PlExperiment& self,
                                                            ::pltxt2htm::PlExperiment&&) noexcept
    -> ::pltxt2htm::PlExperiment& = default;
constexpr auto ::pltxt2htm::PlExperiment::operator==(this PlExperiment const&,
                                                             ::pltxt2htm::PlExperiment const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlDiscussion::PlDiscussion(::pltxt2htm::Ast&& subast_,
                                                          ::pltxt2htm::PlObjectId&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

constexpr ::pltxt2htm::PlDiscussion::PlDiscussion(::pltxt2htm::PlDiscussion&&) noexcept = default;
constexpr auto ::pltxt2htm::PlDiscussion::operator=(this PlDiscussion& self,
                                                            ::pltxt2htm::PlDiscussion&&) noexcept
    -> ::pltxt2htm::PlDiscussion& = default;
constexpr auto ::pltxt2htm::PlDiscussion::operator==(this PlDiscussion const&,
                                                             ::pltxt2htm::PlDiscussion const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlExperiments::PlExperiments(::pltxt2htm::Ast&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

constexpr ::pltxt2htm::PlExperiments::PlExperiments(::pltxt2htm::PlExperiments&&) noexcept = default;
constexpr auto ::pltxt2htm::PlExperiments::operator=(this PlExperiments& self,
                                                             ::pltxt2htm::PlExperiments&&) noexcept
    -> ::pltxt2htm::PlExperiments& = default;
constexpr auto ::pltxt2htm::PlExperiments::operator==(this PlExperiments const&,
                                                              ::pltxt2htm::PlExperiments const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlDiscussions::PlDiscussions(::pltxt2htm::Ast&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

constexpr ::pltxt2htm::PlDiscussions::PlDiscussions(::pltxt2htm::PlDiscussions&&) noexcept = default;
constexpr auto ::pltxt2htm::PlDiscussions::operator=(this PlDiscussions& self,
                                                             ::pltxt2htm::PlDiscussions&&) noexcept
    -> ::pltxt2htm::PlDiscussions& = default;
constexpr auto ::pltxt2htm::PlDiscussions::operator==(this PlDiscussions const&,
                                                              ::pltxt2htm::PlDiscussions const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlUser::PlUser(::pltxt2htm::Ast&& subast_,
                                              ::pltxt2htm::PlObjectId&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

constexpr ::pltxt2htm::PlUser::PlUser(::pltxt2htm::PlUser&&) noexcept = default;
constexpr auto ::pltxt2htm::PlUser::operator=(this PlUser& self, ::pltxt2htm::PlUser&&) noexcept
    -> ::pltxt2htm::PlUser& = default;

constexpr auto ::pltxt2htm::PlUser::operator==(this PlUser const&,
                                                       ::pltxt2htm::PlUser const&) noexcept -> bool = default;

constexpr ::pltxt2htm::PlTrigger::PlTrigger(::pltxt2htm::Ast&& subast_,
                                                    ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

constexpr ::pltxt2htm::PlTrigger::PlTrigger(::pltxt2htm::PlTrigger&&) noexcept = default;
constexpr auto ::pltxt2htm::PlTrigger::operator=(this PlTrigger& self,
                                                         ::pltxt2htm::PlTrigger&&) noexcept
    -> ::pltxt2htm::PlTrigger& = default;
constexpr auto ::pltxt2htm::PlTrigger::operator==(this PlTrigger const&,
                                                          ::pltxt2htm::PlTrigger const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlInternal::PlInternal(::pltxt2htm::Ast&& subast_,
                                                      ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

constexpr ::pltxt2htm::PlInternal::PlInternal(::pltxt2htm::PlInternal&&) noexcept = default;
constexpr auto ::pltxt2htm::PlInternal::operator=(this PlInternal& self,
                                                          ::pltxt2htm::PlInternal&&) noexcept
    -> ::pltxt2htm::PlInternal& = default;
constexpr auto ::pltxt2htm::PlInternal::operator==(this PlInternal const&,
                                                           ::pltxt2htm::PlInternal const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::PlExternal::PlExternal(::pltxt2htm::Ast&& subast_,
                                                      ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

constexpr ::pltxt2htm::PlExternal::PlExternal(::pltxt2htm::PlExternal&&) noexcept = default;
constexpr auto ::pltxt2htm::PlExternal::operator=(this PlExternal& self,
                                                          ::pltxt2htm::PlExternal&&) noexcept
    -> ::pltxt2htm::PlExternal& = default;
constexpr auto ::pltxt2htm::PlExternal::operator==(this PlExternal const&,
                                                           ::pltxt2htm::PlExternal const&) noexcept
    -> bool = default;
