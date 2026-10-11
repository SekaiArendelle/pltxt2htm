#pragma once

#include <cstddef>
#include <cstdint>

#include "enums.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details::symbols::nt {
using Boolean = unsigned char;
using Handle = void*;
using NtStatus = long;
using Ulong = unsigned long;
using Ushort = unsigned short;

enum class wait_type : unsigned int {
    WaitAll,
    WaitAny
};
enum class event_type : unsigned int {
    NotificationEvent,
    SynchronizationEvent
};
enum class timer_type : unsigned int {
    NotificationTimer,
    SynchronizationTimer
};
enum class section_inherit : unsigned int {
    ViewShare = 1,
    ViewUnmap = 2
};
enum class virtual_memory_information_class : unsigned int {
    VmPrefetchInformation,
    VmPagePriorityInformation,
    VmCfgCallTargetInformation,
};

struct unicode_string {
    Ushort length;
    Ushort maximum_length;
    char16_t* buffer;
};

struct client_id {
    Handle unique_process;
    Handle unique_thread;
};

struct object_attributes {
    Ulong length;
    Handle root_directory;
    unicode_string* object_name;
    Ulong attributes;
    void* security_descriptor;
    void* security_quality_of_service;
};

struct io_status_block {
    union {
        NtStatus status;
        void* pointer;
    };

    ::std::size_t information;
};

union large_integer {
    struct {
        Ulong low_part;
        long high_part;
    } parts;

    ::std::int_least64_t quad_part;
};

struct rtl_relative_name_u {
    unicode_string relative_name;
    Handle containing_directory;

    struct {
        long ref_count;
        Handle handle;
    } cur_dir_ref;
};

struct rtl_buffer {
    unsigned char* buffer;
    unsigned char* static_buffer;
    ::std::size_t size;
    ::std::size_t static_size;
    ::std::size_t reserved_for_allocated_size;
    void* reserved_for_imalloc;
};

struct rtl_unicode_string_buffer {
    unicode_string string;
    rtl_buffer byte_buffer;
    char16_t minimum_static_buffer_for_terminal_nul;
};

enum class security_impersonation_level : unsigned int {
    SecurityAnonymous,
    SecurityIdentification,
    SecurityImpersonation,
    SecurityDelegation,
};

struct security_quality_of_service {
    Ulong length;
    security_impersonation_level impersonation_level;
    Boolean context_tracking_mode;
    Boolean effective_only;
};

struct alpc_port_attributes {
    Ulong flags;
    security_quality_of_service security_qos;
    ::std::uint_least64_t max_message_length;
    ::std::uint_least64_t memory_bandwidth;
    ::std::uint_least64_t max_pool_usage;
    ::std::uint_least64_t max_section_size;
    ::std::uint_least64_t max_view_size;
    ::std::uint_least64_t max_total_section_size;
    Ulong dup_object_types;
#if defined(_WIN64)
    Ulong reserved;
#endif
};

struct alpc_message_attributes {
    Ulong allocated_attributes;
    Ulong valid_attributes;
};

struct port_message {
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
        client_id client;
        double reserved;
    };

    Ulong message_id;

    union {
        ::std::size_t client_view_size;
        Ulong callback_id;
    };
};

struct alpc_data_view_attr {
    Ulong flags;
    Handle section_handle;
    void* view_base;
    ::std::size_t view_size;
};

struct alpc_security_attr {
    Ulong flags;
    security_quality_of_service* qos;
    Handle context_handle;
};

struct ps_attribute {
    ::std::size_t attribute;
    ::std::size_t size;

    union {
        ::std::size_t value;
        void* value_ptr;
    };

    ::std::size_t* return_length;
};

struct ps_attribute_list {
    ::std::size_t total_length;
    ps_attribute attributes[32];
};

struct curdir {
    unicode_string DosPath;
    void* Handle;
};

struct rtl_drive_letter_curdir {
    ::std::uint_least16_t Flags;
    ::std::uint_least16_t Length;
    ::std::uint_least32_t TimeStamp;
    unicode_string DosPath;
};

constexpr ::std::size_t rtl_max_drive_letters{32};

struct rtl_user_process_parameters {
    ::std::uint_least32_t MaximumLength;
    ::std::uint_least32_t Length;

    ::std::uint_least32_t Flags;
    ::std::uint_least32_t DebugFlags;

    void* ConsoleHandle;
    ::std::uint_least32_t ConsoleFlags;
    void* StandardInput;
    void* StandardOutput;
    void* StandardError;

    curdir CurrentDirectory;
    unicode_string DllPath;
    unicode_string ImagePathName;
    unicode_string CommandLine;
    char16_t* Environment;

    ::std::uint_least32_t StartingX;
    ::std::uint_least32_t StartingY;
    ::std::uint_least32_t CountX;
    ::std::uint_least32_t CountY;
    ::std::uint_least32_t CountCharsX;
    ::std::uint_least32_t CountCharsY;
    ::std::uint_least32_t FillAttribute;

    ::std::uint_least32_t WindowFlags;
    ::std::uint_least32_t ShowWindowFlags;
    unicode_string WindowTitle;
    unicode_string DesktopInfo;
    unicode_string ShellInfo;
    unicode_string RuntimeData;
    rtl_drive_letter_curdir CurrentDirectories[rtl_max_drive_letters];

    ::std::uint_least32_t EnvironmentSize;
    ::std::uint_least32_t EnvironmentVersion;
    void* PackageDependencyData;
    ::std::uint_least32_t ProcessGroupId;
    ::std::uint_least32_t LoaderThreads;
};

enum class ps_create_state {
    PsCreateInitialState,
    PsCreateFailOnFileOpen,
    PsCreateFailOnSectionCreate,
    PsCreateFailExeFormat,
    PsCreateFailMachineMismatch,
    PsCreateFailExeName,
    PsCreateSuccess,
    PsCreateMaximumStates
};

struct ps_create_info {
    ::std::size_t Size;
    ps_create_state State;

    union {
        struct {
            union u1union {
                ::std::uint_least32_t InitFlags;

                struct {
                    ::std::uint_least8_t WriteOutputOnExit : 1;
                    ::std::uint_least8_t DetectManifest : 1;
                    ::std::uint_least8_t IFEOSkipDebugger : 1;
                    ::std::uint_least8_t IFEODoNotPropagateKeyState : 1;
                    ::std::uint_least8_t SpareBits1 : 4;
                    ::std::uint_least8_t SpareBits2 : 8;
                    ::std::uint_least16_t ProhibitedImageCharacteristics : 16;
                } s1;
            } u1;

            ::std::uint_least32_t AdditionalFileAccess;
        } InitState;

        struct {
            void* FileHandle;
        } FailSection;

        struct {
            ::std::uint_least16_t DllCharacteristics;
        } ExeFormat;

        struct {
            void* IFEOKey;
        } ExeName;

        struct {
            union u2union {
                ::std::uint_least32_t OutputFlags;

                struct {
                    ::std::uint_least8_t ProtectedProcess : 1;
                    ::std::uint_least8_t AddressSpaceOverride : 1;
                    ::std::uint_least8_t DevOverrideEnabled : 1;
                    ::std::uint_least8_t ManifestDetected : 1;
                    ::std::uint_least8_t ProtectedProcessLight : 1;
                    ::std::uint_least8_t SpareBits1 : 3;
                    ::std::uint_least8_t SpareBits2 : 8;
                    ::std::uint_least16_t SpareBits3 : 16;
                } s2;
            } u2;

            void* FileHandle;
            void* SectionHandle;
            ::std::uint_least64_t UserProcessParametersNative;
            ::std::uint_least32_t UserProcessParametersWow64;
            ::std::uint_least32_t CurrentParameterFlags;
            ::std::uint_least64_t PebAddressNative;
            ::std::uint_least32_t PebAddressWow64;
            ::std::uint_least64_t ManifestAddress;
            ::std::uint_least32_t ManifestSize;
        } SuccessState;
    } stateunion;
};

struct memory_range_entry {
    void* virtual_address;
    ::std::size_t number_of_bytes;
};

using pio_apc_routine = void (*)(void*, io_status_block*, Ulong) noexcept;
using ptimer_apc_routine = void (*)(void*, Ulong, long) noexcept;
using user_thread_start_routine = Ulong(PLTXT2HTM_DETAIL_STDCALL*)(void*);

struct SrwLock {
    void* pointer;
};
} // namespace pltxt2htm::details::symbols::nt

#include "../pop_macro.hh"
