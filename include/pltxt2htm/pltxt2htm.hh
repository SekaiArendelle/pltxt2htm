/**
 * @file pltxt2htm.hh
 * @brief Main API header for pltxt2htm library
 * @details Provides the main conversion functions to transform Physics-Lab (pl) text
 *          into HTML format with various levels of feature support
 */

#pragma once

#if __cpp_explicit_this_parameter < 202110L
    #error "Your compiler is too old to compile (e.g. VS2022 MSVC)"
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #warning "gcc/clang are recommended more than MSVC(VS2026)"
#endif

#include <fast_io/fast_io_dsal/string.h>
#include "container/array.hh"
#include "container/string_view.hh"
#include "container/optional.hh"
#include "container/expected.hh"
#include "container/vector.hh"
#include "contracts.hh"
#include "parser.hh"
#include "inline_parser.hh"
#include "optimizer.hh"
#include "details/backend/for_plweb_text.hh"
#include "details/backend/for_plweb_title.hh"
#include "details/backend/for_plunity_text.hh"
#include "version.hh"

namespace pltxt2htm {

/**
 * @brief Convert Physics-Lab text to advanced HTML
 * @tparam ndebug Contract checking mode. Supported values are ::pltxt2htm::Contracts
 *                 enumerators such as ::pltxt2htm::Contracts::quick_enforce and
 *                 ::pltxt2htm::Contracts::ignore
 * @tparam optimize Whether to optimize the AST before HTML generation (default: true)
 * @param[in] pltext The Physics-Lab text content to convert
 * @param[in] host Host URL for generating internal links (e.g., "https://physicslab.example.com")
 * @param[in] project Project identifier for Physics-Lab context
 * @param[in] visitor Visitor identifier for Physics-Lab context
 * @param[in] author Author identifier for Physics-Lab context
 * @param[in] coauthors Co-authors identifier for Physics-Lab context
 * @note The host parameter is used for generating proper internal links to experiments and discussions
 */
template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, bool optimize = true>
[[nodiscard]]
constexpr auto pltxt2fixedadv_html(::pltxt2htm::container::U8StringView pltext,
                                   ::pltxt2htm::container::U8StringView host,
                                   ::pltxt2htm::container::U8StringView project,
                                   ::pltxt2htm::container::U8StringView visitor,
                                   ::pltxt2htm::container::U8StringView author,
                                   ::pltxt2htm::container::U8StringView coauthors) noexcept {
    using parser_result_type = ::std::conditional_t<optimize, ::pltxt2htm::Ast<ndebug>, ::pltxt2htm::Ast<ndebug> const>;
    parser_result_type ast{::pltxt2htm::parse_pltxt<ndebug>(pltext)};
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plweb_text_backend<ndebug, ::pltxt2htm::details::PlWebTextBackendMode::fixedadv_html>(
        ast, host, project, visitor, author, coauthors);
}

/**
 * @brief Convert Physics-Lab text to PLUnity introduction HTML
 * @details Generates HTML using the PLUnity introduction backend with caller-provided
 *          project and user context values for link/text rendering.
 * @tparam ndebug Contract checking mode. Supported values are ::pltxt2htm::Contracts
 *                 enumerators such as ::pltxt2htm::Contracts::quick_enforce and
 *                 ::pltxt2htm::Contracts::ignore
 * @tparam optimize Whether to optimize the AST before HTML generation (default: true)
 * @param[in] pltext The Physics-Lab text content to convert
 * @param[in] project Project identifier for Physics-Lab context
 * @param[in] visitor Visitor identifier for Physics-Lab context
 * @param[in] author Author identifier for Physics-Lab context
 * @param[in] coauthors Co-authors identifier for Physics-Lab context
 * @return Generated HTML string for PLUnity introduction rendering
 * @retval fast_io::u8string UTF-8 string containing the generated HTML
 */
template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, bool optimize = true>
[[nodiscard]]
constexpr auto pltxt2plunity_introduction(::pltxt2htm::container::U8StringView pltext,
                                          ::pltxt2htm::container::U8StringView project,
                                          ::pltxt2htm::container::U8StringView visitor,
                                          ::pltxt2htm::container::U8StringView author,
                                          ::pltxt2htm::container::U8StringView coauthors) noexcept {
    using parser_result_type = ::std::conditional_t<optimize, ::pltxt2htm::Ast<ndebug>, ::pltxt2htm::Ast<ndebug> const>;
    parser_result_type ast{::pltxt2htm::parse_pltxt<ndebug>(pltext)};
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plunity_text_backend<ndebug>(ast, project, visitor, author, coauthors);
}

/**
 * @brief Convert Physics-Lab text to common HTML with basic formatting
 * @details This function provides basic HTML generation with limited feature support:
 *          - Unity color, bold (&lt;b&gt;), and italic (&lt;i&gt;) tags
 *          - Basic HTML escaping and formatting
 *
 *          The text is parsed with the inline-only parser: block-level syntax
 *          (md-atx headings, md-block-quotes, md-list, md-table, code fences and
 *          HTML block tags) is preserved as literal text.
 *
 *          This function is suitable for:
 *          - Simple text formatting needs
 *          - Header rendering where complex formatting isn't needed
 *          - Performance-critical applications that don't need full features
 * @tparam ndebug Contract checking mode. Use `::pltxt2htm::Contracts::quick_enforce` to enforce checks or
 * `::pltxt2htm::Contracts::ignore` to skip them.
 * @tparam optimize Whether to optimize the AST before HTML generation (default: false)
 * @param[in] pltext The Physics-Lab text content to convert
 * @return Generated HTML string with basic formatting support
 * @retval fast_io::u8string UTF-8 string containing the generated basic HTML
 * @note This function is faster than the advanced versions but supports fewer features
 * @note Markdown block syntax and block-level HTML tags render as literal text in this mode
 * @warning AST optimization is disabled by default for this function
 */
template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, bool optimize = false>
[[nodiscard]]
constexpr auto pltxt2common_html(::pltxt2htm::container::U8StringView pltext) noexcept {
    using parser_result_type = ::std::conditional_t<optimize, ::pltxt2htm::Ast<ndebug>, ::pltxt2htm::Ast<ndebug> const>;
    parser_result_type ast{::pltxt2htm::inline_parse_pltxt<ndebug>(pltext)};
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plweb_title_backend<ndebug>(ast);
}

} // namespace pltxt2htm
