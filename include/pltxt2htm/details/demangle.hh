/**
 * @file demangle.hh
 * @brief Itanium C++ ABI symbol demangling for diagnostics.
 * @details Available when the Itanium ABI runtime provides <cxxabi.h>; other
 *          ABIs compile the function to a null-returning stub so callers stay
 *          free of preprocessor branches.
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>

#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif

namespace pltxt2htm::details {

/**
 * @brief Demangle an Itanium ABI symbol name.
 * @param name NUL-terminated symbol name, mangled or plain.
 * @return Demangled text allocated with malloc(), or nullptr when the name is
 *         not mangled or the toolchain has no Itanium ABI demangler. The caller
 *         owns the result and releases it with std::free().
 * @details Demangling is best effort: encodings the C++ ABI runtime demangler
 *          does not support (for example GCC's class-type non-type template
 *          arguments) keep their raw spelling. Some symbol producers (for
 *          example DbgHelp export symbols) strip the leading underscore, so the
 *          prefixed spelling is retried.
 * @note Itanium ABI consumers must link the C++ ABI runtime for
 *       __cxa_demangle; builds that omit it (for example -nostdlib++) must not
 *       include this header or the stacktrace backends that use it.
 */
[[nodiscard]]
inline auto demangle_symbol_name([[maybe_unused]] char const* name) noexcept -> char* {
#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    int status{};
    auto* demangled = ::abi::__cxa_demangle(name, nullptr, nullptr, ::std::addressof(status));
    if (status == 0 && demangled != nullptr) {
        return demangled;
    }
    ::std::free(demangled);
    auto const length = ::std::strlen(name);
    auto* prefixed = static_cast<char*>(::std::malloc(length + 2));
    if (prefixed == nullptr) {
        return nullptr;
    }
    prefixed[0] = '_';
    ::std::memcpy(prefixed + 1, name, length + 1);
    demangled = ::abi::__cxa_demangle(prefixed, nullptr, nullptr, ::std::addressof(status));
    ::std::free(prefixed);
    return status == 0 ? demangled : nullptr;
#else
    return nullptr;
#endif
}

} // namespace pltxt2htm::details
