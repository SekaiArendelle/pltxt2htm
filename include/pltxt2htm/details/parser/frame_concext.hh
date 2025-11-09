#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
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
    ::fast_io::u8string_view link;

    constexpr MdLinkContext(::fast_io::u8string_view pltext_, ::fast_io::u8string_view link_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{::pltxt2htm::NodeType::md_link},
          pltext(::std::move(pltext_)),
          link(::std::move(link_)) {
    }

    constexpr ~MdLinkContext() noexcept = default;
};

} // namespace pltxt2htm::details
