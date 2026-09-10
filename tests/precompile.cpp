/**
 * @file precompile.cpp
 * @brief To improve build performance, we precompile the pltxt2htm.cpp file as static library
 */

#ifdef NDEBUG
    #error "Test fail: NDEBUG is defined"
#endif

#ifndef PLTXT2HTM_ENABLE_CONTEXT_BRANCH_CHECK
    #error "Test fail: PLTXT2HTM_ENABLE_CONTEXT_BRANCH_CHECK is not defined"
#endif

#include <cstdint>
#include <cstdlib>
#include <fast_io/fast_io.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <pltxt2htm/pltxt2htm.hh>
#include <pltxt2htm/experimental/html_parser.hh>

#if defined(pltxt2htm_assert)
    #error "Test fail: pltxt2htm_assert is defined"
#endif

#if defined(pltxt2htm_assume)
    #error "Test fail: pltxt2htm_assume is defined"
#endif

#if defined(pltxt2htm_assert_context_branch)
    #error "Test fail: pltxt2htm_assert_context_branch is defined"
#endif

#if defined __GNUC__ || defined __clang__
    #if __has_attribute(visibility)
        #define PLTXT2HTM_VISIBILITY_DEFAULT __attribute__((visibility("default")))
    #else
        #define PLTXT2HTM_VISIBILITY_DEFAULT
    #endif
#else
    #define PLTXT2HTM_VISIBILITY_DEFAULT
#endif

namespace pltxt2htm_test {

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2common_htmld(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2common_html<::pltxt2htm::Contracts::quick_enforce>(pltext);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2common_html(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2common_html<::pltxt2htm::Contracts::quick_enforce, true>(pltext);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt4unittest(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt4unittest<::pltxt2htm::Contracts::quick_enforce>(pltext);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt4htmlunittest(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    auto ast = ::pltxt2htm::experimental::parse_pltxt_html<::pltxt2htm::Contracts::quick_enforce>(pltext);
    return ::pltxt2htm::details::plweb_text_backend<::pltxt2htm::Contracts::quick_enforce,
                                                    ::pltxt2htm::details::PlWebTextBackendMode::pltxt4unittest>(
        ast, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2roundtrip_htmld(::fast_io::u8string_view pltext) noexcept
    -> ::fast_io::u8string {
    auto ast = ::pltxt2htm::parse_pltxt<::pltxt2htm::Contracts::quick_enforce>(pltext);
    ::pltxt2htm::optimize_ast<::pltxt2htm::Contracts::quick_enforce>(ast);
    return ::pltxt2htm::details::plweb_text_backend<::pltxt2htm::Contracts::quick_enforce,
                                                    ::pltxt2htm::details::PlWebTextBackendMode::roundtrip>(
        ast, u8"_", u8"_", u8"_", u8"_", u8"_");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2fixedadv_htmld(::fast_io::u8string_view pltext) noexcept
    -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(
        pltext, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2fixedadv_htmld(::fast_io::u8string_view pltext, ::fast_io::u8string_view host,
                                                       ::fast_io::u8string_view project,
                                                       ::fast_io::u8string_view visitor,
                                                       ::fast_io::u8string_view author,
                                                       ::fast_io::u8string_view coauthors) noexcept
    -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(pltext, host, project, visitor,
                                                                                   author, coauthors);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2plunity_introduction(::fast_io::u8string_view pltext) noexcept
    -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2plunity_introduction<::pltxt2htm::Contracts::quick_enforce>(
        pltext, u8"$PROJECT", u8"$VISITOR", u8"$AUTHOR", u8"$CO_AUTHORS");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT auto pltxt2plunity_introduction(::fast_io::u8string_view pltext,
                                                             ::fast_io::u8string_view project,
                                                             ::fast_io::u8string_view visitor,
                                                             ::fast_io::u8string_view author,
                                                             ::fast_io::u8string_view coauthors) noexcept
    -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2plunity_introduction<::pltxt2htm::Contracts::quick_enforce>(pltext, project, visitor,
                                                                                          author, coauthors);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT void assert_true_impl(::fast_io::u8string_view file, ::std::size_t line,
                                                   ::fast_io::u8string_view expr, bool cond) noexcept {
    if (!cond) [[unlikely]] {
        ::fast_io::io::perr(::fast_io::u8err(), u8"unittest failed: `", expr, u8"` is false\n  at ", file, u8":",
                            static_cast<::std::size_t>(line), u8"\n");
        ::std::_Exit(EXIT_FAILURE);
    }
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT void assert_equal_impl(::fast_io::u8string_view file, ::std::size_t line,
                                                    ::fast_io::u8string_view html_expr,
                                                    ::fast_io::u8string_view answer_expr, ::fast_io::u8string_view html,
                                                    ::fast_io::u8string_view answer) noexcept {
    if (html != answer) [[unlikely]] {
        ::fast_io::io::perr(::fast_io::u8err(), u8"unittest failed due to `", html_expr, u8" != ", answer_expr,
                            u8"`\n  at ", file, u8":", static_cast<::std::size_t>(line), u8"\n  ", html_expr, u8": ",
                            ::fast_io::u8string_view{::std::data(html), ::std::size(html)}, u8"\n  ", answer_expr,
                            u8": ", ::fast_io::u8string_view{::std::data(answer), ::std::size(answer)}, u8"\n");
        ::std::_Exit(EXIT_FAILURE);
    }
}

} // namespace pltxt2htm_test
