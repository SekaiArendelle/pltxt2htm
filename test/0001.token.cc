#include <concepts>
#include <pltxt2htm/astnode/enum.hh>
#include <pltxt2htm/astnode/plext.hh>
#include <fast_io/fast_io_dsal/vector.h>
#include <exception/exception.hh>

int main() noexcept {
    ::fast_io::vector<::pltxt2htm::PlTxtNode> arr{
        ::pltxt2htm::U8Char{u8'a'},
        ::pltxt2htm::Color{::pltxt2htm::U8Char{u8't'}, ::pltxt2htm::U8Char{u8'e'}, ::pltxt2htm::U8Char{u8's'},
                           ::pltxt2htm::U8Char{u8't'}},
        ::pltxt2htm::A{::pltxt2htm::U8Char{u8't'}, ::pltxt2htm::U8Char{u8'e'}, ::pltxt2htm::U8Char{u8's'},
        ::pltxt2htm::U8Char{u8't'}},
        ::pltxt2htm::Experiment{::pltxt2htm::U8Char{u8't'}, ::pltxt2htm::U8Char{u8'e'}, ::pltxt2htm::U8Char{u8's'},
        ::pltxt2htm::U8Char{u8't'}},
        ::pltxt2htm::Discussion{::pltxt2htm::U8Char{u8't'}, ::pltxt2htm::U8Char{u8'e'}, ::pltxt2htm::U8Char{u8's'},
        ::pltxt2htm::U8Char{u8't'}},
    };

    ::exception::assert_true(arr[0].node_type() == ::pltxt2htm::NodeType::u8char);
    ::exception::assert_true(arr[1].node_type() == ::pltxt2htm::NodeType::color);
    ::exception::assert_true(arr[2].node_type() == ::pltxt2htm::NodeType::a);
    ::exception::assert_true(arr[3].node_type() == ::pltxt2htm::NodeType::experiment);
    ::exception::assert_true(arr[4].node_type() == ::pltxt2htm::NodeType::discussion);

    return 0;
}
