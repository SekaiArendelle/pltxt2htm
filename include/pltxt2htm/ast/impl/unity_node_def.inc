#pragma once

constexpr ::pltxt2htm::UnityColor::UnityColor(::pltxt2htm::Ast&& subast_,
                                                      ::pltxt2htm::container::U8String&& color_) noexcept
    : subast(::std::move(subast_)),
      color(::std::move(color_)) {
}

constexpr ::pltxt2htm::UnityColor::UnityColor(::pltxt2htm::UnityColor&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityColor::operator=(this UnityColor& self,
                                                          ::pltxt2htm::UnityColor&&) noexcept
    -> ::pltxt2htm::UnityColor& = default;
constexpr auto ::pltxt2htm::UnityColor::operator==(this UnityColor const&,
                                                           ::pltxt2htm::UnityColor const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityLink::UnityLink(::pltxt2htm::Ast&& subast_,
                                                    ::pltxt2htm::Url&& url_) noexcept
    : subast(::std::move(subast_)),
      url(::std::move(url_)) {
}

constexpr ::pltxt2htm::UnityLink::UnityLink(::pltxt2htm::UnityLink&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityLink::operator=(this UnityLink& self,
                                                         ::pltxt2htm::UnityLink&&) noexcept
    -> ::pltxt2htm::UnityLink& = default;
constexpr auto ::pltxt2htm::UnityLink::operator==(this UnityLink const&,
                                                          ::pltxt2htm::UnityLink const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnitySize::UnitySize(::pltxt2htm::Ast&& subast_,
                                                    ::pltxt2htm::ValueWithUnit<double> font_size_) noexcept
    : subast(::std::move(subast_)),
      font_size(font_size_) {
}

constexpr ::pltxt2htm::UnitySize::UnitySize(::pltxt2htm::UnitySize&&) noexcept = default;
constexpr auto ::pltxt2htm::UnitySize::operator=(this UnitySize& self,
                                                         ::pltxt2htm::UnitySize&&) noexcept
    -> ::pltxt2htm::UnitySize& = default;
constexpr auto ::pltxt2htm::UnitySize::operator==(this UnitySize const&,
                                                          ::pltxt2htm::UnitySize const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityVoffset::UnityVoffset(::pltxt2htm::Ast&& subast_,
                                                          ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value_) noexcept
    : subast(::std::move(subast_)),
      value(value_) {
}

constexpr ::pltxt2htm::UnityVoffset::UnityVoffset(::pltxt2htm::UnityVoffset&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityVoffset::operator=(this UnityVoffset& self,
                                                            ::pltxt2htm::UnityVoffset&&) noexcept
    -> ::pltxt2htm::UnityVoffset& = default;
constexpr auto ::pltxt2htm::UnityVoffset::operator==(this UnityVoffset const&,
                                                             ::pltxt2htm::UnityVoffset const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityAlign::UnityAlign(::pltxt2htm::Ast&& subast_,
                                                      ::pltxt2htm::TextAlign align_) noexcept
    : subast(::std::move(subast_)),
      align(align_) {
}

constexpr ::pltxt2htm::UnityAlign::UnityAlign(::pltxt2htm::UnityAlign&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityAlign::operator=(this UnityAlign& self,
                                                          ::pltxt2htm::UnityAlign&&) noexcept
    -> ::pltxt2htm::UnityAlign& = default;
constexpr auto ::pltxt2htm::UnityAlign::operator==(this UnityAlign const&,
                                                           ::pltxt2htm::UnityAlign const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityMark::UnityMark(::pltxt2htm::Ast&& subast_,
                                                    ::pltxt2htm::container::U8String&& background_color_) noexcept
    : subast(::std::move(subast_)),
      background_color(::std::move(background_color_)) {
}

constexpr ::pltxt2htm::UnityMark::UnityMark(::pltxt2htm::UnityMark&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityMark::operator=(this UnityMark& self,
                                                         ::pltxt2htm::UnityMark&&) noexcept
    -> ::pltxt2htm::UnityMark& = default;
constexpr auto ::pltxt2htm::UnityMark::operator==(this UnityMark const&,
                                                          ::pltxt2htm::UnityMark const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityMargin::UnityMargin(
    ::pltxt2htm::Ast&& subast_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left_,
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right_) noexcept
    : subast(::std::move(subast_)),
      margins{left_, right_} {
}

constexpr ::pltxt2htm::UnityMargin::UnityMargin(::pltxt2htm::UnityMargin&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityMargin::operator=(this UnityMargin& self,
                                                           ::pltxt2htm::UnityMargin&&) noexcept
    -> ::pltxt2htm::UnityMargin& = default;
constexpr auto ::pltxt2htm::UnityMargin::operator==(this UnityMargin const&,
                                                            ::pltxt2htm::UnityMargin const&) noexcept
    -> bool = default;

constexpr ::pltxt2htm::UnityI::UnityI(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::UnityI::UnityI(::pltxt2htm::UnityI&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityI::operator=(this UnityI& self, ::pltxt2htm::UnityI&&) noexcept
    -> ::pltxt2htm::UnityI& = default;

constexpr auto ::pltxt2htm::UnityI::operator==(this UnityI const&,
                                                       ::pltxt2htm::UnityI const&) noexcept -> bool = default;

constexpr ::pltxt2htm::UnityB::UnityB(::pltxt2htm::Ast&& subast_) noexcept
    : subast(::std::move(subast_)) {
}

constexpr ::pltxt2htm::UnityB::UnityB(::pltxt2htm::UnityB&&) noexcept = default;
constexpr auto ::pltxt2htm::UnityB::operator=(this UnityB& self, ::pltxt2htm::UnityB&&) noexcept
    -> ::pltxt2htm::UnityB& = default;
constexpr auto ::pltxt2htm::UnityB::operator==(this UnityB const&,
                                                       ::pltxt2htm::UnityB const&) noexcept -> bool = default;
