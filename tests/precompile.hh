#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm_test {

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2advanced_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_html(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto to_plunity_introduction(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

void assert_true(bool) noexcept;

} // namespace pltxt2htm_test
