set_policy("check.auto_ignore_flags", false)

includes("../xmake/*.lua")

set_allowedmodes("debug", "release")
add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

option("enable-stacktrace", function()
    set_default(false)
    set_description("Whether using C++23 <stacktrace>")
end)

if has_config("enable-stacktrace") then
    add_defines("PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE")
end

target("pltxt2htm", function()
    set_kind("binary")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")

    if is_plat("windows", "mingw") then add_syslinks("ntdll") end

    if is_plat("wasm") then set_extension(".wasm") end

    on_config(function(target)
        local compiler = path.basename(target:tool("cxx"))
        local linker = path.basename(target:tool("ld"))

        import("lib.detect.find_tool")
        if find_tool("ld.lld") and
            (linker == "clang" or linker == "clang++" or linker == "gcc" or
                linker == "g++") then
            target:add("ldflags", "-fuse-ld=lld")
        end
        if compiler == "clang++" or compiler == "clang" then
            if is_mode("release") then target:add("ldflags", "-flto") end
        end

        if compiler:endswith("gcc") or compiler:endswith("g++") or
            compiler:endswith("clang") or compiler:endswith("clang++") then
            if is_mode("release") then
                target:set("exceptions", "no-cxx")
                target:add("cxxflags", "-fno-rtti")
                target:add("cxxflags", "-fno-unwind-tables")
                target:add("cxxflags", "-fno-asynchronous-unwind-tables")
                target:add("cxxflags", "-fno-ident")
            elseif is_mode("debug") then
                target:add("cxxflags", "-Wall")
                target:add("cxxflags", "-Wextra")
                target:add("cxxflags", "-fexceptions")
            end
        end
    end)

    before_build(function(target)
        local file = io.open("$(projectdir)/repo_info.ignore", "w")

        git_status = os.iorunv("git", {"status", "--porcelain"})
        if git_status ~= "" then
            git_status = os.iorunv("git", {"status"})
            file:write("\"* repo status: UNCOMMITED\\n",
                       git_status:gsub("\n", "\\n"):gsub("\"", "\\\""), "\"")
            file:close()
            return
        end

        -- all edits in git workspace is commited
        git_commit_hash = os.iorunv("git", {"rev-parse", "HEAD"})
        if file then
            file:write("\"* build commit: ", git_commit_hash:sub(1, -2), "\\n\"")
            file:close()
        end
    end)

    after_build(function(target)
        if os.exists("$(projectdir)/repo_info.ignore") and
            os.isfile("$(projectdir)/repo_info.ignore") then
            os.rm("$(projectdir)/repo_info.ignore")
        end
    end)

    on_install(function(target)
        local install_dir = target:installdir()
        if not os.exists(install_dir) or not os.isdir(install_dir) then
            os.mkdir(install_dir)
        end
        os.cp(target:targetfile(), install_dir)
        print("install to " .. path.join(target:scriptdir(), install_dir))
    end)
end)
