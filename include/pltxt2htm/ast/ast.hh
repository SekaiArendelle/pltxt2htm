#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <exception/exception.hh>
#include <memory>

#include "../astnode/node_type.hh"

#include "impl/basic_node_decl.hh"
#include "impl/html_node_decl.hh"
#include "impl/markdown_node_decl.hh"
#include "impl/physics_lab_node_decl.hh"

namespace pltxt2htm::ast2 {

namespace details {

class AstVariant {
public:
    union {
        ::pltxt2htm::ast2::U8Char u8char_node;
        ::pltxt2htm::ast2::InvalidU8Char invalid_u8char_node;
        ::pltxt2htm::ast2::Text text_node;
        ::pltxt2htm::ast2::PlColor pl_color_node;
    };

    ::pltxt2htm::NodeType node_kind;

    constexpr AstVariant(::pltxt2htm::ast2::U8Char&& node)
        : u8char_node(::std::move(node)), node_kind{::pltxt2htm::NodeType::u8char} {}

    constexpr AstVariant(::pltxt2htm::ast2::InvalidU8Char&& node)
        : invalid_u8char_node(::std::move(node)), node_kind{::pltxt2htm::NodeType::invalid_u8char} {}

    constexpr AstVariant(::pltxt2htm::ast2::Text&& node)
        : text_node(::std::move(node)), node_kind{::pltxt2htm::NodeType::text} {}

    constexpr AstVariant(::pltxt2htm::ast2::PlColor&& node)
        : pl_color_node(::std::move(node)), node_kind{::pltxt2htm::NodeType::pl_color} {}

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant const&) noexcept = delete;

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant&& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) {
            case ::pltxt2htm::NodeType::u8char:
                new (::std::addressof(u8char_node)) ::pltxt2htm::ast2::U8Char(::std::move(other.u8char_node));
                break;
            case ::pltxt2htm::NodeType::invalid_u8char:
                new (::std::addressof(invalid_u8char_node)) ::pltxt2htm::ast2::InvalidU8Char(::std::move(other.invalid_u8char_node));
                break;
            case ::pltxt2htm::NodeType::text:
                new (::std::addressof(text_node)) ::pltxt2htm::ast2::Text(::std::move(other.text_node));
                break;
            case ::pltxt2htm::NodeType::pl_color:
                new (::std::addressof(pl_color_node)) ::pltxt2htm::ast2::PlColor(::std::move(other.pl_color_node));
                break;
        }
    }

    constexpr ~AstVariant() noexcept {
        switch (node_kind) {
            case ::pltxt2htm::NodeType::u8char:
                u8char_node.~U8Char();
                break;
            case ::pltxt2htm::NodeType::invalid_u8char:
                invalid_u8char_node.~InvalidU8Char();
                break;
            case ::pltxt2htm::NodeType::text:
                text_node.~Text();
                break;
            case ::pltxt2htm::NodeType::pl_color:
                pl_color_node.~PlColor();
                break;
    #if 1 // todo set as 0
            default:
                ::exception::unreachable();
    #endif
        }
    }
};

} // namespace details

class PlTxtNode {
    ::pltxt2htm::ast2::details::AstVariant node;
};

} // namespace pltxt2htm::ast2

#include "impl/basic_node_def.inc"
#include "impl/html_node_def.inc"
#include "impl/markdown_node_def.inc"
#include "impl/physics_lab_node_def.inc"
