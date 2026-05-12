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
#include <memory>
#include <type_traits>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include "ast/ast.hh"
#include "contracts.hh"
#include "details/utils.hh"
#include "details/push_macro.hh"

namespace pltxt2htm {

namespace details {

class OptimizerContextWithoutInfo {};

/**
 * @brief Base context for AST optimization operations
 * @tparam Iter Iterator type for traversing the AST
 * @details This context maintains the state during AST optimization,
 *          tracking the current AST being processed, the type of nested tag,
 *          and the current iterator position.
 */
class OptimizerContextWithEqualSignTagInfo {
public:
    ::fast_io::u8string_view id_; ///< The value part of the attribute (e.g., "red" in color=red)
};

class OptimizerContextWithPlSizeTagInfo {
public:
    ::std::size_t id_; ///< Numeric size value (e.g., 12 in size=12)
};

class OptimizerContextVariant {
public:
    union {
        ::pltxt2htm::details::OptimizerContextWithoutInfo without_info;
        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo pl_size_tag;
    };

    ::pltxt2htm::NodeType kind; ///< Type of the current nested tag context

    constexpr OptimizerContextVariant(::pltxt2htm::NodeType const kind_) noexcept
        : without_info{},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(
        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo&& equal_sign_tag_context,
        ::pltxt2htm::NodeType const kind_) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(
        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo&& pl_size_tag_context) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
          kind{::pltxt2htm::NodeType::pl_size} {
    }

    constexpr OptimizerContextVariant(OptimizerContextVariant const&) noexcept = delete;

    constexpr OptimizerContextVariant(OptimizerContextVariant&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) {
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_user: {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        default:
            ::std::construct_at(::std::addressof(this->without_info), ::std::move(other.without_info));
            return;
        }
    }

    static_assert(::std::is_trivially_destructible_v<decltype(without_info)>);
    static_assert(::std::is_trivially_destructible_v<decltype(equal_sign_tag)>);
    static_assert(::std::is_trivially_destructible_v<decltype(pl_size_tag)>);

    constexpr ~OptimizerContextVariant() noexcept = default;

    constexpr auto operator=(OptimizerContextVariant const&) noexcept -> OptimizerContextVariant& = delete;

    constexpr auto operator=(OptimizerContextVariant&& other) noexcept -> OptimizerContextVariant& {
#if 0
        if (this == ::std::addressof(other)) {
            ::exception::terminate();
        }
#endif
        this->~OptimizerContextVariant();
        new (this) OptimizerContextVariant(::std::move(other));
        return *this;
    }
};

template<::std::forward_iterator Iter, ::pltxt2htm::Contracts ndebug>
class OptimizerFrameContext {
    ::pltxt2htm::details::OptimizerContextVariant context_data;

public:
    ::pltxt2htm::ast2::Ast<ndebug>* ast; ///< Pointer to the AST being optimized

#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#endif
    Iter iter; ///< Iterator pointing to the current position in the AST

    OptimizerFrameContext(::pltxt2htm::ast2::Ast<ndebug>* ast_, ::pltxt2htm::NodeType const nested_tag_type_,
                          Iter&& iter_) noexcept
        : context_data{nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    OptimizerFrameContext(::pltxt2htm::ast2::Ast<ndebug>* ast_, ::pltxt2htm::NodeType const nested_tag_type_,
                          Iter&& iter_,
                          ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo&& equal_sign_tag_context_) noexcept
        : context_data{::std::move(equal_sign_tag_context_), nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    OptimizerFrameContext(::pltxt2htm::ast2::Ast<ndebug>* ast_, Iter&& iter_,
                          ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo&& pl_size_tag_context_) noexcept
        : context_data{::std::move(pl_size_tag_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrameContext(::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug> const&) noexcept = delete;

    constexpr OptimizerFrameContext(::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>&&) noexcept = default;

    constexpr ~OptimizerFrameContext() noexcept = default;

    constexpr ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>& operator=(
        ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug> const&) noexcept = delete;

    constexpr ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>& operator=(
        ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>&&) noexcept = default;

    constexpr auto get_nested_tag_type(
        this ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug> const& self) noexcept {
        return self.context_data.kind;
    }

    constexpr auto get_equal_sign_tag_id(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id_);
    }

    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return context_data_ref.pl_size_tag.id_;
    }
};

} // namespace details

/**
 * @brief Optimize an Abstract Syntax Tree (AST)
 * @details This function performs various optimizations on the AST to produce
 *          cleaner and more efficient HTML output. The optimization process
 *          traverses the entire AST and applies multiple optimization rules.
 *
 *          Key optimizations performed:
 *          - **Redundant tag removal**: Nested tags of the same type with identical
 *            attributes are merged (e.g., &lt;color=red&gt;&lt;color=blue&gt;text&lt;/color&gt;&lt;/color&gt;
 *            becomes &lt;color=blue&gt;text&lt;/color&gt;)
 *          - **Empty tag elimination**: Tags with empty content are removed entirely
 *          - **Text node merging**: Adjacent text nodes are combined when possible
 *          - **Nested tag flattening**: Deeply nested structures are simplified
 *          - **Whitespace normalization**: Excessive whitespace around line breaks is trimmed
 *
 *          The optimization uses a manual stack-based approach to handle deeply nested
 *          structures without risking stack overflow.
 * @tparam ndebug Contract checking mode. Use `::pltxt2htm::Contracts::quick_enforce` to enforce checks or
 * `::pltxt2htm::Contracts::ignore` to skip them.
 * @param[in,out] ast_init The AST to optimize (modified in-place)
 * @note This function modifies the input AST directly - the original structure is lost
 * @warning The optimization process is recursive and uses manual stack management
 *          to avoid stack overflow with deeply nested structures
 * @warning Some optimizations may change the semantic meaning of the output
 * @see pltxt2htm::details::OptimizerContext for the optimization context structure
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void optimize_ast(::pltxt2htm::ast2::Ast<ndebug>& ast_init) noexcept {
    ::fast_io::stack<
        ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>>
        call_stack{};
    call_stack.push(
        ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>{
            ::std::addressof(ast_init), ::pltxt2htm::NodeType::text, ast_init.begin()});

entry:
    auto&& ast = *(call_stack.top().ast);
    auto&& current_iter = call_stack.top().iter;
    while (current_iter != ast.end()) {
        auto&& node = *current_iter;

        switch (node.get_node_kind()) {
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
        case ::pltxt2htm::NodeType::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_coauthors: {
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeType::text: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::text, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_color: {
            {
                // Optimization: <color=red><color=blue>text</color></color>
                // simplifies to <color=blue>text</color>.
                // The inner color attribute overrides the outer one.
                //
                // Optimization: <color=red><a>text</a></color>
                // simplifies to <a>text</a>.
                // The inner anchor tag's styling takes precedence over the outer color.
                auto&& subast = node.get_subast();
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeType::pl_color ||
                        subnode.get_node_kind() == ::pltxt2htm::NodeType::pl_a) {
                        // SAFETY: We must NOT write `(*current_iter) = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `(*current_iter).get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `*current_iter`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        (*current_iter) = ::std::move(tmp);
                        continue;
                    }
                }
            }
            ::pltxt2htm::NodeType const nested_tag_type{call_stack.top().get_nested_tag_type()};
            // Optimization: If this color matches the parent color, flatten the nesting
            // <color=red>text<color=red>text</color>test</color> -> <color=red>texttexttext</color>
            auto const is_not_same_tag = bool{[nested_tag_type, &call_stack, &node] constexpr noexcept {
                if (nested_tag_type == ::pltxt2htm::NodeType::pl_color) {
                    return node.get_equal_sign_tag_id() != call_stack.top().get_equal_sign_tag_id();
                }
                else if (nested_tag_type == ::pltxt2htm::NodeType::pl_a) {
                    constexpr auto anchor_color_literal = ::pltxt2htm::ast2::PlA<ndebug>::get_color_literal();
                    // TODO Avoid constructing a temporary u8string_view on each comparison
                    auto const anchor_color =
                        ::fast_io::u8string_view{anchor_color_literal.data(), anchor_color_literal.size()};
                    return node.get_equal_sign_tag_id() != anchor_color;
                }
                else {
                    return true; // Different tag types, so not the same
                }
            }()};
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::pl_color, subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                            ::fast_io::mnp::os_c_str(node.get_equal_sign_tag_id())}));
                goto entry;
            }
            else {
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
                ++current_iter;
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_a: {
            {
                // Optimization: <a><color=blue>text</color></a>
                // can be simplified to <color=blue>text</color>
                // The inner color takes precedence over the outer color
                auto&& subast = node.get_subast();
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeType::pl_color) {
                        // SAFETY: We must NOT write `(*current_iter) = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `(*current_iter).get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `*current_iter`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        (*current_iter) = ::std::move(tmp);
                        continue;
                    }
                }
            }
            ::pltxt2htm::NodeType const nested_tag_type{call_stack.top().get_nested_tag_type()};
            // Optimization: If this color matches the parent color, flatten the nesting
            // <a>text<a>text</a>text</a> -> <a>texttexttext</a>
            auto const is_not_same_tag = bool{[nested_tag_type, &call_stack] constexpr noexcept {
                if (nested_tag_type == ::pltxt2htm::NodeType::pl_color) {
                    constexpr auto anchor_color_literal = ::pltxt2htm::ast2::PlA<ndebug>::get_color_literal();
                    // TODO Avoid constructing a temporary u8string_view on each comparison
                    auto const anchor_color =
                        ::fast_io::u8string_view{anchor_color_literal.data(), anchor_color_literal.size()};
                    return anchor_color != call_stack.top().get_equal_sign_tag_id();
                }
                else if (nested_tag_type == ::pltxt2htm::NodeType::pl_a) {
                    return false;
                }
                else {
                    return true; // Different tag types, so not the same
                }
            }()};
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(::std::addressof(subast),
                                                                        ::pltxt2htm::NodeType::pl_a, subast.begin()));
                goto entry;
            }
            else {
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
                ++current_iter;
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto&& subast = node.get_subast();
            if (subast.empty()) {
                // <experiment=123></experiment> can be omitted
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_experiment, subast.begin(),
                    ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                        ::fast_io::mnp::os_c_str(node.get_equal_sign_tag_id())}));
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto&& subast = node.get_subast();
            if (subast.empty()) {
                // <discussion=123></discussion> can be omitted
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_discussion, subast.begin(),
                    ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{::fast_io::u8string_view{
                        node.get_equal_sign_tag_id().data(), node.get_equal_sign_tag_id().size()}}));
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_user: {
            {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    // <user=123></user> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                else if (subast.size() == 1) {
                    // <User=123><user=642cf37a494746375aae306a>physicsLab</user></User> can be
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeType::pl_user) {
                        // SAFETY: We must NOT write `node = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `node.get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `node`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        node = ::std::move(tmp);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top().get_nested_tag_type();
            // Optimization: If the user is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_user ||
                                         node.get_equal_sign_tag_id() != call_stack.top().get_equal_sign_tag_id();
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::pl_user, subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                            ::fast_io::mnp::os_c_str(node.get_equal_sign_tag_id())}));
                goto entry;
            }
            else {
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
                ++current_iter;
                continue;
            }
        }
        case ::pltxt2htm::NodeType::pl_external: {
            auto&& subast = node.get_subast();
            if (subast.empty()) {
                // <external=123></external> can be omitted
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::pl_external, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    // <size=123></size> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                else if (subast.size() == 1) {
                    // <size=12><size=3>physicsLab</size></size> can be
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeType::pl_size) {
                        // SAFETY: We must NOT write `node = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `node.get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `node`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        node = ::std::move(tmp);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top().get_nested_tag_type();
            // Optimization: If the size is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_size ||
                                         node.get_pl_size_tag_id() != call_stack.top().get_pl_size_tag_id();
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo{node.get_pl_size_tag_id()}));
                goto entry;
            }
            else {
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
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
            auto&& nested_tag_type = call_stack.top().get_nested_tag_type();
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::pl_b &&
                                       nested_tag_type != ::pltxt2htm::NodeType::html_strong &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_double_emphasis_asterisk &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_double_emphasis_underscore};
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    // <b></b> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_strong, subast.begin()));
                goto entry;
            }
            else {
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
                ++current_iter;
                continue;
            }
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_p, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            while (current_iter != ast.begin()) {
                auto const node_type = (*(current_iter - 1)).get_node_kind();
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
            // NOTE: All optimization to h1 has side effect
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h1, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h2, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h3, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h4, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h5, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_h6, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del: {
            auto&& nested_tag_type = call_stack.top().get_nested_tag_type();
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_del &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_del};
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_del, subast.begin()));
                goto entry;
            }
            else {
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
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
            auto&& nested_tag_type = call_stack.top().get_nested_tag_type();
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_em &&
                                       nested_tag_type != ::pltxt2htm::NodeType::pl_i &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_single_emphasis_asterisk &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_single_emphasis_underscore};
            if (is_not_same_tag) {
                auto&& subast = node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator,
                                                                ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeType::html_em, subast.begin()));
            }
            else {
                node = ::pltxt2htm::ast2::PlTxtNode<ndebug>{
                    ::pltxt2htm::ast2::Text<ndebug>{::std::move(node.get_subast())}};
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
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_ul, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol: {
            // ol tag can't impl nested tag erasing
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_ol, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li: {
            // li tag can't impl nested tag erasing
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
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
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_code, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_pre: {
            // pre tag can't impl nested tag erasing
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_pre, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_blockquote: {
            // pre tag can't impl nested tag erasing
            auto&& subast = node.get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::ast2::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeType::html_blockquote, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_image:
            [[fallthrough]];
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
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    while (current_iter != ast.begin()) {
        --current_iter;
        auto const node_type = (*current_iter).get_node_kind();
        if (node_type != ::pltxt2htm::NodeType::space && node_type != ::pltxt2htm::NodeType::tab) {
            break;
        }
        ast.erase(current_iter);
    }

    call_stack.pop();
    if (call_stack.empty() == false) {
        ++(call_stack.top().iter);
        goto entry;
    }

    return;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
