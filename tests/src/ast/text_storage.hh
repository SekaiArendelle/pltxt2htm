#pragma once

#include "doctest_config.hh"

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/inline_parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/parser.hh>
#include <utility>

TEST_SUITE("text_storage") {
    using Text = ::pltxt2htm::Text;

    struct CharacterConvertibleIterator {
        using value_type = char8_t;
        using difference_type = ::std::ptrdiff_t;

        char8_t const* pointer{};

        [[nodiscard]]
        constexpr auto operator*() const noexcept -> char8_t {
            return *this->pointer;
        }

        constexpr auto operator++() -> CharacterConvertibleIterator& {
            ++this->pointer;
            return *this;
        }

        constexpr void operator++(int) {
            ++*this;
        }

        [[nodiscard]]
        constexpr auto operator==(CharacterConvertibleIterator const&) const noexcept -> bool = default;

        [[nodiscard]]
        constexpr operator char8_t() const noexcept {
            return u8'x';
        }
    };

    static_assert(::std::input_iterator<CharacterConvertibleIterator>);
    static_assert(::std::convertible_to<CharacterConvertibleIterator, char8_t>);
    static_assert(!::std::is_nothrow_constructible_v<Text, CharacterConvertibleIterator, CharacterConvertibleIterator>);

    static_assert(sizeof(void*) != 8 || Text::capacity() == 71);
    static_assert(sizeof(Text) == sizeof(::pltxt2htm::HtmlSpan));
    static_assert(sizeof(void*) != 8 || sizeof(::pltxt2htm::PlTxtNode) == 80);
    static_assert(::std::is_trivially_copyable_v<Text>);
    static_assert(!::std::is_assignable_v<Text, Text const&>);
    static_assert(!::std::is_assignable_v<Text, Text&&>);
    static_assert(::std::is_nothrow_constructible_v<Text, char8_t const*, char8_t const*>);
    static_assert(
        noexcept(::std::declval<Text&>().append(::std::declval<char8_t const*>(), ::std::declval<char8_t const*>())));

    [[nodiscard]]
    constexpr auto text_special_members_preserve_contents() noexcept -> bool {
        constexpr char8_t code_units[]{u8'a', u8'\0', u8'b'};
        Text const source{code_units, code_units + 3};
        auto copied = source;
        if (copied != source) {
            return false;
        }
        auto moved = ::std::move(copied);
        if (moved != source) {
            return false;
        }
        Text copy_assigned{u8'x'};
        copy_assigned = source;
        if (copy_assigned != source) {
            return false;
        }
        Text move_assigned{u8'y'};
        move_assigned = ::std::move(copy_assigned);
        return move_assigned == source;
    }

    static_assert(text_special_members_preserve_contents());

    [[nodiscard]]
    constexpr auto text_range_uses_iterators() -> bool {
        constexpr char8_t code_units[]{u8'a', u8'\0', u8'b'};
        Text const text{CharacterConvertibleIterator{code_units}, CharacterConvertibleIterator{code_units + 3}};
        return text.size() == 3 && text[0] == u8'a' && text[1] == u8'\0' && text[2] == u8'b';
    }

    static_assert(text_range_uses_iterators());

    TEST_CASE("range construction does not convert iterators to characters") {
        CHECK(text_range_uses_iterators());
    }

    TEST_CASE("copy and move preserve contents") {
        CHECK(text_special_members_preserve_contents());
    }

    TEST_CASE("single character construction and full storage") {
        Text text{u8'\0'};
        REQUIRE(text.size() == 1);
        CHECK(text[0] == u8'\0');
        for (::std::size_t index{1}; index < Text::capacity(); ++index) {
            REQUIRE(text.try_push_back(u8'a'));
        }
        REQUIRE(text.size() == Text::capacity());
        CHECK_FALSE(text.try_push_back(u8'b'));
        CHECK(text.size() == Text::capacity());
        CHECK(text[0] == u8'\0');
        CHECK(text[Text::capacity() - 1] == u8'a');
    }

    TEST_CASE("text is split at capacity") {
        ::pltxt2htm::container::U8String input{};
        for (::std::size_t index{}; index < Text::capacity() * 2 + 1; ++index) {
            input.push_back(u8'a');
        }

        auto const ast = ::pltxt2htm::parse_pltxt(::pltxt2htm::container::U8StringView{input});
        REQUIRE(ast.size() == 3);
        CHECK(ast[0].get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast[0].as_text().size() == Text::capacity());
        CHECK(ast[1].as_text().size() == Text::capacity());
        CHECK(ast[2].as_text().size() == 1);
    }

    TEST_CASE("spaces separate text nodes") {
        auto const ast = ::pltxt2htm::parse_pltxt(u8"abc def");
        REQUIRE(ast.size() == 3);
        CHECK(ast[0].get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast[0].as_text().size() == 3);
        CHECK(ast[1].get_node_kind() == ::pltxt2htm::NodeKind::space);
        CHECK(ast[2].get_node_kind() == ::pltxt2htm::NodeKind::text);
        CHECK(ast[2].as_text().size() == 3);
    }

    TEST_CASE("multibyte text size") {
        auto const ast = ::pltxt2htm::parse_pltxt(u8"a\u20ACb");
        REQUIRE(ast.size() == 1);
        CHECK(ast[0].as_text().size() == 5);
    }

    TEST_CASE("unknown escape text size") {
        auto const ast = ::pltxt2htm::parse_pltxt(u8"abc\\qdef");
        REQUIRE(ast.size() == 1);
        CHECK(ast[0].as_text().size() == 8);
    }

    TEST_CASE("adjacent text nodes merge") {
        ::pltxt2htm::Ast ast{};
        ast.push_back(::pltxt2htm::PlTxtNode::template emplace<Text>(u8'a'));
        ast.push_back(::pltxt2htm::PlTxtNode::template emplace<Text>(u8'b'));
        ast.push_back(::pltxt2htm::PlTxtNode::template emplace<Text>(u8'c'));
        ::pltxt2htm::optimize_ast(ast);
        REQUIRE(ast.size() == 1);
        auto const& text = ast[0].as_text();
        REQUIRE(text.size() == 3);
        CHECK(text[0] == u8'a');
        CHECK(text[1] == u8'b');
        CHECK(text[2] == u8'c');
    }

    TEST_CASE("text merge respects capacity") {
        auto const left_fill = ::std::views::repeat(u8'a', Text::capacity() - 5);
        auto left = Text{left_fill.begin(), left_fill.end()};
        auto const right_fill = ::std::views::repeat(u8'b', ::std::size_t{10});
        auto right = Text{right_fill.begin(), right_fill.end()};
        ::pltxt2htm::Ast ast{};
        ast.push_back(::pltxt2htm::PlTxtNode::template emplace<Text>(::std::move(left)));
        ast.push_back(::pltxt2htm::PlTxtNode::template emplace<Text>(::std::move(right)));
        ::pltxt2htm::optimize_ast(ast);
        REQUIRE(ast.size() == 2);
        auto const& first = ast[0].as_text();
        auto const& second = ast[1].as_text();
        REQUIRE(first.size() == Text::capacity());
        REQUIRE(second.size() == 5);
        for (::std::size_t index{}; index < 5; ++index) {
            CHECK(first[Text::capacity() - 5 + index] == u8'b');
            CHECK(second[index] == u8'b');
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
            auto const ast = ::pltxt2htm::parse_pltxt(pltext);
            REQUIRE(ast.size() == 1);
            CHECK(ast[0].get_node_kind() == ::pltxt2htm::NodeKind::html_strong);

            auto const inline_ast = ::pltxt2htm::inline_parse_pltxt(pltext);
            REQUIRE(inline_ast.size() == 1);
            CHECK(inline_ast[0].get_node_kind() == ::pltxt2htm::NodeKind::html_strong);
        }
    }
}
