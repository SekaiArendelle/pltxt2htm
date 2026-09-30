#pragma once

#include <pltxt2htm/container/string.hh>

namespace pltxt2htm_test {

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt4unittest(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt4htmlunittest(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_html(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2common_htmld(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::pltxt2htm::container::U8StringView) noexcept
    -> ::pltxt2htm::container::U8String;
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2plunity_introduction(::pltxt2htm::container::U8StringView,
                                ::pltxt2htm::container::U8StringView,
                                ::pltxt2htm::container::U8StringView,
                                ::pltxt2htm::container::U8StringView,
                                ::pltxt2htm::container::U8StringView) noexcept
    -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2fixedadv_htmld(::pltxt2htm::container::U8StringView pltext,
                          ::pltxt2htm::container::U8StringView host,
                          ::pltxt2htm::container::U8StringView project,
                          ::pltxt2htm::container::U8StringView visitor,
                          ::pltxt2htm::container::U8StringView author,
                          ::pltxt2htm::container::U8StringView coauthors)
    -> ::pltxt2htm::container::U8String;

[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
auto pltxt2roundtrip_htmld(::pltxt2htm::container::U8StringView) noexcept -> ::pltxt2htm::container::U8String;

void assert_true_impl(::pltxt2htm::container::U8StringView file, ::std::size_t line,
                      ::pltxt2htm::container::U8StringView expr, bool cond) noexcept;

void assert_equal_impl(::pltxt2htm::container::U8StringView file, ::std::size_t line,
                       ::pltxt2htm::container::U8StringView html_expr,
                       ::pltxt2htm::container::U8StringView answer_expr,
                       ::pltxt2htm::container::U8StringView html,
                       ::pltxt2htm::container::U8StringView answer);

} // namespace pltxt2htm_test

#define pltxt2htm_test_assert_equal(html, answer) \
    do { \
        auto const& pltxt2htm_test_html_ref = html; \
        auto const& pltxt2htm_test_answer_ref = answer; \
        ::pltxt2htm_test::assert_equal_impl(::pltxt2htm::container::U8StringView{u8"" __FILE__}, __LINE__, \
                                            ::pltxt2htm::container::U8StringView{u8"" #html}, \
                                            ::pltxt2htm::container::U8StringView{u8"" #answer}, \
                                            pltxt2htm_test_html_ref, pltxt2htm_test_answer_ref); \
    } while (0)

#define pltxt2htm_test_assert_true(...) \
    ::pltxt2htm_test::assert_true_impl(::pltxt2htm::container::U8StringView{u8"" __FILE__}, __LINE__, \
                                       ::pltxt2htm::container::U8StringView{u8"" #__VA_ARGS__}, (__VA_ARGS__))

#define pltxt2htm_test_assert_false(...) \
    ::pltxt2htm_test::assert_true_impl(::pltxt2htm::container::U8StringView{u8"" __FILE__}, __LINE__, \
                                       ::pltxt2htm::container::U8StringView{u8"!(" #__VA_ARGS__ ")"}, !(__VA_ARGS__))
