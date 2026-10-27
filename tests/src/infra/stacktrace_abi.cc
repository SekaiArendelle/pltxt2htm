#if defined(_WIN32)
    #include <windows.h>
    #include <dbghelp.h>
    #include <cstddef>
    #include <pltxt2htm/sys/win32/dbghelp.hh>
    #include <pltxt2htm/sys/win32/kernel32.hh>
    #include <type_traits>

static_assert(sizeof(unsigned long) == sizeof(DWORD));
static_assert(sizeof(unsigned short) == sizeof(USHORT));
static_assert(sizeof(void*) == sizeof(SRWLOCK));
namespace win32 = ::pltxt2htm::sys_win32;
static_assert(sizeof(win32::SymbolInfo) == sizeof(SYMBOL_INFO));
static_assert(alignof(win32::SymbolInfo) == alignof(SYMBOL_INFO));
static_assert(offsetof(win32::SymbolInfo, name) == offsetof(SYMBOL_INFO, Name));
static_assert(offsetof(win32::SymbolInfo, value) == offsetof(SYMBOL_INFO, Value));
static_assert(offsetof(win32::SymbolInfo, mod_base) == offsetof(SYMBOL_INFO, ModBase));
static_assert(sizeof(win32::ImageHlpLine64) == sizeof(IMAGEHLP_LINE64));
static_assert(alignof(win32::ImageHlpLine64) == alignof(IMAGEHLP_LINE64));
static_assert(offsetof(win32::ImageHlpLine64, file_name) == offsetof(IMAGEHLP_LINE64, FileName));
static_assert(offsetof(win32::ImageHlpLine64, address) == offsetof(IMAGEHLP_LINE64, Address));
static_assert(offsetof(win32::SymbolInfo, size_of_struct) == offsetof(SYMBOL_INFO, SizeOfStruct));
static_assert(offsetof(win32::SymbolInfo, type_index) == offsetof(SYMBOL_INFO, TypeIndex));
static_assert(offsetof(win32::SymbolInfo, reserved) == offsetof(SYMBOL_INFO, Reserved));
    #if defined(__MINGW32__)
// MinGW names the SDK's Index field "info".
static_assert(offsetof(win32::SymbolInfo, index) == offsetof(SYMBOL_INFO, info));
    #else
static_assert(offsetof(win32::SymbolInfo, index) == offsetof(SYMBOL_INFO, Index));
    #endif
static_assert(offsetof(win32::SymbolInfo, size) == offsetof(SYMBOL_INFO, Size));
static_assert(offsetof(win32::SymbolInfo, flags) == offsetof(SYMBOL_INFO, Flags));
static_assert(offsetof(win32::SymbolInfo, address) == offsetof(SYMBOL_INFO, Address));
static_assert(offsetof(win32::SymbolInfo, register_number) == offsetof(SYMBOL_INFO, Register));
static_assert(offsetof(win32::SymbolInfo, scope) == offsetof(SYMBOL_INFO, Scope));
static_assert(offsetof(win32::SymbolInfo, tag) == offsetof(SYMBOL_INFO, Tag));
static_assert(offsetof(win32::SymbolInfo, name_len) == offsetof(SYMBOL_INFO, NameLen));
static_assert(offsetof(win32::SymbolInfo, max_name_len) == offsetof(SYMBOL_INFO, MaxNameLen));
static_assert(offsetof(win32::ImageHlpLine64, size_of_struct) == offsetof(IMAGEHLP_LINE64, SizeOfStruct));
static_assert(offsetof(win32::ImageHlpLine64, key) == offsetof(IMAGEHLP_LINE64, Key));
static_assert(offsetof(win32::ImageHlpLine64, line_number) == offsetof(IMAGEHLP_LINE64, LineNumber));
#endif

int main() {
}
