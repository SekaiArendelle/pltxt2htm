#pragma once

#include "doctest_config.hh"

#include <cstddef>
#include <pltxt2htm/ast/ast.hh>

#if __cpp_impl_reflection >= 202506L
    #include <meta>
#endif

#if __cpp_impl_reflection >= 202506L
using Storage = ::pltxt2htm::details::PlTxtNodeStorage;
using Text = ::pltxt2htm::Text;

[[nodiscard]]
consteval auto largest_other_node_size() noexcept -> ::std::size_t {
    ::std::size_t result{};
    template for (constexpr auto member : ::std::define_static_array(
                      ::std::meta::nonstatic_data_members_of(^^Storage, ::std::meta::access_context::unprivileged()))) {
        constexpr auto member_type = ::std::meta::type_of(member);
        if constexpr (member_type == ^^Text) {
            continue;
        }
        auto const member_size = ::std::meta::size_of(member_type);
        if (result < member_size) {
            result = member_size;
        }
    }
    return result;
}

static_assert(sizeof(Text) == largest_other_node_size());
#else
    #warning "node_layout reflection test skipped: requires GCC 16 or newer with C++26"
#endif
