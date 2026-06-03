#pragma once

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH1<ndebug>::MdAtxH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH1<ndebug>::MdAtxH1(::pltxt2htm::MdAtxH1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH1<ndebug>::~MdAtxH1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH1<ndebug>::operator=(this ::pltxt2htm::MdAtxH1<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH1<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH2<ndebug>::MdAtxH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH2<ndebug>::MdAtxH2(::pltxt2htm::MdAtxH2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH2<ndebug>::~MdAtxH2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH2<ndebug>::operator=(this ::pltxt2htm::MdAtxH2<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH2<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH3<ndebug>::MdAtxH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH3<ndebug>::MdAtxH3(::pltxt2htm::MdAtxH3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH3<ndebug>::~MdAtxH3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH3<ndebug>::operator=(this ::pltxt2htm::MdAtxH3<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH3<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH4<ndebug>::MdAtxH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH4<ndebug>::MdAtxH4(::pltxt2htm::MdAtxH4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH4<ndebug>::~MdAtxH4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH4<ndebug>::operator=(this ::pltxt2htm::MdAtxH4<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH4<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH5<ndebug>::MdAtxH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH5<ndebug>::MdAtxH5(::pltxt2htm::MdAtxH5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH5<ndebug>::~MdAtxH5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH5<ndebug>::operator=(this ::pltxt2htm::MdAtxH5<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH5<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH6<ndebug>::MdAtxH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH6<ndebug>::MdAtxH6(::pltxt2htm::MdAtxH6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdAtxH6<ndebug>::~MdAtxH6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdAtxH6<ndebug>::operator=(this ::pltxt2htm::MdAtxH6<ndebug>& self,
                                                       ::pltxt2htm::MdAtxH6<ndebug>&&) noexcept
    -> ::pltxt2htm::MdAtxH6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(
    ::pltxt2htm::Ast<ndebug>&& subast_, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(
    ::pltxt2htm::MdCodeFenceBacktick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceBacktick<ndebug>::~MdCodeFenceBacktick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdCodeFenceBacktick<ndebug>::operator=(this ::pltxt2htm::MdCodeFenceBacktick<ndebug>& self,
                                                                   ::pltxt2htm::MdCodeFenceBacktick<ndebug>&&) noexcept
    -> ::pltxt2htm::MdCodeFenceBacktick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(
    ::pltxt2htm::Ast<ndebug>&& subast_, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(::pltxt2htm::MdCodeFenceTilde<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeFenceTilde<ndebug>::~MdCodeFenceTilde() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdCodeFenceTilde<ndebug>::operator=(this ::pltxt2htm::MdCodeFenceTilde<ndebug>& self,
                                                                ::pltxt2htm::MdCodeFenceTilde<ndebug>&&) noexcept
    -> ::pltxt2htm::MdCodeFenceTilde<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(
    ::pltxt2htm::MdCodeSpan1Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan1Backtick<ndebug>::~MdCodeSpan1Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdCodeSpan1Backtick<ndebug>::operator=(this ::pltxt2htm::MdCodeSpan1Backtick<ndebug>& self,
                                                                   ::pltxt2htm::MdCodeSpan1Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::MdCodeSpan1Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(
    ::pltxt2htm::MdCodeSpan2Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan2Backtick<ndebug>::~MdCodeSpan2Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdCodeSpan2Backtick<ndebug>::operator=(this ::pltxt2htm::MdCodeSpan2Backtick<ndebug>& self,
                                                                   ::pltxt2htm::MdCodeSpan2Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::MdCodeSpan2Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(
    ::pltxt2htm::MdCodeSpan3Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdCodeSpan3Backtick<ndebug>::~MdCodeSpan3Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdCodeSpan3Backtick<ndebug>::operator=(this ::pltxt2htm::MdCodeSpan3Backtick<ndebug>& self,
                                                                   ::pltxt2htm::MdCodeSpan3Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::MdCodeSpan3Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(
    ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>::~MdSingleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>::operator=(
    this ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>& self, ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>::~MdDoubleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>::operator=(
    this ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>& self, ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(
    ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>::~MdTripleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>::operator=(
    this ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>& self, ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTripleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>::~MdSingleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>::operator=(
    this ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>& self,
    ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>::~MdDoubleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>::operator=(
    this ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>& self,
    ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    ::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>::~MdTripleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>::operator=(
    this ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>& self,
    ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTripleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDel<ndebug>::MdDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDel<ndebug>::MdDel(::pltxt2htm::MdDel<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdDel<ndebug>::~MdDel() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdDel<ndebug>::operator=(this ::pltxt2htm::MdDel<ndebug>& self,
                                                     ::pltxt2htm::MdDel<ndebug>&&) noexcept
    -> ::pltxt2htm::MdDel<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLink<ndebug>::MdLink(::pltxt2htm::Ast<ndebug>&& subast_,
                                              ::pltxt2htm::Url<ndebug>&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLink<ndebug>::MdLink(::pltxt2htm::MdLink<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLink<ndebug>::~MdLink() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdLink<ndebug>::operator=(this ::pltxt2htm::MdLink<ndebug>& self,
                                                      ::pltxt2htm::MdLink<ndebug>&&) noexcept
    -> ::pltxt2htm::MdLink<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdImage<ndebug>::MdImage(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::pltxt2htm::Url<ndebug>&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdImage<ndebug>::MdImage(::pltxt2htm::MdImage<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdImage<ndebug>::~MdImage() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdImage<ndebug>::operator=(this ::pltxt2htm::MdImage<ndebug>& self,
                                                       ::pltxt2htm::MdImage<ndebug>&&) noexcept
    -> ::pltxt2htm::MdImage<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdBlockQuotes<ndebug>::MdBlockQuotes(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdBlockQuotes<ndebug>::MdBlockQuotes(::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdBlockQuotes<ndebug>::~MdBlockQuotes() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdBlockQuotes<ndebug>::operator=(this ::pltxt2htm::MdBlockQuotes<ndebug>& self,
                                                             ::pltxt2htm::MdBlockQuotes<ndebug>&&) noexcept
    -> ::pltxt2htm::MdBlockQuotes<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdUl<ndebug>::MdUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdUl<ndebug>::MdUl(::pltxt2htm::MdUl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdUl<ndebug>::~MdUl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdUl<ndebug>::operator=(this ::pltxt2htm::MdUl<ndebug>& self,
                                                    ::pltxt2htm::MdUl<ndebug>&&) noexcept
    -> ::pltxt2htm::MdUl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdOl<ndebug>::MdOl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdOl<ndebug>::MdOl(::pltxt2htm::MdOl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdOl<ndebug>::~MdOl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdOl<ndebug>::operator=(this ::pltxt2htm::MdOl<ndebug>& self,
                                                    ::pltxt2htm::MdOl<ndebug>&&) noexcept
    -> ::pltxt2htm::MdOl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLi<ndebug>::MdLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLi<ndebug>::MdLi(::pltxt2htm::MdLi<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLi<ndebug>::~MdLi() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdLi<ndebug>::operator=(this ::pltxt2htm::MdLi<ndebug>& self,
                                                    ::pltxt2htm::MdLi<ndebug>&&) noexcept
    -> ::pltxt2htm::MdLi<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexInline<ndebug>::MdLatexInline(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexInline<ndebug>::MdLatexInline(::pltxt2htm::MdLatexInline<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexInline<ndebug>::~MdLatexInline() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdLatexInline<ndebug>::operator=(this ::pltxt2htm::MdLatexInline<ndebug>& self,
                                                             ::pltxt2htm::MdLatexInline<ndebug>&&) noexcept
    -> ::pltxt2htm::MdLatexInline<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexBlock<ndebug>::MdLatexBlock(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexBlock<ndebug>::MdLatexBlock(::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdLatexBlock<ndebug>::~MdLatexBlock() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdLatexBlock<ndebug>::operator=(this ::pltxt2htm::MdLatexBlock<ndebug>& self,
                                                            ::pltxt2htm::MdLatexBlock<ndebug>&&) noexcept
    -> ::pltxt2htm::MdLatexBlock<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTable<ndebug>::MdTable(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTable<ndebug>::MdTable(::pltxt2htm::MdTable<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTable<ndebug>::~MdTable() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTable<ndebug>::operator=(this ::pltxt2htm::MdTable<ndebug>& self,
                                                       ::pltxt2htm::MdTable<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTable<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdThead<ndebug>::MdThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdThead<ndebug>::MdThead(::pltxt2htm::MdThead<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdThead<ndebug>::~MdThead() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdThead<ndebug>::operator=(this ::pltxt2htm::MdThead<ndebug>& self,
                                                       ::pltxt2htm::MdThead<ndebug>&&) noexcept
    -> ::pltxt2htm::MdThead<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTbody<ndebug>::MdTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTbody<ndebug>::MdTbody(::pltxt2htm::MdTbody<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTbody<ndebug>::~MdTbody() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTbody<ndebug>::operator=(this ::pltxt2htm::MdTbody<ndebug>& self,
                                                       ::pltxt2htm::MdTbody<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTbody<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTr<ndebug>::MdTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTr<ndebug>::MdTr(::pltxt2htm::MdTr<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTr<ndebug>::~MdTr() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTr<ndebug>::operator=(this ::pltxt2htm::MdTr<ndebug>& self,
                                                    ::pltxt2htm::MdTr<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTr<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTh<ndebug>::MdTh(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::MdTableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTh<ndebug>::MdTh(::pltxt2htm::MdTh<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTh<ndebug>::~MdTh() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTh<ndebug>::operator=(this ::pltxt2htm::MdTh<ndebug>& self,
                                                    ::pltxt2htm::MdTh<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTh<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTd<ndebug>::MdTd(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::MdTableAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTd<ndebug>::MdTd(::pltxt2htm::MdTd<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::MdTd<ndebug>::~MdTd() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::MdTd<ndebug>::operator=(this ::pltxt2htm::MdTd<ndebug>& self,
                                                    ::pltxt2htm::MdTd<ndebug>&&) noexcept
    -> ::pltxt2htm::MdTd<ndebug>& = default;

} // namespace pltxt2htm
