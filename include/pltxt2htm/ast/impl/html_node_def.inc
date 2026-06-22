#pragma once

namespace pltxt2htm {

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
constexpr auto ::pltxt2htm::HtmlH1<ndebug>::operator=(this ::pltxt2htm::HtmlH1<ndebug>& self,
                                                      ::pltxt2htm::HtmlH1<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH1<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH1<ndebug>::operator==(this ::pltxt2htm::HtmlH1<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlH2<ndebug>::operator=(this ::pltxt2htm::HtmlH2<ndebug>& self,
                                                      ::pltxt2htm::HtmlH2<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH2<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH2<ndebug>::operator==(this ::pltxt2htm::HtmlH2<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlH3<ndebug>::operator=(this ::pltxt2htm::HtmlH3<ndebug>& self,
                                                      ::pltxt2htm::HtmlH3<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH3<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH3<ndebug>::operator==(this ::pltxt2htm::HtmlH3<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlH4<ndebug>::operator=(this ::pltxt2htm::HtmlH4<ndebug>& self,
                                                      ::pltxt2htm::HtmlH4<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH4<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH4<ndebug>::operator==(this ::pltxt2htm::HtmlH4<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlH5<ndebug>::operator=(this ::pltxt2htm::HtmlH5<ndebug>& self,
                                                      ::pltxt2htm::HtmlH5<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH5<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH5<ndebug>::operator==(this ::pltxt2htm::HtmlH5<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlH6<ndebug>::operator=(this ::pltxt2htm::HtmlH6<ndebug>& self,
                                                      ::pltxt2htm::HtmlH6<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlH6<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlH6<ndebug>::operator==(this ::pltxt2htm::HtmlH6<ndebug> const&,
                                                       ::pltxt2htm::HtmlH6<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::HtmlP<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::HtmlP(::pltxt2htm::HtmlP<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlP<ndebug>::~HtmlP() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlP<ndebug>::operator=(this ::pltxt2htm::HtmlP<ndebug>& self,
                                                     ::pltxt2htm::HtmlP<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlP<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlP<ndebug>::operator==(this ::pltxt2htm::HtmlP<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlDel<ndebug>::operator=(this ::pltxt2htm::HtmlDel<ndebug>& self,
                                                       ::pltxt2htm::HtmlDel<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlDel<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlDel<ndebug>::operator==(this ::pltxt2htm::HtmlDel<ndebug> const&,
                                                        ::pltxt2htm::HtmlDel<ndebug> const&) noexcept -> bool = default;

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
constexpr auto ::pltxt2htm::HtmlNote<ndebug>::operator=(this ::pltxt2htm::HtmlNote<ndebug>& self,
                                                        ::pltxt2htm::HtmlNote<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlNote<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlNote<ndebug>::operator==(this ::pltxt2htm::HtmlNote<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlEm<ndebug>::operator=(this ::pltxt2htm::HtmlEm<ndebug>& self,
                                                      ::pltxt2htm::HtmlEm<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlEm<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlEm<ndebug>::operator==(this ::pltxt2htm::HtmlEm<ndebug> const&,
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
constexpr auto ::pltxt2htm::HtmlStrong<ndebug>::operator=(this ::pltxt2htm::HtmlStrong<ndebug>& self,
                                                          ::pltxt2htm::HtmlStrong<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlStrong<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlStrong<ndebug>::operator==(this ::pltxt2htm::HtmlStrong<ndebug> const&,
                                                           ::pltxt2htm::HtmlStrong<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlUl<ndebug>::HtmlUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlUl<ndebug>::HtmlUl(::pltxt2htm::HtmlUl<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlUl<ndebug>::HtmlUl(::pltxt2htm::HtmlUl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlUl<ndebug>::~HtmlUl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlUl<ndebug>::operator=(this ::pltxt2htm::HtmlUl<ndebug>& self,
                                                      ::pltxt2htm::HtmlUl<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlUl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlUl<ndebug>::operator==(this ::pltxt2htm::HtmlUl<ndebug> const&,
                                                       ::pltxt2htm::HtmlUl<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlOl<ndebug>::HtmlOl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlOl<ndebug>::HtmlOl(::pltxt2htm::HtmlOl<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlOl<ndebug>::HtmlOl(::pltxt2htm::HtmlOl<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlOl<ndebug>::~HtmlOl() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlOl<ndebug>::operator=(this ::pltxt2htm::HtmlOl<ndebug>& self,
                                                      ::pltxt2htm::HtmlOl<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlOl<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlOl<ndebug>::operator==(this ::pltxt2htm::HtmlOl<ndebug> const&,
                                                       ::pltxt2htm::HtmlOl<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlLi<ndebug>::HtmlLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlLi<ndebug>::HtmlLi(::pltxt2htm::HtmlLi<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlLi<ndebug>::HtmlLi(::pltxt2htm::HtmlLi<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlLi<ndebug>::~HtmlLi() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlLi<ndebug>::operator=(this ::pltxt2htm::HtmlLi<ndebug>& self,
                                                      ::pltxt2htm::HtmlLi<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlLi<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlLi<ndebug>::operator==(this ::pltxt2htm::HtmlLi<ndebug> const&,
                                                       ::pltxt2htm::HtmlLi<ndebug> const&) noexcept -> bool = default;

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
constexpr auto ::pltxt2htm::HtmlCode<ndebug>::operator=(this ::pltxt2htm::HtmlCode<ndebug>& self,
                                                        ::pltxt2htm::HtmlCode<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlCode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlCode<ndebug>::operator==(this ::pltxt2htm::HtmlCode<ndebug> const&,
                                                         ::pltxt2htm::HtmlCode<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlPre<ndebug>::HtmlPre(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlPre<ndebug>::HtmlPre(::pltxt2htm::HtmlPre<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlPre<ndebug>::HtmlPre(::pltxt2htm::HtmlPre<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlPre<ndebug>::~HtmlPre() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlPre<ndebug>::operator=(this ::pltxt2htm::HtmlPre<ndebug>& self,
                                                       ::pltxt2htm::HtmlPre<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlPre<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlPre<ndebug>::operator==(this ::pltxt2htm::HtmlPre<ndebug> const&,
                                                        ::pltxt2htm::HtmlPre<ndebug> const&) noexcept -> bool = default;

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
constexpr auto ::pltxt2htm::HtmlBlockquote<ndebug>::operator=(this ::pltxt2htm::HtmlBlockquote<ndebug>& self,
                                                              ::pltxt2htm::HtmlBlockquote<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlBlockquote<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlBlockquote<ndebug>::operator==(this ::pltxt2htm::HtmlBlockquote<ndebug> const&,
                                                               ::pltxt2htm::HtmlBlockquote<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTable<ndebug>::HtmlTable(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTable<ndebug>::HtmlTable(::pltxt2htm::HtmlTable<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTable<ndebug>::HtmlTable(::pltxt2htm::HtmlTable<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTable<ndebug>::~HtmlTable() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTable<ndebug>::operator=(this ::pltxt2htm::HtmlTable<ndebug>& self,
                                                         ::pltxt2htm::HtmlTable<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTable<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTable<ndebug>::operator==(this ::pltxt2htm::HtmlTable<ndebug> const&,
                                                          ::pltxt2htm::HtmlTable<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTr<ndebug>::HtmlTr(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTr<ndebug>::HtmlTr(::pltxt2htm::HtmlTr<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTr<ndebug>::HtmlTr(::pltxt2htm::HtmlTr<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTr<ndebug>::~HtmlTr() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTr<ndebug>::operator=(this ::pltxt2htm::HtmlTr<ndebug>& self,
                                                      ::pltxt2htm::HtmlTr<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTr<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTr<ndebug>::operator==(this ::pltxt2htm::HtmlTr<ndebug> const&,
                                                       ::pltxt2htm::HtmlTr<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTd<ndebug>::HtmlTd(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTd<ndebug>::HtmlTd(::pltxt2htm::HtmlTd<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTd<ndebug>::HtmlTd(::pltxt2htm::HtmlTd<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTd<ndebug>::~HtmlTd() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTd<ndebug>::operator=(this ::pltxt2htm::HtmlTd<ndebug>& self,
                                                      ::pltxt2htm::HtmlTd<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTd<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTd<ndebug>::operator==(this ::pltxt2htm::HtmlTd<ndebug> const&,
                                                       ::pltxt2htm::HtmlTd<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTh<ndebug>::HtmlTh(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTh<ndebug>::HtmlTh(::pltxt2htm::HtmlTh<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTh<ndebug>::HtmlTh(::pltxt2htm::HtmlTh<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTh<ndebug>::~HtmlTh() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTh<ndebug>::operator=(this ::pltxt2htm::HtmlTh<ndebug>& self,
                                                      ::pltxt2htm::HtmlTh<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTh<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTh<ndebug>::operator==(this ::pltxt2htm::HtmlTh<ndebug> const&,
                                                       ::pltxt2htm::HtmlTh<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlThead<ndebug>::HtmlThead(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlThead<ndebug>::HtmlThead(::pltxt2htm::HtmlThead<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlThead<ndebug>::HtmlThead(::pltxt2htm::HtmlThead<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlThead<ndebug>::~HtmlThead() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlThead<ndebug>::operator=(this ::pltxt2htm::HtmlThead<ndebug>& self,
                                                         ::pltxt2htm::HtmlThead<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlThead<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlThead<ndebug>::operator==(this ::pltxt2htm::HtmlThead<ndebug> const&,
                                                          ::pltxt2htm::HtmlThead<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTbody<ndebug>::HtmlTbody(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTbody<ndebug>::HtmlTbody(::pltxt2htm::HtmlTbody<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTbody<ndebug>::HtmlTbody(::pltxt2htm::HtmlTbody<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTbody<ndebug>::~HtmlTbody() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTbody<ndebug>::operator=(this ::pltxt2htm::HtmlTbody<ndebug>& self,
                                                         ::pltxt2htm::HtmlTbody<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTbody<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTbody<ndebug>::operator==(this ::pltxt2htm::HtmlTbody<ndebug> const&,
                                                          ::pltxt2htm::HtmlTbody<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTfoot<ndebug>::HtmlTfoot(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTfoot<ndebug>::HtmlTfoot(::pltxt2htm::HtmlTfoot<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTfoot<ndebug>::HtmlTfoot(::pltxt2htm::HtmlTfoot<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlTfoot<ndebug>::~HtmlTfoot() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTfoot<ndebug>::operator=(this ::pltxt2htm::HtmlTfoot<ndebug>& self,
                                                         ::pltxt2htm::HtmlTfoot<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlTfoot<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlTfoot<ndebug>::operator==(this ::pltxt2htm::HtmlTfoot<ndebug> const&,
                                                          ::pltxt2htm::HtmlTfoot<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCaption<ndebug>::HtmlCaption(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCaption<ndebug>::HtmlCaption(::pltxt2htm::HtmlCaption<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCaption<ndebug>::HtmlCaption(::pltxt2htm::HtmlCaption<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlCaption<ndebug>::~HtmlCaption() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlCaption<ndebug>::operator=(this ::pltxt2htm::HtmlCaption<ndebug>& self,
                                                           ::pltxt2htm::HtmlCaption<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlCaption<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlCaption<ndebug>::operator==(this ::pltxt2htm::HtmlCaption<ndebug> const&,
                                                            ::pltxt2htm::HtmlCaption<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlColgroup<ndebug>::HtmlColgroup(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlColgroup<ndebug>::HtmlColgroup(::pltxt2htm::HtmlColgroup<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlColgroup<ndebug>::HtmlColgroup(::pltxt2htm::HtmlColgroup<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlColgroup<ndebug>::~HtmlColgroup() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlColgroup<ndebug>::operator=(this ::pltxt2htm::HtmlColgroup<ndebug>& self,
                                                            ::pltxt2htm::HtmlColgroup<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlColgroup<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlColgroup<ndebug>::operator==(this ::pltxt2htm::HtmlColgroup<ndebug> const&,
                                                             ::pltxt2htm::HtmlColgroup<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(::pltxt2htm::Ast<ndebug>&& subast_, ::fast_io::u8string&& color_,
                                                  ::exception::optional<::std::size_t>&& font_size_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)),
      font_size(::std::move(font_size_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(::pltxt2htm::HtmlSpan<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::HtmlSpan(::pltxt2htm::HtmlSpan<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::HtmlSpan<ndebug>::~HtmlSpan() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::operator=(this ::pltxt2htm::HtmlSpan<ndebug>& self,
                                                        ::pltxt2htm::HtmlSpan<ndebug>&&) noexcept
    -> ::pltxt2htm::HtmlSpan<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::HtmlSpan<ndebug>::operator==(this ::pltxt2htm::HtmlSpan<ndebug> const&,
                                                         ::pltxt2htm::HtmlSpan<ndebug> const&) noexcept
    -> bool = default;

} // namespace pltxt2htm
