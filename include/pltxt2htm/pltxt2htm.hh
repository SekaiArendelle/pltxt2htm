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
#include "details/backend/generic.hh"
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
 * @param[in] pltext The Physics-Lab text content to convert
 * @param[in] host Host URL for generating internal links (used for experiment/discussion links)
 * @return Generated HTML string with full formatting support
 * @retval fast_io::u8string UTF-8 string containing the generated HTML
 * @note This is the recommended function for most use cases requiring full feature support
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto pltxt2advanced_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::generic_backend<ndebug, ::pltxt2htm::details::backend_mode::normal_html>(ast, host);
}

/**
 * @brief Convert Physics-Lab text to advanced HTML without escaping less-than symbols
 * @tparam ndebug Debug mode flag - false enables debug checks, true for release mode
 * @tparam optimize Whether to optimize the AST before HTML generation
 * @param[in] pltext The Physics-Lab text content to convert
 * @param[in] host Host URL for generating internal links
 * @return Generated HTML string with full formatting but unescaped < characters
 * @retval fast_io::u8string UTF-8 string containing the generated HTML with unescaped <
 * @warning Only use this function if you understand the security implications
 *          of unescaped HTML output. Unescaped < can lead to XSS vulnerabilities.
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto pltxt2fixedadv_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) noexcept {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::generic_backend<ndebug, ::pltxt2htm::details::backend_mode::main_text_html>(ast, host);
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
 * @param[in] pltext The Physics-Lab text content to convert
 * @return Generated HTML string with basic formatting support
 * @retval fast_io::u8string UTF-8 string containing the generated basic HTML
 * @note This function is faster than the advanced versions but supports fewer features
 * @warning Markdown syntax and advanced HTML features are not supported in this mode
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
    return ::pltxt2htm::details::ast2common_html<ndebug>(ast);
}

} // namespace pltxt2htm
