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

    let html = pltxt2htm::pltxt2advanced_html(pltext);
    println!("{}", html.as_str());
}
