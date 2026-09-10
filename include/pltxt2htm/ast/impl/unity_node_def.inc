#pragma once

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityColor<ndebug>::UnityColor(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::fast_io::u8string&& color_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityColor<ndebug>::UnityColor(::pltxt2htm::UnityColor<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityColor<ndebug>::UnityColor(::pltxt2htm::UnityColor<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityColor<ndebug>::operator=(this UnityColor<ndebug>& self,
                                                          ::pltxt2htm::UnityColor<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityColor<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityColor<ndebug>::operator==(this UnityColor<ndebug> const&,
                                                           ::pltxt2htm::UnityColor<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityLink<ndebug>::UnityLink(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityLink<ndebug>::UnityLink(::pltxt2htm::UnityLink<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityLink<ndebug>::UnityLink(::pltxt2htm::UnityLink<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityLink<ndebug>::operator=(this UnityLink<ndebug>& self,
                                                         ::pltxt2htm::UnityLink<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityLink<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityLink<ndebug>::operator==(this UnityLink<ndebug> const&,
                                                          ::pltxt2htm::UnityLink<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnitySize<ndebug>::UnitySize(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::pltxt2htm::ValueWithUnit<double> font_size_) noexcept
    : subast(::std::move(subast_)),
      font_size(font_size_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnitySize<ndebug>::UnitySize(::pltxt2htm::UnitySize<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnitySize<ndebug>::UnitySize(::pltxt2htm::UnitySize<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnitySize<ndebug>::operator=(this UnitySize<ndebug>& self,
                                                         ::pltxt2htm::UnitySize<ndebug>&&) noexcept
    -> ::pltxt2htm::UnitySize<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnitySize<ndebug>::operator==(this UnitySize<ndebug> const&,
                                                          ::pltxt2htm::UnitySize<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityVoffset<ndebug>::UnityVoffset(::pltxt2htm::Ast<ndebug>&& subast_,
                                                          ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value_) noexcept
    : subast(::std::move(subast_)),
      value(value_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityVoffset<ndebug>::UnityVoffset(::pltxt2htm::UnityVoffset<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityVoffset<ndebug>::UnityVoffset(::pltxt2htm::UnityVoffset<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityVoffset<ndebug>::operator=(this UnityVoffset<ndebug>& self,
                                                            ::pltxt2htm::UnityVoffset<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityVoffset<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityVoffset<ndebug>::operator==(this UnityVoffset<ndebug> const&,
                                                             ::pltxt2htm::UnityVoffset<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityAlign<ndebug>::UnityAlign(::pltxt2htm::Ast<ndebug>&& subast_,
                                                      ::pltxt2htm::TextAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityAlign<ndebug>::UnityAlign(::pltxt2htm::UnityAlign<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityAlign<ndebug>::UnityAlign(::pltxt2htm::UnityAlign<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityAlign<ndebug>::operator=(this UnityAlign<ndebug>& self,
                                                          ::pltxt2htm::UnityAlign<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityAlign<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityAlign<ndebug>::operator==(this UnityAlign<ndebug> const&,
                                                           ::pltxt2htm::UnityAlign<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMark<ndebug>::UnityMark(::pltxt2htm::Ast<ndebug>&& subast_,
                                                    ::fast_io::u8string&& background_color_) noexcept
    : subast(::std::move(subast_)),
      background_color(::std::move(background_color_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMark<ndebug>::UnityMark(::pltxt2htm::UnityMark<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMark<ndebug>::UnityMark(::pltxt2htm::UnityMark<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityMark<ndebug>::operator=(this UnityMark<ndebug>& self,
                                                         ::pltxt2htm::UnityMark<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityMark<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityMark<ndebug>::operator==(this UnityMark<ndebug> const&,
                                                          ::pltxt2htm::UnityMark<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMargin<ndebug>::UnityMargin(
    ::pltxt2htm::Ast<ndebug>&& subast_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
    : subast(::std::move(subast_)),
      left(left_),
      right(right_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMargin<ndebug>::UnityMargin(::pltxt2htm::UnityMargin<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityMargin<ndebug>::UnityMargin(::pltxt2htm::UnityMargin<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityMargin<ndebug>::operator=(this UnityMargin<ndebug>& self,
                                                           ::pltxt2htm::UnityMargin<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityMargin<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityMargin<ndebug>::operator==(this UnityMargin<ndebug> const&,
                                                            ::pltxt2htm::UnityMargin<ndebug> const&) noexcept
    -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityI<ndebug>::UnityI(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityI<ndebug>::UnityI(::pltxt2htm::UnityI<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityI<ndebug>::UnityI(::pltxt2htm::UnityI<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityI<ndebug>::operator=(this UnityI<ndebug>& self, ::pltxt2htm::UnityI<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityI<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityI<ndebug>::operator==(this UnityI<ndebug> const&,
                                                       ::pltxt2htm::UnityI<ndebug> const&) noexcept -> bool = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityB<ndebug>::UnityB(::pltxt2htm::Ast<ndebug>&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityB<ndebug>::UnityB(::pltxt2htm::UnityB<ndebug> const&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr ::pltxt2htm::UnityB<ndebug>::UnityB(::pltxt2htm::UnityB<ndebug>&&) noexcept = default;
template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityB<ndebug>::operator=(this UnityB<ndebug>& self, ::pltxt2htm::UnityB<ndebug>&&) noexcept
    -> ::pltxt2htm::UnityB<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ::pltxt2htm::UnityB<ndebug>::operator==(this UnityB<ndebug> const&,
                                                       ::pltxt2htm::UnityB<ndebug> const&) noexcept -> bool = default;
