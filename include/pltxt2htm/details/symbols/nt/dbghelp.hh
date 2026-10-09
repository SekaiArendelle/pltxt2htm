#pragma once

#include <cstddef>
#include <cstdint>

#if defined(_MSC_VER)
    #define PLTXT2HTM_NT_CALL __stdcall
#elif defined(__i386__)
    #define PLTXT2HTM_NT_CALL __attribute__((stdcall))
#else
    #define PLTXT2HTM_NT_CALL
#endif

namespace pltxt2htm::details::symbols::nt {
// DbgHelp's documented MAX_SYM_NAME scratch capacity.
constexpr ::std::size_t max_symbol_name_length{2000};

// DbgHelp structures use the SDK's default packing even when a consumer has
// changed the packing around its includes.
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

using SymInitialize = int(PLTXT2HTM_NT_CALL*)(void*, char const*, int);
using SymRefreshModuleList = int(PLTXT2HTM_NT_CALL*)(void*);
using SymCleanup = int(PLTXT2HTM_NT_CALL*)(void*);
using SymFromAddr = int(PLTXT2HTM_NT_CALL*)(void*, ::std::uint64_t, ::std::uint64_t*, SymbolInfo*);
using SymGetLineFromAddr64 = int(PLTXT2HTM_NT_CALL*)(void*, ::std::uint64_t, unsigned long*, ImageHlpLine64*);
} // namespace pltxt2htm::details::symbols::nt

#undef PLTXT2HTM_NT_CALL
