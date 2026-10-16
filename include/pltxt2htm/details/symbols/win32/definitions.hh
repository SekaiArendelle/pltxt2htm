#pragma once

#include <cstddef>
#include <cstdint>

#include "../push_macro.hh"

namespace pltxt2htm::details::symbols::win32 {
using Bool = int;
using Boolean = unsigned char;
using Char = char;
using Dword = unsigned long;
using Dword64 = unsigned long long;
using Handle = void*;
using IntPtr = ::std::intptr_t;
using Long = long;
using Long64 = long long;
// Keep module handles opaque instead of importing the SDK's global HINSTANCE__ tag.
using ModuleHandle = Handle;
using Ulong = unsigned long;
using Ulong64 = unsigned long long;
using Uint = unsigned int;
using WideChar = wchar_t;
using Word = unsigned short;

// CONTEXT has a different layout for each Windows architecture.
struct Context;

constexpr ::std::size_t exception_maximum_parameters{15};

struct ExceptionRecord {
    Dword code;
    Dword flags;
    ExceptionRecord* exception_record;
    void* address;
    Dword number_parameters;
    ::std::size_t information[exception_maximum_parameters];
};

struct ExceptionPointers {
    ExceptionRecord* exception_record;
    Context* context_record;
};

struct Coord {
    short x;
    short y;
};

struct SmallRect {
    short left;
    short top;
    short right;
    short bottom;
};

struct CharInfo {
    union {
        Char ascii_char;
        WideChar unicode_char;
    } character;

    Word attributes;
};

struct ConsoleScreenBufferInfo {
    Coord size;
    Coord cursor_position;
    Word attributes;
    SmallRect window;
    Coord maximum_window_size;
};

struct FileTime {
    Dword low_date_time;
    Dword high_date_time;
};

struct SecurityAttributes {
    Dword length;
    void* security_descriptor;
    Bool inherit_handle;
};

struct Overlapped {
    ::std::size_t internal;
    ::std::size_t internal_high;

    union {
        struct {
            Dword offset;
            Dword offset_high;
        };

        void* pointer;
    };

    Handle event;
};

struct ByHandleFileInformation {
    Dword file_attributes;
    FileTime creation_time;
    FileTime last_access_time;
    FileTime last_write_time;
    Dword volume_serial_number;
    Dword file_size_high;
    Dword file_size_low;
    Dword number_of_links;
    Dword file_index_high;
    Dword file_index_low;
};

enum class FileInfoByHandleClass : int {
    FileBasicInfo,
    FileStandardInfo,
    FileNameInfo,
    FileRenameInfo,
    FileDispositionInfo,
    FileAllocationInfo,
    FileEndOfFileInfo,
    FileStreamInfo,
    FileCompressionInfo,
    FileAttributeTagInfo,
    FileIdBothDirectoryInfo,
    FileIdBothDirectoryRestartInfo,
    FileIoPriorityHintInfo,
    FileRemoteProtocolInfo,
    FileFullDirectoryInfo,
    FileFullDirectoryRestartInfo,
    FileStorageInfo,
    FileAlignmentInfo,
    FileIdInfo,
    FileIdExtdDirectoryInfo,
    FileIdExtdDirectoryRestartInfo,
    FileDispositionInfoEx,
    FileRenameInfoEx,
    FileCaseSensitiveInfo,
    FileNormalizedNameInfo,
    MaximumFileInfoByHandleClass
};

struct ProcessInformation {
    Handle process;
    Handle thread;
    Dword process_id;
    Dword thread_id;
};

struct StartupInfoA {
    Dword size;
    Char* reserved;
    Char* desktop;
    Char* title;
    Dword x;
    Dword y;
    Dword x_size;
    Dword y_size;
    Dword x_count_chars;
    Dword y_count_chars;
    Dword fill_attribute;
    Dword flags;
    Word show_window;
    Word reserved2_size;
    unsigned char* reserved2;
    Handle standard_input;
    Handle standard_output;
    Handle standard_error;
};

struct StartupInfoW {
    Dword size;
    WideChar* reserved;
    WideChar* desktop;
    WideChar* title;
    Dword x;
    Dword y;
    Dword x_size;
    Dword y_size;
    Dword x_count_chars;
    Dword y_count_chars;
    Dword fill_attribute;
    Dword flags;
    Word show_window;
    Word reserved2_size;
    unsigned char* reserved2;
    Handle standard_input;
    Handle standard_output;
    Handle standard_error;
};

struct SystemTime {
    Word year;
    Word month;
    Word day_of_week;
    Word day;
    Word hour;
    Word minute;
    Word second;
    Word milliseconds;
};

struct TimeZoneInformation {
    Long bias;
    WideChar standard_name[32];
    SystemTime standard_date;
    Long standard_bias;
    WideChar daylight_name[32];
    SystemTime daylight_date;
    Long daylight_bias;
};

struct SystemInfo {
    union {
        Dword oem_id;

        struct {
            Word processor_architecture;
            Word reserved;
        };
    };

    Dword page_size;
    void* minimum_application_address;
    void* maximum_application_address;
    ::std::size_t active_processor_mask;
    Dword number_of_processors;
    Dword processor_type;
    Dword allocation_granularity;
    Word processor_level;
    Word processor_revision;
};

struct Win32FindDataA {
    Dword file_attributes;
    FileTime creation_time;
    FileTime last_access_time;
    FileTime last_write_time;
    Dword file_size_high;
    Dword file_size_low;
    Dword reserved0;
    Dword reserved1;
    Char file_name[260];
    Char alternate_file_name[14];
};

struct Win32FindDataW {
    Dword file_attributes;
    FileTime creation_time;
    FileTime last_access_time;
    FileTime last_write_time;
    Dword file_size_high;
    Dword file_size_low;
    Dword reserved0;
    Dword reserved1;
    WideChar file_name[260];
    WideChar alternate_file_name[14];
};

struct MemoryRangeEntry {
    void* virtual_address;
    ::std::size_t number_of_bytes;
};

using Procedure = IntPtr(PLTXT2HTM_DETAIL_STDCALL*)();
using ThreadStartRoutine = Dword(PLTXT2HTM_DETAIL_STDCALL*)(void*);
using TimerApcRoutine = void(PLTXT2HTM_DETAIL_STDCALL*)(void*, Dword, Dword);
using VectoredExceptionHandler = Long(PLTXT2HTM_DETAIL_STDCALL*)(ExceptionPointers*);
} // namespace pltxt2htm::details::symbols::win32

#include "../pop_macro.hh"
