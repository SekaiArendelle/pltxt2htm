set_allowedmodes("debug", "release")
add_rules("mode.debug", "mode.release")

includes("../xmake/*.lua")
set_languages("c++23")
set_encodings("utf-8")

add_files("pltxt2htm.cc")
add_includedirs("$(projectdir)/../include")

if is_plat("windows") or is_plat("mingw") then
    add_syslinks("ntdll")
end

local my_on_config = function(target)
    if is_mode("debug") then
        set_symbols("debug")
    end

    local toolchains = target:tool("cxx")
    if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
        target:add("shflags", "-fuse-ld=lld")
        if is_mode("release") then
            target:add("ldflags", "-flto")
        end
    end

    if path.basename(toolchains) == "clang++"
            or path.basename(toolchains) == "clang"
            or path.basename(toolchains) == "gcc"
            or path.basename(toolchains) == "g++" then
        target:add("cxxflags", "-fno-exceptions")
        target:add("cxxflags", "-fno-cxx-exceptions")
        target:add("cxxflags", "-fno-rtti")
        target:add("cxxflags", "-fno-unwind-tables")
        target:add("cxxflags", "-fno-asynchronous-unwind-tables")
        target:add("cxxflags", "-fvisibility=hidden")
        target:add("cxxflags", "-fvisibility-inlines-hidden")
        if is_mode("release") then
            target:add("cxxflags", "-fno-ident")
        end
    end
end

target("pltxt2htm_shared", function ()
    set_kind("shared")

    on_config(function (target)
        my_on_config(target)
    end)
end)

target("pltxt2htm_static", function ()
    set_kind("static")

    on_config(function (target)
        my_on_config(target)
    end)
end)
