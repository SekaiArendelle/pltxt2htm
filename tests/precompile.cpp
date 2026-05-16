/**
 * @file precompile.cpp
 * @brief To improve build performance, we precompile the pltxt2htm.cpp file as static library
 */

#include <fast_io/fast_io.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

#if defined(pltxt2htm_assert)
    #error "Test fail: pltxt2htm_assert is defined"
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
PLTXT2HTM_VISIBILITY_DEFAULT void assert_true(bool cond) noexcept {
    if (!cond) {
        ::exception::terminate();
    }
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
PLTXT2HTM_VISIBILITY_DEFAULT void assert_equal_impl(::fast_io::u8string_view file, ::std::size_t line,
                                                    ::fast_io::u8string_view html_expr,
                                                    ::fast_io::u8string_view answer_expr, ::fast_io::u8string_view html,
                                                    ::fast_io::u8string_view answer) noexcept {
    if (html != answer) {
        ::fast_io::io::perr(::fast_io::u8err(), u8"unittest failed due to `", html_expr, u8" != ", answer_expr,
                            u8"`\n  at ", file, u8":", static_cast<unsigned long long>(line), u8"\n  ", html_expr,
                            u8": ", ::fast_io::u8string_view{::std::data(html), ::std::size(html)}, u8"\n  ",
                            answer_expr, u8": ", ::fast_io::u8string_view{::std::data(answer), ::std::size(answer)},
                            u8"\n");
        ::exception::terminate();
    }
}

} // namespace pltxt2htm_test
