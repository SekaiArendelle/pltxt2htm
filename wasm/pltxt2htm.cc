#if __cpp_exceptions >= 199711L
    #error "Enable exception in wasm is not recommended"
#endif

#include <pltxt2htm/pltxt2htm.h>

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    ::std::size_t ver_major() noexcept {
    return ::pltxt2htm::version::major;
}

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    ::std::size_t ver_minor() noexcept {
    return ::pltxt2htm::version::minor;
}

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    ::std::size_t ver_patch() noexcept {
    return ::pltxt2htm::version::patch;
}

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    char8_t const* advanced_parser(char8_t const* const text) noexcept {
    return ::pltxt2htm::advanced_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text);
}

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    char8_t const* fixedadv_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::fixedadv_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}

extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    char8_t const* common_parser(char8_t const* text) noexcept {
    return ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text);
}
