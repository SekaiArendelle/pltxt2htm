#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm::details {

constexpr void append_pltext_escaped(::fast_io::u8string& result, ::fast_io::u8string_view value) noexcept {
    for (auto const chr : value) {
        switch (chr) {
        case u8'&':
            result.append(u8"&amp;");
            break;
        case u8'\"':
            result.append(u8"&quot;");
            break;
        case u8'\'':
            result.append(u8"&apos;");
            break;
        case u8'<':
            result.append(u8"&lt;");
            break;
        case u8'>':
            result.append(u8"&gt;");
            break;
        default:
            result.push_back(chr);
            break;
        }
    }
}

} // namespace pltxt2htm::details
