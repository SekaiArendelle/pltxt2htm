#pragma once

#include "doctest_config.hh"

#include <pltxt2htm/ast/ast.hh>

TEST_SUITE("ast_operator_eq") {
    using nd = ::pltxt2htm::Contracts;

    auto make_object_id(::pltxt2htm::container::U8StringView const text)
        -> ::pltxt2htm::PlObjectId<::pltxt2htm::Contracts::quick_enforce> {
        return ::pltxt2htm::PlObjectId<::pltxt2htm::Contracts::quick_enforce>::try_make(text)
            .template value<::pltxt2htm::Contracts::quick_enforce>();
    }

    // Empty/stateless nodes
    TEST_CASE("equal-line-break-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        CHECK(a == b);
    }

    TEST_CASE("equal-space-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        CHECK(a == b);
    }

    TEST_CASE("equal-html-break-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        CHECK(a == b);
    }

    TEST_CASE("equal-html-rule-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        CHECK(a == b);
    }

    TEST_CASE("equal-table-column-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableCol>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::TableCol>();
        CHECK(a == b);
    }

    TEST_CASE("equal-markdown-rule-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdHr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdHr>();
        CHECK(a == b);
    }

    TEST_CASE("equal-project-macro-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlMacroProject>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlMacroProject>();
        CHECK(a == b);
    }

    TEST_CASE("equal-invalid-utf8-nodes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::InvalidUtf8>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::InvalidUtf8>();
        CHECK(a == b);
    }

    // Different empty types should NOT be equal
    TEST_CASE("different-empty-node-types") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::LineBreak>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Space>();
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-html-node-types") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlBr>();
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::HtmlHr>();
        CHECK_FALSE(a == b);
    }

    // Text with same/different values
    TEST_CASE("equal-text-nodes") {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        CHECK(a == b);
    }

    TEST_CASE("different-text-nodes") {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        CHECK_FALSE(a == b);
    }

    // Group with empty sub-AST
    TEST_CASE("equal-empty-groups") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_a));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        CHECK(a == b);
    }

    // Group with non-empty sub-AST
    TEST_CASE("equal-groups-with-children") {
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
        CHECK(a == b);
    }

    // Group with different sub-AST content
    TEST_CASE("different-group-children") {
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
        CHECK_FALSE(a == b);
    }

    // Group with different sub-AST size
    TEST_CASE("different-group-sizes") {
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
        CHECK_FALSE(a == b);
    }

    // HtmlH1 with sub-AST
    TEST_CASE("equal-html-heading-children") {
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
        CHECK(a == b);
    }

    TEST_CASE("different-html-heading-children") {
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
        CHECK_FALSE(a == b);
    }

    // HtmlBlockquote with nested sub-AST
    TEST_CASE("equal-nested-blockquotes") {
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
        CHECK(a == b);
    }

    // CodeFence without language
    TEST_CASE("equal-markdown-code-fences-without-language") {
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
        CHECK(a == b);
    }

    // CodeFence with same language
    TEST_CASE("equal-markdown-code-fences-with-same-language") {
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
        CHECK(a == b);
    }

    // CodeFence with different languages
    TEST_CASE("different-markdown-code-fence-languages") {
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
        CHECK_FALSE(a == b);
    }

    // CodeFence: one has language, the other does not
    TEST_CASE("code-fence-language-presence-differs") {
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
        CHECK_FALSE(a == b);
    }

    // CodeFence without language
    TEST_CASE("equal-html-pre-nodes-without-language") {
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
        CHECK(a == b);
    }

    // CodeFence with same language
    TEST_CASE("equal-html-pre-nodes-with-same-language") {
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
        CHECK(a == b);
    }

    // CodeFence with different languages
    TEST_CASE("different-html-pre-languages") {
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
        CHECK_FALSE(a == b);
    }

    // CodeFence: one has language, the other does not
    TEST_CASE("html-pre-language-presence-differs") {
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
        CHECK_FALSE(a == b);
    }

    // MdLink with sub-AST and URL
    TEST_CASE("equal-markdown-links") {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_a),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_b),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        CHECK(a == b);
    }

    // MdLink with different URL
    TEST_CASE("markdown-links-with-different-urls") {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'a'));

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_a),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdLink<nd::quick_enforce>>(
                ::std::move(text_b),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.org").value<nd::quick_enforce>());
        CHECK_FALSE(a == b);
    }

    // PlainText preserves valid UTF-8 for every code point.
    TEST_CASE("plain-text-preserves-valid-utf8") {
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        text.append_code_point(char32_t{0x10FFFF});
        text.append_code_point(char32_t{0xD800});
        text.append_code_point(char32_t{0x110000});
        auto const expected = ::pltxt2htm::container::U8String{u8"\U0010FFFF\uFFFD\uFFFD"};
        CHECK(text.as_string() == expected);
    }

    // PlainText preserves tab and line feed, canonicalizes spaces to the no-break form, and replaces other ASCII
    // controls.
    TEST_CASE("plain-text-normalizes-controls-and-spaces") {
        auto const replacement = ::pltxt2htm::container::U8String{u8"\uFFFD"};
        for (char32_t control{}; control < char32_t{0x20}; ++control) {
            if (control == U'\t' || control == U'\n') {
                continue;
            }
            ::pltxt2htm::PlainText<nd::quick_enforce> text{};
            text.append_code_point(control);
            CHECK(text.as_string() == replacement);
        }
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        text.append_code_point(char32_t{0x7F});
        CHECK(text.as_string() == replacement);

        ::pltxt2htm::PlainText<nd::quick_enforce> boundaries{};
        boundaries.append_code_point(U'\t');
        boundaries.append_code_point(U'\n');
        boundaries.append_code_point(U' ');
        boundaries.append_code_point(U'~');
        boundaries.append_code_point(char32_t{0x80});
        auto const expected = ::pltxt2htm::container::U8String{u8"\t\n\u00A0~\u0080"};
        CHECK(boundaries.as_string() == expected);

        // U+0020 and U+00A0 are the same Physics-Lab space token, so both must be stored identically.
        ::pltxt2htm::PlainText<nd::quick_enforce> ascii_space{};
        ascii_space.append_code_point(U' ');
        ::pltxt2htm::PlainText<nd::quick_enforce> no_break_space{};
        no_break_space.append_code_point(char32_t{0xA0});
        CHECK(ascii_space.as_string() == no_break_space.as_string());
        CHECK(ascii_space.as_string() == ::pltxt2htm::container::U8String{u8"\u00A0"});
    }

    // UTF-8 appends consume one sequence and preserve invalid-prefix recovery.
    TEST_CASE("plain-text-appends-one-utf8-sequence") {
        ::pltxt2htm::PlainText<nd::quick_enforce> text{};
        CHECK(text.append_first_utf8_code_point(u8"") == 0);
        CHECK(text.as_string() == ::pltxt2htm::container::U8String{});
        CHECK(text.append_first_utf8_code_point(u8"Ax") == 1);
        CHECK(text.append_first_utf8_code_point(u8"\u00A2x") == 2);
        CHECK(text.append_first_utf8_code_point(u8"\u20ACx") == 3);
        CHECK(text.append_first_utf8_code_point(u8"\U0001F600x") == 4);
        CHECK(text.append_first_utf8_code_point(u8"\t") == 1);
        CHECK(text.append_first_utf8_code_point(u8"\n") == 1);
        CHECK(text.append_first_utf8_code_point(u8"\xE2\x82"
                                                "x") == 2);
        CHECK(text.append_first_utf8_code_point(u8"\xC0\xAF") == 2);
        CHECK(text.append_first_utf8_code_point(u8"\xFF") == 1);
        auto const expected = ::pltxt2htm::container::U8String{u8"A\u00A2\u20AC\U0001F600\t\n\uFFFD\uFFFD\uFFFD"};
        CHECK(text.as_string() == expected);
    }

    // MdImage with plain-text alt and URL
    TEST_CASE("equal-markdown-images") {
        ::pltxt2htm::PlainText<nd::quick_enforce> alt_a{};
        alt_a.append_code_point(U'a');

        ::pltxt2htm::PlainText<nd::quick_enforce> alt_b{};
        alt_b.append_code_point(U'a');

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdImage<nd::quick_enforce>>(
                ::std::move(alt_a),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdImage<nd::quick_enforce>>(
                ::std::move(alt_b),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        CHECK(a == b);
    }

    // ListLiCheckbox (sub-AST + bool checked)
    TEST_CASE("equal-checked-list-items") {
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
        CHECK(a == b);
    }

    TEST_CASE("different-list-item-checkboxes") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_a), true);
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::ListLiCheckbox<nd::quick_enforce>>(
                ::std::move(ast_b), false);
        CHECK_FALSE(a == b);
    }

    // TableTh / TableTd (sub-AST + TableAlign)
    TEST_CASE("equal-table-headers") {
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
        CHECK(a == b);
    }

    TEST_CASE("different-table-header-alignments") {
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
        CHECK_FALSE(a == b);
    }

    TEST_CASE("equal-table-cells") {
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
        CHECK(a == b);
    }

    // UnityColor (sub-AST + color string)
    TEST_CASE("equal-unity-colors") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::U8String{u8"red"});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::U8String{u8"red"});
        CHECK(a == b);
    }

    TEST_CASE("different-unity-colors") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::U8String{u8"red"});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnityColor<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::container::U8String{u8"blue"});
        CHECK_FALSE(a == b);
    }

    // PlExperiment (sub-AST + ObjectId)
    TEST_CASE("equal-experiments") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_a), make_object_id(u8"642cf37a494746375aae306a"));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_b), make_object_id(u8"642cf37a494746375aae306a"));
        CHECK(a == b);
    }

    TEST_CASE("experiments-with-different-ids") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_a), make_object_id(u8"642cf37a494746375aae306a"));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExperiment<nd::quick_enforce>>(
                ::std::move(ast_b), make_object_id(u8"642cf37a494746375aae306b"));
        CHECK_FALSE(a == b);
    }

    // UnitySize (sub-AST + ValueWithUnit)
    TEST_CASE("equal-unity-sizes") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        CHECK(a == b);
    }

    TEST_CASE("different-unity-size-units") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b), ::pltxt2htm::ValueWithUnit<double>{.value = 16, .unit = ::pltxt2htm::Unit::px});
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-unity-size-values") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::px});
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::UnitySize<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::ValueWithUnit<double>{.value = 14, .unit = ::pltxt2htm::Unit::percent});
        CHECK_FALSE(a == b);
    }

    // PlExternal (sub-AST + Url)
    TEST_CASE("equal-external-nodes") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExternal<nd::quick_enforce>>(
                ::std::move(ast_a),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::PlExternal<nd::quick_enforce>>(
                ::std::move(ast_b),
                ::pltxt2htm::Url::try_make<nd::quick_enforce>(u8"https://example.com").value<nd::quick_enforce>());
        CHECK(a == b);
    }

    // Ast-level comparison (empty)
    TEST_CASE("equal-empty-asts") {
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_b{};
        CHECK(ast_a == ast_b);
    }

    // Ast-level comparison (single matching elements)
    TEST_CASE("equal-single-node-asts") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        CHECK(ast_a == ast_b);
    }

    // Ast-level comparison (multiple matching elements)
    TEST_CASE("equal-multi-node-asts") {
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

        CHECK(ast_a == ast_b);
    }

    // Ast-level comparison (different elements)
    TEST_CASE("different-ast-elements") {
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

        CHECK_FALSE(ast_a == ast_b);
    }

    // Ast-level comparison (different sizes)
    TEST_CASE("different-ast-sizes") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A'));
        ast_b.emplace_back(
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B'));

        CHECK_FALSE(ast_a == ast_b);
    }

    // C++20 operator!= auto-generation
    TEST_CASE("text-inequality-operator") {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'B');
        CHECK(a != b);
    }

    TEST_CASE("inequality-operator-for-equal-text") {
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Text<nd::quick_enforce>>(u8'A');
        CHECK_FALSE(a != b);
    }

    // MdEscape with same/different escaped characters
    TEST_CASE("equal-backslash-escapes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        CHECK(a == b);
        CHECK(a.as_md_escape().get_character() == u8'\\');
    }

    TEST_CASE("equal-asterisk-escapes") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        CHECK(a == b);
    }

    TEST_CASE("different-escape-characters") {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'\\');
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::MdEscape>(u8'*');
        CHECK_FALSE(a == b);
    }

    // UnityI (sub-AST only)
    TEST_CASE("equal-unity-inline-nodes") {
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
        CHECK(a == b);
    }

    // UnityB (sub-AST only)
    TEST_CASE("equal-unity-block-nodes") {
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
        CHECK(a == b);
    }

    // Cross-type: same sub-AST, different wrapper type
    TEST_CASE("different-wrapper-types-with-same-content") {
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
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-wrapper-types-with-equal-empty-content") {
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
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-wrapper-types-with-same-child-node") {
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
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-wrapper-types-with-different-child-nodes") {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::CodeFence<nd::quick_enforce>>(
                ::std::move(ast_a), ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>(
                                        ::pltxt2htm::container::U8String{u8"cpp"}));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>::template emplace<::pltxt2htm::Group<nd::quick_enforce>>(
                ::std::move(ast_b));
        CHECK_FALSE(a == b);
    }

    // Sub-AST count mismatch
    TEST_CASE("different-sub-ast-item-counts") {
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
        CHECK_FALSE(a == b);
    }

    TEST_CASE("different-sub-ast-item-values") {
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
        CHECK_FALSE(a == b);
    }
}
