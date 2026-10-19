#pragma once

#include "doctest_config.hh"

#include <type_traits>
#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("internal_use") {
    using nd = ::pltxt2htm::Contracts;

    TEST_CASE("internal-node-is-move-only") {
        static_assert(!::std::is_copy_constructible_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>);
        static_assert(!::std::is_copy_assignable_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>);
        static_assert(::std::is_move_constructible_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>);
        static_assert(::std::is_move_assignable_v<::pltxt2htm::PlTxtNode<nd::quick_enforce>>);
    }
}
