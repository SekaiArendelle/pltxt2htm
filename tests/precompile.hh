#pragma once

#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/container/string.hh>

namespace pltxt2htm_test {

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt4unittest(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt4htmlunittest(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_html(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_htmld(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::fast_io::u8string_view, ::fast_io::u8string_view, ::fast_io::u8string_view,
                                ::fast_io::u8string_view, ::fast_io::u8string_view) noexcept
    -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::fast_io::u8string_view pltext, ::fast_io::u8string_view host,
                          ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                          ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors)
    -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2roundtrip_htmld(::fast_io::u8string_view) noexcept -> ::pltxt2htm::container::U8String;

void assert_true_impl(::fast_io::u8string_view file, ::std::size_t line, ::fast_io::u8string_view expr,
                      bool cond) noexcept;

/**
 * @brief Views an assertion operand as a UTF-8 view.
 *
 * The operand can be an owning string (frequently a temporary), a literal, or an existing view.
 * Binding it as `T const&` keeps rvalue containers usable, whereas constructing the view from an
 * `U8String&&` is deliberately disabled to prevent dangling views.
 * @tparam T Operand type
 * @param[in] value Operand to view
 * @return A view over the operand, valid for the enclosing full expression
 */
template<typename T>
[[nodiscard]] constexpr auto to_u8string_view(T const& value) noexcept -> ::fast_io::u8string_view {
    return ::pltxt2htm::container::U8StringView{value};
}

void assert_equal_impl(::fast_io::u8string_view file, ::std::size_t line, ::fast_io::u8string_view html_expr,
                       ::fast_io::u8string_view answer_expr, ::fast_io::u8string_view html,
                       ::fast_io::u8string_view answer);

} // namespace pltxt2htm_test

#define pltxt2htm_test_assert_equal(html, answer) \
    ::pltxt2htm_test::assert_equal_impl(::fast_io::u8string_view{u8"" __FILE__}, __LINE__, \
                                        ::fast_io::u8string_view{u8"" #html}, ::fast_io::u8string_view{u8"" #answer}, \
                                        ::pltxt2htm_test::to_u8string_view(html), \
                                        ::pltxt2htm_test::to_u8string_view(answer))

#define pltxt2htm_test_assert_true(...) \
    ::pltxt2htm_test::assert_true_impl(::fast_io::u8string_view{u8"" __FILE__}, __LINE__, \
                                       ::fast_io::u8string_view{u8"" #__VA_ARGS__}, (__VA_ARGS__))

#define pltxt2htm_test_assert_false(...) \
    ::pltxt2htm_test::assert_true_impl(::fast_io::u8string_view{u8"" __FILE__}, __LINE__, \
                                       ::fast_io::u8string_view{u8"!(" #__VA_ARGS__ ")"}, !(__VA_ARGS__))
