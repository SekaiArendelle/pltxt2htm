use std::ffi::CString;

#[test]
fn as_str_accepts_valid_utf8() {
    let value: pltxt2htm::U8String = pltxt2htm::pltxt2common_html(CString::new("Hello").unwrap());

    assert_eq!(value.as_str(), "Hello");
}

#[test]
#[should_panic(expected = "pltxt2htm returned invalid UTF-8")]
fn as_str_rejects_invalid_utf8() {
    let value = pltxt2htm::pltxt2common_html(CString::new("Hello").unwrap());
    unsafe {
        value.as_ptr().cast_mut().write(0xff_u8 as libc::c_char);
    }

    value.as_str();
}
