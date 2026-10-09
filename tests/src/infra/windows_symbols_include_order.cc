#if defined(_WIN32)
    #include <pltxt2htm/details/symbols/nt/ntdll.hh>
    #include <pltxt2htm/details/symbols/win32/dbghelp.hh>
    #include <pltxt2htm/details/symbols/win32/kernel32.hh>

    #include <windows.h>
    #include <dbghelp.h>
    #include <cstddef>

namespace nt = ::pltxt2htm::details::symbols::nt;
namespace win32 = ::pltxt2htm::details::symbols::win32;

static_assert(sizeof(nt::SrwLock) == sizeof(SRWLOCK));
static_assert(sizeof(win32::SymbolInfo) == sizeof(SYMBOL_INFO));
static_assert(offsetof(win32::SymbolInfo, name) == offsetof(SYMBOL_INFO, Name));

auto* volatile nt_close_include_order_reference = &nt::pltxt2htm_nt_close;
auto* volatile load_library_include_order_reference = &win32::pltxt2htm_win32_load_library_ex_w;
#endif

int main() {
}
