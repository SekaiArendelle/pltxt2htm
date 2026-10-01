// This test stands in for a downstream user: it is deliberately built without
// PLTXT2HTM_INTERNAL_USE (see the exclusion in tests/CMakeLists.txt), so that the assignment
// operations pltxt2htm exposes to external users stay covered.
#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

consteval auto code_fence_lifetime_is_valid() -> bool {
    using nd = ::pltxt2htm::Contracts;

    auto highlighted =
        ::pltxt2htm::CodeFence<nd::ignore>(::pltxt2htm::HighlightedCodeAst<nd::ignore>{});
    auto rendered = ::pltxt2htm::CodeFence<nd::ignore>(::pltxt2htm::RenderedCodeAst<nd::ignore>{});
    auto rendered_copy = rendered;

    highlighted = ::std::move(rendered_copy);
    if (highlighted.get_kind() != ::pltxt2htm::CodeFenceKind::rendered) {
        return false;
    }

    rendered = ::pltxt2htm::CodeFence<nd::ignore>(::pltxt2htm::HighlightedCodeAst<nd::ignore>{});
    return rendered.get_kind() == ::pltxt2htm::CodeFenceKind::highlighted;
}

static_assert(code_fence_lifetime_is_valid());

int main() {
    using nd = ::pltxt2htm::Contracts;

    // Assign between nodes of the same kind
    {
        auto const original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto assigned =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Assign across kinds: the destination's previous value must be destroyed and replaced
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();

        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        auto assigned =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast));

        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // The assignment is a deep copy: mutating the source afterwards leaves the destination alone
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        auto original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast));
        auto assigned = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();

        assigned = original;

        ::pltxt2htm::Ast<nd::quick_enforce> new_ast{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B')};
        original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
            ::std::move(new_ast));

        ::pltxt2htm::Ast<nd::quick_enforce> expected_ast{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A')};
        auto const expected =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(expected_ast));
        pltxt2htm_test_assert_true(assigned == expected);
        pltxt2htm_test_assert_false(assigned == original);
    }

    // Self assignment is a no-op and must not destroy the value
    {
        auto const original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'S');
        auto assigned = original;
        assigned = assigned;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Chained assignment
    {
        auto const third =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'C');
        auto second =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        auto first =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');

        first = second = third;
        pltxt2htm_test_assert_true(first == third);
        pltxt2htm_test_assert_true(second == third);
    }

    // Repeated assignment while the active kind changes
    {
        auto source =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X');
        auto target = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();

        target = source;
        pltxt2htm_test_assert_true(target == source);

        ::pltxt2htm::Ast<nd::quick_enforce> ast{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'Y')};
        source = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
            ::std::move(ast));

        target = source;
        pltxt2htm_test_assert_true(target == source);
    }

    // Assigning a whole Ast (vector of nodes)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> const original{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'),
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B')};
        ::pltxt2htm::Ast<nd::quick_enforce> assigned{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'C')};
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Assigning the members that carry extra data
    {
        auto const original = ::pltxt2htm::Group<nd::quick_enforce>(::pltxt2htm::Ast<nd::quick_enforce>{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a')});
        auto assigned = ::pltxt2htm::Group<nd::quick_enforce>(::pltxt2htm::Ast<nd::quick_enforce>{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'b')});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    {
        ::pltxt2htm::HighlightedCodeAst<nd::quick_enforce> original_ast{};
        ::pltxt2htm::container::U8String original_text{u8"int"};
        original_ast.append(original_text, ::pltxt2htm::CodeHighlightKind::keyword);
        auto const original = ::pltxt2htm::CodeFence<nd::quick_enforce>(::std::move(original_ast));
        auto assigned = ::pltxt2htm::CodeFence<nd::quick_enforce>(::pltxt2htm::HighlightedCodeAst<nd::quick_enforce>{});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
        ::pltxt2htm::container::U8String appended_text{u8" main"};
        assigned.get_highlighted_ast().append(appended_text, ::pltxt2htm::CodeHighlightKind::plain);
        pltxt2htm_test_assert_true(assigned != original);
    }

    // CodeFence copy assignment reconstructs the active AST when its kind changes.
    {
        ::pltxt2htm::RenderedCodeAst<nd::quick_enforce> rendered_ast{};
        ::pltxt2htm::container::U8String rendered_text{u8"rendered"};
        rendered_ast.append_text(rendered_text);
        auto rendered = ::pltxt2htm::CodeFence<nd::quick_enforce>(::std::move(rendered_ast));

        auto assigned = ::pltxt2htm::CodeFence<nd::quick_enforce>(::pltxt2htm::HighlightedCodeAst<nd::quick_enforce>{});
        assigned = rendered;
        pltxt2htm_test_assert_true(assigned.get_kind() == ::pltxt2htm::CodeFenceKind::rendered);
        pltxt2htm_test_assert_true(assigned == rendered);

        auto highlighted = ::pltxt2htm::CodeFence<nd::quick_enforce>(::pltxt2htm::HighlightedCodeAst<nd::quick_enforce>{});
        assigned = highlighted;
        pltxt2htm_test_assert_true(assigned.get_kind() == ::pltxt2htm::CodeFenceKind::highlighted);
        pltxt2htm_test_assert_true(assigned == highlighted);
    }

    // CodeFence move assignment also reconstructs the active AST when its kind changes.
    {
        ::pltxt2htm::RenderedCodeAst<nd::quick_enforce> rendered_ast{};
        ::pltxt2htm::container::U8String rendered_text{u8"rendered"};
        rendered_ast.append_text(rendered_text);
        auto rendered = ::pltxt2htm::CodeFence<nd::quick_enforce>(::std::move(rendered_ast));

        auto assigned =
            ::pltxt2htm::CodeFence<nd::quick_enforce>(::pltxt2htm::HighlightedCodeAst<nd::quick_enforce>{});
        assigned = ::std::move(rendered);
        pltxt2htm_test_assert_true(assigned.get_kind() == ::pltxt2htm::CodeFenceKind::rendered);
        pltxt2htm_test_assert_true(assigned.get_rendered_ast().get_nodes().size() == 1);

        auto highlighted =
            ::pltxt2htm::CodeFence<nd::quick_enforce>(::pltxt2htm::HighlightedCodeAst<nd::quick_enforce>{});
        assigned = ::std::move(highlighted);
        pltxt2htm_test_assert_true(assigned.get_kind() == ::pltxt2htm::CodeFenceKind::highlighted);
        pltxt2htm_test_assert_true(assigned.get_highlighted_ast().get_nodes().is_empty());
    }

    {
        auto const original = ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"https://example.com"});
        auto assigned = ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"https://example.org"});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    {
        auto const original = ::pltxt2htm::HtmlH1<nd::quick_enforce>(::pltxt2htm::Ast<nd::quick_enforce>{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a')});
        auto assigned = ::pltxt2htm::HtmlH1<nd::quick_enforce>(::pltxt2htm::Ast<nd::quick_enforce>{
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'b')});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    return 0;
}
