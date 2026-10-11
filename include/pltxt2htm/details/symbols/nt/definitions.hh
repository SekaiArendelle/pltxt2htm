#pragma once

#include <cstddef>
#include "enums.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details::symbols::nt {
using Boolean = unsigned char;
using Handle = void*;
using Long = long;
using Long64 = long long;
using NtStatus = long;
using Uchar = unsigned char;
using Ulong = unsigned long;
using Ulong64 = unsigned long long;
using Ushort = unsigned short;

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

union LargeInteger {
    struct {
        Ulong low_part;
        long high_part;
    } parts;

    Long64 quad_part;
};

struct RtlRelativeNameU {
    UnicodeString relative_name;
    Handle containing_directory;

    struct {
        long ref_count;
        Handle handle;
    } cur_dir_ref;
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
    UnicodeString dos_path;
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
using TimerApcRoutine = void (*)(void*, Ulong, Long);
using UserThreadStartRoutine = Ulong(PLTXT2HTM_DETAIL_STDCALL*)(void*);

struct SrwLock {
    void* pointer;
};
} // namespace pltxt2htm::details::symbols::nt

#include "../pop_macro.hh"
