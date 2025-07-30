#if __cpp_exceptions >= 199711L
    #error "Enable exception in wasm is not recommended"
#endif

#include <emscripten.h>
#include <pltxt2htm/pltxt2htm.h>

EMSCRIPTEN_KEEPALIVE
extern "C" ::std::size_t ver_major() noexcept {
    return ::pltxt2htm::version::major;
}

EMSCRIPTEN_KEEPALIVE
extern "C" ::std::size_t ver_minor() noexcept {
    return ::pltxt2htm::version::minor;
}

EMSCRIPTEN_KEEPALIVE
extern "C" ::std::size_t ver_patch() noexcept {
    return ::pltxt2htm::version::patch;
}

EMSCRIPTEN_KEEPALIVE
extern "C" char8_t const* advanced_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::advanced_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}

EMSCRIPTEN_KEEPALIVE
extern "C" char8_t const* fixedadv_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::fixedadv_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}

EMSCRIPTEN_KEEPALIVE
extern "C" char8_t const* common_parser(char8_t const* text) noexcept {
    return ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text);
}
