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

template<::pltxt2htm::Contracts ndebug>
class OptimizerContextVariant {
public:
    union {
        ::pltxt2htm::details::OptimizerContextWithoutInfo without_info;
        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo pl_size_tag;
    };

    ::pltxt2htm::NodeKind kind; ///< Type of the current nested tag context

    constexpr OptimizerContextVariant(::pltxt2htm::NodeKind const kind_) noexcept
        : without_info{},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(
        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo&& equal_sign_tag_context,
        ::pltxt2htm::NodeKind const kind_) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(
        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo&& pl_size_tag_context) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
          kind{::pltxt2htm::NodeKind::pl_size} {
    }

    constexpr OptimizerContextVariant(::pltxt2htm::details::OptimizerContextVariant<ndebug> const&) noexcept = delete;

    constexpr OptimizerContextVariant(::pltxt2htm::details::OptimizerContextVariant<ndebug>&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user: {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
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

    constexpr auto operator=(::pltxt2htm::details::OptimizerContextVariant<ndebug> const&) noexcept
        -> ::pltxt2htm::details::OptimizerContextVariant<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::OptimizerContextVariant<ndebug>& self,
                             ::pltxt2htm::details::OptimizerContextVariant<ndebug>&& other) noexcept
        -> ::pltxt2htm::details::OptimizerContextVariant<ndebug>& {
        pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"can not assign to self");
        self.~OptimizerContextVariant();
        ::std::construct_at(::std::addressof(self), ::std::move(other));
        return self;
    }
};

template<::std::forward_iterator Iter, ::pltxt2htm::Contracts ndebug>
class OptimizerFrameContext {
    ::pltxt2htm::details::OptimizerContextVariant<ndebug> context_data;

public:
    ::pltxt2htm::Ast<ndebug>* ast; ///< Pointer to the AST being optimized

#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#endif
    Iter iter; ///< Iterator pointing to the current position in the AST

    constexpr OptimizerFrameContext(::pltxt2htm::Ast<ndebug>* ast_, ::pltxt2htm::NodeKind const nested_tag_type_,
                                    Iter&& iter_) noexcept
        : context_data{nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrameContext(
        ::pltxt2htm::Ast<ndebug>* ast_, ::pltxt2htm::NodeKind const nested_tag_type_, Iter&& iter_,
        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo&& equal_sign_tag_context_) noexcept
        : context_data{::std::move(equal_sign_tag_context_), nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrameContext(
        ::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
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
        this ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>& self,
        ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug>&&) noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(
        this ::pltxt2htm::details::OptimizerFrameContext<Iter, ndebug> const& self) noexcept {
        return self.context_data.kind;
    }

    [[nodiscard]]
    constexpr auto get_equal_sign_tag_id(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id_);
    }

    [[nodiscard]]
    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        bool const is_pl_size_tag_type{context_data_ref.kind == ::pltxt2htm::NodeKind::pl_size};
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
constexpr void optimize_ast(::pltxt2htm::Ast<ndebug>& ast_init) noexcept {
    ::fast_io::stack<::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>{
        ::std::addressof(ast_init), ::pltxt2htm::NodeKind::text, ast_init.begin()});

entry:
    auto&& ast = *(::pltxt2htm::details::stack_top<ndebug>(call_stack).ast);
    auto&& current_iter = ::pltxt2htm::details::stack_top<ndebug>(call_stack).iter;
    while (current_iter != ast.end()) {
        auto&& node = *current_iter;

        switch (node.get_node_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::text: {
            auto&& subast = node.as_text().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::text, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            {
                // Optimization: <color=red><color=blue>text</color></color>
                // simplifies to <color=blue>text</color>.
                // The inner color attribute overrides the outer one.
                //
                // Optimization: <color=red><a>text</a></color>
                // simplifies to <a>text</a>.
                // The inner anchor tag's styling takes precedence over the outer color.
                auto&& subast = node.as_pl_color().get_subast();
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_color ||
                        subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_a) {
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
            ::pltxt2htm::NodeKind const nested_tag_type{
                ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type()};
            // Optimization: If this color matches the parent color, flatten the nesting
            // <color=red>text<color=red>text</color>test</color> -> <color=red>texttexttext</color>
            auto const is_different_tag = bool{[nested_tag_type, &call_stack, &node] constexpr noexcept {
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_color) {
                    return node.as_pl_color().get_color() !=
                           ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_equal_sign_tag_id();
                }
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_a) {
                    static constexpr auto anchor_color_literal = ::pltxt2htm::PlA<ndebug>::get_color_literal();
                    static constexpr auto anchor_color =
                        ::fast_io::u8string_view{anchor_color_literal.data(), anchor_color_literal.size()};
                    return node.as_pl_color().get_color() != anchor_color;
                }
                return true; // Different tag types, so not the same
            }()};
            if (is_different_tag) {
                auto&& subast = node.as_pl_color().get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                auto const& equal_sign_tag_id = node.as_pl_color().get_color();
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_color, subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                            ::fast_io::u8string_view{equal_sign_tag_id.data(), equal_sign_tag_id.size()}}));
                goto entry;
            }
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            node =
                ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(node.as_pl_color().get_subast())}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            {
                // Optimization: <a><color=blue>text</color></a>
                // can be simplified to <color=blue>text</color>
                // The inner color takes precedence over the outer color
                auto&& subast = node.as_pl_a().get_subast();
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_color) {
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
            ::pltxt2htm::NodeKind const nested_tag_type{
                ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type()};
            // Optimization: If this color matches the parent color, flatten the nesting
            // <a>text<a>text</a>text</a> -> <a>texttexttext</a>
            auto const is_different_tag = bool{[nested_tag_type, &call_stack] constexpr noexcept {
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_color) {
                    static constexpr auto anchor_color_literal = ::pltxt2htm::PlA<ndebug>::get_color_literal();
                    static constexpr auto anchor_color =
                        ::fast_io::u8string_view{anchor_color_literal.data(), anchor_color_literal.size()};
                    return anchor_color != ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_equal_sign_tag_id();
                }
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_a) {
                    return false;
                }
                return true; // Different tag types, so not the same
            }()};
            if (is_different_tag) {
                auto&& subast = node.as_pl_a().get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_a, subast.begin()));
                goto entry;
            }
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(node.as_pl_a().get_subast())}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion: {
            auto const is_experiment = node.get_node_kind() == ::pltxt2htm::NodeKind::pl_experiment;
            auto&& subast = is_experiment ? node.as_pl_experiment().get_subast() : node.as_pl_discussion().get_subast();
            if (subast.empty()) {
                ast.erase(current_iter);
                continue;
            }
            auto const& equal_sign_tag_id =
                is_experiment ? node.as_pl_experiment().get_id() : node.as_pl_discussion().get_id();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), node.get_node_kind(), subast.begin(),
                    ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                        ::fast_io::u8string_view{equal_sign_tag_id.data(), equal_sign_tag_id.size()}}));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            auto&& subast = node.as_pl_user().get_subast();
            {
                if (subast.empty()) {
                    // <user=123></user> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                if (subast.size() == 1) {
                    // <User=123><user=642cf37a494746375aae306a>physicsLab</user></User> can be
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_user) {
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
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            // Optimization: If the user is the same as the parent node, then ignore the nested tag.
            auto const& equal_sign_tag_id = node.as_pl_user().get_id();
            bool const is_different_tag =
                nested_tag_type != ::pltxt2htm::NodeKind::pl_user ||
                equal_sign_tag_id != ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_equal_sign_tag_id();
            if (is_different_tag) {
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_user, subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                            ::fast_io::u8string_view{equal_sign_tag_id.data(), equal_sign_tag_id.size()}}));
                goto entry;
            }
            node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            auto&& subast = node.as_pl_external().get_subast();
            if (subast.empty()) {
                // <external=123></external> can be omitted
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_external, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            auto&& subast = node.as_pl_size().get_subast();
            {
                if (subast.empty()) {
                    // <size=123></size> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                if (subast.size() == 1) {
                    // <size=12><size=3>physicsLab</size></size> can be
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_size) {
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
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            // Optimization: If the size is the same as the parent node, then ignore the nested tag.
            bool const is_different_tag = nested_tag_type != ::pltxt2htm::NodeKind::pl_size ||
                                          node.as_pl_size().get_size() !=
                                              ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_pl_size_tag_id();
            if (is_different_tag) {
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo{node.as_pl_size().get_size()}));
                goto entry;
            }
            node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b: {
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            bool const is_different_tag{!::pltxt2htm::details::is_strong_like(nested_tag_type)};
            auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                switch (node.get_node_kind()) {
                case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                    return node.as_md_double_emphasis_underscore().get_subast();
                }
                case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                    return node.as_md_double_emphasis_asterisk().get_subast();
                }
                case ::pltxt2htm::NodeKind::html_strong: {
                    return node.as_html_strong().get_subast();
                }
                case ::pltxt2htm::NodeKind::pl_b: {
                    return node.as_pl_b().get_subast();
                }
                default:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }
                }
            }();
            if (is_different_tag) {
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin()));
                goto entry;
            }
            node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            auto&& subast = node.as_html_p().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_p, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br: {
            while (current_iter != ast.begin()) {
                auto const node_type = (*(current_iter - 1)).get_node_kind();
                if (node_type != ::pltxt2htm::NodeKind::space && node_type != ::pltxt2htm::NodeKind::tab) {
                    break;
                }
                --current_iter;
                ast.erase(current_iter);
            }
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            auto&& subast = node.as_html_h1().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h1, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            auto&& subast = node.as_md_atx_h1().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h1, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            auto&& subast = node.as_html_h2().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h2, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            auto&& subast = node.as_md_atx_h2().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h2, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            auto&& subast = node.as_html_h3().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h3, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            auto&& subast = node.as_md_atx_h3().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h3, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            auto&& subast = node.as_html_h4().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h4, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            auto&& subast = node.as_md_atx_h4().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h4, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            auto&& subast = node.as_html_h5().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h5, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            auto&& subast = node.as_md_atx_h5().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h5, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            auto&& subast = node.as_html_h6().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h6, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            auto&& subast = node.as_md_atx_h6().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h6, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_del &&
                                        nested_tag_type != ::pltxt2htm::NodeKind::md_del};
            if (is_different_tag) {
                auto&& subast = node.as_md_del().get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_del, subast.begin()));
                goto entry;
            }
            node =
                ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(node.as_md_del().get_subast())}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_del &&
                                        nested_tag_type != ::pltxt2htm::NodeKind::md_del};
            if (is_different_tag) {
                auto&& subast = node.as_html_del().get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_del, subast.begin()));
                goto entry;
            }
            node =
                ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(node.as_html_del().get_subast())}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em: {
            auto&& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            bool const is_different_tag{!::pltxt2htm::details::is_em_like(nested_tag_type)};
            auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                switch (node.get_node_kind()) {
                case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                    return node.as_md_single_emphasis_underscore().get_subast();
                }
                case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                    return node.as_md_single_emphasis_asterisk().get_subast();
                }
                case ::pltxt2htm::NodeKind::pl_i: {
                    return node.as_pl_i().get_subast();
                }
                case ::pltxt2htm::NodeKind::html_em: {
                    return node.as_html_em().get_subast();
                }
                default:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }
                }
            }();
            if (is_different_tag) {
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push(
                    ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin()));
                goto entry;
            }
            node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
            ++current_iter;
            continue;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            ast.erase(current_iter);
            continue;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            auto&& subast = node.as_md_ul().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_ul, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            auto&& subast = node.as_html_ul().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_ul, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            auto&& subast = node.as_md_ol().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_ol, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            auto&& subast = node.as_html_ol().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_ol, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            auto&& subast = node.as_md_li_checkbox().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_li, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            auto&& subast = node.as_md_li().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_li, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            auto&& subast = node.as_html_li().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_li, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            auto&& subast = node.as_md_code_span_1_backtick().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_code, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            auto&& subast = node.as_md_code_span_2_backtick().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_code, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            auto&& subast = node.as_md_code_span_3_backtick().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_code, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            auto&& subast = node.as_html_code().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_code, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            auto&& subast = node.as_html_pre().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_pre, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            auto&& subast = node.as_html_blockquote().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_blockquote, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            auto&& subast = node.as_md_table().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_table, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            auto&& subast = node.as_html_table().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_table, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            auto&& subast = node.as_html_caption().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_caption, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            auto&& subast = node.as_html_colgroup().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_colgroup, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            auto&& subast = node.as_md_thead().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_thead, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            auto&& subast = node.as_html_thead().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_thead, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            auto&& subast = node.as_md_tbody().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_tbody, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            auto&& subast = node.as_html_tbody().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_tbody, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            auto&& subast = node.as_html_tfoot().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_tfoot, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            auto&& subast = node.as_md_tr().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_tr, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            auto&& subast = node.as_html_tr().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_tr, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            auto&& subast = node.as_md_th().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_th, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            auto&& subast = node.as_html_th().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_th, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            auto&& subast = node.as_md_td().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_td, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            auto&& subast = node.as_html_td().get_subast();
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), ::pltxt2htm::NodeKind::html_td, subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            auto&& subast = node.as_md_triple_emphasis_underscore().get_subast();
            bool const ast_not_empty = !subast.empty();
            pltxt2htm_assert(ast_not_empty, u8"md_triple_emphasis subast must not be empty");
            auto const& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            if (nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk ||
                nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore) {
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            if (::pltxt2htm::details::is_em_like(nested_tag_type)) {
                auto tmp = ::std::move(subast);
                *current_iter =
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>{::std::move(tmp)});
                continue;
            }
            if (::pltxt2htm::details::is_strong_like(nested_tag_type)) {
                auto tmp = ::std::move(subast);
                *current_iter =
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>{::std::move(tmp)});
                continue;
            }
            if (subast.empty()) {
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), node.get_node_kind(), subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            auto&& subast = node.as_md_triple_emphasis_asterisk().get_subast();
            bool const ast_not_empty = !subast.empty();
            pltxt2htm_assert(ast_not_empty, u8"md_triple_emphasis subast must not be empty");
            auto const& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            if (nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk ||
                nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore) {
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Text<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            if (::pltxt2htm::details::is_em_like(nested_tag_type)) {
                auto tmp = ::std::move(subast);
                *current_iter =
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>{::std::move(tmp)});
                continue;
            }
            if (::pltxt2htm::details::is_strong_like(nested_tag_type)) {
                auto tmp = ::std::move(subast);
                *current_iter =
                    ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>{::std::move(tmp)});
                continue;
            }
            if (subast.empty()) {
                ast.erase(current_iter);
                continue;
            }
            call_stack.push(
                ::pltxt2htm::details::OptimizerFrameContext<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                    ::std::addressof(subast), node.get_node_kind(), subast.begin()));
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_plus:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_comma:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_dot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_slash:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_colon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_question:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_at:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_caret:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            ++current_iter;
            continue;
        }
// All enum values are covered above; default is unreachable.
// #if 0 documents intent and keeps the formatter output clean.
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
    }

    while (current_iter != ast.begin()) {
        --current_iter;
        auto const node_type = (*current_iter).get_node_kind();
        if (node_type != ::pltxt2htm::NodeKind::space && node_type != ::pltxt2htm::NodeKind::tab) {
            break;
        }
        ast.erase(current_iter);
    }

    call_stack.pop();
    if (call_stack.empty() == false) {
        ++(::pltxt2htm::details::stack_top<ndebug>(call_stack).iter);
        goto entry;
    }

    return;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
