#pragma once

// proot reports a non-zero TracerPid, so doctest concludes that a debugger is attached
// and breaks on the first failed assertion. That kills the process before the buffered
// report reaches the terminal (exit 133, no output at all), which is far more costly
// than not dropping into a debugger. Remove this if a real debugger is ever attached.
#define DOCTEST_BREAK_INTO_DEBUGGER() ((void)0)

#include <doctest/doctest.h>

#include <string>

#include "precompile.hh"

namespace pltxt2htm_test::details {

/**
 * Render a UTF-8 byte range for a doctest failure message.
 *
 * doctest prints a value it has no stringifier for as `{?}`, which makes a failing
 * `CHECK(html == answer)` unreadable. Backslashes, control bytes and the DEL byte are
 * escaped so that the message keeps its shape and the exact bytes stay unambiguous.
 * Printable bytes, including multi-byte UTF-8, are passed through unchanged, because the
 * expected values are mostly readable text.
 *
 * @param data First byte to render.
 * @param size Number of bytes to render.
 */
[[nodiscard]] inline auto escape_for_message(char8_t const* data, ::std::size_t size) -> ::doctest::String {
    static constexpr char8_t hex_digits[]{u8"0123456789abcdef"};

    ::std::string escaped{};
    escaped.reserve(size);

    for (::std::size_t index{}; index < size; ++index) {
        unsigned char const byte{static_cast<unsigned char>(data[index])};
        switch (byte) {
        case '\\':
            escaped += "\\\\";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            if (byte < 0x20 || byte == 0x7F) {
                escaped += "\\x";
                escaped += static_cast<char>(hex_digits[byte >> 4]);
                escaped += static_cast<char>(hex_digits[byte & 0x0F]);
            }
            else {
                escaped += static_cast<char>(byte);
            }
            break;
        }
    }

    return ::doctest::String{escaped.data(), static_cast<::doctest::String::size_type>(escaped.size())};
}

[[nodiscard]] inline auto escape_for_message(::fast_io::u8string_view value) -> ::doctest::String {
    return escape_for_message(value.data(), value.size());
}

[[nodiscard]] inline auto escape_for_message(::pltxt2htm::container::U8String const& value) -> ::doctest::String {
    return escape_for_message(value.data(), value.size());
}

} // namespace pltxt2htm_test::details

namespace doctest {

// Without these, every failing `CHECK(html == answer)` in the syntax cases reports
// `values: CHECK( {?} == {?} )` instead of the two values that differ.
template<>
struct StringMaker<::fast_io::u8string_view> {
    [[nodiscard]] static auto convert(::fast_io::u8string_view const& value) -> ::doctest::String {
        return ::pltxt2htm_test::details::escape_for_message(value);
    }
};

template<>
struct StringMaker<::pltxt2htm::container::U8String> {
    [[nodiscard]] static auto convert(::pltxt2htm::container::U8String const& value) -> ::doctest::String {
        return ::pltxt2htm_test::details::escape_for_message(value);
    }
};

// A few cases compare against a `u8` literal directly instead of a named answer variable.
// The comparison helpers capture such a literal both as `char8_t[n]` and as
// `char8_t const[n]`, so both spellings need a stringifier.
template<::std::size_t n>
struct StringMaker<char8_t[n]> {
    [[nodiscard]] static auto convert(char8_t const (&value)[n]) -> ::doctest::String {
        return ::pltxt2htm_test::details::escape_for_message(value, n - 1);
    }
};

template<::std::size_t n>
struct StringMaker<char8_t const[n]> {
    [[nodiscard]] static auto convert(char8_t const (&value)[n]) -> ::doctest::String {
        return ::pltxt2htm_test::details::escape_for_message(value, n - 1);
    }
};

} // namespace doctest