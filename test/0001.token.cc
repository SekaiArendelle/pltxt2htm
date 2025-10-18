#include <concepts>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include <pltxt2htm/astnode/node_type.hh>
#include <pltxt2htm/astnode/physics_lab_node.hh>

int main() {
    static_assert(::std::movable<::pltxt2htm::HeapGuard<::pltxt2htm::NodeType>>);

    ::fast_io::vector<::pltxt2htm::PlTxtNode> arr{
        ::pltxt2htm::U8Char{u8'a'},
        ::pltxt2htm::Color{::pltxt2htm::Ast{}, ::fast_io::u8string{u8"red"}},
        ::pltxt2htm::Experiment{::pltxt2htm::Ast{}, ::fast_io::u8string{u8"123"}},
        ::pltxt2htm::Discussion{::pltxt2htm::Ast{}, ::fast_io::u8string{u8"123"}},
    };

    ::exception::assert_true(arr[0].node_type() == ::pltxt2htm::NodeType::u8char);
    ::exception::assert_true(arr[1].node_type() == ::pltxt2htm::NodeType::pl_color);
    ::exception::assert_true(arr[2].node_type() == ::pltxt2htm::NodeType::pl_experiment);
    ::exception::assert_true(arr[3].node_type() == ::pltxt2htm::NodeType::pl_discussion);

    return 0;
}
