/**
 * @file style.hh
 * @brief Presentation values for semantic code-highlighting roles.
 */

#pragma once

#include "../../../ast/code/node.hh"
#include "../../../container/string_view.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto code_style_color(::pltxt2htm::CodeHighlightKind const kind) noexcept
    -> ::pltxt2htm::container::U8StringView {
    switch (kind) /* -Werror=switch */ {
    case ::pltxt2htm::CodeHighlightKind::plain: {
        return u8"";
    }
    case ::pltxt2htm::CodeHighlightKind::keyword: {
        return u8"#cf222e";
    }
    case ::pltxt2htm::CodeHighlightKind::string: {
        return u8"#0a3069";
    }
    case ::pltxt2htm::CodeHighlightKind::number: {
        return u8"#0550ae";
    }
    case ::pltxt2htm::CodeHighlightKind::comment: {
        return u8"#6e7781";
    }
    case ::pltxt2htm::CodeHighlightKind::function: {
        return u8"#8250df";
    }
    case ::pltxt2htm::CodeHighlightKind::macro: {
        return u8"#cf222e";
    }
    case ::pltxt2htm::CodeHighlightKind::preprocessor: {
        return u8"#0550ae";
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected code highlight kind");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable code after exhaustive switch on code highlight kind");
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
