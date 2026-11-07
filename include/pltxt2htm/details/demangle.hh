/**
 * @file demangle.hh
 * @brief Itanium C++ ABI symbol demangling for diagnostics.
 * @details Available when the Itanium ABI runtime provides <cxxabi.h>; other
 *          ABIs compile the function to a null-returning stub so callers stay
 *          free of preprocessor branches.
 */

#pragma once

#include <cstdlib>
#include <memory>

#include <fast_io/fast_io_dsal/string_view.h>
#include "../container/impl/string_decl.hh"

#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif

namespace pltxt2htm::details {

/**
 * @brief Demangle an Itanium ABI symbol name.
 * @param name View of the NUL-terminated symbol name, mangled or plain; the
 *             view must cover the whole name.
 * @return Demangled text allocated with malloc(), or nullptr when the name is
 *         not mangled or the toolchain has no Itanium ABI demangler. The caller
 *         owns the result and releases it with std::free().
 * @details Demangling is best effort: encodings the C++ ABI runtime demangler
 *          does not support (for example GCC's class-type non-type template
 *          arguments) keep their raw spelling. Some symbol producers (for
 *          example DbgHelp export symbols) strip the leading underscore, so the
 *          prefixed spelling is retried.
 * @note This header only declares the container it uses; the caller's
 *       translation unit must also provide the container definitions, for
 *       example by including container/string.hh or stacktrace/stacktrace.hh.
 * @note Itanium ABI consumers must link the C++ ABI runtime for
 *       __cxa_demangle; builds that omit it (for example -nostdlib++) must not
 *       include this header or the stacktrace backends that use it.
 */
[[nodiscard]]
inline auto demangle_symbol_name([[maybe_unused]] ::fast_io::cstring_view name) noexcept -> char* {
#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    int status{};
    auto* demangled = ::abi::__cxa_demangle(name.c_str(), nullptr, nullptr, ::std::addressof(status));
    if (status == 0 && demangled != nullptr) {
        return demangled;
    }
    ::std::free(demangled);
    // The retry spelling needs the stripped underscore back in front, so this
    // path materializes the name once.
    ::pltxt2htm::container::String prefixed{"_"};
    prefixed.append(name.c_str(), name.c_str() + name.size());
    demangled = ::abi::__cxa_demangle(prefixed.c_str(), nullptr, nullptr, ::std::addressof(status));
    return status == 0 ? demangled : nullptr;
#else
    return nullptr;
#endif
}

} // namespace pltxt2htm::details
