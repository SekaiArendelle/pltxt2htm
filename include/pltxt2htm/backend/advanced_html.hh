#pragma once

#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/array.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "frame_context.hh"
#include "../utils.hh"
#include "../astnode/basic.hh"
#include "../astnode/node_type.hh"
#include "../astnode/html_node.hh"
#include "../astnode/physics_lab_node.hh"
#include "../heap_guard.hh"

namespace pltxt2htm::details {

/**
 * @brief Integrate ast nodes to HTML.
 * @tparam ndebug: true  -> release mode, disables most of the checks which is unsafe but fast
 *                 false -> debug mode, enable all checks
 * @tparam escape_less_than: Whether escaping `<` to `&lt;`
 * @param [in] ast_init: Ast of Quantum-Physics's text
 * @note To avoid stack overflow, this function manage `call_stack` by hand.
 */
template<bool ndebug, bool escape_less_than = true>
[[nodiscard]]
constexpr auto ast2advanced_html(
    ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast_init,
    ::fast_io::u8string_view host)
#if __cpp_exceptions < 199711L
    noexcept
#endif
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>,
                     ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        ast_init, ::pltxt2htm::NodeType::base, false, 0));

restart:
    auto&& ast = call_stack.top()->ast_;
    auto&& current_index = call_stack.top()->current_index_;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            auto escape_str = ::fast_io::array{char8_t{0xef}, 0xbf, 0xbd};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            auto escape_str = ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            auto escape_str = ::fast_io::array{u8'&', u8'a', u8'm', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            auto escape_str = ::fast_io::array{u8'&', u8'a', u8'p', u8'o', u8's', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            auto escape_str = ::fast_io::array{u8'&', u8'q', u8'u', u8'o', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            if constexpr (escape_less_than) {
                auto escape_str = ::fast_io::array{u8'&', u8'l', u8't', u8';'};
                result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            } else {
                result.push_back(u8'<');
            }
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            auto escape_str = ::fast_io::array{u8'&', u8'g', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::tab: {
            auto escape_str =
                ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';',
                                 u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = reinterpret_cast<::pltxt2htm::Color const*>(node.release_imul());
            if (color->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            {
                // <color=red><color=blue>text</color></color> can be optimized
                auto&& subast = color->get_subast();
                if (subast.size() == 1) {
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_color) {
                        color = reinterpret_cast<::pltxt2htm::Color const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                (nested_tag_type != ::pltxt2htm::NodeType::pl_color &&
                 nested_tag_type != ::pltxt2htm::NodeType::pl_a) ||
                color->get_color() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
                                          call_stack.top().release_imul())
                                          ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
                color->get_subast(), ::pltxt2htm::NodeType::pl_color, is_not_same_tag, 0,
                ::fast_io::mnp::os_c_str(color->get_color())));
            ++current_index;
            if (is_not_same_tag) {
                auto close_tag1 =
                    ::fast_io::array{u8'<', u8's', u8'p',  u8'a', u8'n', u8' ', u8's', u8't', u8'y', u8'l',
                                     u8'e', u8'=', u8'\"', u8'c', u8'o', u8'l', u8'o', u8'r', u8':'};
                result.append(::fast_io::u8string_view{close_tag1.data(), close_tag1.size()});
                result.append(color->get_color());
                auto close_tag2 = ::fast_io::array{u8';', u8'\"', u8'>'};
                result.append(::fast_io::u8string_view{close_tag2.data(), close_tag2.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto experiment = reinterpret_cast<::pltxt2htm::Experiment const*>(node.release_imul());
            if (experiment->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            {
                auto&& subast = experiment->get_subast();
                if (subast.size() == 1) {
                    // <Experiment=123><experiment=642cf37a494746375aae306a>physicsLab</experiment></Experiment> can be
                    // optimized as <a href=\"localhost:5173/ExperimentSummary/Experiment/642cf37a494746375aae306a\"
                    // internal>physicsLab</a>
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_experiment) {
                        experiment = reinterpret_cast<::pltxt2htm::Experiment const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the experiment is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_experiment ||
                experiment->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
                                            call_stack.top().release_imul())
                                            ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
                experiment->get_subast(), ::pltxt2htm::NodeType::pl_experiment, is_not_same_tag, 0,
                ::fast_io::mnp::os_c_str(experiment->get_id())));
            ++current_index;
            if (is_not_same_tag) {
                result.append(u8"<a href=\"");
                result.append(host);
                result.append(u8"/ExperimentSummary/Experiment/");
                result.append(experiment->get_id());
                result.append(u8"\" internal>");
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto discussion = reinterpret_cast<::pltxt2htm::Discussion const*>(node.release_imul());
            if (discussion->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            {
                auto&& subast = discussion->get_subast();
                if (subast.size() == 1) {
                    // <Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion> can be
                    // optimized as <a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\"
                    // internal>physicsLab</a>
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_discussion) {
                        discussion = reinterpret_cast<::pltxt2htm::Discussion const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the discussion is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_discussion ||
                discussion->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
                                            call_stack.top().release_imul())
                                            ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
                discussion->get_subast(), ::pltxt2htm::NodeType::pl_discussion, is_not_same_tag, 0,
                ::fast_io::mnp::os_c_str(discussion->get_id())));
            ++current_index;
            if (is_not_same_tag) {
                result.append(u8"<a href=\"");
                result.append(host);
                result.append(u8"/ExperimentSummary/Discussion/");
                result.append(discussion->get_id());
                result.append(u8"\" internal>");
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_user: {
            auto user = reinterpret_cast<::pltxt2htm::User const*>(node.release_imul());
            if (user->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            {
                auto&& subast = user->get_subast();
                if (subast.size() == 1) {
                    // <User=123><user=642cf37a494746375aae306a>physicsLab</user></User> can be
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_user) {
                        user = reinterpret_cast<::pltxt2htm::User const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the user is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_user ||
                user->get_id() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
                                      call_stack.top().release_imul())
                                      ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
                user->get_subast(), ::pltxt2htm::NodeType::pl_user, is_not_same_tag, 0,
                ::fast_io::mnp::os_c_str(user->get_id())));
            ++current_index;
            if (is_not_same_tag) {
                auto open_tag1 =
                    ::fast_io::array{u8'<', u8's',  u8'p', u8'a', u8'n', u8' ', u8'c', u8'l',  u8'a', u8's', u8's',
                                     u8'=', u8'\'', u8'R', u8'U', u8's', u8'e', u8'r', u8'\'', u8' ', u8'd', u8'a',
                                     u8't', u8'a',  u8'-', u8'u', u8's', u8'e', u8'r', u8'=',  u8'\''};
                result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
                result.append(user->get_id());
                auto open_tag2 = ::fast_io::array{u8'\'', u8'>'};
                result.append(::fast_io::u8string_view{open_tag2.data(), open_tag2.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            auto size = reinterpret_cast<::pltxt2htm::Size const*>(node.release_imul());
            if (size->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            {
                auto&& subast = size->get_subast();
                if (subast.size() == 1) {
                    // <size=12><size=3>physicsLab</size></size> can be
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_size) {
                        size = reinterpret_cast<::pltxt2htm::Size const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the size is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                nested_tag_type != ::pltxt2htm::NodeType::pl_size ||
                size->get_id() != reinterpret_cast<::pltxt2htm::details::BackendPlSizeTagContext const*>(
                                      call_stack.top().release_imul())
                                      ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendPlSizeTagContext>(
                size->get_subast(), ::pltxt2htm::NodeType::pl_size, is_not_same_tag, 0, size->get_id()));
            ++current_index;
            if (is_not_same_tag) {
                auto open_tag1 = ::fast_io::array{u8'<', u8's', u8'p', u8'a', u8'n',  u8' ', u8's', u8't',
                                                  u8'y', u8'l', u8'e', u8'=', u8'\"', u8'f', u8'o', u8'n',
                                                  u8't', u8'-', u8's', u8'i', u8'z',  u8'e', u8':'};
                result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
                result.append(::pltxt2htm::details::size_t2str(size->get_id() / 2));
                auto open_tag2 = ::fast_io::array{u8'p', u8'x', u8'\"', u8'>'};
                result.append(::fast_io::u8string_view{open_tag2.data(), open_tag2.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_b: {
            auto b = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (b->get_subast().empty()) {
                // e.g. <b></b>
                // Which can be optimized(ignored)
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_b;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                b->get_subast(), ::pltxt2htm::NodeType::pl_b, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'b', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_i: {
            auto i = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (i->get_subast().empty()) {
                // e.g. <i></i>
                // Which can be optimized(ignored)
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::pl_i};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                i->get_subast(), ::pltxt2htm::NodeType::pl_i, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'i', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto p = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (p->get_subast().empty()) {
                // e.g. <p></p>
                // Which can be optimized(ignored)
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_p};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                p->get_subast(), ::pltxt2htm::NodeType::html_p, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'p', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            auto start_tag = ::fast_io::array{u8'<', u8'b', u8'r', u8'>'};
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            break;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h1 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h1};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h1->get_subast(), ::pltxt2htm::NodeType::html_h1, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'1', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto h2 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h2 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h2};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h2->get_subast(), ::pltxt2htm::NodeType::html_h2, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'2', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto h3 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h3 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h3};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h3->get_subast(), ::pltxt2htm::NodeType::html_h3, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'3', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto h4 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h4 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h4};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h4->get_subast(), ::pltxt2htm::NodeType::html_h4, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'4', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto h5 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h5 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h5};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h5->get_subast(), ::pltxt2htm::NodeType::html_h5, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'5', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto h6 = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // NOTE: Empty heading tag can NOT be ignored
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_h6 &&
                                       nested_tag_type != ::pltxt2htm::NodeType::md_atx_h6};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                h6->get_subast(), ::pltxt2htm::NodeType::html_h6, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'h', u8'6', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_del: {
            auto del = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (del->get_subast().empty()) {
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_del};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                del->get_subast(), ::pltxt2htm::NodeType::html_del, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'd', u8'e', u8'l', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_em: {
            auto em = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (em->get_subast().empty()) {
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::html_em};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                em->get_subast(), ::pltxt2htm::NodeType::html_em, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::html_hr: {
            auto start_tag = ::fast_io::array{u8'<', u8'h', u8'r', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            break;
        }
        case ::pltxt2htm::NodeType::html_note: {
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_backslash: {
            result.push_back(u8'\\');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_exclamation: {
            result.push_back(u8'!');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_hash: {
            result.push_back(u8'#');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_dollar: {
            result.push_back(u8'$');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_percent: {
            result.push_back(u8'%');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_paren: {
            result.push_back(u8'(');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_paren: {
            result.push_back(u8')');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_asterisk: {
            result.push_back(u8'*');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_plus: {
            result.push_back(u8'+');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_comma: {
            result.push_back(u8',');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_hyphen: {
            result.push_back(u8'-');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_dot: {
            result.push_back(u8'.');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_slash: {
            result.push_back(u8'/');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_colon: {
            result.push_back(u8':');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_semicolon: {
            result.push_back(u8';');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_equals: {
            result.push_back(u8'=');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_question: {
            result.push_back(u8'?');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_at: {
            result.push_back(u8'@');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_bracket: {
            result.push_back(u8'[');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_bracket: {
            result.push_back(u8']');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_caret: {
            result.push_back(u8'^');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_underscore: {
            result.push_back(u8'_');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_backtick: {
            result.push_back(u8'`');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_brace: {
            result.push_back(u8'{');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_pipe: {
            result.push_back(u8'|');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_brace: {
            result.push_back(u8'}');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_tilde: {
            result.push_back(u8'~');
            break;
        }
        case ::pltxt2htm::NodeType::base:
#if 0
     [[unlikely]] [[fallthrough]];
     default:
#endif
            [[unlikely]] {
                ::exception::unreachable<ndebug>();
            }
        }
    }

    {
        bool is_not_same_tag{call_stack.top()->is_not_same_tag_};
        ::pltxt2htm::NodeType const nested_tag_type{call_stack.top()->nested_tag_type_};
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        } else {
            switch (nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'a', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'a', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_b: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'b', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_i: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'i', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_p: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'p', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'1', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'2', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'3', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'4', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'5', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'6', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_del: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'd', u8'e', u8'l', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::html_em: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'e', u8'm', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::base:
                [[fallthrough]];
            default:
                [[unlikely]] ::exception::unreachable<ndebug>();
            }
        }
    }
}

} // namespace pltxt2htm::details
