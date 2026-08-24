/**
 * @file frame_context.hh
 * @brief Backend frame context for HTML generation
 * @details Defines the context structure used by the HTML backend to track
 *          the current state during AST to HTML conversion
 */

#pragma once

#include <cstddef>
#include <utility>
#include <fast_io/fast_io_dsal/vector.h>
#include "../../ast/ast.hh"
#include "../../contracts.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Empty context for tags that carry no extra data.
 */
class BackendContextWithoutInfo {};

/**
 * @brief Context for ordered-list frames: tracks the current list-item counter.
 */
class BackendContextWithOlInfo {
public:
    ::std::size_t ol_li_count{1}; ///< Running list-item counter for <ol> numbering.
};

/**
 * @brief Context for html_span frames: remembers which style properties were set.
 */
class BackendContextWithHtmlSpanInfo {
public:
    bool has_color{}; ///< Whether the span has a color.
    bool has_font_size{}; ///< Whether the span has a font-size.
    bool has_vertical_align{}; ///< Whether the span has a vertical-align.
};

/**
 * @brief Context for alignment frames: remembers whether a Unity <align> tag was opened.
 * @details Used by ::pltxt2htm::NodeKind::html_p frames so the plunity backend can emit
 *          the matching `</align>` closing tag.
 */
class BackendContextWithAlignInfo {
public:
    bool has_align{}; ///< Whether the frame opened a Unity <align> tag.
};

/**
 * @brief Tagged-union variant of backend context payloads.
 * @details Dispatched on `kind` (::pltxt2htm::NodeKind) - used inside
 *          BackendFrameContext.
 */
template<::pltxt2htm::Contracts ndebug>
class BackendContextVariant {
    union {
        BackendContextWithoutInfo without_info;
        BackendContextWithOlInfo ol_info;
        BackendContextWithHtmlSpanInfo html_span_info;
        BackendContextWithAlignInfo align_info;
    };

    ::pltxt2htm::NodeKind kind;

public:
    constexpr BackendContextVariant(::pltxt2htm::NodeKind const kind_) noexcept
        : without_info{},
          kind{kind_} {
    }

    constexpr BackendContextVariant(::pltxt2htm::NodeKind const kind_,
                                    BackendContextWithOlInfo ol_info_context) noexcept
        : ol_info{::std::move(ol_info_context)},
          kind{kind_} {
    }

    constexpr BackendContextVariant(BackendContextWithHtmlSpanInfo html_span_info_context) noexcept
        : html_span_info{::std::move(html_span_info_context)},
          kind{::pltxt2htm::NodeKind::html_span} {
    }

    constexpr BackendContextVariant(::pltxt2htm::NodeKind const kind_,
                                    BackendContextWithAlignInfo align_info_context) noexcept
        : align_info{::std::move(align_info_context)},
          kind{kind_} {
    }

    constexpr ~BackendContextVariant() noexcept = default;

    [[nodiscard]]
    constexpr auto get_kind(this BackendContextVariant<ndebug> const& self) noexcept -> ::pltxt2htm::NodeKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto as_without_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind != ::pltxt2htm::NodeKind::list_ol && self.kind != ::pltxt2htm::NodeKind::html_span &&
                             self.kind != ::pltxt2htm::NodeKind::html_p,
                         u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(self.without_info);
    }

    [[nodiscard]]
    constexpr auto as_ol_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::list_ol, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(self.ol_info);
    }

    [[nodiscard]]
    constexpr auto as_html_span_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_span, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(self.html_span_info);
    }

    [[nodiscard]]
    constexpr auto as_align_info(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.kind == ::pltxt2htm::NodeKind::html_p, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(self.align_info);
    }
};

/**
 * @brief Basic frame context for HTML backend processing
 * @details This context keeps track of the current AST being processed,
 *          the type of nested tag, and the current index position
 * @note This is used during the recursive AST traversal for HTML generation
 */
template<::pltxt2htm::Contracts ndebug>
class BackendFrameContext {
    BackendContextVariant<ndebug> context_data;
    /* [[nonnull]] */ ::pltxt2htm::Ast<ndebug> const* ast; ///< Reference to the AST being processed

public:
    ::std::size_t current_index; ///< Current index position in the AST

    /**
     * @note construct ast from reference to avoid nullptr issue
     */
    constexpr BackendFrameContext(::pltxt2htm::Ast<ndebug> const& ast_, ::pltxt2htm::NodeKind const nested_tag_type,
                                  ::std::size_t current_index_) noexcept
        : context_data{nested_tag_type},
          ast(::std::addressof(ast_)),
          current_index{current_index_} {
    }

    constexpr BackendFrameContext(::pltxt2htm::Ast<ndebug> const& ast_, ::pltxt2htm::NodeKind const nested_tag_type,
                                  ::std::size_t current_index_, BackendContextWithOlInfo ol_info_context) noexcept
        : context_data{nested_tag_type, ::std::move(ol_info_context)},
          ast(::std::addressof(ast_)),
          current_index{current_index_} {
    }

    constexpr BackendFrameContext(::pltxt2htm::Ast<ndebug> const& ast_, ::std::size_t current_index_,
                                  BackendContextWithHtmlSpanInfo html_span_info_context) noexcept
        : context_data{::std::move(html_span_info_context)},
          ast(::std::addressof(ast_)),
          current_index{current_index_} {
    }

    constexpr BackendFrameContext(::pltxt2htm::Ast<ndebug> const& ast_, ::pltxt2htm::NodeKind const nested_tag_type,
                                  ::std::size_t current_index_, BackendContextWithAlignInfo align_info_context) noexcept
        : context_data{nested_tag_type, ::std::move(align_info_context)},
          ast(::std::addressof(ast_)),
          current_index{current_index_} {
    }

    constexpr BackendFrameContext(BackendFrameContext<ndebug> const&) noexcept = default;
    constexpr BackendFrameContext(BackendFrameContext<ndebug>&&) noexcept = default;

    constexpr ~BackendFrameContext() noexcept = default;

    constexpr BackendFrameContext<ndebug>& operator=(BackendFrameContext<ndebug> const&) noexcept = delete;
    constexpr BackendFrameContext<ndebug>& operator=(BackendFrameContext<ndebug>&&) noexcept = default;

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this BackendFrameContext<ndebug> const& self) noexcept {
        return self.context_data.get_kind();
    }

    [[nodiscard]]
    constexpr auto get_ast(this BackendFrameContext<ndebug> const& self) noexcept -> ::pltxt2htm::Ast<ndebug> const& {
        return *(self.ast);
    }

    [[nodiscard]]
    constexpr auto as_without_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_without_info();
    }

    [[nodiscard]]
    constexpr auto as_ol_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_ol_info();
    }

    [[nodiscard]]
    constexpr auto as_html_span_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_html_span_info();
    }

    [[nodiscard]]
    constexpr auto as_align_info(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.context_data).as_align_info();
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
