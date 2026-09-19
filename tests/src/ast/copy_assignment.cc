// This test stands in for a downstream user: it is deliberately built without
// PLTXT2HTM_INTERNAL_USE (see the exclusion in tests/CMakeLists.txt), so that the assignment
// operations pltxt2htm exposes to external users stay covered.
#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

int main() {
    using nd = ::pltxt2htm::Contracts;

    // Assign between nodes of the same kind
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'A'});
        auto assigned = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'B'});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Assign across kinds: the destination's previous value must be destroyed and replaced
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});

        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::Text<nd::quick_enforce>{u8'H'});
        auto assigned =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Group<nd::quick_enforce>(::std::move(ast)));

        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // The assignment is a deep copy: mutating the source afterwards leaves the destination alone
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast{};
        ast.emplace_back(::pltxt2htm::Text<nd::quick_enforce>{u8'A'});
        auto original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Group<nd::quick_enforce>(::std::move(ast)));
        auto assigned = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});

        assigned = original;

        ::pltxt2htm::Ast<nd::quick_enforce> new_ast{::pltxt2htm::Text<nd::quick_enforce>{u8'B'}};
        original =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Group<nd::quick_enforce>(::std::move(new_ast)));

        ::pltxt2htm::Ast<nd::quick_enforce> expected_ast{::pltxt2htm::Text<nd::quick_enforce>{u8'A'}};
        auto const expected =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Group<nd::quick_enforce>(::std::move(expected_ast)));
        pltxt2htm_test_assert_true(assigned == expected);
        pltxt2htm_test_assert_false(assigned == original);
    }

    // Self assignment is a no-op and must not destroy the value
    {
        auto const original = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'S'});
        auto assigned = original;
        assigned = assigned;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Chained assignment
    {
        auto const third = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'C'});
        auto second = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'B'});
        auto first = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'A'});

        first = second = third;
        pltxt2htm_test_assert_true(first == third);
        pltxt2htm_test_assert_true(second == third);
    }

    // Repeated assignment while the active kind changes
    {
        auto source = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>{u8'X'});
        auto target = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});

        target = source;
        pltxt2htm_test_assert_true(target == source);

        ::pltxt2htm::Ast<nd::quick_enforce> ast{::pltxt2htm::Text<nd::quick_enforce>{u8'Y'}};
        source = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Group<nd::quick_enforce>(::std::move(ast)));

        target = source;
        pltxt2htm_test_assert_true(target == source);
    }

    // Assigning a whole Ast (vector of nodes)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> const original{::pltxt2htm::Text<nd::quick_enforce>{u8'A'},
                                                           ::pltxt2htm::Text<nd::quick_enforce>{u8'B'}};
        ::pltxt2htm::Ast<nd::quick_enforce> assigned{::pltxt2htm::Text<nd::quick_enforce>{u8'C'}};
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    // Assigning the members that carry extra data
    {
        auto const original = ::pltxt2htm::Group<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'a'}});
        auto assigned = ::pltxt2htm::Group<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'b'}});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    {
        auto const original = ::pltxt2htm::CodeFence<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'c'}},
            ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                ::pltxt2htm::container::U8String{u8"cpp"}));
        auto assigned = ::pltxt2htm::CodeFence<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'd'}},
            ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                ::pltxt2htm::container::U8String{u8"rust"}));
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    {
        auto const original = ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"https://example.com"});
        auto assigned = ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"https://example.org"});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    {
        auto const original = ::pltxt2htm::HtmlH1<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'a'}});
        auto assigned = ::pltxt2htm::HtmlH1<nd::quick_enforce>(
            ::pltxt2htm::Ast<nd::quick_enforce>{::pltxt2htm::Text<nd::quick_enforce>{u8'b'}});
        assigned = original;
        pltxt2htm_test_assert_true(assigned == original);
    }

    return 0;
}
