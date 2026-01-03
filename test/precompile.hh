#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm_test {

[[nodiscard]] [[__gnu__::__pure__]]
auto pltxt2advanced_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]] [[__gnu__::__pure__]]
auto pltxt2common_html(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]] [[__gnu__::__pure__]]
auto pltxt2common_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]] [[__gnu__::__pure__]]
auto pltxt2fixedadv_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

void assert_true(bool) noexcept;

} // namespace pltxt2htm_test
