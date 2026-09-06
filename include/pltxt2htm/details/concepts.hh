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
constexpr bool reference_constructs_from_temporary_fallback_v =
    !::std::is_lvalue_reference_v<U> || !::std::is_convertible_v<::std::add_pointer_t<::std::remove_reference_t<U>>,
                                                                 ::std::add_pointer_t<::std::remove_reference_t<T>>>;

template<typename T, typename U>
concept reference_constructs_from_temporary =
#if defined(__cpp_lib_reference_from_temporary)
    ::std::reference_constructs_from_temporary_v<T, U>;
#elif defined(__has_builtin)
    #if __has_builtin(__reference_constructs_from_temporary)
    __reference_constructs_from_temporary(T, U);
    #else
    reference_constructs_from_temporary_fallback_v<T, U>;
    #endif
#else
    reference_constructs_from_temporary_fallback_v<T, U>;
#endif

} // namespace pltxt2htm::details
