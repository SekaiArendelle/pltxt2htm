set_allowedmodes("debug", "coverage")
add_rules("mode.debug", "mode.coverage")
set_defaultmode("debug")

includes("../xmake/*.lua")
set_languages("c++23")
set_encodings("utf-8")

for _, file in ipairs(os.files("*.cc")) do
    local name = path.basename(file)
    target(name, function ()
        set_symbols("debug")
        set_kind("binary")
        set_default(false)
        add_files(name .. ".cc")
        set_warnings("all", "extra")
        add_tests("default")
        add_includedirs("../include")
        add_cxxflags("-Werror=return-type", {tool = {"gcc", "clang"}})
        add_cxxflags("-Werror=switch", {tool = {"gcc", "clang"}})
        add_cxxflags("-Werror=implicit-fallthrough", {tool = {"gcc", "clang"}})
        add_ldflags("-fuse-ld=lld", {force = true, tool = "clang"})
        if is_plat("windows") or is_plat("mingw") then
            add_syslinks("ntdll")
        end
    end)
end
