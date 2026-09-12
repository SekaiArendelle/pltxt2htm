use libc;
use std::ffi::CStr;

pub struct U8String {
    ptr: *const libc::c_char,
}

impl U8String {
    pub(crate) fn new(ptr: *const libc::c_char) -> Self {
        debug_assert!(!ptr.is_null());
        U8String { ptr }
    }

    pub unsafe fn as_ptr(&self) -> *const libc::c_char {
        self.ptr
    }

    pub fn as_cstr(&self) -> &CStr {
        return unsafe { CStr::from_ptr(self.ptr) };
    }

    pub fn as_str(&self) -> &str {
        return self
            .as_cstr()
            .to_str()
            .expect("pltxt2htm returned invalid UTF-8");
    }
}

impl Drop for U8String {
    fn drop(&mut self) {
        unsafe {
            libpltxt2htm_sys::pltxt2htm_free_u8string(self.ptr);
        }
    }
}

impl Clone for U8String {
    fn clone(&self) -> Self {
        let str_len = unsafe { libc::strlen(self.ptr) };
        let ptr = unsafe { libc::malloc(str_len + 1) } as *mut libc::c_char;
        if ptr.is_null() {
            panic!("malloc failed");
        }
        unsafe {
            libc::memcpy(
                ptr as *mut libc::c_void,
                self.ptr as *const libc::c_void,
                str_len,
            )
        };
        unsafe {
            *ptr.offset(str_len as isize) = '\0' as libc::c_char;
        };
        U8String::new(ptr)
    }
}

impl AsRef<CStr> for U8String {
    fn as_ref(&self) -> &CStr {
        self.as_cstr()
    }
}

impl AsRef<str> for U8String {
    fn as_ref(&self) -> &str {
        self.as_str()
    }
}
