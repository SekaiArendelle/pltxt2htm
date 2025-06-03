#pragma once

#include <cstdlib>
#include <cstring>
#include <concepts>
#include <exception/exception.hh>
#include "pltxt2htm.hh"

/**
 * @brief C-Pointer-Style interface for C++ API pltxt2htm::pltxt2html
 * @note Don't forget to free the returned pointer
 */
template<bool ndebug = false, bool disable_log = true>
[[nodiscard]]
constexpr char8_t const* c_pltxt2html(char8_t const* const text, bool is_inline = false) noexcept(disable_log == true) {
#if defined(__wasm__)
    static_assert(disable_log == true, "disable_log must be true when compiling for wasm");
#endif
    auto html = ::pltxt2htm::pltxt2html<ndebug, disable_log>(::fast_io::mnp::os_c_str(text), is_inline);
    char8_t* result = reinterpret_cast<char8_t*>(::std::malloc(html.size() + 1));
    if (result == nullptr) [[unlikely]] {
        // bad alloc error should never be an exception or err-code
        ::exception::terminate();
    }
    ::std::memcpy(result, html.data(), html.size());
    result[html.size()] = u8'\0';
    return result;
}
