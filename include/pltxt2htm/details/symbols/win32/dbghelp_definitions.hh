#pragma once

#include <cstddef>
#include <cstdint>

namespace pltxt2htm::details::symbols::win32 {
constexpr ::std::size_t max_symbol_name_length{2000};

#pragma pack(push, 8)

struct SymbolInfo {
    unsigned long size_of_struct;
    unsigned long type_index;
    ::std::uint64_t reserved[2];
    unsigned long index;
    unsigned long size;
    ::std::uint64_t mod_base;
    unsigned long flags;
    ::std::uint64_t value;
    ::std::uint64_t address;
    unsigned long register_number;
    unsigned long scope;
    unsigned long tag;
    unsigned long name_len;
    unsigned long max_name_len;
    char name[1];
};

struct ImageHlpLine64 {
    unsigned long size_of_struct;
    void* key;
    unsigned long line_number;
    char* file_name;
    ::std::uint64_t address;
};

#pragma pack(pop)
} // namespace pltxt2htm::details::symbols::win32
