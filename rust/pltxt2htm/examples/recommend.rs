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

    let html = pltxt2htm::pltxt2fixedadv_html(
        pltext,
        CString::new("127.0.0.1").unwrap(),
        CString::new("project").unwrap(),
        CString::new("visitor").unwrap(),
        CString::new("author").unwrap(),
        CString::new("coauthors").unwrap(),
    );
    println!("{}", html.as_str());
}
