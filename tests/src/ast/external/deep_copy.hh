#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("deep_copy") {

    // Copy a single stateless node
    TEST_CASE("copy-stateless-node") {
        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::LineBreak>();
        auto const copy = original;
        CHECK(original == copy);
    }

    // Copy a Text node
    TEST_CASE("copy-text-node") {
        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'X');
        auto const copy = original;
        CHECK(original == copy);
    }

    // Deep copy a simple Group node with children
    TEST_CASE("deep-copy-group-children") {
        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'H'));
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'i'));

        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(ast));

        auto const copy = original;
        CHECK(original == copy);
    }

    // Deep copy with nested sub-AST (HtmlBlockquote > HtmlH1 > Text)
    TEST_CASE("deep-copy-nested-sub-ast") {
        ::pltxt2htm::Ast inner{};
        inner.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'a'));

        ::pltxt2htm::Ast outer{};
        outer.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlH1>(::std::move(inner)));

        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::HtmlBlockquote>(::std::move(outer));

        auto const copy = original;
        CHECK(original == copy);
    }

    // Copy independence: modifying original must not affect copy
    TEST_CASE("copy-independence-for-nested-ast") {
        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'));

        auto original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(ast));

        auto copy = original;

        // Mutate original: replace its sub-AST
        ::pltxt2htm::Ast new_ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B')};
        original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(new_ast));

        // Copy must still hold old value
        ::pltxt2htm::Ast expected_ast{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A')};
        auto const expected = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Group>(::std::move(expected_ast));
        CHECK(copy == expected);
        CHECK_FALSE(copy == original);
    }

    // Copy of a node with extra data (TableTh with align)
    TEST_CASE("copy-table-header-with-alignment") {
        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'c'));

        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::TableTh>(
            ::std::move(ast), ::pltxt2htm::TableAlign::center);

        auto const copy = original;
        CHECK(original == copy);
    }

    // Copy of a node with optional language (CodeFence)
    TEST_CASE("copy-code-fence-with-optional-language") {
        ::pltxt2htm::Ast ast{};
        ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'x'));

        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::CodeFence>(
            ::std::move(ast), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                  ::pltxt2htm::container::U8String{u8"cpp"}));

        auto const copy = original;
        CHECK(original == copy);
    }

    // Copy of a node with Url (MdLink)
    TEST_CASE("copy-markdown-link-with-url") {
        ::pltxt2htm::Ast text_ast{};
        text_ast.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8't'));

        auto const original = ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::MdLink>(
            ::std::move(text_ast), ::pltxt2htm::Url::try_make(u8"https://example.com").value());

        auto const copy = original;
        CHECK(original == copy);
    }

    // Copy an Ast (vector of PlTxtNode)
    TEST_CASE("copy-ast") {
        ::pltxt2htm::Ast const original{::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'),
                                        ::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B')};

        auto const copy = original;
        CHECK(original == copy);
    }

    // Ast copy independence
    TEST_CASE("ast-copy-independence") {
        ::pltxt2htm::Ast original{};
        original.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'));

        auto copy = original;

        original.clear();
        original.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'B'));

        ::pltxt2htm::Ast expected{};
        expected.emplace_back(::pltxt2htm::PlTxtNode::template emplace<::pltxt2htm::Text>(u8'A'));

        CHECK(copy == expected);
        CHECK_FALSE(copy == original);
    }
}
