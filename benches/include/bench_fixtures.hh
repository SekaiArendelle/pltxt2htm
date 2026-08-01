#pragma once

#include <benchmark/benchmark.h>
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/contracts.hh>
#include "bench_inputs.hh"

#ifdef NDEBUG
constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif

// Helper: convert range(0) to std::size_t
inline auto as_size(::benchmark::State const& state) -> ::std::size_t {
    return static_cast<::std::size_t>(state.range(0));
}

// -------------------------------------------------------------------
// Parse fixtures — stores input string
// -------------------------------------------------------------------
struct PlainTextParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_plain_text(as_size(state));
    }
};

struct RichMarkdownParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_rich_markdown(as_size(state));
    }
};

struct PlTagsNestedParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_pl_tags_nested(as_size(state));
    }
};

struct PlTagsMixedParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_pl_tags_mixed(as_size(state));
    }
};

struct LatexHeavyParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_latex_heavy(as_size(state));
    }
};

struct StressLongLineParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_stress_long_line(as_size(state));
    }
};

struct StressManyLinesParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_stress_many_lines(as_size(state));
    }
};

struct RedundantColorParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_redundant_color_nesting(as_size(state));
    }
};

struct AdjacentTextParseFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_adjacent_text_nodes(as_size(state));
    }
};

// -------------------------------------------------------------------
// End2End fixtures — stores input string
// -------------------------------------------------------------------
struct FullDocE2EFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_full_document(as_size(state));
    }
};

struct TerseDocE2EFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_terse_document(as_size(state));
    }
};

struct PlainDocE2EFixture : ::benchmark::Fixture {
    ::fast_io::u8string input;

    void SetUp(::benchmark::State& state) override {
        input = make_plain_document(as_size(state));
    }
};

// -------------------------------------------------------------------
// AST builder helpers — construct ASTs directly for optimizer benches
// -------------------------------------------------------------------

inline auto build_nested_color_ast(::std::size_t depth) -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::PlTxtNode<ndebug> inner{::pltxt2htm::U8Char{u8't'}};
    for (::std::size_t i = 0; i < depth; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::std::move(inner));
        inner = ::pltxt2htm::PlTxtNode<ndebug>{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
    }
    ::pltxt2htm::Ast<ndebug> ast;
    ast.push_back(::std::move(inner));
    return ast;
}

inline auto build_adjacent_color_ast(::std::size_t count) -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::Ast<ndebug> ast;
    for (::std::size_t i = 0; i < count; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{
            ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}});
    }
    return ast;
}

inline auto build_mixed_redundant_ast(::std::size_t depth) -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::PlTxtNode<ndebug> inner{::pltxt2htm::U8Char{u8't'}};
    for (::std::size_t i = 0; i < depth; ++i) {
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::PlI<ndebug>{::std::move(sub)}};
        }
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::PlB<ndebug>{::std::move(sub)}};
        }
        {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::std::move(inner));
            inner = ::pltxt2htm::PlTxtNode<ndebug>{
                ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
        }
    }
    ::pltxt2htm::Ast<ndebug> ast;
    ast.push_back(::std::move(inner));
    return ast;
}

inline auto build_html_span_ast(::std::size_t count) -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::Ast<ndebug> ast;
    for (::std::size_t i = 0; i < count; ++i) {
        ::pltxt2htm::Ast<ndebug> sub;
        sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
        ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{
            ::pltxt2htm::HtmlSpan<ndebug>{::std::move(sub), ::fast_io::u8string{u8"color:red;font-size:16px"},
                                          ::exception::optional<::pltxt2htm::FontSizeValue>{::exception::nullopt}}});
    }
    return ast;
}
