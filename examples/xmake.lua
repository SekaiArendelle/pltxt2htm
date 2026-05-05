includes("../xmake/*.lua")

set_allowedmodes("debug", "release")
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
set_languages("c++23")
set_encodings("utf-8")
add_includedirs("$(projectdir)/../include")

for _, file in ipairs(os.files("*.cc")) do
    local name = path.basename(file)
    target(name, function()
        set_kind("binary")
        add_files(file)

        if is_plat("windows") or is_plat("mingw") then add_syslinks("ntdll") end

        on_config(function(target)
            local compiler = path.basename(target:tool("cxx"))
            local linker = path.basename(target:tool("ld"))

            import("lib.detect.find_tool")
            if find_tool("ld.lld") and
                (linker == "clang" or linker == "clang++" or linker == "gcc" or linker == "g++") then
                target:add("ldflags", "-fuse-ld=lld")
            end

            if compiler == "cl" then target:add("cxxflags", "/Zc:u8EscapeEncoding") end
        end)
    end)
end
