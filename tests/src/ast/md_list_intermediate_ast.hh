#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/details/parser/md_list.hh>

TEST_SUITE("md_list_intermediate_ast") {
    template<::std::size_t N>
    constexpr auto text_item(char8_t const(&text)[N]) noexcept {
        return ::pltxt2htm::details::ListLiNode{::pltxt2htm::container::U8String{text}};
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto md_list(Nodes && ... nodes) noexcept {
        ::pltxt2htm::details::ListAst result{};

        (result.emplace_back(::std::forward<Nodes>(nodes)), ...);

        return result;
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ul_item(Nodes && ... nodes) noexcept {
        return ::pltxt2htm::details::ListUlNode(md_list(::std::forward<Nodes>(nodes)...));
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ol_item(Nodes && ... nodes) noexcept {
        return ::pltxt2htm::details::ListOlNode(md_list(::std::forward<Nodes>(nodes)...));
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ol_item(::std::size_t start, Nodes&&... nodes) noexcept {
        return ::pltxt2htm::details::ListOlNode(md_list(::std::forward<Nodes>(nodes)...), start);
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ul_top(Nodes && ... nodes) noexcept {
        return ::pltxt2htm::details::ListBaseNode{ul_item(::std::forward<Nodes>(nodes)...)};
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ol_top(Nodes && ... nodes) noexcept {
        return ::pltxt2htm::details::ListBaseNode{ol_item(::std::forward<Nodes>(nodes)...)};
    }

    template<::pltxt2htm::details::is_list_node_type... Nodes>
    constexpr auto ol_top(::std::size_t start, Nodes&&... nodes) noexcept {
        return ::pltxt2htm::details::ListBaseNode{ol_item(start, ::std::forward<Nodes>(nodes)...)};
    }

    TEST_CASE("equal-list-item-nodes") {
        ::pltxt2htm::details::ListLiNode const node1{::pltxt2htm::container::U8String{u8"test"}};
        ::pltxt2htm::details::ListLiNode const node2{::pltxt2htm::container::U8String{u8"test"}};
        CHECK(node1 == node2);
    }

    TEST_CASE("equal-empty-unordered-list-nodes") {
        ::pltxt2htm::details::ListUlNode const node1{::pltxt2htm::details::ListAst{}};
        ::pltxt2htm::details::ListUlNode const node2{::pltxt2htm::details::ListAst{}};
        CHECK(node1 == node2);
    }

    TEST_CASE("different-list-node-kinds") {
        ::pltxt2htm::details::ListLiNode text_node{::pltxt2htm::container::U8String{u8"test"}};
        ::pltxt2htm::details::ListUlNode ul_node{::pltxt2htm::details::ListAst{}};
        CHECK_FALSE(::pltxt2htm::details::ListBaseNode(::std::move(text_node)) ==
                    ::pltxt2htm::details::ListBaseNode(::std::move(ul_node)));
    }

    TEST_CASE("equal-empty-list-asts") {
        ::pltxt2htm::details::ListAst const ast1{};
        ::pltxt2htm::details::ListAst const ast2{};
        CHECK(ast1 == ast2);
    }

    TEST_CASE("different-list-item-text") {
        auto ast1 = md_list(text_item(u8"test"));
        auto ast2 = md_list(text_item(u8"text"));
        CHECK_FALSE(ast1 == ast2);
    }

    TEST_CASE("nested-list-ast-comparison") {
        auto ast1 = md_list(
            text_item(u8"test"), text_item(u8"test"),
            ul_item(text_item(u8"test"), text_item(u8"test"), ul_item(text_item(u8"test"), text_item(u8"test"))));
        auto ast2 = md_list(text_item(u8"test"), text_item(u8"test"),
                            ul_item(text_item(u8"test"), text_item(u8"test"), ul_item(text_item(u8"test"))));
        auto ast3 = md_list(
            text_item(u8"test"), text_item(u8"test"),
            ul_item(text_item(u8"test"), text_item(u8"test"), ul_item(text_item(u8"test"), text_item(u8"test"))));
        CHECK_FALSE(ast1 == ast2);
        CHECK(ast1 == ast3);
    }

    TEST_CASE("parse-single-unordered-item") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"- text").value();
        auto answer = ul_top(text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-sibling-unordered-items") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"- text\n - text").value();
        auto answer = ul_top(text_item(u8"text"), text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-indented-sibling-items") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"   - text\n - text").value();
        auto answer = ul_top(text_item(u8"text"), text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-nested-unordered-items") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-consistent-indent-as-siblings") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n  - text\n   - text\n    - text").value();
        auto answer = ul_top(text_item(u8"text"), text_item(u8"text"), text_item(u8"text"), text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-nested-item-followed-by-sibling") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n - test").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-deeply-nested-unordered-list") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n     - text\n   - test\n").value();
        auto answer = ul_top(text_item(u8"text"),
                             ul_item(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"test")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-plus-marker-unordered-list") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" + text\n   + text\n     + text\n   + test\n").value();
        auto answer = ul_top(text_item(u8"text"),
                             ul_item(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"test")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-nested-plus-marker-list") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n     - text\n   - test\n - test")
                .value();
        auto answer =
            ul_top(text_item(u8"text"), ul_item(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"test")),
                   text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-nested-list-followed-by-sibling") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8" ");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-whitespace-only-input") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8" -");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-marker-without-space") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - ");
        CHECK(ast.has_value());
    }

    TEST_CASE("accept-empty-unordered-item") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"   ");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-whitespace-only-list-input") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"This is not a md list.");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-non-list-text") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1x test");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-invalid-ordered-marker") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1.");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-empty-ordered-marker") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"  1.");
        CHECK_FALSE(ast.has_value());
    }

    TEST_CASE("reject-indented-ordered-marker") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" * text\n   * text\n     * text\n   * test\n * test")
                .value();
        auto answer =
            ul_top(text_item(u8"text"), ul_item(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"test")),
                   text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-asterisk-marker-list") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - test\n - ");
        auto answer = ul_top(text_item(u8"test"), text_item(u8""));
        CHECK(ast.value().top_node == answer);
    }

    TEST_CASE("parse-empty-item-after-text") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - ");
        auto answer = ul_top(text_item(u8""));
        CHECK(ast.value().top_node == answer);
    }

    TEST_CASE("parse-empty-unordered-item") {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast(u8"  - ");
        auto answer = ul_top(text_item(u8""));
        CHECK(ast.value().top_node == answer);
    }

    TEST_CASE("parse-indented-empty-unordered-item") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n * text").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-different-marker-at-nested-level") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n     - text\n * text").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text"), ul_item(text_item(u8"text"))));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-mixed-markers-in-deep-list") {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n     - text\n   * text").value();
        auto answer = ul_top(text_item(u8"text"),
                             ul_item(text_item(u8"text"), ul_item(text_item(u8"text")), text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-mixed-markers-in-nested-list") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n - text\n * text").value();
        auto answer = ul_top(text_item(u8"text"), text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-mixed-sibling-markers") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n   * text").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text"), text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-dash-and-asterisk-nested-list") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n   - text\n * text").value();
        auto answer = ul_top(text_item(u8"text"), ul_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-mixed-nested-markers") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8" - text\n + test").value();
        auto answer = ul_top(text_item(u8"text"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-plus-marker-list") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"- test\n - test\n   + text").value();
        auto answer = ul_top(text_item(u8"test"), text_item(u8"test"), ul_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-ordered-list-with-dot-markers") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1. test\n 2. test\n   1. text").value();
        auto answer = ol_top(text_item(u8"test"), text_item(u8"test"), ol_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-single-parenthesis-ordered-item") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1) test").value();
        auto answer = ol_top(text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-parenthesis-ordered-siblings") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1) test\n 2) test").value();
        auto answer = ol_top(text_item(u8"test"), text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-nested-parenthesis-ordered-list") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1) test\n 2) test\n   1) text").value();
        auto answer = ol_top(text_item(u8"test"), text_item(u8"test"), ol_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("stop-ordered-list-at-different-delimiter") {
        // . and ) are different marker types -> the list ends at the second line
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1. test\n 2) test").value();
        auto answer = ol_top(text_item(u8"test"));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("parse-mixed-ordered-delimiters-in-nested-list") {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1) test\n 2) test\n   1. text").value();
        auto answer = ol_top(text_item(u8"test"), text_item(u8"test"), ol_item(text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("preserve-ordered-list-start") {
        // start equals the first item number
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"3. test\n 4. test");
        CHECK(result.value().top_node.get_start() == 3);
    }

    TEST_CASE("preserve-nested-ordered-list-start") {
        // nested ordered list keeps its own start
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"1. test\n 2. test\n   3. text").value();
        auto answer = ol_top(text_item(u8"test"), text_item(u8"test"), ol_item(3, text_item(u8"text")));
        CHECK(result.top_node == answer);
    }

    TEST_CASE("preserve-parenthesis-list-start") {
        // ) delimiter start
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"5) test\n 6) test");
        CHECK(result.value().top_node.get_start() == 5);
    }

    TEST_CASE("unordered-list-has-no-start-number") {
        // unordered lists have no start of their own
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast(u8"- text\n - text");
        CHECK(result.value().top_node.get_type() == ::pltxt2htm::details::ListNodeType::list_ul);
    }
}
