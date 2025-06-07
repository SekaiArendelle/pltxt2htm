#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "parser.hh"
#include "backend.hh"

namespace pltxt2htm {

/**
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        Supported syntax are listed in pltxt2htm/astnode.hh: `enum class NodeType`
 * @tparam ndebug: show explanation in README.md Q/A
 * @param pltext The text of Quantum Physics.
 * @param is_inline Whether ignore <size> and line break.
 */
template<bool ndebug = false, bool disable_log = true>
[[nodiscard]]
constexpr auto pltxt2html(::fast_io::u8string_view pltext, bool is_inline = false) noexcept(disable_log == true) {
    return ::pltxt2htm::ast2html<ndebug, disable_log>(::pltxt2htm::parse_pltxt<ndebug, disable_log>(pltext), is_inline);
}

} // namespace pltxt2htm
