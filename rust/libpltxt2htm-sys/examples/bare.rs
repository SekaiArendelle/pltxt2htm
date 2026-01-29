use libc;
use libpltxt2htm_sys;
use std::ffi::{CStr, CString};

fn main() {
    let pltext = CString::new(
        r#"
# Hello Quantum PhysicsLab
With Markdown supports
"#,
    )
    .unwrap();

    #[cfg(debug_assertions)]
    let html = unsafe { libpltxt2htm_sys::pltxt2htm_advanced_parser(pltext.as_ptr()) };
    #[cfg(not(debug_assertions))]
    let html = unsafe { libpltxt2htm_sys::pltxt2htm_advanced_parser(pltext.as_ptr()) };

    println!("{}", unsafe { CStr::from_ptr(html) }.to_str().unwrap());
    unsafe { libc::free(html as *mut libc::c_void) };
}
