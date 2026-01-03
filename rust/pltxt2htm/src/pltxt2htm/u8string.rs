use libc;
use std::ffi::CStr;

pub struct U8String {
    ptr: *const libc::c_char,
}

impl U8String {
    pub fn new(ptr: *const libc::c_char) -> Self {
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
        return unsafe {
            std::str::from_utf8_unchecked(std::slice::from_raw_parts(
                self.ptr as *const u8,
                libc::strlen(self.ptr),
            ))
        };
    }
}

impl Drop for U8String {
    fn drop(&mut self) {
        unsafe {
            libc::free(self.ptr as *mut libc::c_void);
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
