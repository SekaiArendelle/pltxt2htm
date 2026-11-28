#pragma once

#include <charconv>
#include <concepts>
#include <cstddef>
#include <limits>
#include <string>
#include <string_view>

#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/string_view.hh>

namespace pltxt2htm_test::format_details {

/**
 * @brief Convert an integer through the standard library for use as an independent oracle.
 */
template<::std::integral Integer>
[[nodiscard]] auto oracle_int2str(Integer value) -> ::pltxt2htm::container::U8String {
    // digits10 + 1 covers the largest magnitude; one more character covers a sign.
    char buffer[::std::numeric_limits<Integer>::digits10 + 3];
    auto const conversion = ::std::to_chars(buffer, buffer + sizeof(buffer), value);

    ::pltxt2htm::container::U8String result{};
    for (auto pointer = buffer; pointer != conversion.ptr; ++pointer) {
        result.push_back(static_cast<char8_t>(*pointer));
    }
    return result;
}

[[nodiscard]] inline auto to_std_string(::pltxt2htm::container::U8String const& text) -> ::std::string {
    return ::std::string{reinterpret_cast<char const*>(text.data()), text.size()};
}

[[nodiscard]] inline auto as_view(::std::string const& text) -> ::pltxt2htm::container::U8StringView {
    return ::pltxt2htm::container::U8StringView{reinterpret_cast<char8_t const*>(text.data()), text.size()};
}

} // namespace pltxt2htm_test::format_details