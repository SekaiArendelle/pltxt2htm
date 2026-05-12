#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H1<ndebug>::H1(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H1<ndebug>::H1(::pltxt2htm::ast2::H1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H1<ndebug>::~H1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H1<ndebug>::operator=(::pltxt2htm::ast2::H1<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H2<ndebug>::H2(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H2<ndebug>::H2(::pltxt2htm::ast2::H2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H2<ndebug>::~H2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H2<ndebug>::operator=(::pltxt2htm::ast2::H2<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H3<ndebug>::H3(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H3<ndebug>::H3(::pltxt2htm::ast2::H3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H3<ndebug>::~H3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H3<ndebug>::operator=(::pltxt2htm::ast2::H3<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H4<ndebug>::H4(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H4<ndebug>::H4(::pltxt2htm::ast2::H4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H4<ndebug>::~H4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H4<ndebug>::operator=(::pltxt2htm::ast2::H4<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H5<ndebug>::H5(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H5<ndebug>::H5(::pltxt2htm::ast2::H5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H5<ndebug>::~H5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H5<ndebug>::operator=(::pltxt2htm::ast2::H5<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H6<ndebug>::H6(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H6<ndebug>::H6(::pltxt2htm::ast2::H6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::H6<ndebug>::~H6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::H6<ndebug>::operator=(::pltxt2htm::ast2::H6<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::H6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::P<ndebug>::P(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::P<ndebug>::P(::pltxt2htm::ast2::P<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::P<ndebug>::~P() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::P<ndebug>::operator=(::pltxt2htm::ast2::P<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::P<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Del<ndebug>::Del(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Del<ndebug>::Del(::pltxt2htm::ast2::Del<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Del<ndebug>::~Del() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Del<ndebug>::operator=(::pltxt2htm::ast2::Del<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Del<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Note<ndebug>::Note(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Note<ndebug>::Note(::pltxt2htm::ast2::Note<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Note<ndebug>::~Note() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Note<ndebug>::operator=(::pltxt2htm::ast2::Note<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Note<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Em<ndebug>::Em(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Em<ndebug>::Em(::pltxt2htm::ast2::Em<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Em<ndebug>::~Em() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Em<ndebug>::operator=(::pltxt2htm::ast2::Em<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Em<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Strong<ndebug>::Strong(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Strong<ndebug>::Strong(::pltxt2htm::ast2::Strong<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Strong<ndebug>::~Strong() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Strong<ndebug>::operator=(::pltxt2htm::ast2::Strong<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Strong<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ul<ndebug>::Ul(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ul<ndebug>::Ul(::pltxt2htm::ast2::Ul<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ul<ndebug>::~Ul() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Ul<ndebug>::operator=(::pltxt2htm::ast2::Ul<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Ul<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ol<ndebug>::Ol(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ol<ndebug>::Ol(::pltxt2htm::ast2::Ol<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Ol<ndebug>::~Ol() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Ol<ndebug>::operator=(::pltxt2htm::ast2::Ol<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Ol<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Li<ndebug>::Li(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Li<ndebug>::Li(::pltxt2htm::ast2::Li<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Li<ndebug>::~Li() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Li<ndebug>::operator=(::pltxt2htm::ast2::Li<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Li<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Code<ndebug>::Code(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Code<ndebug>::Code(::pltxt2htm::ast2::Code<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Code<ndebug>::~Code() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Code<ndebug>::operator=(::pltxt2htm::ast2::Code<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Code<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Pre<ndebug>::Pre(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Pre<ndebug>::Pre(::pltxt2htm::ast2::Pre<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Pre<ndebug>::~Pre() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Pre<ndebug>::operator=(::pltxt2htm::ast2::Pre<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Pre<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Blockquote<ndebug>::Blockquote(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Blockquote<ndebug>::Blockquote(::pltxt2htm::ast2::Blockquote<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::Blockquote<ndebug>::~Blockquote() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::Blockquote<ndebug>::operator=(::pltxt2htm::ast2::Blockquote<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::Blockquote<ndebug>& = default;

} // namespace pltxt2htm::ast2
