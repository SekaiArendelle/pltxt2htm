use std::ffi::CString;

use libpltxt2htm_sys::libpltxt2htm_sys;
mod c_malloc_string;

pub fn pltxt2common_html(pltext: CString) -> c_malloc_string::CMallocString {
    #[cfg(debug_assertions)]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_common_parserd(pltext.as_ptr()) };
    #[cfg(not(debug_assertions))]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_common_parser(pltext.as_ptr()) };

    return c_malloc_string::CMallocString::new(c_html);
}

pub fn pltxt2advanced_html(pltxt: CString, host: CString) -> c_malloc_string::CMallocString {
    #[cfg(debug_assertions)]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_advanced_parserd(pltxt.as_ptr(), host.as_ptr()) };
    #[cfg(not(debug_assertions))]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_advanced_parser(pltxt.as_ptr(), host.as_ptr()) };

    return c_malloc_string::CMallocString::new(c_html);
}

pub fn pltxt2fixedadv_html(pltext: CString, host: CString) -> c_malloc_string::CMallocString {
    #[cfg(debug_assertions)]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_fixedadv_parserd(pltext.as_ptr(), host.as_ptr()) };
    #[cfg(not(debug_assertions))]
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_fixedadv_parser(pltext.as_ptr(), host.as_ptr()) };

    return c_malloc_string::CMallocString::new(c_html);
}
