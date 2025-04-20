#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "astnode/astnode.hh"

namespace pltxt2htm {

constexpr auto parse_pltxt(::fast_io::u8string_view pltext) noexcept {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> result{};

    return result;
}

/**
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        Supported pl-text syntax are listed in pltxt2htm/astnode.hh: `enum class NodeType`
 * @param pltext The text of Quantum Physics.
 */
constexpr ::fast_io::u8string pltxt2html(::fast_io::u8string_view pltext) {
    ::fast_io::u8string result{};

    return result;
}

} // namespace pltxt2htm
