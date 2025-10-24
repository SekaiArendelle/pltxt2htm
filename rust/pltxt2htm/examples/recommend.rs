use pltxt2htm;
use std::ffi::CString;

fn main() {
    let pltext = CString::new(
        r#"
# Hello Quantum PhysicsLab
With Markdown supports
"#,
    )
    .unwrap();
    let host = CString::new("localhost:5173").unwrap();

    let html = pltxt2htm::pltxt2advanced_html(pltext, host);
    println!("{}", html.to_ffi_cstr().to_str().unwrap());
}
