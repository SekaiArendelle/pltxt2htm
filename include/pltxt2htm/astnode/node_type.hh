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

    // \n
    line_break,
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
    pl_color, // be used by Quantum-Physics
    // Quantum-Physics's <a> tag
    pl_a,
    // link of experiment
    pl_experiment, // be used by Quantum-Physics
    // link of discussion
    pl_discussion, // be used by Quantum-Physics
    // link of user
    pl_user, // be used by Quantum-Physics
    // size of text
    pl_size, // be used by Quantum-Physics

    // bold
    pl_b, // be used by Quantum-Physics, html, markdown(**)
    // italic
    pl_i, // be used by Quantum-Physics, html, markdown(*)

    // paragraph
    html_p, // be used by html, markdown(\n\n)
    // line break
    html_br, // be used by Quantum-Physics(\n), html, markdown(\n)
    // header
    html_h1, // be used by html, markdown
    html_h2,
    html_h3,
    html_h4,
    html_h5,
    html_h6,
    // strikethrough
    html_del,

    // latex
    // latex,
};

} // namespace pltxt2htm
