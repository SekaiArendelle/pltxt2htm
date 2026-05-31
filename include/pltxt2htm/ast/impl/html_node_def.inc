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
constexpr auto ::pltxt2htm::H1<ndebug>::operator=(this ::pltxt2htm::H1<ndebug>& self,
                                                  ::pltxt2htm::H1<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::H2<ndebug>::operator=(this ::pltxt2htm::H2<ndebug>& self,
                                                  ::pltxt2htm::H2<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::H3<ndebug>::operator=(this ::pltxt2htm::H3<ndebug>& self,
                                                  ::pltxt2htm::H3<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::H4<ndebug>::operator=(this ::pltxt2htm::H4<ndebug>& self,
                                                  ::pltxt2htm::H4<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::H5<ndebug>::operator=(this ::pltxt2htm::H5<ndebug>& self,
                                                  ::pltxt2htm::H5<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::H6<ndebug>::operator=(this ::pltxt2htm::H6<ndebug>& self,
                                                  ::pltxt2htm::H6<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::P<ndebug>::operator=(this ::pltxt2htm::P<ndebug>& self, ::pltxt2htm::P<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Del<ndebug>::operator=(this ::pltxt2htm::Del<ndebug>& self,
                                                   ::pltxt2htm::Del<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Note<ndebug>::operator=(this ::pltxt2htm::Note<ndebug>& self,
                                                    ::pltxt2htm::Note<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Em<ndebug>::operator=(this ::pltxt2htm::Em<ndebug>& self,
                                                  ::pltxt2htm::Em<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Strong<ndebug>::operator=(this ::pltxt2htm::Strong<ndebug>& self,
                                                      ::pltxt2htm::Strong<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Ul<ndebug>::operator=(this ::pltxt2htm::Ul<ndebug>& self,
                                                  ::pltxt2htm::Ul<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Ol<ndebug>::operator=(this ::pltxt2htm::Ol<ndebug>& self,
                                                  ::pltxt2htm::Ol<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Li<ndebug>::operator=(this ::pltxt2htm::Li<ndebug>& self,
                                                  ::pltxt2htm::Li<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Code<ndebug>::operator=(this ::pltxt2htm::Code<ndebug>& self,
                                                    ::pltxt2htm::Code<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Pre<ndebug>::operator=(this ::pltxt2htm::Pre<ndebug>& self,
                                                   ::pltxt2htm::Pre<ndebug>&&) noexcept
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
constexpr auto ::pltxt2htm::Blockquote<ndebug>::operator=(this ::pltxt2htm::Blockquote<ndebug>& self,
                                                          ::pltxt2htm::Blockquote<ndebug>&&) noexcept
    -> ::pltxt2htm::Blockquote<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::Table(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::Table(::pltxt2htm::Table<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Table<ndebug>::~Table() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Table<ndebug>::operator=(this ::pltxt2htm::Table<ndebug>& self,
                                                     ::pltxt2htm::Table<ndebug>&&) noexcept
    -> ::pltxt2htm::Table<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tr<ndebug>::Tr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tr<ndebug>::Tr(::pltxt2htm::Tr<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tr<ndebug>::~Tr() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Tr<ndebug>::operator=(this ::pltxt2htm::Tr<ndebug>& self,
                                                  ::pltxt2htm::Tr<ndebug>&&) noexcept
    -> ::pltxt2htm::Tr<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Td<ndebug>::Td(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Td<ndebug>::Td(::pltxt2htm::Td<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Td<ndebug>::~Td() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Td<ndebug>::operator=(this ::pltxt2htm::Td<ndebug>& self,
                                                  ::pltxt2htm::Td<ndebug>&&) noexcept
    -> ::pltxt2htm::Td<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Th<ndebug>::Th(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Th<ndebug>::Th(::pltxt2htm::Th<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Th<ndebug>::~Th() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Th<ndebug>::operator=(this ::pltxt2htm::Th<ndebug>& self,
                                                  ::pltxt2htm::Th<ndebug>&&) noexcept
    -> ::pltxt2htm::Th<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Thead<ndebug>::Thead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Thead<ndebug>::Thead(::pltxt2htm::Thead<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Thead<ndebug>::~Thead() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Thead<ndebug>::operator=(this ::pltxt2htm::Thead<ndebug>& self,
                                                     ::pltxt2htm::Thead<ndebug>&&) noexcept
    -> ::pltxt2htm::Thead<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tbody<ndebug>::Tbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tbody<ndebug>::Tbody(::pltxt2htm::Tbody<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tbody<ndebug>::~Tbody() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Tbody<ndebug>::operator=(this ::pltxt2htm::Tbody<ndebug>& self,
                                                     ::pltxt2htm::Tbody<ndebug>&&) noexcept
    -> ::pltxt2htm::Tbody<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tfoot<ndebug>::Tfoot(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tfoot<ndebug>::Tfoot(::pltxt2htm::Tfoot<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Tfoot<ndebug>::~Tfoot() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Tfoot<ndebug>::operator=(this ::pltxt2htm::Tfoot<ndebug>& self,
                                                     ::pltxt2htm::Tfoot<ndebug>&&) noexcept
    -> ::pltxt2htm::Tfoot<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Caption<ndebug>::Caption(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Caption<ndebug>::Caption(::pltxt2htm::Caption<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Caption<ndebug>::~Caption() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Caption<ndebug>::operator=(this ::pltxt2htm::Caption<ndebug>& self,
                                                       ::pltxt2htm::Caption<ndebug>&&) noexcept
    -> ::pltxt2htm::Caption<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Colgroup<ndebug>::Colgroup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Colgroup<ndebug>::Colgroup(::pltxt2htm::Colgroup<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::Colgroup<ndebug>::~Colgroup() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::Colgroup<ndebug>::operator=(this ::pltxt2htm::Colgroup<ndebug>& self,
                                                        ::pltxt2htm::Colgroup<ndebug>&&) noexcept
    -> ::pltxt2htm::Colgroup<ndebug>& = default;

} // namespace pltxt2htm
