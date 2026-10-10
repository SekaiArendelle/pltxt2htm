/**
 * @file text_parsing.hh
 * @brief Parse plain text and character references into AST nodes.
 */

#pragma once

#include <cstddef>
#include "../../ast/ast.hh"
#include "../../contracts.hh"
#include "../../container/string_view.hh"
#include "../utf8.hh"
#include "character_reference.hh"
#include "url_parsing.hh"

namespace pltxt2htm::details {

/**
 * @brief Find a leading run that cannot start inline syntax or require character processing.
 * @details The returned ASCII range is safe to append directly to Text nodes. Non-ASCII,
 *          semantic whitespace, HTML-special characters, and inline-syntax introducers stop
 *          the scan and remain handled by the existing bytewise parser path.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]] constexpr auto scan_plain_ascii_run(::pltxt2htm::container::U8StringView text) noexcept -> ::std::size_t {
    auto const text_size = text.size();
    for (::std::size_t index{}; index < text_size; ++index) {
        auto const character = text.template index<ndebug>(index);
        if (character <= u8' ' || character >= char8_t{0x7F}) {
            return index;
        }
        switch (character) {
        case u8'&':
        case u8'\'':
        case u8'"':
        case u8'>':
        case u8'\\':
        case u8'{':
        case u8'*':
        case u8'_':
        case u8'~':
        case u8'`':
        case u8'$':
        case u8'[':
        case u8'!':
        case u8'<': {
            return index;
        }
        case u8'H':
            [[fallthrough]];
        case u8'h': {
            if (::pltxt2htm::details::try_parse_url_scheme<ndebug>(text.template subview<ndebug>(index)).has_value()) {
                return index;
            }
            break;
        }
        default: {
            break;
        }
        }
    }
    return text_size;
}

/**
 * @brief Parse one UTF-8 code point and append its original code units to an AST.
 * @details Parser-disallowed ASCII control characters and invalid UTF-8 prefixes append one
 *          InvalidUtf8 node. The returned size preserves the existing invalid-prefix recovery.
 * @tparam ndebug Contract checking mode used for input and AST access.
 * @param text Non-empty input view beginning at the code point to parse.
 * @param[out] result AST receiving character nodes or one InvalidUtf8 node.
 * @return Number of consumed UTF-8 code units.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_utf8_code_point(::pltxt2htm::container::U8StringView text,
                                     ::pltxt2htm::Ast<ndebug>& result) noexcept -> ::std::size_t {
    char8_t const first{text.template index<ndebug>(0)};
    if (::pltxt2htm::details::is_ascii_control_code_point(static_cast<char32_t>(first))) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::InvalidUtf8>());
        return 1;
    }

    auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<ndebug>(text);
    if (decoded.valid == false) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::InvalidUtf8>());
        return decoded.consumed_size;
    }
    for (::std::size_t index{}; index < decoded.consumed_size; ++index) {
        result.append_text(text.template index<ndebug>(index));
    }
    return decoded.consumed_size;
}

/**
 * @brief Decode and append the character reference at the start of a view.
 * @tparam ndebug Contract checking mode used for parsing and AST operations.
 * @param text Input view expected to begin with a character reference.
 * @param[out] result AST receiving decoded semantic character nodes on success.
 * @return Consumed input size, or nullopt when `text` does not begin with a supported reference.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_append_character_reference(::pltxt2htm::container::U8StringView text,
                                              ::pltxt2htm::Ast<ndebug>& result) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto const reference = ::pltxt2htm::details::try_decode_character_reference<ndebug>(text);
    if (reference.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const& decoded = reference.template value<ndebug>();
    result.append_code_point(decoded.first_code_point);
    if (decoded.has_second_code_point()) {
        result.append_code_point(decoded.second_code_point);
    }
    return decoded.consumed_size;
}

} // namespace pltxt2htm::details
