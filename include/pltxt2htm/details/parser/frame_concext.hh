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

    ::pltxt2htm::NodeType kind;

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPltextInfo&& pltext_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : pltext{::std::move(pltext_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo&& equal_sign_tag_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : equal_sign_tag{::std::move(equal_sign_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithExternalTagInfo&& external_tag_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : external_tag{::std::move(external_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo&& pl_size_tag_context,
                             ::pltxt2htm::NodeType node_type) noexcept
        : pl_size_tag{::std::move(pl_size_tag_context)},
          kind{node_type} {
    }

    constexpr ContextVariant(::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo&& md_block_quotes_context,
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

    constexpr ContextVariant(::pltxt2htm::details::ContextVariant<ndebug> const&) noexcept = delete;

    constexpr ContextVariant(::pltxt2htm::details::ContextVariant<ndebug>&& other) noexcept
        : kind{other.kind} {
        switch (this->kind) {
        case ::pltxt2htm::NodeType::pl_color:
        case ::pltxt2htm::NodeType::pl_experiment:
        case ::pltxt2htm::NodeType::pl_discussion:
        case ::pltxt2htm::NodeType::pl_user: {
            ::std::construct_at(::std::addressof(this->equal_sign_tag), ::std::move(other.equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeType::pl_external: {
            ::std::construct_at(::std::addressof(this->external_tag), ::std::move(other.external_tag));
            return;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            ::std::construct_at(::std::addressof(this->pl_size_tag), ::std::move(other.pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeType::md_block_quotes: {
            ::std::construct_at(::std::addressof(this->md_block_quotes), ::std::move(other.md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeType::md_link: {
            ::std::construct_at(::std::addressof(this->md_link), ::std::move(other.md_link));
            return;
        }
        case ::pltxt2htm::NodeType::md_ul:
        case ::pltxt2htm::NodeType::md_ol: {
            ::std::construct_at(::std::addressof(this->md_list), ::std::move(other.md_list));
            return;
        }
        case ::pltxt2htm::NodeType::base:
        case ::pltxt2htm::NodeType::u8char:
        case ::pltxt2htm::NodeType::invalid_u8char:
        case ::pltxt2htm::NodeType::text:
        case ::pltxt2htm::NodeType::line_break:
        case ::pltxt2htm::NodeType::space:
        case ::pltxt2htm::NodeType::ampersand:
        case ::pltxt2htm::NodeType::double_quote:
        case ::pltxt2htm::NodeType::single_quote:
        case ::pltxt2htm::NodeType::less_than:
        case ::pltxt2htm::NodeType::greater_than:
        case ::pltxt2htm::NodeType::tab:
        case ::pltxt2htm::NodeType::pl_a:
        case ::pltxt2htm::NodeType::pl_b:
        case ::pltxt2htm::NodeType::pl_i:
        case ::pltxt2htm::NodeType::pl_macro_project:
        case ::pltxt2htm::NodeType::pl_macro_visitor:
        case ::pltxt2htm::NodeType::pl_macro_author:
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
        case ::pltxt2htm::NodeType::html_p:
        case ::pltxt2htm::NodeType::html_br:
        case ::pltxt2htm::NodeType::html_h1:
        case ::pltxt2htm::NodeType::html_h2:
        case ::pltxt2htm::NodeType::html_h3:
        case ::pltxt2htm::NodeType::html_h4:
        case ::pltxt2htm::NodeType::html_h5:
        case ::pltxt2htm::NodeType::html_h6:
        case ::pltxt2htm::NodeType::html_del:
        case ::pltxt2htm::NodeType::html_hr:
        case ::pltxt2htm::NodeType::html_note:
        case ::pltxt2htm::NodeType::html_em:
        case ::pltxt2htm::NodeType::html_strong:
        case ::pltxt2htm::NodeType::html_ul:
        case ::pltxt2htm::NodeType::html_ol:
        case ::pltxt2htm::NodeType::html_li:
        case ::pltxt2htm::NodeType::html_code:
        case ::pltxt2htm::NodeType::html_pre:
        case ::pltxt2htm::NodeType::html_blockquote:
        case ::pltxt2htm::NodeType::md_atx_h1:
        case ::pltxt2htm::NodeType::md_atx_h2:
        case ::pltxt2htm::NodeType::md_atx_h3:
        case ::pltxt2htm::NodeType::md_atx_h4:
        case ::pltxt2htm::NodeType::md_atx_h5:
        case ::pltxt2htm::NodeType::md_atx_h6:
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
        case ::pltxt2htm::NodeType::md_code_fence_tilde:
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_del:
        case ::pltxt2htm::NodeType::md_image:
        case ::pltxt2htm::NodeType::md_li:
        case ::pltxt2htm::NodeType::md_latex_inline:
        case ::pltxt2htm::NodeType::md_latex_block: {
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        case ::pltxt2htm::NodeType::md_escape_backslash:
        case ::pltxt2htm::NodeType::md_escape_exclamation:
        case ::pltxt2htm::NodeType::md_escape_double_quote:
        case ::pltxt2htm::NodeType::md_escape_hash:
        case ::pltxt2htm::NodeType::md_escape_dollar:
        case ::pltxt2htm::NodeType::md_escape_percent:
        case ::pltxt2htm::NodeType::md_escape_ampersand:
        case ::pltxt2htm::NodeType::md_escape_single_quote:
        case ::pltxt2htm::NodeType::md_escape_left_paren:
        case ::pltxt2htm::NodeType::md_escape_right_paren:
        case ::pltxt2htm::NodeType::md_escape_asterisk:
        case ::pltxt2htm::NodeType::md_escape_plus:
        case ::pltxt2htm::NodeType::md_escape_comma:
        case ::pltxt2htm::NodeType::md_escape_hyphen:
        case ::pltxt2htm::NodeType::md_escape_dot:
        case ::pltxt2htm::NodeType::md_escape_slash:
        case ::pltxt2htm::NodeType::md_escape_colon:
        case ::pltxt2htm::NodeType::md_escape_semicolon:
        case ::pltxt2htm::NodeType::md_escape_less_than:
        case ::pltxt2htm::NodeType::md_escape_equals:
        case ::pltxt2htm::NodeType::md_escape_greater_than:
        case ::pltxt2htm::NodeType::md_escape_question:
        case ::pltxt2htm::NodeType::md_escape_at:
        case ::pltxt2htm::NodeType::md_escape_left_bracket:
        case ::pltxt2htm::NodeType::md_escape_right_bracket:
        case ::pltxt2htm::NodeType::md_escape_caret:
        case ::pltxt2htm::NodeType::md_escape_underscore:
        case ::pltxt2htm::NodeType::md_escape_backtick:
        case ::pltxt2htm::NodeType::md_escape_left_brace:
        case ::pltxt2htm::NodeType::md_escape_pipe:
        case ::pltxt2htm::NodeType::md_escape_right_brace:
        case ::pltxt2htm::NodeType::md_escape_tilde:
        case ::pltxt2htm::NodeType::md_hr:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    constexpr auto operator=(::pltxt2htm::details::ContextVariant<ndebug> const&) noexcept
        -> ::pltxt2htm::details::ContextVariant<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::ContextVariant<ndebug>&&) noexcept
        -> ::pltxt2htm::details::ContextVariant<ndebug>& = delete;

    constexpr ~ContextVariant() noexcept {
        switch (this->kind) {
        case ::pltxt2htm::NodeType::pl_color:
        case ::pltxt2htm::NodeType::pl_experiment:
        case ::pltxt2htm::NodeType::pl_discussion:
        case ::pltxt2htm::NodeType::pl_user: {
            ::std::destroy_at(::std::addressof(this->equal_sign_tag));
            return;
        }
        case ::pltxt2htm::NodeType::pl_external: {
            ::std::destroy_at(::std::addressof(this->external_tag));
            return;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            ::std::destroy_at(::std::addressof(this->pl_size_tag));
            return;
        }
        case ::pltxt2htm::NodeType::md_block_quotes: {
            ::std::destroy_at(::std::addressof(this->md_block_quotes));
            return;
        }
        case ::pltxt2htm::NodeType::md_link: {
            ::std::destroy_at(::std::addressof(this->md_link));
            return;
        }
        case ::pltxt2htm::NodeType::md_ul:
        case ::pltxt2htm::NodeType::md_ol: {
            ::std::destroy_at(::std::addressof(this->md_list));
            return;
        }
        case ::pltxt2htm::NodeType::u8char:
        case ::pltxt2htm::NodeType::invalid_u8char:
        case ::pltxt2htm::NodeType::text:
        case ::pltxt2htm::NodeType::line_break:
        case ::pltxt2htm::NodeType::space:
        case ::pltxt2htm::NodeType::ampersand:
        case ::pltxt2htm::NodeType::double_quote:
        case ::pltxt2htm::NodeType::single_quote:
        case ::pltxt2htm::NodeType::less_than:
        case ::pltxt2htm::NodeType::greater_than:
        case ::pltxt2htm::NodeType::tab:
        case ::pltxt2htm::NodeType::pl_a:
        case ::pltxt2htm::NodeType::pl_b:
        case ::pltxt2htm::NodeType::pl_i:
        case ::pltxt2htm::NodeType::pl_macro_project:
        case ::pltxt2htm::NodeType::pl_macro_visitor:
        case ::pltxt2htm::NodeType::pl_macro_author:
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
        case ::pltxt2htm::NodeType::html_p:
        case ::pltxt2htm::NodeType::html_br:
        case ::pltxt2htm::NodeType::html_h1:
        case ::pltxt2htm::NodeType::html_h2:
        case ::pltxt2htm::NodeType::html_h3:
        case ::pltxt2htm::NodeType::html_h4:
        case ::pltxt2htm::NodeType::html_h5:
        case ::pltxt2htm::NodeType::html_h6:
        case ::pltxt2htm::NodeType::html_del:
        case ::pltxt2htm::NodeType::html_hr:
        case ::pltxt2htm::NodeType::html_note:
        case ::pltxt2htm::NodeType::html_em:
        case ::pltxt2htm::NodeType::html_strong:
        case ::pltxt2htm::NodeType::html_ul:
        case ::pltxt2htm::NodeType::html_ol:
        case ::pltxt2htm::NodeType::html_li:
        case ::pltxt2htm::NodeType::html_code:
        case ::pltxt2htm::NodeType::html_pre:
        case ::pltxt2htm::NodeType::html_blockquote:
        case ::pltxt2htm::NodeType::md_atx_h1:
        case ::pltxt2htm::NodeType::md_atx_h2:
        case ::pltxt2htm::NodeType::md_atx_h3:
        case ::pltxt2htm::NodeType::md_atx_h4:
        case ::pltxt2htm::NodeType::md_atx_h5:
        case ::pltxt2htm::NodeType::md_atx_h6:
        case ::pltxt2htm::NodeType::md_hr:
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
        case ::pltxt2htm::NodeType::md_code_fence_tilde:
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
        case ::pltxt2htm::NodeType::md_del:
        case ::pltxt2htm::NodeType::md_image:
        case ::pltxt2htm::NodeType::md_li:
        case ::pltxt2htm::NodeType::md_latex_inline:
        case ::pltxt2htm::NodeType::md_latex_block: {
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        case ::pltxt2htm::NodeType::base:
        case ::pltxt2htm::NodeType::md_escape_backslash:
        case ::pltxt2htm::NodeType::md_escape_exclamation:
        case ::pltxt2htm::NodeType::md_escape_double_quote:
        case ::pltxt2htm::NodeType::md_escape_hash:
        case ::pltxt2htm::NodeType::md_escape_dollar:
        case ::pltxt2htm::NodeType::md_escape_percent:
        case ::pltxt2htm::NodeType::md_escape_ampersand:
        case ::pltxt2htm::NodeType::md_escape_single_quote:
        case ::pltxt2htm::NodeType::md_escape_left_paren:
        case ::pltxt2htm::NodeType::md_escape_right_paren:
        case ::pltxt2htm::NodeType::md_escape_asterisk:
        case ::pltxt2htm::NodeType::md_escape_plus:
        case ::pltxt2htm::NodeType::md_escape_comma:
        case ::pltxt2htm::NodeType::md_escape_hyphen:
        case ::pltxt2htm::NodeType::md_escape_dot:
        case ::pltxt2htm::NodeType::md_escape_slash:
        case ::pltxt2htm::NodeType::md_escape_colon:
        case ::pltxt2htm::NodeType::md_escape_semicolon:
        case ::pltxt2htm::NodeType::md_escape_less_than:
        case ::pltxt2htm::NodeType::md_escape_equals:
        case ::pltxt2htm::NodeType::md_escape_greater_than:
        case ::pltxt2htm::NodeType::md_escape_question:
        case ::pltxt2htm::NodeType::md_escape_at:
        case ::pltxt2htm::NodeType::md_escape_left_bracket:
        case ::pltxt2htm::NodeType::md_escape_right_bracket:
        case ::pltxt2htm::NodeType::md_escape_caret:
        case ::pltxt2htm::NodeType::md_escape_underscore:
        case ::pltxt2htm::NodeType::md_escape_backtick:
        case ::pltxt2htm::NodeType::md_escape_left_brace:
        case ::pltxt2htm::NodeType::md_escape_pipe:
        case ::pltxt2htm::NodeType::md_escape_right_brace:
        case ::pltxt2htm::NodeType::md_escape_tilde:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
};

template<::pltxt2htm::Contracts ndebug>
class BasicFrameContext {
public:
    ::exception::optional<::pltxt2htm::details::ContextVariant<ndebug>> context_data;

    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr explicit BasicFrameContext() noexcept
        : context_data{::exception::nullopt_t{}} {
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_,
                                         ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}} {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Ast&& subast_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          subast(::std::move(subast_)) {
        bool const is_plain_pltext_type{::pltxt2htm::details::is_plain_pltext_type(nested_tag_type_)};
        pltxt2htm_assert(is_plain_pltext_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::fast_io::u8string&& id_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithEqualSignTagInfo{.pltext = pltext_, .id = ::std::move(id_)},
              nested_tag_type_}} {
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::pltxt2htm::Url&& url_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithExternalTagInfo{.pltext = pltext_, .url = ::std::move(url_)},
              nested_tag_type_}} {
        bool const is_external_tag_type{::pltxt2htm::details::is_external_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_external_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                         ::std::size_t id_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPlSizeTagInfo{.pltext = pltext_, .id = id_},
              nested_tag_type_}} {
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(nested_tag_type_)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"mismatch node type");
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string&& pltext_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdBlockQuotesInfo{::std::move(pltext_)},
              ::pltxt2htm::NodeType::md_block_quotes}} {
    }

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::Url&& link_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdLinkInfo{.pltext = pltext_, .link = ::std::move(link_)},
              ::pltxt2htm::NodeType::md_link}} {
    }

    constexpr explicit BasicFrameContext(::pltxt2htm::NodeType node_type,
                                         ::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithMdListInfo{::std::move(md_list_ast_)}, node_type}} {
        pltxt2htm_assert(node_type == ::pltxt2htm::NodeType::md_ul || node_type == ::pltxt2htm::NodeType::md_ol,
                         u8"mismatch node type");
    }

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept = delete;

    constexpr BasicFrameContext(::pltxt2htm::details::BasicFrameContext<ndebug>&& other) noexcept
        : context_data{::std::move(other.context_data)},
          current_index{other.current_index},
          subast(::std::move(other.subast)) {
    }

    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug> const&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::details::BasicFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::BasicFrameContext<ndebug>& = delete;

    constexpr ~BasicFrameContext() noexcept = default;

    [[nodiscard]]
    constexpr auto has_nested_tag_type(this auto&& self) noexcept -> bool {
        return self.context_data.has_value();
    }

    [[nodiscard]]
    constexpr auto get_nested_tag_type(this auto&& self) noexcept -> ::pltxt2htm::NodeType {
        bool const has_nested_tag_type{self.context_data.has_value()};
        pltxt2htm_assert(has_nested_tag_type, u8"nested tag type is empty");
        return self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().kind;
    }

    [[nodiscard]]
    constexpr auto get_nested_tag_type_optional(this auto&& self) noexcept
        -> ::exception::optional<::pltxt2htm::NodeType> {
        if (!self.context_data.has_value()) {
            return ::exception::nullopt_t{};
        }
        return self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>().kind;
    }

    [[nodiscard]]
    constexpr auto get_pltext(this auto&& self) noexcept -> ::fast_io::u8string_view {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto const& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        if (::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)) {
            return context_data_ref.equal_sign_tag.pltext;
        }
        if (::pltxt2htm::details::is_external_tag_type(context_data_ref.kind)) {
            return context_data_ref.external_tag.pltext;
        }
        if (::pltxt2htm::details::is_pl_size_tag_type(context_data_ref.kind)) {
            return context_data_ref.pl_size_tag.pltext;
        }
        if (::pltxt2htm::details::is_md_block_quotes_type(context_data_ref.kind)) {
            return ::fast_io::u8string_view{context_data_ref.md_block_quotes.pltext.data(),
                                            context_data_ref.md_block_quotes.pltext.size()};
        }
        if (::pltxt2htm::details::is_md_link_type(context_data_ref.kind)) {
            return context_data_ref.md_link.pltext;
        }
        if (::pltxt2htm::details::is_md_list_type(context_data_ref.kind)) {
            [[unlikely]] { ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>(); }
        }
        return context_data_ref.pltext.pltext;
    }

    constexpr auto&& get_equal_sign_tag_id(this auto&& self) noexcept {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id);
    }

    constexpr auto&& get_external_tag_url(this auto&& self) noexcept {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_external_tag_type{::pltxt2htm::details::is_external_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_external_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.external_tag.url);
    }

    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return context_data_ref.pl_size_tag.id;
    }

    constexpr auto&& get_md_link_url(this auto&& self) noexcept {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_md_link_type{::pltxt2htm::details::is_md_link_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_link.link);
    }

    constexpr auto&& get_md_list_ast(this auto&& self) noexcept {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.md_list_ast);
    }

    constexpr auto&& get_md_list_iter(this auto&& self) noexcept {
        bool const has_context_data{self.context_data.has_value()};
        pltxt2htm_assert(has_context_data, u8"context data is empty");
        auto&& context_data_ref = self.context_data.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.iter);
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
