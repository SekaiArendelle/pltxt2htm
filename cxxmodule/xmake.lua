-- build example.cc
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

target("example", function()
    set_kind("binary")
    set_policy("build.c++.modules.std", false)
    add_includedirs("$(projectdir)/../include")
    add_includedirs("$(projectdir)/../include/fast_io")
    add_files("example.cc")
    add_files("$(projectdir)/pltxt2htm/pltxt2htm.cppm")
    add_files("$(projectdir)/fast_io/fast_io.cppm")
    if is_plat("windows", "mingw") then add_links("ntdll") end
    on_config(function(target)
        local toolchains = target:tool("cxx")
        if path.basename(toolchains) == "clang++" or path.basename(toolchains) ==
            "clang" then target:add("ldflags", "-fuse-ld=lld") end
    end)
end)
