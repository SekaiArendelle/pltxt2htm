#include <concepts>
#include <type_traits>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <exception/exception.hh>
#include <pltxt2htm/astnode/node_type.hh>
#include <pltxt2htm/astnode/physics_lab_node.hh>

int main() {
    static_assert(::std::is_copy_constructible_v<::pltxt2htm::details::HeapGuard<::pltxt2htm::NodeType>>);
    static_assert(::std::movable<::pltxt2htm::details::HeapGuard<::pltxt2htm::NodeType>>);

    ::pltxt2htm::Ast text{};
    text.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8't'});
    text.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8'e'});
    text.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8's'});
    text.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8't'});

    ::fast_io::vector<::pltxt2htm::PlTxtNode> arr{
        ::pltxt2htm::U8Char{u8'a'},
        ::pltxt2htm::Color{::pltxt2htm::Ast(text), ::fast_io::u8string{u8"red"}},
        ::pltxt2htm::Experiment{::pltxt2htm::Ast(text), ::fast_io::u8string{u8"123"}},
        ::pltxt2htm::Discussion{::pltxt2htm::Ast(text), ::fast_io::u8string{u8"123"}},
    };

    ::exception::assert_true(arr[0].node_type() == ::pltxt2htm::NodeType::u8char);
    ::exception::assert_true(arr[1].node_type() == ::pltxt2htm::NodeType::pl_color);
    ::exception::assert_true(arr[2].node_type() == ::pltxt2htm::NodeType::pl_experiment);
    ::exception::assert_true(arr[3].node_type() == ::pltxt2htm::NodeType::pl_discussion);

    return 0;
}
