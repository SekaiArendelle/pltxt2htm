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
} // namespace pltxt2htm::details::symbols::win32
