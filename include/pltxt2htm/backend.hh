#pragma once

#include <cstdint>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "astnode/node_type.hh"
#include "astnode/basic.hh"
#include "astnode/plext.hh"
#include "heap_guard.hh"
#include "pltxt2htm/astnode/node_type.hh"

namespace pltxt2htm {

enum class BackendText : ::std::uint_least32_t {
    advanced_html = 0,
    common_html,
};

namespace details {

/**
 * @brief Integrate ast nodes to HTML.
 * @tparam ndebug: true  -> release mode, disables most of the checks which is unsafe but fast
 *                 false -> debug mode, enables most of the checks
 * @tparam disable_log: true -> no any info printed before crashing
 * @tparam T: you should not pass this param because the compiler can infer it automatically.
 * @note tparam T should not be marked const, that's the reason why I use `remove_reference_t`
 * @param [in] ast: Ast of Quantum-Physics's text
 * @param [in] extern_node_type: The type of the parent node (extern_node).
                                 If extern_node_type is ::pltxt2htm::NodeType::base,
                                 the current node is the root node (no parent node).
 * @param [in] extern_node: The pointer of parent node
 */
template<BackendText backend_text, bool ndebug, bool disable_log>
[[nodiscard]]
constexpr auto ast2html(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast,
                        ::fast_io::u8string_view host,
                        ::pltxt2htm::NodeType const extern_node_type = ::pltxt2htm::NodeType::base,
                        ::pltxt2htm::PlTxtNode const* const extern_node = nullptr) noexcept(disable_log == true)
    -> ::fast_io::u8string {
    if constexpr (ndebug == false || disable_log == false) {
        // Checkout whether extern_node_type is a valid type info for ptr extern_node
        if ((extern_node_type != ::pltxt2htm::NodeType::base && extern_node == nullptr) ||
            (extern_node_type == ::pltxt2htm::NodeType::base && extern_node != nullptr)) [[unlikely]] {
            if constexpr (disable_log == false) {
                // panic_print
            }
            if constexpr (ndebug == false) {
                ::exception::terminate();
            }
        }
    }
    ::fast_io::u8string result{};

    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
            break;
        }
        case ::pltxt2htm::NodeType::ampersand: {
            result.append(u8"&amp;");
            break;
        }
        case ::pltxt2htm::NodeType::single_quote: {
            result.append(u8"&apos;");
            break;
        }
        case ::pltxt2htm::NodeType::double_quote: {
            result.append(u8"&quot;");
            break;
        }
        case ::pltxt2htm::NodeType::less_than: {
            result.append(u8"&lt;");
            break;
        }
        case ::pltxt2htm::NodeType::greater_than: {
            result.append(u8"&gt;");
            break;
        }
        case ::pltxt2htm::NodeType::color: {
            auto color = reinterpret_cast<::pltxt2htm::Color const*>(node.release_imul());
            // TODO: <color=red><color=blue>text</color></color> can be optimized
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                extern_node_type != ::pltxt2htm::NodeType::color ||
                color->get_color<ndebug>() !=
                    reinterpret_cast<::pltxt2htm::Color const*>(extern_node)->get_color<ndebug>();
            if (is_not_same_tag) {
                result.append(u8"<span style=\"color:");
                result.append(color->get_color<ndebug>());
                result.append(u8";\">");
            }
            result.append(::pltxt2htm::details::ast2html<backend_text, ndebug, disable_log>(
                color->get_subast<ndebug>(), host, ::pltxt2htm::NodeType::color, color));
            if (is_not_same_tag) {
                result.append(u8"</span>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::experiment: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::discussion: {
            auto discussion = reinterpret_cast<::pltxt2htm::Discussion const*>(node.release_imul());
            // Optimization: If the discussion is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                extern_node_type != ::pltxt2htm::NodeType::discussion ||
                discussion->get_id<ndebug>() !=
                    reinterpret_cast<::pltxt2htm::Discussion const*>(extern_node)->get_id<ndebug>();
            // TODO <Discussion=123><discussion=642cf37a494746375aae306a>physicsLab</discussion></Discussion> can be
            // optimized as <a href=\"localhost:5173/ExperimentSummary/Discussion/642cf37a494746375aae306a\"
            // internal>physicsLab</a>
            if (is_not_same_tag) {
                result.append(u8"<a href=\"");
                result.append(host);
                result.append(u8"/ExperimentSummary/Discussion/");
                result.append(discussion->get_id<ndebug>());
                result.append(u8"\" internal>");
            }
            result.append(::pltxt2htm::details::ast2html<backend_text, ndebug, disable_log>(
                discussion->get_subast<ndebug>(), host, ::pltxt2htm::NodeType::discussion, discussion));
            if (is_not_same_tag) {
                result.append(u8"</a>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::user: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::size: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::b: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::i: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::p: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::br: {
            if constexpr (backend_text == ::pltxt2htm::BackendText::advanced_html) {
                result.append(u8"<br>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::h: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::img: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::latex: {
            //
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

    return result;
}

} // namespace details

/**
 * @brief Integrate ast nodes to HTML.
 * @tparam ndebug: true  -> release mode, disables most of the checks which is unsafe but fast
 *                 false -> debug mode, enables most of the checks
 * @tparam disable_log: true -> no any info printed before crashing
 * @tparam T: you should not pass this param because the compiler can infer it automatically.
 * @note tparam T should not be marked const, that's the reason why I use `remove_reference_t`
 * @tparam backend_text:
 * @param [in] ast: Ast of Quantum-Physics's text
 * @param [in] host:
 * @return
 */
template<BackendText backend_text, bool ndebug, bool disable_log>
[[nodiscard]]
constexpr auto ast2html(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast,
                        ::fast_io::u8string_view host) noexcept(disable_log == true) -> ::fast_io::u8string {
    return ::pltxt2htm::details::ast2html<backend_text, ndebug, disable_log>(ast, host);
}

} // namespace pltxt2htm
