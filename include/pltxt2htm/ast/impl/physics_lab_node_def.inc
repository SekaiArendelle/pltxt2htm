#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::pltxt2htm::container::U8String&& color_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::PlColor<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::PlColor(::pltxt2htm::PlColor<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlColor<ndebug>::~PlColor() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlColor<ndebug>::operator=(this PlColor<ndebug>& self,
                                                       ::pltxt2htm::PlColor<ndebug>&&) noexcept
    -> ::pltxt2htm::PlColor<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlColor<ndebug>::operator==(this PlColor<ndebug> const&,
                                                        ::pltxt2htm::PlColor<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::PlA(::pltxt2htm::PlA<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlA<ndebug>::~PlA() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator=(this PlA<ndebug>& self, ::pltxt2htm::PlA<ndebug>&&) noexcept
    -> ::pltxt2htm::PlA<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlA<ndebug>::operator==(this PlA<ndebug> const&, ::pltxt2htm::PlA<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::PlExperiment(::pltxt2htm::PlExperiment<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiment<ndebug>::~PlExperiment() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator=(this PlExperiment<ndebug>& self,
                                                            ::pltxt2htm::PlExperiment<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExperiment<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiment<ndebug>::operator==(this PlExperiment<ndebug> const&,
                                                             ::pltxt2htm::PlExperiment<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::PlDiscussion(::pltxt2htm::PlDiscussion<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussion<ndebug>::~PlDiscussion() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator=(this PlDiscussion<ndebug>& self,
                                                            ::pltxt2htm::PlDiscussion<ndebug>&&) noexcept
    -> ::pltxt2htm::PlDiscussion<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussion<ndebug>::operator==(this PlDiscussion<ndebug> const&,
                                                             ::pltxt2htm::PlDiscussion<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::Ast<ndebug>&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::PlExperiments<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::PlExperiments(::pltxt2htm::PlExperiments<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExperiments<ndebug>::~PlExperiments() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiments<ndebug>::operator=(this PlExperiments<ndebug>& self,
                                                             ::pltxt2htm::PlExperiments<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExperiments<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExperiments<ndebug>::operator==(this PlExperiments<ndebug> const&,
                                                              ::pltxt2htm::PlExperiments<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::Ast<ndebug>&& subast_,
                                                            ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::PlDiscussions<ndebug> const&) noexcept =
    default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::PlDiscussions(::pltxt2htm::PlDiscussions<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlDiscussions<ndebug>::~PlDiscussions() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussions<ndebug>::operator=(this PlDiscussions<ndebug>& self,
                                                             ::pltxt2htm::PlDiscussions<ndebug>&&) noexcept
    -> ::pltxt2htm::PlDiscussions<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlDiscussions<ndebug>::operator==(this PlDiscussions<ndebug> const&,
                                                              ::pltxt2htm::PlDiscussions<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::Ast<ndebug>&& subast_,
                                              ::pltxt2htm::container::U8String&& id_) noexcept
    : subast(::std::move(subast_)),
      id(::std::move(id_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::PlUser(::pltxt2htm::PlUser<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlUser<ndebug>::~PlUser() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator=(this PlUser<ndebug>& self, ::pltxt2htm::PlUser<ndebug>&&) noexcept
    -> ::pltxt2htm::PlUser<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlUser<ndebug>::operator==(this PlUser<ndebug> const&,
                                                       ::pltxt2htm::PlUser<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::PlTrigger<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::PlTrigger(::pltxt2htm::PlTrigger<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlTrigger<ndebug>::~PlTrigger() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlTrigger<ndebug>::operator=(this PlTrigger<ndebug>& self,
                                                         ::pltxt2htm::PlTrigger<ndebug>&&) noexcept
    -> ::pltxt2htm::PlTrigger<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlTrigger<ndebug>::operator==(this PlTrigger<ndebug> const&,
                                                          ::pltxt2htm::PlTrigger<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::container::U8String&& value_) noexcept
    : subast(::std::move(subast_)),
      value(::std::move(value_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::PlInternal<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::PlInternal(::pltxt2htm::PlInternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlInternal<ndebug>::~PlInternal() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlInternal<ndebug>::operator=(this PlInternal<ndebug>& self,
                                                          ::pltxt2htm::PlInternal<ndebug>&&) noexcept
    -> ::pltxt2htm::PlInternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlInternal<ndebug>::operator==(this PlInternal<ndebug> const&,
                                                           ::pltxt2htm::PlInternal<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::PlExternal(::pltxt2htm::PlExternal<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlExternal<ndebug>::~PlExternal() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator=(this PlExternal<ndebug>& self,
                                                          ::pltxt2htm::PlExternal<ndebug>&&) noexcept
    -> ::pltxt2htm::PlExternal<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlExternal<ndebug>::operator==(this PlExternal<ndebug> const&,
                                                           ::pltxt2htm::PlExternal<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlLink<ndebug>::PlLink(::pltxt2htm::Ast<ndebug>&& subast_, ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlLink<ndebug>::PlLink(::pltxt2htm::PlLink<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlLink<ndebug>::PlLink(::pltxt2htm::PlLink<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlLink<ndebug>::~PlLink() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlLink<ndebug>::operator=(this PlLink<ndebug>& self, ::pltxt2htm::PlLink<ndebug>&&) noexcept
    -> ::pltxt2htm::PlLink<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlLink<ndebug>::operator==(this PlLink<ndebug> const&,
                                                       ::pltxt2htm::PlLink<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::Ast<ndebug>&& subast_,
                                              ::pltxt2htm::ValueWithUnit<double> font_size_) noexcept
    : subast(::std::move(subast_)),
      font_size(font_size_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::PlSize<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::PlSize(::pltxt2htm::PlSize<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlSize<ndebug>::~PlSize() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlSize<ndebug>::operator=(this PlSize<ndebug>& self, ::pltxt2htm::PlSize<ndebug>&&) noexcept
    -> ::pltxt2htm::PlSize<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlSize<ndebug>::operator==(this PlSize<ndebug> const&,
                                                       ::pltxt2htm::PlSize<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlVoffset<ndebug>::PlVoffset(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value_) noexcept
    : subast(::std::move(subast_)),
      value(value_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlVoffset<ndebug>::PlVoffset(::pltxt2htm::PlVoffset<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlVoffset<ndebug>::PlVoffset(::pltxt2htm::PlVoffset<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlVoffset<ndebug>::~PlVoffset() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlVoffset<ndebug>::operator=(this PlVoffset<ndebug>& self,
                                                         ::pltxt2htm::PlVoffset<ndebug>&&) noexcept
    -> ::pltxt2htm::PlVoffset<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlVoffset<ndebug>::operator==(this PlVoffset<ndebug> const&,
                                                          ::pltxt2htm::PlVoffset<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlAlign<ndebug>::PlAlign(::pltxt2htm::Ast<ndebug>&& subast_,
                                                ::pltxt2htm::TextAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlAlign<ndebug>::PlAlign(::pltxt2htm::PlAlign<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlAlign<ndebug>::PlAlign(::pltxt2htm::PlAlign<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlAlign<ndebug>::~PlAlign() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlAlign<ndebug>::operator=(this PlAlign<ndebug>& self,
                                                       ::pltxt2htm::PlAlign<ndebug>&&) noexcept
    -> ::pltxt2htm::PlAlign<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlAlign<ndebug>::operator==(this PlAlign<ndebug> const&,
                                                        ::pltxt2htm::PlAlign<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMark<ndebug>::PlMark(::pltxt2htm::Ast<ndebug>&& subast_,
                                              ::pltxt2htm::container::U8String&& background_color_) noexcept
    : subast(::std::move(subast_)),
      background_color(::std::move(background_color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMark<ndebug>::PlMark(::pltxt2htm::PlMark<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMark<ndebug>::PlMark(::pltxt2htm::PlMark<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMark<ndebug>::~PlMark() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlMark<ndebug>::operator=(this PlMark<ndebug>& self, ::pltxt2htm::PlMark<ndebug>&&) noexcept
    -> ::pltxt2htm::PlMark<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlMark<ndebug>::operator==(this PlMark<ndebug> const&,
                                                       ::pltxt2htm::PlMark<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMargin<ndebug>::PlMargin(
    ::pltxt2htm::Ast<ndebug>&& subast_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
    : subast(::std::move(subast_)),
      left(left_),
      right(right_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMargin<ndebug>::PlMargin(::pltxt2htm::PlMargin<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMargin<ndebug>::PlMargin(::pltxt2htm::PlMargin<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlMargin<ndebug>::~PlMargin() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlMargin<ndebug>::operator=(this PlMargin<ndebug>& self,
                                                        ::pltxt2htm::PlMargin<ndebug>&&) noexcept
    -> ::pltxt2htm::PlMargin<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlMargin<ndebug>::operator==(this PlMargin<ndebug> const&,
                                                         ::pltxt2htm::PlMargin<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::PlI<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::PlI(::pltxt2htm::PlI<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlI<ndebug>::~PlI() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlI<ndebug>::operator=(this PlI<ndebug>& self, ::pltxt2htm::PlI<ndebug>&&) noexcept
    -> ::pltxt2htm::PlI<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlI<ndebug>::operator==(this PlI<ndebug> const&, ::pltxt2htm::PlI<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::PlB<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::PlB(::pltxt2htm::PlB<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::PlB<ndebug>::~PlB() noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlB<ndebug>::operator=(this PlB<ndebug>& self, ::pltxt2htm::PlB<ndebug>&&) noexcept
    -> ::pltxt2htm::PlB<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::PlB<ndebug>::operator==(this PlB<ndebug> const&, ::pltxt2htm::PlB<ndebug> const&) noexcept
    -> bool = default;
