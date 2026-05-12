#include <concepts>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include <pltxt2htm/ast/ast.hh>

int main() {
    static_assert(::std::movable<::pltxt2htm::ast2::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>>);

    ::fast_io::vector<::pltxt2htm::ast2::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>> arr{};
    arr.emplace_back(::pltxt2htm::ast2::U8Char{u8'a'});
    arr.emplace_back(::pltxt2htm::ast2::PlColor<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::ast2::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::fast_io::u8string{u8"red"}});
    arr.emplace_back(::pltxt2htm::ast2::PlExperiment<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::ast2::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::fast_io::u8string{u8"123"}});
    arr.emplace_back(::pltxt2htm::ast2::PlDiscussion<::pltxt2htm::Contracts::quick_enforce>{
        ::pltxt2htm::ast2::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::fast_io::u8string{u8"123"}});

    ::exception::assert_true(arr[0].get_node_kind() == ::pltxt2htm::NodeType::u8char);
    ::exception::assert_true(arr[1].get_node_kind() == ::pltxt2htm::NodeType::pl_color);
    ::exception::assert_true(arr[2].get_node_kind() == ::pltxt2htm::NodeType::pl_experiment);
    ::exception::assert_true(arr[3].get_node_kind() == ::pltxt2htm::NodeType::pl_discussion);

    return 0;
}
