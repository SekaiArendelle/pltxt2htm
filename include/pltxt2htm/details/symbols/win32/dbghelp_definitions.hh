#pragma once

#include "definitions.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details::symbols::win32 {
// Match DbgHelp's packing even when the including file uses a different setting.
#pragma pack(push, 8)

struct Guid {
    Dword data1;
    Word data2;
    Word data3;
    unsigned char data4[8];
};

struct ModLoadData {
    Dword size_of_struct;
    Dword signature;
    void* data;
    Dword size;
    Dword flags;
};

enum class SymType : int {
    None,
    Coff,
    Cv,
    Pdb,
    Export,
    Deferred,
    Sym,
    Dia,
    Virtual,
    NumberOfTypes
};

enum class AddressMode : int {
    Mode1616,
    Mode1632,
    Real,
    Flat
};

struct Address64 {
    Dword64 offset;
    Word segment;
    AddressMode mode;
};

struct KdHelp64 {
    Dword64 thread;
    Dword thread_callback_stack;
    Dword thread_callback_backing_store;
    Dword next_callback;
    Dword frame_pointer;
    Dword64 call_user_mode;
    Dword64 user_callback_dispatcher;
    Dword64 system_range_start;
    Dword64 user_exception_dispatcher;
    Dword64 stack_base;
    Dword64 stack_limit;
    Dword build_version;
    Dword retpoline_stub_function_table_size;
    Dword64 retpoline_stub_function_table;
    Dword retpoline_stub_offset;
    Dword retpoline_stub_size;
    Dword64 reserved[2];
};

struct StackFrame64 {
    Address64 pc;
    Address64 return_address;
    Address64 frame;
    Address64 stack;
    Address64 backing_store;
    void* function_table_entry;
    Dword64 parameters[4];
    Bool far_call;
    Bool virtual_frame;
    Dword64 reserved[3];
    KdHelp64 kd_help;
};

struct SymbolInfo {
    Ulong size_of_struct;
    Ulong type_index;
    Ulong64 reserved[2];
    Ulong index;
    Ulong size;
    Ulong64 mod_base;
    Ulong flags;
    Ulong64 value;
    Ulong64 address;
    Ulong register_number;
    Ulong scope;
    Ulong tag;
    Ulong name_len;
    Ulong max_name_len;
    Char name[1];
};

struct SymbolInfoW {
    Ulong size_of_struct;
    Ulong type_index;
    Ulong64 reserved[2];
    Ulong index;
    Ulong size;
    Ulong64 mod_base;
    Ulong flags;
    Ulong64 value;
    Ulong64 address;
    Ulong register_number;
    Ulong scope;
    Ulong tag;
    Ulong name_len;
    Ulong max_name_len;
    WideChar name[1];
};

struct ImageHlpLine64 {
    Dword size_of_struct;
    void* key;
    Dword line_number;
    Char* file_name;
    Dword64 address;
};

struct ImageHlpLineW64 {
    Dword size_of_struct;
    void* key;
    Dword line_number;
    WideChar* file_name;
    Dword64 address;
};

struct ImageHlpModule64 {
    Dword size_of_struct;
    Dword64 base_of_image;
    Dword image_size;
    Dword time_date_stamp;
    Dword check_sum;
    Dword number_of_symbols;
    SymType symbol_type;
    Char module_name[32];
    Char image_name[256];
    Char loaded_image_name[256];
    Char loaded_pdb_name[256];
    Dword cv_signature;
    Char cv_data[260 * 3];
    Dword pdb_signature;
    Guid pdb_signature70;
    Dword pdb_age;
    Bool pdb_unmatched;
    Bool dbg_unmatched;
    Bool line_numbers;
    Bool global_symbols;
    Bool type_info;
    Bool source_indexed;
    Bool publics;
    Dword machine_type;
    Dword reserved;
};

struct ImageHlpModuleW64 {
    Dword size_of_struct;
    Dword64 base_of_image;
    Dword image_size;
    Dword time_date_stamp;
    Dword check_sum;
    Dword number_of_symbols;
    SymType symbol_type;
    WideChar module_name[32];
    WideChar image_name[256];
    WideChar loaded_image_name[256];
    WideChar loaded_pdb_name[256];
    Dword cv_signature;
    WideChar cv_data[260 * 3];
    Dword pdb_signature;
    Guid pdb_signature70;
    Dword pdb_age;
    Bool pdb_unmatched;
    Bool dbg_unmatched;
    Bool line_numbers;
    Bool global_symbols;
    Bool type_info;
    Bool source_indexed;
    Bool publics;
    Dword machine_type;
    Dword reserved;
};

#pragma pack(pop)

using SymEnumModulesCallback64 = Bool(PLTXT2HTM_DETAIL_STDCALL*)(Char const*, Dword64, void*);
using SymEnumModulesCallbackW64 = Bool(PLTXT2HTM_DETAIL_STDCALL*)(WideChar const*, Dword64, void*);
using SymEnumSymbolsCallback = Bool(PLTXT2HTM_DETAIL_STDCALL*)(SymbolInfo*, Ulong, void*);
using SymEnumSymbolsCallbackW = Bool(PLTXT2HTM_DETAIL_STDCALL*)(SymbolInfoW*, Ulong, void*);
using SymbolRegisteredCallback64 = Bool(PLTXT2HTM_DETAIL_STDCALL*)(Handle, Ulong, Ulong64, Ulong64);
using ReadProcessMemoryRoutine64 = Bool(PLTXT2HTM_DETAIL_STDCALL*)(Handle, Dword64, void*, Dword, Dword*);
using FunctionTableAccessRoutine64 = void*(PLTXT2HTM_DETAIL_STDCALL*)(Handle, Dword64);
using GetModuleBaseRoutine64 = Dword64(PLTXT2HTM_DETAIL_STDCALL*)(Handle, Dword64);
using TranslateAddressRoutine64 = Dword64(PLTXT2HTM_DETAIL_STDCALL*)(Handle, Handle, Address64*);
} // namespace pltxt2htm::details::symbols::win32

#include "../pop_macro.hh"
