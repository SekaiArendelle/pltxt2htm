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
    let host = CString::new("localhost").unwrap();
    let project = CString::new("project").unwrap();
    let visitor = CString::new("visitor").unwrap();
    let author = CString::new("author").unwrap();
    let coauthors = CString::new("coauthors").unwrap();

    let html = unsafe {
        libpltxt2htm_sys::pltxt2htm_fixedadv_parser(
            pltext.as_ptr(),
            host.as_ptr(),
            project.as_ptr(),
            visitor.as_ptr(),
            author.as_ptr(),
            coauthors.as_ptr(),
        )
    };

    println!("{}", unsafe { CStr::from_ptr(html) }.to_str().unwrap());
    unsafe { libc::free(html as *mut libc::c_void) };
}
