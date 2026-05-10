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
        ::pltxt2htm::ast2::LineBreak linebreak_node;
        ::pltxt2htm::ast2::Br br_node;
        ::pltxt2htm::ast2::Space space_node;
        ::pltxt2htm::ast2::LessThan lessthan_node;
        ::pltxt2htm::ast2::GreaterThan greaterthan_node;
        ::pltxt2htm::ast2::Tab tab_node;
        ::pltxt2htm::ast2::Ampersand ampersand_node;
        ::pltxt2htm::ast2::SingleQuotationMark singlequotationmark_node;
        ::pltxt2htm::ast2::DoubleQuotationMark doublequotationmark_node;
        ::pltxt2htm::ast2::Hr hr_node;
        ::pltxt2htm::ast2::H1 h1_node;
        ::pltxt2htm::ast2::H2 h2_node;
        ::pltxt2htm::ast2::H3 h3_node;
        ::pltxt2htm::ast2::H4 h4_node;
        ::pltxt2htm::ast2::H5 h5_node;
        ::pltxt2htm::ast2::H6 h6_node;
        ::pltxt2htm::ast2::P p_node;
        ::pltxt2htm::ast2::Del del_node;
        ::pltxt2htm::ast2::Note note_node;
        ::pltxt2htm::ast2::Em em_node;
        ::pltxt2htm::ast2::Strong strong_node;
        ::pltxt2htm::ast2::Ul ul_node;
        ::pltxt2htm::ast2::Ol ol_node;
        ::pltxt2htm::ast2::Li li_node;
        ::pltxt2htm::ast2::Code code_node;
        ::pltxt2htm::ast2::Pre pre_node;
        ::pltxt2htm::ast2::Blockquote blockquote_node;

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

    constexpr AstVariant(::pltxt2htm::ast2::LineBreak&& node)
        : linebreak_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::line_break} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Br&& node)
        : br_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_br} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Space&& node)
        : space_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::space} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::LessThan&& node)
        : lessthan_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::less_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::GreaterThan&& node)
        : greaterthan_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::greater_than} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Tab&& node)
        : tab_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::tab} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ampersand&& node)
        : ampersand_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::ampersand} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::SingleQuotationMark&& node)
        : singlequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::single_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::DoubleQuotationMark&& node)
        : doublequotationmark_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::double_quote} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Hr&& node)
        : hr_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_hr} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H1&& node)
        : h1_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h1} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H2&& node)
        : h2_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h2} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H3&& node)
        : h3_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h3} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H4&& node)
        : h4_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h4} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H5&& node)
        : h5_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h5} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::H6&& node)
        : h6_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_h6} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::P&& node)
        : p_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_p} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Del&& node)
        : del_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_del} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Note&& node)
        : note_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_note} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Em&& node)
        : em_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_em} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Strong&& node)
        : strong_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_strong} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ul&& node)
        : ul_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_ul} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Ol&& node)
        : ol_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_ol} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Li&& node)
        : li_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_li} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Code&& node)
        : code_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_code} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Pre&& node)
        : pre_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_pre} {
    }

    constexpr AstVariant(::pltxt2htm::ast2::Blockquote&& node)
        : blockquote_node(::std::move(node)),
          node_kind{::pltxt2htm::NodeType::html_blockquote} {
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
            new (::std::addressof(pl_experiment_node))::pltxt2htm::ast2::PlExperiment(
                ::std::move(other.pl_experiment_node));
            break;
        case ::pltxt2htm::NodeType::pl_discussion:
            new (::std::addressof(pl_discussion_node))::pltxt2htm::ast2::PlDiscussion(
                ::std::move(other.pl_discussion_node));
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
            new (::std::addressof(pl_macro_project_node))::pltxt2htm::ast2::PlMacroProject(
                ::std::move(other.pl_macro_project_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            new (::std::addressof(pl_macro_visitor_node))::pltxt2htm::ast2::PlMacroVisitor(
                ::std::move(other.pl_macro_visitor_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_author:
            new (::std::addressof(pl_macro_author_node))::pltxt2htm::ast2::PlMacroAuthor(
                ::std::move(other.pl_macro_author_node));
            break;
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            new (::std::addressof(pl_macro_coauthors_node))::pltxt2htm::ast2::PlMacroCoauthors(
                ::std::move(other.pl_macro_coauthors_node));
            break;

        case ::pltxt2htm::NodeType::line_break:
            new (::std::addressof(linebreak_node))::pltxt2htm::ast2::LineBreak(::std::move(other.linebreak_node));
            break;

        case ::pltxt2htm::NodeType::html_br:
            new (::std::addressof(br_node))::pltxt2htm::ast2::Br(::std::move(other.br_node));
            break;

        case ::pltxt2htm::NodeType::space:
            new (::std::addressof(space_node))::pltxt2htm::ast2::Space(::std::move(other.space_node));
            break;

        case ::pltxt2htm::NodeType::less_than:
            new (::std::addressof(lessthan_node))::pltxt2htm::ast2::LessThan(::std::move(other.lessthan_node));
            break;

        case ::pltxt2htm::NodeType::greater_than:
            new (::std::addressof(greaterthan_node))::pltxt2htm::ast2::GreaterThan(::std::move(other.greaterthan_node));
            break;

        case ::pltxt2htm::NodeType::tab:
            new (::std::addressof(tab_node))::pltxt2htm::ast2::Tab(::std::move(other.tab_node));
            break;

        case ::pltxt2htm::NodeType::ampersand:
            new (::std::addressof(ampersand_node))::pltxt2htm::ast2::Ampersand(::std::move(other.ampersand_node));
            break;

        case ::pltxt2htm::NodeType::single_quote:
            new (::std::addressof(singlequotationmark_node))::pltxt2htm::ast2::SingleQuotationMark(
                ::std::move(other.singlequotationmark_node));
            break;

        case ::pltxt2htm::NodeType::double_quote:
            new (::std::addressof(doublequotationmark_node))::pltxt2htm::ast2::DoubleQuotationMark(
                ::std::move(other.doublequotationmark_node));
            break;

        case ::pltxt2htm::NodeType::html_hr:
            new (::std::addressof(hr_node))::pltxt2htm::ast2::Hr(::std::move(other.hr_node));
            break;

        case ::pltxt2htm::NodeType::html_h1:
            new (::std::addressof(h1_node))::pltxt2htm::ast2::H1(::std::move(other.h1_node));
            break;

        case ::pltxt2htm::NodeType::html_h2:
            new (::std::addressof(h2_node))::pltxt2htm::ast2::H2(::std::move(other.h2_node));
            break;

        case ::pltxt2htm::NodeType::html_h3:
            new (::std::addressof(h3_node))::pltxt2htm::ast2::H3(::std::move(other.h3_node));
            break;

        case ::pltxt2htm::NodeType::html_h4:
            new (::std::addressof(h4_node))::pltxt2htm::ast2::H4(::std::move(other.h4_node));
            break;

        case ::pltxt2htm::NodeType::html_h5:
            new (::std::addressof(h5_node))::pltxt2htm::ast2::H5(::std::move(other.h5_node));
            break;

        case ::pltxt2htm::NodeType::html_h6:
            new (::std::addressof(h6_node))::pltxt2htm::ast2::H6(::std::move(other.h6_node));
            break;

        case ::pltxt2htm::NodeType::html_p:
            new (::std::addressof(p_node))::pltxt2htm::ast2::P(::std::move(other.p_node));
            break;

        case ::pltxt2htm::NodeType::html_del:
            new (::std::addressof(del_node))::pltxt2htm::ast2::Del(::std::move(other.del_node));
            break;

        case ::pltxt2htm::NodeType::html_note:
            new (::std::addressof(note_node))::pltxt2htm::ast2::Note(::std::move(other.note_node));
            break;

        case ::pltxt2htm::NodeType::html_em:
            new (::std::addressof(em_node))::pltxt2htm::ast2::Em(::std::move(other.em_node));
            break;

        case ::pltxt2htm::NodeType::html_strong:
            new (::std::addressof(strong_node))::pltxt2htm::ast2::Strong(::std::move(other.strong_node));
            break;

        case ::pltxt2htm::NodeType::html_ul:
            new (::std::addressof(ul_node))::pltxt2htm::ast2::Ul(::std::move(other.ul_node));
            break;

        case ::pltxt2htm::NodeType::html_ol:
            new (::std::addressof(ol_node))::pltxt2htm::ast2::Ol(::std::move(other.ol_node));
            break;

        case ::pltxt2htm::NodeType::html_li:
            new (::std::addressof(li_node))::pltxt2htm::ast2::Li(::std::move(other.li_node));
            break;

        case ::pltxt2htm::NodeType::html_code:
            new (::std::addressof(code_node))::pltxt2htm::ast2::Code(::std::move(other.code_node));
            break;

        case ::pltxt2htm::NodeType::html_pre:
            new (::std::addressof(pre_node))::pltxt2htm::ast2::Pre(::std::move(other.pre_node));
            break;

        case ::pltxt2htm::NodeType::html_blockquote:
            new (::std::addressof(blockquote_node))::pltxt2htm::ast2::Blockquote(::std::move(other.blockquote_node));
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

        case ::pltxt2htm::NodeType::line_break:
            linebreak_node.~LineBreak();
            break;

        case ::pltxt2htm::NodeType::html_br:
            br_node.~Br();
            break;

        case ::pltxt2htm::NodeType::space:
            space_node.~Space();
            break;

        case ::pltxt2htm::NodeType::less_than:
            lessthan_node.~LessThan();
            break;

        case ::pltxt2htm::NodeType::greater_than:
            greaterthan_node.~GreaterThan();
            break;

        case ::pltxt2htm::NodeType::tab:
            tab_node.~Tab();
            break;

        case ::pltxt2htm::NodeType::ampersand:
            ampersand_node.~Ampersand();
            break;

        case ::pltxt2htm::NodeType::single_quote:
            singlequotationmark_node.~SingleQuotationMark();
            break;

        case ::pltxt2htm::NodeType::double_quote:
            doublequotationmark_node.~DoubleQuotationMark();
            break;

        case ::pltxt2htm::NodeType::html_hr:
            hr_node.~Hr();
            break;

        case ::pltxt2htm::NodeType::html_h1:
            h1_node.~H1();
            break;

        case ::pltxt2htm::NodeType::html_h2:
            h2_node.~H2();
            break;

        case ::pltxt2htm::NodeType::html_h3:
            h3_node.~H3();
            break;

        case ::pltxt2htm::NodeType::html_h4:
            h4_node.~H4();
            break;

        case ::pltxt2htm::NodeType::html_h5:
            h5_node.~H5();
            break;

        case ::pltxt2htm::NodeType::html_h6:
            h6_node.~H6();
            break;

        case ::pltxt2htm::NodeType::html_p:
            p_node.~P();
            break;

        case ::pltxt2htm::NodeType::html_del:
            del_node.~Del();
            break;

        case ::pltxt2htm::NodeType::html_note:
            note_node.~Note();
            break;

        case ::pltxt2htm::NodeType::html_em:
            em_node.~Em();
            break;

        case ::pltxt2htm::NodeType::html_strong:
            strong_node.~Strong();
            break;

        case ::pltxt2htm::NodeType::html_ul:
            ul_node.~Ul();
            break;

        case ::pltxt2htm::NodeType::html_ol:
            ol_node.~Ol();
            break;

        case ::pltxt2htm::NodeType::html_li:
            li_node.~Li();
            break;

        case ::pltxt2htm::NodeType::html_code:
            code_node.~Code();
            break;

        case ::pltxt2htm::NodeType::html_pre:
            pre_node.~Pre();
            break;

        case ::pltxt2htm::NodeType::html_blockquote:
            blockquote_node.~Blockquote();
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
