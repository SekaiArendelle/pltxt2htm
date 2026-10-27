#pragma once

#include "doctest_config.hh"

#include <cstddef>
#include <pltxt2htm/ast/ast.hh>

#if __cpp_impl_reflection >= 202506L
    #include <meta>
#endif

TEST_SUITE("node_layout") {
#if __cpp_impl_reflection >= 202506L
    using Storage = ::pltxt2htm::details::PlTxtNodeStorage;
    using CodeFence = ::pltxt2htm::CodeFence;

    [[nodiscard]]
    consteval auto largest_node_size() noexcept -> ::std::size_t {
        ::std::size_t result{};
        template for (constexpr auto member : ::std::define_static_array(::std::meta::nonstatic_data_members_of(
                          ^^Storage, ::std::meta::access_context::unprivileged()))) {
            auto const member_size = ::std::meta::size_of(::std::meta::type_of(member));
            if (result < member_size) {
                result = member_size;
            }
        }
        return result;
    }

    static_assert(sizeof(CodeFence) == largest_node_size());
    static_assert(sizeof(Storage) == largest_node_size());
    static_assert(sizeof(::pltxt2htm::PlTxtNode) == sizeof(CodeFence) + sizeof(::pltxt2htm::NodeKind) + 4);
#else
    #warning "node_layout reflection test skipped: requires GCC 16 or newer with C++26"
#endif
}
