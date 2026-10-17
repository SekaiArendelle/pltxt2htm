#pragma once

#include <elfutils/libdwfl.h>
#include <memory>
#include <unistd.h>

namespace pltxt2htm::details::stacktrace {
/** Per-resolution session: sessions never share mutable libdwfl state. */
class DwflSession {
    ::Dwfl_Callbacks callbacks_{.find_elf = ::dwfl_linux_proc_find_elf,
                                .find_debuginfo = ::dwfl_standard_find_debuginfo,
                                .section_address = nullptr,
                                .debuginfo_path = nullptr};

public:
    ::Dwfl* handle{};

    DwflSession() noexcept {
        handle = ::dwfl_begin(::std::addressof(callbacks_));
        if (handle == nullptr) {
            return;
        }
        if (::dwfl_linux_proc_report(handle, ::getpid()) != 0 || ::dwfl_report_end(handle, nullptr, nullptr) != 0) {
            ::dwfl_end(handle);
            handle = nullptr;
        }
    }

    DwflSession(DwflSession const&) = delete;
    DwflSession(DwflSession&&) = delete;
    DwflSession& operator=(DwflSession const&) = delete;
    DwflSession& operator=(DwflSession&&) = delete;

    constexpr ~DwflSession() {
        if (handle != nullptr) {
            ::dwfl_end(handle);
        }
    }
};

} // namespace pltxt2htm::details::stacktrace
