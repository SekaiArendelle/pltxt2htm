/**
 * @file optimizer.hh
 * @brief AST optimizer for pltxt2htm
 * @details Provides AST optimization functionality to simplify and optimize
 *          the Abstract Syntax Tree before HTML generation. This includes
 *          removing redundant tags, merging adjacent text nodes, and other
 *          optimizations to produce cleaner HTML output.
 */

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

/**
 * @brief Base context for AST optimization operations
 * @tparam Iter Iterator type for traversing the AST
 * @details This context maintains the state during AST optimization,
 *          tracking the current AST being processed, the type of nested tag,
 *          and the current iterator position
 * @var ast Pointer to the AST being optimized (input/output parameter)
 * @var nested_tag_type Type of the current nested tag context (input parameter)
 * @var iter Iterator pointing to the current position in the AST (input/output parameter)
 */
template<::std::forward_iterator Iter>
class OptimizerContext {
public:
    ::pltxt2htm::Ast* ast; ///< Pointer to the AST being optimized
    ::pltxt2htm::NodeType const nested_tag_type; ///< Type of the current nested tag context
#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#endif
    Iter iter; ///< Iterator pointing to the current position in the AST

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

/**
 * @brief Optimization context for tags with equal-sign attributes
 * @tparam Iter Iterator type for traversing the AST
 * @details Specialized context for optimizing tags that have attributes
 *          in the format key=value, such as <color=red> or <experiment=123>
 * @var id_ The value part of the attribute (e.g., "red" in color=red) (input parameter)
 */
template<::std::forward_iterator Iter>
class OptimizerEqualSignTagContext : public ::pltxt2htm::details::OptimizerContext<Iter> {
public:
    ::fast_io::u8string_view id_; ///< The value part of the attribute (e.g., "red" in color=red)

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

/**
 * @brief Optimization context for size tags with numeric values
 * @tparam Iter Iterator type for traversing the AST
 * @details Specialized context for optimizing size tags that have
 *          numeric values like <size=12> where the value is a std::size_t
 * @var id_ Numeric size value (e.g., 12 in size=12) (input parameter)
 */
template<::std::forward_iterator Iter>
class OptimizerPlSizeTagContext : public ::pltxt2htm::details::OptimizerContext<Iter> {
public:
    ::std::size_t id_; ///< Numeric size value (e.g., 12 in size=12)

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

/**
 * @brief Optimize an Abstract Syntax Tree (AST)
 * @details This function performs various optimizations on the AST to produce
 *          cleaner and more efficient HTML output. Optimizations include:
 *          - Removing redundant nested tags of the same type
 *          - Merging adjacent text nodes when possible
 *          - Eliminating empty formatting tags
 *          - Simplifying nested color/size tags with identical values
 * @tparam ndebug Debug mode flag - controls assertion behavior
 * @param[in,out] ast_init The AST to optimize (modified in-place)
 * @note This function modifies the input AST directly
 * @warning The optimization process is recursive and uses manual stack management
 *          to avoid stack overflow with deeply nested structures
 */
template<bool ndebug>
constexpr void optimize_ast(::pltxt2htm::Ast& ast_init) noexcept {
    ::fast_io::stack<
        ::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>,
        ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>>>
        call_stack{};
    call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>{
        ::std::addressof(ast_init), ::pltxt2htm::NodeType::base, ast_init.begin()});

entry:
    auto&& ast = *(call_stack.top()->ast);
    auto&& current_iter = call_stack.top()->iter;
    while (current_iter != ast.end()) {
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
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeType::text: {
            auto text = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = text->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::text, subast.begin()));
            goto entry;
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
                goto entry;
            } else {
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(color->get_subast())));
                ++current_iter;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(experiment->get_subast())));
                ++current_iter;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(discussion->get_subast())));
                ++current_iter;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(user->get_subast())));
                ++current_iter;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(size->get_subast())));
                ++current_iter;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(b->get_subast())));
                ++current_iter;
                continue;
            }
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto p = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = p->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_p, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            while (current_iter != ast.begin()) {
                auto node_type = (*(current_iter - 1))->node_type();
                if (node_type != ::pltxt2htm::NodeType::space && node_type != ::pltxt2htm::NodeType::tab) {
                    break;
                }
                --current_iter;
                ast.erase(current_iter);
            }
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            // NOTE: All optimization to h1 has side effect
            auto&& subast = h1->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h1, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto h2 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h2->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h2, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto h3 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h3->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h3, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto h4 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h4->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h4, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto h5 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h5->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h5, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto h6 = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = h6->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_h6, subast.begin()));
            goto entry;
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
                goto entry;
            } else {
                node = static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::Text>(::std::move(del->get_subast())));
                ++current_iter;
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
                ++current_iter;
                continue;
            }
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr: {
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeType::html_note: {
            ast.erase(current_iter);
            continue;
        }
        case ::pltxt2htm::NodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul: {
            // ul tag can't impl nested tag erasing
            auto ul = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = ul->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_ul, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol: {
            // ol tag can't impl nested tag erasing
            auto ol = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = ol->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_ol, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li: {
            // li tag can't impl nested tag erasing
            auto li = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = li->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_li, subast.begin()));
            goto entry;
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
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_pre: {
            // pre tag can't impl nested tag erasing
            auto pre = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = pre->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_pre, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_blockquote: {
            // pre tag can't impl nested tag erasing
            auto blockquote = static_cast<::pltxt2htm::details::PairedTagBase*>(node.get_unsafe());
            auto&& subast = blockquote->get_subast();
            call_stack.push(::pltxt2htm::HeapGuard<::pltxt2htm::details::OptimizerContext<::pltxt2htm::Ast::iterator>>(
                ::std::addressof(subast), ::pltxt2htm::NodeType::html_blockquote, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_link: {
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeType::md_block_quotes:
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
            ++current_iter;
            continue;
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
            while (current_iter != ast.begin()) {
                --current_iter;
                auto node_type = (*current_iter)->node_type();
                if (node_type != ::pltxt2htm::NodeType::space && node_type != ::pltxt2htm::NodeType::tab) {
                    break;
                }
                ast.erase(current_iter);
            }
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
                goto entry;
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
            case ::pltxt2htm::NodeType::md_latex_inline:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_latex_block:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_pre:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_li:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_li:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_ul:
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
