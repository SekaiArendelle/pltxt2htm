// This test stands in for a downstream user: it is deliberately built without
// PLTXT2HTM_INTERNAL_USE (see the exclusion in tests/CMakeLists.txt), so that the interface
// class Ast exposes to external users stays covered.
#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

namespace {

using Contracts = ::pltxt2htm::Contracts;
using Node = ::pltxt2htm::PlTxtNode<Contracts::quick_enforce>;
using Ast = ::pltxt2htm::Ast<Contracts::quick_enforce>;
using BackingVector = ::pltxt2htm::container::Vector<Node>;

[[nodiscard]]
constexpr auto text_node(char8_t const code_unit) noexcept -> Node {
    return Node::template emplace<::pltxt2htm::Text<Contracts::quick_enforce>>(code_unit);
}

// Ast wraps the backing vector without adding storage, and mirrors its types and value semantics.
static_assert(sizeof(Ast) == sizeof(BackingVector));
static_assert(::std::is_nothrow_move_constructible_v<Ast>);
static_assert(::std::is_nothrow_move_assignable_v<Ast>);
static_assert(::std::is_nothrow_copy_constructible_v<Ast>);
static_assert(::std::is_nothrow_copy_assignable_v<Ast>);
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

} // namespace

int main() {
    // An empty AST
    {
        Ast ast{};
        pltxt2htm_test_assert_true(ast.is_empty());
        pltxt2htm_test_assert_true(ast.empty());
        pltxt2htm_test_assert_true(ast.size() == 0);
        pltxt2htm_test_assert_true(ast.capacity() == 0);
        pltxt2htm_test_assert_true(ast.data() == nullptr);
        pltxt2htm_test_assert_true(ast.begin() == ast.end());
        pltxt2htm_test_assert_true(ast.cbegin() == ast.cend());
        pltxt2htm_test_assert_true(ast.rbegin() == ast.rend());
        pltxt2htm_test_assert_true(Ast::max_size() == BackingVector::max_size());
    }

    // Construction from an initializer list, and the read-only accessors of a const AST
    {
        Ast const ast{text_node(u8'a'), text_node(u8'b')};
        pltxt2htm_test_assert_true(ast.size() == 2);
        pltxt2htm_test_assert_true(ast.empty() == false);
        pltxt2htm_test_assert_true(ast.is_empty() == false);
        pltxt2htm_test_assert_true(ast.capacity() >= 2);
        pltxt2htm_test_assert_true(ast.data() != nullptr);
        pltxt2htm_test_assert_true(ast.data() == ast.cbegin());
        pltxt2htm_test_assert_true(ast.begin() == ast.cbegin());
        pltxt2htm_test_assert_true(ast.end() == ast.cend());
        pltxt2htm_test_assert_true(ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::text);
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'a');
        pltxt2htm_test_assert_true(ast.index(1).as_text().index(0) == u8'b');
    }

    // Appending, removing and reusing the storage
    {
        Ast ast{};
        Node const node{text_node(u8'x')};
        ast.push_back(node);
        ast.push_back(text_node(u8'y'));
        auto const& appended = ast.emplace_back(text_node(u8'z'));
        pltxt2htm_test_assert_true(ast.size() == 3);
        pltxt2htm_test_assert_true(::std::addressof(appended) == ast.data() + 2);

        ast.pop_back();
        pltxt2htm_test_assert_true(ast.size() == 2);
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'x');

        ast.reserve(16);
        auto const reserved_capacity = ast.capacity();
        pltxt2htm_test_assert_true(reserved_capacity >= 16);
        pltxt2htm_test_assert_true(ast.size() == 2);

        ast.clear();
        pltxt2htm_test_assert_true(ast.is_empty());
        pltxt2htm_test_assert_true(ast.capacity() == reserved_capacity);

        Ast single{text_node(u8'q')};
        single.pop_back();
        pltxt2htm_test_assert_true(single.is_empty());
    }

    // Text appending coalesces adjacent code units and starts a node after non-text content
    {
        Ast ast{};
        ast.append_text(u8'a');
        ast.append_text(u8"bc");
        ast.append_text(::pltxt2htm::container::U8StringView{});
        pltxt2htm_test_assert_true(ast.size() == 1);
        pltxt2htm_test_assert_true(ast.front().as_text().size() == 3);
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'a');
        pltxt2htm_test_assert_true(ast.front().as_text().index(2) == u8'c');

        ast.push_back(Node::template emplace<::pltxt2htm::LineBreak>());
        ast.append_text(u8'd');
        pltxt2htm_test_assert_true(ast.size() == 3);
        pltxt2htm_test_assert_true(ast.index(2).as_text().index(0) == u8'd');
    }

    // Semantic code points become dedicated nodes or coalesced UTF-8 text.
    {
        Ast ast{};
        ast.append_code_point(U'A');
        ast.append_code_point(char32_t{0x20AC});
        ast.append_code_point(U' ');
        ast.append_code_point(char32_t{0xA0});
        ast.append_code_point(U'\n');
        ast.append_code_point(U'&');
        ast.append_code_point(char32_t{0x7F});
        ast.append_code_point(char32_t{0xD800});

        pltxt2htm_test_assert_true(ast.size() == 7);
        pltxt2htm_test_assert_true(ast.index(0).as_text().size() == 4);
        pltxt2htm_test_assert_true(ast.index(0).as_text().index(0) == u8'A');
        pltxt2htm_test_assert_true(ast.index(0).as_text().index(1) == char8_t{0xE2});
        pltxt2htm_test_assert_true(ast.index(1).get_node_kind() == ::pltxt2htm::NodeKind::space);
        pltxt2htm_test_assert_true(ast.index(2).get_node_kind() == ::pltxt2htm::NodeKind::space);
        pltxt2htm_test_assert_true(ast.index(3).get_node_kind() == ::pltxt2htm::NodeKind::line_break);
        pltxt2htm_test_assert_true(ast.index(4).get_node_kind() == ::pltxt2htm::NodeKind::ampersand);
        pltxt2htm_test_assert_true(ast.index(5).get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
        pltxt2htm_test_assert_true(ast.index(6).get_node_kind() == ::pltxt2htm::NodeKind::invalid_utf8);
    }

    // Iteration, reverse iteration and range-based for
    {
        Ast ast{text_node(u8'a'), text_node(u8'b'), text_node(u8'c')};

        ::std::size_t visited{};
        for (auto const& node : ast) {
            pltxt2htm_test_assert_true(node.get_node_kind() == ::pltxt2htm::NodeKind::text);
            ++visited;
        }
        pltxt2htm_test_assert_true(visited == 3);

        pltxt2htm_test_assert_true(ast.begin() != ast.end());
        pltxt2htm_test_assert_true(ast.end() - ast.begin() == 3);
        pltxt2htm_test_assert_true(*ast.rbegin() == ast.index(2));
        pltxt2htm_test_assert_true(ast.rend() == ast.rbegin() + 3);
    }

    // Erasing a single node, an empty range, and a non-empty range
    {
        Ast ast{text_node(u8'a'), text_node(u8'b'), text_node(u8'c')};

        auto const next = ast.erase(ast.begin());
        pltxt2htm_test_assert_true(ast.size() == 2);
        pltxt2htm_test_assert_true(next == ast.begin());
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'b');

        auto const unchanged = ast.erase(ast.end(), ast.end());
        pltxt2htm_test_assert_true(unchanged == ast.end());
        pltxt2htm_test_assert_true(ast.size() == 2);

        auto const after = ast.erase(ast.begin(), ast.begin() + 1);
        pltxt2htm_test_assert_true(ast.size() == 1);
        pltxt2htm_test_assert_true(after == ast.begin());
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'c');
    }

    // Appending a whole range, from an rvalue, an lvalue and an empty source
    {
        Ast target{};
        Ast moved_source{text_node(u8'a')};
        target.append_range(::std::move(moved_source));
        pltxt2htm_test_assert_true(target.size() == 1);

        Ast copied_source{text_node(u8'b')};
        target.append_range(copied_source);
        pltxt2htm_test_assert_true(target.size() == 2);
        pltxt2htm_test_assert_true(copied_source.size() == 1);

        Ast const empty_source{};
        target.append_range(empty_source);
        pltxt2htm_test_assert_true(target.size() == 2);
    }

    // Copy and move preserve the node sequence, and the clone stays independent
    {
        Ast const original{text_node(u8'a')};
        Ast copy = original;
        pltxt2htm_test_assert_true(copy == original);

        copy.push_back(text_node(u8'b'));
        pltxt2htm_test_assert_true(copy.size() == 2);
        pltxt2htm_test_assert_true(original.size() == 1);

        Ast assigned{text_node(u8'z')};
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);

        Ast moved = ::std::move(copy);
        pltxt2htm_test_assert_true(moved.size() == 2);

        assigned = ::std::move(moved);
        pltxt2htm_test_assert_true(assigned.size() == 2);
    }

    // Comparison, including ASTs of different sizes
    {
        Ast left{text_node(u8'a'), text_node(u8'b')};
        Ast same{text_node(u8'a'), text_node(u8'b')};
        Ast shorter{text_node(u8'a')};
        Ast other{text_node(u8'c')};
        pltxt2htm_test_assert_true(left == same);
        pltxt2htm_test_assert_false(left == shorter);
        pltxt2htm_test_assert_false(left == other);
    }

    // Self-assignment and self-swap leave the AST intact
    {
        Ast ast{text_node(u8'a'), text_node(u8'b')};
        Ast& alias = ast;
        alias = ast;
        pltxt2htm_test_assert_true(ast.size() == 2);

        ast.swap(ast);
        pltxt2htm_test_assert_true(ast.size() == 2);
        pltxt2htm_test_assert_true(ast.front().as_text().index(0) == u8'a');
    }

    // Swap, through the member and the free function
    {
        Ast left{text_node(u8'a'), text_node(u8'b')};
        Ast right{text_node(u8'a'), text_node(u8'b')};
        Ast other{text_node(u8'c')};

        left.swap(other);
        pltxt2htm_test_assert_true(left.size() == 1);
        pltxt2htm_test_assert_true(other.size() == 2);
    }

    return 0;
}
