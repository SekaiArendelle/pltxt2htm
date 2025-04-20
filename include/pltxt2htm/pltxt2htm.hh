#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm {

/**
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        + Support following pl tags:
 *          - `<color>`
 *          - `<Experiment>`
 *          - `<Discussion>`
 *        + Support following html tags:
 *          - `<br>`
 *        + Support markdown-like syntax
 *        + Support Latex
 * @param pltext The text of Quantum Physics.
 */
inline ::fast_io::u8string pltxt2html(::fast_io::u8string_view pltext) {
    ::fast_io::u8string result{};

    return result;
}

} // namespace pltxt2htm
