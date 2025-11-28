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
    #warning "gcc/clang are remommended more than MSVC(VS2026)"
#endif

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "parser.hh"
#include "optimizer.hh"
#include "details/backend/advanced_html.hh"
#include "details/backend/common_html.hh"
#include "version.hh"

namespace pltxt2htm {

/**
 * @brief Convert Physics-Lab (pl) text to advanced HTML with full feature support
 * @details This function provides the most comprehensive HTML generation with support for:
 *          - Physics-Lab specific tags (color, experiment, discussion, user, size)
 *          - Full Markdown syntax (headers, lists, emphasis, links, code blocks, etc.)
 *          - HTML elements with proper escaping and formatting
 *          - Internal linking to experiments and discussions
 * @tparam ndebug Debug mode flag - false enables debug checks, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation
 * @param pltext The Physics-Lab text content to convert
 * @param host Host URL for generating internal links (used for experiment/discussion links)
 * @return Generated HTML string with full formatting support
 * @note This is the recommended function for most use cases requiring full feature support
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
constexpr auto pltxt2advanced_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2advanced_html<ndebug>(ast, host);
}

/**
 * @brief Convert Physics-Lab text to advanced HTML without escaping less-than symbols
 * @details This function is identical to pltxt2advanced_html except that `<` characters
 *          are not escaped to `&lt;`. This allows embedding raw HTML within the output.
 *
 *          Use this function when you need to:
 *          - Embed custom HTML tags in the output
 *          - Generate HTML that will be further processed
 *          - Create templates with placeholder HTML
 * @tparam ndebug Debug mode flag - false enables debug checks, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation
 * @param pltext The Physics-Lab text content to convert
 * @param host Host URL for generating internal links
 * @return Generated HTML string with full formatting but unescaped < characters
 * @warning Only use this function if you understand the security implications
 *          of unescaped HTML output. Unescaped < can lead to XSS vulnerabilities.
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
constexpr auto pltxt2fixedadv_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2advanced_html<ndebug, false>(ast, host);
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
 * @tparam ndebug Debug mode flag - false enables debug checks, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation (default: false)
 * @param pltext The Physics-Lab text content to convert
 * @return Generated HTML string with basic formatting support
 * @note This function is faster than the advanced versions but supports fewer features
 * @warning Markdown syntax and advanced HTML features are not supported in this mode
 */
template<bool ndebug = false, bool optimize = false>
[[nodiscard]]
constexpr auto pltxt2common_html(::fast_io::u8string_view pltext) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2common_html<ndebug>(ast);
}

} // namespace pltxt2htm
