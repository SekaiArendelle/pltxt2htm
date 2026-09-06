/**
 * @file concepts.hh
 * @brief Shared concepts for pltxt2htm implementation details.
 */

#pragma once

#include <concepts>

namespace pltxt2htm::details {

template<typename T>
concept is_char_type = ::std::same_as<T, char> || ::std::same_as<T, wchar_t> || ::std::same_as<T, char8_t> ||
                       ::std::same_as<T, char16_t> || ::std::same_as<T, char32_t>;

} // namespace pltxt2htm::details
