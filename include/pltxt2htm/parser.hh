#pragma once

#include <memory>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "heap_guard.hh"
#include "astnode/basic.hh"
#include "astnode/html.hh"
#include "exception/exception.hh"
#include "pltxt2htm/astnode/plext.hh"
#if !defined(__wasm__)
#include "panic.hh"
#endif

namespace pltxt2htm {

namespace details {

template<bool ndebug>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr auto u8string_view_index(::fast_io::u8string_view pltext, ::std::size_t i) noexcept {
    if constexpr (ndebug) {
        return pltext.index_unchecked(i);
    } else {
        return pltext[i];
    }
}

/**
 * @brief Cause `parse_pltxt` call itself recursively, I use `ExternSyntaxType`
 *        to mark whether the current call has extern surrounding html, markdown e.t.c syntax or not.
 */
enum class ExternSyntaxType : ::std::uint_least32_t {
    // No embrassing syntax outside
    none = 0,

    // Embrassing syntax is an Quantum-Physics <color> tag
    pl_color,

    // Embrassing syntax is a markdown header
    md_header,
};

/**
 * @brief Parse pl-text to nodes.
 * @tparam ndebug: Whether disables all debug checks.
 * @tparam disable_log: Whether disables all info printed before crashing.
 * @param pltext: The text readed from Quantum-Physics.
 * @param extern_syntax_type: show `enum class ExternSyntaxType`
 * @param extern_index: after parsing the current tag, extern index should be corrected.
 * @return Quantum-Physics text's ast.
 */
template<bool ndebug, bool disable_log>
[[nodiscard]]
constexpr auto parse_pltxt(
    ::fast_io::u8string_view pltext,
    ::pltxt2htm::details::ExternSyntaxType extern_syntax_type = ::pltxt2htm::details::ExternSyntaxType::none,
    ::std::size_t* extern_index = nullptr) noexcept(disable_log == true)
    -> ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> {
#if defined(__wasm__)
    static_assert(disable_log == true, "disable_log must be true when compiling for wasm");
#endif
    if constexpr (ndebug == false || disable_log == false) {
        if ((extern_syntax_type != ::pltxt2htm::details::ExternSyntaxType::none && extern_index == nullptr) ||
            (extern_syntax_type == ::pltxt2htm::details::ExternSyntaxType::none && extern_index != nullptr))
            [[unlikely]] {
            if constexpr (disable_log == false) {
                // panic_print
            }
            if constexpr (ndebug == false) {
                ::exception::terminate();
            }
        }
    }
    auto result = ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>{};

    ::std::size_t const pltxt_size{pltext.size()};
    for (::std::size_t i{}; i < pltxt_size; ++i) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i)};

        if (chr == u8'\n') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>{});
            continue;
        } else if (chr == u8' ') {
            // TODO should we delete tail space?
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Space>{});
            continue;
        } else if (chr == u8'>') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        } else if (chr == u8'<') {
            // if i is a valid value, i always less than pltxt_size
            if constexpr (ndebug == false || disable_log == false) {
                if (i >= pltxt_size) [[unlikely]] {
                    if constexpr (disable_log == false) {
                        // ::pltxt2htm::details::panic_print();
                    }
                    if constexpr (ndebug == false) {
                        ::exception::terminate();
                    }
                }
            }
            if (i + 1 == pltxt_size) {
                goto not_valid_tag;
            }

            // a trie for tags
            switch (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)) {
            case u8'b':
                [[fallthrough]];
            case u8'B': {
                // parsing: <br>, <bR  >, <BR/>, <br  />
                if (i + 3 >= pltxt_size) {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'r' &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'R') {
                    goto not_valid_tag;
                }
                {
                    ::std::size_t forward_index{i + 3};
                    while (true) {
                        char8_t const forward_chr{
                            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
                        if (forward_chr == u8'>') {
                            i = forward_index;
                            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>{});
                            goto complete_parsing_tag;
                        } else if (forward_chr == u8'/' && forward_index + 1 < pltxt_size &&
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) ==
                                       u8'>') {
                            i = forward_index + 1;
                            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>{});
                            goto complete_parsing_tag;
                        } else if (forward_chr != u8' ') {
                            goto not_valid_tag;
                        }
                        if (forward_index + 1 < pltxt_size) {
                            ++forward_index;
                        } else {
                            goto not_valid_tag;
                        }
                    }
                }

                break;
            }

            case u8'c':
                [[fallthrough]];
            case u8'C': {
                // parsing: <color=$1>$2</color>
                if (i + 7 >= pltxt_size) {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'o' &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'O') {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3) != u8'l' &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3) != u8'L') {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 4) != u8'o' &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 4) != u8'O') {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 5) != u8'r' &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 5) != u8'R') {
                    goto not_valid_tag;
                }
                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 6) != u8'=') {
                    goto not_valid_tag;
                }
                {
                    ::std::size_t forward_index{i + 7};
                    ::fast_io::u8string color{};
                    while (true) {
                        char8_t const forward_chr{
                            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
                        if (forward_chr == u8'>') {
                            break;
                        } else if (forward_chr == u8' ') {
                            while (true) {
                                if (forward_index + 1 >= pltxt_size) {
                                    goto not_valid_tag;
                                }

                                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) ==
                                    u8' ') {
                                    ++forward_index;
                                } else if (::pltxt2htm::details::u8string_view_index<ndebug>(
                                               pltext, forward_index + 1) == u8'>') {
                                    ++forward_index;
                                    break;
                                } else {
                                    goto not_valid_tag;
                                }
                            }
                            break;
                        } else {
                            color.push_back(forward_chr);
                        }
                        if (forward_index + 1 >= pltxt_size) {
                            goto not_valid_tag;
                        } else {
                            ++forward_index;
                        }
                    }
                    // parsing start tag <color> successed
                    i = forward_index;
                    if (i + 1 < pltxt_size) {
                        // if forward_index + 1 >= pltxt_size, it means that a not closed tag in the end of the text
                        // which does not make sense, can be opetimized(ignored) during parsing ast
                        auto subast = ::pltxt2htm::details::parse_pltxt<ndebug, disable_log>(
                            pltext.subview(i + 1), ::pltxt2htm::details::ExternSyntaxType::pl_color,
                            ::std::addressof(i));
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Color>(
                            // TODO .subview_unchecked
                            ::std::move(subast), ::std::move(color)));
                    }
                    goto complete_parsing_tag;
                }
                break;
            }

            case u8'/': {
                if (extern_syntax_type == ::pltxt2htm::details::ExternSyntaxType::pl_color) {
                    if (i + 7 >= pltxt_size) {
                        goto not_valid_tag;
                    }
                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'c' &&
                        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2) != u8'C') {
                        goto not_valid_tag;
                    }
                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3) != u8'o' &&
                        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3) != u8'O') {
                        goto not_valid_tag;
                    }
                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 4) != u8'l' &&
                        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 4) != u8'L') {
                        goto not_valid_tag;
                    }
                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 5) != u8'o' &&
                        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 5) != u8'O') {
                        goto not_valid_tag;
                    }
                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 6) != u8'r' &&
                        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 6) != u8'R') {
                        goto not_valid_tag;
                    }
                    {
                        ::std::size_t forward_index{i + 7};
                        while (true) {
                            char8_t const forward_chr{
                                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
                            if (forward_chr == u8'>') {
                                break;
                            } else if (forward_chr == u8' ') {
                                while (true) {
                                    if (forward_index + 1 >= pltxt_size) {
                                        goto not_valid_tag;
                                    }

                                    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) ==
                                        u8' ') {
                                        ++forward_index;
                                    } else if (::pltxt2htm::details::u8string_view_index<ndebug>(
                                                   pltext, forward_index + 1) == u8'>') {
                                        ++forward_index;
                                        break;
                                    } else {
                                        goto not_valid_tag;
                                    }
                                }
                                break;
                            } else {
                                goto not_valid_tag;
                            }
                            if (forward_index + 1 >= pltxt_size) {
                                goto not_valid_tag;
                            } else {
                                ++forward_index;
                            }
                        }
                        // parsing end tag </color> successed
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        if (extern_index != nullptr) {
                            *extern_index += forward_index + 1;
                        }
                        return result;
                    }
                }

                break;
            }

            default:
                goto not_valid_tag;
            }

        not_valid_tag:
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
        complete_parsing_tag:
            continue;
        } else if (chr <= 0x1f || (0x7f <= chr && chr <= 0x9f)) {
            // utf-8 control characters will be ignored
            continue;
        } else {
            // normal utf-8 characters
            if ((chr & 0b1000'0000) == 0) {
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
            } else if ((chr & 0b1100'0000) == 0b1100'0000 && (chr & 0b0010'0000) == 0) {
                if constexpr (ndebug == false || disable_log == false) {
                    // invalid utf-8 encoding
                    if (i + 1 >= pltxt_size) [[unlikely]] {
                        if constexpr (disable_log == false) {
                            // ::fast_io::perrln("Invalid utf-8 encoding");
                        }
                        if constexpr (ndebug == false) {
                            ::exception::terminate();
                        }
                    }
                }
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                i += 1;
            } else if ((chr & 0b1110'0000) == 0b1110'0000 && (chr & 0b0001'0000) == 0) {
                if constexpr (ndebug == false || disable_log == false) {
                    // invalid utf-8 encoding
                    if (i + 2 >= pltxt_size) [[unlikely]] {
                        if constexpr (disable_log == false) {
                            // ::fast_io::perrln("Invalid utf-8 encoding");
                        }
                        if constexpr (ndebug == false) {
                            ::exception::terminate();
                        }
                    }
                }
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2)});
                i += 2;
            } else if ((chr & 0b1111'0000) == 0b1111'0000 && (chr & 0b0000'1000) == 0) {
                if constexpr (ndebug == false || disable_log == false) {
                    // invalid utf-8 encoding
                    if (i + 3 >= pltxt_size) [[unlikely]] {
                        if constexpr (disable_log == false) {
                            // ::fast_io::perrln("Invalid utf-8 encoding");
                        }
                        if constexpr (ndebug == false) {
                            ::exception::terminate();
                        }
                    }
                }
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 1)});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 2)});
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i + 3)});
                i += 3;
            } else [[unlikely]] {
                // invalid utf-8 encoding
                if constexpr (ndebug && disable_log) {
                    ::exception::unreachable<true>();
                } else {
                    if constexpr (ndebug == false) {
                        ::exception::terminate();
                    }
                    if constexpr (disable_log == false) {
                        // ::fast_io::perrln("Invalid utf-8 encoding");
                    }
                }
            }
        }
    }

    if (extern_index != nullptr) {
        *extern_index += pltxt_size;
    }
    return result;
}

} // namespace details

/**
 * @brief Impl of parse pl-text to nodes.
 * @param pltext: The text readed from Quantum-Physics.
 */
template<bool ndebug, bool disable_log>
[[nodiscard]]
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept(disable_log == true) {
#if defined(__wasm__)
    static_assert(disable_log == true, "disable_log must be true when compiling for wasm");
#endif
    return ::pltxt2htm::details::parse_pltxt<ndebug, disable_log>(pltext);
}

} // namespace pltxt2htm
