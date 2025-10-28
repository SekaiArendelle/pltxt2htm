#pragma once

#include <cstddef>
#include <iterator>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include "pltxt2htm/astnode/node_type.hh"
#include "details/utils.hh"
#include "heap_guard.hh"
#include "astnode/basic.hh"
#include "astnode/node_type.hh"
#include "astnode/physics_lab_node.hh"

namespace pltxt2htm {

namespace details {

template<::std::forward_iterator Iter>
class OptimizerContext {
public:
    ::pltxt2htm::Ast* ast;
    ::pltxt2htm::NodeType const nested_tag_type;
    [[no_unique_address]]
    Iter iter;

    OptimizerContext(::pltxt2htm::Ast* ast_, ::pltxt2htm::NodeType const nested_tag_type_, Iter&& iter_) noexcept
        : ast(ast_),
          nested_tag_type{nested_tag_type_},
          iter{iter_} {
    }

    constexpr OptimizerContext(::pltxt2htm::details::OptimizerContext<Iter> const&) noexcept = default;

    constexpr OptimizerContext(::pltxt2htm::details::OptimizerContext<Iter>&&) noexcept = default;

    constexpr ~OptimizerContext() noexcept = default;

    constexpr ::pltxt2htm::details::OptimizerContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerContext<Iter> const&) noexcept = default;

    constexpr ::pltxt2htm::details::OptimizerContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerContext<Iter>&&) noexcept = default;
};

template<::std::forward_iterator Iter>
class OptimizerEqualSignTagContext : public ::pltxt2htm::details::OptimizerContext<Iter> {
public:
    ::fast_io::u8string_view id_;

    OptimizerEqualSignTagContext(::pltxt2htm::Ast* ast, ::pltxt2htm::NodeType const nested_tag_type, Iter&& iter,
                                 ::fast_io::u8string_view id) noexcept
        : ::pltxt2htm::details::OptimizerContext<Iter>(ast, nested_tag_type, ::std::move(iter)),
          id_{id} {
    }

    constexpr OptimizerEqualSignTagContext(::pltxt2htm::details::OptimizerEqualSignTagContext<Iter> const&) noexcept =
        default;
    constexpr OptimizerEqualSignTagContext(::pltxt2htm::details::OptimizerEqualSignTagContext<Iter>&&) noexcept =
        default;

    constexpr ~OptimizerEqualSignTagContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::OptimizerEqualSignTagContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerEqualSignTagContext<Iter> const&) noexcept = delete;
    constexpr ::pltxt2htm::details::OptimizerEqualSignTagContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerEqualSignTagContext<Iter>&&) noexcept = delete;
};

template<::std::forward_iterator Iter>
class OptimizerPlSizeTagContext : public ::pltxt2htm::details::OptimizerContext<Iter> {
public:
    ::std::size_t id_;

    OptimizerPlSizeTagContext(::pltxt2htm::Ast* ast, ::pltxt2htm::NodeType const nested_tag_type, Iter&& iter,
                              ::std::size_t id) noexcept
        : ::pltxt2htm::details::OptimizerContext<Iter>(ast, nested_tag_type, ::std::move(iter)),
          id_{id} {
    }

    constexpr OptimizerPlSizeTagContext(::pltxt2htm::details::OptimizerPlSizeTagContext<Iter> const&) noexcept =
        default;

    constexpr OptimizerPlSizeTagContext(::pltxt2htm::details::OptimizerPlSizeTagContext<Iter>&&) noexcept = default;

    constexpr ~OptimizerPlSizeTagContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::OptimizerPlSizeTagContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerPlSizeTagContext<Iter> const&) noexcept = delete;

    constexpr ::pltxt2htm::details::OptimizerPlSizeTagContext<Iter>& operator=(
        ::pltxt2htm::details::OptimizerPlSizeTagContext<Iter>&&) noexcept = delete;
};

} // namespace details

template<bool ndebug>
constexpr void optimize_ast(::pltxt2htm::Ast& ast_init) noexcept {
    ::fast_io::stack<
        ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>,
        ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>>>
        call_stack{};
    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>{
        ::std::addressof(ast_init), ::pltxt2htm::NodeType::base, ast_init.begin()});

restart:
    auto&& ast = *(call_stack.top()->ast);
    auto&& current_iter = call_stack.top()->iter;
    for (; current_iter != ast.end(); ++current_iter) {
        auto&& node = *current_iter;

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
        case ::pltxt2htm::NodeType::text: {
            auto text = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = text->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::text, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = static_cast<::pltxt2htm::Color*>(node.get_unsafe());
            {
                // <color=red><color=blue>text</color></color> can be optimized
                auto&& subast = color->get_subast();
                if (subast.size() == 1) {
                    ::pltxt2htm::PlTxtNode* psubnode{::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe()};
                    if (psubnode->node_type() == ::pltxt2htm::NodeType::pl_color) {
                        auto subnode = ::std::move(*static_cast<::pltxt2htm::Color*>(psubnode));
                        (*color) = ::std::move(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                (nested_tag_type != ::pltxt2htm::NodeType::pl_color &&
                 nested_tag_type != ::pltxt2htm::NodeType::pl_a) ||
                color->get_color() !=
                    static_cast<::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator> const*>(
                        call_stack.top().release_imul())
                        ->id_;
            if (is_not_same_tag) {
                auto&& subast = color->get_subast();
                call_stack.push(::pltxt2htm::HeapGuard<
                                ::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator>>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_color, subast.begin(),
                    ::fast_io::mnp::os_c_str(color->get_color())));
                goto restart;
            } else {
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(color->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto experiment = static_cast<::pltxt2htm::Experiment*>(node.get_unsafe());
            {
                auto&& subast = experiment->get_subast();
                if (subast.size() == 1) {
                    // <Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment> can be
                    // optimized as <a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\"
                    // internal>physicsLab</a>
                    auto psubnode = ::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe();
                    if (psubnode->node_type() == ::pltxt2htm::NodeType::pl_experiment) {
                        auto subnode = ::std::move(*static_cast<::pltxt2htm::Experiment*>(psubnode));
                        (*experiment) = ::std::move(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            // Optimization: If the experiment is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_experiment ||
                experiment->get_id() !=
                    static_cast<::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator> const*>(
                        call_stack.top().release_imul())
                        ->id_;
            if (is_not_same_tag) {
                auto&& subast = experiment->get_subast();
                call_stack.push(::pltxt2htm::HeapGuard<
                                ::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator>>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_experiment, subast.begin(),
                    ::fast_io::mnp::os_c_str(experiment->get_id())));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(experiment->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto discussion = static_cast<::pltxt2htm::Discussion*>(node.get_unsafe());
            {
                auto&& subast = discussion->get_subast();
                if (subast.size() == 1) {
                    // <Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion>
                    // can be
                    // optimized as <a
                    // href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\"
                    // internal>physicsLab</a>
                    auto psubnode = ::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe();
                    if (psubnode->node_type() == ::pltxt2htm::NodeType::pl_discussion) {
                        auto subnode = ::std::move(*static_cast<::pltxt2htm::Discussion*>(psubnode));
                        (*discussion) = ::std::move(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            // Optimization: If the discussion is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_discussion ||
                discussion->get_id() !=
                    static_cast<::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator> const*>(
                        call_stack.top().release_imul())
                        ->id_;
            if (is_not_same_tag) {
                auto&& subast = discussion->get_subast();
                call_stack.push(::pltxt2htm::HeapGuard<
                                ::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator>>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_discussion, subast.begin(),
                    ::fast_io::mnp::os_c_str(discussion->get_id())));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(discussion->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_user: {
            auto user = static_cast<::pltxt2htm::User*>(node.get_unsafe());
            {
                auto&& subast = user->get_subast();
                if (subast.size() == 1) {
                    // <User=123><user=642cf37a494746375aae306a>physicsLab</user></User> can be
                    auto psubnode = ::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe();
                    if (psubnode->node_type() == ::pltxt2htm::NodeType::pl_user) {
                        auto subnode = ::std::move(*static_cast<::pltxt2htm::User*>(psubnode));
                        (*user) = ::std::move(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            // Optimization: If the user is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_user ||
                user->get_id() !=
                    static_cast<::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator> const*>(
                        call_stack.top().get_unsafe())
                        ->id_;
            if (is_not_same_tag) {
                auto&& subast = user->get_subast();
                call_stack.push(::pltxt2htm::HeapGuard<
                                ::pltxt2htm::details::OptimizerEqualSignTagContext<::pltxt2htm::Ast::iterator>>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_user, subast.begin(),
                    ::fast_io::mnp::os_c_str(user->get_id())));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(user->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_size: {
            auto size = static_cast<::pltxt2htm::Size*>(node.get_unsafe());
            {
                auto&& subast = size->get_subast();
                if (subast.size() == 1) {
                    // <size=12><size=3>physicsLab</size></size> can be
                    auto psubnode = ::pltxt2htm::details::vector_front<ndebug>(subast).get_unsafe();
                    if (psubnode->node_type() == ::pltxt2htm::NodeType::pl_size) {
                        auto subnode = ::std::move(*static_cast<::pltxt2htm::Size*>(psubnode));
                        (*size) = ::std::move(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            // Optimization: If the size is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_size ||
                size->get_id() !=
                    static_cast<::pltxt2htm::details::OptimizerPlSizeTagContext<::pltxt2htm::Ast::iterator> const*>(
                        call_stack.top().release_imul())
                        ->id_;
            if (is_not_same_tag) {
                auto&& subast = size->get_subast();
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerPlSizeTagContext<::pltxt2htm::Ast::iterator>>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::pl_size, subast.begin(), size->get_id()));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(size->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_b: {
            auto b = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::pl_b &&
                                       nested_tag_type != ::pltxt2htm::NodeType::html_strong &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_double_emphasis_asterisk &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_double_emphasis_underscore};
            if (is_not_same_tag) {
                auto&& subast = b->get_subast();
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_strong, subast.begin()));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(b->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto p = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = p->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_p, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            break;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            // NOTE: All optimization to h1 has side effect
            auto&& subast = h1->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h1, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto h2 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h2->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h2, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto h3 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h3->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h3, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto h4 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h4->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h4, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto h5 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h5->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h5, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto h6 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h6->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h6, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del: {
            auto del = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_del &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_del};
            if (is_not_same_tag) {
                auto&& subast = del->get_subast();
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_del, subast.begin()));
                goto restart;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(del->get_subast())));
                continue;
            }
        }
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_em: {
            auto em = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& nested_tag_type = call_stack.top()->nested_tag_type;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_em &&
                                       nested_tag_type != ::pltxt2htm::NodeType::pl_i &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_single_emphasis_asterisk &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_single_emphasis_underscore};
            if (is_not_same_tag) {
                auto&& subast = em->get_subast();
                call_stack.push(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_em, subast.begin()));
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(em->get_subast())));
                continue;
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr: {
            break;
        }
        case ::pltxt2htm::NodeType::html_note: {
            // TODO remove note node
            break;
        }
        case ::pltxt2htm::NodeType::html_ul: {
            // ul tag can't impl nested tag erasing
            auto ul = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = ul->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_ul, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_li: {
            // li tag can't impl nested tag erasing
            auto li = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = li->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_li, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code: {
            // code tag can't impl nested tag erasing
            auto code = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = code->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_code, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_pre: {
            // pre tag can't impl nested tag erasing
            auto pre = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = pre->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_pre, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_blockquote: {
            // pre tag can't impl nested tag erasing
            auto blockquote = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = blockquote->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_blockquote, subast.begin()));
            goto restart;
        }
        case ::pltxt2htm::NodeType::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            [[fallthrough]]; // TODO optimization support for md_triple_emphasis
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            [[fallthrough]]; // TODO optimization support for md_triple_emphasis
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
        case ::pltxt2htm::NodeType::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_tilde: {
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
            switch (top_frame->nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_experiment:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_discussion:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_user:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_size:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_em:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_del: {
                if (top_frame->ast->empty()) {
                    // Optimization: if the tag is empty, we can skip it
                    call_stack.top()->ast->erase(call_stack.top()->iter);
                } else {
                    ++(call_stack.top()->iter);
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_blockquote:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_tilde:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_span_3_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_code:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_pre:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_li:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_p:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h4:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_h6: {
                // Above tag can't be optimized
                break;
            }
            default:
                // Rest of tags will never hit this branch
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
        }
    }
}

} // namespace pltxt2htm
