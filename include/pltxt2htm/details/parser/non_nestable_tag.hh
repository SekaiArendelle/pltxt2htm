/**
 * @file non_nestable_tag.hh
 * @brief URL-bearing `<tag=...>` parsers that validate against the active parser stack.
 *
 * @details These helpers iterate the call stack to reject forbidden nesting, so
 *          they need the complete ParserFrameContext type.  They live in a
 *          separate header instead of try_parse.hh because try_parse.hh cannot
 *          include frame_context.hh: that header includes html_table.hh, which
 *          in turn includes try_parse.hh.
 */

#pragma once

#include <cstddef>
#include <utility>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/node_kind.hh"

#include "try_parse.hh"
#include "frame_context.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Parse `<tag=value>` and reject it when nested inside non-nestable PL tags.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam prefix_str Tag-name prefix used by `try_parse_equal_sign_tag`.
 * @param[in] pltext The input text to parse at current position.
 * @param[in] call_stack Active parser stack used to detect forbidden nesting.
 * @return Parsed tag result on success, otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug, U8LiteralString prefix_str, auto value_char_predicate>
[[nodiscard]]
constexpr auto try_parse_non_nestable_equal_sign_tag(
    ::fast_io::u8string_view pltext, ::fast_io::stack<ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    auto result = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, prefix_str, value_char_predicate>(pltext);
    if (result.has_value() == false) {
        return ::exception::nullopt;
    }
    for (auto const& v : call_stack.container) {
        // skip
        // e.g. <experiment><experiment>test</experiment>text</experiment>
        // e.g. <experiment><a><experiment>test</experiment>text</a>text</experiment>
        auto const nested_tag_type = v.get_nested_tag_type();
        if (nested_tag_type == ::pltxt2htm::NodeKind::pl_experiment ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_discussion ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_experiments ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_discussions ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_external ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_link || nested_tag_type == ::pltxt2htm::NodeKind::pl_trigger ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_internal) {
            return ::exception::nullopt;
        }
    }
    return result;
}

/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` — `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` — `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` — `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseExternalTagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::exception::optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.

    constexpr TryParseExternalTagResult() noexcept
        : tag_len{},
          url{::exception::nullopt} {
    }

    constexpr TryParseExternalTagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::exception::nullopt} {
    }

    constexpr TryParseExternalTagResult(::std::size_t tag_len_, ::pltxt2htm::Url&& url_) noexcept
        : tag_len{tag_len_},
          url(::std::move(url_)) {
    }

    /// State is derived from the payload: engaged url means valid; otherwise a non-zero
    /// tag_len means the URL failed validation; a zero tag_len means not a tag.
    [[nodiscard]]
    constexpr auto is_valid(this auto const& self) noexcept -> bool {
        return self.url.has_value();
    }

    [[nodiscard]]
    constexpr auto is_invalid_url(this auto const& self) noexcept -> bool {
        return self.url.has_value() == false && self.tag_len != 0;
    }
};

/**
 * @brief Parse `<external=...>` tag and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `external` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return `valid` with tag length + URL on success; `invalid_url` with the tag length when the
 *         opening tag was recognized but its URL failed validation; `not_a_tag` otherwise.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_external_tag(::fast_io::u8string_view pltext,
                                      ::fast_io::stack<ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> TryParseExternalTagResult {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"xternal",
                                                                              ::pltxt2htm::details::is_url_value_char>(
        pltext, call_stack);
    if (result.has_value() == false) {
        return {};
    }
    auto const tag_len = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len;

    auto&& [_, url_str] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto const url_vw = ::fast_io::u8string_view{url_str.data(), url_str.size()};
    auto const scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(url_vw).value_or(::std::size_t{});
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, scheme_end));
    if (opt_auth_end.has_value() == false) {
        return TryParseExternalTagResult{tag_len};
    }
    auto const auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + scheme_end;
    auto const path_end = ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(
                              ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, auth_end)) +
                          auth_end;
    if (path_end != url_vw.size()) {
        return TryParseExternalTagResult{tag_len};
    }
    auto opt_url = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, url_vw.size());
    if (opt_url.has_value() == false) {
        return TryParseExternalTagResult{tag_len};
    }

    return TryParseExternalTagResult{
        tag_len, ::std::move(opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` — `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` — `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` — `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseLinkTagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::exception::optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.

    constexpr TryParseLinkTagResult() noexcept
        : tag_len{},
          url{::exception::nullopt} {
    }

    constexpr TryParseLinkTagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::exception::nullopt} {
    }

    constexpr TryParseLinkTagResult(::std::size_t tag_len_, ::pltxt2htm::Url&& url_) noexcept
        : tag_len{tag_len_},
          url(::std::move(url_)) {
    }

    /// State is derived from the payload: engaged url means valid; otherwise a non-zero
    /// tag_len means the URL failed validation; a zero tag_len means not a tag.
    [[nodiscard]]
    constexpr auto is_valid(this auto const& self) noexcept -> bool {
        return self.url.has_value();
    }

    [[nodiscard]]
    constexpr auto is_invalid_url(this auto const& self) noexcept -> bool {
        return self.url.has_value() == false && self.tag_len != 0;
    }
};

/**
 * @brief Parse `<link="...">` tag (Unity TextMeshPro rich text) and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `link` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return `valid` with tag length + URL on success; `invalid_url` with the tag length when the
 *         opening tag was recognized but its URL failed validation; `not_a_tag` otherwise.
 * @note The Unity TextMeshPro link tag uses a quoted value: &lt;link=&quot;url&quot;&gt;. A value
 *       without surrounding double quotes is rejected so that unquoted `<link=url>` stays plain text.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_link_tag(::fast_io::u8string_view pltext,
                                  ::fast_io::stack<ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> TryParseLinkTagResult {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"ink",
                                                                              ::pltxt2htm::details::is_url_value_char>(
        pltext, call_stack);
    if (result.has_value() == false) {
        return {};
    }
    auto const tag_len = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len;

    auto&& [_, raw_value] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    if (raw_value.size() < 2 || ::pltxt2htm::details::u8string_view_index<ndebug>(raw_value, 0) != u8'"' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(raw_value, raw_value.size() - 1) != u8'"') {
        // unquoted value is not a Unity TextMeshPro link tag: keep the char-by-char fallback
        return {};
    }
    auto const url_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(raw_value, 1, raw_value.size() - 2);
    auto const scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(url_vw).value_or(::std::size_t{});
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, scheme_end));
    if (opt_auth_end.has_value() == false) {
        return TryParseLinkTagResult{tag_len};
    }
    auto const auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + scheme_end;
    auto const path_end = ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(
                              ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, auth_end)) +
                          auth_end;
    if (path_end != url_vw.size()) {
        return TryParseLinkTagResult{tag_len};
    }
    auto opt_url = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, url_vw.size());
    if (opt_url.has_value() == false) {
        return TryParseLinkTagResult{tag_len};
    }

    return TryParseLinkTagResult{tag_len,
                                 ::std::move(opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
