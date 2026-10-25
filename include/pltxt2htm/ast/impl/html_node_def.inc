#pragma once

constexpr ::pltxt2htm::HtmlH1::HtmlH1(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH1::HtmlH1(::pltxt2htm::HtmlH1&&) noexcept = default;

constexpr auto ::pltxt2htm::HtmlH1::operator=(this HtmlH1& self, ::pltxt2htm::HtmlH1&&) noexcept
    -> ::pltxt2htm::HtmlH1& = default;

constexpr auto ::pltxt2htm::HtmlH1::operator==(this HtmlH1 const&,
                                                       ::pltxt2htm::HtmlH1 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlH2::HtmlH2(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH2::HtmlH2(::pltxt2htm::HtmlH2&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlH2::operator=(this HtmlH2& self, ::pltxt2htm::HtmlH2&&) noexcept
    -> ::pltxt2htm::HtmlH2& = default;

constexpr auto ::pltxt2htm::HtmlH2::operator==(this HtmlH2 const&,
                                                       ::pltxt2htm::HtmlH2 const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlH3::HtmlH3(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH3::HtmlH3(::pltxt2htm::HtmlH3&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlH3::operator=(this HtmlH3& self, ::pltxt2htm::HtmlH3&&) noexcept
    -> ::pltxt2htm::HtmlH3& = default;

constexpr auto ::pltxt2htm::HtmlH3::operator==(this HtmlH3 const&,
                                                       ::pltxt2htm::HtmlH3 const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlH4::HtmlH4(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH4::HtmlH4(::pltxt2htm::HtmlH4&&) noexcept = default;

constexpr auto ::pltxt2htm::HtmlH4::operator=(this HtmlH4& self, ::pltxt2htm::HtmlH4&&) noexcept
    -> ::pltxt2htm::HtmlH4& = default;

constexpr auto ::pltxt2htm::HtmlH4::operator==(this HtmlH4 const&,
                                                       ::pltxt2htm::HtmlH4 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlH5::HtmlH5(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH5::HtmlH5(::pltxt2htm::HtmlH5&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlH5::operator=(this HtmlH5& self, ::pltxt2htm::HtmlH5&&) noexcept
    -> ::pltxt2htm::HtmlH5& = default;

constexpr auto ::pltxt2htm::HtmlH5::operator==(this HtmlH5 const&,
                                                       ::pltxt2htm::HtmlH5 const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlH6::HtmlH6(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlH6::HtmlH6(::pltxt2htm::HtmlH6&&) noexcept = default;

constexpr auto ::pltxt2htm::HtmlH6::operator=(this HtmlH6& self, ::pltxt2htm::HtmlH6&&) noexcept
    -> ::pltxt2htm::HtmlH6& = default;

constexpr auto ::pltxt2htm::HtmlH6::operator==(this HtmlH6 const&,
                                                       ::pltxt2htm::HtmlH6 const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlP::HtmlP(::pltxt2htm::Ast&& subast_, ::pltxt2htm::TextAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

constexpr ::pltxt2htm::HtmlP::HtmlP(::pltxt2htm::HtmlP&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlP::operator=(this HtmlP& self, ::pltxt2htm::HtmlP&&) noexcept
    -> ::pltxt2htm::HtmlP& = default;

constexpr auto ::pltxt2htm::HtmlP::operator==(this HtmlP const&,
                                                      ::pltxt2htm::HtmlP const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlDel::HtmlDel(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlDel::HtmlDel(::pltxt2htm::HtmlDel&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlDel::operator=(this HtmlDel& self,
                                                       ::pltxt2htm::HtmlDel&&) noexcept
    -> ::pltxt2htm::HtmlDel& = default;
constexpr auto ::pltxt2htm::HtmlDel::operator==(this HtmlDel const&,
                                                        ::pltxt2htm::HtmlDel const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlU::HtmlU(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlU::HtmlU(::pltxt2htm::HtmlU&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlU::operator=(this HtmlU& self, ::pltxt2htm::HtmlU&&) noexcept
    -> ::pltxt2htm::HtmlU& = default;

constexpr auto ::pltxt2htm::HtmlU::operator==(this HtmlU const&,
                                                      ::pltxt2htm::HtmlU const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlS::HtmlS(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlS::HtmlS(::pltxt2htm::HtmlS&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlS::operator=(this HtmlS& self, ::pltxt2htm::HtmlS&&) noexcept
    -> ::pltxt2htm::HtmlS& = default;

constexpr auto ::pltxt2htm::HtmlS::operator==(this HtmlS const&,
                                                      ::pltxt2htm::HtmlS const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlSup::HtmlSup(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlSup::HtmlSup(::pltxt2htm::HtmlSup&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlSup::operator=(this HtmlSup& self,
                                                       ::pltxt2htm::HtmlSup&&) noexcept
    -> ::pltxt2htm::HtmlSup& = default;
constexpr auto ::pltxt2htm::HtmlSup::operator==(this HtmlSup const&,
                                                        ::pltxt2htm::HtmlSup const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlSub::HtmlSub(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlSub::HtmlSub(::pltxt2htm::HtmlSub&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlSub::operator=(this HtmlSub& self,
                                                       ::pltxt2htm::HtmlSub&&) noexcept
    -> ::pltxt2htm::HtmlSub& = default;
constexpr auto ::pltxt2htm::HtmlSub::operator==(this HtmlSub const&,
                                                        ::pltxt2htm::HtmlSub const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlNote::HtmlNote(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlNote::HtmlNote(::pltxt2htm::HtmlNote&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlNote::operator=(this HtmlNote& self,
                                                        ::pltxt2htm::HtmlNote&&) noexcept
    -> ::pltxt2htm::HtmlNote& = default;
constexpr auto ::pltxt2htm::HtmlNote::operator==(this HtmlNote const&,
                                                         ::pltxt2htm::HtmlNote const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlEm::HtmlEm(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlEm::HtmlEm(::pltxt2htm::HtmlEm&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlEm::operator=(this HtmlEm& self, ::pltxt2htm::HtmlEm&&) noexcept
    -> ::pltxt2htm::HtmlEm& = default;

constexpr auto ::pltxt2htm::HtmlEm::operator==(this HtmlEm const&,
                                                       ::pltxt2htm::HtmlEm const&) noexcept -> bool = default;

constexpr ::pltxt2htm::HtmlStrong::HtmlStrong(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlStrong::HtmlStrong(::pltxt2htm::HtmlStrong&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlStrong::operator=(this HtmlStrong& self,
                                                          ::pltxt2htm::HtmlStrong&&) noexcept
    -> ::pltxt2htm::HtmlStrong& = default;
constexpr auto ::pltxt2htm::HtmlStrong::operator==(this HtmlStrong const&,
                                                           ::pltxt2htm::HtmlStrong const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlMark::HtmlMark(::pltxt2htm::Ast&& subast_,
                                                  ::pltxt2htm::container::U8String&& background_color_) noexcept
    : subast(::std::move(subast_)),
      background_color(::std::move(background_color_)) {
}

constexpr ::pltxt2htm::HtmlMark::HtmlMark(::pltxt2htm::HtmlMark&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlMark::operator=(this HtmlMark& self,
                                                        ::pltxt2htm::HtmlMark&&) noexcept
    -> ::pltxt2htm::HtmlMark& = default;
constexpr auto ::pltxt2htm::HtmlMark::operator==(this HtmlMark const&,
                                                         ::pltxt2htm::HtmlMark const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlCode::HtmlCode(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlCode::HtmlCode(::pltxt2htm::HtmlCode&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlCode::operator=(this HtmlCode& self,
                                                        ::pltxt2htm::HtmlCode&&) noexcept
    -> ::pltxt2htm::HtmlCode& = default;
constexpr auto ::pltxt2htm::HtmlCode::operator==(this HtmlCode const&,
                                                         ::pltxt2htm::HtmlCode const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlBlockquote::HtmlBlockquote(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::HtmlBlockquote::HtmlBlockquote(::pltxt2htm::HtmlBlockquote&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlBlockquote::operator=(this HtmlBlockquote& self,
                                                              ::pltxt2htm::HtmlBlockquote&&) noexcept
    -> ::pltxt2htm::HtmlBlockquote& = default;
constexpr auto ::pltxt2htm::HtmlBlockquote::operator==(this HtmlBlockquote const&,
                                                               ::pltxt2htm::HtmlBlockquote const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlSpan::HtmlSpan(
    ::pltxt2htm::Ast&& subast_, ::pltxt2htm::container::U8String&& color_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_,
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue> vertical_align_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)),
      style{font_size_, vertical_align_} {
}

constexpr ::pltxt2htm::HtmlSpan::HtmlSpan(::pltxt2htm::HtmlSpan&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlSpan::operator=(this HtmlSpan& self,
                                                        ::pltxt2htm::HtmlSpan&&) noexcept
    -> ::pltxt2htm::HtmlSpan& = default;
constexpr auto ::pltxt2htm::HtmlSpan::operator==(this HtmlSpan const&,
                                                         ::pltxt2htm::HtmlSpan const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::HtmlA::HtmlA(::pltxt2htm::Ast&& subast_, ::pltxt2htm::Url&& url_,
                                            bool internal_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)),
      internal(internal_) {
}

constexpr ::pltxt2htm::HtmlA::HtmlA(::pltxt2htm::HtmlA&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlA::operator=(this HtmlA& self, ::pltxt2htm::HtmlA&&) noexcept
    -> ::pltxt2htm::HtmlA& = default;
constexpr auto ::pltxt2htm::HtmlA::operator==(this HtmlA const&,
                                                      ::pltxt2htm::HtmlA const&) noexcept -> bool = default;
constexpr ::pltxt2htm::HtmlDiv::HtmlDiv(
    ::pltxt2htm::Ast&& subast_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
    : subast(::std::move(subast_)),
      margins{left_, right_} {
}

constexpr ::pltxt2htm::HtmlDiv::HtmlDiv(::pltxt2htm::HtmlDiv&&) noexcept = default;
constexpr auto ::pltxt2htm::HtmlDiv::operator=(this HtmlDiv& self,
                                                       ::pltxt2htm::HtmlDiv&&) noexcept
    -> ::pltxt2htm::HtmlDiv& = default;
constexpr auto ::pltxt2htm::HtmlDiv::operator==(this HtmlDiv const&,
                                                        ::pltxt2htm::HtmlDiv const&) noexcept -> bool = default;
