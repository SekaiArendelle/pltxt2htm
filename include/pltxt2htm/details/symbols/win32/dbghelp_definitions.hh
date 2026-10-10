#pragma once

#include <cstddef>
#include "definitions.hh"

namespace pltxt2htm::details::symbols::win32 {
constexpr ::std::size_t max_symbol_name_length{2000};

#pragma pack(push, 8)

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

struct ImageHlpLine64 {
    Dword size_of_struct;
    void* key;
    Dword line_number;
    Char* file_name;
    Dword64 address;
};

#pragma pack(pop)
} // namespace pltxt2htm::details::symbols::win32
