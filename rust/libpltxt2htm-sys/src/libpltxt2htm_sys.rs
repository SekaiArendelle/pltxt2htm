use libc::{c_char, size_t};

#[link(name = "pltxt2htm", kind = "static")]
unsafe extern "C" {
    #[cfg(not(debug_assertions))]
    pub unsafe fn common_parser(pltext: *const c_char) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn common_parserd(pltext: *const c_char) -> *const c_char;
    #[cfg(not(debug_assertions))]
    pub unsafe fn advanced_parser(pltext: *const c_char, host: *const c_char) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn advanced_parserd(pltext: *const c_char, host: *const c_char) -> *const c_char;
    #[cfg(not(debug_assertions))]
    pub unsafe fn fixedadv_parser(pltext: *const c_char, host: *const c_char) -> *const c_char;
    #[cfg(debug_assertions)]
    pub unsafe fn fixedadv_parserd(pltext: *const c_char, host: *const c_char) -> *const c_char;
    pub unsafe fn get_version_major() -> size_t;
    pub unsafe fn get_version_minor() -> size_t;
    pub unsafe fn get_version_patch() -> size_t;
}
