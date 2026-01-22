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

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "parser.hh"
#include "optimizer.hh"
#include "details/backend/for_plweb_text.hh"
#include "details/backend/for_plweb_title.hh"
#include "details/backend/for_plunity_text.hh"
#include "version.hh"

namespace pltxt2htm {

/**
 * @brief Convert Physics-Lab (pl) text to advanced HTML with full feature support, just for unittest
 * @details This function provides the most comprehensive HTML generation with support for:
 *          - Physics-Lab specific tags (color, experiment, discussion, user, size)
 *          - Full Markdown syntax (headers, lists, emphasis, links, code blocks, etc.)
 *          - HTML elements with proper escaping and formatting
 *          - Internal linking to experiments and discussions
 * @tparam ndebug Debug mode flag - false enables debug checks and assertions, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation (default: true)
 * @param[in] pltext The Physics-Lab text content to convert
 * @return Generated HTML string with full formatting support
 * @retval fast_io::u8string UTF-8 string containing the generated HTML
 * @note This is the recommended function for most use cases requiring full feature support
 * @note The function automatically optimizes the AST by default for better performance
 * @warning This function does not support the host parameter - use pltxt2fixedadv_html for that
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto pltxt2advanced_html(::fast_io::u8string_view pltext) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plweb_text_backend<ndebug>(ast, u8"localhost:5173", u8"$PROJECT", u8"$VISITOR",
                                                            u8"$AUTHOR", u8"$CO_AUTHORS");
}

/**
 * @brief Convert Physics-Lab text to advanced HTML
 * @tparam ndebug Debug mode flag - false enables debug checks and assertions, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation (default: true)
 * @param[in] pltext The Physics-Lab text content to convert
 * @param[in] host Host URL for generating internal links (e.g., "https://physicslab.example.com")
 * @param[in] project Project identifier for Physics-Lab context
 * @param[in] visitor Visitor identifier for Physics-Lab context
 * @param[in] author Author identifier for Physics-Lab context
 * @param[in] coauthors Co-authors identifier for Physics-Lab context
 * @note The host parameter is used for generating proper internal links to experiments and discussions
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto pltxt2fixedadv_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host,
                                   ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                                   ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plweb_text_backend<ndebug>(ast, host, project, visitor, author, coauthors);
}

template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto to_plunity_introduction(::fast_io::u8string_view pltext, ::fast_io::u8string_view project,
                                       ::fast_io::u8string_view visitor, ::fast_io::u8string_view author,
                                       ::fast_io::u8string_view coauthors) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plunity_text_backend<ndebug>(ast, project, visitor, author, coauthors);
}

/**
 * @brief Convert Physics-Lab text to common HTML with basic formatting
 * @details This function provides basic HTML generation with limited feature support:
 *          - Physics-Lab color tags only
 *          - Bold (<b>) and italic (<i>) tags only
 *          - Basic HTML escaping and formatting
 *
 *          This function is suitable for:
 *          - Simple text formatting needs
 *          - Header rendering where complex formatting isn't needed
 *          - Performance-critical applications that don't need full features
 * @tparam ndebug Debug mode flag - false enables debug checks and assertions, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation (default: false)
 * @param[in] pltext The Physics-Lab text content to convert
 * @return Generated HTML string with basic formatting support
 * @retval fast_io::u8string UTF-8 string containing the generated basic HTML
 * @note This function is faster than the advanced versions but supports fewer features
 * @warning Markdown syntax and advanced HTML features are not supported in this mode
 * @warning AST optimization is disabled by default for this function
 * @see pltxt2advanced_html for full feature support
 */
template<bool ndebug = false, bool optimize = false>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto pltxt2common_html(::fast_io::u8string_view pltext) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::plweb_title_backend<ndebug>(ast);
}

} // namespace pltxt2htm
