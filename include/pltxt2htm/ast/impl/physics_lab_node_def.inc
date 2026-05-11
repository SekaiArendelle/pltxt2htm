#pragma once

namespace pltxt2htm::ast2 {

constexpr PlColor::PlColor(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& color) noexcept
    : subast(::std::move(subast)), color_(::std::move(color)) {}

constexpr PlColor::PlColor(PlColor&&) noexcept = default;
constexpr PlColor::~PlColor() noexcept = default;
constexpr auto PlColor::operator=(PlColor&&) noexcept -> PlColor& = default;

constexpr PlA::PlA(::pltxt2htm::ast2::Ast&& subast) noexcept : subast(::std::move(subast)) {}
constexpr PlA::PlA(PlA&&) noexcept = default;
constexpr PlA::~PlA() noexcept = default;
constexpr auto PlA::operator=(PlA&&) noexcept -> PlA& = default;


constexpr PlExperiment::PlExperiment(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)), id_(::std::move(id)) {}
constexpr PlExperiment::PlExperiment(PlExperiment&&) noexcept = default;
constexpr PlExperiment::~PlExperiment() noexcept = default;
constexpr auto PlExperiment::operator=(PlExperiment&&) noexcept -> PlExperiment& = default;

constexpr PlDiscussion::PlDiscussion(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)), id_(::std::move(id)) {}
constexpr PlDiscussion::PlDiscussion(PlDiscussion&&) noexcept = default;
constexpr PlDiscussion::~PlDiscussion() noexcept = default;
constexpr auto PlDiscussion::operator=(PlDiscussion&&) noexcept -> PlDiscussion& = default;

constexpr PlUser::PlUser(::pltxt2htm::ast2::Ast&& subast, ::fast_io::u8string&& id) noexcept
    : subast(::std::move(subast)), id_(::std::move(id)) {}
constexpr PlUser::PlUser(PlUser&&) noexcept = default;
constexpr PlUser::~PlUser() noexcept = default;
constexpr auto PlUser::operator=(PlUser&&) noexcept -> PlUser& = default;

constexpr PlExternal::PlExternal(::pltxt2htm::ast2::Ast&& subast, ::pltxt2htm::ast2::Url&& url) noexcept
    : subast(::std::move(subast)), url_(::std::move(url)) {}
constexpr PlExternal::PlExternal(PlExternal&&) noexcept = default;
constexpr PlExternal::~PlExternal() noexcept = default;
constexpr auto PlExternal::operator=(PlExternal&&) noexcept -> PlExternal& = default;

constexpr PlSize::PlSize(::pltxt2htm::ast2::Ast&& subast, ::std::size_t size) noexcept
    : subast(::std::move(subast)), size_(size) {}
constexpr PlSize::PlSize(PlSize&&) noexcept = default;
constexpr PlSize::~PlSize() noexcept = default;
constexpr auto PlSize::operator=(PlSize&&) noexcept -> PlSize& = default;

constexpr PlI::PlI(::pltxt2htm::ast2::Ast&& subast) noexcept : subast(::std::move(subast)) {}
constexpr PlI::PlI(PlI&&) noexcept = default;
constexpr PlI::~PlI() noexcept = default;
constexpr auto PlI::operator=(PlI&&) noexcept -> PlI& = default;

constexpr PlB::PlB(::pltxt2htm::ast2::Ast&& subast) noexcept : subast(::std::move(subast)) {}
constexpr PlB::PlB(PlB&&) noexcept = default;
constexpr PlB::~PlB() noexcept = default;
constexpr auto PlB::operator=(PlB&&) noexcept -> PlB& = default;


} // namespace pltxt2htm::ast2
