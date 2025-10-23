/**
 * @file precompile.cpp
 * @brief To improve build performance, we precompile the pltxt2htm.cpp file as static library
 */

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include <pltxt2htm/pltxt2htm.hh>

namespace pltxt2htm_test {

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
::fast_io::u8string pltxt2common_htmld(::fast_io::u8string_view pltext) noexcept {
    return ::pltxt2htm::pltxt2common_html<false>(pltext);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
::fast_io::u8string pltxt2common_html(::fast_io::u8string_view pltext) noexcept {
    return ::pltxt2htm::pltxt2common_html<false, true>(pltext);
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
::fast_io::u8string pltxt2advanced_htmld(::fast_io::u8string_view pltext) noexcept {
    return ::pltxt2htm::pltxt2advanced_html<false>(pltext, u8"localhost:5173");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
::fast_io::u8string pltxt2fixedadv_htmld(::fast_io::u8string_view pltext) noexcept {
    return ::pltxt2htm::pltxt2fixedadv_html<false>(pltext, u8"localhost:5173");
}

#if __has_cpp_attribute(__gnu__::__used__)
[[__gnu__::__used__]]
#endif
#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
void assert_true(bool cond) noexcept {
    ::exception::assert_true<false>(cond);
}

} // namespace pltxt2htm_test
