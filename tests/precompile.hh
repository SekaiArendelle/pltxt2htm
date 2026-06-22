#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm_test {

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt4unittest(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

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
auto pltxt2plunity_introduction(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::fast_io::u8string_view, ::fast_io::u8string_view, ::fast_io::u8string_view,
                                ::fast_io::u8string_view, ::fast_io::u8string_view) noexcept -> ::fast_io::u8string;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::fast_io::u8string_view) noexcept -> ::fast_io::u8string;
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::fast_io::u8string_view pltext, ::fast_io::u8string_view host,
                          ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                          ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors) -> ::fast_io::u8string;

void assert_true(bool) noexcept;

void assert_equal_impl(::fast_io::u8string_view file, ::std::size_t line, ::fast_io::u8string_view html_expr,
                       ::fast_io::u8string_view answer_expr, ::fast_io::u8string_view html,
                       ::fast_io::u8string_view answer);

} // namespace pltxt2htm_test

#define pltxt2htm_test_assert_equal(html, answer) \
    ::pltxt2htm_test::assert_equal_impl(::fast_io::u8string_view{u8"" __FILE__}, __LINE__, \
                                        ::fast_io::u8string_view{u8"" #html}, ::fast_io::u8string_view{u8"" #answer}, \
                                        ::fast_io::mnp::os_c_str(html), ::fast_io::mnp::os_c_str(answer))
