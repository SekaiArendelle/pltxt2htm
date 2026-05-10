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
        // basic node
        ::pltxt2htm::ast2::U8Char u8char_node;
        ::pltxt2htm::ast2::InvalidU8Char invalid_u8char_node;
        ::pltxt2htm::ast2::Text text_node;

        // html node


        // markdown node


        // physics lab node
        ::pltxt2htm::ast2::PlColor pl_color_node;
        ::pltxt2htm::ast2::PlA pl_a_node;
        ::pltxt2htm::ast2::PlExperiment pl_experiment_node;
        ::pltxt2htm::ast2::PlDiscussion pl_discussion_node;
        ::pltxt2htm::ast2::PlUser pl_user_node;
        ::pltxt2htm::ast2::PlExternal pl_external_node;
        ::pltxt2htm::ast2::PlSize pl_size_node;
        ::pltxt2htm::ast2::PlI pl_i_node;
        ::pltxt2htm::ast2::PlB pl_b_node;
        ::pltxt2htm::ast2::PlMacroProject pl_macro_project_node;
        ::pltxt2htm::ast2::PlMacroVisitor pl_macro_visitor_node;
        ::pltxt2htm::ast2::PlMacroAuthor pl_macro_author_node;
        ::pltxt2htm::ast2::PlMacroCoauthors pl_macro_coauthors_node;
    };

    ::pltxt2htm::NodeType node_kind;

    constexpr AstVariant(::pltxt2htm::ast2::U8Char&& node)
        : u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::u8char} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::InvalidU8Char&& node)
        : invalid_u8char_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::invalid_u8char} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Text&& node)
        : text_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::text} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlColor&& node)
        : pl_color_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_color} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlA&& node)
        : pl_a_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_a} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlExperiment&& node)
        : pl_experiment_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_experiment} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlDiscussion&& node)
        : pl_discussion_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_discussion} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlUser&& node)
        : pl_user_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_user} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlExternal&& node)
        : pl_external_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_external} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlSize&& node)
        : pl_size_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_size} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlI&& node)
        : pl_i_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_i} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlB&& node)
        : pl_b_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_b} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroProject&& node)
        : pl_macro_project_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_project} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroVisitor&& node)
        : pl_macro_visitor_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_visitor} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroAuthor&& node)
        : pl_macro_author_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_author} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::PlMacroCoauthors&& node)
        : pl_macro_coauthors_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::pl_macro_coauthors} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant const&) noexcept = delete;

    constexpr AstVariant(::pltxt2htm::ast2::details::AstVariant&& other) noexcept
        : node_kind(other.node_kind) {
        switch (node_kind) {
        case ::pltxt2htm::NodeType::u8char:
            new (::std::addressof(u8char_node))::pltxt2htm::ast2::U8Char(::std::move(other.u8char_node));
            break;
        case ::pltxt2htm::NodeType::invalid_u8char:
            new (::std::addressof(invalid_u8char_node))::pltxt2htm::ast2::InvalidU8Char(
                ::std::move(other.invalid_u8char_node));
            break;
        case ::pltxt2htm::NodeType::text:
            new (::std::addressof(text_node))::pltxt2htm::ast2::Text(::std::move(other.text_node));
            break;
        case ::pltxt2htm::NodeType::pl_color:
            new (::std::addressof(pl_color_node))::pltxt2htm::ast2::PlColor(::std::move(other.pl_color_node));
            break;
        case ::pltxt2htm::NodeType::pl_a:
            new (::std::addressof(pl_a_node))::pltxt2htm::ast2::PlA(::std::move(other.pl_a_node));
            break;
        case ::pltxt2htm::NodeType::pl_experiment:
            new (::std::addressof(pl_experiment_node))::pltxt2htm::ast2::PlExperiment(::std::move(other.pl_experiment_node));
            break;
        case ::pltxt2htm::NodeType::pl_discussion:
            new (::std::addressof(pl_discussion_node))::pltxt2htm::ast2::PlDiscussion(::std::move(other.pl_discussion_node));
            break;
        case ::pltxt2htm::NodeType::pl_user:
            new (::std::addressof(pl_user_node))::pltxt2htm::ast2::PlUser(::std::move(other.pl_user_node));
            break;
        case ::pltxt2htm::NodeType::pl_external:
            new (::std::addressof(pl_external_node))::pltxt2htm::ast2::PlExternal(::std::move(other.pl_external_node));
            break;
        case ::pltxt2htm::NodeType::pl_size:
            new (::std::addressof(pl_size_node))::pltxt2htm::ast2::PlSize(::std::move(other.pl_size_node));
            break;
        case ::pltxt2htm::NodeType::pl_i:
            new (::std::addressof(pl_i_node))::pltxt2htm::ast2::PlI(::std::move(other.pl_i_node));
            break;
        case ::pltxt2htm::NodeType::pl_b:
            new (::std::addressof(pl_b_node))::pltxt2htm::ast2::PlB(::std::move(other.pl_b_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_project:
            new (::std::addressof(pl_macro_project_node))::pltxt2htm::ast2::PlMacroProject(::std::move(other.pl_macro_project_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            new (::std::addressof(pl_macro_visitor_node))::pltxt2htm::ast2::PlMacroVisitor(::std::move(other.pl_macro_visitor_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_author:
            new (::std::addressof(pl_macro_author_node))::pltxt2htm::ast2::PlMacroAuthor(::std::move(other.pl_macro_author_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            new (::std::addressof(pl_macro_coauthors_node))::pltxt2htm::ast2::PlMacroCoauthors(::std::move(other.pl_macro_coauthors_node));
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
        case ::pltxt2htm::NodeType::pl_a:
            pl_a_node.~PlA();
            break;
        case ::pltxt2htm::NodeType::pl_experiment:
            pl_experiment_node.~PlExperiment();
            break;
        case ::pltxt2htm::NodeType::pl_discussion:
            pl_discussion_node.~PlDiscussion();
            break;
        case ::pltxt2htm::NodeType::pl_user:
            pl_user_node.~PlUser();
            break;
        case ::pltxt2htm::NodeType::pl_external:
            pl_external_node.~PlExternal();
            break;
        case ::pltxt2htm::NodeType::pl_size:
            pl_size_node.~PlSize();
            break;
        case ::pltxt2htm::NodeType::pl_i:
            pl_i_node.~PlI();
            break;
        case ::pltxt2htm::NodeType::pl_b:
            pl_b_node.~PlB();
            break;
        case ::pltxt2htm::NodeType::pl_macro_project:
            pl_macro_project_node.~PlMacroProject();
            break;
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            pl_macro_visitor_node.~PlMacroVisitor();
            break;
        case ::pltxt2htm::NodeType::pl_macro_author:
            pl_macro_author_node.~PlMacroAuthor();
            break;
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            pl_macro_coauthors_node.~PlMacroCoauthors();
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
