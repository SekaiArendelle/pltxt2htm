#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H1<ndebug>::H1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H1<ndebug>::H1(::pltxt2htm::H1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H1<ndebug>::~H1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H1<ndebug>::operator=(::pltxt2htm::H1<ndebug>&&) noexcept
    -> ::pltxt2htm::H1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H2<ndebug>::H2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H2<ndebug>::H2(::pltxt2htm::H2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H2<ndebug>::~H2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H2<ndebug>::operator=(::pltxt2htm::H2<ndebug>&&) noexcept
    -> ::pltxt2htm::H2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H3<ndebug>::H3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H3<ndebug>::H3(::pltxt2htm::H3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H3<ndebug>::~H3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H3<ndebug>::operator=(::pltxt2htm::H3<ndebug>&&) noexcept
    -> ::pltxt2htm::H3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H4<ndebug>::H4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H4<ndebug>::H4(::pltxt2htm::H4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H4<ndebug>::~H4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H4<ndebug>::operator=(::pltxt2htm::H4<ndebug>&&) noexcept
    -> ::pltxt2htm::H4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H5<ndebug>::H5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H5<ndebug>::H5(::pltxt2htm::H5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H5<ndebug>::~H5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H5<ndebug>::operator=(::pltxt2htm::H5<ndebug>&&) noexcept
    -> ::pltxt2htm::H5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H6<ndebug>::H6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H6<ndebug>::H6(::pltxt2htm::H6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::H6<ndebug>::~H6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::H6<ndebug>::operator=(::pltxt2htm::H6<ndebug>&&) noexcept
    -> ::pltxt2htm::H6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::P<ndebug>::P(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::P<ndebug>::P(::pltxt2htm::P<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::P<ndebug>::~P() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::P<ndebug>::operator=(::pltxt2htm::P<ndebug>&&) noexcept
    -> ::pltxt2htm::P<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Del<ndebug>::Del(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Del<ndebug>::Del(::pltxt2htm::Del<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Del<ndebug>::~Del() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Del<ndebug>::operator=(::pltxt2htm::Del<ndebug>&&) noexcept
    -> ::pltxt2htm::Del<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Note<ndebug>::Note(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Note<ndebug>::Note(::pltxt2htm::Note<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Note<ndebug>::~Note() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Note<ndebug>::operator=(::pltxt2htm::Note<ndebug>&&) noexcept
    -> ::pltxt2htm::Note<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Em<ndebug>::Em(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Em<ndebug>::Em(::pltxt2htm::Em<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Em<ndebug>::~Em() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Em<ndebug>::operator=(::pltxt2htm::Em<ndebug>&&) noexcept
    -> ::pltxt2htm::Em<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Strong<ndebug>::Strong(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Strong<ndebug>::Strong(::pltxt2htm::Strong<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Strong<ndebug>::~Strong() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Strong<ndebug>::operator=(::pltxt2htm::Strong<ndebug>&&) noexcept
    -> ::pltxt2htm::Strong<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ul<ndebug>::Ul(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ul<ndebug>::Ul(::pltxt2htm::Ul<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ul<ndebug>::~Ul() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Ul<ndebug>::operator=(::pltxt2htm::Ul<ndebug>&&) noexcept
    -> ::pltxt2htm::Ul<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ol<ndebug>::Ol(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ol<ndebug>::Ol(::pltxt2htm::Ol<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Ol<ndebug>::~Ol() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Ol<ndebug>::operator=(::pltxt2htm::Ol<ndebug>&&) noexcept
    -> ::pltxt2htm::Ol<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Li<ndebug>::Li(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Li<ndebug>::Li(::pltxt2htm::Li<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Li<ndebug>::~Li() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Li<ndebug>::operator=(::pltxt2htm::Li<ndebug>&&) noexcept
    -> ::pltxt2htm::Li<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::Code(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::Code(::pltxt2htm::Code<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Code<ndebug>::~Code() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Code<ndebug>::operator=(::pltxt2htm::Code<ndebug>&&) noexcept
    -> ::pltxt2htm::Code<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Pre<ndebug>::Pre(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Pre<ndebug>::Pre(::pltxt2htm::Pre<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Pre<ndebug>::~Pre() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Pre<ndebug>::operator=(::pltxt2htm::Pre<ndebug>&&) noexcept
    -> ::pltxt2htm::Pre<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Blockquote<ndebug>::Blockquote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Blockquote<ndebug>::Blockquote(::pltxt2htm::Blockquote<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Blockquote<ndebug>::~Blockquote() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Blockquote<ndebug>::operator=(::pltxt2htm::Blockquote<ndebug>&&) noexcept
    -> ::pltxt2htm::Blockquote<ndebug>& = default;

} // namespace pltxt2htm
