#include "precompile.hh"

#include <pltxt2htm/details/parser/md_list.hh>

namespace pltxt2htm_test {

template<::std::size_t N>
constexpr auto text_item(char8_t const (&text)[N]) noexcept {
    return ::pltxt2htm::details::ListLiNode{::fast_io::u8string{text}};
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto md_list(Nodes&&... nodes) noexcept {
    ::pltxt2htm::details::ListAst<ndebug> result{};

    (result.emplace_back(::std::forward<Nodes>(nodes)), ...);

    return result;
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ul_item(Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListUlNode<ndebug>(::pltxt2htm_test::md_list<ndebug>(::std::forward<Nodes>(nodes)...));
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ol_item(Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListOlNode<ndebug>(::pltxt2htm_test::md_list<ndebug>(::std::forward<Nodes>(nodes)...));
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ol_item(::std::size_t start, Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListOlNode<ndebug>(::pltxt2htm_test::md_list<ndebug>(::std::forward<Nodes>(nodes)...),
                                                    start);
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ul_top(Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListBaseNode<ndebug>{
        ::pltxt2htm_test::ul_item<ndebug>(::std::forward<Nodes>(nodes)...)};
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ol_top(Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListBaseNode<ndebug>{
        ::pltxt2htm_test::ol_item<ndebug>(::std::forward<Nodes>(nodes)...)};
}

template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce,
         ::pltxt2htm::details::is_list_node_type... Nodes>
constexpr auto ol_top(::std::size_t start, Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::ListBaseNode<ndebug>{
        ::pltxt2htm_test::ol_item<ndebug>(start, ::std::forward<Nodes>(nodes)...)};
}

} // namespace pltxt2htm_test

int main() {
    {
        ::pltxt2htm::details::ListLiNode const node1{::fast_io::u8string{u8"test"}};
        ::pltxt2htm::details::ListLiNode const node2{::fast_io::u8string{u8"test"}};
        pltxt2htm_test_assert_true(node1 == node2);
    }
    {
        ::pltxt2htm::details::ListUlNode<::pltxt2htm::Contracts::quick_enforce> const node1{
            ::pltxt2htm::details::ListAst<::pltxt2htm::Contracts::quick_enforce>{}};
        ::pltxt2htm::details::ListUlNode<::pltxt2htm::Contracts::quick_enforce> const node2{
            ::pltxt2htm::details::ListAst<::pltxt2htm::Contracts::quick_enforce>{}};
        pltxt2htm_test_assert_true(node1 == node2);
    }
    {
        ::pltxt2htm::details::ListLiNode text_node{::fast_io::u8string{u8"test"}};
        ::pltxt2htm::details::ListUlNode<::pltxt2htm::Contracts::quick_enforce> ul_node{
            ::pltxt2htm::details::ListAst<::pltxt2htm::Contracts::quick_enforce>{}};
        pltxt2htm_test_assert_false(
            ::pltxt2htm::details::ListBaseNode<::pltxt2htm::Contracts::quick_enforce>(::std::move(text_node)) ==
            ::pltxt2htm::details::ListBaseNode<::pltxt2htm::Contracts::quick_enforce>(::std::move(ul_node)));
    }
    {
        ::pltxt2htm::details::ListAst<::pltxt2htm::Contracts::quick_enforce> const ast1{};
        ::pltxt2htm::details::ListAst<::pltxt2htm::Contracts::quick_enforce> const ast2{};
        pltxt2htm_test_assert_true(ast1 == ast2);
    }
    {
        auto ast1 = ::pltxt2htm_test::md_list(::pltxt2htm_test::text_item(u8"test"));
        auto ast2 = ::pltxt2htm_test::md_list(::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_false(ast1 == ast2);
    }
    {
        auto ast1 = ::pltxt2htm_test::md_list(
            ::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"),
                                                                ::pltxt2htm_test::text_item(u8"test"))));
        auto ast2 = ::pltxt2htm_test::md_list(
            ::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"))));
        auto ast3 = ::pltxt2htm_test::md_list(
            ::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"test"),
                                                                ::pltxt2htm_test::text_item(u8"test"))));
        pltxt2htm_test_assert_false(ast1 == ast2);
        pltxt2htm_test_assert_true(ast1 == ast3);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"- text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"- text\n - text")
                .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"   - text\n - text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"),
                                               ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n  - text\n   - text\n    - text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text"),
                                     ::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n - test")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"),
                                               ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                               ::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n     - text\n   - test\n")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                      ::pltxt2htm_test::text_item(u8"test")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" + text\n   + text\n     + text\n   + test\n")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                      ::pltxt2htm_test::text_item(u8"test")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n     - text\n   - test\n - test")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                      ::pltxt2htm_test::text_item(u8"test")),
            ::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" ");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" -");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" - ");
        pltxt2htm_test_assert_true(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"   ");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
            u8"This is not a md list.");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"1x test");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"1.");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"  1.");
        pltxt2htm_test_assert_false(ast.has_value());
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" * text\n   * text\n     * text\n   * test\n * test")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                      ::pltxt2htm_test::text_item(u8"test")),
            ::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto ast =
            ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" - test\n - ");
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8""));
        pltxt2htm_test_assert_true(ast.value<::pltxt2htm::Contracts::quick_enforce>().top_node == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" - ");
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8""));
        pltxt2htm_test_assert_true(ast.value<::pltxt2htm::Contracts::quick_enforce>().top_node == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"  - ");
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8""));
        pltxt2htm_test_assert_true(ast.value<::pltxt2htm::Contracts::quick_enforce>().top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"),
                                               ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n     - text\n * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"))));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n     - text\n   * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"),
                                      ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")),
                                      ::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n - text\n * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n   * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(
            ::pltxt2htm_test::text_item(u8"text"),
            ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text"), ::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8" - text\n   - text\n * text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"),
                                               ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8" - text\n + test")
                .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"text"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"- test\n - test\n   + text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ul_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                     ::pltxt2htm_test::ul_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1. test\n 2. test\n   1. text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                     ::pltxt2htm_test::ol_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"1) test")
                .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1) test\n 2) test")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1) test\n 2) test\n   1) text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                     ::pltxt2htm_test::ol_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        // . and ) are different marker types -> the list ends at the second line
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1. test\n 2) test")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer = ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1) test\n 2) test\n   1. text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                     ::pltxt2htm_test::ol_item(::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }

    {
        // start equals the first item number
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
            u8"3. test\n 4. test");
        pltxt2htm_test_assert_true(result.value<::pltxt2htm::Contracts::quick_enforce>().top_node.get_start() == 3);
    }
    {
        // nested ordered list keeps its own start
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
                          u8"1. test\n 2. test\n   3. text")
                          .value<::pltxt2htm::Contracts::quick_enforce>();
        auto answer =
            ::pltxt2htm_test::ol_top(::pltxt2htm_test::text_item(u8"test"), ::pltxt2htm_test::text_item(u8"test"),
                                     ::pltxt2htm_test::ol_item(3, ::pltxt2htm_test::text_item(u8"text")));
        pltxt2htm_test_assert_true(result.top_node == answer);
    }
    {
        // ) delimiter start
        auto result = ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(
            u8"5) test\n 6) test");
        pltxt2htm_test_assert_true(result.value<::pltxt2htm::Contracts::quick_enforce>().top_node.get_start() == 5);
    }
    {
        // unordered lists have no start of their own
        auto result =
            ::pltxt2htm::details::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"- text\n - text");
        pltxt2htm_test_assert_true(result.value<::pltxt2htm::Contracts::quick_enforce>().top_node.get_type() ==
                                   ::pltxt2htm::details::ListNodeType::list_ul);
    }

    return 0;
}
