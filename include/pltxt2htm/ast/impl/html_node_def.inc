#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr H1<ndebug>::H1(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H1<ndebug>::H1(H1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H1<ndebug>::~H1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H1<ndebug>::operator=(H1<ndebug>&&) noexcept -> H1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr H2<ndebug>::H2(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H2<ndebug>::H2(H2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H2<ndebug>::~H2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H2<ndebug>::operator=(H2<ndebug>&&) noexcept -> H2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr H3<ndebug>::H3(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H3<ndebug>::H3(H3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H3<ndebug>::~H3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H3<ndebug>::operator=(H3<ndebug>&&) noexcept -> H3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr H4<ndebug>::H4(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H4<ndebug>::H4(H4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H4<ndebug>::~H4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H4<ndebug>::operator=(H4<ndebug>&&) noexcept -> H4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr H5<ndebug>::H5(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H5<ndebug>::H5(H5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H5<ndebug>::~H5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H5<ndebug>::operator=(H5<ndebug>&&) noexcept -> H5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr H6<ndebug>::H6(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr H6<ndebug>::H6(H6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr H6<ndebug>::~H6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto H6<ndebug>::operator=(H6<ndebug>&&) noexcept -> H6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr P<ndebug>::P(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr P<ndebug>::P(P<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr P<ndebug>::~P() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto P<ndebug>::operator=(P<ndebug>&&) noexcept -> P<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Del<ndebug>::Del(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Del<ndebug>::Del(Del<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Del<ndebug>::~Del() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Del<ndebug>::operator=(Del<ndebug>&&) noexcept -> Del<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Note<ndebug>::Note(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Note<ndebug>::Note(Note<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Note<ndebug>::~Note() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Note<ndebug>::operator=(Note<ndebug>&&) noexcept -> Note<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Em<ndebug>::Em(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Em<ndebug>::Em(Em<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Em<ndebug>::~Em() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Em<ndebug>::operator=(Em<ndebug>&&) noexcept -> Em<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Strong<ndebug>::Strong(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Strong<ndebug>::Strong(Strong<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Strong<ndebug>::~Strong() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Strong<ndebug>::operator=(Strong<ndebug>&&) noexcept -> Strong<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Ul<ndebug>::Ul(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Ul<ndebug>::Ul(Ul<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Ul<ndebug>::~Ul() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Ul<ndebug>::operator=(Ul<ndebug>&&) noexcept -> Ul<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Ol<ndebug>::Ol(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Ol<ndebug>::Ol(Ol<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Ol<ndebug>::~Ol() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Ol<ndebug>::operator=(Ol<ndebug>&&) noexcept -> Ol<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Li<ndebug>::Li(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Li<ndebug>::Li(Li<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Li<ndebug>::~Li() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Li<ndebug>::operator=(Li<ndebug>&&) noexcept -> Li<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Code<ndebug>::Code(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Code<ndebug>::Code(Code<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Code<ndebug>::~Code() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Code<ndebug>::operator=(Code<ndebug>&&) noexcept -> Code<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Pre<ndebug>::Pre(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Pre<ndebug>::Pre(Pre<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Pre<ndebug>::~Pre() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Pre<ndebug>::operator=(Pre<ndebug>&&) noexcept -> Pre<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr Blockquote<ndebug>::Blockquote(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr Blockquote<ndebug>::Blockquote(Blockquote<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr Blockquote<ndebug>::~Blockquote() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto Blockquote<ndebug>::operator=(Blockquote<ndebug>&&) noexcept -> Blockquote<ndebug>& = default;

} // namespace pltxt2htm::ast2
