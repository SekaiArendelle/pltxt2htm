#if defined(_WIN32)
    #include <windows.h>
    #include <dbghelp.h>
    #include <winternl.h>
    #include <cstddef>
    #include <type_traits>

    #define PLTXT2HTM_DETAIL_DLLIMPORT 11
    #define PLTXT2HTM_DETAIL_STDCALL 12
    #define PLTXT2HTM_DETAIL_STDCALL_ASM_NAME 13
    #define PLTXT2HTM_DETAIL_CDECL 14
    #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME 15

    #include <pltxt2htm/details/symbols/nt/ntdll.hh>
    #include <pltxt2htm/details/symbols/win32/dbghelp.hh>
    #include <pltxt2htm/details/symbols/win32/kernel32.hh>

    #if PLTXT2HTM_DETAIL_DLLIMPORT != 11
        #error "dllimport macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_STDCALL != 12
        #error "stdcall macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_STDCALL_ASM_NAME != 13
        #error "asm-name macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_CDECL != 14
        #error "cdecl macro was not restored"
    #endif
    #if PLTXT2HTM_DETAIL_CDECL_ASM_NAME != 15
        #error "cdecl asm-name macro was not restored"
    #endif

    #undef PLTXT2HTM_DETAIL_DLLIMPORT
    #undef PLTXT2HTM_DETAIL_STDCALL
    #undef PLTXT2HTM_DETAIL_STDCALL_ASM_NAME
    #undef PLTXT2HTM_DETAIL_CDECL
    #undef PLTXT2HTM_DETAIL_CDECL_ASM_NAME

namespace nt = ::pltxt2htm::details::symbols::nt;
namespace win32 = ::pltxt2htm::details::symbols::win32;

using ExpectedCaptureStackBackTrace = nt::Ushort(NTAPI*)(nt::Ulong, nt::Ulong, void**, nt::Ulong*) noexcept;
using ExpectedTryAcquireSrwLockExclusive = nt::Boolean(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedAcquireSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedReleaseSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedDuplicateObject = nt::NtStatus(NTAPI*)(nt::Handle, nt::Handle, nt::Handle, nt::Handle*, nt::Ulong,
                                                     nt::Ulong, nt::Ulong) noexcept;
using ExpectedClose = nt::NtStatus(NTAPI*)(nt::Handle) noexcept;
using ExpectedCriticalSection = nt::NtStatus(NTAPI*)(void*) noexcept;
using ExpectedTryEnterCriticalSection = nt::Ulong(NTAPI*)(void*) noexcept;
using ExpectedAcquirePrivilege = nt::NtStatus(NTAPI*)(nt::Ulong*, nt::Ulong, nt::Ulong, void**) noexcept;
using ExpectedIoApcRoutine = void(NTAPI*)(void*, nt::IoStatusBlock*, nt::Ulong);
using ExpectedTimerApcRoutine = void(NTAPI*)(void*, nt::Ulong, nt::Long);
using ExpectedCreateUserProcess = nt::NtStatus(NTAPI*)(void**, void**, nt::Ulong, nt::Ulong, nt::ObjectAttributes*,
                                                       nt::ObjectAttributes*, nt::Ulong, nt::Ulong,
                                                       nt::RtlUserProcessParameters*, nt::PsCreateInfo*,
                                                       nt::PsAttributeList*) noexcept;
using ExpectedWaitForSingleObject = nt::NtStatus(NTAPI*)(void*, nt::Boolean, nt::Ulong64*) noexcept;
using ExpectedDelayExecution = nt::NtStatus(NTAPI*)(nt::Boolean, nt::Long64*) noexcept;
using ExpectedControlFile = nt::NtStatus(NTAPI*)(void*, void*, nt::IoApcRoutine, void*, nt::IoStatusBlock*,
                                                nt::Ulong, void*, nt::Ulong, void*, nt::Ulong) noexcept;
using ExpectedCreateFile = nt::NtStatus(NTAPI*)(void**, nt::Ulong, nt::ObjectAttributes*,
                                              nt::IoStatusBlock*, nt::Long64*, nt::Ulong,
                                              nt::Ulong, nt::Ulong, nt::Ulong,
                                              void*, nt::Ulong) noexcept;
using ExpectedDbgPrint = nt::Ulong(__cdecl*)(char const*, ...) noexcept;
using ExpectedLoadLibraryExW = win32::ModuleHandle(WINAPI*)(win32::WideChar const*, win32::Handle,
                                                            win32::Dword) noexcept;
using ExpectedGetProcAddress = win32::Procedure(WINAPI*)(win32::ModuleHandle, win32::Char const*) noexcept;
using ExpectedFreeLibrary = win32::Bool(WINAPI*)(win32::ModuleHandle) noexcept;
using ExpectedSymInitialize = win32::Bool(WINAPI*)(win32::Handle, win32::Char const*, win32::Bool) noexcept;
using ExpectedSymRefreshModuleList = win32::Bool(WINAPI*)(win32::Handle) noexcept;
using ExpectedSymCleanup = win32::Bool(WINAPI*)(win32::Handle) noexcept;
using ExpectedSymFromAddr = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64, win32::Dword64*,
                                                 win32::SymbolInfo*) noexcept;
using ExpectedSymGetLineFromAddr64 = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64, win32::Dword*,
                                                          win32::ImageHlpLine64*) noexcept;

static_assert(::std::is_same_v<decltype(&nt::RtlCaptureStackBackTrace), ExpectedCaptureStackBackTrace>);
static_assert(::std::is_same_v<decltype(&nt::RtlTryAcquireSRWLockExclusive), ExpectedTryAcquireSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::RtlAcquireSRWLockExclusive), ExpectedAcquireSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::RtlReleaseSRWLockExclusive), ExpectedReleaseSrwLockExclusive>);
static_assert(::std::is_same_v<decltype(&nt::NtDuplicateObject), ExpectedDuplicateObject>);
static_assert(::std::is_same_v<decltype(&nt::NtClose), ExpectedClose>);
static_assert(::std::is_same_v<decltype(&nt::RtlInitializeCriticalSection), ExpectedCriticalSection>);
static_assert(::std::is_same_v<decltype(&nt::RtlEnterCriticalSection), ExpectedCriticalSection>);
static_assert(::std::is_same_v<decltype(&nt::RtlTryEnterCriticalSection), ExpectedTryEnterCriticalSection>);
static_assert(::std::is_same_v<decltype(&nt::RtlLeaveCriticalSection), ExpectedCriticalSection>);
static_assert(::std::is_same_v<decltype(&nt::RtlDeleteCriticalSection), ExpectedCriticalSection>);
static_assert(::std::is_same_v<decltype(&nt::RtlAcquirePrivilege), ExpectedAcquirePrivilege>);
static_assert(::std::is_same_v<nt::IoApcRoutine, ExpectedIoApcRoutine>);
static_assert(::std::is_same_v<nt::TimerApcRoutine, ExpectedTimerApcRoutine>);
static_assert(::std::is_same_v<decltype(&nt::NtFsControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwFsControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::NtDeviceIoControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwDeviceIoControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::NtCreateFile), ExpectedCreateFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwCreateFile), ExpectedCreateFile>);
static_assert(::std::is_same_v<decltype(&nt::NtCreateUserProcess), ExpectedCreateUserProcess>);
static_assert(::std::is_same_v<decltype(&nt::ZwCreateUserProcess), ExpectedCreateUserProcess>);
static_assert(::std::is_same_v<decltype(&nt::NtWaitForSingleObject), ExpectedWaitForSingleObject>);
static_assert(::std::is_same_v<decltype(&nt::NtDelayExecution), ExpectedDelayExecution>);
static_assert(::std::is_same_v<decltype(&nt::DbgPrint), ExpectedDbgPrint>);
static_assert(::std::is_same_v<decltype(&win32::LoadLibraryExW), ExpectedLoadLibraryExW>);
static_assert(::std::is_same_v<decltype(&win32::GetProcAddress), ExpectedGetProcAddress>);
static_assert(::std::is_same_v<decltype(&win32::FreeLibrary), ExpectedFreeLibrary>);
static_assert(::std::is_same_v<decltype(&win32::SymInitialize), ExpectedSymInitialize>);
static_assert(::std::is_same_v<decltype(&win32::SymRefreshModuleList), ExpectedSymRefreshModuleList>);
static_assert(::std::is_same_v<decltype(&win32::SymCleanup), ExpectedSymCleanup>);
static_assert(::std::is_same_v<decltype(&win32::SymFromAddr), ExpectedSymFromAddr>);
static_assert(::std::is_same_v<decltype(&win32::SymGetLineFromAddr64), ExpectedSymGetLineFromAddr64>);

static_assert(::std::is_same_v<nt::Boolean, BOOLEAN>);
static_assert(::std::is_same_v<nt::Handle, HANDLE>);
static_assert(::std::is_same_v<nt::NtStatus, NTSTATUS>);
static_assert(::std::is_same_v<nt::Long, LONG>);
static_assert(::std::is_same_v<nt::Long64, LONGLONG>);
static_assert(::std::is_same_v<nt::Uchar, UCHAR>);
static_assert(::std::is_same_v<nt::Ulong, ULONG>);
static_assert(::std::is_same_v<nt::Ulong64, ULONG64>);
static_assert(::std::is_same_v<nt::Ushort, USHORT>);
static_assert(sizeof(nt::Privileges) == sizeof(ULONG));
static_assert(sizeof(nt::RtlpCurdirRef) == 2 * sizeof(void*));
static_assert(sizeof(nt::RtlRelativeNameU) == 4 * sizeof(void*));
static_assert(offsetof(nt::RtlRelativeNameU, cur_dir_ref) == 3 * sizeof(void*));
static_assert(::std::is_same_v<::std::underlying_type_t<nt::Privileges>, nt::Ulong>);
static_assert(::std::is_same_v<::std::underlying_type_t<nt::ThreadInformationClass>, nt::Ulong>);
static_assert(sizeof(nt::AlpcPortAttributes) == (sizeof(void*) == 8 ? 72 : 44));
static_assert(offsetof(nt::AlpcPortAttributes, memory_bandwidth) ==
              offsetof(nt::AlpcPortAttributes, max_message_length) + sizeof(::std::size_t));
static_assert(offsetof(nt::RtlUserProcessParameters, environment_version) ==
              offsetof(nt::RtlUserProcessParameters, environment_size) + sizeof(::std::size_t));
static_assert(offsetof(nt::RtlUserProcessParameters, package_dependency_data) ==
              offsetof(nt::RtlUserProcessParameters, environment_size) + 2 * sizeof(::std::size_t));
static_assert(sizeof(nt::SrwLock) == sizeof(SRWLOCK));
static_assert(alignof(nt::SrwLock) == alignof(SRWLOCK));
static_assert(sizeof(nt::UnicodeString) == sizeof(UNICODE_STRING));
static_assert(offsetof(nt::UnicodeString, buffer) == offsetof(UNICODE_STRING, Buffer));
static_assert(sizeof(nt::ObjectAttributes) == sizeof(OBJECT_ATTRIBUTES));
static_assert(offsetof(nt::ObjectAttributes, security_descriptor) == offsetof(OBJECT_ATTRIBUTES, SecurityDescriptor));
static_assert(offsetof(nt::ObjectAttributes, security_quality_of_service) ==
              offsetof(OBJECT_ATTRIBUTES, SecurityQualityOfService));
static_assert(sizeof(nt::IoStatusBlock) == sizeof(IO_STATUS_BLOCK));
static_assert(offsetof(nt::IoStatusBlock, information) == offsetof(IO_STATUS_BLOCK, Information));
static_assert(sizeof(nt::ClientId) == sizeof(CLIENT_ID));
static_assert(sizeof(nt::LargeInteger) == sizeof(LARGE_INTEGER));
static_assert(nt::FileInformationClass::FileBasicInformation == static_cast<nt::FileInformationClass>(4));
static_assert(sizeof(nt::PsCreateInfo) >= sizeof(::std::size_t));
static_assert(offsetof(nt::PsCreateInfo, state_union) > offsetof(nt::PsCreateInfo, state));
static_assert(offsetof(nt::RtlUserProcessParameters, command_line) >
              offsetof(nt::RtlUserProcessParameters, image_path_name));

static_assert(::std::is_same_v<win32::Bool, BOOL>);
static_assert(::std::is_same_v<win32::Char, CHAR>);
static_assert(::std::is_same_v<win32::Dword, DWORD>);
static_assert(::std::is_same_v<win32::Dword64, DWORD64>);
static_assert(::std::is_same_v<win32::Handle, HANDLE>);
static_assert(::std::is_same_v<win32::IntPtr, INT_PTR>);
static_assert(::std::is_same_v<win32::Procedure, FARPROC>);
static_assert(::std::is_same_v<win32::Ulong, ULONG>);
static_assert(::std::is_same_v<win32::Ulong64, ULONG64>);
static_assert(::std::is_same_v<win32::WideChar, WCHAR>);
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
auto* volatile nt_create_file_reference = &nt::NtCreateFile;
auto* volatile zw_create_file_reference = &nt::ZwCreateFile;
auto* volatile dbg_print_reference = &nt::DbgPrint;
auto* volatile capture_reference = &nt::RtlCaptureStackBackTrace;
auto* volatile try_acquire_srw_lock_reference = &nt::RtlTryAcquireSRWLockExclusive;
auto* volatile acquire_srw_lock_reference = &nt::RtlAcquireSRWLockExclusive;
auto* volatile release_srw_lock_reference = &nt::RtlReleaseSRWLockExclusive;
auto* volatile duplicate_object_reference = &nt::NtDuplicateObject;
auto* volatile load_library_reference = &win32::LoadLibraryExW;
auto* volatile get_proc_address_reference = &win32::GetProcAddress;
auto* volatile free_library_reference = &win32::FreeLibrary;
auto* volatile sym_initialize_reference = &win32::SymInitialize;
auto* volatile sym_refresh_module_list_reference = &win32::SymRefreshModuleList;
auto* volatile sym_cleanup_reference = &win32::SymCleanup;
auto* volatile sym_from_addr_reference = &win32::SymFromAddr;
auto* volatile sym_get_line_from_addr_64_reference = &win32::SymGetLineFromAddr64;
#endif

int main() {
}
