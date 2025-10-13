set_allowedmodes("debug", "coverage")
add_rules("mode.debug", "mode.coverage")
set_defaultmode("debug")

includes("../xmake/*.lua")
set_languages("c++23")
set_encodings("utf-8")
add_includedirs("$(projectdir)/../include")

target("precompile", function()
    set_kind("static")
    add_files("precompile.cpp")

    on_config(function (target)
        local compiler = path.basename(target:tool("cxx"))
        if compiler:endswith("gcc")
                or compiler:endswith("g++")
                or compiler:endswith("clang++")
                or compiler:endswith("clang") then
            target:add("cxxflags", "-g")
            target:add("cxxflags", "-Wall")
            target:add("cxxflags", "-Wextra")
            target:add("cxxflags", "-Wshadow")
            target:add("cxxflags", "-Werror=return-type")
            target:add("cxxflags", "-Werror=switch")
            target:add("cxxflags", "-Werror=implicit-fallthrough")
        end
    end)
end)

for _, file in ipairs(os.files("*.cc")) do
    local name = path.basename(file)
    target(name, function ()
        set_kind("binary")
        set_exceptions("cxx")
        set_default(false)
        add_files(name .. ".cc")
        add_deps("precompile")
        add_tests("default")
        if is_plat("windows") or is_plat("mingw") then
            add_syslinks("ntdll")
        end

        on_config(function (target)
            local compiler = path.basename(target:tool("cxx"))
            if compiler:endswith("gcc")
                    or compiler:endswith("g++")
                    or compiler:endswith("clang++")
                    or compiler:endswith("clang") then
                target:add("cxxflags", "-g")
            end

            if compiler == "clang++" or compiler == "clang" then
                if is_mode("coverage") then
                    print("error: lcov only support gcc, please reconfig with `--toolchain=gcc --mode=coverage`")
                    os.exit(1)
                end
                target:add("ldflags", "-fuse-ld=lld")
            end
        end)
    end)
end
