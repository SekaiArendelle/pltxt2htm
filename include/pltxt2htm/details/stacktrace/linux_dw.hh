#pragma once

#include <dlfcn.h>
#include <memory>
#include <unistd.h>
#include "../symbols/linux/libdw.hh"
#include "entry.hh"

namespace pltxt2htm::details::stacktrace {
/** Per-resolution library ownership: sessions never share mutable libdw state. */
class LibdwApi {
public:
    void* library{};
    ::pltxt2htm::details::symbols::linux_dw::Begin begin{};
    ::pltxt2htm::details::symbols::linux_dw::End end{};
    ::pltxt2htm::details::symbols::linux_dw::ProcReport proc_report{};
    ::pltxt2htm::details::symbols::linux_dw::ReportEnd report_end{};
    ::pltxt2htm::details::symbols::linux_dw::AddrModule addr_module{};
    ::pltxt2htm::details::symbols::linux_dw::ModuleInfo module_info{};
    ::pltxt2htm::details::symbols::linux_dw::AddrInfo addr_info{};
    ::pltxt2htm::details::symbols::linux_dw::GetSource get_source{};
    ::pltxt2htm::details::symbols::linux_dw::LineInfo line_info{};
    ::pltxt2htm::details::symbols::linux_dw::CompDir comp_dir{};
    ::pltxt2htm::details::symbols::linux_dw::Callbacks callbacks{};

    template<typename Function>
    [[nodiscard]] Function load(char const* name) noexcept {
        return reinterpret_cast<Function>(::dlsym(library, name));
    }

    explicit LibdwApi(char const* name = "libdw.so.1") noexcept {
        library = ::dlopen(name, RTLD_LAZY | RTLD_LOCAL);
        if (library == nullptr) {
            return;
        }
        namespace dw = ::pltxt2htm::details::symbols::linux_dw;
        begin = load<dw::Begin>("dwfl_begin");
        end = load<dw::End>("dwfl_end");
        proc_report = load<dw::ProcReport>("dwfl_linux_proc_report");
        report_end = load<dw::ReportEnd>("dwfl_report_end");
        addr_module = load<dw::AddrModule>("dwfl_addrmodule");
        module_info = load<dw::ModuleInfo>("dwfl_module_info");
        addr_info = load<dw::AddrInfo>("dwfl_module_addrinfo");
        get_source = load<dw::GetSource>("dwfl_module_getsrc");
        line_info = load<dw::LineInfo>("dwfl_lineinfo");
        comp_dir = load<dw::CompDir>("dwfl_line_comp_dir");
        callbacks.find_elf = load<dw::FindElf>("dwfl_linux_proc_find_elf");
        callbacks.find_debuginfo = load<dw::FindDebugInfo>("dwfl_standard_find_debuginfo");
    }

    LibdwApi(LibdwApi const&) = delete;
    LibdwApi(LibdwApi&&) = delete;
    LibdwApi& operator=(LibdwApi const&) = delete;
    LibdwApi& operator=(LibdwApi&&) = delete;

    constexpr ~LibdwApi() {
        if (library != nullptr) {
            (void)::dlclose(library);
        }
    }

    [[nodiscard]] constexpr bool ready(this LibdwApi const& self) noexcept {
        return self.begin != nullptr && self.end != nullptr && self.proc_report != nullptr &&
               self.report_end != nullptr && self.addr_module != nullptr && self.module_info != nullptr &&
               self.addr_info != nullptr && self.get_source != nullptr && self.line_info != nullptr &&
               self.comp_dir != nullptr && self.callbacks.find_elf != nullptr &&
               self.callbacks.find_debuginfo != nullptr;
    }
};

class DwflSession {
    LibdwApi& api_;

public:
    ::pltxt2htm::details::symbols::linux_dw::Dwfl* handle{};

    explicit DwflSession(LibdwApi& api) noexcept
        : api_{api} {
        if (!api.ready()) {
            return;
        }
        handle = api.begin(::std::addressof(api.callbacks));
        if (handle == nullptr) {
            return;
        }
        if (api.proc_report(handle, ::getpid()) != 0 || api.report_end(handle, nullptr, nullptr) != 0) {
            api.end(handle);
            handle = nullptr;
        }
    }

    DwflSession(DwflSession const&) = delete;
    DwflSession(DwflSession&&) = delete;
    DwflSession& operator=(DwflSession const&) = delete;
    DwflSession& operator=(DwflSession&&) = delete;

    constexpr ~DwflSession() {
        if (handle != nullptr) {
            api_.end(handle);
        }
    }
};

} // namespace pltxt2htm::details::stacktrace
