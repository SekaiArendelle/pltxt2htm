#pragma once

constexpr ::pltxt2htm::MdAtxH1::MdAtxH1(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH1::MdAtxH1(::pltxt2htm::MdAtxH1&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH1::operator=(this MdAtxH1& self,
                                                       ::pltxt2htm::MdAtxH1&&) noexcept
    -> ::pltxt2htm::MdAtxH1& = default;
constexpr auto ::pltxt2htm::MdAtxH1::operator==(this MdAtxH1 const&,
                                                        ::pltxt2htm::MdAtxH1 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdAtxH2::MdAtxH2(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH2::MdAtxH2(::pltxt2htm::MdAtxH2&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH2::operator=(this MdAtxH2& self,
                                                       ::pltxt2htm::MdAtxH2&&) noexcept
    -> ::pltxt2htm::MdAtxH2& = default;
constexpr auto ::pltxt2htm::MdAtxH2::operator==(this MdAtxH2 const&,
                                                        ::pltxt2htm::MdAtxH2 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdAtxH3::MdAtxH3(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH3::MdAtxH3(::pltxt2htm::MdAtxH3&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH3::operator=(this MdAtxH3& self,
                                                       ::pltxt2htm::MdAtxH3&&) noexcept
    -> ::pltxt2htm::MdAtxH3& = default;
constexpr auto ::pltxt2htm::MdAtxH3::operator==(this MdAtxH3 const&,
                                                        ::pltxt2htm::MdAtxH3 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdAtxH4::MdAtxH4(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH4::MdAtxH4(::pltxt2htm::MdAtxH4&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH4::operator=(this MdAtxH4& self,
                                                       ::pltxt2htm::MdAtxH4&&) noexcept
    -> ::pltxt2htm::MdAtxH4& = default;
constexpr auto ::pltxt2htm::MdAtxH4::operator==(this MdAtxH4 const&,
                                                        ::pltxt2htm::MdAtxH4 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdAtxH5::MdAtxH5(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH5::MdAtxH5(::pltxt2htm::MdAtxH5&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH5::operator=(this MdAtxH5& self,
                                                       ::pltxt2htm::MdAtxH5&&) noexcept
    -> ::pltxt2htm::MdAtxH5& = default;
constexpr auto ::pltxt2htm::MdAtxH5::operator==(this MdAtxH5 const&,
                                                        ::pltxt2htm::MdAtxH5 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdAtxH6::MdAtxH6(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdAtxH6::MdAtxH6(::pltxt2htm::MdAtxH6&&) noexcept = default;
constexpr auto ::pltxt2htm::MdAtxH6::operator=(this MdAtxH6& self,
                                                       ::pltxt2htm::MdAtxH6&&) noexcept
    -> ::pltxt2htm::MdAtxH6& = default;
constexpr auto ::pltxt2htm::MdAtxH6::operator==(this MdAtxH6 const&,
                                                        ::pltxt2htm::MdAtxH6 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdCodeSpan1Backtick::MdCodeSpan1Backtick(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdCodeSpan1Backtick::MdCodeSpan1Backtick(
    ::pltxt2htm::MdCodeSpan1Backtick&&) noexcept = default;
constexpr auto ::pltxt2htm::MdCodeSpan1Backtick::operator=(this MdCodeSpan1Backtick& self,
                                                                   ::pltxt2htm::MdCodeSpan1Backtick&&) noexcept
    -> ::pltxt2htm::MdCodeSpan1Backtick& = default;
constexpr auto ::pltxt2htm::MdCodeSpan1Backtick::operator==(
    this MdCodeSpan1Backtick const&, ::pltxt2htm::MdCodeSpan1Backtick const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdCodeSpan2Backtick::MdCodeSpan2Backtick(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdCodeSpan2Backtick::MdCodeSpan2Backtick(
    ::pltxt2htm::MdCodeSpan2Backtick&&) noexcept = default;
constexpr auto ::pltxt2htm::MdCodeSpan2Backtick::operator=(this MdCodeSpan2Backtick& self,
                                                                   ::pltxt2htm::MdCodeSpan2Backtick&&) noexcept
    -> ::pltxt2htm::MdCodeSpan2Backtick& = default;
constexpr auto ::pltxt2htm::MdCodeSpan2Backtick::operator==(
    this MdCodeSpan2Backtick const&, ::pltxt2htm::MdCodeSpan2Backtick const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdCodeSpan3Backtick::MdCodeSpan3Backtick(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdCodeSpan3Backtick::MdCodeSpan3Backtick(
    ::pltxt2htm::MdCodeSpan3Backtick&&) noexcept = default;
constexpr auto ::pltxt2htm::MdCodeSpan3Backtick::operator=(this MdCodeSpan3Backtick& self,
                                                                   ::pltxt2htm::MdCodeSpan3Backtick&&) noexcept
    -> ::pltxt2htm::MdCodeSpan3Backtick& = default;
constexpr auto ::pltxt2htm::MdCodeSpan3Backtick::operator==(
    this MdCodeSpan3Backtick const&, ::pltxt2htm::MdCodeSpan3Backtick const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdSingleEmphasisAsterisk::MdSingleEmphasisAsterisk(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdSingleEmphasisAsterisk::MdSingleEmphasisAsterisk(
    ::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept = default;
constexpr auto ::pltxt2htm::MdSingleEmphasisAsterisk::operator=(
    this MdSingleEmphasisAsterisk& self, ::pltxt2htm::MdSingleEmphasisAsterisk&&) noexcept
    -> ::pltxt2htm::MdSingleEmphasisAsterisk& = default;
constexpr auto ::pltxt2htm::MdSingleEmphasisAsterisk::operator==(
    this MdSingleEmphasisAsterisk const&, ::pltxt2htm::MdSingleEmphasisAsterisk const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdDoubleEmphasisAsterisk::MdDoubleEmphasisAsterisk(
    ::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept = default;
constexpr auto ::pltxt2htm::MdDoubleEmphasisAsterisk::operator=(
    this MdDoubleEmphasisAsterisk& self, ::pltxt2htm::MdDoubleEmphasisAsterisk&&) noexcept
    -> ::pltxt2htm::MdDoubleEmphasisAsterisk& = default;
constexpr auto ::pltxt2htm::MdDoubleEmphasisAsterisk::operator==(
    this MdDoubleEmphasisAsterisk const&, ::pltxt2htm::MdDoubleEmphasisAsterisk const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdTripleEmphasisAsterisk::MdTripleEmphasisAsterisk(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdTripleEmphasisAsterisk::MdTripleEmphasisAsterisk(
    ::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept = default;
constexpr auto ::pltxt2htm::MdTripleEmphasisAsterisk::operator=(
    this MdTripleEmphasisAsterisk& self, ::pltxt2htm::MdTripleEmphasisAsterisk&&) noexcept
    -> ::pltxt2htm::MdTripleEmphasisAsterisk& = default;
constexpr auto ::pltxt2htm::MdTripleEmphasisAsterisk::operator==(
    this MdTripleEmphasisAsterisk const&, ::pltxt2htm::MdTripleEmphasisAsterisk const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdSingleEmphasisUnderscore::MdSingleEmphasisUnderscore(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdSingleEmphasisUnderscore::MdSingleEmphasisUnderscore(
    ::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept = default;
constexpr auto ::pltxt2htm::MdSingleEmphasisUnderscore::operator=(
    this MdSingleEmphasisUnderscore& self, ::pltxt2htm::MdSingleEmphasisUnderscore&&) noexcept
    -> ::pltxt2htm::MdSingleEmphasisUnderscore& = default;
constexpr auto ::pltxt2htm::MdSingleEmphasisUnderscore::operator==(
    this MdSingleEmphasisUnderscore const&, ::pltxt2htm::MdSingleEmphasisUnderscore const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdDoubleEmphasisUnderscore::MdDoubleEmphasisUnderscore(
    ::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept = default;
constexpr auto ::pltxt2htm::MdDoubleEmphasisUnderscore::operator=(
    this MdDoubleEmphasisUnderscore& self, ::pltxt2htm::MdDoubleEmphasisUnderscore&&) noexcept
    -> ::pltxt2htm::MdDoubleEmphasisUnderscore& = default;
constexpr auto ::pltxt2htm::MdDoubleEmphasisUnderscore::operator==(
    this MdDoubleEmphasisUnderscore const&, ::pltxt2htm::MdDoubleEmphasisUnderscore const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdTripleEmphasisUnderscore::MdTripleEmphasisUnderscore(
    ::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdTripleEmphasisUnderscore::MdTripleEmphasisUnderscore(
    ::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept = default;
constexpr auto ::pltxt2htm::MdTripleEmphasisUnderscore::operator=(
    this MdTripleEmphasisUnderscore& self, ::pltxt2htm::MdTripleEmphasisUnderscore&&) noexcept
    -> ::pltxt2htm::MdTripleEmphasisUnderscore& = default;
constexpr auto ::pltxt2htm::MdTripleEmphasisUnderscore::operator==(
    this MdTripleEmphasisUnderscore const&, ::pltxt2htm::MdTripleEmphasisUnderscore const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdDel::MdDel(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdDel::MdDel(::pltxt2htm::MdDel&&) noexcept = default;

constexpr auto ::pltxt2htm::MdDel::operator=(this MdDel& self, ::pltxt2htm::MdDel&&) noexcept
    -> ::pltxt2htm::MdDel& = default;

constexpr auto ::pltxt2htm::MdDel::operator==(this MdDel const&,
                                                      ::pltxt2htm::MdDel const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdLink::MdLink(::pltxt2htm::Ast&& subast_, ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

constexpr ::pltxt2htm::MdLink::MdLink(::pltxt2htm::MdLink&&) noexcept = default;
constexpr auto ::pltxt2htm::MdLink::operator=(this MdLink& self, ::pltxt2htm::MdLink&&) noexcept
    -> ::pltxt2htm::MdLink& = default;

constexpr auto ::pltxt2htm::MdLink::operator==(this MdLink const&,
                                                       ::pltxt2htm::MdLink const&) noexcept -> bool = default;

constexpr ::pltxt2htm::MdImage::MdImage(::pltxt2htm::PlainText&& alt_, ::pltxt2htm::Url&& url_) noexcept
    : alt(::std::move(alt_)),
      url(::std::move(url_)) {
}

constexpr ::pltxt2htm::MdImage::MdImage(::pltxt2htm::MdImage const&) noexcept = default;
constexpr ::pltxt2htm::MdImage::MdImage(::pltxt2htm::MdImage&&) noexcept = default;
constexpr auto ::pltxt2htm::MdImage::operator=(this MdImage& self,
                                                       ::pltxt2htm::MdImage&&) noexcept
    -> ::pltxt2htm::MdImage& = default;
constexpr auto ::pltxt2htm::MdImage::operator==(this MdImage const&,
                                                        ::pltxt2htm::MdImage const&) noexcept -> bool = default;
constexpr ::pltxt2htm::MdBlockQuotes::MdBlockQuotes(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdBlockQuotes::MdBlockQuotes(::pltxt2htm::MdBlockQuotes&&) noexcept = default;
constexpr auto ::pltxt2htm::MdBlockQuotes::operator=(this MdBlockQuotes& self,
                                                             ::pltxt2htm::MdBlockQuotes&&) noexcept
    -> ::pltxt2htm::MdBlockQuotes& = default;
constexpr auto ::pltxt2htm::MdBlockQuotes::operator==(this MdBlockQuotes const&,
                                                              ::pltxt2htm::MdBlockQuotes const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdLatexInline::MdLatexInline(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdLatexInline::MdLatexInline(::pltxt2htm::MdLatexInline&&) noexcept = default;
constexpr auto ::pltxt2htm::MdLatexInline::operator=(this MdLatexInline& self,
                                                             ::pltxt2htm::MdLatexInline&&) noexcept
    -> ::pltxt2htm::MdLatexInline& = default;
constexpr auto ::pltxt2htm::MdLatexInline::operator==(this MdLatexInline const&,
                                                              ::pltxt2htm::MdLatexInline const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::MdLatexBlock::MdLatexBlock(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::MdLatexBlock::MdLatexBlock(::pltxt2htm::MdLatexBlock&&) noexcept = default;
constexpr auto ::pltxt2htm::MdLatexBlock::operator=(this MdLatexBlock& self,
                                                            ::pltxt2htm::MdLatexBlock&&) noexcept
    -> ::pltxt2htm::MdLatexBlock& = default;
constexpr auto ::pltxt2htm::MdLatexBlock::operator==(this MdLatexBlock const&,
                                                             ::pltxt2htm::MdLatexBlock const&) noexcept
    -> bool = default;
