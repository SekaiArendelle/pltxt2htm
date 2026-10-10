/**
 * @file url_node_decl.hh
 * @brief Semantic URL AST node declaration.
 */

#pragma once

#include <utility>

#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "../../details/parser/url_parsing.hh"

namespace pltxt2htm {

/**
 * @brief URL node
 * @details Represents a semantic URL value stored independently of any backend's escaping syntax.
 */
class Url {
    ::pltxt2htm::container::U8String url_str;

    /**
     * @brief Construct from a URL already validated and encoded by try_make.
     */
    constexpr explicit Url(::pltxt2htm::container::U8String&& url) noexcept
        : url_str(::std::move(url)) {
    }

public:
    /**
     * @brief Validate a complete URL and create its semantic, percent-encoded representation.
     * @return A URL on success, or nullopt if any URL component is invalid.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    static constexpr auto try_make(::pltxt2htm::container::U8StringView url) noexcept
        -> ::pltxt2htm::container::Optional<Url> {
        auto const opt_scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(url);
        auto const scheme_end = opt_scheme_end.has_value()
                                    ? opt_scheme_end.template value<ndebug>().template get<ndebug>()
                                    : ::std::size_t{};
        auto const opt_auth_end =
            ::pltxt2htm::details::try_parse_url_authority<ndebug>(url.template subview<ndebug>(scheme_end));
        if (opt_auth_end.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const auth_end = scheme_end + opt_auth_end.template value<ndebug>().template get<ndebug>();
        auto const path_end =
            auth_end + ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(url.template subview<ndebug>(auth_end));
        if (path_end != url.size()) {
            return ::pltxt2htm::container::nullopt;
        }
        ::std::size_t const url_size{url.size()};
        ::pltxt2htm::container::U8String url_str{};
        url_str.template reserve<ndebug>(url_size);
        for (::std::size_t index{}; index < url_size; ++index) {
            auto const chr = url.template index<ndebug>(index);
            if (chr == u8'&') {
                auto const reference =
                    ::pltxt2htm::details::try_decode_character_reference<ndebug>(url.template subview<ndebug>(index));
                if (reference.has_value()) {
                    auto const& decoded = reference.template value<ndebug>();
                    ::pltxt2htm::details::append_code_point_to_url<ndebug>(url_str, decoded.first_code_point);
                    if (decoded.has_second_code_point()) {
                        ::pltxt2htm::details::append_code_point_to_url<ndebug>(url_str, decoded.second_code_point);
                    }
                    index += decoded.consumed_size - 1;
                    continue;
                }
            }
            if (chr > u8'~') {
                // non-ASCII byte (e.g. UTF-8 CJK): percent-encode it so tag URLs keep the raw characters
                ::pltxt2htm::details::append_percent_encoded_url_byte<ndebug>(url_str, chr);
                continue;
            }
            switch (chr) {
            case u8'\'': {
                url_str.append<ndebug>(u8"%27");
                break;
            }
            case u8'\"': {
                url_str.append<ndebug>(u8"%22");
                break;
            }
            case u8'<': {
                url_str.append<ndebug>(u8"%3C");
                break;
            }
            case u8'>': {
                url_str.append<ndebug>(u8"%3E");
                break;
            }
            default: {
                url_str.push_back<ndebug>(chr);
                break;
            }
            }
        }
        return Url{::std::move(url_str)};
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
