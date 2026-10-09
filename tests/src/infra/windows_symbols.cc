#if defined(_WIN32)
    #include <windows.h>
    #include <dbghelp.h>
    #include <winternl.h>
    #include <cstddef>
    #include <cstdint>
    #include <type_traits>

    #define PLTXT2HTM_DETAIL_NT_DLLIMPORT 11
    #define PLTXT2HTM_DETAIL_NT_CALL 12
    #define PLTXT2HTM_DETAIL_NT_ASM_NAME 13
    #define PLTXT2HTM_DETAIL_WIN32_DLLIMPORT 21
    #define PLTXT2HTM_DETAIL_WIN32_CALL 22
    #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME 23

    #include <pltxt2htm/details/symbols/nt/ntdll.hh>
    #include <pltxt2htm/details/symbols/win32/dbghelp.hh>
    #include <pltxt2htm/details/symbols/win32/kernel32.hh>

    #if PLTXT2HTM_DETAIL_NT_DLLIMPORT != 11
        #error "NT import macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_NT_CALL != 12
        #error "NT calling-convention macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_NT_ASM_NAME != 13
        #error "NT asm-name macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_WIN32_DLLIMPORT != 21
        #error "Win32 import macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_WIN32_CALL != 22
        #error "Win32 calling-convention macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_WIN32_ASM_NAME != 23
        #error "Win32 asm-name macro was not restored"
    #endif

    #undef PLTXT2HTM_DETAIL_NT_DLLIMPORT
    #undef PLTXT2HTM_DETAIL_NT_CALL
    #undef PLTXT2HTM_DETAIL_NT_ASM_NAME
    #undef PLTXT2HTM_DETAIL_WIN32_DLLIMPORT
    #undef PLTXT2HTM_DETAIL_WIN32_CALL
    #undef PLTXT2HTM_DETAIL_WIN32_ASM_NAME

namespace nt = ::pltxt2htm::details::symbols::nt;
namespace win32 = ::pltxt2htm::details::symbols::win32;

using ExpectedCaptureStackBackTrace = unsigned short(NTAPI*)(unsigned long, unsigned long, void**,
                                                             unsigned long*) noexcept;
using ExpectedTryAcquireSrwLockExclusive = unsigned char(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedAcquireSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedReleaseSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedDuplicateObject = nt::NtStatus(NTAPI*)(nt::Handle, nt::Handle, nt::Handle, nt::Handle*, unsigned long,
                                                     unsigned long, unsigned long) noexcept;
using ExpectedClose = nt::NtStatus(NTAPI*)(nt::Handle) noexcept;
using ExpectedLoadLibraryExW = win32::ModuleHandle(WINAPI*)(wchar_t const*, win32::Handle, unsigned long) noexcept;
using ExpectedGetProcAddress = win32::Procedure(WINAPI*)(win32::ModuleHandle, char const*) noexcept;
using ExpectedFreeLibrary = int(WINAPI*)(win32::ModuleHandle) noexcept;

static_assert(::std::is_same_v<decltype(&nt::RtlCaptureStackBackTrace), ExpectedCaptureStackBackTrace>);
static_assert(::std::is_same_v<decltype(&nt::RtlTryAcquireSRWLockExclusive), ExpectedTryAcquireSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::RtlAcquireSRWLockExclusive), ExpectedAcquireSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::RtlReleaseSRWLockExclusive), ExpectedReleaseSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::NtDuplicateObject), ExpectedDuplicateObject>);
static_assert(::std::is_same_v<decltype(&nt::NtClose), ExpectedClose>);
static_assert(::std::is_same_v<decltype(&win32::LoadLibraryExW), ExpectedLoadLibraryExW>);
static_assert(::std::is_same_v<decltype(&win32::GetProcAddress), ExpectedGetProcAddress>);
static_assert(::std::is_same_v<decltype(&win32::FreeLibrary), ExpectedFreeLibrary>);

static_assert(sizeof(nt::NtStatus) == sizeof(NTSTATUS));
static_assert(sizeof(nt::SrwLock) == sizeof(SRWLOCK));
static_assert(alignof(nt::SrwLock) == alignof(SRWLOCK));

static_assert(::std::is_same_v<win32::Procedure, FARPROC>);
static_assert(::std::is_same_v<win32::SymInitialize, decltype(&::SymInitialize)>);
static_assert(::std::is_same_v<win32::SymCleanup, decltype(&::SymCleanup)>);

static_assert(sizeof(win32::SymbolInfo) == sizeof(SYMBOL_INFO));
static_assert(alignof(win32::SymbolInfo) == alignof(SYMBOL_INFO));
static_assert(offsetof(win32::SymbolInfo, size_of_struct) == offsetof(SYMBOL_INFO, SizeOfStruct));
static_assert(offsetof(win32::SymbolInfo, type_index) == offsetof(SYMBOL_INFO, TypeIndex));
static_assert(offsetof(win32::SymbolInfo, reserved) == offsetof(SYMBOL_INFO, Reserved));
    #if defined(__MINGW32__)
static_assert(offsetof(win32::SymbolInfo, index) == offsetof(SYMBOL_INFO, info));
    #else
static_assert(offsetof(win32::SymbolInfo, index) == offsetof(SYMBOL_INFO, Index));
    #endif
static_assert(offsetof(win32::SymbolInfo, size) == offsetof(SYMBOL_INFO, Size));
static_assert(offsetof(win32::SymbolInfo, name) == offsetof(SYMBOL_INFO, Name));
static_assert(offsetof(win32::SymbolInfo, flags) == offsetof(SYMBOL_INFO, Flags));
static_assert(offsetof(win32::SymbolInfo, value) == offsetof(SYMBOL_INFO, Value));
static_assert(offsetof(win32::SymbolInfo, mod_base) == offsetof(SYMBOL_INFO, ModBase));
static_assert(offsetof(win32::SymbolInfo, address) == offsetof(SYMBOL_INFO, Address));
static_assert(offsetof(win32::SymbolInfo, register_number) == offsetof(SYMBOL_INFO, Register));
static_assert(offsetof(win32::SymbolInfo, scope) == offsetof(SYMBOL_INFO, Scope));
static_assert(offsetof(win32::SymbolInfo, tag) == offsetof(SYMBOL_INFO, Tag));
static_assert(offsetof(win32::SymbolInfo, name_len) == offsetof(SYMBOL_INFO, NameLen));
static_assert(offsetof(win32::SymbolInfo, max_name_len) == offsetof(SYMBOL_INFO, MaxNameLen));

static_assert(sizeof(win32::ImageHlpLine64) == sizeof(IMAGEHLP_LINE64));
static_assert(alignof(win32::ImageHlpLine64) == alignof(IMAGEHLP_LINE64));
static_assert(offsetof(win32::ImageHlpLine64, size_of_struct) == offsetof(IMAGEHLP_LINE64, SizeOfStruct));
static_assert(offsetof(win32::ImageHlpLine64, key) == offsetof(IMAGEHLP_LINE64, Key));
static_assert(offsetof(win32::ImageHlpLine64, line_number) == offsetof(IMAGEHLP_LINE64, LineNumber));
static_assert(offsetof(win32::ImageHlpLine64, file_name) == offsetof(IMAGEHLP_LINE64, FileName));
static_assert(offsetof(win32::ImageHlpLine64, address) == offsetof(IMAGEHLP_LINE64, Address));

auto* volatile nt_close_reference = &nt::NtClose;
auto* volatile capture_reference = &nt::RtlCaptureStackBackTrace;
auto* volatile try_acquire_srw_lock_reference = &nt::RtlTryAcquireSRWLockExclusive;
auto* volatile acquire_srw_lock_reference = &nt::RtlAcquireSRWLockExclusive;
auto* volatile release_srw_lock_reference = &nt::RtlReleaseSRWLockExclusive;
auto* volatile duplicate_object_reference = &nt::NtDuplicateObject;
auto* volatile load_library_reference = &win32::LoadLibraryExW;
auto* volatile get_proc_address_reference = &win32::GetProcAddress;
auto* volatile free_library_reference = &win32::FreeLibrary;
#endif

int main() {
}
