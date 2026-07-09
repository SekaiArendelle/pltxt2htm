#include "precompile.hh"
#include "bench_harness.hh"
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/optimizer.hh>
#include <pltxt2htm/contracts.hh>
#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>

using namespace pltxt2htm_bench;
constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;

namespace {

::fast_io::u8string make_redundant_color_nesting(::std::size_t depth = 200) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"<color=red>");
    }
    input.append(u8"redundant nested text");
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"</color>");
    }
    return input;
}

::fast_io::u8string make_mixed_redundant(::std::size_t depth = 100) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"<color=red><b><i>");
    }
    input.append(u8"deep mixed formatting");
    for (::std::size_t i = 0; i < depth; ++i) {
        input.append(u8"</i></b></color>");
    }
    return input;
}

::fast_io::u8string make_adjacent_text_nodes(::std::size_t segments = 500) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < segments; ++i) {
        input.append(u8"<color=red>segment_");
        input.push_back(u8'0' + static_cast<char8_t>(i % 10));
        if (i % 2 == 0) {
            input.append(u8"</color><color=red>");
        } else {
            input.append(u8"</color>");
        }
    }
    return input;
}

::fast_io::u8string make_html_span_attrs(::std::size_t spans = 300) {
    ::fast_io::u8string input;
    for (::std::size_t i = 0; i < spans; ++i) {
        input.append(u8R"(<span style="color:red;font-size:16px">text</span>
)");
    }
    return input;
}

} // anonymous namespace

int main() {
    ::fast_io::io::print(::fast_io::u8out(), u8"=== bench_optimize: AST Optimizer Performance ===\n\n");

    auto inputs = ::std::vector<::std::pair<::fast_io::u8string_view, ::fast_io::u8string>>{
        {u8"redundant_color_200x", make_redundant_color_nesting(200)},
        {u8"mixed_redundant_100x", make_mixed_redundant(100)},
        {u8"adjacent_text_500x",   make_adjacent_text_nodes(500)},
        {u8"html_span_attrs_300x", make_html_span_attrs(300)},
    };

    print_csv_header();

    for (auto const& inp : inputs) {
        auto sv = ::fast_io::u8string_view{inp.second.data(), inp.second.size()};
        auto ast = ::pltxt2htm::parse_pltxt<ndebug>(sv);

        {
            auto ast_copy = ast;
            auto res = benchmark_void(
                [&] { ::pltxt2htm::optimize_ast<ndebug>(ast_copy); },
                sv.size(), 5, 20);
            print_result(inp.first, u8"optimize (quick_enforce)", res);
            print_result_csv(inp.first, u8"optimize", res);
        }

        {
            auto res_parse = benchmark(
                [&] { return ::pltxt2htm::parse_pltxt<ndebug>(sv); },
                sv.size(), 0, 3, 10);
            print_result(inp.first, u8"parse (for comparison)", res_parse);
            print_result_csv(inp.first, u8"parse", res_parse);
        }
    }

    return 0;
}
