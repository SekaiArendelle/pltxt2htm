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

template<::pltxt2htm::Contracts ndebug>
class BasicFrameContext {
public:
    union ContextVariant {
        ::pltxt2htm::details::ParserFrameContextWithPltextInfo pltext;
        ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo equal_sign_tag;
        ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo external_tag;
        ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo pl_size_tag;
        ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo md_block_quotes;
        ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo md_link;
        ::pltxt2htm::details::ParserFrameContextWithMdListInfo md_list;

        constexpr ContextVariant() noexcept
            : pltext{} {
        }

        constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPltextInfo&& pltext_context) noexcept
            : pltext{::std::move(pltext_context)} {
        }

        constexpr ContextVariant(
            ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context) noexcept
            : equal_sign_tag{::std::move(equal_sign_tag_context)} {
        }

        constexpr ContextVariant(
            ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo&& external_tag_context) noexcept
            : external_tag{::std::move(external_tag_context)} {
        }

        constexpr ContextVariant(
            ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context) noexcept
            : pl_size_tag{::std::move(pl_size_tag_context)} {
        }

        constexpr ContextVariant(
            ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context) noexcept
            : md_block_quotes{::std::move(md_block_quotes_context)} {
        }

        constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdLinkInfo&& md_link_context) noexcept
            : md_link{::std::move(md_link_context)} {
        }

        constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdListInfo&& md_list_context) noexcept
            : md_list{::std::move(md_list_context)} {
        }

        constexpr ~ContextVariant() noexcept {
        }
    } context_data;

    ::pltxt2htm::NodeType nested_tag_type;
    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{},
          nested_tag_type{nested_tag_type_} {
        bool const is_plain_pltext_type{
            ::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_,
                                         ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_plain_pltext_type{
            ::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Ast&& subast_) noexcept
        : context_data{::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}},
          nested_tag_type{nested_tag_type_},
          subast(::std::move(subast_)) {
        bool const is_plain_pltext_type{
            ::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::fast_io::u8string&& id_) noexcept
        : context_data{
              ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo{.pltext = pltext_, .id = ::std::move(id_)}},
          nested_tag_type{nested_tag_type_} {
        bool const is_equal_sign_tag_type{
            ::pltxt2htm::details::is_equal_sign_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Url&& url_) noexcept
        : context_data{
              ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo{.pltext = pltext_, .url = ::std::move(url_)}},
          nested_tag_type{nested_tag_type_} {
        bool const is_external_tag_type{
            ::pltxt2htm::details::is_external_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_external_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::std::size_t id_) noexcept
        : context_data{::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo{.pltext = pltext_, .id = id_}},
          nested_tag_type{nested_tag_type_} {
        bool const is_pl_size_tag_type{
            ::pltxt2htm::details::is_pl_size_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string&& pltext_) noexcept
        : context_data{::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo{::std::move(pltext_)}},
          nested_tag_type{::pltxt2htm::NodeType::md_block_quotes} {
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::Url&& link_) noexcept
        : context_data{
              ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo{.pltext = pltext_, .link = ::std::move(link_)}},
          nested_tag_type{::pltxt2htm::NodeType::md_link} {
    }

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType node_type,
                                         ::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : context_data{::pltxt2htm::details::ParserFrameContextWithMdListInfo{::std::move(md_list_ast_)}},
          nested_tag_type{node_type} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeType::md_ul || node_type == ::pltxt2htm::NodeType::md_ol,
                         u8"mismatch node type");
    }

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept = delete;

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug>&& other) noexcept
        : context_data{},
          nested_tag_type{other.nested_tag_type},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.equal_sign_tag),
                                ::std::move(other.context_data.equal_sign_tag));
        }
        else if (::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.external_tag),
                                ::std::move(other.context_data.external_tag));
        }
        else if (::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.pl_size_tag),
                                ::std::move(other.context_data.pl_size_tag));
        }
        else if (::pltxt2htm::details::is_md_block_quotes_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.md_block_quotes),
                                ::std::move(other.context_data.md_block_quotes));
        }
        else if (::pltxt2htm::details::is_md_link_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.md_link), ::std::move(other.context_data.md_link));
        }
        else if (::pltxt2htm::details::is_md_list_type(this->nested_tag_type)) {
            ::std::construct_at(::std::addressof(this->context_data.md_list), ::std::move(other.context_data.md_list));
        }
        else {
            ::std::construct_at(::std::addressof(this->context_data.pltext), ::std::move(other.context_data.pltext));
        }
    }

    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;

    constexpr ~BasicFrameContext() noexcept {
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.equal_sign_tag));
            return;
        }
        if (::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.external_tag));
            return;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.pl_size_tag));
            return;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.md_block_quotes));
            return;
        }
        if (::pltxt2htm::details::is_md_link_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.md_link));
            return;
        }
        if (::pltxt2htm::details::is_md_list_type(this->nested_tag_type)) {
            ::std::destroy_at(::std::addressof(this->context_data.md_list));
            return;
        }
        ::std::destroy_at(::std::addressof(this->context_data.pltext));
    }

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
        if (::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)) {
            return this->context_data.equal_sign_tag.pltext;
        }
        if (::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)) {
            return this->context_data.external_tag.pltext;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)) {
            return this->context_data.pl_size_tag.pltext;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(this->nested_tag_type)) {
            return ::fast_io::u8string_view{this->context_data.md_block_quotes.pltext.data(),
                                            this->context_data.md_block_quotes.pltext.size()};
        }
        if (::pltxt2htm::details::is_md_link_type(this->nested_tag_type)) {
            return this->context_data.md_link.pltext;
        }
        if (::pltxt2htm::details::is_md_list_type(this->nested_tag_type)) {
            [[unlikely]] {
                ::exception::unreachable<true>();
            }
        }
        return this->context_data.pltext.pltext;
    }

    constexpr auto get_equal_sign_tag_id() noexcept -> ::fast_io::u8string& {
        bool const is_equal_sign_tag_type{
            ::pltxt2htm::details::is_equal_sign_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return this->context_data.equal_sign_tag.id;
    }

    constexpr auto get_external_tag_url() noexcept -> ::pltxt2htm::Url& {
        bool const is_external_tag_type{
            ::pltxt2htm::details::is_external_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_external_tag_type, u8"context kind mismatch");
        return this->context_data.external_tag.url;
    }

    constexpr auto get_pl_size_tag_id() const noexcept -> ::std::size_t {
        bool const is_pl_size_tag_type{
            ::pltxt2htm::details::is_pl_size_tag_type(this->nested_tag_type)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return this->context_data.pl_size_tag.id;
    }

    constexpr auto get_md_link_url() noexcept -> ::pltxt2htm::Url& {
        bool const is_md_link_type{
            ::pltxt2htm::details::is_md_link_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return this->context_data.md_link.link;
    }

    constexpr auto get_md_list_ast() noexcept -> ::pltxt2htm::details::MdListAst& {
        bool const is_md_list_type{
            ::pltxt2htm::details::is_md_list_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return this->context_data.md_list.md_list_ast;
    }

    constexpr auto get_md_list_iter() noexcept -> ::pltxt2htm::details::MdListAst::iterator& {
        bool const is_md_list_type{
            ::pltxt2htm::details::is_md_list_type(this->nested_tag_type)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return this->context_data.md_list.iter;
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
