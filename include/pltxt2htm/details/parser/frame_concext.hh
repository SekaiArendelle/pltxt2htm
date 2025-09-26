#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../astnode/basic.hh"

namespace pltxt2htm::details {

class BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::NodeType const nested_tag_type;
    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr ~BasicFrameContext() noexcept = default;

protected:
    constexpr BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : pltext(pltext_),
          nested_tag_type{nested_tag_type_} {
    }
};

class BareTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    constexpr BareTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(pltext_, nested_tag_type_) {
    }

    constexpr ~BareTagContext() noexcept = default;
};

class EqualSignTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string id;

    constexpr EqualSignTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                  ::fast_io::u8string&& id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{pltext_, nested_tag_type_},
          id(::std::move(id_)) {
    }

    constexpr ~EqualSignTagContext() noexcept = default;
};

class PlSizeTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::std::size_t id;

    constexpr PlSizeTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                               ::std::size_t id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{pltext_, nested_tag_type_},
          id(::std::move(id_)) {
    }

    constexpr ~PlSizeTagContext() noexcept = default;
};

} // namespace pltxt2htm::details
