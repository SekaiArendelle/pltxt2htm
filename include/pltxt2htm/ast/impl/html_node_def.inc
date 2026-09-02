#pragma once

#include "../../details/push_macro.hh"

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
constexpr ::pltxt2htm::HtmlU<ndebug>::HtmlU(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlU<ndebug>::HtmlU(::pltxt2htm::HtmlU<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlU<ndebug>::HtmlU(::pltxt2htm::HtmlU<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlU<ndebug>::~HtmlU() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlU<ndebug>::operator=(this HtmlU<ndebug>& self, ::pltxt2htm::HtmlU<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlU<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlU<ndebug>::operator==(this HtmlU<ndebug> const&,
                                                      ::pltxt2htm::HtmlU<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlS<ndebug>::HtmlS(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlS<ndebug>::HtmlS(::pltxt2htm::HtmlS<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlS<ndebug>::HtmlS(::pltxt2htm::HtmlS<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlS<ndebug>::~HtmlS() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlS<ndebug>::operator=(this HtmlS<ndebug>& self, ::pltxt2htm::HtmlS<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlS<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlS<ndebug>::operator==(this HtmlS<ndebug> const&,
                                                      ::pltxt2htm::HtmlS<ndebug> const&) noexcept -> bool = default;

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
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::encode_vertical_align_unit(::pltxt2htm::Unit const unit) noexcept
    -> VerticalAlignStorageKind {
    switch (unit) /* -Werror=switch */ {
    case ::pltxt2htm::Unit::px: {
        return VerticalAlignStorageKind::length_px;
    }
    case ::pltxt2htm::Unit::percent: {
        return VerticalAlignStorageKind::length_percent;
    }
    case ::pltxt2htm::Unit::em: {
        return VerticalAlignStorageKind::length_em;
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected vertical-align unit");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable after vertical-align unit switch");
}

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::encode_vertical_align_keyword(
    ::pltxt2htm::VerticalAlignKeyword const keyword) noexcept -> VerticalAlignStorageKind {
    switch (keyword) /* -Werror=switch */ {
    case ::pltxt2htm::VerticalAlignKeyword::baseline: {
        return VerticalAlignStorageKind::keyword_baseline;
    }
    case ::pltxt2htm::VerticalAlignKeyword::sub: {
        return VerticalAlignStorageKind::keyword_sub;
    }
    case ::pltxt2htm::VerticalAlignKeyword::super: {
        return VerticalAlignStorageKind::keyword_super;
    }
    case ::pltxt2htm::VerticalAlignKeyword::text_top: {
        return VerticalAlignStorageKind::keyword_text_top;
    }
    case ::pltxt2htm::VerticalAlignKeyword::text_bottom: {
        return VerticalAlignStorageKind::keyword_text_bottom;
    }
    case ::pltxt2htm::VerticalAlignKeyword::middle: {
        return VerticalAlignStorageKind::keyword_middle;
    }
    case ::pltxt2htm::VerticalAlignKeyword::top: {
        return VerticalAlignStorageKind::keyword_top;
    }
    case ::pltxt2htm::VerticalAlignKeyword::bottom: {
        return VerticalAlignStorageKind::keyword_bottom;
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected vertical-align keyword");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable after vertical-align keyword switch");
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(
    ::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size_,
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)) {
    if (font_size_.has_value()) {
        auto const& font_size = font_size_.template value<ndebug>();
        style.font_size_value = font_size.value;
        style.font_size_unit = font_size.unit;
        style.has_font_size = true;
    }
    if (vertical_align_.has_value() == false) {
        return;
    }
    auto const& vertical_align = vertical_align_.template value<ndebug>();
    if (vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
        style.vertical_align_kind = encode_vertical_align_keyword(vertical_align.get_keyword());
        return;
    }
    auto const length = vertical_align.get_length();
    style.vertical_align_length = length.value;
    style.vertical_align_kind = encode_vertical_align_unit(length.unit);
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
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::get_font_size(this HtmlSpan<ndebug> const& self) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> {
    if (self.style.has_font_size == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::ValueWithUnit<double>{self.style.font_size_value, self.style.font_size_unit};
}

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::get_vertical_align(this HtmlSpan<ndebug> const& self) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> {
    switch (self.style.vertical_align_kind) /* -Werror=switch */ {
    case VerticalAlignStorageKind::none: {
        return ::pltxt2htm::container::nullopt;
    }
    case VerticalAlignStorageKind::keyword_baseline: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::baseline};
    }
    case VerticalAlignStorageKind::keyword_sub: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::sub};
    }
    case VerticalAlignStorageKind::keyword_super: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::super};
    }
    case VerticalAlignStorageKind::keyword_text_top: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::text_top};
    }
    case VerticalAlignStorageKind::keyword_text_bottom: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::text_bottom};
    }
    case VerticalAlignStorageKind::keyword_middle: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::middle};
    }
    case VerticalAlignStorageKind::keyword_top: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::top};
    }
    case VerticalAlignStorageKind::keyword_bottom: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{::pltxt2htm::VerticalAlignKeyword::bottom};
    }
    case VerticalAlignStorageKind::length_px: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{self.style.vertical_align_length, ::pltxt2htm::Unit::px}};
    }
    case VerticalAlignStorageKind::length_percent: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{self.style.vertical_align_length, ::pltxt2htm::Unit::percent}};
    }
    case VerticalAlignStorageKind::length_em: {
        return ::pltxt2htm::VerticalAlignValue<ndebug>{
            ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t>{self.style.vertical_align_length, ::pltxt2htm::Unit::em}};
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected stored vertical-align kind");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable after stored vertical-align kind switch");
}

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
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
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

#include "../../details/pop_macro.hh"
