#pragma once

#include <elf.h>
#include <sys/types.h>
#if __has_include(<elfutils/libdwfl.h>)
    #include <elfutils/libdwfl.h>
#endif

namespace pltxt2htm::details::symbols::linux_dw {
// Minimal public libdwfl ABI for optional runtime loading. Development headers
// are not required. GElf and Dwarf address/word types use ELF64 widths on ELF32
// hosts too. When headers exist, use their opaque types directly for ABI checks.
#if __has_include(<elfutils/libdwfl.h>)
using Dwfl = ::Dwfl;
using Module = ::Dwfl_Module;
using Line = ::Dwfl_Line;
using Elf = ::Elf;
#else
struct Dwfl;
struct Module;
struct Line;
struct Elf;
#endif

using FindElf = int (*)(Module*, void**, char const*, Elf64_Addr, char**, Elf**);
using FindDebugInfo = int (*)(Module*, void**, char const*, Elf64_Addr, char const*, char const*, Elf64_Word, char**);
using SectionAddress = int (*)(Module*, void**, char const*, Elf64_Addr, char const*, Elf64_Word, Elf64_Shdr const*,
                               Elf64_Addr*);

struct Callbacks {
    FindElf find_elf{};
    FindDebugInfo find_debuginfo{};
    SectionAddress section_address{};
    char** debuginfo_path{};
};

using Begin = Dwfl* (*)(Callbacks const*);
using End = void (*)(Dwfl*);
using ProcReport = int (*)(Dwfl*, pid_t);
using Removed = int (*)(Module*, void*, char const*, Elf64_Addr, void*);
using ReportEnd = int (*)(Dwfl*, Removed, void*);
using AddrModule = Module* (*)(Dwfl*, Elf64_Addr);
using ModuleInfo = char const* (*)(Module*, void***, Elf64_Addr*, Elf64_Addr*, Elf64_Addr*, Elf64_Addr*, char const**,
                                   char const**);
using AddrInfo = char const* (*)(Module*, Elf64_Addr, Elf64_Off*, Elf64_Sym*, Elf64_Word*, Elf**, Elf64_Addr*);
using GetSource = Line* (*)(Module*, Elf64_Addr);
using LineInfo = char const* (*)(Line*, Elf64_Addr*, int*, int*, Elf64_Xword*, Elf64_Xword*);
using CompDir = char const* (*)(Line*);
} // namespace pltxt2htm::details::symbols::linux_dw
