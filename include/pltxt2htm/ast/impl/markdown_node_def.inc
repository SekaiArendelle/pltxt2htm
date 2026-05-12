#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH1<ndebug>::MdAtxH1(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH1<ndebug>::MdAtxH1(::pltxt2htm::ast2::MdAtxH1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH1<ndebug>::~MdAtxH1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH1<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH1<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH2<ndebug>::MdAtxH2(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH2<ndebug>::MdAtxH2(::pltxt2htm::ast2::MdAtxH2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH2<ndebug>::~MdAtxH2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH2<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH2<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH3<ndebug>::MdAtxH3(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH3<ndebug>::MdAtxH3(::pltxt2htm::ast2::MdAtxH3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH3<ndebug>::~MdAtxH3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH3<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH3<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH4<ndebug>::MdAtxH4(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH4<ndebug>::MdAtxH4(::pltxt2htm::ast2::MdAtxH4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH4<ndebug>::~MdAtxH4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH4<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH4<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH5<ndebug>::MdAtxH5(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH5<ndebug>::MdAtxH5(::pltxt2htm::ast2::MdAtxH5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH5<ndebug>::~MdAtxH5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH5<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH5<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH6<ndebug>::MdAtxH6(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH6<ndebug>::MdAtxH6(::pltxt2htm::ast2::MdAtxH6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdAtxH6<ndebug>::~MdAtxH6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdAtxH6<ndebug>::operator=(::pltxt2htm::ast2::MdAtxH6<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdAtxH6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(
    ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>::~MdCodeFenceBacktick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>::operator=(
    ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdCodeFenceBacktick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_, ::exception::optional<::fast_io::u8string>&& lang_) noexcept
    : subast(::std::move(subast_)),
      lang(::std::move(lang_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(
    ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>::~MdCodeFenceTilde() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>::operator=(
    ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>&&) noexcept -> ::pltxt2htm::ast2::MdCodeFenceTilde<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(
    ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>::~MdCodeSpan1Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>::operator=(
    ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdCodeSpan1Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(
    ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>::~MdCodeSpan2Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>::operator=(
    ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdCodeSpan2Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(
    ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>::~MdCodeSpan3Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>::operator=(
    ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdCodeSpan3Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(
    ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>::~MdSingleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>::operator=(
    ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdSingleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>::~MdDoubleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>::operator=(
    ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdDoubleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(
    ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>::~MdTripleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>::operator=(
    ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdTripleEmphasisAsterisk<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>::~MdSingleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>::operator=(
    ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdSingleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>::~MdDoubleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>::operator=(
    ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdDoubleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>::~MdTripleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>::operator=(
    ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdTripleEmphasisUnderscore<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDel<ndebug>::MdDel(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDel<ndebug>::MdDel(::pltxt2htm::ast2::MdDel<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdDel<ndebug>::~MdDel() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdDel<ndebug>::operator=(::pltxt2htm::ast2::MdDel<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdDel<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLink<ndebug>::MdLink(::pltxt2htm::ast2::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::ast2::Url<ndebug>&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLink<ndebug>::MdLink(::pltxt2htm::ast2::MdLink<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLink<ndebug>::~MdLink() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdLink<ndebug>::operator=(::pltxt2htm::ast2::MdLink<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdLink<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdImage<ndebug>::MdImage(::pltxt2htm::ast2::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::ast2::Url<ndebug>&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdImage<ndebug>::MdImage(::pltxt2htm::ast2::MdImage<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdImage<ndebug>::~MdImage() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdImage<ndebug>::operator=(::pltxt2htm::ast2::MdImage<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdImage<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdBlockQuotes<ndebug>::MdBlockQuotes(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdBlockQuotes<ndebug>::MdBlockQuotes(::pltxt2htm::ast2::MdBlockQuotes<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdBlockQuotes<ndebug>::~MdBlockQuotes() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdBlockQuotes<ndebug>::operator=(::pltxt2htm::ast2::MdBlockQuotes<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdBlockQuotes<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdUl<ndebug>::MdUl(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdUl<ndebug>::MdUl(::pltxt2htm::ast2::MdUl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdUl<ndebug>::~MdUl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdUl<ndebug>::operator=(::pltxt2htm::ast2::MdUl<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdUl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdOl<ndebug>::MdOl(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdOl<ndebug>::MdOl(::pltxt2htm::ast2::MdOl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdOl<ndebug>::~MdOl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdOl<ndebug>::operator=(::pltxt2htm::ast2::MdOl<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdOl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLi<ndebug>::MdLi(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLi<ndebug>::MdLi(::pltxt2htm::ast2::MdLi<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLi<ndebug>::~MdLi() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdLi<ndebug>::operator=(::pltxt2htm::ast2::MdLi<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdLi<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexInline<ndebug>::MdLatexInline(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexInline<ndebug>::MdLatexInline(::pltxt2htm::ast2::MdLatexInline<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexInline<ndebug>::~MdLatexInline() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdLatexInline<ndebug>::operator=(::pltxt2htm::ast2::MdLatexInline<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdLatexInline<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexBlock<ndebug>::MdLatexBlock(::pltxt2htm::ast2::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexBlock<ndebug>::MdLatexBlock(::pltxt2htm::ast2::MdLatexBlock<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::ast2::MdLatexBlock<ndebug>::~MdLatexBlock() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::ast2::MdLatexBlock<ndebug>::operator=(::pltxt2htm::ast2::MdLatexBlock<ndebug>&&) noexcept
    -> ::pltxt2htm::ast2::MdLatexBlock<ndebug>& = default;

} // namespace pltxt2htm::ast2
