#include <concepts>
#include <type_traits>
#include <utility>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/vector.hh>

#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

namespace {

constexpr auto test_contracts = ::pltxt2htm::Contracts::quick_enforce;
using TestNode = ::pltxt2htm::PlTxtNode<test_contracts>;

template<typename Node, typename... Args>
concept CanEmplaceTestNode = requires(Args&&... args) {
    { TestNode::template emplace<Node>(::std::forward<Args>(args)...) } -> ::std::same_as<TestNode>;
};

[[nodiscard]]
constexpr auto test_constexpr_emplace() noexcept -> bool {
    auto node = TestNode::emplace<::pltxt2htm::U8Char>(u8'x');
    return node.get_node_kind() == ::pltxt2htm::NodeKind::u8char && node.as_u8char().chr == u8'x';
}

[[nodiscard]]
constexpr auto test_constexpr_move() noexcept -> bool {
    auto source = TestNode::emplace<::pltxt2htm::U8Char>(u8'x');
    auto moved = TestNode{::std::move(source)};
    return moved.get_node_kind() == ::pltxt2htm::NodeKind::u8char && moved.as_u8char().chr == u8'x';
}

static_assert(CanEmplaceTestNode<::pltxt2htm::U8Char, char8_t>);
static_assert(CanEmplaceTestNode<::pltxt2htm::InvalidUtf8>);
static_assert(!CanEmplaceTestNode<int>);
static_assert(::std::is_standard_layout_v<TestNode>);
static_assert(noexcept(TestNode::emplace<::pltxt2htm::U8Char>(u8'x')));
static_assert(test_constexpr_emplace());
static_assert(test_constexpr_move());
static_assert(::pltxt2htm::details::PlTxtNodeTraits<test_contracts, ::pltxt2htm::U8Char>::kind ==
              ::pltxt2htm::NodeKind::u8char);

} // namespace

int main() {
    static_assert(::std::movable<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>>);

    ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>> arr{};
    arr.emplace_back(TestNode::template emplace<::pltxt2htm::U8Char>(u8'a'));
    arr.emplace_back(TestNode::template emplace<::pltxt2htm::UnityColor<::pltxt2htm::Contracts::quick_enforce>>(
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"red"}));
    arr.emplace_back(TestNode::template emplace<::pltxt2htm::PlExperiment<::pltxt2htm::Contracts::quick_enforce>>(
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}));
    arr.emplace_back(TestNode::template emplace<::pltxt2htm::PlDiscussion<::pltxt2htm::Contracts::quick_enforce>>(
        ::pltxt2htm::Ast<::pltxt2htm::Contracts::quick_enforce>{}, ::pltxt2htm::container::U8String{u8"123"}));

    auto invalid_utf8 = TestNode::emplace<::pltxt2htm::InvalidUtf8>();
    auto unity_color = TestNode::emplace<::pltxt2htm::UnityColor<test_contracts>>(
        ::pltxt2htm::Ast<test_contracts>{}, ::pltxt2htm::container::U8String{u8"blue"});

    pltxt2htm_test_assert_true(arr.template index<::pltxt2htm::Contracts::quick_enforce>(0).get_node_kind() ==
                               ::pltxt2htm::NodeKind::u8char);
    pltxt2htm_test_assert_true(arr.template index<::pltxt2htm::Contracts::quick_enforce>(1).get_node_kind() ==
                               ::pltxt2htm::NodeKind::unity_color);
    pltxt2htm_test_assert_true(arr.template index<::pltxt2htm::Contracts::quick_enforce>(2).get_node_kind() ==
                               ::pltxt2htm::NodeKind::pl_experiment);
    pltxt2htm_test_assert_true(arr.template index<::pltxt2htm::Contracts::quick_enforce>(3).get_node_kind() ==
                               ::pltxt2htm::NodeKind::pl_discussion);
    pltxt2htm_test_assert_true(invalid_utf8.get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
    pltxt2htm_test_assert_true(unity_color.get_node_kind() == ::pltxt2htm::NodeKind::unity_color);
    pltxt2htm_test_assert_true(unity_color.as_unity_color().get_color() == ::pltxt2htm::container::U8String{u8"blue"});

    return 0;
}
