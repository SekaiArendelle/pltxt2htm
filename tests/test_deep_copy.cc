#include <pltxt2htm/ast/ast.hh>

int main() {
    using nd = ::pltxt2htm::Contracts;

    // Copy a single stateless node
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});
        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Copy a U8Char node
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'X'});
        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Deep copy a simple Text node with children
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast.emplace_back(::pltxt2htm::U8Char{u8'i'});

        auto const original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast)));

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Deep copy with nested sub-AST (HtmlBlockquote > HtmlH1 > U8Char)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> inner{};
        inner.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> outer{};
        outer.emplace_back(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(inner)));

        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::HtmlBlockquote<nd::quick_enforce>(::std::move(outer)));

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Copy independence: modifying original must not affect copy
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::U8Char{u8'A'});

        auto original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast)));

        auto copy = original;

        // Mutate original: replace its sub-AST
        ::pltxt2htm::Ast<nd::quick_enforce> new_ast{};
        new_ast.emplace_back(::pltxt2htm::U8Char{u8'B'});
        original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(new_ast)));

        // Copy must still hold old value
        ::pltxt2htm::Ast<nd::quick_enforce> expected_ast{};
        expected_ast.emplace_back(::pltxt2htm::U8Char{u8'A'});
        auto const expected =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(expected_ast)));
        ::exception::assert_true<false>(copy == expected);
        ::exception::assert_false<false>(copy == original);
    }

    // Copy of a node with extra data (MdTh with align)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::U8Char{u8'c'});

        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTh<nd::quick_enforce>(::std::move(ast), ::pltxt2htm::MdTableAlign::center));

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Copy of a node with optional language (MdCodeFenceBacktick)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::U8Char{u8'x'});

        auto const original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
                ::std::move(ast), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Copy of a node with Url (MdLink)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> text_ast{};
        text_ast.emplace_back(::pltxt2htm::U8Char{u8't'});

        ::pltxt2htm::Ast<nd::quick_enforce> url_ast{};
        url_ast.emplace_back(::pltxt2htm::U8Char{u8'x'});

        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdLink<nd::quick_enforce>(
            ::std::move(text_ast), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast))));

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Copy an Ast (vector of PlTxtNode)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> original{};
        original.emplace_back(::pltxt2htm::U8Char{u8'A'});
        original.emplace_back(::pltxt2htm::U8Char{u8'B'});

        auto const copy = original;
        ::exception::assert_true<false>(original == copy);
    }

    // Ast copy independence
    {
        ::pltxt2htm::Ast<nd::quick_enforce> original{};
        original.emplace_back(::pltxt2htm::U8Char{u8'A'});

        auto copy = original;

        original.clear();
        original.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::pltxt2htm::Ast<nd::quick_enforce> expected{};
        expected.emplace_back(::pltxt2htm::U8Char{u8'A'});

        ::exception::assert_true<false>(copy == expected);
        ::exception::assert_false<false>(copy == original);
    }

    return 0;
}
