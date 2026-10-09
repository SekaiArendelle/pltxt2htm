#if defined(_WIN32)
    #include <windows.h>
    #include <dbghelp.h>
    #include <cstddef>
    #include <pltxt2htm/details/symbols/nt/dbghelp.hh>
    #include <pltxt2htm/details/symbols/nt/ntdll.hh>
    #include <pltxt2htm/details/symbols/nt/kernel32.hh>
    #include <type_traits>

static_assert(sizeof(unsigned long) == sizeof(DWORD));
static_assert(sizeof(unsigned short) == sizeof(USHORT));
static_assert(sizeof(void*) == sizeof(SRWLOCK));
namespace nt = ::pltxt2htm::details::symbols::nt;
static_assert(::std::is_same_v<nt::SymInitialize, decltype(&::SymInitialize)>);
static_assert(::std::is_same_v<nt::SymCleanup, decltype(&::SymCleanup)>);
static_assert(sizeof(nt::SymbolInfo) == sizeof(SYMBOL_INFO));
static_assert(alignof(nt::SymbolInfo) == alignof(SYMBOL_INFO));
static_assert(offsetof(nt::SymbolInfo, name) == offsetof(SYMBOL_INFO, Name));
static_assert(offsetof(nt::SymbolInfo, value) == offsetof(SYMBOL_INFO, Value));
static_assert(offsetof(nt::SymbolInfo, mod_base) == offsetof(SYMBOL_INFO, ModBase));
static_assert(sizeof(nt::ImageHlpLine64) == sizeof(IMAGEHLP_LINE64));
static_assert(alignof(nt::ImageHlpLine64) == alignof(IMAGEHLP_LINE64));
static_assert(offsetof(nt::ImageHlpLine64, file_name) == offsetof(IMAGEHLP_LINE64, FileName));
static_assert(offsetof(nt::ImageHlpLine64, address) == offsetof(IMAGEHLP_LINE64, Address));
static_assert(offsetof(nt::SymbolInfo, size_of_struct) == offsetof(SYMBOL_INFO, SizeOfStruct));
static_assert(offsetof(nt::SymbolInfo, type_index) == offsetof(SYMBOL_INFO, TypeIndex));
static_assert(offsetof(nt::SymbolInfo, reserved) == offsetof(SYMBOL_INFO, Reserved));
    #if defined(__MINGW32__)
// MinGW names the SDK's Index field "info".
static_assert(offsetof(nt::SymbolInfo, index) == offsetof(SYMBOL_INFO, info));
    #else
static_assert(offsetof(nt::SymbolInfo, index) == offsetof(SYMBOL_INFO, Index));
    #endif
static_assert(offsetof(nt::SymbolInfo, size) == offsetof(SYMBOL_INFO, Size));
static_assert(offsetof(nt::SymbolInfo, flags) == offsetof(SYMBOL_INFO, Flags));
static_assert(offsetof(nt::SymbolInfo, address) == offsetof(SYMBOL_INFO, Address));
static_assert(offsetof(nt::SymbolInfo, register_number) == offsetof(SYMBOL_INFO, Register));
static_assert(offsetof(nt::SymbolInfo, scope) == offsetof(SYMBOL_INFO, Scope));
static_assert(offsetof(nt::SymbolInfo, tag) == offsetof(SYMBOL_INFO, Tag));
static_assert(offsetof(nt::SymbolInfo, name_len) == offsetof(SYMBOL_INFO, NameLen));
static_assert(offsetof(nt::SymbolInfo, max_name_len) == offsetof(SYMBOL_INFO, MaxNameLen));
static_assert(offsetof(nt::ImageHlpLine64, size_of_struct) == offsetof(IMAGEHLP_LINE64, SizeOfStruct));
static_assert(offsetof(nt::ImageHlpLine64, key) == offsetof(IMAGEHLP_LINE64, Key));
static_assert(offsetof(nt::ImageHlpLine64, line_number) == offsetof(IMAGEHLP_LINE64, LineNumber));
#endif

#if defined(__linux__) && __has_include(<elfutils/libdwfl.h>)
    #include <pltxt2htm/details/symbols/linux/libdw.hh>
    #include <type_traits>
    #include <cstddef>
namespace dw = ::pltxt2htm::details::symbols::linux_dw;
static_assert(sizeof(dw::Callbacks) == sizeof(Dwfl_Callbacks));
static_assert(alignof(dw::Callbacks) == alignof(Dwfl_Callbacks));
static_assert(offsetof(dw::Callbacks, find_elf) == offsetof(Dwfl_Callbacks, find_elf));
static_assert(offsetof(dw::Callbacks, find_debuginfo) == offsetof(Dwfl_Callbacks, find_debuginfo));
static_assert(offsetof(dw::Callbacks, section_address) == offsetof(Dwfl_Callbacks, section_address));
static_assert(offsetof(dw::Callbacks, debuginfo_path) == offsetof(Dwfl_Callbacks, debuginfo_path));
static_assert(::std::is_same_v<dw::FindElf, decltype(Dwfl_Callbacks::find_elf)>);
static_assert(::std::is_same_v<dw::FindDebugInfo, decltype(Dwfl_Callbacks::find_debuginfo)>);
static_assert(::std::is_same_v<dw::SectionAddress, decltype(Dwfl_Callbacks::section_address)>);
static_assert(::std::is_same_v<dw::End, decltype(&::dwfl_end)>);
static_assert(::std::is_same_v<dw::ProcReport, decltype(&::dwfl_linux_proc_report)>);
static_assert(::std::is_same_v<dw::ReportEnd, decltype(&::dwfl_report_end)>);
static_assert(::std::is_same_v<dw::AddrModule, decltype(&::dwfl_addrmodule)>);
static_assert(::std::is_same_v<dw::ModuleInfo, decltype(&::dwfl_module_info)>);
static_assert(::std::is_same_v<dw::AddrInfo, decltype(&::dwfl_module_addrinfo)>);
static_assert(::std::is_same_v<dw::GetSource, decltype(&::dwfl_module_getsrc)>);
static_assert(::std::is_same_v<dw::LineInfo, decltype(&::dwfl_lineinfo)>);
static_assert(::std::is_same_v<dw::CompDir, decltype(&::dwfl_line_comp_dir)>);
#endif

int main() {
}
