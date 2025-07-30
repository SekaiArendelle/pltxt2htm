#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma message("Fuck you, MSVC! Use [gcc/clang](https://github.com/24bit-xjkp/toolchains/releases) instead")
#endif

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "parser.hh"
#include "backend/advanced_html.hh"
#include "backend/common_html.hh"
#include "version.hh"

namespace pltxt2htm {

/**
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        Supported syntax are listed in pltxt2htm/astnode.hh: `enum class NodeType`
 * @tparam ndebug: show explanation in README.md Q/A
 * @param pltext The text of Quantum Physics.
 */
template<bool ndebug = false>
[[nodiscard]]
constexpr auto pltxt2advanced_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host)
#if __cpp_exceptions < 199711L
    noexcept
#endif
{
    return ::pltxt2htm::details::ast2advanced_html<ndebug>(::pltxt2htm::parse_pltxt<ndebug>(pltext), host);
}

/**
 * @brief The only diffrence between pltxt2advanced_html and pltxt2fixedadv_html is that
 *        `<` won't be transformed to `&lt;`
 */
template<bool ndebug = false>
[[nodiscard]]
constexpr auto pltxt2fixedadv_html(::fast_io::u8string_view pltext, ::fast_io::u8string_view host)
#if __cpp_exceptions < 199711L
    noexcept
#endif
{
    return ::pltxt2htm::details::ast2advanced_html<ndebug, false>(::pltxt2htm::parse_pltxt<ndebug>(pltext), host);
}

template<bool ndebug = false>
[[nodiscard]]
constexpr auto pltxt2common_html(::fast_io::u8string_view pltext)
#if __cpp_exceptions < 199711L
    noexcept
#endif
{
    return ::pltxt2htm::details::ast2common_html<ndebug>(::pltxt2htm::parse_pltxt<ndebug>(pltext));
}

} // namespace pltxt2htm
