set_policy("check.auto_ignore_flags", false)

includes("../xmake/*.lua")

add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

target("pltxt2htm", function()
    set_kind("binary")
    add_files("pltxt2htm.cc")
    add_includedirs("$(projectdir)/../include")

    if is_plat("windows", "mingw") then
        add_syslinks("ntdll")
    end

    if is_plat("wasm") then
        set_extension(".wasm")
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

        if is_mode("release") and (path.basename(toolchains) == "clang++"
                or path.basename(toolchains) == "clang"
                or path.basename(toolchains) == "gcc"
                or path.basename(toolchains) == "g++") then
            target:add("cxxflags", "-fno-rtti")
            target:add("cxxflags", "-fno-unwind-tables")
            target:add("cxxflags", "-fno-asynchronous-unwind-tables")
            target:add("cxxflags", "-fno-ident")
        end
    end)

    before_build(function (target)
        git_commit_hash = os.iorunv("git", {"rev-parse", "HEAD"})
        local file = io.open("$(projectdir)/commit_hash.ignore", "w")
        if file then
            file:write("\"* build commit: ", git_commit_hash:sub(1, -2), "\\n\"")
            file:close()
        end
    end)

    after_build(function (target)
        if os.exists("$(projectdir)/commit_hash.ignore")
                and os.isfile("$(projectdir)/commit_hash.ignore") then
            os.rm("$(projectdir)/commit_hash.ignore")
        end
    end)

    on_install(function (target)
        import("utility.utility", {rootdir = target:scriptdir() .. "/../xmake"})
        local toolchain = get_config("toolchain")
        local triplet = nil
        local modifier = nil
        -- On Windows, the default toolchain is MSVC, which is not supported in this project
        -- On Linux, the default toolchain is GCC
        -- On MacOS, the default toolchain is AppleClang, which is also not supported in the project
        -- For other OS, I don't care.
        -- But which toolchain is the default is not important
        -- Following code just to get the build triplet
        if toolchain == nil or toolchain:endswith("clang") or toolchain == "clang-cl" then
            if toolchain == nil or toolchain == "clang" or toolchain == "clang-cl" then
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
    end)
end)
