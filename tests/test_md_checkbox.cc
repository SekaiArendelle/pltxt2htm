#include <pltxt2htm/details/parser/md_list.hh>
#include <pltxt2htm/details/parser/frame_concext.hh>
#include <pltxt2htm/parser.hh>
#include <pltxt2htm/ast/ast.hh>
#include <cstdio>

int main() {
    // Test 1: optionally_to_md_list_ast
    {
        using detail = ::pltxt2htm::details;
        auto opt = detail::optionally_to_md_list_ast<::pltxt2htm::Contracts::quick_enforce>(u8"- [ ] task");
        if (!opt.has_value()) {
            std::printf("FAIL: optionally_to_md_list_ast returned nullopt\n");
            return 1;
        }
        auto& md_ast = opt.template value<false>().ast;
        if (md_ast.empty()) {
            std::printf("FAIL: ast is empty\n");
            return 1;
        }
        auto iter = md_ast.begin();
        if (iter->get_type() != detail::MdListNodeType::text) {
            std::printf("FAIL: not a text node\n");
            return 1;
        }
        if (!iter->is_checkbox()) {
            std::printf("FAIL: is_checkbox() is false in MdListAst\n");
            return 1;
        }
        std::printf("PASS: optionally_to_md_list_ast checkbox=%d checked=%d\n",
                    (int)iter->is_checkbox(), (int)iter->is_checked());
    }

    // Test 2: parse_pltxt top-level
    {
        auto ast = ::pltxt2htm::parse_pltxt<::pltxt2htm::Contracts::quick_enforce>(u8"- [ ] task");
        std::printf("Top-level AST has %zu nodes\n", ast.size());
        for (auto& node : ast) {
            std::printf("  node kind = %d\n", (int)node.get_node_kind());
            if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_li) {
                std::printf("  md_li: checkbox=%d checked=%d\n",
                            (int)node.is_checkbox(), (int)node.is_checked());
            }
            else if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_ul) {
                auto& subast = node.get_subast();
                std::printf("  md_ul has %zu children\n", subast.size());
                for (auto& child : subast) {
                    std::printf("    child kind = %d\n", (int)child.get_node_kind());
                    if (child.get_node_kind() == ::pltxt2htm::NodeKind::md_li) {
                        std::printf("    md_li: checkbox=%d checked=%d\n",
                                    (int)child.is_checkbox(), (int)child.is_checked());
                    }
                }
            }
        }
        // Actually verify
        bool found = false;
        for (auto& node : ast) {
            if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_li && node.is_checkbox()) {
                found = true;
            }
            else if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_ul) {
                for (auto& child : node.get_subast()) {
                    if (child.get_node_kind() == ::pltxt2htm::NodeKind::md_li && child.is_checkbox()) {
                        found = true;
                    }
                }
            }
        }
        if (!found) {
            std::printf("FAIL: no checkbox md_li found in parse_pltxt result\n");
            return 1;
        }
        std::printf("PASS: parse_pltxt result has checkbox\n");
    }
    return 0;
}
