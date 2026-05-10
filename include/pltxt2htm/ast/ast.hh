#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include "../astnode/node_type.hh"

#include "impl/node_decl.hh"
#include "impl/basic_node.hh"
#include "impl/html_node.hh"
#include "impl/markdown_node.hh"
#include "impl/physics_lab_node.hh"

namespace pltxt2htm::ast2 {

namespace details {

class AstVariant {
    union {
        ::pltxt2htm::ast2::U8Char u8char_node;
        ::pltxt2htm::ast2::InvalidU8Char invalid_u8char_node;
        ::pltxt2htm::ast2::Text text_node;
        ::pltxt2htm::ast2::PlColor pl_color_node;
    };

    ::pltxt2htm::NodeType node_kind;
};

} // namespace details

class PlTxtNode {
    ::pltxt2htm::ast2::details::AstVariant node;
};

} // namespace pltxt2htm::ast2
