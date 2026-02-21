includes("../xmake/*.lua")

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

option("python")

target("pltxt2htm", function()
    set_kind("shared")
    set_prefixname("")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")
    set_exceptions("no-cxx")
    if is_plat("windows", "mingw") then add_links("ntdll") end

    local python = nil
    on_config(function(target)
        local compiler = path.basename(target:tool("cxx"))
        local linker = path.basename(target:tool("ld"))
        if compiler == "clang++" or compiler == "clang" or compiler == "gcc" or
            compiler == "g++" then
            target:add("cxxflags", "-fno-rtti")
            target:add("cxxflags", "-fPIC")
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

        -- if python install dir have been passed in console
        python = get_config("python")

        if not python then
            import("lib.detect.find_tool")
            python = find_tool("python").program
            if not python then python = find_tool("python3").program end

            if not python then
                print("error: python or python3 not found")
                os.exit(1)
            else
                print("detecting for python executable .. " .. python)
            end
        end

        if not os.exists(python) or not os.isfile(python) then
            print("error: invalid python executable path")
            os.exit(1)
        end

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

        local py_include_dir = os.iorunv(python, {
            "-c",
            "import sysconfig; print(sysconfig.get_path('include'), end='')"
        })
        target:add("includedirs", py_include_dir)

        if is_plat("windows") or is_plat("mingw") then
            target:add("linkdirs", py_include_dir .. "/../libs")
            local py_rt_path = os.iorunv(python, {
                "-c",
                "import sys; print(f'python{sys.version_info.major}{sys.version_info.minor}', end='')"
            })
            target:add("links", py_rt_path)
        else
            local python3_config = os.iorunv(python, {
                "-c",
                "import sys, sysconfig; print(f'python3.{sys.version_info.minor}-config', end='')"
            })
            local ldflags = os.iorunv(python3_config, {"--ldflags", "--embed"})
            target:add("shflags", ldflags)
        end
    end) -- on_config

    after_build(function(target)
        local lib_plat_cpy_dir = os.iorunv(python, {"after_build.py"})
        os.cp(target:targetfile(), lib_plat_cpy_dir)
        print("copying " .. target:targetfile() .. " to " .. lib_plat_cpy_dir)
    end) -- after_build
end) -- target
