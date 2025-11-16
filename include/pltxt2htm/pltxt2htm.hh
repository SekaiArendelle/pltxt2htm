#pragma once

#if __cpp_explicit_this_parameter < 202110L
    #error "Your compiler is too old to compile (e.g. VS2022 MSVC)"
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #warning "While VS2026 MSVC mostly works properly, gcc/clang are remommended."
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
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        Supported syntax are listed in pltxt2htm/astnode.hh: `enum class NodeType`
 * @tparam ndebug: Whether enable more debug checks like NDEBUG macro. show details in README.md Q/A
 * @tparam optimize: whether optimize the generated html
 * @param pltext The text of Quantum Physics.
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
 * @brief The only diffrence between pltxt2advanced_html and pltxt2fixedadv_html is that
 *        `<` won't be transformed to `&lt;`
 * @tparam ndebug: Whether enable more debug checks like NDEBUG macro. show details in README.md Q/A
 * @tparam optimize: whether optimize the generated html
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
 * @tparam ndebug: Whether enable more debug checks like NDEBUG macro. show details in README.md Q/A
 * @tparam optimize: whether optimize the generated html
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
