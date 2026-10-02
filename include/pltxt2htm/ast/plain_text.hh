/**
 * @file plain_text.hh
 * @brief Valid UTF-8 semantic plain text used by AST nodes.
 */

#pragma once

#include <utility>

#include "../container/string.hh"
#include "../contracts.hh"
#include "../details/utf8.hh"

namespace pltxt2htm::details {

class PlainTextBuilder;

} // namespace pltxt2htm::details

namespace pltxt2htm {

/**
 * @brief Parsed plain-text semantics stored as valid UTF-8.
 * @details Source syntax such as HTML character references and Markdown escapes is resolved before storage.
 *          Invalid UTF-8 input is represented by U+FFFD.
 */
class PlainText {
    ::pltxt2htm::container::U8String text;

    friend class ::pltxt2htm::details::PlainTextBuilder;

    constexpr explicit PlainText(::pltxt2htm::container::U8String&& text_) noexcept
        : text(::std::move(text_)) {
    }

public:
    constexpr PlainText() noexcept = default;
    constexpr PlainText(PlainText const&) noexcept = default;
    constexpr PlainText(PlainText&&) noexcept = default;
    constexpr ~PlainText() noexcept = default;
    constexpr auto operator=(this PlainText& self, PlainText const&) noexcept -> PlainText& = default;
    constexpr auto operator=(this PlainText& self, PlainText&&) noexcept -> PlainText& = default;

    [[nodiscard]]
    constexpr auto operator==(this PlainText const&, PlainText const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto as_string(this PlainText const& self) noexcept -> ::pltxt2htm::container::U8String const& {
        return self.text;
    }
};

} // namespace pltxt2htm

namespace pltxt2htm::details {

/**
 * @brief Append-only builder that preserves PlainText's valid-UTF-8 invariant.
 */
class PlainTextBuilder {
    ::pltxt2htm::container::U8String text{};

public:
    template<::pltxt2htm::Contracts ndebug>
    constexpr void append_code_point(char32_t const code_point) noexcept {
        if (::pltxt2htm::details::is_unicode_scalar_value(code_point) == false) {
            append_replacement_character<ndebug>();
            return;
        }
        ::pltxt2htm::details::append_utf8_code_point<ndebug>(text, code_point);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void append_replacement_character() noexcept {
        text.template append<ndebug>(u8"\uFFFD");
    }

    [[nodiscard]]
    constexpr auto finish(this PlainTextBuilder&& self) noexcept -> ::pltxt2htm::PlainText {
        return ::pltxt2htm::PlainText{::std::move(self.text)};
    }
};

} // namespace pltxt2htm::details
