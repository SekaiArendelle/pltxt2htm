includes("../xmake/*.lua")

add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

option("py_bin_dir")
option("py_include_dir")
option("py_lib_dir")

-- function is_valid_py_lib_dir(dir)
--     for  _, a_file in ipairs(os.files(lib .. "/libpython*.so")) do
--         return true
--     end
--     return false
-- end

target("pltxt2htm", function ()
    set_kind("shared")
    set_prefixname("")
    add_files("pltxt2htm.cc")
    add_includedirs("../include")
    set_exceptions("no-cxx")
    add_cxxflags("-fno-rtti", {tools = {"clangxx", "gcc"}})
    add_cxxflags("-fno-unwind-tables")
    add_cxxflags("-fno-asynchronous-unwind-tables")
    if is_mode("release") then
        add_cxflags("-fno-ident")
    end
    add_cxxflags("-fvisibility=hidden", {tools = {"clangxx", "gcc"}})
    if is_plat("windows", "mingw") then
        add_links("ntdll")
    end

    local py_bin_dir = nil
    on_config(function (target)
        local toolchains = target:tool("cxx")
        if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
            target:add("shflags", "-fuse-ld=lld")
        end

        import("lib.detect.find_tool")

        -- if python install dir have been passed in console
        py_bin_dir = get_config("py_bin_dir")
        local py_include_dir = get_config("py_include_dir")
        local py_lib_dir = get_config("py_lib_dir")

        if not py_bin_dir then
            py_bin_dir = path.directory(find_tool("python").program)
            if not py_bin_dir then
                py_bin_dir = path.directory(find_tool("python3").program)
            end

            if not py_bin_dir then
                print("error: python3 or python not found")
                os.exit(1)
            else
                print("detecting for python bin dir .. " .. py_bin_dir)
            end
        end

        local py_install_dir = py_bin_dir

        if is_plat("windows", "mingw") then
            if not py_include_dir then
                py_include_dir = py_install_dir .. "/include"
            end
            if not py_lib_dir then
                if is_plat("mingw") then
                    py_lib_dir = py_install_dir
                elseif is_plat("windows") then
                    py_lib_dir = py_install_dir .. "/libs"
                else
                    print("unreachable code touched, please bug report")
                    os.exit(1)
                end
            end
        elseif is_plat("linux") then
            local current_env = os.getenvs()
            local sys_usr_dir = nil
            if current_env["TERMUX__PREFIX"] ~= nil then
                -- usr dir in termux
                sys_usr_dir = current_env["TERMUX__PREFIX"]
            else
                -- usr dir in normal linux distribution
                sys_usr_dir = "/usr"
            end

            if py_install_dir == sys_usr_dir .. "/bin" or py_install_dir == sys_usr_dir .. "/sbin" then
                -- detect python in system path
                if not py_include_dir then
                    for _, a_dir in ipairs(os.dirs(sys_usr_dir .. "/include/*")) do
                        -- detecting python header in system include dir
                        if path.basename(a_dir) == "python3" then
                            py_include_dir = a_dir
                        end
                    end
                end
                if not py_lib_dir then
                    py_lib_dir = sys_usr_dir .. "/lib"
                end

                local is_valid_py_lib_dir = false
                for  _, a_file in ipairs(os.files(py_lib_dir .. "/libpython*.so")) do
                    is_valid_py_lib_dir = true
                end
                if not is_valid_py_lib_dir then
                    py_lib_dir = sys_usr_dir .. "/lib/x86_64-linux-gnu"
                end
            else
                -- detect python in another install path
                py_install_dir = path.directory(py_install_dir)
                if not py_include_dir then
                    for _, a_dir in ipairs(os.dirs(py_install_dir .. "/include/*")) do
                        -- detecting python header in system include dir
                        if path.basename(a_dir) == "python3" then
                            py_include_dir = a_dir
                        end
                    end
                end
                if not py_lib_dir then
                    py_lib_dir = py_install_dir .. "/lib"
                end
            end
        else
            print("unreachable code touched, please bug report")
            os.exit(1)
        end

        if not py_include_dir then
            print("error: detect python include dir failed")
            os.exit(1)
        end
        if not py_lib_dir then
            print("error: detect python lib dir failed")
            os.exit(1)
        end
        if not os.exists(py_include_dir .. "/Python.h") then
            print("error: Python.h not found in " .. py_include_dir)
            os.exit(1)
        end

        print("detecting for python include dir .. " .. py_include_dir)
        if is_plat("mingw") then
            if not os.exists(py_lib_dir .. "/python3.dll") then
                print("error: python3.dll not found in " .. py_lib_dir)
                os.exit(1)
            end
        elseif is_plat("windows") then
            if not os.exists(py_lib_dir .. "/python3.lib") then
                print("error: python3.lib not found in " .. py_lib_dir)
                os.exit(1)
            end
        elseif is_plat("linux") then
            local is_valid_py_lib_dir = false
            for  _, a_file in ipairs(os.files(py_lib_dir .. "/libpython*.so")) do
                is_valid_py_lib_dir = true
            end
            if not is_valid_py_lib_dir then
                print("error: libpython3 not found in " .. py_lib_dir)
                os.exit(1)
            end
        else
            print("unreachable code touched, please bug report")
            os.exit(1)
        end
        print("detecting for python lib dir .. " .. py_lib_dir)

        local suffix = nil
        if is_plat("windows", "mingw") then
            suffix = os.iorunv(py_bin_dir .. "\\python.exe", {"-c", "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"})
        elseif is_plat("linux") then
            suffix = os.iorunv(py_bin_dir .. "/python3", {"-c", "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"})
        else
            print("unreachable code touched, please bug report")
            os.exit(1)
        end
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

        target:add("includedirs", py_include_dir)
        target:add("linkdirs", py_lib_dir)
        if is_plat("windows", "mingw") then
            for  _, a_file in ipairs(os.files(py_lib_dir .. "/python*.dll")) do
                target:add("links", path.basename(a_file))
            end
        else
            for  _, a_file in ipairs(os.files(py_lib_dir .. "/libpython*.so")) do
                target:add("links", string.sub(path.basename(a_file), 4))
            end
        end
    end) -- on_config

    after_build(function (target)
        if is_plat("windows", "mingw") then
            os.iorunv(py_bin_dir .. "\\python.exe", {"after_build.py"})
        elseif is_plat("linux") then
            os.iorunv(py_bin_dir .. "/python3", {"after_build.py"})
        else
            print("unreachable code touched, please bug report")
            os.exit(1)
        end
        os.cp(target:targetfile(), target:scriptdir() .. "/build/")
        print("copying " .. target:targetfile() .. " to " .. target:scriptdir() .. "/build/")
    end) -- after_build
end) -- target
