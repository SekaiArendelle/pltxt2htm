/**
 * @file for_plunity_text.hh
 * @brief Advanced HTML backend for pltxt2htm
 * @details Generates full-featured HTML output with comprehensive support for
 *          Physics-Lab tags, Markdown syntax, and HTML elements.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
#include "../call_stack.hh"
#include <fast_io/fast_io_dsal/vector.h>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
#include "frame_context.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Append an entity reference to Unity Rich Text output.
 * @details Numeric character references (e.g. `&#38;`, `&#x26;`, `&#X2A;`) are decoded to
 *          the character they denote. `<` and `>` are emitted in their escaped full-width
 *          form (`<size=20>\uff1c</size>` / `<size=20>\uff1e</size>`) so that Unity's
 *          TextMeshPro does not interpret them as rich text tag delimiters. Any other
 *          (named) reference is emitted verbatim as `&` + value + `;`.
 * @tparam ndebug Contract checking mode.
 * @param value Entity content between `&` and `;` (e.g. `amp`, `#38`, `#x26`).
 * @param[out] out Output buffer receiving the encoded output.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_entity_reference_to_plunity_richtext(::pltxt2htm::container::U8String const& value,
                                                           ::pltxt2htm::container::U8String& out) noexcept {
    ::pltxt2htm::container::U8StringView const value_view{value};
    ::std::size_t const value_size{value_view.size()};
    bool decoded{};
    if (value_size > 1 && value_view.template index<ndebug>(0) == u8'#') {
        auto index = ::std::size_t{1};
        bool const hex{value_view.template index<ndebug>(index) == u8'x' ||
                       value_view.template index<ndebug>(index) == u8'X'};
        if (hex) {
            ++index;
        }
        auto const digit_begin{index};
        char32_t const base{hex ? char32_t{16} : char32_t{10}};
        char32_t code{};
        bool valid{true};
        for (; index < value_size; ++index) {
            auto const chr = value_view.template index<ndebug>(index);
            char32_t digit{};
            if (::pltxt2htm::details::is_ascii_digit(chr)) {
                digit = static_cast<char32_t>(chr - u8'0');
            }
            else if (hex && u8'a' <= chr && chr <= u8'f') {
                digit = static_cast<char32_t>(chr - u8'a') + 10;
            }
            else if (hex && u8'A' <= chr && chr <= u8'F') {
                digit = static_cast<char32_t>(chr - u8'A') + 10;
            }
            else {
                valid = false;
                break;
            }
            if (code > (char32_t{0x10FFFF} - digit) / base) {
                valid = false;
                break;
            }
            code = code * base + digit;
        }
        if (valid && index > digit_begin && code <= char32_t{0x10FFFF} &&
            (code < char32_t{0xD800} || code > char32_t{0xDFFF})) {
            if (code == char32_t{0x3C}) {
                out.append(u8"<size=20>\uff1c</size>");
            }
            else if (code == char32_t{0x3E}) {
                out.append(u8"<size=20>\uff1e</size>");
            }
            else if (code < char32_t{0x80}) {
                out.push_back(static_cast<char8_t>(code));
            }
            else if (code < char32_t{0x800}) {
                out.push_back(static_cast<char8_t>(0xC0 | (code >> 6)));
                out.push_back(static_cast<char8_t>(0x80 | (code & 0x3F)));
            }
            else if (code < char32_t{0x10000}) {
                out.push_back(static_cast<char8_t>(0xE0 | (code >> 12)));
                out.push_back(static_cast<char8_t>(0x80 | ((code >> 6) & 0x3F)));
                out.push_back(static_cast<char8_t>(0x80 | (code & 0x3F)));
            }
            else {
                out.push_back(static_cast<char8_t>(0xF0 | (code >> 18)));
                out.push_back(static_cast<char8_t>(0x80 | ((code >> 12) & 0x3F)));
                out.push_back(static_cast<char8_t>(0x80 | ((code >> 6) & 0x3F)));
                out.push_back(static_cast<char8_t>(0x80 | (code & 0x3F)));
            }
            decoded = true;
        }
    }
    if (decoded == false) {
        out.push_back(u8'&');
        out.append(value_view);
        out.push_back(u8';');
    }
}

/**
 * @brief Convert a simple (leaf-only) AST to Unity Rich Text with unescaping.
 * @tparam ndebug Contract checking mode.
 * @param ast The AST to convert (should only contain leaf/character-like nodes).
 * @param[out] out Output buffer receiving the Unity Rich Text string.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void convert_simple_pltxt_ast_to_plunity_richtext(::pltxt2htm::Ast<ndebug> const& ast,
                                                            ::pltxt2htm::container::U8String& out) noexcept {
    out.template reserve<ndebug>(out.size() + ast.size() * 6);
    for (auto&& node : ast) {
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            auto&& active_node{node.as_u8char()};
            out.push_back(active_node.chr);
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            out.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::space: {
            out.append(u8"\u00A0");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            out.push_back(u8'&');
            continue;
        }
        case ::pltxt2htm::NodeKind::entity_reference: {
            auto&& active_node{node.as_entity_reference()};
            ::pltxt2htm::details::append_entity_reference_to_plunity_richtext<ndebug>(active_node.get_value(), out);
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote: {
            out.push_back(u8'\'');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            out.push_back(u8'\"');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            out.push_back(u8'<');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            out.push_back(u8'>');
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            out.push_back(u8'\t');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            out.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            out.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            out.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            out.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            out.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            out.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            out.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            out.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            out.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            out.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            out.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            out.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            out.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            out.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            out.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            out.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            out.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            out.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            out.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            out.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            out.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            out.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            out.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            out.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            out.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            out.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            out.push_back(u8'~');
            continue;
        }
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in Unity text escaping");
            }
        }
    }
}

/**
 * @brief Convert AST nodes to Unity Rich Text format.
 * @details Generates Unity Rich Text with support for Physics-Lab tags,
 *          Markdown syntax, and HTML-like elements mapped to Unity-compatible tags.
 *          Uses a goto-based state machine to avoid stack overflow.
 * @tparam ndebug Contract checking mode.
 * @param ast_init The AST to convert.
 * @param project Project identifier for Physics-Lab context.
 * @param visitor Visitor identifier for Physics-Lab context.
 * @param author Author identifier for Physics-Lab context.
 * @param coauthors Co-authors identifier for Physics-Lab context.
 * @return A string containing Unity Rich Text output.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto plunity_text_backend(::pltxt2htm::Ast<ndebug> const& ast_init,
                                    ::pltxt2htm::container::U8StringView project,
                                    ::pltxt2htm::container::U8StringView visitor,
                                    ::pltxt2htm::container::U8StringView author,
                                    ::pltxt2htm::container::U8StringView coauthors) noexcept
    -> ::pltxt2htm::container::U8String {
    ::pltxt2htm::container::U8String result{};
    ::pltxt2htm::details::CallStack<BackendFrame<ndebug>> call_stack{};
    call_stack.push_frame(BackendFrame<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));
    ::std::size_t list_nesting_depth{};

entry:
    while (true) {
        auto&& current_frame = call_stack.template current_frame<ndebug>();
        auto const& ast = current_frame.get_ast();
        auto&& current_index = current_frame.current_index;
        ::std::size_t const ast_size{ast.size()};
        for (; current_index < ast_size; ++current_index) {
            auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

            switch (node.get_node_kind()) /* -Werror=switch */ {
            case ::pltxt2htm::NodeKind::u8char: {
                auto&& active_node{node.as_u8char()};
                result.push_back(active_node.chr);
                continue;
            }
            case ::pltxt2htm::NodeKind::invalid_u8char: {
                result.append(u8"\uFFFD");
                continue;
            }
            case ::pltxt2htm::NodeKind::text: {
                auto&& active_node{node.as_text()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::space: {
                result.append(u8"\u00A0");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::ampersand: {
                result.push_back(u8'&');
                continue;
            }
            case ::pltxt2htm::NodeKind::entity_reference: {
                auto&& active_node{node.as_entity_reference()};
                ::pltxt2htm::details::append_entity_reference_to_plunity_richtext<ndebug>(active_node.get_value(),
                                                                                          result);
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_single_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::single_quote: {
                result.push_back(u8'\'');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_double_quote:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::double_quote: {
                result.push_back(u8'\"');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_less_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::less_than: {
                result.append(u8"<size=20>\uff1c</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_greater_than:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::greater_than: {
                result.append(u8"<size=20>\uff1e</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::tab: {
                result.push_back(u8'\t');
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_color: {
                auto&& active_node{node.as_pl_color()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_color, 0));
                ++current_index;
                result.append(u8"<color=");
                result.append(active_node.get_color());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a: {
                auto&& active_node{node.as_pl_a()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag =
                    ::pltxt2htm::details::concat(U8LiteralString{u8"<color="},
                                                 ::pltxt2htm::PlA<ndebug>::get_color_literal(), U8LiteralString{u8">"});
                result.append(::pltxt2htm::container::U8StringView{open_tag});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                auto&& active_node{node.as_pl_experiment()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_experiment, 0));
                ++current_index;
                result.append(u8"<experiment=");
                result.append(active_node.get_id());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                auto&& active_node{node.as_pl_discussion()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_discussion, 0));
                ++current_index;
                result.append(u8"<discussion=");
                result.append(active_node.get_id());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                auto&& active_node{node.as_pl_experiments()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_experiments, 0));
                ++current_index;
                result.append(u8"<experiments=");
                result.append(active_node.get_value());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                auto&& active_node{node.as_pl_discussions()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_discussions, 0));
                ++current_index;
                result.append(u8"<discussions=");
                result.append(active_node.get_value());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                auto&& active_node{node.as_pl_external()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(active_node.get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                auto&& active_node{node.as_pl_link()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_link, 0));
                ++current_index;
                result.append(u8"<link=\"");
                result.append(active_node.get_url().as_string());
                result.append(u8"\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                auto&& active_node{node.as_pl_user()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_user, 0));
                ++current_index;
                result.append(u8"<user=");
                result.append(active_node.get_id());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                auto&& active_node{node.as_pl_trigger()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_trigger, 0));
                ++current_index;
                result.append(u8"<trigger=");
                result.append(active_node.get_value());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                auto&& active_node{node.as_pl_internal()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_internal, 0));
                ++current_index;
                result.append(u8"<internal=");
                result.append(active_node.get_value());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                auto&& active_node{node.as_pl_size()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_size, 0));
                ++current_index;
                result.append(u8"<size=");
                auto const& pl_size = active_node.get_font_size();
                result.append(::pltxt2htm::details::double2str(pl_size.value));
                switch (pl_size.unit) /* -Werror=switch */ {
                case ::pltxt2htm::Unit::percent: {
                    result.push_back(u8'%');
                    break;
                }
                case ::pltxt2htm::Unit::em: {
                    result.append(u8"em");
                    break;
                }
                case ::pltxt2htm::Unit::px: {
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for size");
                    }
#endif
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                auto&& active_node{node.as_pl_voffset()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_voffset, 0));
                ++current_index;
                auto const voffset = active_node.get_value();
                result.append(u8"<voffset=");
                result.append(::pltxt2htm::details::ptrdiff_t2str(voffset.value));
                switch (voffset.unit) /* -Werror=switch */ {
                case ::pltxt2htm::Unit::em: {
                    result.append(u8"em");
                    break;
                }
                case ::pltxt2htm::Unit::px: {
                    result.append(u8"px");
                    break;
                }
                case ::pltxt2htm::Unit::percent:
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    [[fallthrough]];
                default:
#endif
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for voffset");
                    }
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                auto&& active_node{node.as_pl_align()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_align, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                auto const align = active_node.get_align();
                switch (align) /* -Werror=switch */ {
                case ::pltxt2htm::TextAlign::left: {
                    result.append(u8"<align=left>");
                    break;
                }
                case ::pltxt2htm::TextAlign::center: {
                    result.append(u8"<align=center>");
                    break;
                }
                case ::pltxt2htm::TextAlign::right: {
                    result.append(u8"<align=right>");
                    break;
                }
                case ::pltxt2htm::TextAlign::justify: {
                    result.append(u8"<align=justified>");
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for align");
                    }
#endif
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                auto&& active_node{node.as_pl_margin()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_margin, 0));
                ++current_index;
                auto const margin_left = active_node.get_left();
                auto const margin_right = active_node.get_right();
                result.append(u8"<margin");
                if (margin_left.has_value()) {
                    result.append(u8" left=");
                    auto const& margin_value = margin_left.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for margin");
                        }
#endif
                    }
                }
                if (margin_right.has_value()) {
                    result.append(u8" right=");
                    auto const& margin_value = margin_right.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for margin");
                        }
#endif
                    }
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                auto&& active_node{node.as_html_div()};
                // An HTML <div style="margin-left:...;margin-right:..."> maps back to the Unity TMP margin tag.
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_div, 0));
                ++current_index;
                auto const margin_left = active_node.get_left();
                auto const margin_right = active_node.get_right();
                result.append(u8"<margin");
                if (margin_left.has_value()) {
                    result.append(u8" left=");
                    auto const& margin_value = margin_left.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for div");
                        }
#endif
                    }
                }
                if (margin_right.has_value()) {
                    result.append(u8" right=");
                    auto const& margin_value = margin_right.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for div");
                        }
#endif
                    }
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto&& active_node{node.as_html_span()};
                auto const& span_color = active_node.get_color();
                auto const& span_font_size = active_node.get_font_size();
                auto const& span_vertical_align = active_node.get_vertical_align();
                bool const has_color = !span_color.empty();
                bool const has_font_size = span_font_size.has_value();
                // Only px lengths map to <voffset>; keywords/percent have no TMP_Text equivalent.
                bool const has_vertical_align = [&] {
                    if (span_vertical_align.has_value() == false) {
                        return false;
                    }
                    auto const& val = span_vertical_align.template value<ndebug>();
                    return val.get_kind() == ::pltxt2htm::VerticalAlignKind::length &&
                           val.get_length().unit == ::pltxt2htm::Unit::px;
                }();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), 0,
                                         BackendContextWithHtmlSpanInfo{.has_color = has_color,
                                                                        .has_font_size = has_font_size,
                                                                        .has_vertical_align = has_vertical_align}));
                ++current_index;
                if (has_color) {
                    result.append(u8"<color=");
                    result.append(span_color);
                    result.push_back(u8'>');
                }
                if (has_font_size) {
                    auto const& font_size = span_font_size.template value<ndebug>();
                    result.append(u8"<size=");
                    switch (font_size.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.append(::pltxt2htm::details::double2str(font_size.value));
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(::pltxt2htm::details::double2str(font_size.value));
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        // plunity <size> counts two scale units per CSS px; match the /2 applied
                        // by the plunity->plweb mapping (e.g. 12.5px -> <size=25>, size=11 -> 6px).
                        result.append(::pltxt2htm::details::double2str(font_size.value * 2));
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for font size");
                        }
#endif
                    }
                    result.push_back(u8'>');
                }
                if (has_vertical_align) {
                    result.append(u8"<voffset=");
                    result.append(::pltxt2htm::details::ptrdiff_t2str(
                        span_vertical_align.template value<ndebug>().get_length().value));
                    result.push_back(u8'>');
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                auto&& active_node{node.as_html_a()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_a, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(active_node.get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                auto&& active_node{node.as_md_double_emphasis_underscore()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_double_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                auto&& active_node{node.as_md_double_emphasis_asterisk()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_b: {
                auto&& active_node{node.as_pl_b()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_b, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                auto&& active_node{node.as_html_strong()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_strong, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                auto&& active_node{node.as_html_mark()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_mark, 0));
                ++current_index;
                auto const& mark_background_color = active_node.get_background_color();
                result.append(u8"<mark=");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::pltxt2htm::container::U8String purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color, ::pltxt2htm::container::U8StringView{mark_background_color});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in Unity "
                                     u8"rich-text tags.");
                }
                result.append(mark_background_color);
                result.append(u8">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                auto&& active_node{node.as_pl_mark()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_mark, 0));
                ++current_index;
                auto const& mark_background_color = active_node.get_background_color();
                result.append(u8"<mark=");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::pltxt2htm::container::U8String purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color, ::pltxt2htm::container::U8StringView{mark_background_color});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in Unity "
                                     u8"rich-text tags.");
                }
                result.append(mark_background_color);
                result.append(u8">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto&& active_node{node.as_html_p()};
                auto const align = active_node.get_align();
                bool const has_align = align != ::pltxt2htm::TextAlign::left;
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_p, 0,
                                                           BackendContextWithAlignInfo{.has_align = has_align}));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                if (has_align) {
                    result.append(u8"<align=");
                    if (align == ::pltxt2htm::TextAlign::center) {
                        result.append(u8"center");
                    }
                    else if (align == ::pltxt2htm::TextAlign::right) {
                        result.append(u8"right");
                    }
                    else if (align == ::pltxt2htm::TextAlign::justify) {
                        result.append(u8"justified");
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected paragraph text alignment");
                    }
                    result.push_back(u8'>');
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_br:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::line_break: {
                result.push_back(u8'\n');
                continue;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                auto&& active_node{node.as_html_h1()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h1, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=38><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                auto&& active_node{node.as_md_atx_h1()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h1, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=38><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                auto&& active_node{node.as_html_h2()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h2, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=37><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                auto&& active_node{node.as_md_atx_h2()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h2, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=37><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                auto&& active_node{node.as_html_h3()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h3, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=36><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                auto&& active_node{node.as_md_atx_h3()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h3, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=36><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                auto&& active_node{node.as_html_h4()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h4, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=35><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                auto&& active_node{node.as_md_atx_h4()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h4, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=35><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                auto&& active_node{node.as_html_h5()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h5, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                auto&& active_node{node.as_md_atx_h5()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h5, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                auto&& active_node{node.as_html_h6()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h6, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                auto&& active_node{node.as_md_atx_h6()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h6, 0));
                ++current_index;
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                auto&& active_node{node.as_md_del()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_del, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                auto&& active_node{node.as_html_del()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                auto&& active_node{node.as_html_code()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_code, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_s: {
                auto&& active_node{node.as_pl_s()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_s, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                auto&& active_node{node.as_html_sup()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_sup, 0));
                ++current_index;
                result.append(u8"<sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                auto&& active_node{node.as_html_sub()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_sub, 0));
                ++current_index;
                result.append(u8"<sub>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                auto&& active_node{node.as_pl_u()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_u, 0));
                ++current_index;
                result.append(u8"<u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                auto&& active_node{node.as_md_single_emphasis_underscore()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_single_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                auto&& active_node{node.as_md_single_emphasis_asterisk()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                auto&& active_node{node.as_pl_i()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_i, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                auto&& active_node{node.as_html_em()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_em, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr: {
                if (result.empty() == false && result.template back<ndebug>() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"---\n");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_note: {
                continue;
            }
            case ::pltxt2htm::NodeKind::list_ul: {
                auto&& active_node{node.as_list_ul()};
                auto const& list_ul = active_node;
                pltxt2htm_assert(list_ul.get_subast().empty() == false, u8"List container must not be empty");
                auto const parent_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push_frame(BackendFrame<ndebug>(list_ul.get_subast(), ::pltxt2htm::NodeKind::list_ul, 0));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                auto&& active_node{node.as_list_ol()};
                auto const& list_ol = active_node;
                pltxt2htm_assert(list_ol.get_subast().empty() == false, u8"List container must not be empty");
                auto const parent_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push_frame(
                    BackendFrame<ndebug>(list_ol.get_subast(), ::pltxt2htm::NodeKind::list_ol, 0,
                                         BackendContextWithOlInfo{.ol_li_count = list_ol.get_start()}));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                auto&& active_node{node.as_list_li()};
                auto&& list_frame = call_stack.template current_frame<ndebug>();
                auto const list_tag_type = list_frame.get_nested_tag_type();
                pltxt2htm_assert(
                    list_tag_type == ::pltxt2htm::NodeKind::list_ol || list_tag_type == ::pltxt2htm::NodeKind::list_ul,
                    u8"Invalid tag type");
                auto const indent_level = list_nesting_depth;
                for (::std::size_t i = 1; i < indent_level; ++i) {
                    result.append(u8"  ");
                }
                if (list_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    ::std::size_t& ol_li_count = list_frame.as_ol_info().ol_li_count;
                    result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                    result.append(u8". ");
                    ++ol_li_count;
                }
                else if (list_tag_type == ::pltxt2htm::NodeKind::list_ul) {
                    if (indent_level % 3 == 1) {
                        result.append(u8"\u2022 ");
                    }
                    else if (indent_level % 3 == 2) {
                        result.append(u8"\u2218 ");
                    }
                    else if (indent_level % 3 == 0) {
                        result.append(u8"\u25ab ");
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected indent level remainder");
                    }
                }
                else [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type for list item");
                }
                ++current_index;
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::list_li, 0));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                auto&& active_node{node.as_list_li_checkbox()};
                auto&& list_frame = call_stack.template current_frame<ndebug>();
                auto const list_tag_type = list_frame.get_nested_tag_type();
                auto const indent_level = list_nesting_depth;
                for (::std::size_t i = 1; i < indent_level; ++i) {
                    result.append(u8"  ");
                }
                if (list_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    ::std::size_t& ol_li_count = list_frame.as_ol_info().ol_li_count;
                    result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                    result.append(u8". ");
                    ++ol_li_count;
                    if (active_node.is_checked()) {
                        result.append(u8"\u2611 ");
                    }
                    else {
                        result.append(u8"\u2610 ");
                    }
                }
                else if (list_tag_type == ::pltxt2htm::NodeKind::list_ul) {
                    if (indent_level % 3 == 1) {
                        result.append(u8"\u2022 ");
                    }
                    else if (indent_level % 3 == 2) {
                        result.append(u8"\u2218 ");
                    }
                    else if (indent_level % 3 == 0) {
                        result.append(u8"\u25ab ");
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected indent level remainder");
                    }
                    if (active_node.is_checked()) {
                        result.append(u8"\u2611 ");
                    }
                    else {
                        result.append(u8"\u2610 ");
                    }
                }
                else [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type for checkbox list item");
                }
                ++current_index;
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::list_li_checkbox, 0));
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                auto&& active_node{node.as_md_code_span_1_backtick()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                auto&& active_node{node.as_md_code_span_2_backtick()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_2_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                auto&& active_node{node.as_md_code_span_3_backtick()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_3_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-SarasaMonoSC SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                auto&& active_node{node.as_md_latex_inline()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_latex_inline, 0));
                ++current_index;
                result.push_back(u8'$');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                auto&& active_node{node.as_md_latex_block()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_latex_block, 0));
                ++current_index;
                result.append(u8"$$");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                auto&& active_node{node.as_md_block_quotes()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_block_quotes, 0));
                ++current_index;
                result.append(u8"<margin left=2em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                auto&& active_node{node.as_html_blockquote()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_blockquote, 0));
                ++current_index;
                result.append(u8"<margin left=2em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                auto&& active_node{node.as_table()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>table<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                auto&& active_node{node.as_table_tr()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tr, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tr<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto&& active_node{node.as_table_td()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_td, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>td");
                auto const align = active_node.get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                auto&& active_node{node.as_table_th()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_th, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>th");
                auto const align = active_node.get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                auto&& active_node{node.as_table_thead()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_thead, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>thead<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                auto&& active_node{node.as_table_tbody()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tbody, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tbody<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                auto&& active_node{node.as_table_tfoot()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tfoot, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tfoot<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                auto&& active_node{node.as_table_caption()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_caption, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>caption<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                auto&& active_node{node.as_table_colgroup()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_colgroup, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>colgroup<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_col: {
                result.append(u8"<size=20>\uff1c</size>col<size=20>\uff1e</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_img: {
                auto&& active_node{node.as_html_img()};
                result.append(u8"<size=20>\uff1c</size>img src=\"");
                result.append(active_node.get_src());
                result.append(u8"\" alt=\"");
                result.append(active_node.get_alt());
                result.append(u8"\"<size=20>\uff1e</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                auto&& active_node{node.as_md_triple_emphasis_underscore()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_triple_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<b><i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                auto&& active_node{node.as_md_triple_emphasis_asterisk()};
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<b><i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::url: {
                auto&& active_node{node.as_url()};
                auto const& url_str = active_node.as_string();
                result.append(u8"<external=");
                result.append(url_str);
                result.push_back(u8'>');
                result.append(url_str);
                result.append(u8"</external>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                auto&& active_node{node.as_md_link()};
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(active_node.get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                auto&& active_node{node.as_md_image()};
                result.append(u8"![");
                ::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(active_node.get_subast(),
                                                                                           result);
                result.append(u8"](");
                result.append(active_node.get_url().as_string());
                result.push_back(u8')');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_backslash: {
                result.push_back(u8'\\');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_exclamation: {
                result.push_back(u8'!');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_hash: {
                result.push_back(u8'#');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_dollar: {
                result.push_back(u8'$');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_percent: {
                result.push_back(u8'%');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_left_paren: {
                result.push_back(u8'(');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_right_paren: {
                result.push_back(u8')');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_asterisk: {
                result.push_back(u8'*');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_plus: {
                result.push_back(u8'+');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_comma: {
                result.push_back(u8',');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_hyphen: {
                result.push_back(u8'-');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_dot: {
                result.push_back(u8'.');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_slash: {
                result.push_back(u8'/');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_colon: {
                result.push_back(u8':');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_semicolon: {
                result.push_back(u8';');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_equals: {
                result.push_back(u8'=');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_question: {
                result.push_back(u8'?');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_at: {
                result.push_back(u8'@');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
                result.push_back(u8'[');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
                result.push_back(u8']');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_caret: {
                result.push_back(u8'^');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_underscore: {
                result.push_back(u8'_');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_backtick: {
                result.push_back(u8'`');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_left_brace: {
                result.push_back(u8'{');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_pipe: {
                result.push_back(u8'|');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_right_brace: {
                result.push_back(u8'}');
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_tilde: {
                result.push_back(u8'~');
                continue;
            }
            case ::pltxt2htm::NodeKind::code_fence: {
                auto&& active_node{node.as_code_fence()};
                result.append(u8"<font=\"PhysicsLab-SarasaMonoSC SDF\">\n");
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::code_fence, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_macro_project: {
                result.append(project);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_visitor: {
                result.append(visitor);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_author: {
                result.append(author);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
                result.append(coauthors);
                continue;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected macro node kind in Unity backend");
                }
#endif
            }
        }

        {
            bool const has_parent{call_stack.has_parent()};
            auto const top_frame = call_stack.template pop_frame<ndebug>();
            if (!has_parent) {
                return result;
            }
            switch (top_frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeKind::text: {
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_color: {
                result.append(u8"</color>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto const& span_info = top_frame.as_html_span_info();
                if (span_info.has_vertical_align) {
                    result.append(u8"</voffset>");
                }
                if (span_info.has_font_size) {
                    result.append(u8"</size>");
                }
                if (span_info.has_color) {
                    result.append(u8"</color>");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                result.append(u8"</experiment>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                result.append(u8"</discussion>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                result.append(u8"</experiments>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                result.append(u8"</discussions>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                result.append(u8"</external>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                result.append(u8"</link>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                result.append(u8"</user>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                result.append(u8"</trigger>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                result.append(u8"</internal>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                result.append(u8"</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                result.append(u8"</voffset>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                result.append(u8"</align>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                result.append(u8"</margin>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                result.append(u8"</margin>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_strong: {
                result.append(u8"</b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                result.append(u8"</mark>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                result.append(u8"</mark>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_em: {
                result.append(u8"</i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                result.append(u8"</i></b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                if (top_frame.as_align_info().has_align) {
                    result.append(u8"</align>");
                }
                auto const& parent_frame = call_stack.template current_frame<ndebug>();
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.template back<ndebug>() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h4:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h4: {
                result.append(u8"</b></size>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h6:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h6: {
                result.append(u8"</b>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_s: {
                result.append(u8"</s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                result.append(u8" </font>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                result.append(u8"</sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                result.append(u8"</sub>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                result.append(u8"</u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_ol: {
                --list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_li: {
                result.push_back(u8'\n');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                result.append(u8" </font>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                result.push_back(u8'$');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                result.append(u8"$$");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                result.append(u8"</margin>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                result.append(u8"<size=20>\uff1c</size>/table<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                result.append(u8"<size=20>\uff1c</size>/tr<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                result.append(u8"<size=20>\uff1c</size>/td<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                result.append(u8"<size=20>\uff1c</size>/th<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                result.append(u8"<size=20>\uff1c</size>/thead<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                result.append(u8"<size=20>\uff1c</size>/tbody<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                result.append(u8"<size=20>\uff1c</size>/tfoot<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                result.append(u8"<size=20>\uff1c</size>/caption<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                result.append(u8"<size=20>\uff1c</size>/colgroup<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                result.append(u8"</margin>\n");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::code_fence: {
                result.append(u8"\n</font>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::url:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_a: {
                result.append(u8"</external>");
                goto entry;
            }
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type in Unity text backend");
                }
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
