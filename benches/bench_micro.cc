#include "bench_harness.hh"
#include <pltxt2htm/ast/ast.hh>
#include <pltxt2htm/ast/node_kind.hh>
#include <pltxt2htm/contracts.hh>
#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>

using namespace pltxt2htm_bench;
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

namespace {

void bench_node_creation() {
    ::fast_io::io::print(::fast_io::u8out(), u8"--- Node Creation ---\n");

    {
        auto res = benchmark([] {
            ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::U8Char{u8'A'}};
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode(U8Char)", u8"construct", res);
        print_result_csv(u8"PlTxtNode(U8Char)", u8"construct", res);
    }

    {
        auto res = benchmark([] {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
            ::pltxt2htm::PlTxtNode<ndebug> node{::pltxt2htm::Text<ndebug>{::std::move(sub)}};
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode(Text)", u8"construct", res);
        print_result_csv(u8"PlTxtNode(Text)", u8"construct", res);
    }

    {
        auto res = benchmark([] {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
            ::pltxt2htm::PlTxtNode<ndebug> node{
                ::pltxt2htm::PlColor<ndebug>{::std::move(sub), ::fast_io::u8string{u8"red"}}};
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode(PlColor)", u8"construct", res);
        print_result_csv(u8"PlTxtNode(PlColor)", u8"construct", res);
    }

    {
        auto res = benchmark([] {
            ::pltxt2htm::Ast<ndebug> sub;
            sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8't'}});
            ::pltxt2htm::PlTxtNode<ndebug> node{
                ::pltxt2htm::HtmlSpan<ndebug>{::std::move(sub), ::fast_io::u8string{u8"color:red;"},
                                               ::exception::optional<::std::size_t>{}}};
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode(HtmlSpan)", u8"construct", res);
        print_result_csv(u8"PlTxtNode(HtmlSpan)", u8"construct", res);
    }

    {
        auto res = benchmark([] {
            ::pltxt2htm::Ast<ndebug> text_sub, url_sub;
            text_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'L'}});
            url_sub.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'/'}});
            ::pltxt2htm::Url<ndebug> url{::std::move(url_sub)};
            ::pltxt2htm::PlTxtNode<ndebug> node{
                ::pltxt2htm::MdLink<ndebug>{::std::move(text_sub), ::std::move(url)}};
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode(MdLink)", u8"construct", res);
        print_result_csv(u8"PlTxtNode(MdLink)", u8"construct", res);
    }
}

void bench_node_move() {
    ::fast_io::io::print(::fast_io::u8out(), u8"--- Node Move ---\n");

    {
        ::pltxt2htm::PlTxtNode<ndebug> src{::pltxt2htm::U8Char{u8'A'}};
        auto res = benchmark([&] {
            ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
            src = ::std::move(dst);
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode move", u8"trivial", res);
        print_result_csv(u8"PlTxtNode move", u8"trivial", res);
    }

    {
        ::pltxt2htm::Ast<ndebug> nested;
        nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
        nested.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'B'}});
        ::pltxt2htm::PlTxtNode<ndebug> src{
            ::pltxt2htm::Text<ndebug>{::std::move(nested)}};
        auto res = benchmark([&] {
            ::pltxt2htm::PlTxtNode<ndebug> dst{::std::move(src)};
            src = ::std::move(dst);
            return ::fast_io::u8string{};
        }, 0, 0, 100, 1000);
        print_result(u8"PlTxtNode move", u8"with sub-ast", res);
        print_result_csv(u8"PlTxtNode move", u8"with_sub_ast", res);
    }
}

void bench_ast_append() {
    ::fast_io::io::print(::fast_io::u8out(), u8"--- AST Append ---\n");

    {
        auto res = benchmark([] {
            ::pltxt2htm::Ast<ndebug> ast;
            for (int i = 0; i < 1000; ++i) {
                ast.push_back(::pltxt2htm::PlTxtNode<ndebug>{::pltxt2htm::U8Char{u8'A'}});
            }
            return ::fast_io::u8string{};
        }, 0, 0, 10, 50);
        print_result(u8"AST push_back 1000 nodes", u8"", res);
        print_result_csv(u8"AST push_back 1000 nodes", u8"", res);
    }
}

void bench_string_ops() {
    ::fast_io::io::print(::fast_io::u8out(), u8"--- String Operations ---\n");

    {
        auto res = benchmark([] {
            ::fast_io::u8string s;
            s.reserve(10000);
            for (int i = 0; i < 100; ++i) {
                s.append(u8"<span style=\"color:red;\">text</span>");
            }
            return s;
        }, 0, 0, 10, 100);
        print_result(u8"u8string append \u00d7100 (reserved)", u8"", res);
        print_result_csv(u8"u8string append x100 (reserved)", u8"", res);
    }

    {
        auto res = benchmark([] {
            ::fast_io::u8string s;
            for (int i = 0; i < 100; ++i) {
                s.append(u8"<span style=\"color:red;\">text</span>");
            }
            return s;
        }, 0, 0, 10, 100);
        print_result(u8"u8string append \u00d7100 (no reserve)", u8"", res);
        print_result_csv(u8"u8string append x100 (no reserve)", u8"", res);
    }
}

} // anonymous namespace

int main() {
    ::fast_io::io::print(::fast_io::u8out(), u8"=== bench_micro: Micro-Operation Benchmarks ===\n\n");

    print_csv_header();

    bench_node_creation();
    ::fast_io::io::print(::fast_io::u8out(), u8"\n");
    bench_node_move();
    ::fast_io::io::print(::fast_io::u8out(), u8"\n");
    bench_ast_append();
    ::fast_io::io::print(::fast_io::u8out(), u8"\n");
    bench_string_ops();

    return 0;
}
