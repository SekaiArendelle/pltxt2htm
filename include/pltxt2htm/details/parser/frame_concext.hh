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
        switch (this->kind) /* -Werror=switch */ {
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
        case ::pltxt2htm::NodeType::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br:
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
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6:
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
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_block: {
            ::std::construct_at(::std::addressof(this->pltext), ::std::move(other.pltext));
            return;
        }
        case ::pltxt2htm::NodeType::base:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than:
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
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_single_quote:
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
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_greater_than:
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
        switch (this->kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_discussion:
            [[fallthrough]];
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
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_ol: {
            ::std::destroy_at(::std::addressof(this->md_list));
            return;
        }
        case ::pltxt2htm::NodeType::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br:
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
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6:
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
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_block: {
            ::std::destroy_at(::std::addressof(this->pltext));
            return;
        }
        case ::pltxt2htm::NodeType::base:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than:
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
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_single_quote:
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
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_greater_than:
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
        case ::pltxt2htm::NodeType::md_hr:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
};

template<::pltxt2htm::Contracts ndebug>
class BasicFrameContext {
    ::pltxt2htm::details::ContextVariant<ndebug> context_data;
public:

    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr explicit BasicFrameContext(::fast_io::u8string_view pltext_,
                                         ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : context_data{::pltxt2htm::details::ContextVariant<ndebug>{
              ::pltxt2htm::details::ParserFrameContextWithPltextInfo{pltext_}, nested_tag_type_}},
          subast{} {
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
    constexpr auto get_nested_tag_type(this auto&& self) noexcept -> ::pltxt2htm::NodeType {
        return self.context_data.kind;
    }

    [[nodiscard]]
    constexpr auto get_pltext(this auto&& self) noexcept -> ::fast_io::u8string_view {
        auto const& context_data_ref = self.context_data;
        switch (context_data_ref.kind) /* -Werror=switch */ {
        case ::pltxt2htm::NodeType::base:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::invalid_u8char:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::space:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::tab:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br:
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
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_em:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6:
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
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_image:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_block: {
            return context_data_ref.pltext.pltext;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_user: {
            return context_data_ref.equal_sign_tag.pltext;
        }
        case ::pltxt2htm::NodeType::pl_external: {
            return context_data_ref.external_tag.pltext;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            return context_data_ref.pl_size_tag.pltext;
        }
        case ::pltxt2htm::NodeType::md_block_quotes: {
            auto const& pltext = context_data_ref.md_block_quotes.pltext;
            return ::fast_io::u8string_view{pltext.data(), pltext.size()};
        }
        case ::pltxt2htm::NodeType::md_link: {
            return context_data_ref.md_link.pltext;
        }
        case ::pltxt2htm::NodeType::pl_macro_project:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_visitor:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_author:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_macro_coauthors:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_backslash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_exclamation:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_hash:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_dollar:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_percent:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_single_quote:
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
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_equals:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_escape_greater_than:
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
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_ol:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        // suppress GCC -Wreturn-type warning
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    constexpr auto&& get_equal_sign_tag_id(this auto&& self) noexcept {
        auto&& context_data_ref = self.context_data;
        bool const is_equal_sign_tag_type{::pltxt2htm::details::is_equal_sign_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_equal_sign_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.equal_sign_tag.id);
    }

    constexpr auto&& get_external_tag_url(this auto&& self) noexcept {
        auto&& context_data_ref = self.context_data;
        bool const is_external_tag_type{::pltxt2htm::details::is_external_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_external_tag_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.external_tag.url);
    }

    constexpr auto get_pl_size_tag_id(this auto&& self) noexcept -> ::std::size_t {
        auto&& context_data_ref = self.context_data;
        bool const is_pl_size_tag_type{::pltxt2htm::details::is_pl_size_tag_type(context_data_ref.kind)};
        pltxt2htm_assert(is_pl_size_tag_type, u8"context kind mismatch");
        return context_data_ref.pl_size_tag.id;
    }

    constexpr auto&& get_md_link_url(this auto&& self) noexcept {
        auto&& context_data_ref = self.context_data;
        bool const is_md_link_type{::pltxt2htm::details::is_md_link_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_link_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_link.link);
    }

    constexpr auto&& get_md_list_ast(this auto&& self) noexcept {
        auto&& context_data_ref = self.context_data;
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.md_list_ast);
    }

    constexpr auto&& get_md_list_iter(this auto&& self) noexcept {
        auto&& context_data_ref = self.context_data;
        bool const is_md_list_type{::pltxt2htm::details::is_md_list_type(context_data_ref.kind)};
        pltxt2htm_assert(is_md_list_type, u8"context kind mismatch");
        return ::std::forward_like<decltype(self)>(context_data_ref.md_list.iter);
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
