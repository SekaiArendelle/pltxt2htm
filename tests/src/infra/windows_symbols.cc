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

    #include <pltxt2htm/sys/nt/ntdll.hh>
    #include <pltxt2htm/sys/win32/dbghelp.hh>
    #include <pltxt2htm/sys/win32/kernel32.hh>

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

namespace nt = ::pltxt2htm::sys_nt;
namespace win32 = ::pltxt2htm::sys_win32;

using ExpectedCaptureStackBackTrace = nt::Ushort(NTAPI*)(nt::Ulong, nt::Ulong, void**, nt::Ulong*) noexcept;
using ExpectedTryAcquireSrwLockExclusive = nt::Boolean(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedAcquireSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedReleaseSrwLockExclusive = void(NTAPI*)(nt::SrwLock*) noexcept;
using ExpectedDuplicateObject = nt::NtStatus(NTAPI*)(nt::Handle, nt::Handle, nt::Handle, nt::Handle*, nt::Ulong,
                                                     nt::Ulong, nt::Ulong) noexcept;
using ExpectedClose = nt::NtStatus(NTAPI*)(nt::Handle) noexcept;
using ExpectedCriticalSection = nt::NtStatus(NTAPI*)(void*) noexcept;
using ExpectedTryEnterCriticalSection = nt::Ulong(NTAPI*)(void*) noexcept;
using ExpectedPebLock = nt::NtStatus(NTAPI*)() noexcept;
using ExpectedAcquirePrivilege = nt::NtStatus(NTAPI*)(nt::Ulong*, nt::Ulong, nt::Ulong, void**) noexcept;
using ExpectedIoApcRoutine = void(NTAPI*)(void*, nt::IoStatusBlock*, nt::Ulong);
using ExpectedTimerApcRoutine = void(NTAPI*)(void*, nt::Ulong, nt::Long);
using ExpectedUserThreadStartRoutine = nt::NtStatus(NTAPI*)(void*);
using ExpectedInitUnicodeString = void(NTAPI*)(nt::UnicodeString*, char16_t const*) noexcept;
using ExpectedQueryEnvironmentVariable = nt::NtStatus(NTAPI*)(void*, nt::UnicodeString const*,
                                                              nt::UnicodeString*) noexcept;
using ExpectedDosPathNameToNtPathName = nt::Boolean(NTAPI*)(char16_t const*, nt::UnicodeString*, char16_t**,
                                                            nt::RtlRelativeNameU*) noexcept;
using ExpectedDosPathNameToNtPathNameWithStatus = nt::NtStatus(NTAPI*)(char16_t const*, nt::UnicodeString*, char16_t**,
                                                                       nt::RtlRelativeNameU*) noexcept;
using ExpectedCreateUserProcess = nt::NtStatus(NTAPI*)(nt::Handle*, nt::Handle*, nt::Ulong, nt::Ulong,
                                                       nt::ObjectAttributes const*, nt::ObjectAttributes const*,
                                                       nt::Ulong, nt::Ulong, nt::RtlUserProcessParameters*,
                                                       nt::PsCreateInfo*, nt::PsAttributeList*) noexcept;
using ExpectedCreateProcess = nt::NtStatus(NTAPI*)(nt::Handle*, nt::Ulong, nt::ObjectAttributes const*, void*,
                                                   nt::Boolean, void*, void*, void*) noexcept;
using ExpectedWaitForSingleObject = nt::NtStatus(NTAPI*)(void*, nt::Boolean, nt::Long64*) noexcept;
using ExpectedDelayExecution = nt::NtStatus(NTAPI*)(nt::Boolean, nt::Long64*) noexcept;
using ExpectedSetTimer = nt::NtStatus(NTAPI*)(void*, nt::Long64*, nt::TimerApcRoutine, void*, nt::Boolean, nt::Long,
                                              nt::Boolean*) noexcept;
using ExpectedMapViewOfSection = nt::NtStatus(NTAPI*)(void*, void*, void**, ::std::size_t, ::std::size_t, nt::Long64*,
                                                      ::std::size_t*, nt::SectionInherit, nt::Ulong,
                                                      nt::Ulong) noexcept;
using ExpectedEventState = nt::NtStatus(NTAPI*)(void*, nt::Long*) noexcept;
using ExpectedAlpcConnectPort = nt::NtStatus(NTAPI*)(nt::Handle*, nt::UnicodeString const*, nt::ObjectAttributes*,
                                                     nt::AlpcPortAttributes*, nt::Ulong, void*, nt::PortMessage*,
                                                     ::std::size_t*, nt::AlpcMessageAttributes*,
                                                     nt::AlpcMessageAttributes*, nt::Long64*) noexcept;
using ExpectedAlpcGetHeaderSize = nt::Ulong(NTAPI*)(nt::Ulong) noexcept;
using ExpectedAlpcQueryInformationMessage = nt::NtStatus(NTAPI*)(void*, nt::PortMessage*,
                                                                 nt::AlpcMessageInformationClass, void*, nt::Ulong,
                                                                 nt::Ulong*) noexcept;
using ExpectedControlFile = nt::NtStatus(NTAPI*)(void*, void*, nt::IoApcRoutine, void*, nt::IoStatusBlock*, nt::Ulong,
                                                 void*, nt::Ulong, void*, nt::Ulong) noexcept;
using ExpectedCreateFile = nt::NtStatus(NTAPI*)(nt::Handle*, nt::Ulong, nt::ObjectAttributes const*, nt::IoStatusBlock*,
                                                nt::Long64*, nt::Ulong, nt::Ulong, nt::Ulong, nt::Ulong, void*,
                                                nt::Ulong) noexcept;
using ExpectedDbgPrint = nt::Ulong(__cdecl*)(char const*, ...) noexcept;
using ExpectedLoadLibraryExW = win32::ModuleHandle(WINAPI*)(win32::WideChar const*, win32::Handle,
                                                            win32::Dword) noexcept;
using ExpectedGetProcAddress = win32::Procedure(WINAPI*)(win32::ModuleHandle, win32::Char const*) noexcept;
using ExpectedFreeLibrary = win32::Bool(WINAPI*)(win32::ModuleHandle) noexcept;
using ExpectedGetLastError = win32::Dword(WINAPI*)() noexcept;
using ExpectedFormatMessageW = win32::Dword(WINAPI*)(win32::Dword, void const*, win32::Dword, win32::Dword,
                                                     win32::WideChar*, win32::Dword, win32::Char**) noexcept;
using ExpectedTryAcquireSrwLock = win32::Boolean(WINAPI*)(void*) noexcept;
using ExpectedCreateFileW = win32::Handle(WINAPI*)(win32::WideChar const*, win32::Dword, win32::Dword,
                                                   win32::SecurityAttributes*, win32::Dword, win32::Dword,
                                                   win32::Handle) noexcept;
using ExpectedReadFile = win32::Bool(WINAPI*)(win32::Handle, void*, win32::Dword, win32::Dword*,
                                              win32::Overlapped*) noexcept;
using ExpectedTerminateProcess = win32::Bool(WINAPI*)(win32::Handle, UINT) noexcept;
using ExpectedExitProcess = void(WINAPI*)(UINT) noexcept;
using ExpectedCreateThread = win32::Handle(WINAPI*)(win32::SecurityAttributes*, ::std::size_t,
                                                    win32::ThreadStartRoutine, void*, win32::Dword,
                                                    win32::Dword*) noexcept;
using ExpectedSymInitialize = win32::Bool(WINAPI*)(win32::Handle, win32::Char const*, win32::Bool) noexcept;
using ExpectedSymRefreshModuleList = win32::Bool(WINAPI*)(win32::Handle) noexcept;
using ExpectedSymCleanup = win32::Bool(WINAPI*)(win32::Handle) noexcept;
using ExpectedSymFromAddr = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64, win32::Dword64*,
                                                 win32::SymbolInfo*) noexcept;
using ExpectedSymGetLineFromAddr64 = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64, win32::Dword*,
                                                          win32::ImageHlpLine64*) noexcept;
using ExpectedSymSetOptions = win32::Dword(WINAPI*)(win32::Dword) noexcept;
using ExpectedSymInitializeW = win32::Bool(WINAPI*)(win32::Handle, win32::WideChar const*, win32::Bool) noexcept;
using ExpectedSymLoadModuleExW = win32::Dword64(WINAPI*)(win32::Handle, win32::Handle, win32::WideChar const*,
                                                         win32::WideChar const*, win32::Dword64, win32::Dword,
                                                         win32::ModLoadData*, win32::Dword) noexcept;
using ExpectedSymGetModuleInfoW64 = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64,
                                                         win32::ImageHlpModuleW64*) noexcept;
using ExpectedSymEnumSymbolsW = win32::Bool(WINAPI*)(win32::Handle, win32::Ulong64, win32::WideChar const*,
                                                     win32::SymEnumSymbolsCallbackW, void*) noexcept;
using ExpectedSymEnumSymbolsCallbackW = BOOL(WINAPI*)(win32::SymbolInfoW*, ULONG, PVOID);
using ExpectedSymGetLineFromAddrW64 = win32::Bool(WINAPI*)(win32::Handle, win32::Dword64, win32::Dword*,
                                                           win32::ImageHlpLineW64*) noexcept;
using ExpectedUnDecorateSymbolNameW = win32::Dword(WINAPI*)(win32::WideChar const*, win32::WideChar*, win32::Dword,
                                                            win32::Dword) noexcept;
using ExpectedTranslateAddressRoutine64 = DWORD64(WINAPI*)(HANDLE, HANDLE, win32::Address64*);
using ExpectedStackWalk64 = BOOL(WINAPI*)(DWORD, HANDLE, HANDLE, win32::StackFrame64*, PVOID,
                                          win32::ReadProcessMemoryRoutine64, win32::FunctionTableAccessRoutine64,
                                          win32::GetModuleBaseRoutine64, win32::TranslateAddressRoutine64) noexcept;

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
static_assert(::std::is_same_v<decltype(&nt::RtlAcquirePebLock), ExpectedPebLock>);
static_assert(::std::is_same_v<decltype(&nt::RtlReleasePebLock), ExpectedPebLock>);
static_assert(::std::is_same_v<decltype(&nt::RtlAcquirePrivilege), ExpectedAcquirePrivilege>);
static_assert(::std::is_same_v<nt::IoApcRoutine, ExpectedIoApcRoutine>);
static_assert(::std::is_same_v<nt::TimerApcRoutine, ExpectedTimerApcRoutine>);
static_assert(::std::is_same_v<nt::UserThreadStartRoutine, ExpectedUserThreadStartRoutine>);
static_assert(::std::is_same_v<decltype(&nt::RtlInitUnicodeString), ExpectedInitUnicodeString>);
static_assert(::std::is_same_v<decltype(&nt::RtlQueryEnvironmentVariable_U), ExpectedQueryEnvironmentVariable>);
static_assert(::std::is_same_v<decltype(&nt::RtlDosPathNameToNtPathName_U), ExpectedDosPathNameToNtPathName>);
static_assert(::std::is_same_v<decltype(&nt::RtlDosPathNameToNtPathName_U_WithStatus),
                               ExpectedDosPathNameToNtPathNameWithStatus>);
static_assert(::std::is_same_v<decltype(&nt::NtFsControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwFsControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::NtDeviceIoControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwDeviceIoControlFile), ExpectedControlFile>);
static_assert(::std::is_same_v<decltype(&nt::NtCreateFile), ExpectedCreateFile>);
static_assert(::std::is_same_v<decltype(&nt::ZwCreateFile), ExpectedCreateFile>);
static_assert(::std::is_same_v<decltype(&nt::NtCreateUserProcess), ExpectedCreateUserProcess>);
static_assert(::std::is_same_v<decltype(&nt::ZwCreateUserProcess), ExpectedCreateUserProcess>);
static_assert(::std::is_same_v<decltype(&nt::NtCreateProcess), ExpectedCreateProcess>);
static_assert(::std::is_same_v<decltype(&nt::ZwCreateProcess), ExpectedCreateProcess>);
static_assert(::std::is_same_v<decltype(&nt::NtMapViewOfSection), ExpectedMapViewOfSection>);
static_assert(::std::is_same_v<decltype(&nt::ZwMapViewOfSection), ExpectedMapViewOfSection>);
static_assert(::std::is_same_v<decltype(&nt::NtSetEvent), ExpectedEventState>);
static_assert(::std::is_same_v<decltype(&nt::ZwSetEvent), ExpectedEventState>);
static_assert(::std::is_same_v<decltype(&nt::NtResetEvent), ExpectedEventState>);
static_assert(::std::is_same_v<decltype(&nt::ZwResetEvent), ExpectedEventState>);
static_assert(::std::is_same_v<decltype(&nt::NtAlpcConnectPort), ExpectedAlpcConnectPort>);
static_assert(::std::is_same_v<decltype(&nt::ZwAlpcConnectPort), ExpectedAlpcConnectPort>);
static_assert(::std::is_same_v<decltype(&nt::AlpcGetHeaderSize), ExpectedAlpcGetHeaderSize>);
static_assert(::std::is_same_v<decltype(&nt::NtAlpcQueryInformationMessage), ExpectedAlpcQueryInformationMessage>);
static_assert(::std::is_same_v<decltype(&nt::ZwAlpcQueryInformationMessage), ExpectedAlpcQueryInformationMessage>);
static_assert(::std::is_same_v<decltype(&nt::NtWaitForSingleObject), ExpectedWaitForSingleObject>);
static_assert(::std::is_same_v<decltype(&nt::NtDelayExecution), ExpectedDelayExecution>);
static_assert(::std::is_same_v<decltype(&nt::NtSetTimer), ExpectedSetTimer>);
static_assert(::std::is_same_v<decltype(&nt::ZwSetTimer), ExpectedSetTimer>);
static_assert(::std::is_same_v<decltype(&nt::DbgPrint), ExpectedDbgPrint>);
static_assert(::std::is_same_v<decltype(&win32::LoadLibraryExW), ExpectedLoadLibraryExW>);
static_assert(::std::is_same_v<decltype(&win32::GetProcAddress), ExpectedGetProcAddress>);
static_assert(::std::is_same_v<decltype(&win32::FreeLibrary), ExpectedFreeLibrary>);
static_assert(::std::is_same_v<decltype(&win32::GetLastError), ExpectedGetLastError>);
static_assert(::std::is_same_v<decltype(&win32::FormatMessageW), ExpectedFormatMessageW>);
static_assert(::std::is_same_v<decltype(&win32::TryAcquireSRWLockExclusive), ExpectedTryAcquireSrwLock>);
static_assert(::std::is_same_v<decltype(&win32::CreateFileW), ExpectedCreateFileW>);
static_assert(::std::is_same_v<decltype(&win32::ReadFile), ExpectedReadFile>);
static_assert(::std::is_same_v<decltype(&win32::TerminateProcess), ExpectedTerminateProcess>);
static_assert(::std::is_same_v<decltype(&win32::ExitProcess), ExpectedExitProcess>);
static_assert(::std::is_same_v<decltype(&win32::CreateThread), ExpectedCreateThread>);
static_assert(::std::is_same_v<decltype(&win32::SymInitialize), ExpectedSymInitialize>);
static_assert(::std::is_same_v<decltype(&win32::SymRefreshModuleList), ExpectedSymRefreshModuleList>);
static_assert(::std::is_same_v<decltype(&win32::SymCleanup), ExpectedSymCleanup>);
static_assert(::std::is_same_v<decltype(&win32::SymFromAddr), ExpectedSymFromAddr>);
static_assert(::std::is_same_v<decltype(&win32::SymGetLineFromAddr64), ExpectedSymGetLineFromAddr64>);
static_assert(::std::is_same_v<decltype(&win32::SymSetOptions), ExpectedSymSetOptions>);
static_assert(::std::is_same_v<decltype(&win32::SymInitializeW), ExpectedSymInitializeW>);
static_assert(::std::is_same_v<decltype(&win32::SymLoadModuleExW), ExpectedSymLoadModuleExW>);
static_assert(::std::is_same_v<decltype(&win32::SymGetModuleInfoW64), ExpectedSymGetModuleInfoW64>);
static_assert(::std::is_same_v<decltype(&win32::SymEnumSymbolsW), ExpectedSymEnumSymbolsW>);
static_assert(::std::is_same_v<win32::SymEnumSymbolsCallbackW, ExpectedSymEnumSymbolsCallbackW>);
static_assert(::std::is_same_v<decltype(&win32::SymGetLineFromAddrW64), ExpectedSymGetLineFromAddrW64>);
static_assert(::std::is_same_v<decltype(&win32::UnDecorateSymbolNameW), ExpectedUnDecorateSymbolNameW>);
static_assert(::std::is_same_v<win32::ReadProcessMemoryRoutine64, PREAD_PROCESS_MEMORY_ROUTINE64>);
static_assert(::std::is_same_v<win32::FunctionTableAccessRoutine64, PFUNCTION_TABLE_ACCESS_ROUTINE64>);
static_assert(::std::is_same_v<win32::GetModuleBaseRoutine64, PGET_MODULE_BASE_ROUTINE64>);
static_assert(::std::is_same_v<win32::TranslateAddressRoutine64, ExpectedTranslateAddressRoutine64>);
static_assert(::std::is_same_v<decltype(&win32::StackWalk64), ExpectedStackWalk64>);

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
static_assert(offsetof(nt::RtlUserProcessParameters, redirection_dll_name) >
              offsetof(nt::RtlUserProcessParameters, loader_threads));
static_assert(::std::is_same_v<decltype(nt::RtlUserProcessParameters::default_threadpool_cpu_set_masks), nt::Ulong64*>);
static_assert(sizeof(nt::RtlUserProcessParameters) ==
              offsetof(nt::RtlUserProcessParameters, attribute_list) + sizeof(void*));
static_assert(sizeof(nt::SrwLock) == sizeof(SRWLOCK));
static_assert(alignof(nt::SrwLock) == alignof(SRWLOCK));
static_assert(sizeof(nt::UnicodeString) == sizeof(UNICODE_STRING));
static_assert(offsetof(nt::UnicodeString, buffer) == offsetof(UNICODE_STRING, Buffer));
static_assert(sizeof(nt::AnsiString) == sizeof(STRING));
static_assert(offsetof(nt::AnsiString, buffer) == offsetof(STRING, Buffer));
static_assert(::std::is_same_v<decltype(nt::RtlDriveLetterCurdir::dos_path), nt::AnsiString>);
static_assert(sizeof(nt::ObjectAttributes) == sizeof(OBJECT_ATTRIBUTES));
static_assert(offsetof(nt::ObjectAttributes, security_descriptor) == offsetof(OBJECT_ATTRIBUTES, SecurityDescriptor));
static_assert(offsetof(nt::ObjectAttributes, security_quality_of_service) ==
              offsetof(OBJECT_ATTRIBUTES, SecurityQualityOfService));
static_assert(sizeof(nt::IoStatusBlock) == sizeof(IO_STATUS_BLOCK));
static_assert(offsetof(nt::IoStatusBlock, information) == offsetof(IO_STATUS_BLOCK, Information));
static_assert(sizeof(nt::ClientId) == sizeof(CLIENT_ID));
static_assert(sizeof(nt::Long64) == sizeof(LARGE_INTEGER));
static_assert(nt::FileInformationClass::FileBasicInformation == static_cast<nt::FileInformationClass>(4));
static_assert(sizeof(nt::PsCreateInfo) >= sizeof(::std::size_t));
static_assert(offsetof(nt::PsCreateInfo, state_union) > offsetof(nt::PsCreateInfo, state));
static_assert(offsetof(nt::RtlUserProcessParameters, command_line) >
              offsetof(nt::RtlUserProcessParameters, image_path_name));

static_assert(::std::is_same_v<win32::Bool, BOOL>);
static_assert(::std::is_same_v<win32::Boolean, BOOLEAN>);
static_assert(::std::is_same_v<win32::Char, CHAR>);
static_assert(::std::is_same_v<win32::Dword, DWORD>);
static_assert(::std::is_same_v<win32::Dword64, DWORD64>);
static_assert(::std::is_same_v<win32::Handle, HANDLE>);
static_assert(::std::is_same_v<win32::IntPtr, INT_PTR>);
static_assert(::std::is_same_v<win32::Procedure, FARPROC>);
static_assert(::std::is_same_v<win32::Ulong, ULONG>);
static_assert(::std::is_same_v<win32::Ulong64, ULONG64>);
static_assert(::std::is_same_v<win32::Uint, UINT>);
static_assert(::std::is_same_v<win32::WideChar, WCHAR>);
static_assert(sizeof(win32::ExceptionRecord) == sizeof(EXCEPTION_RECORD));
static_assert(alignof(win32::ExceptionRecord) == alignof(EXCEPTION_RECORD));
static_assert(offsetof(win32::ExceptionRecord, information) == offsetof(EXCEPTION_RECORD, ExceptionInformation));
static_assert(sizeof(win32::ExceptionPointers) == sizeof(EXCEPTION_POINTERS));
static_assert(alignof(win32::ExceptionPointers) == alignof(EXCEPTION_POINTERS));
static_assert(offsetof(win32::ExceptionPointers, exception_record) == offsetof(EXCEPTION_POINTERS, ExceptionRecord));
static_assert(offsetof(win32::ExceptionPointers, context_record) == offsetof(EXCEPTION_POINTERS, ContextRecord));
static_assert(sizeof(win32::Coord) == sizeof(COORD));
static_assert(sizeof(win32::SmallRect) == sizeof(SMALL_RECT));
static_assert(sizeof(win32::CharInfo) == sizeof(CHAR_INFO));
static_assert(sizeof(win32::ConsoleScreenBufferInfo) == sizeof(CONSOLE_SCREEN_BUFFER_INFO));
static_assert(sizeof(win32::FileTime) == sizeof(FILETIME));
static_assert(sizeof(win32::SecurityAttributes) == sizeof(SECURITY_ATTRIBUTES));
static_assert(sizeof(win32::Overlapped) == sizeof(OVERLAPPED));
static_assert(offsetof(win32::Overlapped, internal) == offsetof(OVERLAPPED, Internal));
static_assert(offsetof(win32::Overlapped, offset) == offsetof(OVERLAPPED, Offset));
static_assert(offsetof(win32::Overlapped, pointer) == offsetof(OVERLAPPED, Pointer));
static_assert(offsetof(win32::Overlapped, event) == offsetof(OVERLAPPED, hEvent));
static_assert(sizeof(win32::ByHandleFileInformation) == sizeof(BY_HANDLE_FILE_INFORMATION));
static_assert(sizeof(win32::ProcessInformation) == sizeof(PROCESS_INFORMATION));
static_assert(sizeof(win32::StartupInfoA) == sizeof(STARTUPINFOA));
static_assert(sizeof(win32::StartupInfoW) == sizeof(STARTUPINFOW));
static_assert(offsetof(win32::StartupInfoA, reserved2) == offsetof(STARTUPINFOA, lpReserved2));
static_assert(offsetof(win32::StartupInfoA, standard_input) == offsetof(STARTUPINFOA, hStdInput));
static_assert(offsetof(win32::StartupInfoW, reserved2) == offsetof(STARTUPINFOW, lpReserved2));
static_assert(offsetof(win32::StartupInfoW, standard_input) == offsetof(STARTUPINFOW, hStdInput));
static_assert(sizeof(win32::SystemTime) == sizeof(SYSTEMTIME));
static_assert(sizeof(win32::TimeZoneInformation) == sizeof(TIME_ZONE_INFORMATION));
static_assert(sizeof(win32::SystemInfo) == sizeof(SYSTEM_INFO));
static_assert(offsetof(win32::SystemInfo, oem_id) == offsetof(SYSTEM_INFO, dwOemId));
static_assert(offsetof(win32::SystemInfo, processor_architecture) == offsetof(SYSTEM_INFO, wProcessorArchitecture));
static_assert(offsetof(win32::SystemInfo, page_size) == offsetof(SYSTEM_INFO, dwPageSize));
static_assert(sizeof(win32::Win32FindDataA) == sizeof(WIN32_FIND_DATAA));
static_assert(sizeof(win32::Win32FindDataW) == sizeof(WIN32_FIND_DATAW));
static_assert(offsetof(win32::Win32FindDataW, file_name) == offsetof(WIN32_FIND_DATAW, cFileName));
static_assert(offsetof(win32::Win32FindDataW, alternate_file_name) == offsetof(WIN32_FIND_DATAW, cAlternateFileName));
static_assert(sizeof(win32::MemoryRangeEntry) == sizeof(WIN32_MEMORY_RANGE_ENTRY));
static_assert(sizeof(win32::FileInfoByHandleClass) == sizeof(FILE_INFO_BY_HANDLE_CLASS));
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
static_assert(sizeof(win32::Guid) == sizeof(GUID));
static_assert(sizeof(win32::ModLoadData) == sizeof(MODLOAD_DATA));
static_assert(::std::is_layout_compatible_v<win32::ModLoadData, MODLOAD_DATA>);
static_assert(alignof(win32::ModLoadData) == alignof(MODLOAD_DATA));
static_assert(offsetof(win32::ModLoadData, size_of_struct) == offsetof(MODLOAD_DATA, ssize));
static_assert(offsetof(win32::ModLoadData, signature) == offsetof(MODLOAD_DATA, ssig));
static_assert(offsetof(win32::ModLoadData, data) == offsetof(MODLOAD_DATA, data));
static_assert(offsetof(win32::ModLoadData, size) == offsetof(MODLOAD_DATA, size));
static_assert(offsetof(win32::ModLoadData, flags) == offsetof(MODLOAD_DATA, flags));
static_assert(::std::is_same_v<decltype(win32::ModLoadData::data), decltype(MODLOAD_DATA::data)>);
static_assert(sizeof(win32::SymType) == sizeof(SYM_TYPE));
static_assert(sizeof(win32::AddressMode) == sizeof(ADDRESS_MODE));
static_assert(static_cast<int>(win32::AddressMode::Flat) == AddrModeFlat);
static_assert(sizeof(win32::Address64) == sizeof(ADDRESS64));
static_assert(alignof(win32::Address64) == alignof(ADDRESS64));
static_assert(offsetof(win32::Address64, mode) == offsetof(ADDRESS64, Mode));
static_assert(sizeof(win32::KdHelp64) == sizeof(KDHELP64));
static_assert(::std::is_layout_compatible_v<win32::KdHelp64, KDHELP64>);
static_assert(offsetof(win32::KdHelp64, retpoline_stub_function_table) ==
              offsetof(KDHELP64, RetpolineStubFunctionTable));
static_assert(sizeof(win32::StackFrame64) == sizeof(STACKFRAME64));
static_assert(alignof(win32::StackFrame64) == alignof(STACKFRAME64));
static_assert(offsetof(win32::StackFrame64, backing_store) == offsetof(STACKFRAME64, AddrBStore));
static_assert(offsetof(win32::StackFrame64, parameters) == offsetof(STACKFRAME64, Params));
static_assert(offsetof(win32::StackFrame64, kd_help) == offsetof(STACKFRAME64, KdHelp));
static_assert(sizeof(win32::SymbolInfoW) == sizeof(SYMBOL_INFOW));
static_assert(::std::is_layout_compatible_v<win32::SymbolInfoW, SYMBOL_INFOW>);
static_assert(alignof(win32::SymbolInfoW) == alignof(SYMBOL_INFOW));
static_assert(offsetof(win32::SymbolInfoW, reserved) == offsetof(SYMBOL_INFOW, Reserved));
static_assert(offsetof(win32::SymbolInfoW, flags) == offsetof(SYMBOL_INFOW, Flags));
static_assert(offsetof(win32::SymbolInfoW, address) == offsetof(SYMBOL_INFOW, Address));
static_assert(offsetof(win32::SymbolInfoW, name) == offsetof(SYMBOL_INFOW, Name));
static_assert(sizeof(win32::ImageHlpLineW64) == sizeof(IMAGEHLP_LINEW64));
static_assert(offsetof(win32::ImageHlpLineW64, file_name) == offsetof(IMAGEHLP_LINEW64, FileName));
static_assert(sizeof(win32::ImageHlpModule64) == sizeof(IMAGEHLP_MODULE64));
static_assert(alignof(win32::ImageHlpModule64) == alignof(IMAGEHLP_MODULE64));
static_assert(offsetof(win32::ImageHlpModule64, pdb_signature70) == offsetof(IMAGEHLP_MODULE64, PdbSig70));
static_assert(offsetof(win32::ImageHlpModule64, reserved) == offsetof(IMAGEHLP_MODULE64, Reserved));
static_assert(sizeof(win32::ImageHlpModuleW64) == sizeof(IMAGEHLP_MODULEW64));
static_assert(alignof(win32::ImageHlpModuleW64) == alignof(IMAGEHLP_MODULEW64));
static_assert(offsetof(win32::ImageHlpModuleW64, pdb_signature70) == offsetof(IMAGEHLP_MODULEW64, PdbSig70));
static_assert(offsetof(win32::ImageHlpModuleW64, reserved) == offsetof(IMAGEHLP_MODULEW64, Reserved));

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
auto* volatile stack_walk_64_reference = &win32::StackWalk64;
#endif

int main() {
}
