#pragma once

namespace pltxt2htm::ast2 {

constexpr MdAtxH1::MdAtxH1(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH1::MdAtxH1(MdAtxH1&&) noexcept = default;
constexpr MdAtxH1::~MdAtxH1() noexcept = default;
constexpr auto MdAtxH1::operator=(MdAtxH1&&) noexcept -> MdAtxH1& = default;

constexpr MdAtxH2::MdAtxH2(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH2::MdAtxH2(MdAtxH2&&) noexcept = default;
constexpr MdAtxH2::~MdAtxH2() noexcept = default;
constexpr auto MdAtxH2::operator=(MdAtxH2&&) noexcept -> MdAtxH2& = default;

constexpr MdAtxH3::MdAtxH3(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH3::MdAtxH3(MdAtxH3&&) noexcept = default;
constexpr MdAtxH3::~MdAtxH3() noexcept = default;
constexpr auto MdAtxH3::operator=(MdAtxH3&&) noexcept -> MdAtxH3& = default;

constexpr MdAtxH4::MdAtxH4(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH4::MdAtxH4(MdAtxH4&&) noexcept = default;
constexpr MdAtxH4::~MdAtxH4() noexcept = default;
constexpr auto MdAtxH4::operator=(MdAtxH4&&) noexcept -> MdAtxH4& = default;

constexpr MdAtxH5::MdAtxH5(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH5::MdAtxH5(MdAtxH5&&) noexcept = default;
constexpr MdAtxH5::~MdAtxH5() noexcept = default;
constexpr auto MdAtxH5::operator=(MdAtxH5&&) noexcept -> MdAtxH5& = default;

constexpr MdAtxH6::MdAtxH6(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdAtxH6::MdAtxH6(MdAtxH6&&) noexcept = default;
constexpr MdAtxH6::~MdAtxH6() noexcept = default;
constexpr auto MdAtxH6::operator=(MdAtxH6&&) noexcept -> MdAtxH6& = default;

constexpr MdCodeFenceBacktick::MdCodeFenceBacktick(::pltxt2htm::ast2::Ast&& subast, ::exception::optional<::fast_io::u8string>&& lang) noexcept
    : subast_(::std::move(subast)), lang_(::std::move(lang)) {}
constexpr MdCodeFenceBacktick::MdCodeFenceBacktick(MdCodeFenceBacktick&&) noexcept = default;
constexpr MdCodeFenceBacktick::~MdCodeFenceBacktick() noexcept = default;
constexpr auto MdCodeFenceBacktick::operator=(MdCodeFenceBacktick&&) noexcept -> MdCodeFenceBacktick& = default;

constexpr MdCodeFenceTilde::MdCodeFenceTilde(::pltxt2htm::ast2::Ast&& subast, ::exception::optional<::fast_io::u8string>&& lang) noexcept
    : subast_(::std::move(subast)), lang_(::std::move(lang)) {}
constexpr MdCodeFenceTilde::MdCodeFenceTilde(MdCodeFenceTilde&&) noexcept = default;
constexpr MdCodeFenceTilde::~MdCodeFenceTilde() noexcept = default;
constexpr auto MdCodeFenceTilde::operator=(MdCodeFenceTilde&&) noexcept -> MdCodeFenceTilde& = default;

constexpr MdCodeSpan1Backtick::MdCodeSpan1Backtick(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdCodeSpan1Backtick::MdCodeSpan1Backtick(MdCodeSpan1Backtick&&) noexcept = default;
constexpr MdCodeSpan1Backtick::~MdCodeSpan1Backtick() noexcept = default;
constexpr auto MdCodeSpan1Backtick::operator=(MdCodeSpan1Backtick&&) noexcept -> MdCodeSpan1Backtick& = default;

constexpr MdCodeSpan2Backtick::MdCodeSpan2Backtick(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdCodeSpan2Backtick::MdCodeSpan2Backtick(MdCodeSpan2Backtick&&) noexcept = default;
constexpr MdCodeSpan2Backtick::~MdCodeSpan2Backtick() noexcept = default;
constexpr auto MdCodeSpan2Backtick::operator=(MdCodeSpan2Backtick&&) noexcept -> MdCodeSpan2Backtick& = default;

constexpr MdCodeSpan3Backtick::MdCodeSpan3Backtick(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdCodeSpan3Backtick::MdCodeSpan3Backtick(MdCodeSpan3Backtick&&) noexcept = default;
constexpr MdCodeSpan3Backtick::~MdCodeSpan3Backtick() noexcept = default;
constexpr auto MdCodeSpan3Backtick::operator=(MdCodeSpan3Backtick&&) noexcept -> MdCodeSpan3Backtick& = default;

constexpr MdSingleEmphasisAsterisk::MdSingleEmphasisAsterisk(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdSingleEmphasisAsterisk::MdSingleEmphasisAsterisk(MdSingleEmphasisAsterisk&&) noexcept = default;
constexpr MdSingleEmphasisAsterisk::~MdSingleEmphasisAsterisk() noexcept = default;
constexpr auto MdSingleEmphasisAsterisk::operator=(MdSingleEmphasisAsterisk&&) noexcept -> MdSingleEmphasisAsterisk& = default;

constexpr MdDoubleEmphasisAsterisk::MdDoubleEmphasisAsterisk(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdDoubleEmphasisAsterisk::MdDoubleEmphasisAsterisk(MdDoubleEmphasisAsterisk&&) noexcept = default;
constexpr MdDoubleEmphasisAsterisk::~MdDoubleEmphasisAsterisk() noexcept = default;
constexpr auto MdDoubleEmphasisAsterisk::operator=(MdDoubleEmphasisAsterisk&&) noexcept -> MdDoubleEmphasisAsterisk& = default;

constexpr MdTripleEmphasisAsterisk::MdTripleEmphasisAsterisk(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdTripleEmphasisAsterisk::MdTripleEmphasisAsterisk(MdTripleEmphasisAsterisk&&) noexcept = default;
constexpr MdTripleEmphasisAsterisk::~MdTripleEmphasisAsterisk() noexcept = default;
constexpr auto MdTripleEmphasisAsterisk::operator=(MdTripleEmphasisAsterisk&&) noexcept -> MdTripleEmphasisAsterisk& = default;

constexpr MdSingleEmphasisUnderscore::MdSingleEmphasisUnderscore(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdSingleEmphasisUnderscore::MdSingleEmphasisUnderscore(MdSingleEmphasisUnderscore&&) noexcept = default;
constexpr MdSingleEmphasisUnderscore::~MdSingleEmphasisUnderscore() noexcept = default;
constexpr auto MdSingleEmphasisUnderscore::operator=(MdSingleEmphasisUnderscore&&) noexcept -> MdSingleEmphasisUnderscore& = default;

constexpr MdDoubleEmphasisUnderscore::MdDoubleEmphasisUnderscore(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdDoubleEmphasisUnderscore::MdDoubleEmphasisUnderscore(MdDoubleEmphasisUnderscore&&) noexcept = default;
constexpr MdDoubleEmphasisUnderscore::~MdDoubleEmphasisUnderscore() noexcept = default;
constexpr auto MdDoubleEmphasisUnderscore::operator=(MdDoubleEmphasisUnderscore&&) noexcept -> MdDoubleEmphasisUnderscore& = default;

constexpr MdTripleEmphasisUnderscore::MdTripleEmphasisUnderscore(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdTripleEmphasisUnderscore::MdTripleEmphasisUnderscore(MdTripleEmphasisUnderscore&&) noexcept = default;
constexpr MdTripleEmphasisUnderscore::~MdTripleEmphasisUnderscore() noexcept = default;
constexpr auto MdTripleEmphasisUnderscore::operator=(MdTripleEmphasisUnderscore&&) noexcept -> MdTripleEmphasisUnderscore& = default;

constexpr MdDel::MdDel(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdDel::MdDel(MdDel&&) noexcept = default;
constexpr MdDel::~MdDel() noexcept = default;
constexpr auto MdDel::operator=(MdDel&&) noexcept -> MdDel& = default;

constexpr MdLink::MdLink(::pltxt2htm::ast2::Ast&& subast, ::pltxt2htm::ast2::Url&& url) noexcept
    : subast_(::std::move(subast)), url_(::std::move(url)) {}
constexpr MdLink::MdLink(MdLink&&) noexcept = default;
constexpr MdLink::~MdLink() noexcept = default;
constexpr auto MdLink::operator=(MdLink&&) noexcept -> MdLink& = default;

constexpr MdImage::MdImage(::pltxt2htm::ast2::Ast&& subast, ::pltxt2htm::ast2::Url&& url) noexcept
    : subast_(::std::move(subast)), url_(::std::move(url)) {}
constexpr MdImage::MdImage(MdImage&&) noexcept = default;
constexpr MdImage::~MdImage() noexcept = default;
constexpr auto MdImage::operator=(MdImage&&) noexcept -> MdImage& = default;

constexpr MdBlockQuotes::MdBlockQuotes(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdBlockQuotes::MdBlockQuotes(MdBlockQuotes&&) noexcept = default;
constexpr MdBlockQuotes::~MdBlockQuotes() noexcept = default;
constexpr auto MdBlockQuotes::operator=(MdBlockQuotes&&) noexcept -> MdBlockQuotes& = default;

constexpr MdUl::MdUl(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdUl::MdUl(MdUl&&) noexcept = default;
constexpr MdUl::~MdUl() noexcept = default;
constexpr auto MdUl::operator=(MdUl&&) noexcept -> MdUl& = default;

constexpr MdOl::MdOl(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdOl::MdOl(MdOl&&) noexcept = default;
constexpr MdOl::~MdOl() noexcept = default;
constexpr auto MdOl::operator=(MdOl&&) noexcept -> MdOl& = default;

constexpr MdLi::MdLi(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdLi::MdLi(MdLi&&) noexcept = default;
constexpr MdLi::~MdLi() noexcept = default;
constexpr auto MdLi::operator=(MdLi&&) noexcept -> MdLi& = default;

constexpr MdLatexInline::MdLatexInline(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdLatexInline::MdLatexInline(MdLatexInline&&) noexcept = default;
constexpr MdLatexInline::~MdLatexInline() noexcept = default;
constexpr auto MdLatexInline::operator=(MdLatexInline&&) noexcept -> MdLatexInline& = default;

constexpr MdLatexBlock::MdLatexBlock(::pltxt2htm::ast2::Ast&& subast) noexcept : subast_(::std::move(subast)) {}
constexpr MdLatexBlock::MdLatexBlock(MdLatexBlock&&) noexcept = default;
constexpr MdLatexBlock::~MdLatexBlock() noexcept = default;
constexpr auto MdLatexBlock::operator=(MdLatexBlock&&) noexcept -> MdLatexBlock& = default;

} // namespace pltxt2htm::ast2
