#pragma once

namespace pltxt2htm::details::symbols::nt {
using Handle = void*;
using NtStatus = long;

struct SrwLock {
    void* pointer;
};
} // namespace pltxt2htm::details::symbols::nt
