#include <pltxt2htm/pltxt2htm.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

#if __has_attribute(visibility)
    #define PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT __attribute__((visibility("default")))
#else
    #define PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT
#endif

#if __has_cpp_attribute(__gnu__::__used__)
    #define PLTXT2HTM_C_SRC_USED [[__gnu__::__used__]]
#else
    #define PLTXT2HTM_C_SRC_USED
#endif

#define PLTXT2HTM_C_SRC_EXPORT_FUNCTION \
    PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT PLTXT2HTM_C_SRC_USED auto

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2common_html(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2common_html<
#if defined(NDEBUG)
        ::pltxt2htm::Contracts::ignore
#else
        ::pltxt2htm::Contracts::quick_enforce
#endif
        >(pltext);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2advanced_html(::fast_io::u8string_view pltext) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2advanced_html<
#if defined(NDEBUG)
        ::pltxt2htm::Contracts::ignore
#else
        ::pltxt2htm::Contracts::quick_enforce
#endif
        >(pltext);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2plunity_introduction(::fast_io::u8string_view text, ::fast_io::u8string_view project,
                                             ::fast_io::u8string_view visitor, ::fast_io::u8string_view author,
                                             ::fast_io::u8string_view coauthors) noexcept -> ::fast_io::u8string {
    return ::pltxt2htm::pltxt2plunity_introduction<
#if defined(NDEBUG)
        ::pltxt2htm::Contracts::ignore
#else
        ::pltxt2htm::Contracts::quick_enforce
#endif
        >(text, project, visitor, author, coauthors);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_major() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::major;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_minor() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::minor;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_patch() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::patch;
}
