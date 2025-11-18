#include "pltxt2htm/details/parser/md_list.hh"

inline namespace pltxt2htm_test {

template<::std::size_t N>
constexpr auto text_item(char8_t const (&text)[N]) noexcept {
    return ::pltxt2htm::details::MdListTextNode{::fast_io::u8string{text}};
}

template<::pltxt2htm::details::is_md_list_node... Nodes>
constexpr auto md_list(Nodes&&... nodes) noexcept {
    ::pltxt2htm::details::MdListAst result{};

    ((result.emplace_back(::pltxt2htm::HeapGuard<::std::remove_cvref_t<Nodes>>(::std::forward<Nodes>(nodes)))), ...);

    return result;
}

template<::pltxt2htm::details::is_md_list_node... Nodes>
constexpr auto sub_md_list_item(Nodes&&... nodes) noexcept {
    return ::pltxt2htm::details::MdListSublistNode(::pltxt2htm_test::md_list(::std::forward<Nodes>(nodes)...));
}

} // namespace pltxt2htm_test

int main() {
    {
        ::pltxt2htm::details::MdListTextNode node1{::fast_io::u8string{u8"test"}};
        ::pltxt2htm::details::MdListTextNode node2{::fast_io::u8string{u8"test"}};
        ::exception::assert_true<false>(node1 == node2);
    }
    {
        ::pltxt2htm::details::MdListSublistNode node1{::pltxt2htm::details::MdListAst{}};
        ::pltxt2htm::details::MdListSublistNode node2{::pltxt2htm::details::MdListAst{}};
        ::exception::assert_true<false>(node1 == node2);
    }
    {
        ::pltxt2htm::details::MdListAst ast1{};
        ::pltxt2htm::details::MdListAst ast2{};
        ::exception::assert_true<false>(ast1 == ast2);
    }
    {
        auto ast1 = md_list(text_item(u8"test"));
        auto ast2 = md_list(text_item(u8"text"));
        ::exception::assert_false<false>(ast1 == ast2);
    }
    {
        auto ast1 = md_list(text_item(u8"test"), text_item(u8"test"),
                            sub_md_list_item(text_item(u8"test"), text_item(u8"test"),
                                             sub_md_list_item(text_item(u8"test"), text_item(u8"test"))));
        auto ast2 =
            md_list(text_item(u8"test"), text_item(u8"test"),
                    sub_md_list_item(text_item(u8"test"), text_item(u8"test"), sub_md_list_item(text_item(u8"test"))));
        auto ast3 = md_list(text_item(u8"test"), text_item(u8"test"),
                            sub_md_list_item(text_item(u8"test"), text_item(u8"test"),
                                             sub_md_list_item(text_item(u8"test"), text_item(u8"test"))));
        ::exception::assert_false<false>(ast1 == ast2);
        ::exception::assert_true<false>(ast1 == ast3);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8"- text").value();
        auto answer = md_list(text_item(u8"text"));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8"- text\n - text").value();
        auto answer = md_list(text_item(u8"text"), text_item(u8"text"));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8"   - text\n - text").value();
        auto answer = md_list(text_item(u8"text"), text_item(u8"text"));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8" - text\n   - text").value();
        auto answer = md_list(text_item(u8"text"), sub_md_list_item(text_item(u8"text")));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8" - text\n  - text\n   - text\n    - text")
                       .value();
        auto answer = md_list(text_item(u8"text"), text_item(u8"text"), text_item(u8"text"), text_item(u8"text"));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8" - text\n   - text\n - test").value();
        auto answer = md_list(text_item(u8"text"), sub_md_list_item(text_item(u8"text")), text_item(u8"test"));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast =
            ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8" - text\n   - text\n     - text\n   - test\n")
                .value();
        auto answer =
            md_list(text_item(u8"text"),
                    sub_md_list_item(text_item(u8"text"), sub_md_list_item(text_item(u8"text")), text_item(u8"test")));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast =
            ::pltxt2htm::details::optionally_to_md_list_ast<false>(u8" + text\n   + text\n     + text\n   + test\n")
                .value();
        auto answer =
            md_list(text_item(u8"text"),
                    sub_md_list_item(text_item(u8"text"), sub_md_list_item(text_item(u8"text")), text_item(u8"test")));
        ::exception::assert_true<false>(ast == answer);
    }
    {
        auto ast = ::pltxt2htm::details::optionally_to_md_list_ast<false>(
                       u8" - text\n   - text\n     - text\n   - test\n - test")
                       .value();
        auto answer =
            md_list(text_item(u8"text"),
                    sub_md_list_item(text_item(u8"text"), sub_md_list_item(text_item(u8"text")), text_item(u8"test")),
                    text_item(u8"test"));
        ::exception::assert_true<false>(ast == answer);
    }

    return 0;
}
