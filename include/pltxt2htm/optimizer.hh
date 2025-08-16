#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include "utils.hh"
#include "astnode/basic.hh"
#include "astnode/node_type.hh"
#include "astnode/physics_lab_node.hh"

namespace pltxt2htm {

namespace details {

class OptimizerContext {
public:
    ::pltxt2htm::Ast* ast;
    ::pltxt2htm::NodeType nested_tag_type;
    ::std::size_t current_index;

    OptimizerContext(::pltxt2htm::Ast* node_, ::pltxt2htm::NodeType nested_tag_type_,
                     ::std::size_t current_index_) noexcept
        : ast(node_),
          nested_tag_type{nested_tag_type_},
          current_index{current_index_} {
    }

    constexpr OptimizerContext(::pltxt2htm::details::OptimizerContext const&) noexcept = default;

    constexpr OptimizerContext(::pltxt2htm::details::OptimizerContext&&) noexcept = default;

    constexpr ~OptimizerContext() noexcept = default;

    constexpr ::pltxt2htm::details::OptimizerContext& operator=(
        ::pltxt2htm::details::OptimizerContext const&) noexcept = default;

    constexpr ::pltxt2htm::details::OptimizerContext& operator=(::pltxt2htm::details::OptimizerContext&&) noexcept =
        default;
};

} // namespace details

template<bool ndebug>
constexpr void optimize_ast(::pltxt2htm::Ast& ast_init) noexcept {
    ::fast_io::stack<::pltxt2htm::details::OptimizerContext, ::fast_io::list<::pltxt2htm::details::OptimizerContext>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::OptimizerContext{::std::addressof(ast_init), ::pltxt2htm::NodeType::base, 0});

restart:
    auto&& ast = *(call_stack.top().ast);
    auto&& current_index = call_stack.top().current_index;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::tab: {
            break;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = reinterpret_cast<::pltxt2htm::Color*>(node.get_unsafe());

            // {
            //     // <color=red><color=blue>text</color></color> can be optimized
            //     auto&& subast = color->get_subast();
            //     if (subast.size() == 1) {
            //         auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe();
            //         if (subnode->node_type() == ::pltxt2htm::NodeType::pl_color) {
            //             (*color) = ::std::move(*reinterpret_cast<::pltxt2htm::Color*>(subnode));
            //         }
            //     }
            // }
            // auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            // bool const is_not_same_tag =
            //     (nested_tag_type != ::pltxt2htm::NodeType::pl_color &&
            //      nested_tag_type != ::pltxt2htm::NodeType::pl_a) ||
            //     color->get_color() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
            //                               call_stack.top().release_imul())
            //                               ->id_;
            call_stack.push(::pltxt2htm::details::OptimizerContext{::std::addressof(color->get_subast()),
                                                                   ::pltxt2htm::NodeType::pl_color, 0});
            goto restart;
        }
        // case ::pltxt2htm::NodeType::pl_experiment: {
        //     auto experiment = reinterpret_cast<::pltxt2htm::Experiment const*>(node.release_imul());
        //     if (experiment->get_subast().empty()) {
        //         // Optimization: if the tag is empty, we can skip it
        //         break;
        //     }
        //     {
        //         auto&& subast = experiment->get_subast();
        //         if (subast.size() == 1) {
        //             // <Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment> can
        //             be
        //             // optimized as <a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\"
        //             // internal>physicsLab</a>
        //             auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
        //             if (subnode->node_type() == ::pltxt2htm::NodeType::pl_experiment) {
        //                 experiment = reinterpret_cast<::pltxt2htm::Experiment const*>(subnode);
        //             }
        //         }
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     // Optimization: If the experiment is the same as the parent node, then ignore the nested tag.
        //     bool const is_not_same_tag =
        //         nested_tag_type != ::pltxt2htm::NodeType::pl_experiment ||
        //         experiment->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
        //                                     call_stack.top().release_imul())
        //                                     ->id_;
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
        //         experiment->get_subast(), ::pltxt2htm::NodeType::pl_experiment, is_not_same_tag, 0,
        //         ::fast_io::mnp::os_c_str(experiment->get_id())));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::pl_discussion: {
        //     auto discussion = reinterpret_cast<::pltxt2htm::Discussion const*>(node.release_imul());
        //     if (discussion->get_subast().empty()) {
        //         // Optimization: if the tag is empty, we can skip it
        //         break;
        //     }
        //     {
        //         auto&& subast = discussion->get_subast();
        //         if (subast.size() == 1) {
        //             // <Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion> can
        //             be
        //             // optimized as <a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\"
        //             // internal>physicsLab</a>
        //             auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
        //             if (subnode->node_type() == ::pltxt2htm::NodeType::pl_discussion) {
        //                 discussion = reinterpret_cast<::pltxt2htm::Discussion const*>(subnode);
        //             }
        //         }
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     // Optimization: If the discussion is the same as the parent node, then ignore the nested tag.
        //     bool const is_not_same_tag =
        //         nested_tag_type != ::pltxt2htm::NodeType::pl_discussion ||
        //         discussion->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
        //                                     call_stack.top().release_imul())
        //                                     ->id_;
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
        //         discussion->get_subast(), ::pltxt2htm::NodeType::pl_discussion, is_not_same_tag, 0,
        //         ::fast_io::mnp::os_c_str(discussion->get_id())));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::pl_user: {
        //     auto user = reinterpret_cast<::pltxt2htm::User const*>(node.release_imul());
        //     if (user->get_subast().empty()) {
        //         // Optimization: if the tag is empty, we can skip it
        //         break;
        //     }
        //     {
        //         auto&& subast = user->get_subast();
        //         if (subast.size() == 1) {
        //             // <User=123><user=642cf37a494746375aae306a>physicsLab</user></User> can be
        //             auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
        //             if (subnode->node_type() == ::pltxt2htm::NodeType::pl_user) {
        //                 user = reinterpret_cast<::pltxt2htm::User const*>(subnode);
        //             }
        //         }
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     // Optimization: If the user is the same as the parent node, then ignore the nested tag.
        //     bool const is_not_same_tag =
        //         nested_tag_type != ::pltxt2htm::NodeType::pl_user ||
        //         user->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
        //                               call_stack.top().release_imul())
        //                               ->id_;
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
        //         user->get_subast(), ::pltxt2htm::NodeType::pl_user, is_not_same_tag, 0,
        //         ::fast_io::mnp::os_c_str(user->get_id())));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::pl_size: {
        //     auto size = reinterpret_cast<::pltxt2htm::Size const*>(node.release_imul());
        //     if (size->get_subast().empty()) {
        //         // Optimization: if the tag is empty, we can skip it
        //         break;
        //     }
        //     {
        //         auto&& subast = size->get_subast();
        //         if (subast.size() == 1) {
        //             // <size=12><size=3>physicsLab</size></size> can be
        //             auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
        //             if (subnode->node_type() == ::pltxt2htm::NodeType::pl_size) {
        //                 size = reinterpret_cast<::pltxt2htm::Size const*>(subnode);
        //             }
        //         }
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     // Optimization: If the size is the same as the parent node, then ignore the nested tag.
        //     bool const is_not_same_tag =
        //         nested_tag_type != ::pltxt2htm::NodeType::pl_size ||
        //         size->get_id() != reinterpret_cast<::pltxt2htm::details::BackendPlSizeTagContext const*>(
        //                               call_stack.top().release_imul())
        //                               ->id_;
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendPlSizeTagContext>(
        //         size->get_subast(), ::pltxt2htm::NodeType::pl_size, is_not_same_tag, 0, size->get_id()));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_strong:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::pl_b: {
        //     auto b = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (b->get_subast().empty()) {
        //         // e.g. <b></b>
        //         // Which can be optimized(ignored)
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_b;
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         b->get_subast(), ::pltxt2htm::NodeType::pl_b, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_p: {
        //     auto p = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (p->get_subast().empty()) {
        //         // e.g. <p></p>
        //         // Which can be optimized(ignored)
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_p};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         p->get_subast(), ::pltxt2htm::NodeType::html_p, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::line_break:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::html_br: {
        //     break;
        // }
        // case ::pltxt2htm::NodeType::html_h1:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h1: {
        //     auto h1 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h1 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h1};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h1->get_subast(), ::pltxt2htm::NodeType::html_h1, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_h2:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h2: {
        //     auto h2 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h2 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h2};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h2->get_subast(), ::pltxt2htm::NodeType::html_h2, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_h3:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h3: {
        //     auto h3 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h3 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h3};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h3->get_subast(), ::pltxt2htm::NodeType::html_h3, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_h4:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h4: {
        //     auto h4 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h4 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h4};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h4->get_subast(), ::pltxt2htm::NodeType::html_h4, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_h5:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h5: {
        //     auto h5 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h5 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h5};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h5->get_subast(), ::pltxt2htm::NodeType::html_h5, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_h6:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::md_atx_h6: {
        //     auto h6 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // NOTE: Empty heading tag can NOT be ignored
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h6 &&
        //                                nested_tag_type != ::pltxt2htm::NodeType::md_atx_h6};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         h6->get_subast(), ::pltxt2htm::NodeType::html_h6, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_del: {
        //     auto del = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (del->get_subast().empty()) {
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_del};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         del->get_subast(), ::pltxt2htm::NodeType::html_del, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::pl_i:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::html_em: {
        //     auto em = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (em->get_subast().empty()) {
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_em};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         em->get_subast(), ::pltxt2htm::NodeType::html_em, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::md_hr:
        //     [[fallthrough]];
        // case ::pltxt2htm::NodeType::html_hr: {
        //     break;
        // }
        // case ::pltxt2htm::NodeType::html_note: {
        //     break;
        // }
        // case ::pltxt2htm::NodeType::html_ul: {
        //     auto ul = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (ul->get_subast().empty()) {
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_ul};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         ul->get_subast(), ::pltxt2htm::NodeType::html_ul, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_li: {
        //     auto li = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     if (li->get_subast().empty()) {
        //         break;
        //     }
        //     auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
        //     bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_li};
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        //         li->get_subast(), ::pltxt2htm::NodeType::html_li, is_not_same_tag, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_code: {
        //     auto code = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // Note: Despite `<code></code>` is empty, we still need to handle it
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendHtmlPreCodeContext>(
        //         code->get_subast(), ::pltxt2htm::NodeType::html_code, 0));
        //     ++current_index;
        //     goto restart;
        // }
        // case ::pltxt2htm::NodeType::html_pre: {
        //     auto pre = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
        //     // Note: Despite `<code></code>` is empty, we still need to handle it
        //     call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendHtmlPreCodeContext>(
        //         pre->get_subast(), ::pltxt2htm::NodeType::html_pre, 0));
        //     ++current_index;
        //     goto restart;
        // }
        case ::pltxt2htm::NodeType::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_tilde: {
            break;
        }
        case ::pltxt2htm::NodeType::base:
#if 0
            [[unlikely]] [[fallthrough]];
        default:
#endif
            [[unlikely]] {
                ::exception::unreachable<ndebug>();
            }
        }
    }

    {
        auto top_frame = ::std::move(call_stack.top());
        call_stack.pop();
        if (call_stack.empty()) {
            return;
        } else {
            switch (top_frame.nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                if (top_frame.ast->empty()) {
                    // Optimization: if the tag is empty, we can skip it
                    call_stack.top().ast->erase_index(call_stack.top().current_index);
                } else {
                    call_stack.top().current_index += 1;
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_em:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_i: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_p: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_del: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_ul: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_li: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_code: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_pre: {
                goto restart;
            }
            case ::pltxt2htm::NodeType::base:
                [[fallthrough]];
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
        }
    }
}

} // namespace pltxt2htm
