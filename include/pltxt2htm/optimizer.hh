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
#include "container/optional.hh"
#include "container/string_view.hh"
#include <fast_io/fast_io_dsal/list.h>
#include "details/call_stack.hh"
#include "ast/ast.hh"
#include "ast/value_unit.hh"
#include "ast/vertical_align_value.hh"
#include "contracts.hh"
#include "details/utils.hh"
#include "details/push_macro.hh"

namespace pltxt2htm {

namespace details {

/**
 * @brief Empty context for optimizer tags that carry no extra data.
 */
class OptimizerContextWithoutInfo {};

/**
 * @brief Context for optimizer tags with a string attribute (e.g., color=red).
 */
class OptimizerContextWithEqualSignTagInfo {
public:
    ::pltxt2htm::container::U8StringView id; ///< The value part of the attribute (e.g., "red" in color=red)
};

/**
 * @brief Context for optimizer <size=N> / <size=N%> / <size=Nem> tags.
 */
class OptimizerContextWithPlSizeTagInfo {
public:
    ::pltxt2htm::ValueWithUnit<double> value; ///< Font size value+unit (e.g., {12, px} in size=12)
};

/**
 * @brief Context for optimizer <voffset=N> tags.
 */
class OptimizerContextWithPlVoffsetTagInfo {
public:
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value; ///< Vertical offset value+unit (e.g., 5px in voffset=5)
};

/**
 * @brief Context for optimizer html_span frames, remembers color, font-size and vertical-align.
 */
template<::pltxt2htm::Contracts ndebug>
class OptimizerContextWithHtmlSpanInfo {
public:
    ::pltxt2htm::container::U8StringView color{};
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};
};

/**
 * @brief Context for optimizer html_mark frames, remembers the background-color.
 */
template<::pltxt2htm::Contracts ndebug>
class OptimizerContextWithHtmlMarkInfo {
public:
    ::pltxt2htm::container::U8StringView background_color{};
};

/**
 * @brief Context for optimizer pl_mark frames, remembers the background-color.
 */
template<::pltxt2htm::Contracts ndebug>
class OptimizerContextWithPlMarkInfo {
public:
    ::pltxt2htm::container::U8StringView background_color{};
};

/**
 * @brief Tagged-union variant of optimizer context payloads.
 * @details Dispatched on `kind` (::pltxt2htm::NodeKind) - used inside
 *          OptimizerFrame.
 */
template<::pltxt2htm::Contracts ndebug>
class OptimizerContextVariant {
public:
    union {
        OptimizerContextWithoutInfo without_info;
        OptimizerContextWithEqualSignTagInfo equal_sign_tag;
        OptimizerContextWithPlSizeTagInfo pl_size_tag;
        OptimizerContextWithPlVoffsetTagInfo pl_voffset_tag;
        OptimizerContextWithHtmlSpanInfo<ndebug> html_span_info;
        OptimizerContextWithHtmlMarkInfo<ndebug> html_mark_info;
        OptimizerContextWithPlMarkInfo<ndebug> pl_mark_info;
    };

    ::pltxt2htm::NodeKind kind; ///< Type of the current nested tag context

    constexpr OptimizerContextVariant(::pltxt2htm::NodeKind const kind_) noexcept
        : without_info{},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithEqualSignTagInfo equal_sign_tag_context,
                                      ::pltxt2htm::NodeKind const kind_) noexcept
        : equal_sign_tag{equal_sign_tag_context},
          kind{kind_} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithPlSizeTagInfo pl_size_tag_context) noexcept
        : pl_size_tag{pl_size_tag_context},
          kind{::pltxt2htm::NodeKind::pl_size} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithPlVoffsetTagInfo pl_voffset_tag_context) noexcept
        : pl_voffset_tag{pl_voffset_tag_context},
          kind{::pltxt2htm::NodeKind::pl_voffset} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithHtmlSpanInfo<ndebug>&& html_span_context) noexcept
        : html_span_info{::std::move(html_span_context)},
          kind{::pltxt2htm::NodeKind::html_span} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithHtmlMarkInfo<ndebug>&& html_mark_context) noexcept
        : html_mark_info{::std::move(html_mark_context)},
          kind{::pltxt2htm::NodeKind::html_mark} {
    }

    constexpr OptimizerContextVariant(OptimizerContextWithPlMarkInfo<ndebug>&& pl_mark_context) noexcept
        : pl_mark_info{::std::move(pl_mark_context)},
          kind{::pltxt2htm::NodeKind::pl_mark} {
    }

    constexpr OptimizerContextVariant(OptimizerContextVariant<ndebug> const&) noexcept = delete;

    constexpr OptimizerContextVariant(OptimizerContextVariant<ndebug>&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiments:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussions:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_trigger:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_internal: {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_voffset: {
            ::std::construct_at(::std::addressof(this->pl_voffset_tag), ::std::move(other.pl_voffset_tag));
            return;
        }
        case ::pltxt2htm::NodeKind::html_span: {
            ::std::construct_at(::std::addressof(this->html_span_info), ::std::move(other.html_span_info));
            return;
        }
        case ::pltxt2htm::NodeKind::html_mark: {
            ::std::construct_at(::std::addressof(this->html_mark_info), ::std::move(other.html_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::pl_mark: {
            ::std::construct_at(::std::addressof(this->pl_mark_info), ::std::move(other.pl_mark_info));
            return;
        }
        case ::pltxt2htm::NodeKind::group:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_align:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_margin:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_external:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_u:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_s:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_sub:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_div:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::list_li_checkbox:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::invalid_utf8:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than:
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
        case ::pltxt2htm::NodeKind::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_img:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::table_col:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_escape:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::code_fence:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block: {
            ::std::construct_at(::std::addressof(this->without_info), ::std::move(other.without_info));
            return;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in OptimizerContextVariant move constructor");
            }
#endif
        }
    }

    static_assert(::std::is_trivially_destructible_v<decltype(without_info)>);
    static_assert(::std::is_trivially_destructible_v<decltype(equal_sign_tag)>);
    static_assert(::std::is_trivially_destructible_v<decltype(pl_size_tag)>);
    static_assert(::std::is_trivially_destructible_v<decltype(pl_voffset_tag)>);
    static_assert(::std::is_trivially_destructible_v<decltype(html_span_info)>);
    static_assert(::std::is_trivially_destructible_v<decltype(html_mark_info)>);
    static_assert(::std::is_trivially_destructible_v<decltype(pl_mark_info)>);

    constexpr ~OptimizerContextVariant() noexcept = default;

    constexpr auto operator=(OptimizerContextVariant<ndebug> const&) noexcept
        -> OptimizerContextVariant<ndebug>& = delete;

    constexpr auto operator=(this OptimizerContextVariant<ndebug>& self,
                             OptimizerContextVariant<ndebug>&& other) noexcept -> OptimizerContextVariant<ndebug>& {
        pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"can not assign to self");
        self.~OptimizerContextVariant();
        ::std::construct_at(::std::addressof(self), ::std::move(other));
        return self;
    }
};

/**
 * @brief Activation record for the AST optimizer traversal.
 * @details Holds the current AST, a typed context variant, and the iterator
 *          into the AST being processed. Manually managed on a call-stack to
 *          avoid stack overflow on deeply nested input.
 * @tparam Iter Forward-iterator type over the AST (typically Ast::iterator).
 * @tparam ndebug Contract checking mode.
 */
template<::std::forward_iterator Iter, ::pltxt2htm::Contracts ndebug>
class OptimizerFrame {
    OptimizerContextVariant<ndebug> context_data;

public:
    ::pltxt2htm::Ast<ndebug>* ast; ///< Pointer to the AST being optimized

#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#endif
    Iter iter; ///< Iterator pointing to the current position in the AST

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, ::pltxt2htm::NodeKind const nested_tag_type_,
                             Iter&& iter_) noexcept
        : context_data{nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, ::pltxt2htm::NodeKind const nested_tag_type_, Iter&& iter_,
                             OptimizerContextWithEqualSignTagInfo equal_sign_tag_context_) noexcept
        : context_data{equal_sign_tag_context_, nested_tag_type_},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
                             OptimizerContextWithPlSizeTagInfo pl_size_tag_context_) noexcept
        : context_data{::std::move(pl_size_tag_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
                             OptimizerContextWithPlVoffsetTagInfo pl_voffset_tag_context_) noexcept
        : context_data{::std::move(pl_voffset_tag_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
                             OptimizerContextWithHtmlSpanInfo<ndebug>&& html_span_context_) noexcept
        : context_data{::std::move(html_span_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
                             OptimizerContextWithHtmlMarkInfo<ndebug>&& html_mark_context_) noexcept
        : context_data{::std::move(html_mark_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(::pltxt2htm::Ast<ndebug>* ast_, Iter&& iter_,
                             OptimizerContextWithPlMarkInfo<ndebug>&& pl_mark_context_) noexcept
        : context_data{::std::move(pl_mark_context_)},
          ast(ast_),
          iter{iter_} {
    }

    constexpr OptimizerFrame(OptimizerFrame<Iter, ndebug> const&) noexcept = delete;

    constexpr OptimizerFrame(OptimizerFrame<Iter, ndebug>&&) noexcept = default;

    constexpr ~OptimizerFrame() noexcept = default;

    constexpr OptimizerFrame<Iter, ndebug>& operator=(OptimizerFrame<Iter, ndebug> const&) noexcept = delete;

    constexpr OptimizerFrame<Iter, ndebug>& operator=(this OptimizerFrame<Iter, ndebug>& self,
                                                      OptimizerFrame<Iter, ndebug>&&) noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this OptimizerFrame<Iter, ndebug> const& self) noexcept {
        return self.context_data.kind;
    }

    [[nodiscard]]
    constexpr auto get_equal_sign_tag_id(this auto&& self) noexcept -> decltype(auto) {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind),
                         u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id);
    }

    [[nodiscard]]
    constexpr auto get_pl_size_tag_value(this auto const& self) noexcept -> ::pltxt2htm::ValueWithUnit<double> {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::pl_size, u8"context kind mismatch");
        return context_data_ref.pl_size_tag.value;
    }

    [[nodiscard]]
    constexpr auto get_pl_voffset_tag_value(this auto const& self) noexcept
        -> ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::pl_voffset, u8"context kind mismatch");
        return context_data_ref.pl_voffset_tag.value;
    }

    [[nodiscard]]
    constexpr auto get_html_span_color(this auto const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::html_span, u8"context kind mismatch");
        return context_data_ref.html_span_info.color;
    }

    [[nodiscard]]
    constexpr auto get_html_span_font_size(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::html_span, u8"context kind mismatch");
        return context_data_ref.html_span_info.font_size;
    }

    [[nodiscard]]
    constexpr auto get_html_span_vertical_align(this auto const& self) noexcept
        -> ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::html_span, u8"context kind mismatch");
        return context_data_ref.html_span_info.vertical_align;
    }

    [[nodiscard]]
    constexpr auto get_html_mark_background_color(this auto const& self) noexcept
        -> ::pltxt2htm::container::U8StringView {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::html_mark, u8"context kind mismatch");
        return context_data_ref.html_mark_info.background_color;
    }

    [[nodiscard]]
    constexpr auto get_pl_mark_background_color(this auto const& self) noexcept
        -> ::pltxt2htm::container::U8StringView {
        auto&& context_data_ref = self.context_data;
        pltxt2htm_assert(context_data_ref.kind == ::pltxt2htm::NodeKind::pl_mark, u8"context kind mismatch");
        return context_data_ref.pl_mark_info.background_color;
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
 *          - **Group node merging**: Adjacent group nodes are combined when possible
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
    ::pltxt2htm::details::CallStack<
        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>>
        call_stack{};
    call_stack.push_frame(::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>{
        ::std::addressof(ast_init), ::pltxt2htm::NodeKind::group, ast_init.begin()});

entry:
    while (true) {
        auto&& ast = *(call_stack.template current_frame<ndebug>().ast);
        auto&& current_iter = call_stack.template current_frame<ndebug>().iter;
        while (current_iter != ast.end()) {
            auto&& node = *current_iter;

            switch (node.get_node_kind()) /* -Werror=switch */ {
            case ::pltxt2htm::NodeKind::u8char:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::invalid_utf8:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_escape:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::space:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::single_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::double_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::less_than:
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
            case ::pltxt2htm::NodeKind::group: {
                auto&& active_node = node.as_group();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::group, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_color: {
                auto&& active_node = node.as_pl_color();
                auto&& subast = active_node.get_subast();

                // Optimization: <color=red><color=blue>text</color></color>
                // simplifies to <color=blue>text</color>.
                // The inner color attribute overrides the outer one.
                //
                // Optimization: <color=red><a>text</a></color>
                // simplifies to <a>text</a>.
                // The inner anchor tag's styling takes precedence over the outer color.
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_color ||
                        subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_a ||
                        subnode.get_node_kind() == ::pltxt2htm::NodeKind::html_span) {
                        // SAFETY: We must NOT write `node = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `node.get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `*current_iter`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        node = ::std::move(tmp);
                        continue;
                    }
                }

                ::pltxt2htm::NodeKind const nested_tag_type{
                    call_stack.template current_frame<ndebug>().get_nested_tag_type()};
                // Optimization: If this color matches the parent color, flatten the nesting
                // <color=red>text<color=red>text</color>test</color> -> <color=red>texttexttext</color>
                auto const is_different_tag = bool{[nested_tag_type, &call_stack, &active_node] constexpr noexcept {
                    if (nested_tag_type == ::pltxt2htm::NodeKind::pl_color) {
                        return active_node.get_color() !=
                               call_stack.template current_frame<ndebug>().get_equal_sign_tag_id();
                    }
                    if (nested_tag_type == ::pltxt2htm::NodeKind::pl_a) {
                        static constexpr auto anchor_color_literal = ::pltxt2htm::PlA<ndebug>::get_color_literal();
                        static constexpr auto anchor_color = ::pltxt2htm::container::U8StringView{anchor_color_literal};
                        return active_node.get_color() != anchor_color;
                    }
                    return true; // Different tag types, so not the same
                }()};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    auto const& equal_sign_tag_id = active_node.get_color();
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_color, subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                                ::pltxt2htm::container::U8StringView{equal_sign_tag_id}}));
                    goto entry;
                }
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto&& active_node = node.as_html_span();
                // Merge attributes rather than discarding the outer span.
                // Inner values override outer for conflicts; outer values not
                // set on inner are preserved (CSS cascading).
                // e.g. <span style="font-size:20px"><span style="color:red">t</span></span>
                //      -> <span style="color:red;font-size:20px">t</span>
                auto&& subast = active_node.get_subast();
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::html_span) {
                        auto&& active_subnode = subnode.as_html_span();
                        auto const& outer_color = active_node.get_color();
                        auto const outer_fs = active_node.get_font_size();
                        auto const outer_va = active_node.get_vertical_align();
                        auto const& inner_color = active_subnode.get_color();
                        auto const inner_fs = active_subnode.get_font_size();
                        auto const inner_va = active_subnode.get_vertical_align();
                        auto merged_color = ::fast_io::u8string{inner_color.empty() ? outer_color : inner_color};
                        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> merged_fs{
                            ::pltxt2htm::container::nullopt};
                        if (inner_fs.has_value()) {
                            merged_fs = inner_fs.template value<ndebug>();
                        }
                        else if (outer_fs.has_value()) {
                            merged_fs = outer_fs.template value<ndebug>();
                        }
                        ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> merged_va{
                            ::pltxt2htm::container::nullopt};
                        if (inner_va.has_value()) {
                            merged_va = inner_va.template value<ndebug>();
                        }
                        else if (outer_va.has_value()) {
                            merged_va = outer_va.template value<ndebug>();
                        }
                        // SAFETY: Move inner's subast to a temporary first to break aliasing.
                        // `subnode` is a reference into `node.get_subast()`.
                        auto inner_subast = ::std::move(active_subnode.get_subast());
                        node = ::pltxt2htm::PlTxtNode<ndebug>{
                            ::pltxt2htm::HtmlSpan<ndebug>{::std::move(inner_subast), ::std::move(merged_color),
                                                          ::std::move(merged_fs), ::std::move(merged_va)}};
                        continue;
                    }
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_color) {
                        auto const outer_fs = active_node.get_font_size();
                        auto const outer_va = active_node.get_vertical_align();
                        ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> merged_fs{
                            ::pltxt2htm::container::nullopt};
                        if (outer_fs.has_value()) {
                            merged_fs = outer_fs.template value<ndebug>();
                        }
                        auto inner_subast = ::std::move(subnode.as_pl_color().get_subast());
                        auto const& inner_color = subnode.as_pl_color().get_color();
                        node = ::pltxt2htm::PlTxtNode<ndebug>{
                            ::pltxt2htm::HtmlSpan<ndebug>{::std::move(inner_subast), ::fast_io::u8string{inner_color},
                                                          ::std::move(merged_fs), ::std::move(outer_va)}};
                        continue;
                    }
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_a) {
                        auto tmp = ::std::move(subnode);
                        node = ::std::move(tmp);
                        continue;
                    }
                }
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }

                // Optimization: If the same attrs as parent html_span or color tag, flatten.
                // <span style="color:red">a<span style="color:red">b</span>c</span>
                // -> <span style="color:red">abc</span>
                // <color=red>a<span style="color:red">b</span>c</color>
                // -> <span style="color:red">abc</span>
                ::pltxt2htm::NodeKind const nested_tag_type{
                    call_stack.template current_frame<ndebug>().get_nested_tag_type()};
                if (nested_tag_type == ::pltxt2htm::NodeKind::html_span) {
                    auto const& parent_frame = call_stack.template current_frame<ndebug>();
                    auto const& node_color = active_node.get_color();
                    ::pltxt2htm::container::U8StringView const node_color_view{node_color};
                    auto const& node_fs = active_node.get_font_size();
                    auto const& node_va = active_node.get_vertical_align();
                    bool const same_font_size = node_fs == parent_frame.get_html_span_font_size();
                    bool const same_vertical_align = node_va == parent_frame.get_html_span_vertical_align();
                    if (node_color_view == parent_frame.get_html_span_color() && same_font_size &&
                        same_vertical_align) {
                        node = ::pltxt2htm::PlTxtNode<ndebug>{
                            ::pltxt2htm::Group<ndebug>{::std::move(active_node.get_subast())}};
                        ++current_iter;
                        continue;
                    }
                }
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_color) {
                    auto const& parent_color_id = call_stack.template current_frame<ndebug>().get_equal_sign_tag_id();
                    auto const& node_color = active_node.get_color();
                    ::pltxt2htm::container::U8StringView const node_color_view{node_color};
                    auto const& node_fs = active_node.get_font_size();
                    auto const& node_va = active_node.get_vertical_align();
                    if (node_color_view == parent_color_id && !node_fs.has_value() && !node_va.has_value()) {
                        node = ::pltxt2htm::PlTxtNode<ndebug>{
                            ::pltxt2htm::Group<ndebug>{::std::move(active_node.get_subast())}};
                        ++current_iter;
                        continue;
                    }
                }
                if (nested_tag_type == ::pltxt2htm::NodeKind::pl_a) {
                    static constexpr auto anchor_color_literal = ::pltxt2htm::PlA<ndebug>::get_color_literal();
                    static constexpr auto anchor_color = ::pltxt2htm::container::U8StringView{anchor_color_literal};
                    auto const& node_color = active_node.get_color();
                    ::pltxt2htm::container::U8StringView const node_color_view{node_color};
                    auto const& node_fs = active_node.get_font_size();
                    auto const& node_va = active_node.get_vertical_align();
                    if (node_color_view == anchor_color && !node_fs.has_value() && !node_va.has_value()) {
                        node = ::pltxt2htm::PlTxtNode<ndebug>{
                            ::pltxt2htm::Group<ndebug>{::std::move(active_node.get_subast())}};
                        ++current_iter;
                        continue;
                    }
                }

                auto const& span_color = active_node.get_color();
                auto const span_font_size = active_node.get_font_size();
                auto const span_vertical_align = active_node.get_vertical_align();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithHtmlSpanInfo<ndebug>{
                            ::pltxt2htm::container::U8StringView{span_color}, span_font_size, span_vertical_align}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                auto&& active_node = node.as_html_a();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_a, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a: {
                auto&& active_node = node.as_pl_a();
                auto&& subast = active_node.get_subast();

                // Optimization: <a><color=blue>text</color></a>
                // can be simplified to <color=blue>text</color>
                // The inner color takes precedence over the outer color
                if (subast.size() == 1) {
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_color ||
                        subnode.get_node_kind() == ::pltxt2htm::NodeKind::html_span) {
                        // SAFETY: We must NOT write `node = ::std::move(subnode);` directly.
                        // `subnode` is a reference into `node.get_subast()`. When the move-assignment
                        // operator of the node runs, it first destructs the old value at `*current_iter`, which
                        // in turn destructs `subnode` (since `subnode` lives inside that sub-AST). That means
                        // `subnode` is destroyed *before* its contents are moved -- a use-after-free.
                        // By moving `subnode` into a temporary first, we extract the value before the
                        // destination is touched, breaking the aliasing.
                        auto tmp = ::std::move(subnode);
                        node = ::std::move(tmp);
                        continue;
                    }
                }

                ::pltxt2htm::NodeKind const nested_tag_type{
                    call_stack.template current_frame<ndebug>().get_nested_tag_type()};
                // Optimization: If this color matches the parent color, flatten the nesting
                // <a>text<a>text</a>text</a> -> <a>texttexttext</a>
                auto const is_different_tag = bool{[nested_tag_type, &call_stack] constexpr noexcept {
                    static constexpr auto anchor_color_literal = ::pltxt2htm::PlA<ndebug>::get_color_literal();
                    static constexpr auto anchor_color = ::pltxt2htm::container::U8StringView{anchor_color_literal};
                    if (nested_tag_type == ::pltxt2htm::NodeKind::pl_a) {
                        return false;
                    }
                    if (nested_tag_type == ::pltxt2htm::NodeKind::pl_color) {
                        return anchor_color != call_stack.template current_frame<ndebug>().get_equal_sign_tag_id();
                    }
                    if (nested_tag_type == ::pltxt2htm::NodeKind::html_span) {
                        return anchor_color != call_stack.template current_frame<ndebug>().get_html_span_color();
                    }
                    return true; // Different tag types, so not the same
                }()};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_a, subast.begin()));
                    goto entry;
                }
                // Optimization: If the color is the same as the parent node, then ignore the nested tag.
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_experiment:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_experiments:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_discussion:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_discussions: {
                auto&& subast = [&]() -> decltype(auto) {
                    switch (node.get_node_kind()) {
                    case ::pltxt2htm::NodeKind::pl_experiment: {
                        auto&& active_node = node.as_pl_experiment();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::pl_experiments: {
                        auto&& active_node = node.as_pl_experiments();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::pl_discussion: {
                        auto&& active_node = node.as_pl_discussion();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::pl_discussions: {
                        auto&& active_node = node.as_pl_discussions();
                        return active_node.get_subast();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unreachable in optimizer experiment/discussion subast switch");
                        }
                    }
                }();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                auto const& equal_sign_tag_id = [&]() -> decltype(auto) {
                    switch (node.get_node_kind()) {
                    case ::pltxt2htm::NodeKind::pl_experiment: {
                        auto&& active_node = node.as_pl_experiment();
                        return active_node.get_id();
                    }
                    case ::pltxt2htm::NodeKind::pl_experiments: {
                        auto&& active_node = node.as_pl_experiments();
                        return active_node.get_value();
                    }
                    case ::pltxt2htm::NodeKind::pl_discussion: {
                        auto&& active_node = node.as_pl_discussion();
                        return active_node.get_id();
                    }
                    case ::pltxt2htm::NodeKind::pl_discussions: {
                        auto&& active_node = node.as_pl_discussions();
                        return active_node.get_value();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unreachable in optimizer experiment/discussion value switch");
                        }
                    }
                }();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                            ::pltxt2htm::container::U8StringView{equal_sign_tag_id}}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                auto&& active_node = node.as_pl_user();
                auto&& subast = active_node.get_subast();
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
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                // Optimization: If the user is the same as the parent node, then ignore the nested tag.
                auto const& equal_sign_tag_id = active_node.get_id();
                bool const is_different_tag =
                    nested_tag_type != ::pltxt2htm::NodeKind::pl_user ||
                    equal_sign_tag_id != call_stack.template current_frame<ndebug>().get_equal_sign_tag_id();
                if (is_different_tag) {
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_user, subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{
                                ::pltxt2htm::container::U8StringView{equal_sign_tag_id}}));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                auto&& active_node = node.as_pl_trigger();
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    // <trigger=...></trigger> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{::pltxt2htm::container::U8StringView{
                            active_node.get_value().data(), active_node.get_value().size()}}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                auto&& active_node = node.as_pl_internal();
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    // <internal=...></internal> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithEqualSignTagInfo{::pltxt2htm::container::U8StringView{
                            active_node.get_value().data(), active_node.get_value().size()}}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                auto&& active_node = node.as_pl_external();
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    // <external=123></external> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_external, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                auto&& active_node = node.as_pl_link();
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    // <link="url"></link> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_link, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                auto&& active_node = node.as_pl_size();
                auto&& subast = active_node.get_subast();
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
                auto&& frame = call_stack.template current_frame<ndebug>();
                // Optimization: If the size (value and unit) is the same as the parent node, ignore the nested tag.
                bool const is_different_tag = frame.get_nested_tag_type() != ::pltxt2htm::NodeKind::pl_size ||
                                              active_node.get_font_size() != frame.get_pl_size_tag_value();
                if (is_different_tag) {
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithPlSizeTagInfo{active_node.get_font_size()}));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                auto&& active_node = node.as_pl_voffset();
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    // <voffset=5></voffset> can be omitted
                    ast.erase(current_iter);
                    continue;
                }
                if (subast.size() == 1) {
                    // <voffset=5><voffset=3>physicsLab</voffset></voffset> can be
                    auto& subnode = ::pltxt2htm::details::vector_front<ndebug>(subast);
                    if (subnode.get_node_kind() == ::pltxt2htm::NodeKind::pl_voffset) {
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
                auto&& frame = call_stack.template current_frame<ndebug>();
                // Optimization: If the offset is the same as the parent node, ignore the nested tag.
                bool const is_different_tag = frame.get_nested_tag_type() != ::pltxt2htm::NodeKind::pl_voffset ||
                                              active_node.get_value() != frame.get_pl_voffset_tag_value();
                if (is_different_tag) {
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithPlVoffsetTagInfo{active_node.get_value()}));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                auto&& active_node = node.as_pl_align();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_align, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                auto&& active_node = node.as_pl_margin();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::pl_margin, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_b: {
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                bool const is_different_tag{!::pltxt2htm::details::is_strong_like(nested_tag_type)};
                auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                    switch (node.get_node_kind()) {
                    case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                        auto&& active_node = node.as_md_double_emphasis_underscore();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                        auto&& active_node = node.as_md_double_emphasis_asterisk();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::html_strong: {
                        auto&& active_node = node.as_html_strong();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::pl_b: {
                        auto&& active_node = node.as_pl_b();
                        return active_node.get_subast();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected node kind in emphasis switch");
                        }
                    }
                }();
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), node.get_node_kind(), subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto&& active_node = node.as_html_p();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_p, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                auto&& active_node = node.as_html_div();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_div, subast.begin()));
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
                auto&& active_node = node.as_html_h1();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h1, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                auto&& active_node = node.as_md_atx_h1();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h1, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                auto&& active_node = node.as_html_h2();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h2, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                auto&& active_node = node.as_md_atx_h2();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h2, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                auto&& active_node = node.as_html_h3();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h3, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                auto&& active_node = node.as_md_atx_h3();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h3, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                auto&& active_node = node.as_html_h4();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h4, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                auto&& active_node = node.as_md_atx_h4();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h4, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                auto&& active_node = node.as_html_h5();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h5, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                auto&& active_node = node.as_md_atx_h5();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h5, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                auto&& active_node = node.as_html_h6();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_h6, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                auto&& active_node = node.as_md_atx_h6();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_atx_h6, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_del: {
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                    switch (node.get_node_kind()) {
                    case ::pltxt2htm::NodeKind::md_del: {
                        auto&& active_node = node.as_md_del();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::html_del: {
                        auto&& active_node = node.as_html_del();
                        return active_node.get_subast();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected node kind in strikethrough switch");
                        }
                    }
                }();
                bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_del &&
                                            nested_tag_type != ::pltxt2htm::NodeKind::md_del};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::html_del, subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                auto&& active_node = node.as_html_code();
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = active_node.get_subast();
                bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_code};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::html_code, subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                auto&& active_node = node.as_html_mark();
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = active_node.get_subast();
                auto const& node_background_color = active_node.get_background_color();
                ::pltxt2htm::container::U8StringView const node_background_color_view{node_background_color};
                if (nested_tag_type != ::pltxt2htm::NodeKind::html_mark) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithHtmlMarkInfo<ndebug>{
                                node_background_color_view}));
                    goto entry;
                }
                // Optimization: same-tag mark with an identical background-color is flattened.
                // <mark style="background-color:yellow">a<mark style="background-color:yellow">b</mark>c</mark>
                // -> <mark style="background-color:yellow">abc</mark>
                if (node_background_color_view ==
                    call_stack.template current_frame<ndebug>().get_html_mark_background_color()) {
                    node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                    ++current_iter;
                    continue;
                }
                // Different background-color: keep the nesting and recurse into the inner mark.
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithHtmlMarkInfo<ndebug>{node_background_color_view}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                auto&& active_node = node.as_pl_mark();
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = active_node.get_subast();
                auto const& node_background_color = active_node.get_background_color();
                ::pltxt2htm::container::U8StringView const node_background_color_view{node_background_color};
                if (nested_tag_type != ::pltxt2htm::NodeKind::pl_mark) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), subast.begin(),
                            ::pltxt2htm::details::OptimizerContextWithPlMarkInfo<ndebug>{node_background_color_view}));
                    goto entry;
                }
                // Optimization: same-tag pl_mark with an identical background color is flattened.
                // <mark=yellow>a<mark=yellow>b</mark>c</mark>
                // -> <mark=yellow>abc</mark>
                if (node_background_color_view ==
                    call_stack.template current_frame<ndebug>().get_pl_mark_background_color()) {
                    node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                    ++current_iter;
                    continue;
                }
                // Different background-color: keep the nesting and recurse into the inner mark.
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), subast.begin(),
                        ::pltxt2htm::details::OptimizerContextWithPlMarkInfo<ndebug>{node_background_color_view}));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_u: {
                auto&& active_node = node.as_html_u();
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = active_node.get_subast();
                bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_u};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::html_u, subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_s: {
                auto&& active_node = node.as_html_s();
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                auto&& subast = active_node.get_subast();
                bool const is_different_tag{nested_tag_type != ::pltxt2htm::NodeKind::html_s};
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), ::pltxt2htm::NodeKind::html_s, subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                auto&& active_node = node.as_html_sup();
                // nested <sup> shifts the baseline further, so same-tag nesting must NOT be flattened
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_sup, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                auto&& active_node = node.as_html_sub();
                // nested <sub> shifts the baseline further, so same-tag nesting must NOT be flattened
                auto&& subast = active_node.get_subast();
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_sub, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_em: {
                auto&& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                bool const is_different_tag{!::pltxt2htm::details::is_em_like(nested_tag_type)};
                auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                    switch (node.get_node_kind()) {
                    case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                        auto&& active_node = node.as_md_single_emphasis_underscore();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                        auto&& active_node = node.as_md_single_emphasis_asterisk();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::pl_i: {
                        auto&& active_node = node.as_pl_i();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::html_em: {
                        auto&& active_node = node.as_html_em();
                        return active_node.get_subast();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected node kind");
                        }
                    }
                }();
                if (is_different_tag) {
                    if (subast.empty()) {
                        ast.erase(current_iter);
                        continue;
                    }
                    call_stack.push_frame(
                        ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                            ::std::addressof(subast), node.get_node_kind(), subast.begin()));
                    goto entry;
                }
                node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                ++current_iter;
                continue;
            }
            case ::pltxt2htm::NodeKind::html_note: {
                ast.erase(current_iter);
                continue;
            }
            case ::pltxt2htm::NodeKind::list_ul: {
                auto&& active_node = node.as_list_ul();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::list_ul, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                auto&& active_node = node.as_list_ol();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::list_ol, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                auto&& active_node = node.as_list_li_checkbox();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::list_li_checkbox, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                auto&& active_node = node.as_list_li();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::list_li, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                auto&& active_node = node.as_md_code_span_1_backtick();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_code_span_1_backtick, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                auto&& active_node = node.as_md_code_span_2_backtick();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_code_span_2_backtick, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                auto&& active_node = node.as_md_code_span_3_backtick();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::md_code_span_3_backtick, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                auto&& active_node = node.as_html_blockquote();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::html_blockquote, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                auto&& active_node = node.as_table();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                auto&& active_node = node.as_table_caption();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_caption, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                auto&& active_node = node.as_table_colgroup();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_colgroup, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                auto&& active_node = node.as_table_thead();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_thead, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                auto&& active_node = node.as_table_tbody();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_tbody, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                auto&& active_node = node.as_table_tfoot();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_tfoot, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                auto&& active_node = node.as_table_tr();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_tr, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                auto&& active_node = node.as_table_th();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_th, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto&& active_node = node.as_table_td();
                auto&& subast = active_node.get_subast();
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), ::pltxt2htm::NodeKind::table_td, subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                ::pltxt2htm::NodeKind const node_kind{node.get_node_kind()};
                auto&& subast = [&]() -> ::pltxt2htm::Ast<ndebug>& {
                    switch (node_kind) {
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                        auto&& active_node = node.as_md_triple_emphasis_underscore();
                        return active_node.get_subast();
                    }
                    case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                        auto&& active_node = node.as_md_triple_emphasis_asterisk();
                        return active_node.get_subast();
                    }
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected node kind in triple emphasis switch");
                        }
                    }
                }();
                pltxt2htm_assert(!subast.empty(), u8"md_triple_emphasis subast must not be empty");
                auto const& nested_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                if (nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk ||
                    nested_tag_type == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore) {
                    node = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::Group<ndebug>{::std::move(subast)}};
                    ++current_iter;
                    continue;
                }
                if (::pltxt2htm::details::is_em_like(nested_tag_type)) {
                    auto tmp = ::std::move(subast);
                    if (node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore) {
                        *current_iter = ::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::MdDoubleEmphasisUnderscore<ndebug>{::std::move(tmp)});
                    }
                    else if (node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk) {
                        *current_iter = ::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::MdDoubleEmphasisAsterisk<ndebug>(::std::move(tmp)));
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected node kind");
                    }
                    continue;
                }
                if (::pltxt2htm::details::is_strong_like(nested_tag_type)) {
                    auto tmp = ::std::move(subast);
                    if (node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_underscore) {
                        *current_iter = ::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::MdSingleEmphasisUnderscore<ndebug>{::std::move(tmp)});
                    }
                    else if (node_kind == ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk) {
                        *current_iter = ::pltxt2htm::PlTxtNode<ndebug>(
                            ::pltxt2htm::MdSingleEmphasisAsterisk<ndebug>{::std::move(tmp)});
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected node kind");
                    }
                    continue;
                }
                if (subast.empty()) {
                    ast.erase(current_iter);
                    continue;
                }
                call_stack.push_frame(
                    ::pltxt2htm::details::OptimizerFrame<typename ::pltxt2htm::Ast<ndebug>::iterator, ndebug>(
                        ::std::addressof(subast), node.get_node_kind(), subast.begin()));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_col:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_img:
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
            case ::pltxt2htm::NodeKind::code_fence: {
                ++current_iter;
                continue;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected node kind in escape/code-fence switch");
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

        bool const has_parent{call_stack.has_parent()};
        call_stack.template discard_current_frame<ndebug>();
        if (has_parent) {
            ++(call_stack.template current_frame<ndebug>().iter);
            goto entry;
        }
        return;
    }
    return;
}

} // namespace pltxt2htm

#include "details/pop_macro.hh"
