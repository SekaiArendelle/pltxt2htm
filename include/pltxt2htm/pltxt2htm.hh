#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma message("Fuck you, MSVC! Use [gcc/clang](https://github.com/24bit-xjkp/toolchains/releases) instead")
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
constexpr auto pltxt2advanced_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) /* throws */ {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2advanced_html<ndebug>(::std::move(ast), host);
}

/**
 * @brief The only diffrence between pltxt2advanced_html and pltxt2fixedadv_html is that
 *        `<` won't be transformed to `&lt;`
 * @tparam ndebug: Whether enable more debug checks like NDEBUG macro. show details in README.md Q/A
 * @tparam optimize: whether optimize the generated html
 */
template<bool ndebug = false, bool optimize = true>
[[nodiscard]]
constexpr auto pltxt2fixedadv_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host) /* throws */ {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2advanced_html<ndebug, false>(::std::move(ast), host);
}

/**
 * @tparam ndebug: Whether enable more debug checks like NDEBUG macro. show details in README.md Q/A
 * @tparam optimize: whether optimize the generated html
 */
template<bool ndebug = false, bool optimize = false>
[[nodiscard]]
constexpr auto pltxt2common_html(::fast_io::u8string_view pltext) /* throws */ {
    auto ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
    if constexpr (optimize) {
        ::pltxt2htm::optimize_ast<ndebug>(ast);
    }
    return ::pltxt2htm::details::ast2common_html<ndebug>(::std::move(ast));
}

} // namespace pltxt2htm
