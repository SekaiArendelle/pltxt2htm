#include <concepts>
#include <fast_io/fast_io_dsal/vector.h>
#include <pltxt2htm/container/string.hh>

#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

int main() {
    static_assert(::std::movable<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>>);

    ::fast_io::vector<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>> arr{};
    arr.emplace_back(::pltxt2htm::U8Char{u8'a'});
    arr.emplace_back(::pltxt2htm::PlColor<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"red"}});
    arr.emplace_back(::pltxt2htm::PlExperiment<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}});
    arr.emplace_back(::pltxt2htm::PlDiscussion<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}});

    pltxt2htm_test_assert_true(arr[0].get_node_kind() == ::pltxt2htm::NodeKind::u8char);
    pltxt2htm_test_assert_true(arr[1].get_node_kind() == ::pltxt2htm::NodeKind::pl_color);
    pltxt2htm_test_assert_true(arr[2].get_node_kind() == ::pltxt2htm::NodeKind::pl_experiment);
    pltxt2htm_test_assert_true(arr[3].get_node_kind() == ::pltxt2htm::NodeKind::pl_discussion);

    return 0;
}
