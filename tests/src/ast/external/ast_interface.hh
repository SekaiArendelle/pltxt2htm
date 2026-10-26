#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("ast_interface") {
    using Node = ::pltxt2htm::PlTxtNode;
    using Ast = ::pltxt2htm::Ast;
    using AstBearingNode = ::pltxt2htm::Group;
    using BackingVector = ::pltxt2htm::container::Vector<Node>;

    [[nodiscard]]
    constexpr auto text_node(char8_t const code_unit) noexcept -> Node {
        return Node::template emplace<::pltxt2htm::Text>(code_unit);
    }

    // Ast wraps the backing vector without adding storage, and mirrors its types and value semantics.
    static_assert(sizeof(Ast) == sizeof(BackingVector));
    static_assert(::std::is_nothrow_move_constructible_v<Ast>);
    static_assert(::std::is_nothrow_move_assignable_v<Ast>);
    static_assert(::std::is_nothrow_copy_constructible_v<Ast>);
    static_assert(::std::is_nothrow_copy_assignable_v<Ast>);
    static_assert(::std::is_nothrow_copy_constructible_v<AstBearingNode>);
    static_assert(::std::is_nothrow_copy_assignable_v<AstBearingNode>);
    static_assert(::std::is_nothrow_destructible_v<Ast>);
    static_assert(::std::is_same_v<Ast::allocator_type, BackingVector::allocator_type>);
    static_assert(::std::is_same_v<Ast::value_type, Node>);
    static_assert(::std::is_same_v<Ast::size_type, ::std::size_t>);
    static_assert(::std::is_same_v<Ast::difference_type, ::std::ptrdiff_t>);
    static_assert(::std::is_same_v<Ast::reference, Node&>);
    static_assert(::std::is_same_v<Ast::const_reference, Node const&>);
    static_assert(::std::is_same_v<Ast::pointer, Node*>);
    static_assert(::std::is_same_v<Ast::const_pointer, Node const*>);
    static_assert(::std::is_same_v<Ast::iterator, Node*>);
    static_assert(::std::is_same_v<Ast::const_iterator, Node const*>);
    static_assert(::std::is_same_v<Ast::reverse_iterator, ::std::reverse_iterator<Node*>>);
    static_assert(::std::is_same_v<Ast::const_reverse_iterator, ::std::reverse_iterator<Node const*>>);

    // An empty AST
    TEST_CASE("empty-ast") {
        Ast ast{};
        CHECK(ast.is_empty());
        CHECK(ast.empty());
        CHECK(ast.size() == 0);
        CHECK(ast.capacity() == 0);
        CHECK(ast.data() == nullptr);
        CHECK(ast.begin() == ast.end());
        CHECK(ast.cbegin() == ast.cend());
        CHECK(ast.rbegin() == ast.rend());
        CHECK(Ast::max_size() == BackingVector::max_size());
    }

    // Construction from an initializer list, and the read-only accessors of a const AST
    TEST_CASE("initializer-list-and-const-accessors") {
        Ast const ast{text_node(u8'a'), text_node(u8'b')};
        CHECK(ast.size() == 2);
        CHECK(ast.empty() == false);
        CHECK(ast.is_empty() == false);
        CHECK(ast.capacity() >= 2);
        CHECK(ast.data() != nullptr);
        CHECK(ast.data() == ast.cbegin());
        CHECK(ast.begin() == ast.cbegin());
        CHECK(ast.end() == ast.cend());
        CHECK(ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast.front().as_text().index(0) == u8'a');
        CHECK(ast.index(1).as_text().index(0) == u8'b');
    }

    // Appending, removing and reusing the storage
    TEST_CASE("append-remove-and-reuse-storage") {
        Ast ast{};
        Node const node{text_node(u8'x')};
        ast.push_back(node);
        ast.push_back(text_node(u8'y'));
        auto const& appended = ast.emplace_back(text_node(u8'z'));
        CHECK(ast.size() == 3);
        CHECK(::std::addressof(appended) == ast.data() + 2);

        ast.pop_back();
        CHECK(ast.size() == 2);
        CHECK(ast.front().as_text().index(0) == u8'x');

        ast.reserve(16);
        auto const reserved_capacity = ast.capacity();
        CHECK(reserved_capacity >= 16);
        CHECK(ast.size() == 2);

        ast.clear();
        CHECK(ast.is_empty());
        CHECK(ast.capacity() == reserved_capacity);

        Ast single{text_node(u8'q')};
        single.pop_back();
        CHECK(single.is_empty());
    }

    // Text appending coalesces adjacent code units and starts a node after non-text content
    TEST_CASE("text-appending-coalesces-adjacent-code-units") {
        Ast ast{};
        ast.append_text(u8'a');
        ast.append_text(u8"bc");
        ast.append_text(::pltxt2htm::container::U8StringView{});
        CHECK(ast.size() == 1);
        CHECK(ast.front().as_text().size() == 3);
        CHECK(ast.front().as_text().index(0) == u8'a');
        CHECK(ast.front().as_text().index(2) == u8'c');

        ast.push_back(Node::template emplace<::pltxt2htm::LineBreak>());
        ast.append_text(u8'd');
        CHECK(ast.size() == 3);
        CHECK(ast.index(2).as_text().index(0) == u8'd');
    }

    // Semantic code points become dedicated nodes or coalesced UTF-8 text.
    TEST_CASE("semantic-code-points") {
        Ast ast{};
        ast.append_code_point(U'A');
        ast.append_code_point(char32_t{0x20AC});
        ast.append_code_point(U' ');
        ast.append_code_point(char32_t{0xA0});
        ast.append_code_point(U'\n');
        ast.append_code_point(U'&');
        ast.append_code_point(char32_t{0x7F});
        ast.append_code_point(char32_t{0xD800});

        CHECK(ast.size() == 7);
        CHECK(ast.index(0).as_text().size() == 4);
        CHECK(ast.index(0).as_text().index(0) == u8'A');
        CHECK(ast.index(0).as_text().index(1) == char8_t{0xE2});
        CHECK(ast.index(1).get_node_kind() == ::pltxt2htm::NodeKind::space);
        CHECK(ast.index(2).get_node_kind() == ::pltxt2htm::NodeKind::space);
        CHECK(ast.index(3).get_node_kind() == ::pltxt2htm::NodeKind::line_break);
        CHECK(ast.index(4).get_node_kind() == ::pltxt2htm::NodeKind::ampersand);
        CHECK(ast.index(5).get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
        CHECK(ast.index(6).get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
    }

    // Iteration, reverse iteration and range-based for
    TEST_CASE("iteration-and-reverse-iteration") {
        Ast ast{text_node(u8'a'), text_node(u8'b'), text_node(u8'c')};

        ::std::size_t visited{};
        for (auto const& node : ast) {
            CHECK(node.get_node_kind() == ::pltxt2htm::NodeKind::text);
            ++visited;
        }
        CHECK(visited == 3);

        CHECK(ast.begin() != ast.end());
        CHECK(ast.end() - ast.begin() == 3);
        CHECK(*ast.rbegin() == ast.index(2));
        CHECK(ast.rend() == ast.rbegin() + 3);
    }

    // Erasing a single node, an empty range, and a non-empty range
    TEST_CASE("erase-single-node-and-ranges") {
        Ast ast{text_node(u8'a'), text_node(u8'b'), text_node(u8'c')};

        auto const next = ast.erase(ast.begin());
        CHECK(ast.size() == 2);
        CHECK(next == ast.begin());
        CHECK(ast.front().as_text().index(0) == u8'b');

        auto const unchanged = ast.erase(ast.end(), ast.end());
        CHECK(unchanged == ast.end());
        CHECK(ast.size() == 2);

        auto const after = ast.erase(ast.begin(), ast.begin() + 1);
        CHECK(ast.size() == 1);
        CHECK(after == ast.begin());
        CHECK(ast.front().as_text().index(0) == u8'c');
    }

    // Appending a whole range, from an rvalue, an lvalue and an empty source
    TEST_CASE("append-ranges-from-lvalue-and-rvalue") {
        Ast target{};
        Ast moved_source{text_node(u8'a')};
        target.append_range(::std::move(moved_source));
        CHECK(target.size() == 1);

        Ast copied_source{text_node(u8'b')};
        target.append_range(copied_source);
        CHECK(target.size() == 2);
        CHECK(copied_source.size() == 1);

        Ast const empty_source{};
        target.append_range(empty_source);
        CHECK(target.size() == 2);
    }

    // Copy and move preserve the node sequence, and the clone stays independent
    TEST_CASE("copy-move-and-independent-clone") {
        Ast const original{text_node(u8'a')};
        Ast copy = original;
        CHECK(copy == original);

        copy.push_back(text_node(u8'b'));
        CHECK(copy.size() == 2);
        CHECK(original.size() == 1);

        Ast assigned{text_node(u8'z')};
        assigned = original;
        CHECK(assigned == original);

        Ast moved = ::std::move(copy);
        CHECK(moved.size() == 2);

        assigned = ::std::move(moved);
        CHECK(assigned.size() == 2);
    }

    // Comparison, including ASTs of different sizes
    TEST_CASE("ast-comparison") {
        Ast left{text_node(u8'a'), text_node(u8'b')};
        Ast same{text_node(u8'a'), text_node(u8'b')};
        Ast shorter{text_node(u8'a')};
        Ast other{text_node(u8'c')};
        CHECK(left == same);
        CHECK_FALSE(left == shorter);
        CHECK_FALSE(left == other);
    }

    // Self-assignment and self-swap leave the AST intact
    TEST_CASE("self-assignment-and-self-swap") {
        Ast ast{text_node(u8'a'), text_node(u8'b')};
        Ast& alias = ast;
        alias = ast;
        CHECK(ast.size() == 2);

        ast.swap(ast);
        CHECK(ast.size() == 2);
        CHECK(ast.front().as_text().index(0) == u8'a');
    }

    // Swap, through the member and the free function
    TEST_CASE("member-and-free-swap") {
        Ast left{text_node(u8'a'), text_node(u8'b')};
        Ast right{text_node(u8'a'), text_node(u8'b')};
        Ast other{text_node(u8'c')};

        left.swap(other);
        CHECK(left.size() == 1);
        CHECK(other.size() == 2);
    }
}
