#pragma once

#include "doctest_config.hh"

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/vector.hh>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/details/parser/try_parse.hh>

TEST_SUITE("node_kind") {
    using TestNode = ::pltxt2htm::PlTxtNode;

    // PlObjectId has no public constructor, so the node-building cases go through the factory.
    constexpr auto make_object_id(::pltxt2htm::container::U8StringView const text) -> ::pltxt2htm::PlObjectId {
        return ::pltxt2htm::PlObjectId::try_make(text).value();
    }

    consteval auto test_md_escape_result() -> bool {
        auto const trailing = ::pltxt2htm::details::try_parse_md_escape(u8"\\");
        if (trailing.has_value() == false) {
            return false;
        }
        auto const& trailing_result = trailing.value();
        if (trailing_result.character != u8'\\' ||
            trailing_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
            trailing_result.advance_count != 1) {
            return false;
        }

        auto const invalid = ::pltxt2htm::details::try_parse_md_escape(u8"\\a");
        if (invalid.has_value() == false) {
            return false;
        }
        auto const& invalid_result = invalid.value();
        if (invalid_result.character != u8'\\' ||
            invalid_result.kind != ::pltxt2htm::details::MdEscapeKind::literal_backslash ||
            invalid_result.advance_count != 1) {
            return false;
        }

        auto const escaped = ::pltxt2htm::details::try_parse_md_escape(u8"\\*");
        if (escaped.has_value() == false) {
            return false;
        }
        auto const& escaped_result = escaped.value();
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
        auto node = TestNode::emplace<::pltxt2htm::Text>(u8'x');
        return node.get_node_kind() == ::pltxt2htm::NodeKind::text && node.as_text()[0] == u8'x';
    }

    [[nodiscard]]
    constexpr auto test_constexpr_move() noexcept -> bool {
        auto source = TestNode::emplace<::pltxt2htm::Text>(u8'x');
        auto moved = TestNode{::std::move(source)};
        return moved.get_node_kind() == ::pltxt2htm::NodeKind::text && moved.as_text()[0] == u8'x';
    }

    static_assert(CanEmplaceTestNode<::pltxt2htm::Text, char8_t>);
    static_assert(CanEmplaceTestNode<::pltxt2htm::InvalidUtf8>);
    static_assert(!CanEmplaceTestNode<int>);
    static_assert(::std::is_standard_layout_v<TestNode>);
    static_assert(noexcept(TestNode::emplace<::pltxt2htm::Text>(u8'x')));
    static_assert(test_constexpr_emplace());
    static_assert(test_constexpr_move());
    static_assert(::pltxt2htm::details::PlTxtNodeTraits<::pltxt2htm::Text>::kind == ::pltxt2htm::NodeKind::text);
    static_assert(test_md_escape_result());
    static_assert(::std::is_trivially_copyable_v<::pltxt2htm::details::TryParseMdEscapeResult>);
    static_assert(sizeof(::pltxt2htm::details::TryParseMdEscapeResult) <= sizeof(::std::size_t) * 2);
    static_assert(::std::movable<::pltxt2htm::PlTxtNode>);

    TEST_CASE("node-kinds-and-storage-indexing") {
        ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode> arr{};
        arr.emplace_back(TestNode::template emplace<::pltxt2htm::Text>(u8'a'));
        arr.emplace_back(TestNode::template emplace<::pltxt2htm::UnityColor>(
            ::pltxt2htm::Ast{}, ::pltxt2htm::container::U8String{u8"red"}));
        arr.emplace_back(TestNode::template emplace<::pltxt2htm::PlExperiment>(
            ::pltxt2htm::Ast{}, make_object_id(u8"642cf37a494746375aae306a")));
        arr.emplace_back(TestNode::template emplace<::pltxt2htm::PlDiscussion>(
            ::pltxt2htm::Ast{}, make_object_id(u8"642cf37a494746375aae306a")));

        auto invalid_utf8 = TestNode::emplace<::pltxt2htm::InvalidUtf8>();
        auto unity_color =
            TestNode::emplace<::pltxt2htm::UnityColor>(::pltxt2htm::Ast{}, ::pltxt2htm::container::U8String{u8"blue"});

        CHECK(arr[0].get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(arr[1].get_node_kind() == ::pltxt2htm::NodeKind::unity_color);
        CHECK(arr[2].get_node_kind() == ::pltxt2htm::NodeKind::pl_experiment);
        CHECK(arr[3].get_node_kind() == ::pltxt2htm::NodeKind::pl_discussion);
        CHECK(invalid_utf8.get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
        CHECK(unity_color.get_node_kind() == ::pltxt2htm::NodeKind::unity_color);
        CHECK(unity_color.as_unity_color().get_color() == ::pltxt2htm::container::U8String{u8"blue"});
    }
}
