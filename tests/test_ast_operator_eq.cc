#include <pltxt2htm/ast/ast.hh>

int main() {
    using nd = ::pltxt2htm::Contracts;

    // Empty/stateless nodes
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Space{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Space{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlBr{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlBr{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlHr{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlHr{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlCol{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlCol{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdHr{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdHr{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlMacroProject{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlMacroProject{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::InvalidU8Char{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::InvalidU8Char{});
        ::exception::assert_true<false>(a == b);
    }

    // Different empty types should NOT be equal
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::LineBreak{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Space{});
        ::exception::assert_false<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlBr{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlHr{});
        ::exception::assert_false<false>(a == b);
    }

    // U8Char with same/different values
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'B'});
        ::exception::assert_false<false>(a == b);
    }

    // Text with empty sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_true<false>(a == b);
    }

    // Text with non-empty sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'i'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'i'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_true<false>(a == b);
    }

    // Text with different sub-AST content
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'i'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'o'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }

    // Text with different sub-AST size
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'i'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'H'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }

    // HtmlH1 with sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'X'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'X'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'X'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'Y'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }

    // HtmlBlockquote with nested sub-AST
    {
        ::pltxt2htm::Ast<nd::quick_enforce> inner_a{};
        inner_a.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> outer_a{};
        outer_a.emplace_back(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(inner_a)));

        ::pltxt2htm::Ast<nd::quick_enforce> inner_b{};
        inner_b.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> outer_b{};
        outer_b.emplace_back(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(inner_b)));

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::HtmlBlockquote<nd::quick_enforce>(::std::move(outer_a)));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::HtmlBlockquote<nd::quick_enforce>(::std::move(outer_b)));
        ::exception::assert_true<false>(a == b);
    }

    // MdCodeFenceBacktick without language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'a'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        ::exception::assert_true<false>(a == b);
    }

    // MdCodeFenceBacktick with same language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'a'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        ::exception::assert_true<false>(a == b);
    }

    // MdCodeFenceBacktick with different languages
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'a'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'a'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"python"})));
        ::exception::assert_false<false>(a == b);
    }

    // MdCodeFenceBacktick: one has language, other does not
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        ::exception::assert_false<false>(a == b);
    }

    // MdCodeFenceTilde without language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        ::exception::assert_true<false>(a == b);
    }

    // MdCodeFenceTilde with same language
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        ::exception::assert_true<false>(a == b);
    }

    // MdCodeFenceTilde with different languages
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"python"})));
        ::exception::assert_false<false>(a == b);
    }

    // MdCodeFenceTilde: one has language, other does not
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::exception::nullopt_t{})));
        ::exception::assert_false<false>(a == b);
    }

    // MdLink with sub-AST and URL
    {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_a{};
        url_ast_a.emplace_back(::pltxt2htm::U8Char{u8'x'});

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_b{};
        url_ast_b.emplace_back(::pltxt2htm::U8Char{u8'x'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdLink<nd::quick_enforce>(
            ::std::move(text_a), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_a))));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdLink<nd::quick_enforce>(
            ::std::move(text_b), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_b))));
        ::exception::assert_true<false>(a == b);
    }

    // MdLink with different URL
    {
        ::pltxt2htm::Ast<nd::quick_enforce> text_a{};
        text_a.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_a{};
        url_ast_a.emplace_back(::pltxt2htm::U8Char{u8'x'});

        ::pltxt2htm::Ast<nd::quick_enforce> text_b{};
        text_b.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_b{};
        url_ast_b.emplace_back(::pltxt2htm::U8Char{u8'y'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdLink<nd::quick_enforce>(
            ::std::move(text_a), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_a))));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdLink<nd::quick_enforce>(
            ::std::move(text_b), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_b))));
        ::exception::assert_false<false>(a == b);
    }

    // MdImage with sub-AST and URL
    {
        ::pltxt2htm::Ast<nd::quick_enforce> alt_a{};
        alt_a.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_a{};
        url_ast_a.emplace_back(::pltxt2htm::U8Char{u8'x'});

        ::pltxt2htm::Ast<nd::quick_enforce> alt_b{};
        alt_b.emplace_back(::pltxt2htm::U8Char{u8'a'});
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_b{};
        url_ast_b.emplace_back(::pltxt2htm::U8Char{u8'x'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdImage<nd::quick_enforce>(
            ::std::move(alt_a), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_a))));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdImage<nd::quick_enforce>(
            ::std::move(alt_b), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_b))));
        ::exception::assert_true<false>(a == b);
    }

    // MdLiCheckbox (sub-AST + bool checked)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8't'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8't'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdLiCheckbox<nd::quick_enforce>(::std::move(ast_a), true));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdLiCheckbox<nd::quick_enforce>(::std::move(ast_b), true));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdLiCheckbox<nd::quick_enforce>(::std::move(ast_a), true));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdLiCheckbox<nd::quick_enforce>(::std::move(ast_b), false));
        ::exception::assert_false<false>(a == b);
    }

    // MdTh / MdTd (sub-AST + MdTableAlign)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'h'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'h'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTh<nd::quick_enforce>(::std::move(ast_a), ::pltxt2htm::MdTableAlign::center));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTh<nd::quick_enforce>(::std::move(ast_b), ::pltxt2htm::MdTableAlign::center));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'h'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'h'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTh<nd::quick_enforce>(::std::move(ast_a), ::pltxt2htm::MdTableAlign::left));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTh<nd::quick_enforce>(::std::move(ast_b), ::pltxt2htm::MdTableAlign::center));
        ::exception::assert_false<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'd'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'd'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTd<nd::quick_enforce>(::std::move(ast_a), ::pltxt2htm::MdTableAlign::right));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::MdTd<nd::quick_enforce>(::std::move(ast_b), ::pltxt2htm::MdTableAlign::right));
        ::exception::assert_true<false>(a == b);
    }

    // PlColor (sub-AST + color string)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlColor<nd::quick_enforce>(::std::move(ast_a), ::fast_io::u8string{u8"red"}));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlColor<nd::quick_enforce>(::std::move(ast_b), ::fast_io::u8string{u8"red"}));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlColor<nd::quick_enforce>(::std::move(ast_a), ::fast_io::u8string{u8"red"}));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlColor<nd::quick_enforce>(::std::move(ast_b), ::fast_io::u8string{u8"blue"}));
        ::exception::assert_false<false>(a == b);
    }

    // PlExperiment (sub-AST + id string)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlExperiment<nd::quick_enforce>(::std::move(ast_a), ::fast_io::u8string{u8"exp1"}));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlExperiment<nd::quick_enforce>(::std::move(ast_b), ::fast_io::u8string{u8"exp1"}));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlExperiment<nd::quick_enforce>(::std::move(ast_a), ::fast_io::u8string{u8"exp1"}));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::PlExperiment<nd::quick_enforce>(::std::move(ast_b), ::fast_io::u8string{u8"exp2"}));
        ::exception::assert_false<false>(a == b);
    }

    // PlSize (sub-AST + size_t)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlSize<nd::quick_enforce>(::std::move(ast_a), 14));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlSize<nd::quick_enforce>(::std::move(ast_b), 14));
        ::exception::assert_true<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlSize<nd::quick_enforce>(::std::move(ast_a), 14));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlSize<nd::quick_enforce>(::std::move(ast_b), 16));
        ::exception::assert_false<false>(a == b);
    }

    // PlExternal (sub-AST + Url)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_a{};
        url_ast_a.emplace_back(::pltxt2htm::U8Char{u8'x'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ::pltxt2htm::Ast<nd::quick_enforce> url_ast_b{};
        url_ast_b.emplace_back(::pltxt2htm::U8Char{u8'x'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlExternal<nd::quick_enforce>(
            ::std::move(ast_a), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_a))));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlExternal<nd::quick_enforce>(
            ::std::move(ast_b), ::pltxt2htm::Url<nd::quick_enforce>(::std::move(url_ast_b))));
        ::exception::assert_true<false>(a == b);
    }

    // Ast-level comparison (empty)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_a{};
        ::pltxt2htm::Ast<nd::quick_enforce> const ast_b{};
        ::exception::assert_true<false>(ast_a == ast_b);
    }

    // Ast-level comparison (single matching elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'A'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'A'});

        ::exception::assert_true<false>(ast_a == ast_b);
    }

    // Ast-level comparison (multiple matching elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::exception::assert_true<false>(ast_a == ast_b);
    }

    // Ast-level comparison (different elements)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'C'});

        ::exception::assert_false<false>(ast_a == ast_b);
    }

    // Ast-level comparison (different sizes)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'A'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::exception::assert_false<false>(ast_a == ast_b);
    }

    // C++20 operator!= auto-generation
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'B'});
        ::exception::assert_true<false>(a != b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::U8Char{u8'A'});
        ::exception::assert_false<false>(a != b);
    }

    // MdEscape (all empty, always equal)
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeBackslash{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeBackslash{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeAsterisk{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeAsterisk{});
        ::exception::assert_true<false>(a == b);
    }
    {
        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeBackslash{});
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdEscapeAsterisk{});
        ::exception::assert_false<false>(a == b);
    }

    // PlI (sub-AST only)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8't'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8't'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlI<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlI<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_true<false>(a == b);
    }

    // PlB (sub-AST only)
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8't'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8't'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlB<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlB<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_true<false>(a == b);
    }

    // Cross-type: same sub-AST, different wrapper type
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'i'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'i'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'X'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'X'});

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(
            ::pltxt2htm::HtmlBlockquote<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8't'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8't'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlI<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::PlB<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};

        auto const a = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceBacktick<nd::quick_enforce>(
            ::std::move(ast_a), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        auto const b = ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::MdCodeFenceTilde<nd::quick_enforce>(
            ::std::move(ast_b), ::exception::optional<::fast_io::u8string>(::fast_io::u8string{u8"cpp"})));
        ::exception::assert_false<false>(a == b);
    }

    // Sub-AST count mismatch
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'H'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'i'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'!'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'H'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::Text<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }
    {
        ::pltxt2htm::Ast<nd::quick_enforce> ast_a{};
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'A'});
        ast_a.emplace_back(::pltxt2htm::U8Char{u8'B'});

        ::pltxt2htm::Ast<nd::quick_enforce> ast_b{};
        ast_b.emplace_back(::pltxt2htm::U8Char{u8'A'});

        auto const a =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_a)));
        auto const b =
            ::pltxt2htm::PlTxtNode<nd::quick_enforce>(::pltxt2htm::HtmlH1<nd::quick_enforce>(::std::move(ast_b)));
        ::exception::assert_false<false>(a == b);
    }

    return 0;
}
