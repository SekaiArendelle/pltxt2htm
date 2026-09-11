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

template<typename T, typename U>
concept reference_constructs_from_temporary =
#if __cpp_lib_reference_from_temporary >= 202202L
    ::std::reference_constructs_from_temporary_v<T, U>;
#elif defined(__has_builtin)
    #if __has_builtin(__reference_constructs_from_temporary)
    __reference_constructs_from_temporary(T, U);
    #else
        #error "compiler too old for __reference_constructs_from_temporary"
    #endif
#else
    #error "__has_builtin is not supported; compiler too old for reference_constructs_from_temporary_v"
#endif

} // namespace pltxt2htm::details
