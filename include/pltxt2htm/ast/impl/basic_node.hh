#pragma once

#include "node_decl.hh"

namespace pltxt2htm::ast2 {

class U8Char {
    char8_t chr;
};

class InvalidU8Char {};

class Text {
    ::pltxt2htm::ast2::Ast subast;
};

} // namespace pltxt2htm::ast2
