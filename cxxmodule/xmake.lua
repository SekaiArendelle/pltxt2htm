-- build example.cc

includes("../xmake/*.lua")

add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_encodings("utf-8")

target("example", function ()
    set_kind("binary")
    add_includedirs("$(projectdir)/../include")
    add_includedirs("$(projectdir)/../include/fast_io")
    add_files("example.cc")
    add_files("$(projectdir)/pltxt2htm/pltxt2htm.cppm")
    -- add_files("$(projectdir)/fast_io/fast_io.cppm")
    if is_plat("windows", "mingw") then
        add_links("ntdll")
    end
    on_config(function (target)
        local toolchains = target:tool("cxx")
        if path.basename(toolchains) == "clang++" or path.basename(toolchains) == "clang" then
            target:add("ldflags", "-fuse-ld=lld")
        end
    end)
end)
