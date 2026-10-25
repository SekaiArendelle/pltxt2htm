#pragma once

#include "doctest_config.hh"

#include <type_traits>
#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("internal_use") {

    static_assert(!::std::is_copy_constructible_v<::pltxt2htm::PlTxtNode>);
    static_assert(!::std::is_copy_assignable_v<::pltxt2htm::PlTxtNode>);
    static_assert(::std::is_move_constructible_v<::pltxt2htm::PlTxtNode>);
    static_assert(::std::is_move_assignable_v<::pltxt2htm::PlTxtNode>);
}
