/**
 * @file url_node_decl.hh
 * @brief Semantic URL AST node declaration.
 */

#pragma once

#include <utility>

#include "../../container/string.hh"

namespace pltxt2htm {

/**
 * @brief URL node
 * @details Represents a semantic URL value stored independently of any backend's escaping syntax.
 */
class Url {
    ::pltxt2htm::container::U8String url_str;

public:
    /**
     * @brief Construct a ::pltxt2htm::Url from a semantic URL string.
     * @param url The URL string without HTML attribute escaping.
     */
    constexpr explicit Url(::pltxt2htm::container::U8String&& url) noexcept
        : url_str(::std::move(url)) {
    }

    constexpr Url(Url const&) noexcept = default;
    constexpr Url(Url&&) noexcept = default;
    constexpr ~Url() noexcept = default;
    constexpr auto operator=(Url const&) noexcept -> Url& = default;
    constexpr auto operator=(this Url& self, Url&&) noexcept -> Url& = default;

    [[nodiscard]]
    constexpr auto operator==(this Url const&, Url const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto as_string(this Url const& self) noexcept -> ::pltxt2htm::container::U8String const& {
        return self.url_str;
    }
};

} // namespace pltxt2htm
