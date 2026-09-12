use std::ffi::CString;

#[test]
fn common_parser_links_and_runs() {
    let html = pltxt2htm::pltxt2common_html(CString::new("Hello").unwrap());

    assert_eq!(html.as_str(), "Hello");
}
