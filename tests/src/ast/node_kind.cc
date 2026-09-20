#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/vector.hh>

#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/details/parser/try_parse.hh>

namespace {

constexpr auto test_contracts = ::pltxt2htm::Contracts::quick_enforce;
using TestNode = ::pltxt2htm::PlTxtNode<test_contracts>;

consteval auto test_md_escape_result() -> bool {
    auto const trailing = ::pltxt2htm::details::try_parse_md_escape<test_contracts>(u8"\\");
    if (trailing.has_value() == false) {
        return false;
    }
    auto const& trailing_result = trailing.template value<test_contracts>();
    if (trailing_result.character != u8'\\' ||
        trailing_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
        trailing_result.advance_count != 1) {
        return false;
    }

    auto const invalid = ::pltxt2htm::details::try_parse_md_escape<test_contracts>(u8"\\a");
    if (invalid.has_value() == false) {
        return false;
    }
    auto const& invalid_result = invalid.template value<test_contracts>();
    if (invalid_result.character != u8'\\' ||
        invalid_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
        invalid_result.advance_count != 1) {
        return false;
    }

    auto const escaped = ::pltxt2htm::details::try_parse_md_escape<test_contracts>(u8"\\*");
    if (escaped.has_value() == false) {
        return false;
    }
    auto const& escaped_result = escaped.template value<test_contracts>();
    return escaped_result.character == u8'*' &&
           escaped_result.kind == ::pltxt2htm::details::MdEscapeKind::escaped_punctuation &&
           escaped_result.advance_count == 2;
}

template<typename Node, typename... Args>
concept CanEmplaceTestNode = requires(Args&&... args) {
    { TestNode::template emplace<Node>(::std::forward<Args>(args)...) } -> ::std::same_as<TestNode>;
};

[[nodiscard]]
constexpr auto test_constexpr_emplace() noexcept -> bool {
    auto node = TestNode::emplace<::pltxt2htm::Text<test_contracts>>(u8'x');
    return node.get_node_kind() == ::pltxt2htm::NodeKind::text && node.as_text().index(0) == u8'x';
}

[[nodiscard]]
constexpr auto test_constexpr_move() noexcept -> bool {
    auto source = TestNode::emplace<::pltxt2htm::Text<test_contracts>>(u8'x');
    auto moved = TestNode{::std::move(source)};
    return moved.get_node_kind() == ::pltxt2htm::NodeKind::text && moved.as_text().index(0) == u8'x';
}

static_assert(CanEmplaceTestNode<::pltxt2htm::Text<test_contracts>, char8_t>);
static_assert(CanEmplaceTestNode<::pltxt2htm::InvalidUtf8>);
static_assert(!CanEmplaceTestNode<int>);
static_assert(::std::is_standard_layout_v<TestNode>);
static_assert(noexcept(TestNode::emplace<::pltxt2htm::Text<test_contracts>>(u8'x')));
static_assert(test_constexpr_emplace());
static_assert(test_constexpr_move());
static_assert(::pltxt2htm::details::PlTxtNodeTraits<test_contracts, ::pltxt2htm::Text<test_contracts>>::kind ==
              ::pltxt2htm::NodeKind::text);
static_assert(test_md_escape_result());
static_assert(::std::is_trivially_copyable_v<::pltxt2htm::details::TryParseMdEscapeResult>);
static_assert(sizeof(::pltxt2htm::details::TryParseMdEscapeResult) <= sizeof(::std::size_t) * 2);

} // namespace

int main() {
    static_assert(::std::movable<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>>);

    ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode<::pltxt2htm::Contracts::quick_enforce>> arr{};
    arr.emplace_back(TestNode::template emplace<::pltxt2htm::Text<test_contracts>>(u8'a'));
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
                               ::pltxt2htm::NodeKind::text);
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
