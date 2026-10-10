/**
 * @file character_processing.hh
 * @brief Process UTF-8 code points, AST characters, and HTML character references.
 */

#pragma once

#include <cstddef>
#include "../../ast/ast.hh"
#include "../../contracts.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../utf8.hh"
#include "character_reference.hh"
#include "html_named_character_references.hh"
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
 * @brief Test whether a code point is an ASCII control character.
 * @param code_point Code point to test.
 * @return `true` for U+0000 through U+001F and U+007F.
 */
[[nodiscard]]
constexpr auto is_ascii_control_code_point(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x1F} || code_point == char32_t{0x7F};
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
 * @brief Append one semantic Unicode code point to an AST.
 * @details Characters with dedicated semantic nodes use those nodes. Other scalar values are
 *          encoded as UTF-8 and appended to Text nodes. ASCII controls and invalid scalar
 *          values append one InvalidUtf8 node.
 * @tparam ndebug Contract checking mode used for AST operations.
 * @param code_point Semantic code point to append.
 * @param[out] result AST receiving the corresponding node or nodes.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_code_point_to_ast(char32_t code_point, ::pltxt2htm::Ast<ndebug>& result) noexcept {
    switch (code_point) {
    case U'\n': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::LineBreak>());
        return;
    }
    case U' ':
    case char32_t{0xA0}: {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::Space>());
        return;
    }
    case U'&': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::Ampersand>());
        return;
    }
    case U'\'': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::SingleQuote>());
        return;
    }
    case U'"': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::DoubleQuote>());
        return;
    }
    case U'<': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::LessThan>());
        return;
    }
    case U'>': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::GreaterThan>());
        return;
    }
    case U'\t': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::Tab>());
        return;
    }
    default: {
        break;
    }
    }

    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::InvalidUtf8>());
        return;
    }

    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    if (encoded.size == 0) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<::pltxt2htm::InvalidUtf8>());
        return;
    }
    for (::std::size_t index{}; index < encoded.size; ++index) {
        result.append_text(encoded.code_units[index]);
    }
}

/**
 * @brief Append a decoded character-reference code point to a UTF-8 string.
 * @details ASCII controls are normalized to U+FFFD before encoding. Character-reference
 *          decoding guarantees that every other input is a Unicode scalar value.
 * @tparam ndebug Contract checking mode.
 * @param[out] result Output string receiving the encoded code point.
 * @param code_point Decoded character-reference code point.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_character_reference_code_point(::pltxt2htm::container::U8String& result,
                                                     char32_t code_point) noexcept {
    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        code_point = char32_t{0xFFFD};
    }
    ::pltxt2htm::details::append_utf8_code_point<ndebug>(result, code_point);
}


/**
 * @brief Append a decoded character reference to an AST.
 * @tparam ndebug Contract checking mode used for AST operations.
 * @param reference Previously decoded one- or two-code-point reference.
 * @param[out] result AST receiving the semantic character nodes.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_character_reference_to_ast(TryDecodeCharacterReferenceResult const& reference,
                                                 ::pltxt2htm::Ast<ndebug>& result) noexcept {
    ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.first_code_point, result);
    if (reference.has_second_code_point()) {
        ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.second_code_point, result);
    }
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
    ::pltxt2htm::details::append_character_reference_to_ast<ndebug>(decoded, result);
    return decoded.consumed_size;
}

/**
 * @brief Decode every supported HTML character reference in a string.
 * @details Unknown, malformed, and unterminated references are copied literally. Decoded
 *          ASCII controls are emitted as U+FFFD to match parser output normalization.
 * @tparam ndebug Contract checking mode used for input and reference-table access.
 * @param text Input text that may contain character references.
 * @return UTF-8 text with all supported references decoded.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto decode_character_references(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::container::U8String {
    ::pltxt2htm::container::U8String result{};
    ::std::size_t const text_size{text.size()};
    result.template reserve<ndebug>(text_size);
    for (::std::size_t index{}; index < text_size;) {
        if (text.template index<ndebug>(index) == u8'&') {
            auto const decoded =
                ::pltxt2htm::details::try_decode_character_reference<ndebug>(text.template subview<ndebug>(index));
            if (decoded.has_value()) {
                auto const& reference = decoded.template value<ndebug>();
                ::pltxt2htm::details::append_character_reference_code_point<ndebug>(result, reference.first_code_point);
                if (reference.has_second_code_point()) {
                    ::pltxt2htm::details::append_character_reference_code_point<ndebug>(result,
                                                                                        reference.second_code_point);
                }
                index += reference.consumed_size;
                continue;
            }
        }
        result.push_back<ndebug>(text.template index<ndebug>(index));
        ++index;
    }
    return result;
}

/**
 * @brief Parse an HTML attribute's source text into valid semantic plain text.
 * @details Character references are decoded, invalid UTF-8 becomes U+FFFD, and unknown references remain literal.
 *          Spaces reach the canonical no-break form through `PlainText`, matching Markdown image alt text.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto make_plain_text_from_html_attribute(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::PlainText<ndebug> {
    ::pltxt2htm::PlainText<ndebug> result{};
    ::std::size_t const text_size{text.size()};
    for (::std::size_t index{}; index < text_size;) {
        char8_t const character{text.template index<ndebug>(index)};
        if (character == u8'&') {
            auto const decoded =
                ::pltxt2htm::details::try_decode_character_reference<ndebug>(text.template subview<ndebug>(index));
            if (decoded.has_value()) {
                auto const& reference = decoded.template value<ndebug>();
                result.append_code_point(reference.first_code_point);
                if (reference.has_second_code_point()) {
                    result.append_code_point(reference.second_code_point);
                }
                index += reference.consumed_size;
                continue;
            }
        }
        index += result.append_first_utf8_code_point(text.template subview<ndebug>(index));
    }
    return result;
}

} // namespace pltxt2htm::details
