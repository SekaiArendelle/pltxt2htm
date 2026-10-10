#include "precompile.hh"

#include <pltxt2htm/ast/ast.hh>

int main() {
    using nd = ::pltxt2htm::Contracts;

    // Empty/stateless nodes
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableCol>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableCol>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdHr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdHr>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlMacroProject>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlMacroProject>();
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::InvalidUtf8>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::InvalidUtf8>();
        pltxt2htm_test_assert_true(a == b);
    }

    // Different empty types should NOT be equal
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        pltxt2htm_test_assert_false(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        pltxt2htm_test_assert_false(a == b);
    }

    // Text with same/different values
    {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        pltxt2htm_test_assert_false(a == b);
    }

    // Group with empty sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_true(a == b);
    }

    // Group with non-empty sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_true(a == b);
    }

    // Group with different sub-AST content
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'o'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }

    // Group with different sub-AST size
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }

    // HtmlH1 with sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'Y'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }

    // HtmlBlockquote with nested sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> inner_a{};
        inner_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> outer_a{};
        outer_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(inner_a)));

        ::pltxt2htm::Ast<nd::quick_enforce> inner_b{};
        inner_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> outer_b{};
        outer_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(inner_b)));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBlockquote<nd::quick_enforce>>(
                ::std::move(outer_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBlockquote<nd::quick_enforce>>(
                ::std::move(outer_b));
        pltxt2htm_test_assert_true(a == b);
    }

    // CodeFence without language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        pltxt2htm_test_assert_true(a == b);
    }

    // CodeFence with same language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        pltxt2htm_test_assert_true(a == b);
    }

    // CodeFence with different languages
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"python"}));
        pltxt2htm_test_assert_false(a == b);
    }

    // CodeFence: one has language, the other does not
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        pltxt2htm_test_assert_false(a == b);
    }

    // CodeFence without language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        pltxt2htm_test_assert_true(a == b);
    }

    // CodeFence with same language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        pltxt2htm_test_assert_true(a == b);
    }

    // CodeFence with different languages
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"python"}));
        pltxt2htm_test_assert_false(a == b);
    }

    // CodeFence: one has language, the other does not
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(::pltxt2htm::container::nullopt));
        pltxt2htm_test_assert_false(a == b);
    }

    // MdLink with sub-AST and URL
    {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_a), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_b), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        pltxt2htm_test_assert_true(a == b);
    }

    // MdLink with different URL
    {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_a), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_b), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"y"}));
        pltxt2htm_test_assert_false(a == b);
    }

    // PlainText preserves valid UTF-8 for every code point.
    {
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        text.append_code_point(char32_t{0x10FFFF});
        text.append_code_point(char32_t{0xD800});
        text.append_code_point(char32_t{0x110000});
        auto const expected = ::pltxt2htm::container::U8String{u8"\U0010FFFF\uFFFD\uFFFD"};
        pltxt2htm_test_assert_true(text.as_string() == expected);
    }

    // PlainText preserves tab and line feed, canonicalizes spaces to the no-break form, and replaces other ASCII
    // controls.
    {
        auto const replacement = ::pltxt2htm::container::U8String{u8"\uFFFD"};
        for (char32_t control{}; control < char32_t{0x20}; ++control) {
            if (control == U'\t' || control == U'\n') {
                continue;
            }
            ::pltxt2htm::PlainText<nd::quick_enforce> text{};
            text.append_code_point(control);
            pltxt2htm_test_assert_true(text.as_string() == replacement);
        }
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        text.append_code_point(char32_t{0x7F});
        pltxt2htm_test_assert_true(text.as_string() == replacement);

        ::pltxt2htm::PlainText<nd::quick_enforce> boundaries{};
        boundaries.append_code_point(U'\t');
        boundaries.append_code_point(U'\n');
        boundaries.append_code_point(U' ');
        boundaries.append_code_point(U'~');
        boundaries.append_code_point(char32_t{0x80});
        auto const expected = ::pltxt2htm::container::U8String{u8"\t\n\u00A0~\u0080"};
        pltxt2htm_test_assert_true(boundaries.as_string() == expected);

        // U+0020 and U+00A0 are the same Physics-Lab space token, so both must be stored identically.
        ::pltxt2htm::PlainText<nd::quick_enforce> ascii_space{};
        ascii_space.append_code_point(U' ');
        ::pltxt2htm::PlainText<nd::quick_enforce> no_break_space{};
        no_break_space.append_code_point(char32_t{0xA0});
        pltxt2htm_test_assert_true(ascii_space.as_string() == no_break_space.as_string());
        pltxt2htm_test_assert_true(ascii_space.as_string() == ::pltxt2htm::container::U8String{u8"\u00A0"});
    }

    // UTF-8 appends consume one sequence and preserve invalid-prefix recovery.
    {
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"") == 0);
        pltxt2htm_test_assert_true(text.as_string() == ::pltxt2htm::container::U8String{});
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"Ax") == 1);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\u00A2x") == 2);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\u20ACx") == 3);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\U0001F600x") == 4);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\t") == 1);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\n") == 1);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\xE2\x82"
                                                                     "x") == 2);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\xC0\xAF") == 2);
        pltxt2htm_test_assert_true(text.append_first_utf8_code_point(u8"\xFF") == 1);
        auto const expected = ::pltxt2htm::container::U8String{u8"A\u00A2\u20AC\U0001F600\t\n\uFFFD\uFFFD\uFFFD"};
        pltxt2htm_test_assert_true(text.as_string() == expected);
    }

    // MdImage with plain-text alt and URL
    {
        ::pltxt2htm::PlainText<nd::quick_enforce> alt_a{};
        alt_a.append_code_point(U'a');

        ::pltxt2htm::PlainText<nd::quick_enforce> alt_b{};
        alt_b.append_code_point(U'a');

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdImage<nd::quick_enforce>>(
                ::std::move(alt_a), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdImage<nd::quick_enforce>>(
                ::std::move(alt_b), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        pltxt2htm_test_assert_true(a == b);
    }

    // ListLiCheckbox (sub-AST + bool checked)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_a), true);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_b), true);
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_a), true);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_b), false);
        pltxt2htm_test_assert_false(a == b);
    }

    // TableTh / TableTd (sub-AST + TableAlign)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'h'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'h'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTh<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::TableAlign::center);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTh<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::TableAlign::center);
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'h'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'h'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTh<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::TableAlign::left);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTh<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::TableAlign::center);
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'd'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'd'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTd<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::TableAlign::right);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableTd<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::TableAlign::right);
        pltxt2htm_test_assert_true(a == b);
    }

    // UnityColor (sub-AST + color string)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::U8String{u8"red"});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::U8String{u8"red"});
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::U8String{u8"red"});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::U8String{u8"blue"});
        pltxt2htm_test_assert_false(a == b);
    }

    // PlExperiment (sub-AST + ObjectId)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::PlObjectId<nd::quick_enforce>{::pltxt2htm::container::U8String{u8"642cf37a494746375aae306a"}});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::PlObjectId<nd::quick_enforce>{::pltxt2htm::container::U8String{u8"642cf37a494746375aae306a"}});
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::PlObjectId<nd::quick_enforce>{::pltxt2htm::container::U8String{u8"642cf37a494746375aae306a"}});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::PlObjectId<nd::quick_enforce>{::pltxt2htm::container::U8String{u8"642cf37a494746375aae306b"}});
        pltxt2htm_test_assert_false(a == b);
    }

    // UnitySize (sub-AST + ValueWithUnit)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        pltxt2htm_test_assert_true(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::ValueWithUnit<double>{.value = 16, .unit = ::pltxt2htm::Unit::px});
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::percent});
        pltxt2htm_test_assert_false(a == b);
    }

    // PlExternal (sub-AST + Url)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExternal<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExternal<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::Url(::pltxt2htm::container::U8String{u8"x"}));
        pltxt2htm_test_assert_true(a == b);
    }

    // Ast-level comparison (empty)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_b{};
        pltxt2htm_test_assert_true(ast_a == ast_b);
    }

    // Ast-level comparison (single matching elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        pltxt2htm_test_assert_true(ast_a == ast_b);
    }

    // Ast-level comparison (multiple matching elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        pltxt2htm_test_assert_true(ast_a == ast_b);
    }

    // Ast-level comparison (different elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'C'));

        pltxt2htm_test_assert_false(ast_a == ast_b);
    }

    // Ast-level comparison (different sizes)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        pltxt2htm_test_assert_false(ast_a == ast_b);
    }

    // C++20 operator!= auto-generation
    {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        pltxt2htm_test_assert_true(a != b);
    }
    {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        pltxt2htm_test_assert_false(a != b);
    }

    // MdEscape with same/different escaped characters
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        pltxt2htm_test_assert_true(a == b);
        pltxt2htm_test_assert_true(a.as_md_escape().get_character() == u8'\\');
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        pltxt2htm_test_assert_true(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        pltxt2htm_test_assert_false(a == b);
    }

    // UnityI (sub-AST only)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityI<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityI<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_true(a == b);
    }

    // UnityB (sub-AST only)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityB<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityB<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_true(a == b);
    }

    // Cross-type: same sub-AST, different wrapper type
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'X'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBlockquote<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8't'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityI<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityB<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }

    // Sub-AST count mismatch
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'i'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'!'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'H'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlH1<nd::quick_enforce>>(
                ::std::move(ast_b));
        pltxt2htm_test_assert_false(a == b);
    }

    return 0;
}
