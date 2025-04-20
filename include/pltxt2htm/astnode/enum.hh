#pragma once

#include <cstdint>

namespace pltxt2htm {

/**
 * @brief ast node type of Quantum-Physics (aka. Physics-Lab, pl) text
 * @details show details in astnode.hh, every AstNode owns a NodeType
 */
enum class NodeType : ::std::uint_least32_t {
    base = 0,
    text,

    // pl tags
    color,
    a,
    experiment,
    discussion,
    user,
    size,
    b,
    i,

    // html tags
    br,
    h,
    p,

    // md syntaxs
    // latex
    latex,
};

} // namespace pltxt2htm
