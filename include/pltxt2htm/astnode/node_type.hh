#pragma once

#include <cstdint>

namespace pltxt2htm {

/**
 * @brief ast node type of Quantum-Physics (aka. Physics-Lab, pl) text
 * @details show details in astnode.hh, every AstNode owns a NodeType
 */
enum class NodeType : ::std::uint_least32_t {
    base = 0,
    // a utf-8 char/string
    u8char,

    // space will be squashed by html render
    space,
    // &
    ampersand,
    // "
    double_quote,
    // '
    single_quote,
    // left angle bracket
    less_than,
    // right angle bracket
    greater_than,

    // set color of text
    color, // be used by Quantum-Physics
    // bule color of text
    a, // be used by Quantum-Physics
    // link of experiment
    experiment, // be used by Quantum-Physics
    // link of discussion
    discussion, // be used by Quantum-Physics
    // link of user
    user, // be used by Quantum-Physics
    // size of text
    size, // be used by Quantum-Physics
    // bold
    b, // be used by Quantum-Physics, html, markdown(**)
    // italic
    i, // be used by Quantum-Physics, html, markdown(*)

    // paragraph
    p, // be used by html, markdown(\n\n)
    // line break
    br, // be used by Quantum-Physics(\n), html, markdown(\n)
    // header
    h, // be used by html, markdown(#)
    // image
    img,

    // latex
    latex,
};

} // namespace pltxt2htm
