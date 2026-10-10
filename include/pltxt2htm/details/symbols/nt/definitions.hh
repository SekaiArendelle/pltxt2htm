#pragma once

namespace pltxt2htm::details::symbols::nt {
using Boolean = unsigned char;
using Handle = void*;
using NtStatus = long;
using Ulong = unsigned long;
using Ushort = unsigned short;

struct SrwLock {
    void* pointer;
};
} // namespace pltxt2htm::details::symbols::nt
