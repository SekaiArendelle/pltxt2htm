includes("../xmake/*.lua")

add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

target("pltxt2htm", function()
    set_kind("binary")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")

    if is_plat("windows") or is_plat("mingw") then
        add_syslinks("ntdll")
    end

    if is_mode("release") then
        set_exceptions("no-cxx")
    elseif is_mode("debug") then
        set_warnings("all")
        set_warnings("extra")
    end

    on_config(function (target)
        local toolchains = target:tool("cxx")
        if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
            target:add("ldflags", "-fuse-ld=lld")
            if is_mode("release") then
                target:add("ldflags", "-flto")
            end
        end

        if is_mode("release") and path.basename(toolchains) == "clang++"
                or path.basename(toolchains) == "clang"
                or path.basename(toolchains) == "gcc"
                or path.basename(toolchains) == "g++" then
            target:add("cxxflags", "-fno-rtti")
            target:add("cxxflags", "-fno-unwind-tables")
            target:add("cxxflags", "-fno-asynchronous-unwind-tables")
            target:add("cxxflags", "-fno-ident")
        end
    end)

    on_install(function (target)
        import("utility.utility", {rootdir = target:scriptdir() .. "/../xmake"})
        local toolchain = get_config("toolchain")
        local triplet = nil
        local modifier = nil
        if toolchain:endswith("clang") then
            if toolchain == "clang" then
                triplet, modifier = utility.get_target_modifier("target", "clang")
            else
                triplet, modifier = utility.get_target_modifier(string.sub(toolchain, 0, -7), "clang")
            end
        elseif toolchain:endswith("gcc") then
            if toolchain == "gcc" then
                triplet, modifier = utility.get_target_modifier("target", "gcc")
            else
                triplet, modifier = utility.get_target_modifier(string.sub(toolchain, 0, -5), "gcc")
            end
        else
            print("unknown toolchain: " .. toolchain)
            os.exit(1)
        end
        local install_dir = target:installdir()
        local infer_out_dir = false
        if not install_dir or install_dir == "auto" then
            infer_out_dir = true
        end
        if infer_out_dir then
            install_dir = triplet .. "-" .. "pltxt2htm-cmd-" .. get_config("mode")
        end
        if not os.exists(install_dir) or not os.isdir(install_dir) then
            os.mkdir(install_dir)
        end
        os.cp(target:targetfile(), install_dir)
        print("install to " .. path.join(target:scriptdir(), install_dir))
        if infer_out_dir then
            import("utils.archive")
            archive.archive(install_dir .. ".zip", install_dir)
            print("archive to \"" .. install_dir .. ".zip\"")
        end
    end)
end)
