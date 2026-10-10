/**
 * @file url_parsing.hh
 * @brief URL component validation and URL byte encoding helpers.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include "../../container/non_zero.hh"
#include "../../container/optional.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "../utf8.hh"
#include "../utils.hh"
#include "character_reference.hh"

namespace pltxt2htm::details {

/**
 * @brief Detect and return the end offset of a leading `http://` or `https://` scheme.
 * @details Matching is ASCII case-insensitive and takes constant time without scanning for a domain.
 * @param pltext Input that may begin with a supported scheme.
 * @return The scheme length (7 or 8), or nullopt when no supported scheme matches.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_scheme(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZeroUsize> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"http">(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const after_http = pltext.template subview<ndebug>(4);
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"://">(after_http)) {
        return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(7);
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"s://">(after_http)) {
        return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(8);
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Check whether a parsed domain ends in an accepted top-level domain.
 */
[[nodiscard]]
constexpr bool has_allowed_url_tld(::pltxt2htm::container::U8StringView domain) noexcept {
    return domain.ends_with(u8".com") || domain.ends_with(u8".net") || domain.ends_with(u8".org") ||
           domain.ends_with(u8".cn") || domain.ends_with(u8".edu") || domain.ends_with(u8".gov") ||
           domain.ends_with(u8".io") || domain.ends_with(u8".ai") || domain.ends_with(u8".co") ||
           domain.ends_with(u8".me") || domain.ends_with(u8".cc") || domain.ends_with(u8".tv") ||
           domain.ends_with(u8".info") || domain.ends_with(u8".biz") || domain.ends_with(u8".us") ||
           domain.ends_with(u8".uk") || domain.ends_with(u8".jp") || domain.ends_with(u8".hk") ||
           domain.ends_with(u8".tw") || domain.ends_with(u8".xyz") || domain.ends_with(u8".top");
}

/**
 * @brief Parse and validate a URL domain in one pass.
 * @details `pltext` must start at the domain (the caller subviews past the scheme, if any);
 *          the returned index is relative to `pltext`.
 * @param[in] pltext Input text starting at the domain.
 * @return The relative index after the domain, or nullopt when a label or TLD is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_domain(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZeroUsize> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    bool label_has_char{};
    bool label_ended_with_hyphen{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (::pltxt2htm::details::is_ascii_alpha(chr) || ::pltxt2htm::details::is_ascii_digit(chr)) {
            label_has_char = true;
            label_ended_with_hyphen = false;
        }
        else if (chr == u8'-') {
            if (label_has_char == false) {
                return ::pltxt2htm::container::nullopt;
            }
            label_ended_with_hyphen = true;
        }
        else if (chr == u8'.') {
            if (label_has_char == false || label_ended_with_hyphen) {
                return ::pltxt2htm::container::nullopt;
            }
            label_has_char = false;
            label_ended_with_hyphen = false;
        }
        else {
            break;
        }
        ++current_index;
    }

    if (label_has_char == false || label_ended_with_hyphen) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const domain = pltext.template subview<ndebug>(0, current_index);
    if (::pltxt2htm::details::has_allowed_url_tld(domain) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(current_index);
}

/**
 * @brief Parse and validate a URL port and its following delimiter.
 * @details `pltext` must start at the port digits (the caller subviews past the `:`); the
 *          returned index is relative to `pltext`.
 * @param[in] pltext Input text starting at the port.
 * @return The relative index after the port, or nullopt when the port is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_port(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZeroUsize> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::uint_least32_t port{};
    ::std::size_t current_index{};
    ::std::size_t port_size{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        port = port * 10 + static_cast<::std::uint_least32_t>(chr - u8'0');
        ++current_index;
        ++port_size;
        if (port_size > 5) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    if (port_size == 0 || port > 65535) {
        return ::pltxt2htm::container::nullopt;
    }
    if (current_index < pltext_size) {
        auto const next_chr = pltext.template index<ndebug>(current_index);
        if (next_chr != u8'/' && next_chr != u8'?' && next_chr != u8'#') {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(current_index);
}

/**
 * @brief Parse and validate the authority part (domain + port) of a URL.
 *
 * Does NOT detect the scheme - the caller must pass a view starting at the domain (e.g. a
 * subview past the scheme, or the whole candidate when no scheme is present). Supports
 * domain validation and optional port. Does NOT parse the path, query, or fragment - that
 * is the caller's responsibility.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the domain.
 * @return The relative index after the port (or after the domain if no port), or nullopt when
 *         domain/port validation fails.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_authority(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::container::NonZeroUsize> {
    auto const opt_domain_end = ::pltxt2htm::details::try_parse_url_domain<ndebug>(pltext);
    if (opt_domain_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const domain_end = opt_domain_end.template value<ndebug>().template get<ndebug>();
    if (domain_end >= pltext.size() || pltext.template index<ndebug>(domain_end) != u8':') {
        return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(domain_end);
    }
    auto const opt_port_end =
        ::pltxt2htm::details::try_parse_url_port<ndebug>(pltext.template subview<ndebug>(domain_end + 1));
    if (opt_port_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::container::NonZeroUsize::from<ndebug>(
        domain_end + 1 + opt_port_end.template value<ndebug>().template get<ndebug>());
}

/**
 * @brief Parse the simple URL path: printable ASCII, stops at `<` `>` `"` or non-printable characters.
 * @details `pltext` must start at the path (the caller subviews past the authority); the
 *          returned index is relative to `pltext`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the path.
 * @return The relative index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_simple(::pltxt2htm::container::U8StringView pltext) noexcept -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext.is_empty() == false) {
        auto const chr = pltext.template index<ndebug>(0);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return 0;
        }
    }
    ::std::size_t current_index{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++current_index;
    }
    return current_index;
}

/**
 * @brief Parse a URL path that may contain non-ASCII bytes (percent-encoded later).
 * @details Like try_parse_url_path_simple but also accepts bytes >= 0x7F so tag URLs
 *          (html_a / pl_external / unity_link) can carry UTF-8 characters (e.g. CJK);
 *          Url::try_make percent-encodes them. Auto-detected URLs stay ASCII-only.
 *          `pltext` must start at the path (the caller subviews past the authority); the
 *          returned index is relative to `pltext`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the path.
 * @return The relative index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_unicode(::pltxt2htm::container::U8StringView pltext) noexcept -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext.is_empty() == false) {
        auto const chr = pltext.template index<ndebug>(0);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return 0;
        }
    }
    ::std::size_t current_index{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr < u8'!' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++current_index;
    }
    return current_index;
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_percent_encoded_url_byte(::pltxt2htm::container::U8String& result, char8_t byte) noexcept {
    result.push_back<ndebug>(u8'%');
    auto const hi = static_cast<unsigned>(byte) >> 4;
    auto const lo = static_cast<unsigned>(byte) & 0x0F;
    result.push_back<ndebug>(static_cast<char8_t>(hi < 10 ? u8'0' + hi : u8'A' + (hi - 10)));
    result.push_back<ndebug>(static_cast<char8_t>(lo < 10 ? u8'0' + lo : u8'A' + (lo - 10)));
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_code_point_to_url(::pltxt2htm::container::U8String& result, char32_t code_point) noexcept {
    if (code_point < char32_t{0x80}) {
        auto const chr = static_cast<char8_t>(code_point);
        if (chr < u8'!' || chr > u8'~' || chr == u8'\'' || chr == u8'<' || chr == u8'>' || chr == u8'"') {
            ::pltxt2htm::details::append_percent_encoded_url_byte<ndebug>(result, chr);
        }
        else {
            result.push_back<ndebug>(chr);
        }
        return;
    }

    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    for (::std::size_t index{}; index < encoded.size; ++index) {
        ::pltxt2htm::details::append_percent_encoded_url_byte<ndebug>(result, encoded.code_units[index]);
    }
}

} // namespace pltxt2htm::details
