#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "md_list.hh"
#include "../../astnode/basic.hh"

namespace pltxt2htm::details {

class BasicFrameContext {
public:
    ::pltxt2htm::NodeType const nested_tag_type;
    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr ~BasicFrameContext() noexcept = default;

protected:
    constexpr BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : nested_tag_type{nested_tag_type_} {
    }

    constexpr BasicFrameContext(::pltxt2htm::NodeType const nested_tag_type_, ::pltxt2htm::Ast&& subast_) noexcept
        : nested_tag_type{nested_tag_type_},
          subast(::std::move(subast_)) {
    }
};

class BareTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;

    constexpr BareTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(nested_tag_type_),
          pltext(pltext_) {
    }

    constexpr BareTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                             ::pltxt2htm::Ast&& subast_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(nested_tag_type_, ::std::move(subast_)),
          pltext(pltext_) {
    }

    constexpr ~BareTagContext() noexcept = default;
};

class EqualSignTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string id;

    constexpr EqualSignTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                  ::fast_io::u8string&& id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{nested_tag_type_},
          pltext(pltext_),
          id(::std::move(id_)) {
    }

    constexpr ~EqualSignTagContext() noexcept = default;
};

class PlSizeTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;
    ::std::size_t id;

    constexpr PlSizeTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                               ::std::size_t id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{nested_tag_type_},
          pltext(pltext_),
          id(::std::move(id_)) {
    }

    constexpr ~PlSizeTagContext() noexcept = default;
};

class MdBlockQuotesContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string pltext;

    constexpr explicit MdBlockQuotesContext(::fast_io::u8string&& pltext_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_block_quotes},
          pltext(::std::move(pltext_)) {
    }

    constexpr ~MdBlockQuotesContext() noexcept = default;
};

class MdLinkContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;
    ::fast_io::u8string link;

    // TODO can constructor be marked with explicit?
    constexpr MdLinkContext(::fast_io::u8string_view pltext_, ::fast_io::u8string&& link_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_link},
          pltext(::std::move(pltext_)),
          link(::std::move(link_)) {
    }

    constexpr ~MdLinkContext() noexcept = default;
};

class MdUlContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::pltxt2htm::details::MdListAst md_list_ast;
    ::pltxt2htm::details::MdListAst::iterator iter;

    constexpr explicit MdUlContext(::pltxt2htm::details::MdListAst&& md_list_ast_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_ul},
          md_list_ast(::std::move(md_list_ast_)),
          iter(this->md_list_ast.begin()) {
    }

    constexpr MdUlContext(::pltxt2htm::details::MdUlContext&& other) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_ul},
          md_list_ast(::std::move(other.md_list_ast)),
          iter(::std::move(other.iter)) {
    }

    constexpr ~MdUlContext() noexcept = default;
};

} // namespace pltxt2htm::details
