use libc::{c_char, size_t};

#[link(name = "pltxt2htm", kind = "static")]
unsafe extern "C" {
    #[cfg(not(debug_assertions))]
    pub unsafe fn pltxt2htm_common_parser(pltext: *const c_char) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn pltxt2htm_common_parserd(pltext: *const c_char) -> *const c_char;
    #[cfg(not(debug_assertions))]
    pub unsafe fn pltxt2htm_advanced_parser(pltext: *const c_char) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn pltxt2htm_advanced_parserd(pltext: *const c_char) -> *const c_char;
    #[cfg(not(debug_assertions))]
    pub unsafe fn pltxt2htm_fixedadv_parser(
        pltext: *const c_char,
        host: *const c_char,
        project: *const c_char,
        visitor: *const c_char,
        author: *const c_char,
        coauthors: *const c_char,
    ) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn pltxt2htm_fixedadv_parserd(
        pltext: *const c_char,
        host: *const c_char,
        project: *const c_char,
        visitor: *const c_char,
        author: *const c_char,
        coauthors: *const c_char,
    ) -> *const c_char;
    pub unsafe fn pltxt2htm_version_major() -> size_t;
    pub unsafe fn pltxt2htm_version_minor() -> size_t;
    pub unsafe fn pltxt2htm_version_patch() -> size_t;
}
