includes("../xmake/*.lua")
set_policy("check.auto_ignore_flags", false)
add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

option("python-version", function()
    set_description("Python version to download via xrepo (required, e.g. xmake f --python-version=3.12)")
end)

if has_config("python-version") then
    add_requires("python " .. get_config("python-version"), {system = false})
end

option("enable-stacktrace", function()
    set_default(false)
    set_description("Whether using C++23 <stacktrace>")
end)

if has_config("enable-stacktrace") then
    add_defines("PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE")
end

target("pltxt2htm", function()
    set_kind("shared")
    set_prefixname("")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")
    add_packages("python")
    set_exceptions("no-cxx")
    if is_plat("windows", "mingw") then add_links("ntdll") end

    local python = nil
    on_config(function(target)
        local pyver = assert(get_config("python-version"), "--python-version is required (example: xmake f --python-version=3.12)")

        local compiler = path.basename(target:tool("cxx"))
        local linker = path.basename(target:tool("ld"))
        if compiler == "clang++" or compiler == "clang" or compiler == "gcc" or
            compiler == "g++" then
            target:add("shflags", "-fPIC")
            target:add("cxxflags", "-fno-rtti")
            target:add("cxxflags", "-fno-unwind-tables")
            target:add("cxxflags", "-fno-asynchronous-unwind-tables")
            target:add("cxxflags", "-fvisibility=hidden")
            target:add("cxxflags", "-fvisibility-inlines-hidden")
            if is_mode("release") then
                target:add("cxxflags", "-fno-ident")
            end
        end
        import("lib.detect.find_tool")
        if find_tool("ld.lld") and
            (linker == "gcc" or linker == "g++" or linker == "clang++" or linker ==
                "clang") then
            target:add("shflags", "-fuse-ld=lld")
            if is_mode("release") then target:add("shflags", "-flto") end
        end

        import("core.base.global")
        for _, dir in ipairs(os.dirs(path.join(global.directory(), "packages", "p", "python", pyver .. "*", "*"))) do
            if is_host("windows") then
                local exe = path.join(dir, "python.exe")
                if os.isfile(exe) then
                    python = exe
                end
            else
                for _, name in ipairs({"python3", "python"}) do
                    local exe = path.join(dir, "bin", name)
                    if os.isfile(exe) then
                        python = exe
                        break
                    end
                end
            end
            if python then break end
        end

        assert(python and os.isfile(python), "python not found in xrepo")

        print("using xrepo python at " .. python)

        local suffix = os.iorunv(python, {
            "-c",
            "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"
        })
        if suffix and suffix ~= "None" then
            suffix = suffix:trim()
            target:set("extension", suffix)
        else
            if target:is_plat("windows", "mingw") then
                target:set("extension", ".pyd")
            else
                target:set("extension", ".so")
            end
        end
    end) -- on_config

    after_build(function(target)
        local lib_plat_cpy_dir = os.iorunv(python, {
            "-c", [[
import os, sys, sysconfig
pyver = f"{sys.version_info.major}{sys.version_info.minor}"
dest = os.path.join(sys.argv[1], "build",
    f"lib.{sysconfig.get_platform()}-cpython-{pyver}")
os.makedirs(dest, exist_ok=True)
print(dest, end='')
]], os.projectdir()
        })
        os.cp(target:targetfile(), lib_plat_cpy_dir)
        print("copying " .. target:targetfile() .. " to " .. lib_plat_cpy_dir)
    end) -- after_build
end) -- target
