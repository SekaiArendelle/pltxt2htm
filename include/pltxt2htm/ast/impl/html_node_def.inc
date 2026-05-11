#pragma once

namespace pltxt2htm::ast2 {

constexpr H1::H1(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H1::H1(H1&&) noexcept = default;
constexpr H1::~H1() noexcept = default;
constexpr auto H1::operator=(H1&&) noexcept -> H1& = default;

constexpr H2::H2(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H2::H2(H2&&) noexcept = default;
constexpr H2::~H2() noexcept = default;
constexpr auto H2::operator=(H2&&) noexcept -> H2& = default;

constexpr H3::H3(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H3::H3(H3&&) noexcept = default;
constexpr H3::~H3() noexcept = default;
constexpr auto H3::operator=(H3&&) noexcept -> H3& = default;

constexpr H4::H4(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H4::H4(H4&&) noexcept = default;
constexpr H4::~H4() noexcept = default;
constexpr auto H4::operator=(H4&&) noexcept -> H4& = default;

constexpr H5::H5(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H5::H5(H5&&) noexcept = default;
constexpr H5::~H5() noexcept = default;
constexpr auto H5::operator=(H5&&) noexcept -> H5& = default;

constexpr H6::H6(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr H6::H6(H6&&) noexcept = default;
constexpr H6::~H6() noexcept = default;
constexpr auto H6::operator=(H6&&) noexcept -> H6& = default;

constexpr P::P(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr P::P(P&&) noexcept = default;
constexpr P::~P() noexcept = default;
constexpr auto P::operator=(P&&) noexcept -> P& = default;

constexpr Del::Del(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Del::Del(Del&&) noexcept = default;
constexpr Del::~Del() noexcept = default;
constexpr auto Del::operator=(Del&&) noexcept -> Del& = default;

constexpr Note::Note(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Note::Note(Note&&) noexcept = default;
constexpr Note::~Note() noexcept = default;
constexpr auto Note::operator=(Note&&) noexcept -> Note& = default;

constexpr Em::Em(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Em::Em(Em&&) noexcept = default;
constexpr Em::~Em() noexcept = default;
constexpr auto Em::operator=(Em&&) noexcept -> Em& = default;

constexpr Strong::Strong(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Strong::Strong(Strong&&) noexcept = default;
constexpr Strong::~Strong() noexcept = default;
constexpr auto Strong::operator=(Strong&&) noexcept -> Strong& = default;

constexpr Ul::Ul(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Ul::Ul(Ul&&) noexcept = default;
constexpr Ul::~Ul() noexcept = default;
constexpr auto Ul::operator=(Ul&&) noexcept -> Ul& = default;

constexpr Ol::Ol(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Ol::Ol(Ol&&) noexcept = default;
constexpr Ol::~Ol() noexcept = default;
constexpr auto Ol::operator=(Ol&&) noexcept -> Ol& = default;

constexpr Li::Li(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Li::Li(Li&&) noexcept = default;
constexpr Li::~Li() noexcept = default;
constexpr auto Li::operator=(Li&&) noexcept -> Li& = default;

constexpr Code::Code(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Code::Code(Code&&) noexcept = default;
constexpr Code::~Code() noexcept = default;
constexpr auto Code::operator=(Code&&) noexcept -> Code& = default;

constexpr Pre::Pre(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Pre::Pre(Pre&&) noexcept = default;
constexpr Pre::~Pre() noexcept = default;
constexpr auto Pre::operator=(Pre&&) noexcept -> Pre& = default;

constexpr Blockquote::Blockquote(::pltxt2htm::ast2::Ast&& subast) noexcept
    : subast_(::std::move(subast)) {}
constexpr Blockquote::Blockquote(Blockquote&&) noexcept = default;
constexpr Blockquote::~Blockquote() noexcept = default;
constexpr auto Blockquote::operator=(Blockquote&&) noexcept -> Blockquote& = default;

} // namespace pltxt2htm::ast2
