#pragma once

#include <memory>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "astnode/basic.hh"
#include "astnode/html.hh"

namespace pltxt2htm::details {

/**
 * @brief Parse pl-text to nodes.
 * @param pltext The text readed from Quantum-Physics.
 */
template<bool ndebug>
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept {
    // TODO rewrite my own unique_ptr, without bad_alloc exception and nullptr is not allowed
    auto result = ::fast_io::vector<::std::unique_ptr<::pltxt2htm::PlTxtNode>>{};

    auto const pltxt_size  = pltext.size();
    for (::std::size_t i{}; i < pltxt_size; ++i) {
#if __has_cpp_attribute(indeterminate)
        // https://en.cppreference.com/w/cpp/language/attributes/indeterminate
        char8_t chr [[indeterminate]];
#else
        char8_t chr;
#endif
        if constexpr (ndebug) {
            chr = pltext.index_unchecked(i);
        } else {
            chr = pltext[i];
        }

#ifdef _WIN32
        // On Windows, the newline is \r\n
        if (chr == u8'\r')
#else
        // On Unix like OS, the newline is \n
        if (chr == u8'\n')
#endif
        {
            // If the current char is \r and the next char is not \n, ignore this rare case
            if (i == pltxt_size - 1) [[unlikely]] {
                // If the last char is \r, ignore it
                // ... [\r] [EOF]
                //      ^
                //     chr
                break;
            }
#if __has_cpp_attribute(indeterminate)
            // https://en.cppreference.com/w/cpp/language/attributes/indeterminate
            char8_t next_chr [[indeterminate]];
#else
            char8_t next_chr;
#endif
            if constexpr (ndebug) {
                next_chr = pltext.index_unchecked(i + 1);
            } else {
                next_chr = pltext[i + 1];
            }
            if (next_chr != u8'\n') [[unlikely]] {
                // If the next char is not \n, ignore this rare case
                // ... [\r] [Other char but not \n] ...
                //      ^             ^
                //     chr        next_chr
                continue;
            }

            result.push_back(::std::make_unique<::pltxt2htm::Br>());
#ifdef _WIN32
            // On Windows, the newline is \r\n
            ++i;
#endif
        } else {
            // TODO 
            result.push_back(::std::make_unique<::pltxt2htm::U8Char>(chr));
        }
    }

    return result;
}

} // namespace pltxt2htm::details
