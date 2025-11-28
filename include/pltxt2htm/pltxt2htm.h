/**
 * @file pltxt2htm.h
 * @brief C-style API interface for pltxt2htm library
 * @details Provides C-compatible function interfaces that return heap-allocated
 *          strings. These functions are suitable for FFI (Foreign Function Interface)
 *          and usage from languages that can interface with C.
 * @warning The returned strings must be freed using std::free() to avoid memory leaks
 */

#pragma once

#include <cstdlib>
#include <cstring>
#include <utility>
#include <concepts>
#include <exception/exception.hh>
#include "pltxt2htm.hh"

namespace pltxt2htm {

namespace details {

/**
 * @brief C-style pointer wrapper for C++ API functions
 * @tparam Func The C++ function to wrap
 * @tparam Args Argument types for the function
 * @param args Arguments to forward to the function
 * @return Heap-allocated UTF-8 string that must be freed by caller
 * @note This function allocates memory on the heap and copies the result string
 * @warning The caller is responsible for freeing the returned pointer using std::free()
 */
template<auto Func, typename... Args>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
#if __has_cpp_attribute(__gnu__::__nonnull__)
[[__gnu__::__nonnull__]]
#endif
constexpr char8_t const* c_ptr_style_wrapper(Args&&... args) noexcept(
    noexcept((Func(::std::forward<Args&&>(args)...)))) {
    auto html = Func(::std::forward<Args&&>(args)...);
    char8_t* result = static_cast<char8_t*>(::std::malloc(html.size() + 1));
    if (result == nullptr) [[unlikely]] {
        // bad alloc error should never be an exception or err-code
        ::exception::terminate();
    }
    ::std::memcpy(result, html.data(), html.size());
    result[html.size()] = u8'\0';
    return result;
}

} // namespace details

/**
 * @brief C-style interface for advanced HTML conversion
 * @details Converts Physics-Lab text to advanced HTML with full feature support.
 *          This is the C wrapper around pltxt2htm::pltxt2advanced_html.
 * @tparam ndebug Debug mode flag - false for debug checks, true for release mode
 * @param text The Physics-Lab text to convert (null-terminated UTF-8 string)
 * @param host Host URL for internal links (null-terminated UTF-8 string)
 * @return Heap-allocated UTF-8 string containing the HTML output
 * @note The returned pointer must be freed using std::free() to avoid memory leaks
 * @warning This function calls std::malloc() and std::memcpy() internally
 * @see pltxt2htm::pltxt2advanced_html for the underlying C++ implementation
 */
template<bool ndebug = false>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
#if __has_cpp_attribute(__gnu__::__nonnull__)
[[__gnu__::__nonnull__]]
#endif
constexpr char8_t const* advanced_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::details::c_ptr_style_wrapper<::pltxt2htm::pltxt2advanced_html<ndebug>>(
        ::fast_io::mnp::os_c_str(text), ::fast_io::mnp::os_c_str(host));
}

/**
 * @brief C-style interface for fixed advanced HTML conversion
 * @details Converts Physics-Lab text to advanced HTML without escaping < characters.
 *          This is the C wrapper around pltxt2htm::pltxt2fixedadv_html.
 * @tparam ndebug Debug mode flag - false for debug checks, true for release mode
 * @param text The Physics-Lab text to convert (null-terminated UTF-8 string)
 * @param host Host URL for internal links (null-terminated UTF-8 string)
 * @return Heap-allocated UTF-8 string containing the HTML output
 * @note The returned pointer must be freed using std::free() to avoid memory leaks
 * @warning The HTML output contains unescaped < characters which may pose security risks
 * @see pltxt2htm::pltxt2fixedadv_html for the underlying C++ implementation
 */
template<bool ndebug = false>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
#if __has_cpp_attribute(__gnu__::__nonnull__)
[[__gnu__::__nonnull__]]
#endif
constexpr char8_t const* fixedadv_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::details::c_ptr_style_wrapper<::pltxt2htm::pltxt2fixedadv_html<ndebug>>(
        ::fast_io::mnp::os_c_str(text), ::fast_io::mnp::os_c_str(host));
}

/**
 * @brief C-style interface for common HTML conversion
 * @details Converts Physics-Lab text to basic HTML with limited formatting support.
 *          This is the C wrapper around pltxt2htm::pltxt2common_html.
 * @tparam ndebug Debug mode flag - false for debug checks, true for release mode
 * @param text The Physics-Lab text to convert (null-terminated UTF-8 string)
 * @return Heap-allocated UTF-8 string containing the HTML output
 * @note The returned pointer must be freed using std::free() to avoid memory leaks
 * @note Only supports basic formatting: color tags, bold, and italic
 * @see pltxt2htm::pltxt2common_html for the underlying C++ implementation
 */
template<bool ndebug = false>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
#if __has_cpp_attribute(__gnu__::__nonnull__)
[[__gnu__::__nonnull__]]
#endif
constexpr char8_t const* common_parser(char8_t const* const text) noexcept {
    return ::pltxt2htm::details::c_ptr_style_wrapper<::pltxt2htm::pltxt2common_html<ndebug>>(
        ::fast_io::mnp::os_c_str(text));
}

} // namespace pltxt2htm
