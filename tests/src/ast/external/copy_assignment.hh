#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("copy_assignment") {

    // Assign between nodes of the same kind
    TEST_CASE("assign-same-node-kind") {
        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A');
        auto assigned = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B');
        assigned = original;
        CHECK(assigned == original);
    }

    // Assign across kinds: the destination's previous value must be destroyed and replaced
    TEST_CASE("assign-different-node-kinds") {
        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>();

        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'H'));
        auto assigned = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(ast));

        assigned = original;
        CHECK(assigned == original);
    }

    // The assignment is a deep copy: mutating the source afterwards leaves the destination alone
    TEST_CASE("assignment-is-a-deep-copy") {
        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'));
        auto original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(ast));
        auto assigned = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>();

        assigned = original;

        ::pltxt2htm::Ast new_ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B')};
        original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(new_ast));

        ::pltxt2htm::Ast expected_ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A')};
        auto const expected = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(expected_ast));
        CHECK(assigned == expected);
        CHECK_FALSE(assigned == original);
    }

    // Self assignment is a no-op and must not destroy the value
    TEST_CASE("self-assignment") {
        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'S');
        auto assigned = original;
        // Route through an alias so this exercises the real self-assignment guard in PlTxtNode's copy
        // assignment without tripping clang's -Wself-assign-overloaded.
        auto const& self = assigned;
        assigned = self;
        CHECK(assigned == original);
    }

    // Chained assignment
    TEST_CASE("chained-assignment") {
        auto const third = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'C');
        auto second = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B');
        auto first = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A');

        first = second = third;
        CHECK(first == third);
        CHECK(second == third);
    }

    // Repeated assignment while the active kind changes
    TEST_CASE("repeated-assignment-across-kinds") {
        auto source = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'X');
        auto target = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>();

        target = source;
        CHECK(target == source);

        ::pltxt2htm::Ast ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'Y')};
        source = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(ast));

        target = source;
        CHECK(target == source);
    }

    // Assigning a whole Ast (vector of nodes)
    TEST_CASE("assign-whole-ast") {
        ::pltxt2htm::Ast const original{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'),
                                        ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B')};
        ::pltxt2htm::Ast assigned{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'C')};
        assigned = original;
        CHECK(assigned == original);
    }

    TEST_CASE("assign-group") {
        auto const original =
            ::pltxt2htm::Group(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'a')});
        auto assigned =
            ::pltxt2htm::Group(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'b')});
        assigned = original;
        CHECK(assigned == original);
    }

    // Assign a CodeFence including its optional language.
    TEST_CASE("assign-code-fence") {
        auto const original =
            ::pltxt2htm::CodeFence(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'c')},
                                   ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                       ::pltxt2htm::container::U8String{u8"cpp"}));
        auto assigned =
            ::pltxt2htm::CodeFence(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'd')},
                                   ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                       ::pltxt2htm::container::U8String{u8"rust"}));
        assigned = original;
        CHECK(assigned == original);
    }

    // Assign a URL value.
    TEST_CASE("assign-url") {
        auto const original = ::pltxt2htm::Url::try_make(u8"https://example.com").value();
        auto assigned = ::pltxt2htm::Url::try_make(u8"https://example.org").value();
        assigned = original;
        CHECK(assigned == original);
    }

    // Assign an HTML heading with its child AST.
    TEST_CASE("assign-html-heading") {
        auto const original =
            ::pltxt2htm::HtmlH1(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'a')});
        auto assigned =
            ::pltxt2htm::HtmlH1(::pltxt2htm::Ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'b')});
        assigned = original;
        CHECK(assigned == original);
    }
}
