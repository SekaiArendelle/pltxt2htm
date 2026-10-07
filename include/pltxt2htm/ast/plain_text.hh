/**
 * @file plain_text.hh
 * @brief Valid UTF-8 semantic plain text used by AST nodes.
 */

#pragma once

#include "../container/string.hh"
#include "../contracts.hh"
#include "../details/utf8.hh"

namespace pltxt2htm {

/**
 * @brief Parsed plain-text semantics stored as valid UTF-8.
 * @details Source syntax such as HTML character references and Markdown escapes is resolved before storage.
 *          Invalid Unicode scalar values and ASCII controls (U+0000-U+001F and U+007F) become U+FFFD.
 */
template<::pltxt2htm::Contracts ndebug>
class PlainText {
    ::pltxt2htm::container::U8String text;

public:
    constexpr PlainText() noexcept = default;
    constexpr PlainText(PlainText const&) noexcept = default;
    constexpr PlainText(PlainText&&) noexcept = default;
    constexpr ~PlainText() noexcept = default;
    constexpr auto operator=(this PlainText& self, PlainText const&) noexcept -> PlainText& = default;
    constexpr auto operator=(this PlainText& self, PlainText&&) noexcept -> PlainText& = default;

    constexpr void append_code_point(this PlainText& self, char32_t const code_point) noexcept {
        if (::pltxt2htm::details::is_unicode_scalar_value(code_point) == false || code_point <= char32_t{0x1F} ||
            code_point == char32_t{0x7F}) {
            self.append_replacement_character();
            return;
        }
        ::pltxt2htm::details::append_utf8_code_point<ndebug>(self.text, code_point);
    }

    constexpr void append_replacement_character(this PlainText& self) noexcept {
        self.text.template append<ndebug>(u8"\uFFFD");
    }

    [[nodiscard]]
    constexpr auto operator==(this PlainText const&, PlainText const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto as_string(this PlainText const& self) noexcept -> ::pltxt2htm::container::U8String const& {
        return self.text;
    }
};

} // namespace pltxt2htm
