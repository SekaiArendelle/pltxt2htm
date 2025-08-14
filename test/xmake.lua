set_allowedmodes("debug", "coverage")
add_rules("mode.debug", "mode.coverage")
set_defaultmode("debug")

includes("../xmake/*.lua")
set_languages("c++23")
set_encodings("utf-8")
add_includedirs("$(projectdir)/../include")

-- requires g++ >= 14
-- requires clang++ >= 20

target("precompile", function()
    set_kind("static")
    set_symbols("debug")
    add_files("precompile.cpp")
    set_warnings("all", "extra")
    add_cxxflags("-Werror=return-type", {tool = {"gcc", "clang"}})
    add_cxxflags("-Werror=switch", {tool = {"gcc", "clang"}})
    add_cxxflags("-Werror=implicit-fallthrough", {tool = {"gcc", "clang"}})
    add_cxxflags("-Wshadow", {tool = {"gcc", "clang"}})
end)

for _, file in ipairs(os.files("*.cc")) do
    local name = path.basename(file)
    target(name, function ()
        set_symbols("debug")
        set_kind("binary")
        set_exceptions("cxx")
        set_default(false)
        add_files(name .. ".cc")
        add_deps("precompile")
        add_tests("default")
        if os.host() == "windows" then
            add_syslinks("ntdll")
        end

        on_config(function (target)
            local toolchains = target:tool("cxx")
            if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
                if is_mode("coverage") then
                    print("error: lcov only support gcc, please reconfig with `--toolchain=gcc --mode=coverage`")
                    os.exit(1)
                end
                target:add("ldflags", "-fuse-ld=lld")
            end
        end)
    end)
end
