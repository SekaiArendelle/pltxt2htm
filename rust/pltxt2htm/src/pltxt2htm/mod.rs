mod u8string;
pub mod version;

use libpltxt2htm_sys;
use std::ffi::CString;
pub use u8string::U8String;

pub fn pltxt2common_html(pltext: CString) -> U8String {
    let c_html = unsafe { libpltxt2htm_sys::pltxt2htm_common_parser(pltext.as_ptr()) };

    return U8String::new(c_html);
}

pub fn pltxt2fixedadv_html(
    pltext: CString,
    host: CString,
    project: CString,
    visitor: CString,
    author: CString,
    coauthors: CString,
) -> U8String {
    let c_html = unsafe {
        libpltxt2htm_sys::pltxt2htm_fixedadv_parser(
            pltext.as_ptr(),
            host.as_ptr(),
            project.as_ptr(),
            visitor.as_ptr(),
            author.as_ptr(),
            coauthors.as_ptr(),
        )
    };

    return U8String::new(c_html);
}
