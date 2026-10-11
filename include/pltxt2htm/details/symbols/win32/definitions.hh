#pragma once

#include <cstdint>

namespace pltxt2htm::details::symbols::win32 {
using Bool = int;
using Char = char;
using Dword = unsigned long;
using Dword64 = unsigned long long;
using Handle = void*;
using IntPtr = ::std::intptr_t;
// Keep module handles opaque instead of importing the SDK's global HINSTANCE__ tag.
using ModuleHandle = void*;
using Ulong = unsigned long;
using Ulong64 = unsigned long long;
using WideChar = wchar_t;
#if defined(_MSC_VER)
using Procedure = IntPtr(__stdcall*)();
#elif defined(__i386__)
using Procedure = IntPtr(__attribute__((stdcall)) *)();
#else
using Procedure = IntPtr (*)();
#endif
} // namespace pltxt2htm::details::symbols::win32
