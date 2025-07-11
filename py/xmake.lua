includes("../xmake/*.lua")

add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

option("py_bin_dir")
option("py_include_dir")
option("py_lib_dir")

target("pltxt2htm", function ()
    set_kind("shared")
    set_prefixname("")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")
    set_exceptions("no-cxx")
    if is_plat("windows", "mingw") then
        add_links("ntdll")
    end

    local py_bin_dir = nil
    on_config(function (target)
        local is_valid_py_lib_dir = function(dir)
            for  _, a_file in ipairs(os.files(dir .. "/libpython*.so")) do
                return true
            end
            return false
        end

        local check_is_valid_dir = function(dir)
            if not dir then
                return
            end
            if not os.exists(dir) then
                print("error: " .. dir .. " not found")
                os.exit(1)
            end
            if not os.isdir(dir) then
                print("error: " .. dir .. " is not a directory")
                os.exit(1)
            end
        end

        local get_sys_usr_dir = function ()
            assert(is_plat("linux"), "the current platform is not linux")

            local current_env = os.getenvs()
            if current_env["TERMUX__PREFIX"] ~= nil then
                -- usr dir in termux
                return current_env["TERMUX__PREFIX"]
            else
                -- usr dir in normal linux distribution
                return "/usr"
            end
        end

        local get_py_install_dir = function (py_bin_dir)
            if is_plat("windows", "mingw") then
                if path.basename(py_bin_dir) == "Scripts" then
                    -- found python in virtual environment on windows
                    return path.directory(py_bin_dir)
                else
                    return py_bin_dir
                end
            elseif is_plat("linux") then
                local sys_usr_dir = get_sys_usr_dir()

                if py_bin_dir == sys_usr_dir .. "/bin" or py_bin_dir == sys_usr_dir .. "/sbin" then
                    return py_bin_dir
                else
                    return path.directory(py_bin_dir)
                end
            end
        end

        function parse_pyvenv_cfg(file_path)
            local file = io.open(file_path, "r")
            if not file then
                return nil
            end

            local lines = {}
            for line in file:lines() do
                table.insert(lines, line)
            end
            file:close()

            local config = {}
            for _, line in ipairs(lines) do
                line = line:match("^%s*(.-)%s*$")
                if line ~= "" and not line:match("^#") then
                    local key, value = line:match("^(%S+)%s*=%s*(.*)$")
                    if key and value then
                        config[key] = value
                    end
                end
            end

            return config
        end

        local detect_py_include_and_lib_dir = function(py_install_dir)
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

                    if not is_valid_py_lib_dir(py_lib_dir) then
                        py_lib_dir = sys_usr_dir .. "/lib/x86_64-linux-gnu"
                    end
                else
                    -- detect python in another install path
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
                if not is_valid_py_lib_dir(py_lib_dir) then
                    print("error: libpython3 not found in " .. py_lib_dir)
                    os.exit(1)
                end
            else
                print("unreachable code touched, please bug report")
                os.exit(1)
            end
            print("detecting for python lib dir .. " .. py_lib_dir)

            return py_include_dir, py_lib_dir
        end

        local toolchains = target:tool("cxx")
        if path.basename(toolchains) == "clang++"
                or path.basename(toolchains) == "clang"
                or path.basename(toolchains) == "gcc"
                or path.basename(toolchains) == "g++" then
            target:add("cxxflags", "-fno-rtti")
            target:add("cxxflags", "-fno-unwind-tables")
            target:add("cxxflags", "-fno-asynchronous-unwind-tables")
            target:add("cxxflags", "-fvisibility=hidden")
            target:add("cxxflags", "-fvisibility-inlines-hidden")
            if is_mode("release") then
                target:add("cxxflags", "-fno-ident")
            end
        end
        if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
            target:add("shflags", "-fuse-ld=lld")
            target:add("shflags", "-flto")
        end

        -- if python install dir have been passed in console
        py_bin_dir = get_config("py_bin_dir")
        local py_include_dir = get_config("py_include_dir")
        local py_lib_dir = get_config("py_lib_dir")

        check_is_valid_dir(py_bin_dir)
        check_is_valid_dir(py_include_dir)
        check_is_valid_dir(py_lib_dir)

        if not py_bin_dir then
            import("lib.detect.find_tool")
            py_bin_dir = path.directory(find_tool("python").program)
            if not py_bin_dir then
                py_bin_dir = path.directory(find_tool("python3").program)
            end

            if not py_bin_dir then
                print("error: python or python3 not found")
                os.exit(1)
            else
                print("detecting for python bin dir .. " .. py_bin_dir)
            end
        end

        local py_install_dir = get_py_install_dir(py_bin_dir)
        if os.exists(py_install_dir .. "/pyvenv.cfg") and os.isfile(py_install_dir .. "/pyvenv.cfg") then
            -- if detect python in venv, rectify the install dir
            local real_py_install_dir = parse_pyvenv_cfg(py_install_dir .. "/pyvenv.cfg")["home"]
            if real_py_install_dir then
                print("Found python in venv, rectify python install dir to " .. real_py_install_dir)
                py_install_dir = get_py_install_dir(real_py_install_dir)
            end
        end

        local py_include_dir, py_lib_dir = detect_py_include_and_lib_dir(py_install_dir)

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
        local lib_plat_cpy_dir = nil
        if is_plat("windows", "mingw") then
            lib_plat_cpy_dir = os.iorunv(py_bin_dir .. "\\python.exe", {"after_build.py"})
        elseif is_plat("linux") then
            lib_plat_cpy_dir = os.iorunv(py_bin_dir .. "/python3", {"after_build.py"})
        else
            print("unreachable code touched, please bug report")
            os.exit(1)
        end
        os.cp(target:targetfile(), lib_plat_cpy_dir)
        print("copying " .. target:targetfile() .. " to " .. lib_plat_cpy_dir)
    end) -- after_build
end) -- target
