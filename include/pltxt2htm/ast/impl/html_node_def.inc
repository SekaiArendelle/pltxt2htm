#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH1<ndebug>::HtmlH1(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH1<ndebug>::HtmlH1(::pltxt2htm::HtmlH1<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH1<ndebug>::HtmlH1(::pltxt2htm::HtmlH1<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH1<ndebug>::~HtmlH1() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH1<ndebug>::operator=(this HtmlH1<ndebug>& self, ::pltxt2htm::HtmlH1<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH1<ndebug>::operator==(this HtmlH1<ndebug> const&,
                                                       ::pltxt2htm::HtmlH1<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH2<ndebug>::HtmlH2(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH2<ndebug>::HtmlH2(::pltxt2htm::HtmlH2<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH2<ndebug>::HtmlH2(::pltxt2htm::HtmlH2<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH2<ndebug>::~HtmlH2() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH2<ndebug>::operator=(this HtmlH2<ndebug>& self, ::pltxt2htm::HtmlH2<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH2<ndebug>::operator==(this HtmlH2<ndebug> const&,
                                                       ::pltxt2htm::HtmlH2<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH3<ndebug>::HtmlH3(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH3<ndebug>::HtmlH3(::pltxt2htm::HtmlH3<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH3<ndebug>::HtmlH3(::pltxt2htm::HtmlH3<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH3<ndebug>::~HtmlH3() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH3<ndebug>::operator=(this HtmlH3<ndebug>& self, ::pltxt2htm::HtmlH3<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH3<ndebug>::operator==(this HtmlH3<ndebug> const&,
                                                       ::pltxt2htm::HtmlH3<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH4<ndebug>::HtmlH4(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH4<ndebug>::HtmlH4(::pltxt2htm::HtmlH4<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH4<ndebug>::HtmlH4(::pltxt2htm::HtmlH4<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH4<ndebug>::~HtmlH4() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH4<ndebug>::operator=(this HtmlH4<ndebug>& self, ::pltxt2htm::HtmlH4<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH4<ndebug>::operator==(this HtmlH4<ndebug> const&,
                                                       ::pltxt2htm::HtmlH4<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH5<ndebug>::HtmlH5(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH5<ndebug>::HtmlH5(::pltxt2htm::HtmlH5<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH5<ndebug>::HtmlH5(::pltxt2htm::HtmlH5<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH5<ndebug>::~HtmlH5() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH5<ndebug>::operator=(this HtmlH5<ndebug>& self, ::pltxt2htm::HtmlH5<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH5<ndebug>::operator==(this HtmlH5<ndebug> const&,
                                                       ::pltxt2htm::HtmlH5<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH6<ndebug>::HtmlH6(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH6<ndebug>::HtmlH6(::pltxt2htm::HtmlH6<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH6<ndebug>::HtmlH6(::pltxt2htm::HtmlH6<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlH6<ndebug>::~HtmlH6() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH6<ndebug>::operator=(this HtmlH6<ndebug>& self, ::pltxt2htm::HtmlH6<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH6<ndebug>::operator==(this HtmlH6<ndebug> const&,
                                                       ::pltxt2htm::HtmlH6<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::TextAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::HtmlP<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::HtmlP<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::~HtmlP() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlP<ndebug>::operator=(this HtmlP<ndebug>& self, ::pltxt2htm::HtmlP<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlP<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlP<ndebug>::operator==(this HtmlP<ndebug> const&,
                                                      ::pltxt2htm::HtmlP<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDel<ndebug>::HtmlDel(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDel<ndebug>::HtmlDel(::pltxt2htm::HtmlDel<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDel<ndebug>::HtmlDel(::pltxt2htm::HtmlDel<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDel<ndebug>::~HtmlDel() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlDel<ndebug>::operator=(this HtmlDel<ndebug>& self,
                                                       ::pltxt2htm::HtmlDel<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlDel<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlDel<ndebug>::operator==(this HtmlDel<ndebug> const&,
                                                        ::pltxt2htm::HtmlDel<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSup<ndebug>::HtmlSup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSup<ndebug>::HtmlSup(::pltxt2htm::HtmlSup<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSup<ndebug>::HtmlSup(::pltxt2htm::HtmlSup<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSup<ndebug>::~HtmlSup() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSup<ndebug>::operator=(this HtmlSup<ndebug>& self,
                                                       ::pltxt2htm::HtmlSup<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlSup<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSup<ndebug>::operator==(this HtmlSup<ndebug> const&,
                                                        ::pltxt2htm::HtmlSup<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSub<ndebug>::HtmlSub(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSub<ndebug>::HtmlSub(::pltxt2htm::HtmlSub<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSub<ndebug>::HtmlSub(::pltxt2htm::HtmlSub<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSub<ndebug>::~HtmlSub() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSub<ndebug>::operator=(this HtmlSub<ndebug>& self,
                                                       ::pltxt2htm::HtmlSub<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlSub<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSub<ndebug>::operator==(this HtmlSub<ndebug> const&,
                                                        ::pltxt2htm::HtmlSub<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlNote<ndebug>::HtmlNote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlNote<ndebug>::HtmlNote(::pltxt2htm::HtmlNote<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlNote<ndebug>::HtmlNote(::pltxt2htm::HtmlNote<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlNote<ndebug>::~HtmlNote() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlNote<ndebug>::operator=(this HtmlNote<ndebug>& self,
                                                        ::pltxt2htm::HtmlNote<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlNote<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlNote<ndebug>::operator==(this HtmlNote<ndebug> const&,
                                                         ::pltxt2htm::HtmlNote<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlEm<ndebug>::HtmlEm(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlEm<ndebug>::HtmlEm(::pltxt2htm::HtmlEm<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlEm<ndebug>::HtmlEm(::pltxt2htm::HtmlEm<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlEm<ndebug>::~HtmlEm() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlEm<ndebug>::operator=(this HtmlEm<ndebug>& self, ::pltxt2htm::HtmlEm<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlEm<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlEm<ndebug>::operator==(this HtmlEm<ndebug> const&,
                                                       ::pltxt2htm::HtmlEm<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlStrong<ndebug>::HtmlStrong(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlStrong<ndebug>::HtmlStrong(::pltxt2htm::HtmlStrong<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlStrong<ndebug>::HtmlStrong(::pltxt2htm::HtmlStrong<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlStrong<ndebug>::~HtmlStrong() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlStrong<ndebug>::operator=(this HtmlStrong<ndebug>& self,
                                                          ::pltxt2htm::HtmlStrong<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlStrong<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlStrong<ndebug>::operator==(this HtmlStrong<ndebug> const&,
                                                           ::pltxt2htm::HtmlStrong<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlMark<ndebug>::HtmlMark(::pltxt2htm::Ast<ndebug>&& subast_,
                                                  ::fast_io::u8string&& background_color_) noexcept
    : subast(::std::move(subast_)),
      background_color(::std::move(background_color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlMark<ndebug>::HtmlMark(::pltxt2htm::HtmlMark<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlMark<ndebug>::HtmlMark(::pltxt2htm::HtmlMark<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlMark<ndebug>::~HtmlMark() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlMark<ndebug>::operator=(this HtmlMark<ndebug>& self,
                                                        ::pltxt2htm::HtmlMark<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlMark<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlMark<ndebug>::operator==(this HtmlMark<ndebug> const&,
                                                         ::pltxt2htm::HtmlMark<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCode<ndebug>::HtmlCode(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCode<ndebug>::HtmlCode(::pltxt2htm::HtmlCode<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCode<ndebug>::HtmlCode(::pltxt2htm::HtmlCode<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCode<ndebug>::~HtmlCode() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlCode<ndebug>::operator=(this HtmlCode<ndebug>& self,
                                                        ::pltxt2htm::HtmlCode<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlCode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlCode<ndebug>::operator==(this HtmlCode<ndebug> const&,
                                                         ::pltxt2htm::HtmlCode<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlBlockquote<ndebug>::HtmlBlockquote(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlBlockquote<ndebug>::HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlBlockquote<ndebug>::HtmlBlockquote(::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlBlockquote<ndebug>::~HtmlBlockquote() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlBlockquote<ndebug>::operator=(this HtmlBlockquote<ndebug>& self,
                                                              ::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlBlockquote<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlBlockquote<ndebug>::operator==(this HtmlBlockquote<ndebug> const&,
                                                               ::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(
    ::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_,
    ::pltxt2htm::container::optional<::pltxt2htm::ValueWithUnit<double>> font_size_,
    ::pltxt2htm::container::optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)),
      font_size(font_size_),
      vertical_align(vertical_align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(::pltxt2htm::HtmlSpan<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::~HtmlSpan() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::operator=(this HtmlSpan<ndebug>& self,
                                                        ::pltxt2htm::HtmlSpan<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlSpan<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::operator==(this HtmlSpan<ndebug> const&,
                                                         ::pltxt2htm::HtmlSpan<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlA<ndebug>::HtmlA(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_,
                                            bool internal_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)),
      internal(internal_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlA<ndebug>::HtmlA(::pltxt2htm::HtmlA<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlA<ndebug>::HtmlA(::pltxt2htm::HtmlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlA<ndebug>::~HtmlA() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlA<ndebug>::operator=(this HtmlA<ndebug>& self, ::pltxt2htm::HtmlA<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlA<ndebug>::operator==(this HtmlA<ndebug> const&,
                                                      ::pltxt2htm::HtmlA<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDiv<ndebug>::HtmlDiv(
    ::pltxt2htm::Ast<ndebug>&& subast_,
    ::pltxt2htm::container::optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
    : subast(::std::move(subast_)),
      left(left_),
      right(right_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDiv<ndebug>::HtmlDiv(::pltxt2htm::HtmlDiv<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDiv<ndebug>::HtmlDiv(::pltxt2htm::HtmlDiv<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlDiv<ndebug>::~HtmlDiv() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlDiv<ndebug>::operator=(this HtmlDiv<ndebug>& self,
                                                       ::pltxt2htm::HtmlDiv<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlDiv<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlDiv<ndebug>::operator==(this HtmlDiv<ndebug> const&,
                                                        ::pltxt2htm::HtmlDiv<ndebug> const&) noexcept -> bool = default;
