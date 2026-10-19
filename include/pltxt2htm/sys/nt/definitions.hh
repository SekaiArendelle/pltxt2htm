#pragma once

#include <cstddef>
#include "../push_macro.hh"

namespace pltxt2htm::sys::nt {
using Boolean = unsigned char;
using Handle = void*;
using Long = long;
using Long64 = long long;
using NtStatus = long;
using Uchar = unsigned char;
using Ulong = unsigned long;
using Ulong64 = unsigned long long;
using Ushort = unsigned short;

// Enum values adapted from include/fast_io/fast_io_hosted/platforms/nt/nt_definitions.h.
enum class AlpcMessageInformationClass : int {
    AlpcMessageSidInformation = 0,
    AlpcMessageTokenModifiedIdInformation,
    AlpcMessageDirectStatusInformation,
    AlpcMessageHandleInformation,
    MaxAlpcMessageInfoClass
};

enum class FileInformationClass : int {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation = 2,
    FileBothDirectoryInformation = 3,
    FileBasicInformation = 4,
    FileStandardInformation = 5,
    FileInternalInformation = 6,
    FileEaInformation = 7,
    FileAccessInformation = 8,
    FileNameInformation = 9,
    FileRenameInformation = 10,
    FileLinkInformation = 11,
    FileNamesInformation = 12,
    FileDispositionInformation = 13,
    FilePositionInformation = 14,
    FileFullEaInformation = 15,
    FileModeInformation = 16,
    FileAlignmentInformation = 17,
    FileAllInformation = 18,
    FileAllocationInformation = 19,
    FileEndOfFileInformation = 20,
    FileAlternateNameInformation = 21,
    FileStreamInformation = 22,
    FilePipeInformation = 23,
    FilePipeLocalInformation = 24,
    FilePipeRemoteInformation = 25,
    FileMailslotQueryInformation = 26,
    FileMailslotSetInformation = 27,
    FileCompressionInformation = 28,
    FileObjectIdInformation = 29,
    FileCompletionInformation = 30,
    FileMoveClusterInformation = 31,
    FileQuotaInformation = 32,
    FileReparsePointInformation = 33,
    FileNetworkOpenInformation = 34,
    FileAttributeTagInformation = 35,
    FileTrackingInformation = 36,
    FileIdBothDirectoryInformation = 37,
    FileIdFullDirectoryInformation = 38,
    FileValidDataLengthInformation = 39,
    FileShortNameInformation = 40,
    FileIoCompletionNotificationInformation = 41,
    FileIoStatusBlockRangeInformation = 42,
    FileIoPriorityHintInformation = 43,
    FileSfioReserveInformation = 44,
    FileSfioVolumeInformation = 45,
    FileHardLinkInformation = 46,
    FileProcessIdsUsingFileInformation = 47,
    FileNormalizedNameInformation = 48,
    FileNetworkPhysicalNameInformation = 49,
    FileIdGlobalTxDirectoryInformation = 50,
    FileIsRemoteDeviceInformation = 51,
    FileUnusedInformation = 52,
    FileNumaNodeInformation = 53,
    FileStandardLinkInformation = 54,
    FileRemoteProtocolInformation = 55,

    FileRenameInformationBypassAccessCheck = 56,
    FileLinkInformationBypassAccessCheck = 57,

    FileVolumeNameInformation = 58,
    FileIdInformation = 59,
    FileIdExtdDirectoryInformation = 60,
    FileReplaceCompletionInformation = 61,
    FileHardLinkFullIdInformation = 62,
    FileIdExtdBothDirectoryInformation = 63,
    FileDispositionInformationEx = 64,
    FileRenameInformationEx = 65,
    FileRenameInformationExBypassAccessCheck = 66,
    FileDesiredStorageClassInformation = 67,
    FileStatInformation = 68,
    FileMemoryPartitionInformation = 69,
    FileStatLxInformation = 70,
    FileCaseSensitiveInformation = 71,
    FileLinkInformationEx = 72,
    FileLinkInformationExBypassAccessCheck = 73,
    FileStorageReserveIdInformation = 74,
    FileCaseSensitiveInformationForceAccessCheck = 75,
};

enum class FsInformationClass : int {
    FileFsVolumeInformation = 1,
    FileFsLabelInformation,
    FileFsSizeInformation,
    FileFsDeviceInformation,
    FileFsAttributeInformation,
    FileFsControlInformation,
    FileFsFullSizeInformation,
    FileFsObjectIdInformation,
    FileFsDriverPathInformation,
    FileFsVolumeFlagsInformation,
    FileFsSectorSizeInformation,
    FileFsDataCopyInformation,
    FileFsMetadataSizeInformation,
    FileFsFullSizeInformationEx,
    FileFsMaximumInformation
};

enum class ObjectInformationClass : int {
    ObjectBasicInformation = 0,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectTypesInformation,
    ObjectHandleFlagInformation,
    ObjectSessionInformation,
    ObjectSessionObjectInformation,
    MaxObjectInfoClass
};

enum class ProcessInformationClass : int {
    ProcessBasicInformation = 0,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    ProcessIoPriority,
    ProcessExecuteFlags,
    ProcessResourceManagement,
    ProcessCookie,
    ProcessImageInformation,
    ProcessCycleTime,
    ProcessPagePriority,
    ProcessInstrumentationCallback,
    ProcessThreadStackAllocation,
    ProcessWorkingSetWatchEx,
    ProcessImageFileNameWin32,
    ProcessImageFileMapping,
    ProcessAffinityUpdateMode,
    ProcessMemoryAllocationMode,
    ProcessGroupInformation,
    ProcessTokenVirtualizationEnabled,
    ProcessConsoleHostProcess,
    ProcessWindowInformation,
    ProcessHandleInformation,
    ProcessMitigationPolicy,
    ProcessDynamicFunctionTableInformation,
    ProcessHandleCheckingMode,
    ProcessKeepAliveCount,
    ProcessRevokeFileHandles,
    ProcessWorkingSetControl,
    ProcessHandleTable,
    ProcessCheckStackExtentsMode,
    ProcessCommandLineInformation,
    ProcessProtectionInformation,
    ProcessMemoryExhaustion,
    ProcessFaultInformation,
    ProcessTelemetryIdInformation,
    ProcessCommitReleaseInformation,
    ProcessDefaultCpuSetsInformation,
    ProcessAllowedCpuSetsInformation,
    ProcessSubsystemProcess,
    ProcessJobMemoryInformation,
    ProcessInPrivate,
    ProcessRaiseUMExceptionOnInvalidHandleClose,
    ProcessIumChallengeResponse,
    ProcessChildProcessInformation,
    ProcessHighGraphicsPriorityInformation,
    ProcessSubsystemInformation,
    ProcessEnergyValues,
    ProcessActivityThrottleState,
    ProcessActivityThrottlePolicy,
    ProcessWin32kSyscallFilterInformation,
    ProcessDisableSystemAllowedCpuSets,
    ProcessWakeInformation,
    ProcessEnergyTrackingState,
    ProcessManageWritesToExecutableMemory,
    ProcessCaptureTrustletLiveDump,
    ProcessTelemetryCoverage,
    ProcessEnclaveInformation,
    ProcessEnableReadWriteVmLogging,
    ProcessUptimeInformation,
    ProcessImageSection,
    ProcessDebugAuthInformation,
    ProcessSystemResourceManagement,
    ProcessSequenceNumber,
    ProcessLoaderDetour,
    ProcessSecurityDomainInformation,
    ProcessCombineSecurityDomainsInformation,
    ProcessEnableLogging,
    ProcessLeapSecondInformation,
    ProcessFiberShadowStackAllocation,
    ProcessFreeFiberShadowStackAllocation,
    ProcessAltSystemCallInformation,
    ProcessDynamicEHContinuationTargets,
    MaxProcessInfoClass
};

enum class RtlPathType : int {
    RtlPathTypeUnknown = 0,
    RtlPathTypeUncAbsolute,
    RtlPathTypeDriveAbsolute,
    RtlPathTypeDriveRelative,
    RtlPathTypeRooted,
    RtlPathTypeRelative,
    RtlPathTypeLocalDevice,
    RtlPathTypeRootLocalDevice,
};

enum class SectionInformationClass : int {
    SectionBasicInformation = 0,
    SectionImageInformation,
    SectionRelocationInformation,
    MaxSectionInfoClass
};

enum class SystemInformationClass : int {
    SystemBasicInformation = 0,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemNextEventIdInformation,
    SystemEventIdsInformation,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemPlugPlayBusInformation,
    SystemDockInformation,
    SystemPowerInformation,
    SystemProcessorSpeedInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation
};

enum class ThreadInformationClass : Ulong {
    ThreadBasicInformation = 0,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    ThreadSwitchLegacyState,
    ThreadIsTerminated,
    ThreadLastSystemCall,
    ThreadIoPriority,
    ThreadCycleTime,
    ThreadPagePriority,
    ThreadActualBasePriority,
    ThreadTebInformation,
    ThreadCSwitchMon,
    MaxThreadInfoClass
};

enum class Privileges : Ulong {
    SE_CREATE_TOKEN_PRIVILEGE = 2,
    SE_ASSIGNPRIMARYTOKEN_PRIVILEGE = 3,
    SE_LOCK_MEMORY_PRIVILEGE = 4,
    SE_INCREASE_QUOTA_PRIVILEGE = 5,
    SE_MACHINE_ACCOUNT_PRIVILEGE = 6,
    SE_TCB_PRIVILEGE = 7,
    SE_SECURITY_PRIVILEGE = 8,
    SE_TAKE_OWNERSHIP_PRIVILEGE = 9,
    SE_LOAD_DRIVER_PRIVILEGE = 10,
    SE_SYSTEM_PROFILE_PRIVILEGE = 11,
    SE_SYSTEMTIME_PRIVILEGE = 12,
    SE_PROF_SINGLE_PROCESS_PRIVILEGE = 13,
    SE_INC_BASE_PRIORITY_PRIVILEGE = 14,
    SE_CREATE_PAGEFILE_PRIVILEGE = 15,
    SE_CREATE_PERMANENT_PRIVILEGE = 16,
    SE_BACKUP_PRIVILEGE = 17,
    SE_RESTORE_PRIVILEGE = 18,
    SE_SHUTDOWN_PRIVILEGE = 19,
    SE_DEBUG_PRIVILEGE = 20,
    SE_AUDIT_PRIVILEGE = 21,
    SE_SYSTEM_ENVIRONMENT_PRIVILEGE = 22,
    SE_CHANGE_NOTIFY_PRIVILEGE = 23,
    SE_REMOTE_SHUTDOWN_PRIVILEGE = 24,
    SE_UNDOCK_PRIVILEGE = 25,
    SE_SYNC_AGENT_PRIVILEGE = 26,
    SE_ENABLE_DELEGATION_PRIVILEGE = 27,
    SE_MANAGE_VOLUME_PRIVILEGE = 28,
    SE_IMPERSONATE_PRIVILEGE = 29,
    SE_CREATE_GLOBAL_PRIVILEGE = 30,
    SE_TRUSTED_CREDMAN_ACCESS_PRIVILEGE = 31,
    SE_RELABEL_PRIVILEGE = 32,
    SE_INC_WORKING_SET_PRIVILEGE = 33,
    SE_TIME_ZONE_PRIVILEGE = 34,
    SE_CREATE_SYMBOLIC_LINK_PRIVILEGE = 35,
    SE_DELEGATE_SESSION_USER_IMPERSONATE_PRIVILEGE = 36,
};

enum class WaitType : int {
    WaitAll = 0,
    WaitAny
};

enum class EventType : int {
    NotificationEvent = 0,
    SynchronizationEvent
};

enum class TimerType : int {
    NotificationTimer = 0,
    SynchronizationTimer
};

enum class SectionInherit : int {
    ViewShare = 1,
    ViewUnmap = 2
};

enum class VirtualMemoryInformationClass : int {
    VmPrefetchInformation = 0,
    VmPagePriorityInformation,
    VmCfgCallTargetInformation,
};

enum class SecurityImpersonationLevel : int {
    SecurityAnonymous = 0,
    SecurityIdentification,
    SecurityImpersonation,
    SecurityDelegation,
};

enum class PsCreateState : int {
    PsCreateInitialState = 0,
    PsCreateFailOnFileOpen,
    PsCreateFailOnSectionCreate,
    PsCreateFailExeFormat,
    PsCreateFailMachineMismatch,
    PsCreateFailExeName,
    PsCreateSuccess,
    PsCreateMaximumStates
};

struct AnsiString {
    Ushort length;
    Ushort maximum_length;
    char* buffer;
};

struct UnicodeString {
    Ushort length;
    Ushort maximum_length;
    char16_t* buffer;
};

struct ClientId {
    Handle unique_process;
    Handle unique_thread;
};

struct ObjectAttributes {
    Ulong length;
    Handle root_directory;
    UnicodeString* object_name;
    Ulong attributes;
    void* security_descriptor;
    void* security_quality_of_service;
};

struct IoStatusBlock {
    union {
        NtStatus status;
        void* pointer;
    };

    ::std::size_t information;
};

struct RtlpCurdirRef {
    Long ref_count;
    Handle handle;
};

struct RtlRelativeNameU {
    UnicodeString relative_name;
    Handle containing_directory;
    RtlpCurdirRef* cur_dir_ref;
};

struct RtlBuffer {
    unsigned char* buffer;
    unsigned char* static_buffer;
    ::std::size_t size;
    ::std::size_t static_size;
    ::std::size_t reserved_for_allocated_size;
    void* reserved_for_imalloc;
};

struct RtlUnicodeStringBuffer {
    UnicodeString string;
    RtlBuffer byte_buffer;
    char16_t minimum_static_buffer_for_terminal_nul;
};

struct SecurityQualityOfService {
    Ulong length;
    SecurityImpersonationLevel impersonation_level;
    Boolean context_tracking_mode;
    Boolean effective_only;
};

struct AlpcPortAttributes {
    Ulong flags;
    SecurityQualityOfService security_qos;
    ::std::size_t max_message_length;
    ::std::size_t memory_bandwidth;
    ::std::size_t max_pool_usage;
    ::std::size_t max_section_size;
    ::std::size_t max_view_size;
    ::std::size_t max_total_section_size;
    Ulong dup_object_types;
#if defined(_WIN64)
    Ulong reserved;
#endif
};

struct AlpcMessageAttributes {
    Ulong allocated_attributes;
    Ulong valid_attributes;
};

struct PortMessage {
    union {
        struct {
            Ushort data_length;
            Ushort total_length;
        } lengths;

        Ulong length;
    } u1;

    union {
        struct {
            Ushort type;
            Ushort data_info_offset;
        } fields;

        Ulong zero_init;
    } u2;

    union {
        ClientId client;
        double reserved;
    };

    Ulong message_id;

    union {
        ::std::size_t client_view_size;
        Ulong callback_id;
    };
};

struct AlpcDataViewAttr {
    Ulong flags;
    Handle section_handle;
    void* view_base;
    ::std::size_t view_size;
};

struct AlpcSecurityAttr {
    Ulong flags;
    SecurityQualityOfService* qos;
    Handle context_handle;
};

struct PsAttribute {
    ::std::size_t attribute;
    ::std::size_t size;

    union {
        ::std::size_t value;
        void* value_ptr;
    };

    ::std::size_t* return_length;
};

struct PsAttributeList {
    ::std::size_t total_length;
    // Capacity for the caller's attributes; total_length selects the active entries.
    PsAttribute attributes[32];
};

struct Curdir {
    UnicodeString dos_path;
    void* handle;
};

struct RtlDriveLetterCurdir {
    Ushort flags;
    Ushort length;
    Ulong time_stamp;
    AnsiString dos_path;
};

constexpr ::std::size_t rtl_max_drive_letters{32};

struct RtlUserProcessParameters {
    Ulong maximum_length;
    Ulong length;

    Ulong flags;
    Ulong debug_flags;

    void* console_handle;
    Ulong console_flags;
    void* standard_input;
    void* standard_output;
    void* standard_error;

    Curdir current_directory;
    UnicodeString dll_path;
    UnicodeString image_path_name;
    UnicodeString command_line;
    char16_t* environment;

    Ulong starting_x;
    Ulong starting_y;
    Ulong count_x;
    Ulong count_y;
    Ulong count_chars_x;
    Ulong count_chars_y;
    Ulong fill_attribute;

    Ulong window_flags;
    Ulong show_window_flags;
    UnicodeString window_title;
    UnicodeString desktop_info;
    UnicodeString shell_info;
    UnicodeString runtime_data;
    RtlDriveLetterCurdir current_directories[rtl_max_drive_letters];

    ::std::size_t environment_size;
    ::std::size_t environment_version;
    void* package_dependency_data;
    Ulong process_group_id;
    Ulong loader_threads;
    UnicodeString redirection_dll_name;
    UnicodeString heap_partition_name;
    Ulong64* default_threadpool_cpu_set_masks;
    Ulong default_threadpool_cpu_set_mask_count;
    Ulong default_threadpool_thread_maximum;
    Ulong heap_memory_type_mask;
    void* attribute_list;
};

struct PsCreateInfo {
    ::std::size_t size;
    PsCreateState state;

    union {
        struct {
            union {
                Ulong init_flags;

                struct {
                    Uchar write_output_on_exit : 1;
                    Uchar detect_manifest : 1;
                    Uchar ifeo_skip_debugger : 1;
                    Uchar ifeo_do_not_propagate_key_state : 1;
                    Uchar spare_bits1 : 4;
                    Uchar spare_bits2 : 8;
                    Ushort prohibited_image_characteristics : 16;
                } s1;
            } u1;

            Ulong additional_file_access;
        } init_state;

        struct {
            void* file_handle;
        } fail_section;

        struct {
            Ushort dll_characteristics;
        } exe_format;

        struct {
            void* ifeo_key;
        } exe_name;

        struct {
            union {
                Ulong output_flags;

                struct {
                    Uchar protected_process : 1;
                    Uchar address_space_override : 1;
                    Uchar dev_override_enabled : 1;
                    Uchar manifest_detected : 1;
                    Uchar protected_process_light : 1;
                    Uchar spare_bits1 : 3;
                    Uchar spare_bits2 : 8;
                    Ushort spare_bits3 : 16;
                } s2;
            } u2;

            void* file_handle;
            void* section_handle;
            Ulong64 user_process_parameters_native;
            Ulong user_process_parameters_wow64;
            Ulong current_parameter_flags;
            Ulong64 peb_address_native;
            Ulong peb_address_wow64;
            Ulong64 manifest_address;
            Ulong manifest_size;
        } success_state;
    } state_union;
};

struct MemoryRangeEntry {
    void* virtual_address;
    ::std::size_t number_of_bytes;
};

using IoApcRoutine = void(PLTXT2HTM_DETAIL_STDCALL*)(void*, IoStatusBlock*, Ulong);
using TimerApcRoutine = void(PLTXT2HTM_DETAIL_STDCALL*)(void*, Ulong, Long);
using UserThreadStartRoutine = NtStatus(PLTXT2HTM_DETAIL_STDCALL*)(void*);

struct SrwLock {
    void* pointer;
};
} // namespace pltxt2htm::sys::nt

#include "../pop_macro.hh"
