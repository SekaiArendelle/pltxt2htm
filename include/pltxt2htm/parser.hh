#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "heap_guard.hh"
#include "astnode/basic.hh"
#include "astnode/html.hh"
#include "exception/exception.hh"

namespace pltxt2htm::details {

template<bool ndebug>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
constexpr auto u8string_view_index(::fast_io::u8string_view pltext, ::std::size_t i) noexcept {
    if constexpr (ndebug) {
        return pltext.index_unchecked(i);
    } else {
        return pltext[i];
    }
}

/**
 * @brief Parse pl-text to nodes.
 * @param pltext The text readed from Quantum-Physics.
 */
template<bool ndebug>
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept {
    auto result = ::fast_io::vector<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>{};

    auto const pltxt_size = pltext.size();
    for (::std::size_t i{}; i < pltxt_size; ++i) {
        char8_t const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);

        if (chr == u8'\n') {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Br>{});
        } else if (chr <= 0x1f || (0x7f <= chr && chr <= 0x9f)) {
            // utf-8 control characters will be ignored
            continue;
        } else {
            if ((chr & 0b1000'0000) == 0) {
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
            } else if ((chr & 0b1100'0000) == 0b1100'0000 && (chr & 0b0010'0000) == 0) {
                if (i + 1 >= pltxt_size) [[unlikely]] {
                    // TODO raise utf8_decode_error
                    ::exception::terminate();
                }
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                i += 1;
            } else if ((chr & 0b1110'0000) == 0b1110'0000 && (chr & 0b0001'0000) == 0) {
                if (i + 2 >= pltxt_size) [[unlikely]] {
                    ::exception::terminate();
                }
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2)});
                i += 2;
            } else if ((chr & 0b1111'0000) == 0b1111'0000 && (chr & 0b0000'1000) == 0) {
                if (i + 3 >= pltxt_size) [[unlikely]] {
                    ::exception::terminate();
                }
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2)});
                result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3)});
                i += 3;
            }
        }
    }

    return result;
}

} // namespace pltxt2htm::details
