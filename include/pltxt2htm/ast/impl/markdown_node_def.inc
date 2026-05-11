#pragma once

namespace pltxt2htm::ast2 {

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH1<ndebug>::MdAtxH1(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH1<ndebug>::MdAtxH1(MdAtxH1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH1<ndebug>::~MdAtxH1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH1<ndebug>::operator=(MdAtxH1<ndebug>&&) noexcept -> MdAtxH1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH2<ndebug>::MdAtxH2(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH2<ndebug>::MdAtxH2(MdAtxH2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH2<ndebug>::~MdAtxH2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH2<ndebug>::operator=(MdAtxH2<ndebug>&&) noexcept -> MdAtxH2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH3<ndebug>::MdAtxH3(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH3<ndebug>::MdAtxH3(MdAtxH3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH3<ndebug>::~MdAtxH3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH3<ndebug>::operator=(MdAtxH3<ndebug>&&) noexcept -> MdAtxH3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH4<ndebug>::MdAtxH4(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH4<ndebug>::MdAtxH4(MdAtxH4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH4<ndebug>::~MdAtxH4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH4<ndebug>::operator=(MdAtxH4<ndebug>&&) noexcept -> MdAtxH4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH5<ndebug>::MdAtxH5(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH5<ndebug>::MdAtxH5(MdAtxH5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH5<ndebug>::~MdAtxH5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH5<ndebug>::operator=(MdAtxH5<ndebug>&&) noexcept -> MdAtxH5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH6<ndebug>::MdAtxH6(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH6<ndebug>::MdAtxH6(MdAtxH6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdAtxH6<ndebug>::~MdAtxH6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdAtxH6<ndebug>::operator=(MdAtxH6<ndebug>&&) noexcept -> MdAtxH6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                                           ::exception::optional<::fast_io::u8string>&& lang) noexcept
    : subast_(::std::move(subast)),
      lang_(::std::move(lang)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceBacktick<ndebug>::MdCodeFenceBacktick(MdCodeFenceBacktick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceBacktick<ndebug>::~MdCodeFenceBacktick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdCodeFenceBacktick<ndebug>::operator=(MdCodeFenceBacktick<ndebug>&&) noexcept
    -> MdCodeFenceBacktick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                                     ::exception::optional<::fast_io::u8string>&& lang) noexcept
    : subast_(::std::move(subast)),
      lang_(::std::move(lang)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceTilde<ndebug>::MdCodeFenceTilde(MdCodeFenceTilde<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeFenceTilde<ndebug>::~MdCodeFenceTilde() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdCodeFenceTilde<ndebug>::operator=(MdCodeFenceTilde<ndebug>&&) noexcept
    -> MdCodeFenceTilde<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan1Backtick<ndebug>::MdCodeSpan1Backtick(MdCodeSpan1Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan1Backtick<ndebug>::~MdCodeSpan1Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdCodeSpan1Backtick<ndebug>::operator=(MdCodeSpan1Backtick<ndebug>&&) noexcept
    -> MdCodeSpan1Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan2Backtick<ndebug>::MdCodeSpan2Backtick(MdCodeSpan2Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan2Backtick<ndebug>::~MdCodeSpan2Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdCodeSpan2Backtick<ndebug>::operator=(MdCodeSpan2Backtick<ndebug>&&) noexcept
    -> MdCodeSpan2Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan3Backtick<ndebug>::MdCodeSpan3Backtick(MdCodeSpan3Backtick<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdCodeSpan3Backtick<ndebug>::~MdCodeSpan3Backtick() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdCodeSpan3Backtick<ndebug>::operator=(MdCodeSpan3Backtick<ndebug>&&) noexcept
    -> MdCodeSpan3Backtick<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisAsterisk<ndebug>::MdSingleEmphasisAsterisk(MdSingleEmphasisAsterisk<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisAsterisk<ndebug>::~MdSingleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdSingleEmphasisAsterisk<ndebug>::operator=(MdSingleEmphasisAsterisk<ndebug>&&) noexcept
    -> MdSingleEmphasisAsterisk& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisAsterisk<ndebug>::MdDoubleEmphasisAsterisk(MdDoubleEmphasisAsterisk<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisAsterisk<ndebug>::~MdDoubleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdDoubleEmphasisAsterisk<ndebug>::operator=(MdDoubleEmphasisAsterisk<ndebug>&&) noexcept
    -> MdDoubleEmphasisAsterisk& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisAsterisk<ndebug>::MdTripleEmphasisAsterisk(MdTripleEmphasisAsterisk<ndebug>&&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisAsterisk<ndebug>::~MdTripleEmphasisAsterisk() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdTripleEmphasisAsterisk<ndebug>::operator=(MdTripleEmphasisAsterisk<ndebug>&&) noexcept
    -> MdTripleEmphasisAsterisk& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisUnderscore<ndebug>::MdSingleEmphasisUnderscore(
    MdSingleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdSingleEmphasisUnderscore<ndebug>::~MdSingleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdSingleEmphasisUnderscore<ndebug>::operator=(MdSingleEmphasisUnderscore<ndebug>&&) noexcept
    -> MdSingleEmphasisUnderscore& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisUnderscore<ndebug>::MdDoubleEmphasisUnderscore(
    MdDoubleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdDoubleEmphasisUnderscore<ndebug>::~MdDoubleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdDoubleEmphasisUnderscore<ndebug>::operator=(MdDoubleEmphasisUnderscore<ndebug>&&) noexcept
    -> MdDoubleEmphasisUnderscore& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    ::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisUnderscore<ndebug>::MdTripleEmphasisUnderscore(
    MdTripleEmphasisUnderscore<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdTripleEmphasisUnderscore<ndebug>::~MdTripleEmphasisUnderscore() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdTripleEmphasisUnderscore<ndebug>::operator=(MdTripleEmphasisUnderscore<ndebug>&&) noexcept
    -> MdTripleEmphasisUnderscore& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdDel<ndebug>::MdDel(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdDel<ndebug>::MdDel(MdDel<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdDel<ndebug>::~MdDel() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdDel<ndebug>::operator=(MdDel<ndebug>&&) noexcept -> MdDel<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdLink<ndebug>::MdLink(::pltxt2htm::ast2::Ast<ndebug>&& subast, ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept
    : subast_(::std::move(subast)),
      url_(::std::move(url)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdLink<ndebug>::MdLink(MdLink<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdLink<ndebug>::~MdLink() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdLink<ndebug>::operator=(MdLink<ndebug>&&) noexcept -> MdLink<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdImage<ndebug>::MdImage(::pltxt2htm::ast2::Ast<ndebug>&& subast,
                                   ::pltxt2htm::ast2::Url<ndebug>&& url) noexcept
    : subast_(::std::move(subast)),
      url_(::std::move(url)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdImage<ndebug>::MdImage(MdImage<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdImage<ndebug>::~MdImage() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdImage<ndebug>::operator=(MdImage<ndebug>&&) noexcept -> MdImage<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdBlockQuotes<ndebug>::MdBlockQuotes(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdBlockQuotes<ndebug>::MdBlockQuotes(MdBlockQuotes<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdBlockQuotes<ndebug>::~MdBlockQuotes() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdBlockQuotes<ndebug>::operator=(MdBlockQuotes<ndebug>&&) noexcept -> MdBlockQuotes<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdUl<ndebug>::MdUl(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdUl<ndebug>::MdUl(MdUl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdUl<ndebug>::~MdUl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdUl<ndebug>::operator=(MdUl<ndebug>&&) noexcept -> MdUl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdOl<ndebug>::MdOl(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdOl<ndebug>::MdOl(MdOl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdOl<ndebug>::~MdOl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdOl<ndebug>::operator=(MdOl<ndebug>&&) noexcept -> MdOl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdLi<ndebug>::MdLi(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdLi<ndebug>::MdLi(MdLi<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdLi<ndebug>::~MdLi() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdLi<ndebug>::operator=(MdLi<ndebug>&&) noexcept -> MdLi<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexInline<ndebug>::MdLatexInline(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexInline<ndebug>::MdLatexInline(MdLatexInline<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexInline<ndebug>::~MdLatexInline() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdLatexInline<ndebug>::operator=(MdLatexInline<ndebug>&&) noexcept -> MdLatexInline<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexBlock<ndebug>::MdLatexBlock(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept
    : subast_(::std::move(subast)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexBlock<ndebug>::MdLatexBlock(MdLatexBlock<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr MdLatexBlock<ndebug>::~MdLatexBlock() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto MdLatexBlock<ndebug>::operator=(MdLatexBlock<ndebug>&&) noexcept -> MdLatexBlock<ndebug>& = default;

} // namespace pltxt2htm::ast2
