/**
 * @file ascii.hh
 * @brief ASCII character classification helpers.
 */

#pragma once

namespace pltxt2htm::details {

/**
 * @brief Check if a character is an ASCII letter.
 * @param chr The character to test.
 * @return true if chr is in [a-zA-Z].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_alpha(char8_t const chr) noexcept {
    return (u8'a' <= chr && chr <= u8'z') || (u8'A' <= chr && chr <= u8'Z');
}

/**
 * @brief Check if a character is an ASCII digit.
 * @param chr The character to test.
 * @return true if chr is in [0-9].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_digit(char8_t const chr) noexcept {
    return u8'0' <= chr && chr <= u8'9';
}

/**
 * @brief Check if a character is an ASCII hexadecimal digit.
 * @param chr The character to test.
 * @return true if chr is in [0-9a-fA-F].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_hexdigit(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_digit(chr) || (u8'a' <= chr && chr <= u8'f') ||
           (u8'A' <= chr && chr <= u8'F');
}

/**
 * @brief Check if a character is printable ASCII excluding space.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_graphic(char8_t const chr) noexcept {
    return u8'!' <= chr && chr <= u8'~';
}

/**
 * @brief Check if a character is ASCII punctuation.
 * @details This is the character set CommonMark permits after a backslash escape.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_punctuation(char8_t const chr) noexcept {
    return (u8'!' <= chr && chr <= u8'/') || (u8':' <= chr && chr <= u8'@') || (u8'[' <= chr && chr <= u8'`') ||
           (u8'{' <= chr && chr <= u8'~');
}

/**
 * @brief Check if a character is allowed in a URL-valued tag attribute.
 * @details Printable ASCII plus any non-ASCII byte (e.g. UTF-8 CJK); non-ASCII bytes are
 *          percent-encoded later by Url::try_make.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_url_value_char(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_graphic(chr) || chr >= char8_t(0x80);
}

} // namespace pltxt2htm::details
