/**
 * @file url_scheme.hh
 * @brief Parse URL schemes supported by automatic links.
 */

#pragma once

#include "../../container/non_zero.hh"
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "../utils.hh"

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

} // namespace pltxt2htm::details
