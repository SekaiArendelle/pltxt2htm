/**
 * @file frame_concext.hh
 * @brief Parser frame contexts for parsing pl-text
 * @details Defines context storage used during parsing.
 * @note The filename "concext" is intentionally spelled this way (not "context")
 *       to avoid potential naming conflicts in the codebase.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "md_list.hh"
#include "../../contracts.hh"
#include "../../astnode/basic.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

class ParserFrameContextWithPltextInfo {
public:
    ::fast_io::u8string_view pltext;
};

class ParserFrameContextWithEqualSignTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string id;
};

class ParserFrameContextWithExternalTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url url;
};

class ParserFrameContextWithPlSizeTagInfo {
public:
    ::fast_io::u8string_view pltext;
    ::std::size_t id;
};

class ParserFrameContextWithMdBlockQuotesInfo {
public:
    ::fast_io::u8string pltext;
};

class ParserFrameContextWithMdLinkInfo {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::Url link;
};

class ParserFrameContextWithMdListInfo {
public:
    ::pltxt2htm::details::MdListAst md_list_ast;
    ::pltxt2htm::details::MdListAst::iterator iter;

    constexpr explicit ParserFrameContextWithMdListInfo(::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : md_list_ast(::std::move(md_list_ast_)),
          iter(md_list_ast.begin()) {
    }
};

class ContextVariant {
public:
    union {
        ::pltxt2htm::details::ParserFrameContextWithPltextInfo pltext;
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo external_tag;
        ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo pl_size_tag;
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo md_block_quotes;
        ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo md_link;
        ::pltxt2htm::details::ParserFrameContextWithMdListInfo md_list;
    };

    ::pltxt2htm::NodeType kind{::pltxt2htm::NodeType::base};

    constexpr ContextVariant() noexcept
        : pltext{},
          kind{::pltxt2htm::NodeType::base} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPltextInfo&& pltext_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : pltext{::std::move(pltext_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context,
        ::pltxt2htm::NodeType node_type) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo&& external_tag_context,
        ::pltxt2htm::NodeType node_type) noexcept
        : external_tag{::std::move(external_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context,
        ::pltxt2htm::NodeType node_type) noexcept
        : md_block_quotes{::std::move(md_block_quotes_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdLinkInfo&& md_link_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : md_link{::std::move(md_link_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdListInfo&& md_list_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : md_list{::std::move(md_list_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(ContextVariant const&) noexcept = delete;

    constexpr ContextVariant(ContextVariant&& other) noexcept
        : kind{other.kind} {
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        if (::pltxt2htm::details::is_external_tag_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->external_tag), ::std::move(other.external_tag));
            return;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->md_block_quotes), ::std::move(other.md_block_quotes));
            return;
        }
        if (::pltxt2htm::details::is_md_link_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->md_link), ::std::move(other.md_link));
            return;
        }
        if (::pltxt2htm::details::is_md_list_type(this->kind)) {
            ::std::construct_at(::std::addressof(this->md_list), ::std::move(other.md_list));
            return;
        }
        ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
    }

    constexpr auto operator=(ContextVariant const&) noexcept -> ContextVariant& = delete;
    constexpr auto operator=(ContextVariant&&) noexcept -> ContextVariant& = delete;

    constexpr ~ContextVariant() noexcept {
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->equal_sign_tag));
            return;
        }
        if (::pltxt2htm::details::is_external_tag_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->external_tag));
            return;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->pl_size_tag));
            return;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->md_block_quotes));
            return;
        }
        if (::pltxt2htm::details::is_md_link_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->md_link));
            return;
        }
        if (::pltxt2htm::details::is_md_list_type(this->kind)) {
            ::std::destroy_at(::std::addressof(this->md_list));
            return;
        }
        ::std::destroy_at(::std::addressof(this->pltext));
    }
};

template<::pltxt2htm::Contracts ndebug>
class BasicFrameContext {
private:
    [[nodiscard]]
    static constexpr auto move_context_data_from(::pltxt2htm::details::BasicFrameContext<ndebug>& other) noexcept
        -> ::exception::optional<ContextVariant> {
        if (!other.context_data.has_value()) {
            return ::exception::nullopt_t{};
        }
        return ::std::move(other.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
    }

public:
    ::exception::optional<ContextVariant> context_data;

    ::pltxt2htm::NodeType nested_tag_type;
    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{::exception::nullopt_t{}},
          nested_tag_type{nested_tag_type_} {
        bool const is_without_info_type{nested_tag_type_ == ::pltxt2htm::NodeType::base};
        pltxt2htm_assert(is_without_info_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_,
                                         ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{ContextVariant{::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Ast&& subast_) noexcept
        : context_data{ContextVariant{::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          nested_tag_type{nested_tag_type_},
          subast(::std::move(subast_)) {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::fast_io::u8string&& id_) noexcept
        : context_data{ContextVariant{
              ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo{.pltext = pltext_, .id = ::std::move(id_)},
              nested_tag_type_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Url&& url_) noexcept
        : context_data{ContextVariant{::pltxt2htm::details::ParserFrameContextWithExternalTagInfo{
              .pltext = pltext_, .url = ::std::move(url_)},
              nested_tag_type_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_external_tag_type{::pltxt2htm::details::is_external_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_external_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::std::size_t id_) noexcept
        : context_data{
              ContextVariant{::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo{.pltext = pltext_, .id = id_},
                             nested_tag_type_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string&& pltext_) noexcept
        : context_data{
              ContextVariant{::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo{::std::move(pltext_)},
                             ::pltxt2htm::NodeType::md_block_quotes}},
          nested_tag_type{::pltxt2htm::NodeType::md_block_quotes} {
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::Url&& link_) noexcept
        : context_data{ContextVariant{
              ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo{.pltext = pltext_, .link = ::std::move(link_)},
              ::pltxt2htm::NodeType::md_link}},
          nested_tag_type{::pltxt2htm::NodeType::md_link} {
    }

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType node_type,
                                         ::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : context_data{
              ContextVariant{::pltxt2htm::details::ParserFrameContextWithMdListInfo{::std::move(md_list_ast_)}, node_type}},
          nested_tag_type{node_type} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeType::md_ul || node_type == ::pltxt2htm::NodeType::md_ol,
                         u8"mismatch node type");
    }

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept = delete;

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug>&& other) noexcept
        : context_data{move_context_data_from(other)},
          nested_tag_type{other.nested_tag_type},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
    }

    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;

    constexpr ~BasicFrameContext() noexcept = default;

    constexpr auto operator->() noexcept -> ::pltxt2htm::details::BasicFrameContext<ndebug>* {
        return this;
    }

    constexpr auto operator->() const noexcept -> ::pltxt2htm::details::BasicFrameContext<ndebug> const* {
        return this;
    }

    constexpr auto get_unsafe() noexcept -> ::pltxt2htm::details::BasicFrameContext<ndebug>* {
        return this;
    }

    constexpr auto get_unsafe() const noexcept -> ::pltxt2htm::details::BasicFrameContext<ndebug> const* {
        return this;
    }

    constexpr auto release_imul() const noexcept -> ::pltxt2htm::details::BasicFrameContext<ndebug> const* {
        return this;
    }

    [[nodiscard]]
    constexpr auto get_pltext() const noexcept -> ::fast_io::u8string_view {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto const& context_data_ref = this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)) {
            return context_data_ref.equal_sign_tag.pltext;
        }
        if (::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)) {
            return context_data_ref.external_tag.pltext;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)) {
            return context_data_ref.pl_size_tag.pltext;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(this->nested_tag_type)) {
            return ::fast_io::u8string_view{context_data_ref.md_block_quotes.pltext.data(),
                                            context_data_ref.md_block_quotes.pltext.size()};
        }
        if (::pltxt2htm::details::is_md_link_type(this->nested_tag_type)) {
            return context_data_ref.md_link.pltext;
        }
        if (::pltxt2htm::details::is_md_list_type(this->nested_tag_type)) {
            [[unlikely]] { ::exception::unreachable<true>(); }
        }
        return context_data_ref.pltext.pltext;
    }

    constexpr auto get_equal_sign_tag_id() noexcept -> ::fast_io::u8string& {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().equal_sign_tag.id;
    }

    constexpr auto get_external_tag_url() noexcept -> ::pltxt2htm::Url& {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_external_tag_type{::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_external_tag_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().external_tag.url;
    }

    constexpr auto get_pl_size_tag_id() const noexcept -> ::std::size_t {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().pl_size_tag.id;
    }

    constexpr auto get_md_link_url() noexcept -> ::pltxt2htm::Url& {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_md_link_type{::pltxt2htm::details::is_md_link_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().md_link.link;
    }

    constexpr auto get_md_list_ast() noexcept -> ::pltxt2htm::details::MdListAst& {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().md_list.md_list_ast;
    }

    constexpr auto get_md_list_iter() noexcept -> ::pltxt2htm::details::MdListAst::iterator& {
        bool const has_context_data{this->context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return this->context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().md_list.iter;
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
