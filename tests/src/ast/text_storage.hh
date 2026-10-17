#pragma once

#include "doctest_config.hh"

#include <cstddef>
#include <ranges>
#include <type_traits>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/inline_parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/parser.hh>
#include <utility>

TEST_SUITE("text_storage") {
    constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
    using Text = ::pltxt2htm::Text<ndebug>;

    static_assert(sizeof(void*) != 8 || Text::capacity() == 71);
    static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::PlTxtNode<ndebug>) == 80);
    static_assert(::std::is_nothrow_constructible_v<Text, char8_t const*, char8_t const*>);
    static_assert(
        noexcept(::std::declval<Text&>().append(::std::declval<char8_t const*>(), ::std::declval<char8_t const*>())));

    TEST_CASE("text is split at capacity") {
        ::pltxt2htm::container::U8String input{};
        for (::std::size_t index{}; index < Text::capacity() * 2 + 1; ++index) {
            input.push_back<ndebug>(u8'a');
        }

        auto const ast = ::pltxt2htm::parse_pltxt<ndebug>(::pltxt2htm::container::U8StringView{input});
        REQUIRE(ast.size() == 3);
        CHECK(ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast.index(0).as_text().size() == Text::capacity());
        CHECK(ast.index(1).as_text().size() == Text::capacity());
        CHECK(ast.index(2).as_text().size() == 1);
    }

    TEST_CASE("spaces separate text nodes") {
        auto const ast = ::pltxt2htm::parse_pltxt<ndebug>(u8"abc def");
        REQUIRE(ast.size() == 3);
        CHECK(ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast.index(0).as_text().size() == 3);
        CHECK(ast.index(1).get_node_kind() == ::pltxt2htm::NodeKind::space);
        CHECK(ast.index(2).get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast.index(2).as_text().size() == 3);
    }

    TEST_CASE("multibyte text size") {
        auto const ast = ::pltxt2htm::parse_pltxt<ndebug>(u8"a\u20ACb");
        REQUIRE(ast.size() == 1);
        CHECK(ast.index(0).as_text().size() == 5);
    }

    TEST_CASE("unknown escape text size") {
        auto const ast = ::pltxt2htm::parse_pltxt<ndebug>(u8"abc\\qdef");
        REQUIRE(ast.size() == 1);
        CHECK(ast.index(0).as_text().size() == 8);
    }

    TEST_CASE("adjacent text nodes merge") {
        ::pltxt2htm::Ast<ndebug> ast{};
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<Text>(u8'a'));
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<Text>(u8'b'));
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<Text>(u8'c'));
        ::pltxt2htm::optimize_ast<ndebug>(ast);
        REQUIRE(ast.size() == 1);
        auto const& text = ast.index(0).as_text();
        REQUIRE(text.size() == 3);
        CHECK(text.index(0) == u8'a');
        CHECK(text.index(1) == u8'b');
        CHECK(text.index(2) == u8'c');
    }

    TEST_CASE("text merge respects capacity") {
        auto const left_fill = ::std::views::repeat(u8'a', Text::capacity() - 5);
        auto left = Text{left_fill.begin(), left_fill.end()};
        auto const right_fill = ::std::views::repeat(u8'b', ::std::size_t{10});
        auto right = Text{right_fill.begin(), right_fill.end()};
        ::pltxt2htm::Ast<ndebug> ast{};
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<Text>(::std::move(left)));
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>::template emplace<Text>(::std::move(right)));
        ::pltxt2htm::optimize_ast<ndebug>(ast);
        REQUIRE(ast.size() == 2);
        auto const& first = ast.index(0).as_text();
        auto const& second = ast.index(1).as_text();
        REQUIRE(first.size() == Text::capacity());
        REQUIRE(second.size() == 5);
        for (::std::size_t index{}; index < 5; ++index) {
            CHECK(first.index(Text::capacity() - 5 + index) == u8'b');
            CHECK(second.index(index) == u8'b');
        }
    }

    TEST_CASE("unclosed strong tag keeps node kind") {
        // A <strong> that is never closed is attached by the end-of-input path. It has to
        // keep its own kind, so that the node kind never depends on whether the closing tag
        // was written. Every render backend prints html_strong and unity_b alike, so only an
        // AST-level check can pin this; the syntax cases stay end-to-end.
        constexpr ::pltxt2htm::container::U8StringView unclosed_strong[]{
            u8"<strong>text",
            u8"<strong>text</q>",
        };
        for (auto const pltext : unclosed_strong) {
            auto const ast = ::pltxt2htm::parse_pltxt<ndebug>(pltext);
            REQUIRE(ast.size() == 1);
            CHECK(ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::html_strong);

            auto const inline_ast = ::pltxt2htm::inline_parse_pltxt<ndebug>(pltext);
            REQUIRE(inline_ast.size() == 1);
            CHECK(inline_ast.index(0).get_node_kind() == ::pltxt2htm::NodeKind::html_strong);
        }
    }
}
